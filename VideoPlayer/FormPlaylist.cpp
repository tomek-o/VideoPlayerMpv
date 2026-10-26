//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormPlaylist.h"
#include "FormIntroOutroSkip.h"
#include "FormAddUrl.h"
#include "Log.h"
#include "Settings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmPlaylist::TfrmPlaylist(TComponent* Owner, AnsiString fileName)
	: TForm(Owner),
	callbackStartPlaying(NULL),
	callbackGetPlaylists(NULL),
	fileName(fileName),
	fileLoaded(false)
{
	playlist.setBaseDir(ExtractFileDir(fileName));
}
//---------------------------------------------------------------------------

int TfrmPlaylist::load(void)
{
	if (fileLoaded)
		return 0;
	int rc = playlist.loadFromFile(this->fileName);
	edFilter->Text = playlist.getFilterText();
	update();
	int position = playlist.getPosition();
	int count = lvPlaylist->Items->Count;
	if (position >= 0 && position < count)
	{
		lvPlaylist->Items->Item[position]->Selected = true;
		//lvPlaylist->Selected->MakeVisible(true);
		lvPlaylist->Selected->MakeVisible(false);
	}
	fileLoaded = true;
	return rc;
}

int TfrmPlaylist::saveToFile(AnsiString fileName)
{
	if (fileName != "")
		this->fileName = fileName;
	if (lvPlaylist->Selected)
	{
		playlist.setPosition(lvPlaylist->Selected->Index);
	}
	else
	{
    	playlist.setPosition(-1);
	}
	return playlist.saveToFile(this->fileName);
}

int TfrmPlaylist::deletePlaylistFile(void)
{
	if (fileName == "")
		return -1;
	if (FileExists(fileName))
	{
		if (DeleteFile(fileName) == false)
			return -2;
	}
	return 0;
}


void __fastcall TfrmPlaylist::miAddFilesClick(TObject *Sender)
{
	OpenDialog->Filter = "Video files|*.mp4;*.mkv;*.avi;*.mpg;*.wmv;*.flv;*.mov;*.rmvb;*.webm;*.3gp;*.asf|All files|*.*";
	OpenDialog->FilterIndex = 0;
	AnsiString dir = getLastFileDir();
	if (dir != "" && DirectoryExists(dir))
	{
		OpenDialog->InitialDir = dir;
	}
	if (OpenDialog->Execute())
	{
		std::vector<AnsiString> fileNames;
		for (int i=0; i<OpenDialog->Files->Count; i++)
		{
			fileNames.push_back(OpenDialog->Files->Strings[i]);
		}
		playlist.add(fileNames);
		update();
	}
}
//---------------------------------------------------------------------------

void TfrmPlaylist::update(void)
{
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	//LOG("%p update: %u filtered entries (%p)", this, entries.size(), &entries);
	lvPlaylist->Items->Count = entries.size();
	lvPlaylist->Invalidate(); 
}

void __fastcall TfrmPlaylist::lvPlaylistData(TObject *Sender, TListItem *Item)
{
	int id = Item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[id];

	if (entry.entry.mark)
	{
		Item->ImageIndex = 2;
	}
	else
	{
		Item->ImageIndex = -1;
	}

	if (entry.entry.fileName != "")
	{
		Item->SubItems->Add(ExtractFileName(entry.entry.fileName));
		AnsiString asSize;
		asSize.sprintf("%.1f MB", static_cast<double>(entry.entry.size) / (1024*1024));
		Item->SubItems->Add(asSize);
		if (entry.entry.length < 0)
		{
			Item->SubItems->Add("");
		}
		else if (entry.entry.length > 0.1)
		{
			double length = entry.entry.length;
			int hours = static_cast<int>(length / 3600);
			length -= hours * 3600;
			int minutes = static_cast<int>(length / 60);
			length -= minutes * 60;
			int seconds = static_cast<int>(length);
			AnsiString text;
			text.sprintf("%d:%02d:%02d", hours, minutes, seconds);
			Item->SubItems->Add(text);
		}
		else
		{
			Item->SubItems->Add("?");
		}
		Item->SubItems->Add(entry.entry.timeStamp);
		if (entry.entry.bitrateVideoMin != PlaylistEntry::BITRATE_DEFAULT)
		{
			if (entry.entry.bitrateVideoMin == entry.entry.bitrateVideoMax)
			{
				Item->SubItems->Add(entry.entry.bitrateVideoMin);
			}
			else
			{
				AnsiString str;
				str.sprintf("%d ... %d", entry.entry.bitrateVideoMin, entry.entry.bitrateVideoMax);
				Item->SubItems->Add(str);
			}
		}
		else
		{
			Item->SubItems->Add("");
		}
		if (entry.entry.bitrateAudioMin != PlaylistEntry::BITRATE_DEFAULT)
		{
			if (entry.entry.bitrateAudioMin == entry.entry.bitrateAudioMax)
			{
				Item->SubItems->Add(entry.entry.bitrateAudioMin);
			}
			else
			{
				AnsiString str;
				str.sprintf("%d ... %d", entry.entry.bitrateAudioMin, entry.entry.bitrateAudioMax);
				Item->SubItems->Add(str);
			}
		}
		else
		{
			Item->SubItems->Add("");
		}
		// playback progress
		if (entry.entry.length > 0)
		{
			AnsiString text;
			double percents = 100.0 * entry.entry.playbackProgress / entry.entry.length;
			if (entry.entry.playbackProgress > 0 && entry.entry.playbackProgress + 3 >= entry.entry.length)
				percents = 100.0;
			text.sprintf("%d%%", static_cast<int>(percents + 0.5));
			Item->SubItems->Add(text);
		}
		else
		{
			Item->SubItems->Add("");
		}
	}
	else
	{
		if (entry.entry.name != "")
			Item->SubItems->Add(entry.entry.name);
		else
			Item->SubItems->Add(entry.entry.url);
	}
}
//---------------------------------------------------------------------------

