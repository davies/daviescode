#ifndef __DEF_H__
#define __DEF_H__

#define MPEG_PROGRAM_END_CODE    0x000001B9
#define PACK_START_CODE          0x000001BA
#define SYSTEM_HEADER_START_CODE 0x000001BB
#define PACKET_START_CODE_PREFIX 0x000001

#define PROGRAM_ASSOCIATION_TABLE 0x0000
#define CONDITIONAL_ACCESS_TABLE  0x0001
#define NULL_PACKET               0x1FFF

#define TRANSPORT_SYNC_BYTE      0x47
#define PROGRAM_STREAM_MAP       0xBC
#define PRIVATE_STREAM_1         0xBD
#define PADDING_STREAM           0xBE
#define PRIVATE_STREAM_2         0xBF
#define ECM_STREAM               0xF0
#define EMM_STREAM               0xF1
#define PROGRAM_STREAM_DIRECTORY 0xFF
#define DSMCC_STREAM             0xF2
#define ITUTRECH222TYPEE_STREAM  0xF8
#define VIDEO_STREAM_0           0xE0
#define VIDEO_STREAM_F           0xEF
#define AUDIO_STREAM_0           0xC0
#define AUDIO_STREAM_1F          0xDF
#define SUBSTREAM_AC3_0          0x80
#define SUBSTREAM_AC3_8          0x87
#define SUBSTREAM_DTS_0          0x88
#define SUBSTREAM_DTS_8          0x8F
#define SUBSTREAM_PCM_0          0xA0
#define SUBSTREAM_PCM_F          0xAF
#define SUBSTREAM_SUBPIC_0       0x20
#define SUBSTREAM_SUBPIC_1F      0x3F

#define MAX_AUDIO_STREAMS ((AUDIO_STREAM_1F-AUDIO_STREAM_0+1)+(SUBSTREAM_AC3_8-SUBSTREAM_AC3_0+1))
#define NOT_A_AUDIOSTREAM			0
#define AUDIOSTREAM_TYPE_MPA		1
#define AUDIOSTREAM_TYPE_AC3		2
#define AUDIOSTREAM_TYPE_UNKNOWN    3

#define START_CODE				 0x000001
#define PICTURE_START_CODE       0x00000100
#define PICTURE_CODING_EXTENSION 0x00000104
#define USER_DATA_START_CODE     0x000001B2
#define SEQUENCE_HEADER_CODE     0x000001B3
#define EXTENSION_START_CODE     0x000001B5
#define SEQUENCE_END_CODE        0x000001B7
#define GROUP_START_CODE         0x000001B8

/* extension start code IDs */
#define SEQUENCE_EXTENSION_ID                    1
#define SEQUENCE_DISPLAY_EXTENSION_ID            2
#define QUANT_MATRIX_EXTENSION_ID                3
#define COPYRIGHT_EXTENSION_ID                   4
#define SEQUENCE_SCALABLE_EXTENSION_ID           5
#define PICTURE_DISPLAY_EXTENSION_ID             7
#define PICTURE_CODING_EXTENSION_ID              8
#define PICTURE_SPATIAL_SCALABLE_EXTENSION_ID    9
#define PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID  10

#define FAST_FORWARD 0
#define SLOW_MOTION  1
#define FREEZE_FRAME 2
#define FAST_REVERSE 3
#define SLOW_REVERSE 4

#define PACK_SIZE			2048
#define MAX_PES_PACKET_SIZE 2048*2
#define MAX_PES_COUNT       5

#define MPEG1				1
#define MPEG2				2

#define MAX_SEQUENCE_COUNT  8000

typedef char			int8;
typedef unsigned char	uint8;
typedef short			int16;
typedef unsigned short	uint16;
typedef int				int32;
typedef unsigned int	uint32;
typedef long			int64;
typedef unsigned long	uint64;

#endif

