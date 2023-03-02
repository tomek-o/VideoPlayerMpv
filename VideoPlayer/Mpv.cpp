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
	callbackMediaInfoUpdate(NULL)
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

	mediaInfo.videoBitrate = mediaInfo.audioBitrateKnown = false;
	filePositionValid = false;
	fileLengthValid = false;
	if (this->callbackMediaInfoUpdate)
	{
    	callbackMediaInfoUpdate();
	}
#if 0
	cmdLine.cat_printf("\"%s\" ", cfg.asInstance.c_str());
	cmdLine.cat_printf("-slave -identify -colorkey 0x%x -quiet -keepaspect ", cfg.colorkey);
	if (cfg.softVol)
	{
		int db;
		if (softVolLevel > 0)
		{
        	cfg.softVolLevel = softVolLevel;
		}

		if (cfg.softVolLevel <= 0)
		{
			db = -120;
		}
		else
		{
			db = (int)(20.0f * log10((float)cfg.softVolLevel*cfg.softVolMax/100.0f/100.0f));
		}
		cmdLine.cat_printf("-softvol -af volume=%d -softvol-max %u ", db, cfg.softVolMax);
	}
	//cmdLine.cat_printf("-framedrop -nofs -noterm-osd -idx -hr-mp3-seek -nobps -ni ");
	//cmdLine.cat_printf("-wid %d ", (int)cfg.parent);
	if (cfg.asExtraParams != "")
	{
		cmdLine.cat_printf("%s ", cfg.asExtraParams.c_str());
	}
	if (extraParams != "")
	{
		cmdLine.cat_printf("%s ", extraParams.c_str());
	}

	cmdLine.cat_printf("\"%s\"", filename.c_str());
	LOG("******************************\nRun: %s", cmdLine.c_str());
	int status = run(cmdLine);
	if (status)
	{
		if (callbackStopPlaying)
			callbackStopPlaying();
	}
#endif

	{
		int64_t val = cfg.softVolMax;
		if (mpv_set_property(mpv, "volume-max", MPV_FORMAT_INT64, &val) < 0) {
			LOG("failed to set mpv volume-max");
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
	if (fileLengthValid)
	{
		if (seconds > fileLength)
		{
			return -1;
		}
	}
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

	AnsiString msg;
	if (text.Pos("'") != 0)
	{
		// not sure how to escape this...
		return -1;
	}
	AnsiString durationStr = duration;
	const char* cmd[] = { "show-text", text.c_str(), durationStr.c_str(), "0", NULL };
	return mpv_command(mpv, cmd);
}

int MPlayer::stop(bool useCallback)
{
	if (mpv == NULL)
		return -1;
	LOG("Stopping");
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
    case MPV_EVENT_VIDEO_RECONFIG:
        // something like --autofit-larger=95%
		//Autofit(95, true, false);
		break;
	case MPV_EVENT_LOG_MESSAGE: {
		const mpv_event_log_message *msg = (const mpv_event_log_message *)e.data;
		LOG("%s", msg->text);
		break;
	}
	case MPV_EVENT_PROPERTY_CHANGE: {
		const mpv_event_property *prop = (mpv_event_property *)e.data;
		if (strcmp(prop->name, "time-pos")) {
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
		}
        break;
	}
	case MPV_EVENT_END_FILE:
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

	mpv_request_log_messages(mpv, "warn");	

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

	mpv_observe_property(mpv, 0, "media-title", MPV_FORMAT_NONE);
	mpv_observe_property(mpv, 0, "video-bitrate", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "audio-bitrate", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "duration", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);

	return 0;
}

void MPlayer::MpvDestroy(void)
{
	if (mpv) {
		mpv_terminate_destroy(mpv);
		mpv = NULL;
	}
}

