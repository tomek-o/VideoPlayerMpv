//---------------------------------------------------------------------------

#ifndef ScEditH
#define ScEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <Scintilla.h> 
#include <SciLexer.h>
#include "ScEditStyle.h"

class TScEdit : public TWinControl
{
protected:
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall WndProc(Messages::TMessage &Message);
	void setCppStyle(int colorSchemeId);
	void setJsonStyle(int colorSchemeId);
	void setLuaStyle(void);
	void SetExtraStyle(void);
public:
	enum { MARGIN_SCRIPT_FOLD_INDEX = 1 };

	__fastcall TScEdit(Classes::TComponent* AOwner)
		:TWinControl(AOwner){;}

	sptr_t SendEditor(unsigned int iMessage, uptr_t wParam = 0, sptr_t lParam = 0)
	{
		return SendMessage(Handle, iMessage, wParam, lParam);
	}
	/** \note must be called BEFORE setStyle */
	void setFont(AnsiString name, int size);
	/** \note must be called AFTER setFont */
	void setStyle(enum ScEditStyle style, int colorSchemeId);
	void copy(void);
};


#endif
