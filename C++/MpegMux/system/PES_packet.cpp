#include "PES_packet.h"

#pragma warning(disable:4800)

PES_Packet::PES_Packet(unsigned char _version)
{
	version = _version;
	stream_id = 0;
	if( version == MPEG2){
		PES_scambling_control = 0;
		PES_priority = false;
		data_alignment_indicator = false;
		copyright = true;
		original_or_copy = true;

		PTS_DTS_flags = 0;
		ESCR_flag = false;
		ES_rate_flag = false;
		DSM_trick_mode_flag = false;
		additional_copy_info_flag = false;
		PES_CRC_flag = false;
		PES_extension_flag = false;
		PES_header_data_length = 0;
	
		PES_packet_length = 3 + PES_header_data_length;
	}else{
		//MPEG1
		PES_packet_length = 1;
		PSTD_buffer_flag = false;
		PTS_DTS_flags = 0;
	}

	stuffing_length = 0;
	data_length = 0;
	memset(data,0,sizeof(data));
}

PES_Packet::PES_Packet(BitStream &bs)
{
//	decode(bs);
}

bool PES_Packet::parse(BitStream &bs)
{
	if( version == MPEG2)
		return parse2(bs);
	else
		return parse1(bs);
}

bool PES_Packet::encode(BitStream &bs)
{
	if( version == MPEG2){
		return encode2(bs);
	}else{
		return encode1(bs);
	}
}

bool PES_Packet::parse1(BitStream &bs)
{
	bs.getbits(24);

	stream_id = bs.getbits(8);
	PES_packet_length = bs.getbits(16);
	if ( PES_packet_length > MAX_PES_PACKET_SIZE ) {
		return false;
	}
	if( stream_id != PRIVATE_STREAM_2)
	{
		int byte_used = 0;
		while ( bs.nextbits(1) == 1 ) {
			bs.getbits(8);
			stuffing_length ++;
			byte_used ++;
		}
		if( bs.nextbits(2) == 1 ){
			PSTD_buffer_flag = true;
			bs.getbits(2);
			PSTD_buffer_scale = bs.get1bit();
			PSTD_buffer_size = bs.getbits(13);
			byte_used += 2;
		}
		if( bs.nextbits(4)  == 0x2 ){
			PTS_DTS_flags = bs.getbits(4);

			PTS = bs.getbits(3) << 30;
			bs.get1bit();
			PTS |= bs.getbits(15) << 15;
			bs.get1bit();
			PTS |= bs.getbits(15);
			bs.get1bit();
			PTS_value = (long double) PTS / 90000.0;

			byte_used += 5;
		}else if ( bs.nextbits(4) == 3) {
			PTS_DTS_flags = bs.getbits(4);

			PTS = bs.getbits(3) << 30;
			bs.get1bit();
			PTS |= bs.getbits(15) << 15;
			bs.get1bit();
			PTS |= bs.getbits(15);
			bs.get1bit();
			PTS_value = (long double) PTS / 90000.0;
			
			bs.getbits(4);
			DTS = bs.getbits(3) << 30;
			bs.get1bit();
			DTS |= bs.getbits(15) << 15;
			bs.get1bit();
			DTS |= bs.getbits(15);
			bs.get1bit();
			DTS_value = (long double) DTS / 90000.0;
			
			byte_used += 10;
		}else if ( bs.nextbits(8) == 0x0f ) {
			bs.getbits(8);
			byte_used ++;
		}
		data_length = PES_packet_length-byte_used; 
		bs.read(data,data_length);
	}

	return true;
}

unsigned short PES_Packet::calcu_packet_length1()
{
	unsigned short length = 0;
	return length;
}

