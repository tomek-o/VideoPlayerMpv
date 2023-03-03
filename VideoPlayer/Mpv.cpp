//---------------------------------------------------------------------------
#pragma hdrstop

/**	\note Do not use just "Mplayer" as this module name - link conflict with
	another module installed with IDE.
*/

#include "Mpv.h"
#include "Log.h"
#include <mpv/client.h>
#include <SysUtils.hpp>
#include <math.h>
#include <stdio.h>

#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

MPlayer mplayer;

MPlayer::MPlayer():
	mpv(NULL),
	parent(INVALID_HANDLE_VALUE),
	timer(NULL),
	filePositionValid(false),
	filePosition(0.0),
	fileLengthValid(false),
	fileLength(0.0),
	callbackStopPlaying(NULL),
	callbackMediaInfoUpdate(NULL),
	fileStarted(false),
	absoluteSeekRequested(-1)
{
	timer = new TTimer(NULL);
	timer->Interval = 20;
	timer->OnTimer = timerTimer;
	timer->Enabled = true;
}

int MPlayer::Configure(const Cfg& cfg)
{
	this->cfg = cfg;
	return 0;
}

MPlayer::~MPlayer()
{
	if (timer) {
		timer->Enabled = false;
		delete timer;
		timer = NULL;
	}
	MpvDestroy();
}

int MPlayer::run(AnsiString cmdLine)
{
	return 0;
}

void MPlayer::OnStopPlayingFn(void)
{
	if (callbackStopPlaying)
		callbackStopPlaying();
}

int MPlayer::play(AnsiString filename, int softVolLevel, AnsiString extraParams)
{
	AnsiString cmdLine;
	this->filename = filename;

	if (mpv == NULL)
		MpvCreate();
	if (mpv == NULL)
		return -1;
	LOG("play %s", filename.c_str());

	mediaInfo.videoBitrate = mediaInfo.audioBitrateKnown = false;
	filePositionValid = false;
	fileLengthValid = false;
	if (this->callbackMediaInfoUpdate)
	{
    	callbackMediaInfoUpdate();
	}

	{
		int64_t val = cfg.softVolMax;
		if (mpv_set_property(mpv, "volume-max", MPV_FORMAT_INT64, &val) < 0) {
			LOG("failed to set mpv volume-max");
		} else {
        	LOG("mpv volume-max set to %d", cfg.softVolMax);
		}
	}

	changeVolumeAbs(cfg.softVolLevel);

	const char *cmd[] = { "loadfile", filename.c_str(), NULL };
	int status = mpv_command(mpv, cmd);

	return status;
}

int MPlayer::pause(bool state)
{
	if (mpv == NULL)
		return -1;
	int flag = state;
	return mpv_set_property(mpv, "pause", MPV_FORMAT_FLAG, &flag);
}

int MPlayer::frame_step(void)
{
	if (mpv == NULL)
		return -1;
    const char *cmd[] = { "frame-step", NULL };
	return mpv_command(mpv, cmd);
}

int MPlayer::seekRelative(int seconds)
{
	if (mpv == NULL)
		return -1;
	AnsiString secondsStr = seconds;
	const char *cmd[] = { "seek", secondsStr.c_str(), "relative", NULL };
	return mpv_command(mpv, cmd);
}

int MPlayer::seekAbsolute(double seconds)
{
	if (mpv == NULL)
		return -1;
	if (!fileStarted)
	{
		LOG("Delaying seekAbsolute to %.1f", seconds);
		absoluteSeekRequested = seconds;
		return 0;
	}
	if (fileLengthValid)
	{
		if (seconds > fileLength)
		{
			absoluteSeekRequested = -1;
			return -1;
		}
	}
	LOG("seekAbsolute to %.1f", seconds);
	absoluteSeekRequested = -1;
	AnsiString msg;
	msg.sprintf("%f", seconds);
	AnsiString secondsStr = seconds;
	const char *cmd[] = { "seek", msg.c_str(), "absolute", NULL };
	return mpv_command(mpv, cmd);
}

