 #include "bitStream.h"

BitStream::BitStream(const string _filename,bool _write)
{
//	_filename = __filename;
	writable = _write;
	byteidx = 0;
	bitidx = 0;
	lastBytes = 0;
	bufcount = 0;
	eobs = false;
	memset(buff,0,sizeof(buff));

	try{
	if( _filename.length() > 0){
		if( writable ){
			if ( (bitfile = fopen(_filename.c_str(),"wb+") ) == NULL ){
				printf("Unable to create file %s for writing.\n", _filename.c_str());
				exit(1);
			}
		}else{
			if ( (bitfile = fopen(_filename.c_str(),"rb") ) == NULL){
				printf("Unable to open file %s for reading.\n", _filename.c_str());
				exit(1);
			}
		}
		filelength = _filelength(bitfile->_file);
	}else{
		writable = true;
		filelength = 0;
		if( (bitfile = tmpfile()) == NULL){
			fprintf(stderr,"Can't create tmp file\n");
			exit(1);
		}
	}
	}
	catch ( ... ) {
		fprintf(stderr,"something wrong in BitStream \n");
	}

}

/*close the device containing the bit stream after a read process*/
BitStream::~BitStream()
{
	if( writable && byteidx > 0 ){
		flush_buffer();
	}
	if (bitfile )
		fclose(bitfile);
}

bool BitStream::refill_buffer()
{
	lastBytes += bufcount;
	byteidx = 0;
	bitidx = 0;
	bufcount = fread(buff, sizeof(unsigned char), BITSTREAM_BUFF_SIZE, bitfile);
	if( bufcount <= 0 ){
		bufcount = 0;
		eobs = true;
		return false;
	}
	return true;
}

void BitStream::flush_buffer()
{
	lastBytes += byteidx;
	fwrite(buff,sizeof(unsigned char),byteidx,bitfile);
	memset(buff,0,sizeof(buff));
	byteidx = 0;
	bitidx = 0;
}

/*read 1 bit from the bit stream */
bool BitStream::get1bit()
{
	bool bit;
	
	bit = (buff[byteidx] >> (7-bitidx) ) & 0x1;
	bitidx++;
	if (bitidx ==8)
	{
		bitidx = 0;
		byteidx++;
		if( byteidx >= bufcount){
			refill_buffer();
		}
	}

	return bit;
}

/*read 1 bit from the bit stream */
void BitStream::set1bit(bool val)
{
	buff[byteidx] |= (val & 0x01) << (7-bitidx);
	bitidx++;
	if (bitidx ==8){
		bitidx = 0;
		byteidx++;
		if ( byteidx >= BITSTREAM_BUFF_SIZE ) {
			flush_buffer();
		}
	}
}

/*read length bit from the bit stream */
unsigned int BitStream::getbits(unsigned int length)
{
	unsigned int rval = 0;
	while( length ){
		if( length + bitidx >= 8){
			rval <<= 8-bitidx;
			rval |= buff[byteidx] & ((1<<(8-bitidx))-1) ;
			length -= 8-bitidx;
//			buff[byteidx++] = 0;
			bitidx = 0;
			byteidx ++;
			if( byteidx >= bufcount){
				refill_buffer();
			}
		}else{
			rval <<= length;
			int last = (8 - bitidx - length);
			rval |=  (buff[byteidx] & ((1<<(8-bitidx))-1) ) >> last;
			bitidx += length;
			length = 0;
		}
	}
	return rval;
}

void BitStream::setbits(unsigned int val,unsigned int length)
{
	unsigned int rval = 0;
	while( length ){
		if( length + bitidx >= 8) {   //当前字节写不下全部位
			unsigned char more = length + bitidx - 8 ;
			buff[byteidx] |= (val >> more) & ((1<<(8-bitidx))-1);
			byteidx++;
			buff[byteidx] = 0;//将读过的数据置零
			bitidx = 0;
//			rval &= ~( ( (~(int)0) >> more) << more );
			length = more; //剩下的
			if( byteidx >= BITSTREAM_BUFF_SIZE){
				flush_buffer();
			}
		}else{
			//当前字节可以写下全部位
			unsigned char less = ( 8 - bitidx - length);
			buff[byteidx] |= ( val & ( (1<<length)-1 ) ) << less;
			bitidx += length;
			length = 0;
		}
	}
}

#define CHECK_END {if( eobs) return false;}

bool BitStream::seek_sync(unsigned int sync, unsigned int length)
{
	unsigned int maxi = (1<<length)-1;
	
	if( bitidx){
		getbits( 8-bitidx );
	}

	if( byteidx >= bufcount)
		refill_buffer();

	CHECK_END;
	while ( nextbits(length) != sync) {
		CHECK_END;
		getbits(8);
	}
	
	return true;
}

void BitStream::alignbits()
{
	if ( bitidx ) {
		setbits(0,8-bitidx);
	}
}

/*look ahead for the next length bits from the bit stream */
unsigned int BitStream::nextbits(unsigned int length)
{
	unsigned char tmp_bfr[8];

	if( (bufcount-byteidx)*8-bitidx < length){
		int less = length - ((bufcount-byteidx)*8-bitidx);
		for(int i=0;i<bufcount-byteidx;i++)
			tmp_bfr[i]=buff[byteidx+i];
		int result = fread(tmp_bfr+i,1,less/8,bitfile);
		if( result < less/8){
			return 0;
		}
		fseek(bitfile,-less/8,SEEK_CUR);
	}else{
		int need_bytes = 1;
		if ( length > 8-bitidx ) {
			need_bytes += ceil( (length-8+bitidx)/8.0 );
		}
		memcpy(tmp_bfr,buff+byteidx,need_bytes );
	}

	int bit_idx = bitidx;
	int byte_idx = 0;
	unsigned int rval = 0;
	while( length ){
		if( length + bit_idx >= 8){
			rval <<= 8-bit_idx;
			rval |= tmp_bfr[byte_idx] & ((1<<(8-bit_idx))-1) ;
			length -= 8-bit_idx;
			bit_idx = 0;
			byte_idx ++;
		}else{
			rval <<= length;
			int last = (8 - bit_idx - length);
			rval |=  (tmp_bfr[byte_idx] & ((1<<(8-bit_idx))-1) ) >> last;
			bit_idx += length;
			length = 0;
		}
	}
	return rval;
}

int BitStream::read(unsigned char* _buf, unsigned int length)
{
	if( bitidx ){
		getbits(8-bitidx);
	}
	if ( eobs ) {
		return 0;
	}
	if ( bufcount-byteidx >= length ) {
		memcpy(_buf,buff+byteidx,length);
		byteidx += length;
		return length;
	}else{
		int last = bufcount-byteidx;
		memcpy(_buf,buff+byteidx,last);
		byteidx = bufcount;
		int readed = fread(_buf+last,1,length-last,bitfile);
		lastBytes += readed;
		refill_buffer();
		return last + readed;
	}
}

int BitStream::write(unsigned char* _buf,unsigned int length)
{
	if( bitidx ){
		setbits(0,8-bitidx);
	}
	flush_buffer();
	fwrite(_buf,1,length,bitfile);
	lastBytes += length;
	return length;
}

void BitStream::reset(bool flush )
{
	try{
	if( flush && writable && byteidx > 0 ){
		flush_buffer();
	}
	if( bitfile ){
		rewind(bitfile);
	}
	byteidx = 0;
	bitidx = 0;
	eobs = false;
	bufcount = 0;
	lastBytes = 0;
	}
	catch ( ... ) {
		fprintf(stderr,"error while flush \n");
	}

}