bool PES_Packet::encode1(BitStream &bs)
{
	bs.setbits(PACKET_START_CODE_PREFIX,24);

	bs.setbits(stream_id,8);
	bs.setbits(PES_packet_length,16);

	if( stream_id != PRIVATE_STREAM_2)
	{
		for(int i=0;i<stuffing_length;i++)
			bs.setbits(0xff,8);
		if( PSTD_buffer_flag ){
			bs.setbits(1,2);
			bs.set1bit(PSTD_buffer_scale);
			bs.setbits(PSTD_buffer_size,13);
		}
		if( PTS_DTS_flags  == 0x2 ){
			bs.setbits(PTS_DTS_flags,4);

			bs.setbits((PTS>>30) & 0x7,3);
			bs.set1bit();
			bs.setbits((PTS>>15)&((1<<15)-1),15);
			bs.set1bit();
			bs.setbits(PTS & ((1<<15)-1),15);
			bs.set1bit();
		}else if ( PTS_DTS_flags == 0x3) {
			bs.setbits(PTS_DTS_flags,4);

			bs.setbits((PTS>>30) & 0x7,3);
			bs.set1bit();
			bs.setbits((PTS>>15)&((1<<15)-1),15);
			bs.set1bit();
			bs.setbits(PTS & ((1<<15)-1),15);
			bs.set1bit();

			bs.setbits(1,4);

			bs.setbits((DTS>>30) & 0x7,3);
			bs.set1bit();
			bs.setbits((DTS>>15)&((1<<15)-1),15);
			bs.set1bit();
			bs.setbits(DTS & ((1<<15)-1),15);
			bs.set1bit();
		}else{
			bs.setbits(0x0f,8);
		}
		bs.write(data,data_length);
	}else{
		return false;
	}

	return true;
}