int MPlayer::setOsdLevel(int level)
{
	if (mpv == NULL)
		return -1;
	int64_t val = level;
	if (mpv_set_property(mpv, "osd-level", MPV_FORMAT_INT64, &val) < 0) {
		LOG("failed to set mpv osd level");
		return -2;
	}
	return 0;
}

int MPlayer::changeVolume(int delta)
{
	if (mpv == NULL)
		return -1;

	if (cfg.softVolLevel + delta < 0 || cfg.softVolLevel + delta > cfg.softVolMax)
		return 0;
	cfg.softVolLevel += delta;

	int64_t val = cfg.softVolLevel;
	if (mpv_set_property(mpv, "volume", MPV_FORMAT_INT64, &val) < 0) {
		LOG("failed to set mpv volume");
		return -2;
	}
	return 0;
}

int MPlayer::changeVolumeAbs(int val)
{
	if (mpv == NULL)
		return -1;

	AnsiString msg;
	cfg.softVolLevel = val;
	int64_t val64 = cfg.softVolLevel;
	if (mpv_set_property(mpv, "volume", MPV_FORMAT_INT64, &val64) < 0) {
		LOG("failed to set mpv volume");
		return -2;
	}
	return 0;
}

int MPlayer::osdShowText(AnsiString text, int duration)
{
	if (mpv == NULL)
		return -1;

	AnsiString durationStr = duration;
	const char* cmd[] = { "show-text", text.c_str(), durationStr.c_str(), "0", NULL };
	return mpv_command(mpv, cmd);
}

int MPlayer::stop(bool useCallback)
{
	if (mpv == NULL)
		return -1;
	LOG("Stopping");
	fileStarted = false;	
    const char *cmd[] = { "stop", NULL };
	return mpv_command(mpv, cmd);
}

void __fastcall MPlayer::timerTimer(TObject *Sender)
{
    while (mpv) {
        const mpv_event *e = mpv_wait_event(mpv, 0);
        if (e->event_id == MPV_EVENT_NONE)
            break;
        OnMpvEvent(*e);
    }
}

