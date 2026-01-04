//---------------------------------------------------------------------------

#ifndef FormIntroOutroSkipH
#define FormIntroOutroSkipH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmIntroOutroSkip : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblIntroSkip;
	TComboBox *cbIntroSkip;
	TLabel *lblOutroSkip;
	TComboBox *cbOutroSkip;
	TPanel *pnlBottom;
	TButton *btnCancel;
	TButton *btnApply;
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmIntroOutroSkip(TComponent* Owner);
	void SetTimes(unsigned int intro, unsigned int outro);
	void GetTimes(unsigned int &intro, unsigned int &outro);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmIntroOutroSkip *frmIntroOutroSkip;
//---------------------------------------------------------------------------
#endif