bool PES_Packet::parse2(BitStream &bs)
{
	if( bs.getbits(24) != PACKET_START_CODE_PREFIX){
		return false;
	}
	
	stream_id = bs.getbits(8);
	PES_packet_length = bs.getbits(16);
	if ( PES_packet_length > MAX_PES_PACKET_SIZE ) {
		return false;
	}

	if( stream_id != PROGRAM_STREAM_MAP
		&& stream_id != PADDING_STREAM
		&& stream_id != PRIVATE_STREAM_2
		&& stream_id != ECM_STREAM
		&& stream_id != EMM_STREAM
		&& stream_id != PROGRAM_STREAM_DIRECTORY)
	{
		if ( bs.getbits(2) != 2 ) {
			return false;
		}

		PES_scambling_control = bs.getbits(2);
		PES_priority = bs.get1bit();
		data_alignment_indicator = bs.get1bit();
		copyright = bs.get1bit();
		original_or_copy = bs.get1bit();
		PTS_DTS_flags = bs.getbits(2);
		ESCR_flag = bs.get1bit();
		ES_rate_flag = bs.get1bit();
		DSM_trick_mode_flag = bs.get1bit();
		additional_copy_info_flag = bs.get1bit();
		PES_CRC_flag = bs.get1bit();
		PES_extension_flag = bs.get1bit();

		PES_header_data_length = bs.getbits(8);
		int byte_used = 0;

		if( PTS_DTS_flags  == 0x2 ){
			if( bs.getbits(4) != 2 ) 
				return false;
			PTS = bs.getbits(3) << 30;
			bs.get1bit();
			PTS |= bs.getbits(15) << 15;
			bs.get1bit();
			PTS |= bs.getbits(15);
			bs.get1bit();
			PTS_value = (long double) PTS / 90000.0;
			byte_used += 5;
		}
		if ( PTS_DTS_flags == 3) {
			if( bs.getbits(4) != 3 )
				return false;
			PTS = bs.getbits(3) << 30;
			bs.get1bit();
			PTS |= bs.getbits(15) << 15;
			bs.get1bit();
			PTS |= bs.getbits(15);
			bs.get1bit();
			PTS_value = (long double) PTS / 90000.0;
			if ( bs.getbits(4) != 1) {
				return false;
			}
			DTS = bs.getbits(3) << 30;
			bs.get1bit();
			DTS |= bs.getbits(15) << 15;
			bs.get1bit();
			DTS |= bs.getbits(15);
			bs.get1bit();
			DTS_value = (long double) DTS / 90000.0;
			byte_used += 10;
		}
		if ( ESCR_flag ) {
			bs.getbits(2);
			ESCR_base = bs.getbits(3) << 30;
			bs.get1bit();
			ESCR_base |= bs.getbits(15) << 15;
			bs.get1bit();
			ESCR_base |= bs.getbits(15);
			bs.get1bit();
			ESCR_extension = bs.getbits(9);
			bs.get1bit();

			byte_used += 6;
		}
		if (ES_rate_flag) {
			bs.get1bit();
			ES_rate = bs.getbits(22);
			bs.get1bit();

			byte_used += 3;
		}
		if ( DSM_trick_mode_flag ) {
			trick_mode_control = bs.getbits(3);
			if( trick_mode_control == 0 ||  trick_mode_control == 3){
				field_id = bs.getbits(2);
				intra_slice_refresh = bs.get1bit();
				frequency_stuncationi = bs.getbits(2);
			}else if ( trick_mode_control == 1 ||  trick_mode_control == 4) {
				field_rep_cntrl = bs.getbits(5);
			}else if ( trick_mode_control == 2 ) {
				field_id = bs.getbits(2);
				bs.getbits(3);
			}else 
				bs.getbits(5);
			byte_used ++;
		}
		if (additional_copy_info_flag) {
			bs.get1bit();
			additional_copy_info = bs.getbits(7);
			byte_used ++;
		}
		if (PES_CRC_flag) {
			previous_PES_packet_CRC = bs.getbits(16);
			byte_used += 2;
		}
		if (PES_extension_flag) {
			PES_private_data_flag = bs.get1bit();
			pack_header_field_flag = bs.get1bit();
			program_packet_sequence_counter_flag = bs.get1bit();
			PSTD_buffer_flag = bs.get1bit();
			bs.getbits(3);
			PES_extension_flag_2 = bs.get1bit();
			
			byte_used ++;

			if ( PES_private_data_flag) {
				for( int i=0;i<16;i++)
					PES_private_data[i] = bs.getbits(8);
				byte_used += 16;
			}
			if ( pack_header_field_flag) {
				pack_field_length = bs.getbits(8);
				byte_used ++;
				byte_used += pack_field_length;
				//Pack header
			}
			if (program_packet_sequence_counter_flag) {
				bs.get1bit();
				program_packet_sequence_counter = bs.getbits(7);
				bs.get1bit();
				MPEG1_MPEG2_identifier = bs.get1bit();
				original_stuff_length = bs.getbits(6);
				byte_used +=2;
			}
			if ( PSTD_buffer_flag) {
				if ( bs.getbits(2) != 1) {
					return false;
				}
				PSTD_buffer_scale = bs.get1bit();
				PSTD_buffer_size = bs.getbits(13);
				byte_used += 2;
			}
			if ( PES_extension_flag_2 ) {
				bs.get1bit();
				PES_extension_field_length = bs.getbits(7);
				byte_used ++;
				for( int i=0;i<PES_extension_field_length;i++)
					bs.getbits(8);
				//byte_used ++;
				byte_used += PES_extension_field_length;
			}
		}

		for(int i=0;i<PES_header_data_length -byte_used;i++){
			bs.getbits(8);
		}
		
		data_length = PES_packet_length - PES_header_data_length - 3;
		//vob support
		if ( stream_id == PRIVATE_STREAM_1 ) {
			substream_id = bs.getbits(8);
			frame_header = bs.getbits(8);
			first_access_unit_pointer = bs.getbits(16);
			data_length -= 4;
		}

		if ( data_length < 0 ) {
			return false;
		}
		bs.read(data,data_length);

	}else if ( stream_id == PROGRAM_STREAM_MAP
		|| stream_id == PRIVATE_STREAM_2
		|| stream_id == ECM_STREAM
		|| stream_id == EMM_STREAM
		|| stream_id == PROGRAM_STREAM_DIRECTORY ) {
		data_length = PES_packet_length;
		bs.read(data,PES_packet_length);
	}else if ( stream_id == PADDING_STREAM ) {
		data_length = PES_packet_length;
		bs.read(data,PES_packet_length);
	}

	return true;
}

