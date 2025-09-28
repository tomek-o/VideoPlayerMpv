//---------------------------------------------------------------------------

#ifndef FormMpvSetPropertyH
#define FormMpvSetPropertyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmMpvSetProperty : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblName;
	TEdit *edName;
	TLabel *lblValue;
	TEdit *edValue;
	TButton *btnSet;
	TLabel *lblStatus;
	TLabel *lblExamples;
	TComboBox *cbExamples;
	TButton *btnLoadExample;
	void __fastcall btnSetClick(TObject *Sender);
	void __fastcall btnLoadExampleClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmMpvSetProperty(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMpvSetProperty *frmMpvSetProperty;
//---------------------------------------------------------------------------
#endif
