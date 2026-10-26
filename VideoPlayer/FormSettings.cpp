//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSettings.h"
#include "FormHotkeys.h"
#include "LogUnit.h"
#include "FormAbout.h"
#include <SysUtils.hpp>
#include <FileCtrl.hpp>
#include "common/Autostart.h"

namespace {

AnsiString GetAutostartValueName(void)
{
	return ChangeFileExt(ExtractFileName(Application->ExeName), "");
}

AnsiString GetAutostartCommand(void)
{
	return "\"" + Application->ExeName + "\"";
}

/** \brief Check if HKCU Run entry exists and points to this executable
*/
bool IsAutostartEnabled(void)
{
	return Autostart::IsEnabled(GetAutostartValueName(), GetAutostartCommand());
}

int SetAutostart(bool state)
{
	bool ok;
	if (state)
		ok = Autostart::Enable(GetAutostartValueName(), GetAutostartCommand());
	else
		ok = Autostart::Disable(GetAutostartValueName());
	return ok ? 0 : -1;
}

}	// namespace

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSettings *frmSettings;
//---------------------------------------------------------------------------
__fastcall TfrmSettings::TfrmSettings(TComponent* Owner)
	: TForm(Owner)
{
	this->appSettings = NULL;
	frmHotkeys = new TfrmHotkeys(tsHotkeys);
	frmHotkeys->Parent = tsHotkeys;
	frmHotkeys->Visible = true;

	pcSettings->ActivePage = tsMplayer;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::FormShow(TObject *Sender)
{
    assert(appSettings);
	tmpSettings = *appSettings;
	chbAlwaysOnTop->Checked = tmpSettings.frmMain.bAlwaysOnTop;
	chbExitFullscreenOnStop->Checked = tmpSettings.frmMain.bExitFullScreenOnStop;
	if (tmpSettings.frmMain.controlPanelPosition < cbControlPanelPosition->Items->Count)
	{
		cbControlPanelPosition->ItemIndex = tmpSettings.frmMain.controlPanelPosition;
	}
	chbIgnoreMouseMovementInFullScreenPlayback->Checked = tmpSettings.frmMain.ignoreMouseMovementInFullScreenPlayback;
	chbShowTrayIcon->Checked = tmpSettings.gui.showTrayIcon;
	chbStartMinimizedToTray->Checked = tmpSettings.gui.startMinimizedToTray;
	chbAutostart->Checked = IsAutostartEnabled();
	chbShowTrayBalloonOnPlayStart->Checked = tmpSettings.gui.showTrayBalloonOnPlayStart;
	chbIgnorePrevCommandWhenPlayingFirstItem->Checked = tmpSettings.frmMain.ignorePrevCommandWhenPlayingFirstItem;
	chbIgnoreNextCommandWhenPlayingLastItem->Checked = tmpSettings.frmMain.ignoreNextCommandWhenPlayingLastItem;
	chbPauseOnSuspend->Checked = tmpSettings.frmMain.pauseOnSuspend;
	chbExitFullscreenOnSuspend->Checked = tmpSettings.frmMain.exitFullScreenOnSuspend;
	chbResumePlaybackOnStart->Checked = tmpSettings.frmMain.resumePlaybackOnStart;
	chbLoopPlaylist->Checked = tmpSettings.frmMain.loopPlaylist;
	chbMouseWheelSeekWithoutShift->Checked = tmpSettings.frmMain.mouseWheelSeekWithoutShift;

	chbLogToFile->Checked = tmpSettings.Logging.bLogToFile;
	cmbMaxUiLogLines->ItemIndex = -1;
	for (int i=0; i<cmbMaxUiLogLines->Items->Count; i++)
	{
		if ((unsigned int)StrToInt(cmbMaxUiLogLines->Items->Strings[i]) >= tmpSettings.Logging.iMaxUiLogLines)
		{
			cmbMaxUiLogLines->ItemIndex = i;
			break;
		}
	}
	if (cmbMaxUiLogLines->ItemIndex == -1)
	{
		cmbMaxUiLogLines->ItemHeight = cmbMaxUiLogLines->Items->Count - 1;
	}


	edMplayerSoftVolMax->Text = tmpSettings.Mplayer.softVolMax;
	chbUseSeparateVolumeForEachFile->Checked = tmpSettings.Mplayer.useSeparateVolumeForEachFile;
	chbMplayerShowFileNameOnPlayStart->Checked = tmpSettings.Mplayer.showFileNameOnPlayStart;
	chbMpvShowPropertyEditor->Checked = tmpSettings.Mplayer.showPropertyEditor;
	pnlBackgroundColor->Color = static_cast<TColor>(tmpSettings.Mplayer.backgroundColor);

	frmHotkeys->SetCfg(&tmpSettings.hotKeyConf);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::btnCancelClick(TObject *Sender)
{
	this->Close();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::btnApplyClick(TObject *Sender)
{
	tmpSettings.frmMain.bExitFullScreenOnStop = chbExitFullscreenOnStop->Checked;
	tmpSettings.frmMain.controlPanelPosition = static_cast<Settings::_frmMain::ControlPanelPosition>(cbControlPanelPosition->ItemIndex);
	tmpSettings.frmMain.ignoreMouseMovementInFullScreenPlayback = chbIgnoreMouseMovementInFullScreenPlayback->Checked;
	tmpSettings.gui.showTrayIcon = chbShowTrayIcon->Checked;
	tmpSettings.gui.startMinimizedToTray = chbStartMinimizedToTray->Checked;
	tmpSettings.gui.showTrayBalloonOnPlayStart = chbShowTrayBalloonOnPlayStart->Checked;
	tmpSettings.frmMain.ignorePrevCommandWhenPlayingFirstItem = chbIgnorePrevCommandWhenPlayingFirstItem->Checked;
	tmpSettings.frmMain.ignoreNextCommandWhenPlayingLastItem = chbIgnoreNextCommandWhenPlayingLastItem->Checked;
	tmpSettings.frmMain.pauseOnSuspend = chbPauseOnSuspend->Checked;
	tmpSettings.frmMain.exitFullScreenOnSuspend = chbExitFullscreenOnSuspend->Checked;
	tmpSettings.frmMain.resumePlaybackOnStart = chbResumePlaybackOnStart->Checked;
	tmpSettings.frmMain.loopPlaylist = chbLoopPlaylist->Checked;
	tmpSettings.frmMain.mouseWheelSeekWithoutShift = chbMouseWheelSeekWithoutShift->Checked;

	tmpSettings.Logging.bLogToFile = chbLogToFile->Checked;

	tmpSettings.Mplayer.softVolMax = StrToIntDef(edMplayerSoftVolMax->Text, 200);
	if (tmpSettings.Mplayer.softVolMax < 50 || tmpSettings.Mplayer.softVolMax > 1000)
		tmpSettings.Mplayer.softVolMax = 200;
	tmpSettings.Mplayer.useSeparateVolumeForEachFile = chbUseSeparateVolumeForEachFile->Checked;

	tmpSettings.Mplayer.showFileNameOnPlayStart = chbMplayerShowFileNameOnPlayStart->Checked;
	tmpSettings.Mplayer.showPropertyEditor = chbMpvShowPropertyEditor->Checked;

	if (chbAutostart->Checked != IsAutostartEnabled())
	{
		if (SetAutostart(chbAutostart->Checked) != 0)
		{
			MessageBox(this->Handle, "Failed to update autostart setting in registry.",
				Application->Title.c_str(), MB_ICONEXCLAMATION);
		}
	}

	*appSettings = tmpSettings;
	this->Close();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::btnBackgroundColorClick(TObject *Sender)
{
	ColorDialog->Color = static_cast<TColor>(tmpSettings.Mplayer.backgroundColor);
	if (ColorDialog->Execute())
	{
		tmpSettings.Mplayer.backgroundColor = ColorToRGB(ColorDialog->Color) & 0xFFFFFF;
		pnlBackgroundColor->Color = static_cast<TColor>(tmpSettings.Mplayer.backgroundColor);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::btnBackgroundColorDefaultClick(TObject *Sender)
{
	tmpSettings.Mplayer.backgroundColor = Settings::_Mplayer::BACKGROUND_COLOR_DEF;
	pnlBackgroundColor->Color = static_cast<TColor>(tmpSettings.Mplayer.backgroundColor);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::chbAlwaysOnTopClick(TObject *Sender)
{
	tmpSettings.frmMain.bAlwaysOnTop = chbAlwaysOnTop->Checked;	
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::cmbMaxUiLogLinesChange(TObject *Sender)
{
	tmpSettings.Logging.iMaxUiLogLines = StrToInt(cmbMaxUiLogLines->Text);	
}
//---------------------------------------------------------------------------


void __fastcall TfrmSettings::btnShowLogWindowClick(TObject *Sender)
{
	frmLog->Show();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::btnAboutClick(TObject *Sender)
{
	frmAbout->ShowModal();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::FormKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_ESCAPE)
		Close();	
}
//---------------------------------------------------------------------------

