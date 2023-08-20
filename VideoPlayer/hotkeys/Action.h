#ifndef ActionH
#define ActionH

#include <System.hpp>

struct Action
{
	enum Type
	{
		TYPE_NONE = 0,
		TYPE_PLAY_STOP,
		TYPE_PLAY_PAUSE,
		TYPE_SEEK_M3,
		TYPE_SEEK_P3,
		TYPE_SEEK_M60,
		TYPE_SEEK_P60,
		TYPE_TOGGLE_FULLSCREEN,
		TYPE_EXIT_FS_EXIT,
		TYPE_SHOW_FILE_INFO,
		TYPE_MINIMIZE,
		TYPE_TOGGLE_OSD,
		TYPE_SKIP,
		TYPE_PREV,
		TYPE_DELETE_FILE,
		TYPE_SHOW_LOG,
		TYPE_VOLUME_UP,
		TYPE_VOLUME_DOWN,
		TYPE_SCRIPT,				///< execute script
		TYPE_OPEN_SCRIPT_WINDOW,
		TYPE_TOGGLE_SUB_VISIBILITY,

		TYPE_LIMITER
	} type;
	int id;
	AnsiString file;			///< script file

	Action(void):
		type(TYPE_NONE),
		id(0)
	{
	}

	bool operator==(const Action& right) const {
		if (type != right.type ||
			id != right.id ||
			file != right.file
			)
		{
			return false;
		}
		return true;
	}

	bool operator!=(const Action& right) const {
    	return !(*this == right);
	}

	static const char* getTypeDescription(enum Type type);

};

#endif