const PlaylistEntry* TfrmPlaylist::getFileToPlay(void)
{
	int id = -1;
	if (lvPlaylist->Selected)
	{
		id = lvPlaylist->Selected->Index;
	}
	else
	{
		if (lvPlaylist->Items->Count)
			id = 0;
	}
	if (id >= 0)
	{
		const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
        assert(static_cast<int>(entries.size()) > id);
		int realId = entries[id].id;
		// return entry from basic, unfiltered container as this have longer life span
		// entry from filtered list might be deleted when re-filtering after bitrate info is received from played file
		return &playlist.getEntry(realId);
	}
	else
	{
		return NULL;
	}
}

AnsiString TfrmPlaylist::getLastFileDir(void)
{
	// directory of last file entry (skipping URLs), relative path resolved
	const std::vector<PlaylistEntry>& entries = playlist.getEntries();
	for (int i=static_cast<int>(entries.size())-1; i>=0; i--)
	{
		if (entries[i].fileName != "")
			return ExtractFileDir(ExpandFileName(playlist.getFullPath(entries[i].fileName)));
	}
	return "";
}

int TfrmPlaylist::getFilteredIndex(unsigned int id)
{
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	for (unsigned int i=0; i<entries.size(); i++)
	{
		if (entries[i].id == id)
			return i;
	}
	return -1;
}

void TfrmPlaylist::selectEntry(unsigned int id)
{
	// id is index in unfiltered list - find its row in filtered view
	int row = getFilteredIndex(id);
	if (row < 0 && playlist.getFilterText() != "")
	{
		// entry hidden by filter - clear it directly, not relying on edFilter OnChange
		// (window might be not created yet, e.g. file from command line when starting minimized)
		edFilter->Text = "";
		playlist.filter("");
		update();
		row = getFilteredIndex(id);
	}
	if (row >= 0 && row < lvPlaylist->Items->Count)
	{
		lvPlaylist->Items->Item[row]->Selected = true;
	}
}

void TfrmPlaylist::setFiles(const std::vector<AnsiString>& filenames, bool switchTo)
{
	if (switchTo)
	{
		for (int i=0; i<lvPlaylist->Items->Count; i++)
		{
			lvPlaylist->Items->Item[i]->Selected = false;
		}
		int id = -1;
		if (filenames.size() == 1)
		{
			id = playlist.findByName(filenames[0]);
		}
		if (id < 0 && !filenames.empty())
		{
			// new entries are appended - select first of them
			id = playlist.getEntries().size();
			playlist.add(filenames);
			update();
		}
		if (id >= 0)
		{
			selectEntry(id);
		}
	}
	else
	{
		playlist.add(filenames);
		update();
	}
}

