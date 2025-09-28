//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("hotkeys\FormHotkeys.cpp", frmHotkeys);
USEFORM("scripting\FormLuaScript.cpp", frmLuaScript);
USEFORM("FormMain.cpp", frmMain);
USEFORM("FormSettings.cpp", frmSettings);
USEFORM("FormAbout.cpp", frmAbout);
USEFORM("LogUnit.cpp", frmLog);
USEFORM("FormMediaBrowser.cpp", frmMediaBrowser);
USEFORM("FormPlaylist.cpp", frmPlaylist);
USEFORM("FormTextEditor.cpp", frmTextEditor);
USEFORM("scripting\FormLuaScriptHelp.cpp", frmLuaScriptHelp);
USEFORM("FormMpvSetProperty.cpp", frmMpvSetProperty);
//---------------------------------------------------------------------------
#pragma link "jsoncpp.lib"
#pragma link "common.lib"
#pragma link "libmpv-2.lib"
#pragma link "lua_static.lib"
#pragma link "scintilla.lib"

WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "Video Player";
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TfrmLog), &frmLog);
		Application->CreateForm(__classid(TfrmSettings), &frmSettings);
		Application->CreateForm(__classid(TfrmAbout), &frmAbout);
		Application->CreateForm(__classid(TfrmMpvSetProperty), &frmMpvSetProperty);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
