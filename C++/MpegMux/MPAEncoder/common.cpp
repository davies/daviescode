/***********************************************************************
*
*  Global Include Files
*
***********************************************************************/

#include    "common.h"
#include	"alloc_tables.h"
#include	<ctype.h>

/***********************************************************************
*
*  Global Variable Definitions
*
***********************************************************************/

/* 1: MPEG-1, 0: MPEG-2 LSF, 1995-07-11 shn */
double  s_freq[2][4] = {{22.05, 24, 16, 0}, {44.1, 48, 32, 0}};

/* 1: MPEG-1, 0: MPEG-2 LSF, 1995-07-11 shn */
int     bitrate[2][3][15] = {
	{{0,32,48,56,64,80,96,112,128,144,160,176,192,224,256},
	{0,8,16,24,32,40,48,56,64,80,96,112,128,144,160},
	{0,8,16,24,32,40,48,56,64,80,96,112,128,144,160}},
	{{0,32,64,96,128,160,192,224,256,288,320,352,384,416,448},
	{0,32,48,56,64,80,96,112,128,160,192,224,256,320,384},
	{0,32,40,48,56,64,80,96,112,128,160,192,224,256,320}}
};

double FAR multiple[64] = {
	2.00000000000000, 1.58740105196820, 1.25992104989487,
		1.00000000000000, 0.79370052598410, 0.62996052494744, 0.50000000000000,
		0.39685026299205, 0.31498026247372, 0.25000000000000, 0.19842513149602,
		0.15749013123686, 0.12500000000000, 0.09921256574801, 0.07874506561843,
		0.06250000000000, 0.04960628287401, 0.03937253280921, 0.03125000000000,
		0.02480314143700, 0.01968626640461, 0.01562500000000, 0.01240157071850,
		0.00984313320230, 0.00781250000000, 0.00620078535925, 0.00492156660115,
		0.00390625000000, 0.00310039267963, 0.00246078330058, 0.00195312500000,
		0.00155019633981, 0.00123039165029, 0.00097656250000, 0.00077509816991,
		0.00061519582514, 0.00048828125000, 0.00038754908495, 0.00030759791257,
		0.00024414062500, 0.00019377454248, 0.00015379895629, 0.00012207031250,
		0.00009688727124, 0.00007689947814, 0.00006103515625, 0.00004844363562,
		0.00003844973907, 0.00003051757813, 0.00002422181781, 0.00001922486954,
		0.00001525878906, 0.00001211090890, 0.00000961243477, 0.00000762939453,
		0.00000605545445, 0.00000480621738, 0.00000381469727, 0.00000302772723,
		0.00000240310869, 0.00000190734863, 0.00000151386361, 0.00000120155435,
		1E-20
};

enum byte_order NativeByteOrder = order_unknown;

/***********************************************************************
*
*  Global Function Definitions
*
***********************************************************************/

/***********************************************************************
*
* Using the decoded info the appropriate possible quantization per
* subband table is loaded
*
**********************************************************************/

int pick_table(frame_params *fr_ps)   /* choose table, load if necess, return # sb's */
{
	static int translate[3][2][16] ={ 
		{ { 0,2,2,2,2,2,2,0,0,0,1,1,1,1,1,0 } ,
		{   0,2,2,0,0,0,1,1,1,1,1,1,1,1,1,0 } } ,
		{ { 0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0 } ,
		{   0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0 } } ,
		{ { 0,3,3,3,3,3,3,0,0,0,1,1,1,1,1,0 } ,
		{   0,3,3,0,0,0,1,1,1,1,1,1,1,1,1,0 } } 
	};
	static al_table *tables[5] = { &alloc_0, &alloc_1, &alloc_2, &alloc_3 , &alloc_4 };
	static int sblims[5] = { 27 , 30 , 8, 12 , 30 };
	
    int table = translate[fr_ps->header->sampling_frequency][2-fr_ps->stereo][fr_ps->header->bitrate_index];
	fr_ps->alloc = tables[table];
	
	return sblims[table];
}

