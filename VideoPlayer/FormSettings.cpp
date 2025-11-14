//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSettings.h"
#include "FormHotkeys.h"
#include "LogUnit.h"
#include "FormAbout.h"
#include <SysUtils.hpp>
#include <FileCtrl.hpp>

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
	tmpSettings.Logging.bLogToFile = chbLogToFile->Checked;

	tmpSettings.Mplayer.softVolMax = StrToIntDef(edMplayerSoftVolMax->Text, 200);
	if (tmpSettings.Mplayer.softVolMax < 50 || tmpSettings.Mplayer.softVolMax > 1000)
		tmpSettings.Mplayer.softVolMax = 200;
	tmpSettings.Mplayer.useSeparateVolumeForEachFile = chbUseSeparateVolumeForEachFile->Checked;

	tmpSettings.Mplayer.showFileNameOnPlayStart = chbMplayerShowFileNameOnPlayStart->Checked;
	tmpSettings.Mplayer.showPropertyEditor = chbMpvShowPropertyEditor->Checked;

	*appSettings = tmpSettings;
	this->Close();	
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

