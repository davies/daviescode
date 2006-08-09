#include <memory.h>
#include "demux.h"

const int mpeg1_skip_table[16] = {0, 0, 4, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int 
Demux::decode(uint8_t * buf, int length)
{
  uint8_t *end = buf + length;
  uint8_t *es_start  = buf;
  int es_length = 0;

    /*
     * the demuxer keeps some state between calls:
     * if "state" = DEMUX_HEADER, then "head_buf" contains the first
     *     "bytes" bytes from some header.
     * if "state" == DEMUX_DATA, then we need to copy "bytes" bytes
     *     of ES data before the next header.
     * if "state" == DEMUX_SKIP, then we need to skip "bytes" bytes
     *     of data before the next header.
     *
     * NEEDBYTES makes sure we have the requested number of bytes for a
     * header. If we dont, it copies what we have into head_buf and returns,
     * so that when we come back with more data we finish decoding this header.
     *
     * DONEBYTES updates "buf" to point after the header we just parsed.
     */

    uint8_t * header;
    int bytes;
    int len;

#define NEEDBYTES(x)                                            \
    {                                                           \
        int missing;                                            \
                                                                \
        missing = (x) - bytes;                                  \
        if (missing > 0) {                                      \
            if (header == head_buf) {                           \
                if (missing <= end - buf) {                     \
                    memcpy (header + bytes, buf, missing);      \
                    buf += missing;                             \
                    bytes = (x);                                \
                } else {                                        \
                    memcpy (header + bytes, buf, end - buf);    \
                    state_bytes = bytes + end - buf;            \
                    return es_length;                           \
                }                                               \
            } else {                                            \
                memcpy (head_buf, header, bytes);               \
                state = DEMUX_HEADER;                           \
                state_bytes = bytes;                            \
                return es_length;                               \
            }                                                   \
        }                                                       \
    }

#define DONEBYTES(x)            \
    {                           \
        if (header != head_buf) \
            buf = header + (x); \
    }

#define MOVE(begin, len)                    \
    {                                       \
        if (es_start == (begin))            \
          es_start += (len);                \
        else                                \
          for (int i = 0; i < (len); i++)   \
              *es_start++ = (begin)[i];     \
        es_length += (len);                 \
    }                                     

    
    switch (state) {
    case DEMUX_HEADER:
        if (state_bytes > 0) {
            header = head_buf;
            bytes = state_bytes;
            goto continue_header;
        }
        break;
    case DEMUX_DATA:
        if (state_bytes > end - buf) {
            MOVE(buf, end - buf);
            state_bytes -= end - buf;
            return es_length;
        }
        MOVE(buf, state_bytes);
        buf += state_bytes;
        break;
    case DEMUX_SKIP:
        if (state_bytes > end - buf) {
            state_bytes -= end - buf;
            return es_length;
        }
        buf += state_bytes;
        break;
    }

    while (1) {
        header = buf;
        bytes = end - buf;
    continue_header:
        NEEDBYTES (4);
        if (header[0] || header[1] || (header[2] != 1)) {
            if (header != head_buf) {
                buf++;
                continue;
            } else {
                header[0] = header[1];
                header[1] = header[2];
                header[2] = header[3];
                bytes = 3;
                goto continue_header;
            }
        }
        switch (header[3]) {
        case 0xb9:      /* program end code */
            /* DONEBYTES (4); */
            /* break;         */
            return es_length;
        case 0xba:      /* pack header */
            NEEDBYTES (12);
            if ((header[4] & 0xc0) == 0x40) {   /* mpeg2 */
                NEEDBYTES (14);
                len = 14 + (header[13] & 7);
                NEEDBYTES (len);
                DONEBYTES (len);
                /* header points to the mpeg2 pack header */
            } else if ((header[4] & 0xf0) == 0x20) {    /* mpeg1 */
                DONEBYTES (12);
                /* header points to the mpeg1 pack header */
            } else {
                errors++;   //weird pack header
                buf++;
            }
            break;
        case 0xbd:      /* private stream 1 */
            NEEDBYTES (7);
            if ((header[6] & 0xc0) == 0x80) {   /* mpeg2 */
                NEEDBYTES (9);
                len = 10 + header[8];
                NEEDBYTES (len);
                /* header points to the mpeg2 pes header */
            } else {    /* mpeg1 */
                len = 7;
                while ((header-1)[len] == 0xff) {
                    len++;
                    NEEDBYTES (len);
                    if (len == 23) {
                        errors++;  // too much stuffing
                        break;
                    }
                }
                if (((header-1)[len] & 0xc0) == 0x40) {
                    len += 2;
                    NEEDBYTES (len);
                }
                len += mpeg1_skip_table[(header - 1)[len] >> 4] + 1;
                NEEDBYTES (len);
                /* header points to the mpeg1 pes header */
            }
            len += 3;
            NEEDBYTES (len);
            DONEBYTES (len);
            bytes = 6 + (header[4] << 8) + header[5] - len;
            if (bytes > end - buf) {
                MOVE(buf, end - buf);
                state = DEMUX_DATA;
                state_bytes = bytes - (end - buf);
                return es_length;
            } else if (bytes <= 0)
                continue;
/*    {                                       
      uint8_t *begin = buf;
      int len = bytes;
        if (es_start == (begin))            
          es_start += (len);                
        else                                
          for (int i = 0; i < (len); i++)   
              *es_start++ = (begin)[i];     
        es_length += (len);                 
    }                                    */
            MOVE(buf, bytes);
            buf += bytes;
            break;
        default:
            if (header[3] < 0xb9) {
                errors++;  // looks like a video stream, not system stream
                buf++;
                continue;
            } else {
                NEEDBYTES (6);
                DONEBYTES (6);
                bytes = (header[4] << 8) + header[5];
                if (bytes > end - buf) {
                    state = DEMUX_SKIP;
                    state_bytes = bytes - (end - buf);
                    return es_length;
                }
                buf += bytes;
            }
        }
    }
}
