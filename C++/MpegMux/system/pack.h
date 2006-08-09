#ifndef __PACK_H__
#define __PACK_H__

#include "..\common\def.h"
#include "..\common\BitStream.h"
#include "PES_packet.h"
#include "SystemHeader.h"

class Pack
{
public:
	Pack();
	Pack(BitStream &bs);
	~Pack();
	
	bool parse(BitStream &bs);
	bool encode(BitStream &bs);

	PES_Packet* getPacket(int index){return packet[index];}
	void addPaddingPacket( int _pack_length = PACK_SIZE);

	void setVersion(int _version){ 
		version = _version;
	}
	int getVersion(){ return version;}

	int getStreamID();
	void setStreamID(unsigned char _stream_id);
	int getSubStreamID();
	void setSubStreamID(unsigned char _substream_id);

	bool isAudioStream();
	int getAudioIndex();
	bool isVideoStream();
	int getAudeoStreamType();

	long double getSCR(){ return SCR;};
	void setSCR( long double _SCR){
		SCR = _SCR;
		SCR_base = SCR * 90000.0;
		SCR_extension = (SCR * 90000.0 - SCR_base) * 300.0;
	};
	long double getPTS(){ 
		if ( packet[0] ) {
			return packet[0]->getPTS();
		}else
			return 0;
	}
	void setPTS(long double _PTS_value){
		if( packet[0] ){
			packet[0]->setPTS(_PTS_value);
		}
	}

	unsigned int getProgramMuxRate(){ return program_mux_rate;};
	void setProgramMuxRate(unsigned int _program_mux_rate){program_mux_rate = _program_mux_rate;};

	bool hasSystemHeader(){ return system_header_flag;};
	SystemHeader& getSystemHeader(){ return system_header;};
	void setSystemHeader(const SystemHeader &header){
		system_header_flag = true;
		system_header = header;
	}
	
//	void addPES_packet()
	int length();
	
	int packData(BitStream& bs,int _pack_length = PACK_SIZE);
	void writeData(BitStream& out);
	
	void reset();
private:
	unsigned char version;
	long double SCR;
	unsigned long SCR_base;
	unsigned long SCR_extension;
	unsigned int program_mux_rate;
	unsigned char pack_stuffing_length;
//	unsigned char stream_id;
	
	bool system_header_flag;
	SystemHeader system_header;

	PES_Packet* packet[MAX_PES_COUNT];
	int packet_count;
};

#endif

