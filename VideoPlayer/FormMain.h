//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>

#include "Settings.h"
#include <vector>

struct Action;
struct HotkeyConf;
class TrayIcon;

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlControl;
	TPanel *pnlControlRight;
	TSpeedButton *btnClose;
	TSpeedButton *btnSettings;
	TSpeedButton *btnStop;
	TTimer *tmrShowControl;
	TSpeedButton *btnPlay;
	TTimer *tmrCursorHide;
	TSpeedButton *btnFullscreen;
	TSpeedButton *btnMinimize;
	TSpeedButton *btnPauseStill;
	TTimer *tmrRefreshControl;
	TLabel *lblV;
	TLabel *lblA;
	TPanel *pnlMain;
	TTimer *tmrAntirepeat;
	TTimer *tmrSavePlaylists;
	TPopupMenu *popupTray;
	TMenuItem *miExit;
	TMenuItem *miPlay;
	TMenuItem *miPause;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall actShowAboutExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall actShowLogExecute(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall btnSettingsClick(TObject *Sender);
	void __fastcall btnPlayClick(TObject *Sender);
	void __fastcall pnlDragWindow(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall pnlControlMouseLeave(TObject *Sender);
	void __fastcall pnlVideoMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall tmrShowControlTimer(TObject *Sender);
	void __fastcall btnStopClick(TObject *Sender);
	void __fastcall tmrCursorHideTimer(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
	void __fastcall btnFullscreenClick(TObject *Sender);
	void __fastcall btnMinimizeClick(TObject *Sender);
	void __fastcall btnPauseStillClick(TObject *Sender);
	void __fastcall tmrRefreshControlTimer(TObject *Sender);
	void __fastcall tmrAntirepeatTimer(TObject *Sender);
	void __fastcall tmrSavePlaylistsTimer(TObject *Sender);
	void __fastcall miExitClick(TObject *Sender);
	void __fastcall miPlayClick(TObject *Sender);
	void __fastcall miPauseClick(TObject *Sender);
private:	// User declarations
	TrayIcon *trIcon;
	void ApplySettings(const Settings &prev);
	bool allowControlHide;
	void ShowMediaBrowser(bool state);
	enum STATE
	{
		STOP = 0,
		PLAY,
		PAUSE
	} state;
	void SetState(enum STATE state);
	void Play(void);
	void Pause(void);
	void Stop(void);
	void Skip(void);
	void Prev(void);
	void ToggleFullscreen(void);
	void ToggleOsd(void);
	void ToggleSubVisibility(void);
	void ChangeVolume(int delta);
	void OpenFiles(std::vector<AnsiString> filenames);
	void WriteSettings(void);
	void __fastcall WMDropFiles(TWMDropFiles &message);
	int mouseMoveLastX, mouseMoveLastY;	// MouseMove is called 2x per second even if mouse is not moving - ?
	void UpdateFilePos(void);
	void OnAddOutputText(const char* text);
	void OnClearOutput(void);
	void RunScript(int srcType, int srcId, AnsiString filename, bool showLog);

	const HotKeyConf *lastHotkey;
	void RegisterGlobalHotKeys(void);
	void __fastcall WMHotKey(TWMHotKey &Message);
	void ExecAction(const struct Action& action);
	bool MousePosOverControlPanel(const TPoint &Position);
	int OnGetCurrentFileName(AnsiString &filename);
	void __fastcall OnTrayIconLeftBtnDown(TObject *Sender);
	void ToggleVisibility(void);
	void UpdateTrayIcon(void);			

protected:
	void __fastcall CreateParams(TCreateParams   &Params);
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
	__fastcall ~TfrmMain(void);
	void CallbackStartPlayingFn(void);
	void CallbackStopPlayingFn(void);
	void CallbackMediaInfoUpdateFn(void);

	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
		VCL_MESSAGE_HANDLER(WM_HOTKEY, TWMHotKey, WMHotKey)
	END_MESSAGE_MAP(TForm);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