void hdr_to_frps(frame_params *fr_ps) /* interpret data in hdr str to fields in fr_ps */
{
	layer *hdr = fr_ps->header;     /* (or pass in as arg?) */
	
    fr_ps->actual_mode = hdr->mode;
    fr_ps->stereo = (hdr->mode == MPG_MD_MONO) ? 1 : 2;
    if (hdr->lay == 2)
		fr_ps->sblimit = pick_table(fr_ps);     /* alloc, tab_num set in pick_table */
    else
		fr_ps->sblimit = SBLIMIT;
    if(hdr->mode == MPG_MD_JOINT_STEREO)
        fr_ps->jsbound = (hdr->mode_ext<<2)+4;
    else
        fr_ps->jsbound = fr_ps->sblimit;
}

/*******************************************************************************
*
*  Allocate number of bytes of memory equal to "block".
*
*******************************************************************************/

void  FAR *mem_alloc(unsigned long   block, char            *item)
{
    void    *ptr;
    ptr = (void FAR *) malloc((unsigned int)block); /* far memory, 93-08-24 ss */
    if (ptr != NULL){
		memset(ptr, 0, block);
    }else{
        printf("Unable to allocate %s\n", item);
        exit(0);
    }
    return(ptr);
}


/****************************************************************************
*
*  Free memory pointed to by "*ptr_addr".
*
*****************************************************************************/

void    mem_free(void    **ptr_addr)
{
    if (*ptr_addr != NULL){
        free(*ptr_addr);
       *ptr_addr = NULL;
    }
}

/*****************************************************************************
*
*  Routines to determine byte order and swap bytes
*
*****************************************************************************/

enum byte_order DetermineByteOrder()
{
    char s[ sizeof(long) + 1 ];
    union
    {
        long longval;
        char charval[ sizeof(long) ];
    } probe;
    probe.longval = 0x41424344L;  /* ABCD in ASCII */
    strncpy( s, probe.charval, sizeof(long) );
    s[ sizeof(long) ] = '\0';
    /* fprintf( stderr, "byte order is %s\n", s ); */
    if ( strcmp(s, "ABCD") == 0 )
        return order_bigEndian;
    else if ( strcmp(s, "DCBA") == 0 )
        return order_littleEndian;
    else
       return order_unknown;
}

void SwapBytesInWords( short *loc, int words )
{
    int i;
    short thisval;
    char *dst, *src;
    src = (char *) &thisval;
    for ( i = 0; i < words; i++ )
    {
        thisval = *loc;
        dst = (char *) loc++;
        dst[0] = src[1];
        dst[1] = src[0];
    }
}


/*****************************************************************************
*
*  bit_stream.c package
*  Author:  Jean-Georges Fritsch, C-Cube Microsystems
*
*****************************************************************************/

/********************************************************************
This package provides functions to write (exclusive or read)
information from (exclusive or to) the bit stream.

 If the bit stream is opened in read mode only the get functions are
 available. If the bit stream is opened in write mode only the put
 functions are available.
********************************************************************/

/*open_bit_stream_w(); open the device to write the bit stream into it    */
/*open_bit_stream_r(); open the device to read the bit stream from it     */
/*close_bit_stream();  close the device containing the bit stream         */
/*alloc_buffer();      open and initialize the buffer;                    */
/*desalloc_buffer();   empty and close the buffer                         */
/*back_track_buffer();     goes back N bits in the buffer                 */
/*unsigned int get1bit();  read 1 bit from the bit stream                 */
/*unsigned long getbits(); read N bits from the bit stream                */
/*                                    the bit stream                        */
/*unsigned long look_ahead(); grep the next N bits in the bit stream without*/
/*                            changing the buffer pointer                   */
/*put1bit(); write 1 bit from the bit stream  */
/*put1bit(); write 1 bit from the bit stream  */
/*putbits(); write N bits from the bit stream */
/*unsigned long sstell(); return the current bit stream length (in bits)    */
/*int end_bs(); return 1 if the end of bit stream reached otherwise 0       */
/*int seek_sync(); return 1 if a sync word was found in the bit stream      */
/*                 otherwise returns 0                                      */

