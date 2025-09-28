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

namespace
{

struct Example
{
	const char* description;
	const char* name;
	const char* value;
} examples[] =
{
	{ "Remove black bars", "vf", "crop=in_w:in_w/2.4" },
};

}


__fastcall TfrmMpvSetProperty::TfrmMpvSetProperty(TComponent* Owner)
	: TForm(Owner)
{
	for (unsigned int i=0; i<sizeof(examples)/sizeof(examples[0]); i++)
	{
		const Example& example = examples[i];
		cbExamples->Items->Add(example.description);
	}
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
void __fastcall TfrmMpvSetProperty::btnLoadExampleClick(TObject *Sender)
{
	int index = cbExamples->ItemIndex;
	if (index < 0)
		return;
	const Example& example = examples[index];
	edName->Text = example.name;
	edValue->Text = example.value;	
}
//---------------------------------------------------------------------------
