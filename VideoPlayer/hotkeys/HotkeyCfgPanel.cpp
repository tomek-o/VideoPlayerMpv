//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "HotkeyCfgPanel.h"
#include "HotKeyConf.h"
#include "common/KeybKeys.h"
//#include "Settings.h"
#include "FormLuaScript.h"
#pragma package(smart_init)

namespace {
	enum { ACTION_COMBOBOX_WIDTH_SCRIPT = 110 };
	enum { ACTION_COMBOBOX_WIDTH_REGULAR = 230 };
}	// namespace

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(THotkeyCfgPanel *)
{
	static HotKeyConf cfg;
	new THotkeyCfgPanel(NULL, cfg);
}
//---------------------------------------------------------------------------
__fastcall THotkeyCfgPanel::THotkeyCfgPanel(TComponent* Owner, HotKeyConf &cfg)
	: TPanel(Owner),
	cfg(cfg)
{
	Width = 596;
	Height = 32;

	int left = 16;

	chbGlobal = new TCheckBox(this);
	chbGlobal->Parent = this;
	chbGlobal->Left = left;
	chbGlobal->Top = 8;
	//chbGlobal->Caption = "Global";
	chbGlobal->Visible = true;

	left += 32;
	chbCtrl = new TCheckBox(this);
	chbCtrl->Parent = this;
	chbCtrl->Left = left;
	chbCtrl->Top = 8;
	//chbCtrl->Caption = "Ctrl";
	chbCtrl->Visible = true;

	left += 24;
	chbShift = new TCheckBox(this);
	chbShift->Parent = this;
	chbShift->Left = left;
	chbShift->Top = 8;
	//chbShift->Caption = "Shift";
	chbShift->Visible = true;

	left += 24;
	chbAlt = new TCheckBox(this);
	chbAlt->Parent = this;
	chbAlt->Left = left;
	chbAlt->Top = 8;
	//chbAlt->Caption = "Alt";
	chbAlt->Visible = true;

	left += 24;
	cbKey = new TComboBox(this);
	cbKey->Parent = this;
	cbKey->Left = left;
	cbKey->Top = 5;
	cbKey->Width = 160;
	cbKey->Style = Stdctrls::csDropDownList;
	cbKey->Visible = true;
	cbKey->DropDownCount = 12;

	left += cbKey->Width + 10;
	cbAction = new TComboBox(this);
	cbAction->Parent = this;
	cbAction->Left = left;
	cbAction->Top = 5;
	cbAction->Width = ACTION_COMBOBOX_WIDTH_SCRIPT;
	cbAction->Style = Stdctrls::csDropDownList;
	cbAction->Visible = true;
#if 0
	left += cbAction->Width + 20;
	cbId = new TComboBox(this);
	cbId->Parent = this;
	cbId->Left = left;
	cbId->Top = 5;
	cbId->Width = 40;
	cbId->Style = Stdctrls::csDropDownList;
	cbId->Visible = true;
	left += cbId->Width + 28;
#else
	left += cbAction->Width + 10;
#endif
	edFile = new TEdit(this);
	edFile->Parent = this;
	edFile->Left = left;
	edFile->Top = 5;
	edFile->Width = 80;
	edFile->Visible = false;

	left += edFile->Width + 6;
	btnBrowse = new TButton(this);
	btnBrowse->Parent = this;
	btnBrowse->Left = left;
	btnBrowse->Top = 5;
	btnBrowse->Width = 20;
	btnBrowse->Height = 20;
	btnBrowse->Caption = "...";
	btnBrowse->Visible = false;

	left += btnBrowse->Width + 6;
	btnEdit = new TButton(this);
	btnEdit->Parent = this;
	btnEdit->Left = left;
	btnEdit->Top = 5;
	btnEdit->Width = 28;
	btnEdit->Height = 20;
	btnEdit->Caption = "Edit";
	btnEdit->Visible = false;

	left += btnEdit->Width + 20;

	btnRemove = new TButton(this);
	btnRemove->Parent = this;
	btnRemove->Left = left;
	btnRemove->Top = 5;
	btnRemove->Width = 24;
	btnRemove->Height = 20;
	btnRemove->Caption = "X";
	btnRemove->Visible = true;
}

void __fastcall THotkeyCfgPanel::UpdateCfg(TObject *Sender)
{
	if (Sender == cbAction)	// do not update action type if not needed - TEdit losing focus for script name 
	{
		ChangeActionType();
	}

	cfg.modifiers = 0;
	if (chbCtrl->Checked)
		cfg.modifiers |= HotKeyConf::CTRL;
	if (chbShift->Checked)
		cfg.modifiers |= HotKeyConf::SHIFT;
	if (chbAlt->Checked)
		cfg.modifiers |= HotKeyConf::ALT;
	if (cbKey->ItemIndex >= 0)
		cfg.keyCode = vkey_list[cbKey->ItemIndex].name;
	else
		cfg.keyCode = "";
	int id = vkey_find(cfg.keyCode.c_str());
	if (id >= 0)
	{
		cfg.vkCode = vkey_list[id].vkey;
	}
	else
	{
    	cfg.vkCode = -1;
	}
	cfg.global = chbGlobal->Checked;
	cfg.action.type = static_cast<Action::Type>(cbAction->ItemIndex);
	//cfg.action.id = cbId->ItemIndex;
	cfg.action.file = edFile->Text;
}