/* refill the buffer from the input device when the buffer becomes empty    */
int refill_buffer(Bit_stream_struc *bs)
/* bit stream structure */
{
	register int i=bs->buf_size-2-bs->buf_byte_idx;
	register unsigned long n=1;
	register int index=0;
	char val[2];
	
	while ((i>=0) && (!bs->eob)) {
		
		if (bs->format == BINARY)
			n = fread(&bs->buf[i--], sizeof(unsigned char), 1, bs->pt);
		
		else {
			while((index < 2) && n) {
				n = fread(&val[index], sizeof(char), 1, bs->pt);
				switch (val[index]) {
				case 0x30:
				case 0x31:
				case 0x32:
				case 0x33:
				case 0x34:
				case 0x35:
				case 0x36:
				case 0x37:
				case 0x38:
				case 0x39:
				case 0x41:
				case 0x42:
				case 0x43:
				case 0x44:
				case 0x45:
				case 0x46:
					index++;
					break;
				default: break;
				}
			}
			
			if (val[0] <= 0x39)   bs->buf[i] = (val[0] - 0x30) << 4;
			else  bs->buf[i] = (val[0] - 0x37) << 4;
			if (val[1] <= 0x39)   bs->buf[i--] |= (val[1] - 0x30);
			else  bs->buf[i--] |= (val[1] - 0x37);
			index = 0;
		}
		
		if (!n) {
			bs->eob= i+1;
		}
		
    }
	return 0;
}

static char *he = "0123456789ABCDEF";

/* empty the buffer to the output device when the buffer becomes full */
void empty_buffer(Bit_stream_struc *bs, int minimum)
/* bit stream structure */
/* end of the buffer to empty */
{
	register int i;
	
#if BS_FORMAT == BINARY
	for (i=bs->buf_size-1;i>=minimum;i--)
		fwrite(&bs->buf[i], sizeof(unsigned char), 1, bs->pt);
#else
	for (i=bs->buf_size-1;i>=minimum;i--) {
		char val[2];
		val[0] = he[((bs->buf[i] >> 4) & 0x0F)];
		val[1] = he[(bs->buf[i] & 0x0F)];
		fwrite(val, sizeof(char), 2, bs->pt);
	}
#endif
	fflush(bs->pt); /* NEW SS to assist in debugging*/
	
	for (i=minimum-1; i>=0; i--)
		bs->buf[bs->buf_size - minimum + i] = bs->buf[i];
	
	bs->buf_byte_idx = bs->buf_size -1 - minimum;
	bs->buf_bit_idx = 8;
}

/* open the device to write the bit stream into it */
void open_bit_stream_w(Bit_stream_struc *bs, char *bs_filenam, int size)
{
	if ((bs->pt = fopen(bs_filenam, "wb")) == NULL) {
		printf("Could not create \"%s\".\n", bs_filenam);
		exit(1);
	}
	alloc_buffer(bs, size);
	bs->buf_byte_idx = size-1;
	bs->buf_bit_idx=8;
	bs->totbit=0;
	bs->mode = WRITE_MODE;
	bs->eob = FALSE;
	bs->eobs = FALSE;
}

/* open the device to read the bit stream from it */
void open_bit_stream_r(Bit_stream_struc *bs, char * bs_filenam, int size)
/* bit stream structure */
/* name of the bit stream file */
/* size of the buffer */
{
	register unsigned long n;
	register unsigned char flag = 1;
	unsigned char val;
	
	if ((bs->pt = fopen(bs_filenam, "rb")) == NULL) {
		printf("Could not find \"%s\".\n", bs_filenam);
		exit(1);
	}
	
	do {
		n = fread(&val, sizeof(unsigned char), 1, bs->pt);
		switch (val) {
		case 0x30:
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
		case 0x35:
		case 0x36:
		case 0x37:
		case 0x38:
		case 0x39:
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x44:
		case 0x45:
		case 0x46:
		case 0xa:  /* \n */
		case 0xd:  /* cr */
		case 0x1a:  /* sub */
			break;
			
		default: /* detection of an binary character */
			flag--;
			break;
		}
		
	} while (flag & n);
	
	if (flag) {
		printf ("the bit stream file %s is an ASCII file\n", bs_filenam);
		bs->format = ASCII;
	}
	else {
		bs->format = BINARY;
		printf ("the bit stream file %s is a BINARY file\n", bs_filenam);
	}
	
	fclose(bs->pt);
	
	if ((bs->pt = fopen(bs_filenam, "rb")) == NULL) {
		printf("Could not find \"%s\".\n", bs_filenam);
		exit(1);
	}
	
	alloc_buffer(bs, size);
	bs->buf_byte_idx=0;
	bs->buf_bit_idx=0;
	bs->totbit=0;
	bs->mode = READ_MODE;
	bs->eob = FALSE;
	bs->eobs = FALSE;
}

