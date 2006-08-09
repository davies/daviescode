               #include "pack.h"

#pragma warning(disable:4800)

/************************************************************************/
/* Pack                                                                 */
/************************************************************************/
Pack::Pack()
{
	//init
	version = 0;
//	stream_id = 0;
	SCR = 0.0;
	SCR_base = 0;
	SCR_extension = 0;
	system_header_flag = false;
	pack_stuffing_length = 0;
	program_mux_rate = 0;

	packet_count = 0;
	for(int i=0;i<MAX_PES_COUNT;i++){
		packet[i] = NULL;
	}
}

Pack::~Pack()
{
	for(int i=0;i<MAX_PES_COUNT;i++)
		if ( packet[i] ) {
			delete packet[i];
			packet[i] = NULL;
		}
}

void Pack::reset()
{
	SCR = 0.0;
	SCR_base = 0;
	SCR_extension = 0;
	system_header_flag = false;
	pack_stuffing_length = 0;
//	program_mux_rate = 0;

	for(int i=0;i<MAX_PES_COUNT;i++){
		if ( packet[i] ) {
			delete packet[i];
			packet[i] = NULL;
		}
	}
	packet_count = 0;
}


int Pack::getStreamID(){
	if ( packet[0] ){
		return packet[0]->getStreamID();
	}else
		return 0;
}

void Pack::setStreamID(unsigned char _stream_id){
	if (packet[0]) {
		packet[0]->setStreamID( _stream_id);
	}
}

int Pack::getSubStreamID(){
	if ( packet[0] ){
		return packet[0]->getSubStreamID();
	}else
		return 0;
}

void Pack::setSubStreamID(unsigned char _substream_id){
	if (packet[0]) {
		packet[0]->setSubStreamID( _substream_id);
	}
}

bool Pack::isAudioStream()
{
	return getStreamID() >= AUDIO_STREAM_0 && getStreamID() <= AUDIO_STREAM_1F
		|| getStreamID() == PRIVATE_STREAM_1 && getSubStreamID() >= SUBSTREAM_AC3_0 && getSubStreamID() <= SUBSTREAM_AC3_8; 
}

int Pack::getAudeoStreamType()
{
	if( isAudioStream() ){
		if( getStreamID() >= AUDIO_STREAM_0 && getStreamID() <= AUDIO_STREAM_1F ){
			return AUDIOSTREAM_TYPE_MPA;
		}else if (getStreamID() == PRIVATE_STREAM_1 && getSubStreamID() >= SUBSTREAM_AC3_0 && getSubStreamID() <= SUBSTREAM_AC3_8) {
			return AUDIOSTREAM_TYPE_AC3;
		}else{
			return AUDIOSTREAM_TYPE_UNKNOWN;
		}
	}else{
		return NOT_A_AUDIOSTREAM;
	}
}

int Pack::getAudioIndex() 
{
	if ( getAudeoStreamType() == AUDIOSTREAM_TYPE_MPA ) {
		return getStreamID() - AUDIO_STREAM_0 ;
	}else if ( getAudeoStreamType() == AUDIOSTREAM_TYPE_AC3) {
		return getSubStreamID() - SUBSTREAM_AC3_0;
	}else{
		return -1;
	}
}

bool Pack::isVideoStream()
{
	return getStreamID() >= VIDEO_STREAM_0 && getStreamID() <= VIDEO_STREAM_F; 
}

void Pack::addPaddingPacket(int _pack_size )
{
	packet[packet_count] = new PES_Packet(version);
	packet[packet_count++]->setASPadding( _pack_size - length() );
}

bool Pack::parse(BitStream &bs)
{
	if ( bs.getbits(32) != PACK_START_CODE) {
		return false;
	}

	if( bs.getbits(2) == 1 ){
		version = MPEG2;
	}else{
		version = MPEG1;
		bs.getbits(2);
	}

	SCR_base = bs.getbits(3) << 30;
	bs.get1bit();
	SCR_base |= bs.getbits(15) << 15;
	bs.get1bit();
	SCR_base |= bs.getbits(15);
	bs.get1bit();
	SCR = (long double)SCR_base / 90000.0;
	if( version == MPEG2 ){
		SCR_extension = bs.getbits(9);
		SCR += (long double)SCR_extension / 27000000.0;
	}
	bs.get1bit();
	program_mux_rate = bs.getbits(22);
	bs.get1bit();
	if( version == MPEG2 ){
		bs.getbits(1+5);
		pack_stuffing_length = bs.getbits(3);
		for(int i=0;i<pack_stuffing_length;i++)
			bs.getbits(8);
	}
	
	//System Header
	system_header_flag = false;
	if( bs.nextbits(32) == SYSTEM_HEADER_START_CODE ){

		system_header.parse(bs);
		system_header_flag = true;
	}
	
	//PES_packet
	packet_count = 0;
//	stream_id = 0;
	while ( bs.nextbits(32) != PACK_START_CODE
		&& bs.nextbits(32) != MPEG_PROGRAM_END_CODE
		&& bs.nextbits(24) == PACKET_START_CODE_PREFIX ) {
		if (packet[packet_count]) 
			delete packet[packet_count];
		packet[packet_count] = new PES_Packet(version);
		if ( !packet[packet_count] ) {
			fprintf(stderr,"\nNo enough memory\n");
			exit(1);
		}
		if ( !packet[packet_count]->parse(bs)) {
			return false;
		}
		packet_count ++;
	}

	return true;
}

bool Pack::encode(BitStream &bs)
{
	bs.setbits(PACK_START_CODE,32);

	if (version == MPEG2 ) {
		bs.setbits(1,2);
	}else{
		bs.setbits(2,4);
	}

	bs.setbits((SCR_base>>30) & 0x7,3);
	bs.set1bit();
	bs.setbits((SCR_base>>15)&((1<<15)-1),15);
	bs.set1bit();
	bs.setbits(SCR_base & ((1<<15)-1),15);
	bs.set1bit();
	if( version == MPEG2 ){
		bs.setbits(SCR_extension,9);
	}

	bs.set1bit();
	bs.setbits(program_mux_rate,22);
	bs.set1bit();

	if( version == MPEG2 ){
		bs.setbits(0xf,1+5);
		bs.setbits(pack_stuffing_length,3);
		for(int i=0;i<pack_stuffing_length;i++)
			bs.setbits(0xf,8);
	}
	
	//System Header
	if( system_header_flag){
		system_header.encode(bs);
	}

	//PES_packet
	for( int i=0;i<packet_count;i++){
		packet[i]->encode(bs);
	}

	return true;
}

/*
 *	将原始数据流打包成Pack
 */
int Pack::packData(BitStream& bs,int _pack_length)
{
	if( packet[packet_count] )
		delete packet[packet_count];
	packet[packet_count] = new PES_Packet(version);
	return packet[packet_count++]->packData(bs,_pack_length - length() );
}

void Pack::writeData(BitStream& out)
{
//	for(int i=0;i<packet_count;i++){
	if( packet_count)
		packet[0]->writeData(out);
//	}
}

int Pack::length()
{
	int length = 0;
	if( version == MPEG1){
		length = 4+5+3;
	}else{
		length = 4+6+3+1+pack_stuffing_length; //MPEG2
	}
	if( system_header_flag )
		length += system_header.length();
	for(int i=0;i<packet_count;i++){
		length += packet[i]->length();
	}

	return length;
}