bool PES_Packet::encode2(BitStream &bs)
{
	bs.setbits(PACKET_START_CODE_PREFIX,24);
	
	bs.setbits(stream_id,8);
	bs.setbits(PES_packet_length,16);

	if( stream_id != PROGRAM_STREAM_MAP
		&& stream_id != PADDING_STREAM
		&& stream_id != PRIVATE_STREAM_2
		&& stream_id != ECM_STREAM
		&& stream_id != EMM_STREAM
		&& stream_id != PROGRAM_STREAM_DIRECTORY)
	{
		bs.setbits(2,2);

		bs.setbits(PES_scambling_control,2);
		bs.set1bit(PES_priority);
		bs.set1bit(data_alignment_indicator);
		bs.set1bit(copyright);
		bs.set1bit(original_or_copy);
		bs.setbits(PTS_DTS_flags,2);
		bs.set1bit(ESCR_flag);
		bs.set1bit(ES_rate_flag);
		bs.set1bit(DSM_trick_mode_flag);
		bs.set1bit(additional_copy_info_flag);
		bs.set1bit(PES_CRC_flag);
		bs.set1bit(PES_extension_flag);

		bs.setbits(PES_header_data_length,8);		

		int byte_used = 0;

		if( PTS_DTS_flags  == 0x2 ){
			bs.setbits(PTS_DTS_flags,4);

			bs.setbits((PTS>>30) & 0x7,3);
			bs.set1bit();
			bs.setbits((PTS>>15)&((1<<15)-1),15);
			bs.set1bit();
			bs.setbits(PTS & ((1<<15)-1),15);
			bs.set1bit();

			byte_used += 5;
		}else if ( PTS_DTS_flags == 3) {
			bs.setbits(PTS_DTS_flags,4);

			bs.setbits((PTS>>30) & 0x7,3);
			bs.set1bit();
			bs.setbits((PTS>>15)&((1<<15)-1),15);
			bs.set1bit();
			bs.setbits(PTS & ((1<<15)-1),15);
			bs.set1bit();

			bs.setbits(1,4);

			bs.setbits((DTS>>30) & 0x7,3);
			bs.set1bit();
			bs.setbits((DTS>>15)&((1<<15)-1),15);
			bs.set1bit();
			bs.setbits(DTS & ((1<<15)-1),15);
			bs.set1bit();

			byte_used += 10;
		}
		if ( ESCR_flag ) {
			bs.setbits(3,2);

			bs.setbits((ESCR_base>>30) & 0x7,3);
			bs.set1bit();
			bs.setbits((ESCR_base>>15)&((1<<15)-1),15);
			bs.set1bit();
			bs.setbits(ESCR_base & ((1<<15)-1),15);
			bs.set1bit();
			
			bs.setbits(ESCR_extension,9);
			bs.set1bit();

			byte_used += 6;
		}
		if (ES_rate_flag) {
			bs.set1bit();
			bs.setbits(ESCR_base,22);
			bs.set1bit();
			byte_used += 3;
		}
		if ( DSM_trick_mode_flag ) {
			bs.setbits(trick_mode_control,3);
			
			if( trick_mode_control == 0 ||  trick_mode_control == 3){
				bs.setbits(field_id,2);
				bs.set1bit(intra_slice_refresh);
				bs.setbits(frequency_stuncationi,2);
			}else if ( trick_mode_control == 1 ||  trick_mode_control == 4) {
				bs.setbits(field_rep_cntrl,5);
			}else if ( trick_mode_control == 2 ) {
				bs.setbits(field_id,2);
				bs.setbits(0xf,3);
			}else 
				bs.setbits(0xf,5);

			byte_used ++;
		}
		if (additional_copy_info_flag) {
			bs.set1bit();
			bs.setbits(additional_copy_info,7);
			byte_used ++;
		}
		if (PES_CRC_flag) {
			bs.setbits(previous_PES_packet_CRC,16);
			byte_used += 2;
		}
		if (PES_extension_flag) {
			//PES_extension_flag_2 = bs.get1bit();

			bs.set1bit(PES_private_data_flag);
			bs.set1bit(pack_header_field_flag);
			bs.set1bit(program_packet_sequence_counter_flag);
			bs.set1bit(PSTD_buffer_flag);
			bs.setbits(0xf,3);
			bs.set1bit(PES_extension_flag_2);

			byte_used ++;

			if ( PES_private_data_flag) {
				for( int i=0;i<16;i++)
					bs.setbits(PES_private_data[i],8);
				byte_used += 16;
			}
/*			if ( pack_header_field_flag) {
				pack_field_length = bs.getbits(8);
				byte_used ++;
				byte_used += pack_field_length;
				//Pack header
			}
*/			if (program_packet_sequence_counter_flag) {
				bs.set1bit();
				bs.setbits(program_packet_sequence_counter,7);
				bs.set1bit();
				bs.set1bit(MPEG1_MPEG2_identifier);
				bs.setbits(original_stuff_length,6);
				byte_used +=2;
			}
			if ( PSTD_buffer_flag) {
				bs.setbits(1,2);
				bs.set1bit(PSTD_buffer_scale);
				bs.setbits(PSTD_buffer_size,13);
				byte_used += 2;
			}
			if ( PES_extension_flag_2 ) {
				bs.set1bit();
				bs.setbits(PES_extension_field_length,7);
				byte_used ++;
				for( int i=0;i<PES_extension_field_length;i++)
					bs.setbits(0xff,8);
				byte_used += PES_extension_field_length;
			}
		}

		for(int i=0;i<PES_header_data_length -byte_used;i++){
			bs.setbits(0xff,8);
		}
		data_length = PES_packet_length - PES_header_data_length - 3;
		bs.write(data,data_length);

	}else if ( stream_id == PROGRAM_STREAM_MAP
		|| stream_id == PRIVATE_STREAM_2
		|| stream_id == ECM_STREAM
		|| stream_id == EMM_STREAM
		|| stream_id == PROGRAM_STREAM_DIRECTORY ) {
		bs.write(data,PES_packet_length);
	}else if ( stream_id == PADDING_STREAM ) {
		bs.write(data,PES_packet_length);
	}

	return true;
}