/*close the device containing the bit stream after a read process*/
void close_bit_stream_r(Bit_stream_struc *bs)
{
	fclose(bs->pt);
	desalloc_buffer(bs);
}

/*close the device containing the bit stream after a write process*/
void close_bit_stream_w(Bit_stream_struc *bs)
/* bit stream structure */
{
	empty_buffer(bs, bs->buf_byte_idx);
	fclose(bs->pt);
	desalloc_buffer(bs);
}

/*open and initialize the buffer; */
void alloc_buffer(Bit_stream_struc *bs, int size)
/* bit stream structure */
{
	bs->buf = (unsigned char FAR *) mem_alloc(size*sizeof(unsigned
		char), "buffer");
	bs->buf_size = size;
}

/*empty and close the buffer */
void desalloc_buffer(Bit_stream_struc * bs)
/* bit stream structure */
{
	free(bs->buf);
}

int putmask[9]={0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff};
int clearmask[9]={0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x0};

void back_track_buffer(Bit_stream_struc *bs,int N) /* goes back N bits in the buffer */
{
	int tmp = N - (N/8)*8;
	register int i;
	
	bs->totbit -= N;
	for (i=bs->buf_byte_idx;i< bs->buf_byte_idx+N/8-1;i++) bs->buf[i] = 0;
	bs->buf_byte_idx += N/8;
	if ( (tmp + bs->buf_bit_idx) <= 8) {
		bs->buf_bit_idx += tmp;
	}
	else {
		bs->buf_byte_idx ++;
		bs->buf_bit_idx += (tmp - 8);
	}
	bs->buf[bs->buf_byte_idx] &= clearmask[bs->buf_bit_idx];
}

int mask[8]={0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};

/*read 1 bit from the bit stream */
unsigned int get1bit(Bit_stream_struc *bs)
/* bit stream structure */
{
	unsigned int bit;
	register int i;
	
	bs->totbit++;
	
	if (!bs->buf_bit_idx) {
        bs->buf_bit_idx = 8;
        bs->buf_byte_idx--;
        if ((bs->buf_byte_idx < MINIMUM) || (bs->buf_byte_idx < bs->eob)) {
			if (bs->eob)
                bs->eobs = TRUE;
			else {
                for (i=bs->buf_byte_idx; i>=0;i--)
					bs->buf[bs->buf_size-1-bs->buf_byte_idx+i] = bs->buf[i];
                refill_buffer(bs);
                bs->buf_byte_idx = bs->buf_size-1;
			}
        }
	}
	bit = bs->buf[bs->buf_byte_idx]&mask[bs->buf_bit_idx-1];
	bit = bit >> (bs->buf_bit_idx-1);
	bs->buf_bit_idx--;
	return(bit);
}

/*write 1 bit from the bit stream */
void put1bit(Bit_stream_struc *bs, int bit)
/* bit stream structure */
/* bit to write into the buffer */
{
	bs->totbit++;
	
	bs->buf[bs->buf_byte_idx] |= (bit&0x1) << (bs->buf_bit_idx-1);
	bs->buf_bit_idx--;
	if (!bs->buf_bit_idx) {
		bs->buf_bit_idx = 8;
		bs->buf_byte_idx--;
		if (bs->buf_byte_idx < 0)
			empty_buffer(bs, MINIMUM);
		bs->buf[bs->buf_byte_idx] = 0;
	}
}

