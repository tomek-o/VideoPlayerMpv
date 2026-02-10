//---------------------------------------------------------------------------


#pragma hdrstop

#include "PlaylistEntry.h"
#include "Log.h"
#include <json/json.h>
#include <SysUtils.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

bool PlaylistEntry::isValid(void) const
{
	if (fileName == "" && url == "")
		return false;
	return true;
}

void PlaylistEntry::fromJson(const Json::Value &jv)
{
	jv.getAString("fileName", fileName);
	jv.getAString("url", url);
	jv.getAString("name", name);
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
	jv.getUInt("skipIntroLength", skipIntroLength);
	jv.getUInt("skipOutroLength", skipOutroLength);
}

void PlaylistEntry::toJson(Json::Value &jv) const
{
	if (fileName != "")
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
		if (skipIntroLength != 0)
		{
			jv["skipIntroLength"] = skipIntroLength;
		}
		if (skipOutroLength != 0)
		{
			jv["skipOutroLength"] = skipOutroLength;
		}
	}
	else if (url != "")
	{
		jv["url"] = url;
		jv["name"] = name;
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

AnsiString PlaylistEntry::getDescription(void) const
{
	AnsiString text;
	if (fileName != "")
	{
		text.sprintf("File: %s", System::AnsiToUtf8(ExtractFileName(fileName)).c_str());
	}
	else if (name != "")
	{
		text = name;
	}
	else if (url != "")
	{
		text = url;
	}
	else
	{
		LOG("Strange: entry without filename?\n");
	}
	return text;
}