/*
 *	将原始数据流
 */
int PES_Packet::packData(BitStream& bs,int target_packet_length){
	if( data_length != 0){ //已经有数据
		return -1;  
	}
	data_length = bs.read(data,target_packet_length - length());
	PES_packet_length += data_length;

	return data_length;
}

void PES_Packet::writeData(BitStream & out)
{
	out.write(data,data_length);
}

int PES_Packet::length()
{
	return PES_packet_length + 3 + 1 + 2;
}

void PES_Packet::setASPadding(int _packet_size )
{
	stream_id = PADDING_STREAM;
	data_length = _packet_size - 3 -1 -2 - PES_packet_length;
	PES_packet_length += data_length;
	memset(data,0,sizeof(data));
}

inline int PES_Packet::getPTS_DTS_size(int _PTS_DTS_flag){
	if ( _PTS_DTS_flag == 2 ) {
		return 5;
	}else if ( _PTS_DTS_flag == 3 ) {
		return 10;
	}else{
		if ( version == MPEG2 ) {
			return 0;
		}else{
			return 1;
		}
	}
}

void PES_Packet::setPTS_DTS_flag(int _PTS_DTS_flag)
{
	//改变PES_packet的大小
	PES_header_data_length += getPTS_DTS_size(_PTS_DTS_flag) - getPTS_DTS_size(PTS_DTS_flags);
	PES_packet_length += getPTS_DTS_size(_PTS_DTS_flag) - getPTS_DTS_size(PTS_DTS_flags);
	PTS_DTS_flags = _PTS_DTS_flag;
}

double PES_Packet::getPTS(){
	if ( PTS_DTS_flags ) {
		return PTS_value;
	}else{
		return 0.0;
	}
}

void PES_Packet::setPTS(double _PTS_value){
	PTS_value = _PTS_value;
	PTS = PTS_value * 90000.0;
	setPTS_DTS_flag(0x2);
}

double PES_Packet::getDTS(){
	if ( PTS_DTS_flags == 0x3 ) {
		return DTS_value;
	}else{
		return 0.0;
	}
}

void PES_Packet::setDTS(double _DTS_value){
	DTS_value = _DTS_value;
	DTS = DTS_value * 90000.0;
	setPTS_DTS_flag(0x3);
}

int PES_Packet::getFirstSyncPos(unsigned int syncCode,unsigned long width)
{
	int syncPos = -1;
	int i;
	unsigned int val = getFirstBytes(width/8);
	unsigned int mask_val =  ( 0x100 << (width-8) ) - 1;
	if ( val == syncCode ) {
		return 0;
	}
	for( i=width/8;i<data_length;i++){
		val <<= 8;
		val |= data[i];
		val &= mask_val;
		if ( val == syncCode ) {
			return i-width/8+1;
		}
	}
	return -1;
}

uint32 PES_Packet::getFirstBytes(int byte_length)
{
	unsigned int word = 0;
	for(int i=0;i<byte_length;i++){
		word <<= 8;
		word |= data[i];
	}
	return word;
}

uint32 PES_Packet::getLastBytes(int byte_length)
{
	uint32 word = 0;
	if ( data_length < byte_length ) {
		return word;
	}
	for(int i=data_length-byte_length;i<data_length;i++){
		word <<= 8 ;
		word |= data[i];
	}
	return word;
}

uint32 PES_Packet::getBytes(int offset,int byte_length)
{
	uint32 word = 0;
	for( int i=0;i<byte_length;i++){
		word <<= 8;
		word |= data[offset+i];
	}
	return word;
}