/*look ahead for the next N bits from the bit stream */
unsigned long look_ahead(Bit_stream_struc *bs, int N)
/* bit stream structure */
/* number of bits to read from the bit stream */
{
	unsigned long val=0;
	register int j = N;
	register int k, tmp;
	register int bit_idx = bs->buf_bit_idx;
	register int byte_idx = bs->buf_byte_idx;
	
	if (N > MAX_LENGTH)
		printf("Cannot read or write more than %d bits at a time.\n", MAX_LENGTH);
	
	while (j > 0) {
		if (!bit_idx) {
			bit_idx = 8;
			byte_idx--;
		}
		k = MIN (j, bit_idx);
		tmp = bs->buf[byte_idx]&putmask[bit_idx];
		tmp = tmp >> (bit_idx-k);
		val |= tmp << (j-k);
		bit_idx -= k;
		j -= k;
	}
	return(val);
}

/*read N bit from the bit stream */
unsigned long getbits(Bit_stream_struc *bs, int N)
/* bit stream structure */
/* number of bits to read from the bit stream */
{
	unsigned long val=0;
	register int i;
	register int j = N;
	register int k, tmp;
	
	if (N > MAX_LENGTH)
		printf("Cannot read or write more than %d bits at a time.\n", MAX_LENGTH);
	
	bs->totbit += N;
	while (j > 0) {
		if (!bs->buf_bit_idx) {
			bs->buf_bit_idx = 8;
			bs->buf_byte_idx--;
			if ((bs->buf_byte_idx < MINIMUM) || (bs->buf_byte_idx < bs->eob)) {
				if (bs->eob)
					bs->eobs = TRUE;
				else {
					for (i=bs->buf_byte_idx; i>=0;i--)
						bs->buf[bs->buf_size-1-bs->buf_byte_idx+i] = bs->buf[i];
					refill_buffer(bs);
					bs->buf_byte_idx = bs->buf_size-1;
				}
			}
		}
		k = MIN (j, bs->buf_bit_idx);
		tmp = bs->buf[bs->buf_byte_idx]&putmask[bs->buf_bit_idx];
		tmp = tmp >> (bs->buf_bit_idx-k);
		val |= tmp << (j-k);
		bs->buf_bit_idx -= k;
		j -= k;
	}
	return(val);
}

/*write N bits into the bit stream */
void putbits(Bit_stream_struc *bs, unsigned int val, int N)
{
	register int j = N;
	register int k, tmp;
	
	if (N > MAX_LENGTH)
		printf("Cannot read or write more than %d bits at a time.\n", MAX_LENGTH);
	
	bs->totbit += N;
	while (j > 0) {
		k = MIN(j, bs->buf_bit_idx);
		tmp = val >> (j-k);
		bs->buf[bs->buf_byte_idx] |= (tmp&putmask[k]) << (bs->buf_bit_idx-k);
		bs->buf_bit_idx -= k;
		if (!bs->buf_bit_idx) {
			bs->buf_bit_idx = 8;
			bs->buf_byte_idx--;
			if (bs->buf_byte_idx < 0)
				empty_buffer(bs, MINIMUM);
			bs->buf[bs->buf_byte_idx] = 0;
		}
		j -= k;
	}
}

/*return the current bit stream length (in bits)*/
unsigned long sstell(Bit_stream_struc *bs)
/* bit stream structure */
{
	return(bs->totbit);
}

/*return the status of the bit stream*/
/* returns 1 if end of bit stream was reached */
/* returns 0 if end of bit stream was not reached */
int end_bs(Bit_stream_struc *bs)
/* bit stream structure */
{
	return(bs->eobs);
}

/*this function seeks for a byte aligned sync word in the bit stream and
places the bit stream pointer right after the sync.
This function returns 1 if the sync was found otherwise it returns 0  */
int seek_sync(Bit_stream_struc *bs, long sync, int N)
{
	unsigned long aligning, stell(Bit_stream_struc*);
	unsigned long val;
	long maxi = (int)pow(2.0, (FLOAT)N) - 1;
	
	aligning = sstell(bs)%ALIGNING;
	if (aligning)
		getbits(bs, (int)(ALIGNING-aligning));
	
	val = getbits(bs, N);
	while (((val&maxi) != sync) && (!end_bs(bs))) {
        val <<= ALIGNING;
        val |= getbits(bs, ALIGNING);
	}
	
	if (end_bs(bs)) return(0);
	else return(1);
}
/*****************************************************************************
*
*  End of bit_stream.c package
*
*****************************************************************************/

