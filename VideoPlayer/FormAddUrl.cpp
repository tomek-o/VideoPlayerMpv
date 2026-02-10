//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormAddUrl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAddUrl *frmAddUrl;
//---------------------------------------------------------------------------
__fastcall TfrmAddUrl::TfrmAddUrl(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmAddUrl::FormShow(TObject *Sender)
{
	ModalResult = mrNone;
}
//---------------------------------------------------------------------------