void THotkeyCfgPanel::Start(void)
{
	chbCtrl->OnClick = NULL;
	chbShift->OnClick = NULL;
	chbAlt->OnClick = NULL;
	cbKey->OnChange = NULL;
	cbAction->OnChange = NULL;
	//cbId->OnChange = NULL;
	chbGlobal->OnClick = NULL;

	for (unsigned int i=0; i<vkey_list_size(); i++)
	{
    	cbKey->Items->Add(vkey_list[i].description);
	}
	for (int i=0; i<Action::TYPE_LIMITER; i++)
	{
    	cbAction->Items->Add(Action::getTypeDescription(static_cast<Action::Type>(i)));
	}
#if 0
	for (int i=0; i<(1+ProgrammableButtons::EXT_CONSOLE_COLUMNS)*(ProgrammableButtons::CONSOLE_BTNS_PER_COLUMN); i++)
	{
		cbId->Items->Add(i);
	}
#endif

	chbCtrl->Checked = cfg.modifiers & HotKeyConf::CTRL;
	chbShift->Checked = cfg.modifiers & HotKeyConf::SHIFT;
	chbAlt->Checked = cfg.modifiers & HotKeyConf::ALT;
	cbKey->ItemIndex = vkey_find(cfg.keyCode.c_str());	// can't do this in constructor
	cbAction->ItemIndex = cfg.action.type;
	ChangeActionType();
	//cbId->ItemIndex = cfg.action.id;
	edFile->Text = cfg.action.file;
	chbGlobal->Checked = cfg.global;

	chbCtrl->OnClick = UpdateCfg;
	chbShift->OnClick = UpdateCfg;
	chbAlt->OnClick = UpdateCfg;
	cbKey->OnChange = UpdateCfg;
	cbAction->OnChange = UpdateCfg;
	//cbId->OnChange = UpdateCfg;
	edFile->OnChange = UpdateCfg;
	chbGlobal->OnClick = UpdateCfg;
	btnBrowse->OnClick = btnBrowseClick;
	btnEdit->OnClick = btnEditClick;
}

void THotkeyCfgPanel::ChangeActionType(void)
{
	//cbId->Visible = false;
	edFile->Visible = false;
	btnBrowse->Visible = false;
	btnEdit->Visible = false;

	Action::Type type = static_cast<Action::Type>(cbAction->ItemIndex);
	switch (type)
	{
	case Action::TYPE_NONE:
		break;
	case Action::TYPE_SCRIPT:
		edFile->Visible = true;
		btnBrowse->Visible = true;
		btnEdit->Visible = true;
		cbAction->Width = ACTION_COMBOBOX_WIDTH_SCRIPT;
		break;
	default:
		break;
	}

	if (type != Action::TYPE_SCRIPT)
	{
		cbAction->Width = ACTION_COMBOBOX_WIDTH_REGULAR;
	}
}
void __fastcall THotkeyCfgPanel::btnBrowseClick(TObject *Sender)
{
	TOpenDialog *openDialog = new TOpenDialog(NULL);

	AnsiString dir = ExtractFileDir(Application->ExeName) + "\\scripts";
	ForceDirectories(dir);
	openDialog->InitialDir = dir;
	openDialog->Filter = "Lua files (*.lua)|*.lua|All files|*.*";
	AnsiString fileName = dir + "\\" + edFile->Text;
	if (FileExists(fileName))
		openDialog->FileName = fileName;
	else
		openDialog->FileName = "";
	if (openDialog->Execute())
	{
		if (UpperCase(dir) != UpperCase(ExtractFileDir(openDialog->FileName)))
		{
			MessageBox(this->Handle, "Entry was not updated.\nFor portability script files must be placed in \"script\" subdirectory.", this->Caption.c_str(), MB_ICONEXCLAMATION);
		}
		else
		{
			edFile->Text = ExtractFileName(openDialog->FileName);
		}
	}

	delete openDialog;
}

void __fastcall THotkeyCfgPanel::btnEditClick(TObject *Sender)
{
	AnsiString dir = ExtractFileDir(Application->ExeName) + "\\scripts";
	ForceDirectories(dir);
	if (edFile->Text == "")
	{
		edFile->Text = AnsiString("hotkey_") + FormatDateTime("yyyymmdd_hhnnss_zzz", Now()) + ".lua";
	}
	AnsiString fileName = dir + "\\" + edFile->Text;
	TfrmLuaScript *frmLuaScript = new TfrmLuaScript(NULL);
	frmLuaScript->OpenFile(fileName);
	frmLuaScript->ShowModal();
}

//---------------------------------------------------------------------------
namespace Hotkeycfgpanel
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(THotkeyCfgPanel)};
		RegisterComponents("Samples", classes, 0);
	}
}
//---------------------------------------------------------------------------
