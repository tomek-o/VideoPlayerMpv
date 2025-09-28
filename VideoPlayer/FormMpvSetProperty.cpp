//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMpvSetProperty.h"
#include "Mpv.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMpvSetProperty *frmMpvSetProperty;
//---------------------------------------------------------------------------
__fastcall TfrmMpvSetProperty::TfrmMpvSetProperty(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmMpvSetProperty::btnSetClick(TObject *Sender)
{
	int status = mplayer.setProperty(edName->Text, edValue->Text);
	if (status)
		lblStatus->Caption = "Failed to set property";
	else
		lblStatus->Caption = "Property set";
}
//---------------------------------------------------------------------------
