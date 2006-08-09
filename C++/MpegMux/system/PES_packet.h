#ifndef __PES_PACKET_H__
#define __PES_PACKET_H__

#include "..\common\def.h"
#include "..\common\BitStream.h"

class PES_Packet
{
public:
	PES_Packet(	unsigned char _version );
	PES_Packet(BitStream &bs);
	
	bool parse(BitStream &bs);
	bool encode(BitStream &bs);

	unsigned char getStreamID(){ return stream_id;};
	void setStreamID(unsigned char _stream_id){ stream_id = _stream_id;}
	unsigned char getSubStreamID() { return substream_id;}
	void setSubStreamID(unsigned char _substream_id){ substream_id = _substream_id;}
	
	int packData(BitStream& bs,int _packet_length);
	void writeData(BitStream & out);
	void setASPadding(int _packet_length );	

	int getFirstSyncPos(unsigned int sync,unsigned long bit_length);
	unsigned int getFirstBytes(int byte_length);
	unsigned int getLastBytes(int byte_length);
	unsigned int getBytes(int pos,int byte_length);

	int length();
	
	int getPTS_DTS_size(int _PTS_DTS_flag);
	void setPTS_DTS_flag(int _PTS_DTS_flag);
	double getPTS();
	void setPTS(double _PTS_value);
	double getDTS();
	void setDTS(double _DTS_value);

protected:
	bool parse1(BitStream &bs);
	bool encode1(BitStream &bs);
	bool parse2(BitStream &bs);
	bool encode2(BitStream &bs);
	unsigned short calcu_packet_length1();
	unsigned short calcu_packet_length2();
private:
	unsigned char stream_id;
	unsigned short PES_packet_length;

	unsigned char PES_scambling_control;
	bool PES_priority,data_alignment_indicator;
	bool copyright,original_or_copy;
	unsigned char PTS_DTS_flags;
	bool ESCR_flag, ES_rate_flag, DSM_trick_mode_flag;
	bool additional_copy_info_flag, PES_CRC_flag, PES_extension_flag;

	unsigned char PES_header_data_length;

	unsigned long PTS,DTS;
	unsigned long ESCR_base,ESCR_extension,ES_rate;

	unsigned char trick_mode_control;
	unsigned char field_id,intra_slice_refresh,frequency_stuncationi;
	unsigned char field_rep_cntrl;

	unsigned char additional_copy_info;
	unsigned short previous_PES_packet_CRC;

	//extension
	bool PES_private_data_flag, pack_header_field_flag;
	bool program_packet_sequence_counter_flag, PSTD_buffer_flag;
	bool PES_extension_flag_2;
	unsigned char PES_private_data[16];
	unsigned char pack_field_length;   //
	unsigned char program_packet_sequence_counter;
	bool MPEG1_MPEG2_identifier;
	unsigned char original_stuff_length;
	bool PSTD_buffer_scale;
	unsigned short PSTD_buffer_size;
	unsigned char PES_extension_field_length;

//	unsigned long substream_id, SCR_value, PTS_value, DTS_value;
//	unsigned long stream_packets[256], stream_bytes[256];
//	unsigned long substream_packets[256], substream_bytes[256];

	unsigned char substream_id;
	unsigned char frame_header;
	unsigned short first_access_unit_pointer;

	int stuffing_length;
	int data_length;
	unsigned char data[MAX_PES_PACKET_SIZE];

	unsigned char version;
	long double scr_value, PTS_value, DTS_value;
//	int layer, bit_rate, freq, padding_bit;
};

#endif