//---------------------------------------------------------------------------


#pragma hdrstop

#include "PlaylistEntry.h"
#include <json/json.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

bool PlaylistEntry::isValid(void) const
{
	if (fileName == "")
		return false;
	return true;
}

void PlaylistEntry::fromJson(const Json::Value &jv)
{
	jv.getAString("fileName", fileName);
	size = jv.get("size", size).asUInt64();
	jv.getAString("timeStamp", timeStamp);
	jv.getBool("mark", mark);
	jv.getDouble("length", length);
	jv.getDouble("playbackProgress", playbackProgress);
	jv.getInt("bitrateVideoMin", bitrateVideoMin);
	jv.getInt("bitrateVideoMax", bitrateVideoMax);
	jv.getInt("bitrateAudioMin", bitrateAudioMin);
	jv.getInt("bitrateAudioMax", bitrateAudioMax);
	{
		// backward compatibility with mplayer-based version
		const Json::Value &jbv = jv["bitrateVideo"];
		if (jbv.type() == Json::intValue || jbv.type() == Json::uintValue) {
			bitrateVideoMin = bitrateVideoMax = jbv.asInt();
		}

		const Json::Value &jba = jv["bitrateAudio"];
		if (jba.type() == Json::intValue || jbv.type() == Json::uintValue) {
			bitrateAudioMin = bitrateAudioMax = jba.asInt();
		}
	}
	jv.getAString("mplayerExtraParams", mplayerExtraParams);
	jv.getInt("softVol", softVolLevel);
}

void PlaylistEntry::toJson(Json::Value &jv) const
{
	jv["fileName"] = fileName;
	jv["size"] = size;
	jv["timeStamp"] = timeStamp;
	jv["mark"] = mark;
	jv["length"] = length;
	jv["playbackProgress"] = playbackProgress;
	if (bitrateVideoMin != BITRATE_DEFAULT)
	{
		jv["bitrateVideoMin"] = bitrateVideoMin;
	}
	if (bitrateVideoMax != BITRATE_DEFAULT)
	{
		jv["bitrateVideoMax"] = bitrateVideoMax;
	}
	if (bitrateAudioMin != BITRATE_DEFAULT)
	{
		jv["bitrateAudioMin"] = bitrateAudioMin;
	}
	if (bitrateAudioMax != BITRATE_DEFAULT)
	{
		jv["bitrateAudioMax"] = bitrateAudioMax;
	}
	if (mplayerExtraParams != "")
	{
		jv["mplayerExtraParams"] = mplayerExtraParams;
	}
	if (softVolLevel != SOFTVOL_LEVEL_DEFAULT)
	{
		jv["softVol"] = softVolLevel;
	}
}


