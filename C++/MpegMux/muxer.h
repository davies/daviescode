  #ifndef __MUX_H__
#define __MUX_H__

#include "mpa/mpa.h"
#include "system/pack.h"
#include "VideoStream.h"

//in Converter.cpp
extern void convert(FILE* inAC3,FILE* outMPA);

class Muxer
{
public:
	Muxer(string _inFileName,string _outFileName = "",
		int _horizontal = 0,int _vertical = 0,int _video_bit_rate = 0.0,
		int _audioIndex1 = 1,double _leftScale = 0.0,
		int _audioIndex2 = 2,double _rightScale = 0.0,
		bool _m2v_out = false, bool _au3_inc = true);
	~Muxer();
	void demux();
	bool convert();
	bool mux();
	
	static double getMaxVol(MpaStream* mpaStream,int ch);
	static MpaStream* normalize(MpaStream* mpaStream, double leftLevel,double rightLevel,
		bool swap = false);
	void normalize();

	MpaStream* muxMpa(MpaStream* leftCh,MpaStream* rightCh);
	bool muxMpa();

	void writeScale();
	
	bool doVideo();

	bool process();
protected:
	unsigned int getVideoBitrate(BitStream *bs);
	int getUnusedIndex();

private:

	int version;
	string inFileName,outFileName;
	
	BitStream *inMPEG;
	BitStream *outMPEG;

	unsigned int video_bit_rate;
	int video_stream_id;
	int video_PSTD_buffer_size_bound;
	double video_delay;
	double video_dPTS;
	double video_PTS_DTS;
	VideoStream* videoBS;
	int horizontal;
	int vertical;
	int video_new_bit_rate;

	int leftChIndex,rightChIndex;
	double leftLevel,rightLevel;
	int mixedAudioIndex;
	int audio_stream_count;
	double delay[MAX_AUDIO_STREAMS];
	MpaStream* audioBS[MAX_AUDIO_STREAMS];
	int audioType[MAX_AUDIO_STREAMS];

	bool m2v_out ;
	bool au3_inc ;
};

#endif