void __fastcall TfrmPlaylist::lvPlaylistKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if (Key == VK_DELETE)
	{
		if (!lvPlaylist->Selected)
			return;
		std::set<unsigned int> ids;
		const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
		for (int i=0; i<lvPlaylist->Items->Count; i++)
		{
			if (lvPlaylist->Items->Item[i]->Selected)
			{
				ids.insert(entries[i].id);
			}
		}
		playlist.remove(ids);
		update();
		lvPlaylist->ClearSelection();
	}
	else if (Key == VK_RETURN)
	{
    	play();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::edFilterChange(TObject *Sender)
{
	AnsiString text = edFilter->Text;
	playlist.filter(text);
	update();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::btnFilterClearClick(TObject *Sender)
{
	edFilter->Text = "";	
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::lvPlaylistDblClick(TObject *Sender)
{
	play();
}
//---------------------------------------------------------------------------

void TfrmPlaylist::play(void)
{
	assert(callbackStartPlaying);
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;
	item->MakeVisible(false);
	callbackStartPlaying();
}

bool TfrmPlaylist::hasNextFile(void)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return false;
	int id = item->Index;
	if (id < lvPlaylist->Items->Count - 1)
	{
		return true;
	}
	return appSettings.frmMain.loopPlaylist;
}

int TfrmPlaylist::playNextFile(void)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return -2;
	int id = item->Index;
	if (id < lvPlaylist->Items->Count - 1)
	{
		item->Selected = false;
		id++;
		TListItem *nextItem = lvPlaylist->Items->Item[id];
		nextItem->Selected = true;
		play();
		return 0;
	}
	else if (appSettings.frmMain.loopPlaylist)
	{
		item->Selected = false;
		TListItem *nextItem = lvPlaylist->Items->Item[0];
		nextItem->Selected = true;
		nextItem->MakeVisible(false);
		play();
		return 0;
	}
	return -3;
}

bool TfrmPlaylist::hasPrevFile(void)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return false;
	int id = item->Index;
	if (id > 0)
	{
		return true;
	}
	return appSettings.frmMain.loopPlaylist;
}

int TfrmPlaylist::playPrevFile(void)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return -2;
	int id = item->Index;
	if (id > 0)
	{
		item->Selected = false;
		id--;
		TListItem *nextItem = lvPlaylist->Items->Item[id];
		nextItem->Selected = true;
		play();
		return 0;
	}
	else if (appSettings.frmMain.loopPlaylist)
	{
		item->Selected = false;
		TListItem *nextItem = lvPlaylist->Items->Item[lvPlaylist->Items->Count - 1];
		nextItem->Selected = true;
		nextItem->MakeVisible(false);
		play();
		return 0;
	}
	return -3;
}

int TfrmPlaylist::renamePlaylistFile(AnsiString newName)
{
	if (FileExists(fileName))	// playlist might be not saved yet
	{
		if (RenameFile(fileName, newName) == false)
		{
			return -1;
		}
	}
	fileName = newName;
	playlist.setBaseDir(ExtractFileDir(fileName));
	return 0;
}

void __fastcall TfrmPlaylist::miRenameFileClick(TObject *Sender)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;

	item->MakeVisible(false);

	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[id];

	if (entry.entry.fileName == "")
		return;	// URL entry - nothing to rename
	// copy - entry reference might be invalidated by playback updates while dialog is shown
	AnsiString fileName = entry.entry.fileName;
	unsigned int entryId = entry.id;

	AnsiString name = ExtractFileName(fileName);
	bool ret = InputQuery("File name", "Name for the file", name);
	if (ret == false || name == ExtractFileName(fileName))
		return;

	// keep relative/absolute form; relative entry without directory part stays without it
	AnsiString dir = ExtractFileDir(fileName);
	AnsiString newFileName = (dir == "") ? name : (dir + "\\" + name);
	int status = playlist.rename(entryId, newFileName);
	if (status != 0)
	{
		MessageBox(this->Handle, "Failed to rename file.", Application->Title.c_str(), MB_ICONEXCLAMATION);
		return;
	}

	update();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miDeleteFilesClick(TObject *Sender)
{
	if (!lvPlaylist->Selected)
		return;
	std::set<unsigned int> ids;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	for (int i=0; i<lvPlaylist->Items->Count; i++)
	{
		if (lvPlaylist->Items->Item[i]->Selected)
		{
			ids.insert(entries[i].id);
		}
	}
	AnsiString msg;
	if (ids.size() == 1)
	{
		const PlaylistEntry& entry = playlist.getEntry(*ids.begin());
		msg.sprintf("Delete %s?", entry.fileName.c_str());
	}
	else
	{
		msg.sprintf("Delete %u files?", ids.size());
	}

	if (MessageBox(this->Handle, msg.c_str(),
		Application->Title.c_str(), MB_YESNO | MB_DEFBUTTON2 | MB_ICONEXCLAMATION) != IDYES)
	{
		return;
	}

	playlist.removeWithFiles(ids);
	update();
	lvPlaylist->ClearSelection();
}
//---------------------------------------------------------------------------