void MPlayer::OnMpvEvent(const mpv_event &e)
{
	if (e.event_id != MPV_EVENT_LOG_MESSAGE && e.event_id != MPV_EVENT_PROPERTY_CHANGE)
	{
		LOG("Event: id = %d (%s)", static_cast<int>(e.event_id), mpv_event_name(e.event_id));
	}
	switch (e.event_id)
	{
	case MPV_EVENT_LOG_MESSAGE: {
		const mpv_event_log_message *msg = (const mpv_event_log_message *)e.data;
		LOG("%s", msg->text);
		break;
	}
	case MPV_EVENT_PROPERTY_CHANGE: {
		const mpv_event_property *prop = (mpv_event_property *)e.data;
		if (strcmp(prop->name, "time-pos") &&
			strcmp(prop->name, "video-bitrate") &&
			strcmp(prop->name, "audio-bitrate")
			) {
			LOG("Event: id = %d (%s: %s)", static_cast<int>(e.event_id), mpv_event_name(e.event_id), prop->name);
		}
        if (strcmp(prop->name, "media-title") == 0) {
            char *data = NULL;
			if (mpv_get_property(mpv, prop->name, MPV_FORMAT_OSD_STRING, &data) < 0) {
				//SetTitle("mpv");
			} else {
			#if 0
				wxString title = wxString::FromUTF8(data);
				if (!title.IsEmpty())
					title += " - ";
				title += "mpv";
				SetTitle(title);
			#endif
				mpv_free(data);
            }
		} else if (strcmp(prop->name, "video-bitrate") == 0) {
			double data = 0;
			if (mpv_get_property(mpv, prop->name, MPV_FORMAT_DOUBLE, &data) == 0) {
				if (mediaInfo.videoBitrateKnown == false) {
					mediaInfo.videoBitrateKnown = true;
					mediaInfo.videoBitrate = static_cast<int>(data);
					if (this->callbackMediaInfoUpdate)
					{
						callbackMediaInfoUpdate();
					}
				}
			}
		} else if (strcmp(prop->name, "audio-bitrate") == 0) {
			double data = 0;
			if (mpv_get_property(mpv, prop->name, MPV_FORMAT_DOUBLE, &data) == 0) {
				if (mediaInfo.audioBitrateKnown == false) {
					mediaInfo.audioBitrateKnown = true;
					mediaInfo.audioBitrate = static_cast<int>(data);
					if (this->callbackMediaInfoUpdate)
					{
						callbackMediaInfoUpdate();
					}
				}
			}
		} else if (strcmp(prop->name, "duration") == 0) {
			double data = 0;
			if (mpv_get_property(mpv, prop->name, MPV_FORMAT_DOUBLE, &data) == 0) {
				if (data > 0.0001) {
					fileLength = data;
					fileLengthValid = true;
					if (this->callbackMediaInfoUpdate)
					{
						callbackMediaInfoUpdate();
					}
				} else {
					fileLengthValid = false;
				}
			}
		} else if (strcmp(prop->name, "time-pos") == 0) {
			double data = 0;
			if (mpv_get_property(mpv, prop->name, MPV_FORMAT_DOUBLE, &data) == 0) {
				filePosition = data;
				filePositionValid = true;
			}
		} else if (strcmp(prop->name, "volume") == 0) {
			double data = 0;
			if (mpv_get_property(mpv, prop->name, MPV_FORMAT_DOUBLE, &data) == 0) {
				LOG("volume = %d", static_cast<int>(data));
			}
		}
        break;
	}
	case MPV_EVENT_START_FILE:
		break;
	case MPV_EVENT_FILE_LOADED:
		fileStarted = true;
		if (absoluteSeekRequested > 0) {
			seekAbsolute(absoluteSeekRequested);
		}
		break;
	case MPV_EVENT_END_FILE:
		fileStarted = false;
		if (callbackStopPlaying)
			callbackStopPlaying();	
		break;
    case MPV_EVENT_SHUTDOWN:
        MpvDestroy();
        break;
    default:
        break;
	}
}

int MPlayer::MpvCreate(void)
{
	MpvDestroy();
	
	mpv = mpv_create();
	if (!mpv) {
		LOG("failed to create mpv instance");
		return -1;
	}

	mpv_request_log_messages(mpv, "info");	

	int64_t wid = reinterpret_cast<int64_t>(parent);
	if (mpv_set_property(mpv, "wid", MPV_FORMAT_INT64, &wid) < 0) {
		LOG("failed to set mpv wid");
		MpvDestroy();
		return -2;
	}

	if (mpv_initialize(mpv) < 0) {
		LOG("failed to initialize mpv");
		MpvDestroy();
		return -3;
	}

	{
		int64_t val = 1;
		if (mpv_set_property(mpv, "osd-level", MPV_FORMAT_INT64, &val) < 0) {
			LOG("failed to set mpv osd level");
			return -2;
		}
	}

	{
		int val = 1;
		if (mpv_set_property(mpv, "osd-bar", MPV_FORMAT_FLAG, &val) < 0) {
			LOG("failed to set mpv osd-bar");
		} else {
        	LOG("mpv osd-bar enabled");
		}
	}

	mpv_observe_property(mpv, 0, "media-title", MPV_FORMAT_NONE);
	mpv_observe_property(mpv, 0, "video-bitrate", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "audio-bitrate", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "duration", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "volume", MPV_FORMAT_DOUBLE);

	return 0;
}

void MPlayer::MpvDestroy(void)
{
	if (mpv) {
		mpv_terminate_destroy(mpv);
		mpv = NULL;
	}
}

