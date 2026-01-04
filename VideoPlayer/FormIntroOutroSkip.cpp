//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormIntroOutroSkip.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmIntroOutroSkip *frmIntroOutroSkip;
//---------------------------------------------------------------------------
__fastcall TfrmIntroOutroSkip::TfrmIntroOutroSkip(TComponent* Owner)
	: TForm(Owner)
{
	for (unsigned int i=0; i<=20; i++)
	{
		cbIntroSkip->Items->Add(i);
		cbOutroSkip->Items->Add(i);
	}
	for (unsigned int i=25; i<=90; i+=5)
	{
		cbIntroSkip->Items->Add(i);
		cbOutroSkip->Items->Add(i);
	}
}
//---------------------------------------------------------------------------

void TfrmIntroOutroSkip::SetTimes(unsigned int intro, unsigned int outro)
{
	cbIntroSkip->Text = intro;
	cbOutroSkip->Text = outro;
}

void TfrmIntroOutroSkip::GetTimes(unsigned int &intro, unsigned int &outro)
{
	intro = StrToIntDef(cbIntroSkip->Text, 0);
	outro = StrToIntDef(cbOutroSkip->Text, 0);
}

void __fastcall TfrmIntroOutroSkip::FormKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_ESCAPE)
		Close();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmIntroOutroSkip::FormShow(TObject *Sender)
{
	ModalResult = mrNone;	
}
//---------------------------------------------------------------------------