void TfrmPlaylist::deleteFile(AnsiString filename)
{
	std::set<unsigned int> ids;
	const std::vector<PlaylistEntry>& entries = playlist.getEntries();
	for (unsigned int i=0; i<entries.size(); i++)
	{
		if (entries[i].fileName == filename || playlist.getFullPath(entries[i].fileName) == filename)
			ids.insert(i);
	}
	playlist.removeWithFiles(ids);
	update();
	//lvPlaylist->ClearSelection();
}

void __fastcall TfrmPlaylist::lvPlaylistColumnClick(TObject *Sender,
      TListColumn *Column)
{
	bool ascending = true;
	if (Column->ImageIndex == 0)
	{
		ascending = false;
	}

	for (int i=0; i<lvPlaylist->Columns->Count; i++)
	{
    	lvPlaylist->Column[i]->ImageIndex = -1;
	}

	if (ascending)
	{
		Column->ImageIndex = 0;
	}
	else
	{
		Column->ImageIndex = 1;
	}

	enum Playlist::SortType sortType;
	switch (Column->Index)
	{
	case 0:
		sortType = Playlist::SortByMark;
		break;
	case 1:
		sortType = Playlist::SortByFileName;
		break;
	case 2:
		sortType = Playlist::SortBySize;
		break;
	case 3:
		sortType = Playlist::SortByLength;
		break;
	case 4:
		sortType = Playlist::SortByTimeStamp;
		break;
	case 5:
		sortType = Playlist::SortByBitrateVideo;
		break;
	case 6:
		sortType = Playlist::SortByBitrateAudio;
		break;
	case 7:
		sortType = Playlist::SortByPlaybackProgress;
		break;
	default:
		assert(!"Unhandled sort column!");
		return;
	}

	playlist.sort(sortType, ascending);

	update();
	lvPlaylist->ClearSelection();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miShufflePlaylistClick(TObject *Sender)
{
	playlist.sort(Playlist::SortRandom, false);
	update();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miRemoveDuplicatesClick(TObject *Sender)
{
	playlist.removeDuplicates();
	update();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miRemoveMissingFromListClick(TObject *Sender)
{
	playlist.removeMissingFiles();
	update();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miMarkSetClick(TObject *Sender)
{
	if (!lvPlaylist->Selected)
		return;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	for (int i=0; i<lvPlaylist->Items->Count; i++)
	{
		if (lvPlaylist->Items->Item[i]->Selected)
		{
			playlist.markSet(entries[i].id);
		}
	}
	update();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miMarkRemoveClick(TObject *Sender)
{
	if (!lvPlaylist->Selected)
		return;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	for (int i=0; i<lvPlaylist->Items->Count; i++)
	{
		if (lvPlaylist->Items->Item[i]->Selected)
		{
			playlist.markClear(entries[i].id);
		}
	}
	update();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miMarkDuplicatesBySizeClick(TObject *Sender)
{
	playlist.markDuplicatesBySize();
	update();
}
//---------------------------------------------------------------------------

void TfrmPlaylist::setFileLength(double length)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;
	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[id];
    playlist.setLength(entry.id, length);
	update();
}

void TfrmPlaylist::setFileBitrateVideo(int val)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;
	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[id];
	playlist.setBitrateVideo(entry.id, val);
	update();
}

void TfrmPlaylist::setFileBitrateAudio(int val)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;
	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[id];
    playlist.setBitrateAudio(entry.id, val);
	update();
}

void TfrmPlaylist::setFilePosition(double position)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;
	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[id];
	playlist.setFilePos(entry.id, position);
	update();
}

void TfrmPlaylist::setFileSoftVol(int val)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;
	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[id];
	playlist.setFileSoftVol(entry.id, id, val);
}

double TfrmPlaylist::getFilePosition(AnsiString file) const {
	return playlist.getFilePos(file);
}

void __fastcall TfrmPlaylist::miGoToFileClick(TObject *Sender)
{
	if (!lvPlaylist->Selected)
		return;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[lvPlaylist->Selected->Index];

	AnsiString params;
	params.sprintf("/e, /select, %s", playlist.getFullPath(entry.entry.fileName).c_str());
	ShellExecute(NULL, "open", "explorer.exe", params.c_str(), NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miSetMplayerExtraParamsClick(TObject *Sender)
{
	if (!lvPlaylist->Selected)
		return;

	AnsiString params;
	{
		const FilteredPlaylistEntry& fentry = playlist.getFilteredEntries()[lvPlaylist->Selected->Index];
		params = fentry.entry.mplayerExtraParams;
		bool ret = InputQuery("Extra parameters", "Extra parameters for mplayer", params);
		if (ret == false)
			return;
	}

	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	for (int i=0; i<lvPlaylist->Items->Count; i++)
	{
		if (lvPlaylist->Items->Item[i]->Selected)
		{
			playlist.setMplayerExtraParams(entries[i].id, params);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miAddRemainingFilesFromFolderClick(
      TObject *Sender)
{
	const std::vector<PlaylistEntry>& entries = playlist.getEntries();
	if (entries.empty())
	{
		MessageBox(this->Handle, "Playlist is empty - cannot determine associated folder.", Application->Title.c_str(), MB_ICONWARNING);
		return;
	}

	AnsiString dir = getLastFileDir();
	if (dir == "" || DirectoryExists(dir) == false)
	{
		MessageBox(this->Handle, "Directory associated with last playlist entry does not exist.", Application->Title.c_str(), MB_ICONWARNING);
		return;
	}

	// entries might be stored as relative paths - compare resolved, case-insensitive paths
	std::set<AnsiString> playlistSet;
	for (unsigned int i=0; i<entries.size(); i++)
	{
		if (entries[i].fileName != "")
			playlistSet.insert(UpperCase(ExpandFileName(playlist.getFullPath(entries[i].fileName))));
	}

	std::vector<AnsiString> filesToAdd;
	const char* extensions[] = {".mp4", ".avi", ".mpg", ".mpeg", ".flv", ".mov",
		".mkv", ".wmv", ".webm", ".qt"};

	int iAttributes = faAnyFile;
	Sysutils::TSearchRec sr;
	AnsiString path = dir + "\\*.*";
	if (FindFirst(path, iAttributes, sr) == 0)
    {
        do
        {
            // subdirs
			if (sr.Attr & faDirectory) {
				// skip
            }
            else
            {
				AnsiString ext = ExtractFileExt(sr.Name);
				for (unsigned int i=0; i<sizeof(extensions)/sizeof(extensions[0]); i++)
				{
					if (extensions[i] == LowerCase(ext))
					{
						AnsiString Path = ExtractFilePath(path) + sr.Name;
						if (playlistSet.find(UpperCase(ExpandFileName(Path))) == playlistSet.end())
						{
							filesToAdd.push_back(Path);
						}
						break;
					}
				}
			}
        } while (FindNext(sr) == 0);
        FindClose(sr);
	}

	playlist.add(filesToAdd);
	update();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miResetBitrateInfoClick(TObject *Sender)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;

	item->MakeVisible(false);

	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[id];

	int status = playlist.resetBitrateInfo(entry.id);
	if (status != 0)
	{
		MessageBox(this->Handle, "Failed to reset bitrate info for file.", Application->Title.c_str(), MB_ICONEXCLAMATION);
		return;
	}

	update();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miSetIntroOutroSkipClick(TObject *Sender)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;
	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &entry = entries[id];

	frmIntroOutroSkip->SetTimes(entry.entry.skipIntroLength, entry.entry.skipOutroLength);
	if (frmIntroOutroSkip->ShowModal() == mrOk)
	{
		unsigned int intro = 0, outro = 0;
		frmIntroOutroSkip->GetTimes(intro, outro);
		std::set<unsigned int> ids;		
		for (int i=0; i<lvPlaylist->Items->Count; i++)
		{
			if (lvPlaylist->Items->Item[i]->Selected)
			{
				ids.insert(entries[i].id);
			}
		}
		playlist.setSkipIntroOutro(ids, intro, outro);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miAddUrlClick(TObject *Sender)
{
	frmAddUrl->Caption = "Add URL";
	frmAddUrl->ShowModal();
	if (frmAddUrl->ModalResult == mrOk)
	{
		playlist.addUrl(frmAddUrl->getUrl(), frmAddUrl->getName());
		update();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miEditUrlClick(TObject *Sender)
{
	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;
	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &fentry = entries[id];

	frmAddUrl->Caption = "Edit URL";
	frmAddUrl->set(fentry.entry.url, fentry.entry.name);
	frmAddUrl->ShowModal();
	if (frmAddUrl->ModalResult == mrOk)
	{
		playlist.updateUrl(fentry.id, frmAddUrl->getUrl(), frmAddUrl->getName());
		update();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::popupMenuPopup(TObject *Sender)
{
	miMoveToPlaylist->Clear();
	moveTargets.clear();
	if (callbackGetPlaylists)
	{
		std::vector<TfrmPlaylist*> playlists;
		callbackGetPlaylists(playlists);
		for (unsigned int i=0; i<playlists.size(); i++)
		{
			if (playlists[i] == this)
				continue;
			TMenuItem* item = new TMenuItem(popupMenu);
			item->AutoHotkeys = maManual;
			item->Caption = playlists[i]->getCaption();
			item->Tag = moveTargets.size();
			item->OnClick = miMoveToPlaylistTargetClick;
			miMoveToPlaylist->Add(item);
			moveTargets.push_back(playlists[i]);
		}
	}
	miMoveToPlaylist->Enabled = (lvPlaylist->Selected != NULL && !moveTargets.empty());
	miPathsSelectedRelative->Enabled = (lvPlaylist->Selected != NULL);
	miPathsSelectedAbsolute->Enabled = (lvPlaylist->Selected != NULL);

	TListItem *item = lvPlaylist->Selected;
	if (item == NULL)
		return;
	int id = item->Index;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	const FilteredPlaylistEntry &fentry = entries[id];

	miEditUrl->Visible = (fentry.entry.fileName == "");	
}
//---------------------------------------------------------------------------


void __fastcall TfrmPlaylist::miMoveToPlaylistTargetClick(TObject *Sender)
{
	TMenuItem *item = dynamic_cast<TMenuItem*>(Sender);
	if (item == NULL || item->Tag < 0 || item->Tag >= static_cast<int>(moveTargets.size()))
		return;
	if (!lvPlaylist->Selected)
		return;
	std::set<unsigned int> ids;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	for (int i=0; i<lvPlaylist->Items->Count; i++)
	{
		if (lvPlaylist->Items->Item[i]->Selected)
		{
			ids.insert(entries[i].id);
		}
	}
	std::vector<PlaylistEntry> moved;
	for (std::set<unsigned int>::const_iterator iter = ids.begin(); iter != ids.end(); ++iter)
	{
		moved.push_back(playlist.getEntry(*iter));
	}
	moveTargets[item->Tag]->addEntries(moved);
	playlist.remove(ids);
	update();
	lvPlaylist->ClearSelection();
}
//---------------------------------------------------------------------------

void TfrmPlaylist::addEntries(const std::vector<PlaylistEntry>& entries)
{
	// target playlist might not be loaded yet - load it first so saving does not overwrite its file
	load();
	playlist.addEntries(entries);
	update();
}

std::set<unsigned int> TfrmPlaylist::getSelectedIds(void)
{
	std::set<unsigned int> ids;
	const std::vector<FilteredPlaylistEntry>& entries = playlist.getFilteredEntries();
	for (int i=0; i<lvPlaylist->Items->Count; i++)
	{
		if (lvPlaylist->Items->Item[i]->Selected)
		{
			ids.insert(entries[i].id);
		}
	}
	return ids;
}

std::set<unsigned int> TfrmPlaylist::getAllIds(void)
{
	// whole playlist, not only entries matching current filter
	std::set<unsigned int> ids;
	for (unsigned int i=0; i<playlist.getEntries().size(); i++)
	{
		ids.insert(i);
	}
	return ids;
}

void TfrmPlaylist::convertPaths(const std::set<unsigned int>& ids, bool relative)
{
	int count;
	if (relative)
		count = playlist.makePathsRelative(ids);
	else
		count = playlist.makePathsAbsolute(ids);
	LOG("%s: converted %d path(s) to %s", fileName.c_str(), count, relative ? "relative" : "absolute");
	update();
}

void __fastcall TfrmPlaylist::miPathsAllRelativeClick(TObject *Sender)
{
	convertPaths(getAllIds(), true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miPathsAllAbsoluteClick(TObject *Sender)
{
	convertPaths(getAllIds(), false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miPathsSelectedRelativeClick(TObject *Sender)
{
	convertPaths(getSelectedIds(), true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmPlaylist::miPathsSelectedAbsoluteClick(TObject *Sender)
{
	convertPaths(getSelectedIds(), false);
}
//---------------------------------------------------------------------------
