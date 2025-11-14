//---------------------------------------------------------------------------

#ifndef FormSettingsH
#define FormSettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include "Settings.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>

class TfrmHotkeys;

class TfrmSettings : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlBottom;
	TButton *btnCancel;
	TButton *btnApply;
	TPageControl *pcSettings;
	TTabSheet *tsGeneral;
	TTabSheet *tsLogging;
	TCheckBox *chbAlwaysOnTop;
	TLabel *lblUiCapacity;
	TComboBox *cmbMaxUiLogLines;
	TCheckBox *chbLogToFile;
	TTabSheet *tsMplayer;
	TOpenDialog *OpenDialog;
	TButton *btnShowLogWindow;
	TCheckBox *chbExitFullscreenOnStop;
	TLabel *lblSoftVolMax;
	TEdit *edMplayerSoftVolMax;
	TLabel *lblMplayerPercent;
	TButton *btnAbout;
	TCheckBox *chbMplayerShowFileNameOnPlayStart;
	TTabSheet *tsHotkeys;
	TLabel *lblControlPanelPosition;
	TComboBox *cbControlPanelPosition;
	TCheckBox *chbUseSeparateVolumeForEachFile;
	TCheckBox *chbMpvShowPropertyEditor;
	TCheckBox *chbIgnoreMouseMovementInFullScreenPlayback;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnApplyClick(TObject *Sender);
	void __fastcall chbAlwaysOnTopClick(TObject *Sender);
	void __fastcall cmbMaxUiLogLinesChange(TObject *Sender);
	void __fastcall btnShowLogWindowClick(TObject *Sender);
	void __fastcall btnAboutClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
	TfrmHotkeys *frmHotkeys;

public:		// User declarations
	__fastcall TfrmSettings(TComponent* Owner);
	Settings *appSettings;
	Settings tmpSettings;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSettings *frmSettings;
//---------------------------------------------------------------------------
#endif
