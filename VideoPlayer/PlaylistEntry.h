//---------------------------------------------------------------------------

#ifndef PlaylistEntryH
#define PlaylistEntryH
//---------------------------------------------------------------------------

#include <System.hpp>
#include <stdint.h>

namespace Json
{
	class Value;
}

struct PlaylistEntry
{
	AnsiString fileName;

	AnsiString url;
	AnsiString name;

	uint64_t size;
	AnsiString timeStamp;
	bool mark;
	double length;			// value < 0 if not checked; value = 0 if not recognized by mplayer
	double playbackProgress;// storing highest playback position in relation to file time
    enum { BITRATE_DEFAULT = -1 };
	int bitrateVideoMin, bitrateVideoMax;
	int bitrateAudioMin, bitrateAudioMax;
	int softVolLevel;
	enum { SOFTVOL_LEVEL_DEFAULT = 50 };
	AnsiString mplayerExtraParams;
	unsigned int skipIntroLength;
	unsigned int skipOutroLength;
	PlaylistEntry(void):
		size(0),
		mark(false),
		length(-1.0),
		playbackProgress(0.0),
		bitrateVideoMin(BITRATE_DEFAULT),
		bitrateVideoMax(BITRATE_DEFAULT),
		bitrateAudioMin(BITRATE_DEFAULT),
		bitrateAudioMax(BITRATE_DEFAULT),
		softVolLevel(SOFTVOL_LEVEL_DEFAULT),
		skipIntroLength(0),
		skipOutroLength(0)
	{}
	bool isValid(void) const;
	void fromJson(const Json::Value &jv);
	void toJson(Json::Value &jv) const;

	AnsiString getTarget(void) const {
		if (fileName != "")
			return fileName;
		return url;
	}
	AnsiString getDescription(void) const;
};

#endif
