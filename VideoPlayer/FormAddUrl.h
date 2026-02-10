//---------------------------------------------------------------------------

#ifndef FormAddUrlH
#define FormAddUrlH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmAddUrl : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlBottom;
	TButton *btnCancel;
	TButton *btnApply;
	TLabel *lblUrl;
	TEdit *edUrl;
	TLabel *lblName;
	TEdit *edName;
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAddUrl(TComponent* Owner);
	AnsiString getUrl(void) {
		return edUrl->Text.Trim();
	}
	AnsiString getName(void) {
		return edName->Text.Trim();
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddUrl *frmAddUrl;
//---------------------------------------------------------------------------
#endif
