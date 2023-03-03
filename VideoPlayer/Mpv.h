//---------------------------------------------------------------------------

#ifndef MpvH
#define MpvH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <Graphics.hpp>

struct mpv_handle;
struct mpv_event;

namespace System
{
	class TObject;
}

namespace Extctrls
{
	class TTimer;
}

class MPlayer
{
private:
	mpv_handle *mpv;
	HANDLE parent;
	Extctrls::TTimer *timer;	
	int process_priority;

	AnsiString filename;
	bool filePositionValid;
	double filePosition;
	bool fileLengthValid;
	double fileLength;

	int run(AnsiString cmdLine);

	typedef void (__closure *CallbackStopPlaying)(void);
	typedef void (__closure *CallbackMediaInfoUpdate)(void);

	void __fastcall timerTimer(System::TObject *Sender);
	void onMpvEvent(const mpv_event &e);
	int mpvCreate(void);
	void mpvDestroy(void);

public:
	MPlayer(void);
	~MPlayer();
	void setParent(HANDLE parent)
	{
    	this->parent = parent;
	}
	struct Cfg
	{
		int softVolLevel;
		int softVolMax;
	};
	int configure(const Cfg& cfg);
	const Cfg& getCfg(void)
	{
    	return cfg;
	}
	void __fastcall setCmdLine(AnsiString cmdline);
	void __fastcall lineReceived(AnsiString line);
	void __fastcall playerExited();

	int play(AnsiString filename, int softVolLevel, AnsiString extraParams);
	AnsiString getFilename(void) const
	{
    	return filename;
	}
	int frameStep(void);
	int pause(bool state);
	int seekRelative(int seconds);
	int seekAbsolute(double pos);
	int setOsdLevel(int level);
	int changeVolume(int delta);
	int changeVolumeAbs(int val);
	int osdShowText(AnsiString text, int duration);
	int stop(bool useCallback = true);

	void onStopPlayingFn(void);
	CallbackStopPlaying callbackStopPlaying;
	CallbackMediaInfoUpdate callbackMediaInfoUpdate;

	struct MediaInfo
	{
		bool videoBitrateKnown;
		int videoBitrate;
		bool audioBitrateKnown;
		int audioBitrate;
		MediaInfo(void):
			videoBitrateKnown(false),
			videoBitrate(0),
			audioBitrateKnown(false),
			audioBitrate(0)
		{}
	} mediaInfo;

	double getFilePosition(void) const {
		if (filePositionValid == false)
			return -1;
		return filePosition;
	}

	double getFileLength(void) const {
		if (fileLengthValid == false)
			return -1;
		return fileLength;
	}

private:
	Cfg cfg;
	bool fileStarted;
	double absoluteSeekRequested;
};

extern class MPlayer mplayer;

#endif
