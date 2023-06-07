//---------------------------------------------------------------------------

#ifndef HotkeyCfgPanelH
#define HotkeyCfgPanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

struct HotKeyConf;

class PACKAGE THotkeyCfgPanel : public TPanel
{
private:
	HotKeyConf &cfg;
	void __fastcall UpdateCfg(TObject *Sender);
	void ChangeActionType(void);	
protected:
	void __fastcall btnBrowseClick(TObject *Sender);
	void __fastcall btnEditClick(TObject *Sender);
public:
	__fastcall THotkeyCfgPanel(TComponent* Owner, HotKeyConf &cfg);
	void Start(void);
__published:
	TCheckBox *chbShift;
	TCheckBox *chbAlt;
	TCheckBox *chbCtrl;
	TComboBox *cbKey;
	TComboBox *cbAction;
	//TComboBox *cbId;
	TCheckBox *chbGlobal;
	TButton	  *btnRemove;

	TEdit     *edFile;
	TButton   *btnBrowse;
	TButton   *btnEdit;
};
//---------------------------------------------------------------------------
#endif
