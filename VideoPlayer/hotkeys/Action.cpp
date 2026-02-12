#include "Action.h"

const char* Action::getTypeDescription(enum Action::Type type)
{
	switch (type)
	{
	case TYPE_NONE:
		return "none";
	case TYPE_STOP:
		return "stop";
	case TYPE_PLAY_PAUSE:
		return "play/pause";
	case TYPE_SEEK_M3:
		return "seek -3 s";
	case TYPE_SEEK_P3:
		return "seek +3 s";
	case TYPE_SEEK_M60:
		return "seek -60 s";
	case TYPE_SEEK_P60:
		return "seek +60 s";
	case TYPE_TOGGLE_FULLSCREEN:
		return "toggle fullscreen";
	case TYPE_EXIT_FS_EXIT:
		return "exit fullscreen / exit";
	case TYPE_SHOW_FILE_INFO:
		return "show file info";
	case TYPE_MINIMIZE:
		return "minimize";
	case TYPE_TOGGLE_OSD:
		return "toggle OSD";
	case TYPE_SKIP:
		return "skip (next)";
	case TYPE_PREV:
		return "previous";
	case TYPE_DELETE_FILE:
		return "delete file";
	case TYPE_SHOW_LOG:
		return "show log";
	case TYPE_VOLUME_UP:
		return "volume up";
	case TYPE_VOLUME_DOWN:
		return "volume down";
	case TYPE_SCRIPT:
		return "script";
	case TYPE_OPEN_SCRIPT_WINDOW:
		return "open script window";
	case TYPE_TOGGLE_SUB_VISIBILITY:
		return "toggle subtitles visibility";
	case TYPE_PAUSE_FRAME_STEP:
		return "pause / frame step";
	default:
		return "???";
	}
}

