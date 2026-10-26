/** \file
	\brief OVCP / VideoPlayer / VideoPlayerMpv changelog
*/

/** \page changelog Changelog

Version 1.1
- FIXED: cached files list keeps previously selected and focused item after refreshing and sorting
- added "Go to file" and "Copy file to..." to context menu of opera cached files list
- Enter used to stop playing (or start if used on cached files list)
- added button toggling fullscreen mode (duplicating 'F' shortcut)
- added "Minimize" button 
- file dropped to window or passed by command line goes to "Other" tab and is played immediately

Version 1.2
- clicking on the player screen with the right mouse button toggles OSD level (same as with MPlayer) 
- FIXED: play button stayed down when clicked but no file was selected
- added "Pause" button
- added mplayer output log window (see "Settings")
- FIXED: possible hangup when playing damaged files (caused by not handling mplayer output pipe)
- FIXED: some annoying behavior related to toolbar auto hiding 
- most mplayer-style hotkeys are active only during playback (problems when entering filename to "Other" tab")

Version 1.3
- removed borlndmm.dll and cc3270mt.dll dependencies
- changed Escape action: if application is maximized it goes to normal mode, otherwise it is closing
- added option to automatically exit fullscreen when playing is stopped

Version 1.4
- FIXED: control toolbar not showing sometimes after switching to fullscreen
- changed MPlayer key color to one less likely to cause collision (video visible through other window on top of player)
- set *.flv as default extension when copying files from cache
- added option to enable MPlayer software volume amplification (for quiet movie files), default up to 200% 

Version 1.5
- software-controlled volume level is stored in settings

Version 1.6
- default extension for saved files changed to mp4
- added audio/video bitrate display to main window

Version 2.0
- added mp4 extension to open file dialog

- aplication name changed to VideoPlayer
- removed functionality related to Opera (v12) cache
- added playlists

- if single file is passed from command line: search for previous same entry on playlist before adding new entry (e.g. when player would be started multiple time with same source file or rtsp stream)
- if TEdit filter control is not focused "F"/"f" key works also when stopped (toggling fullscreen)
- added "About..." button to settings window

- settings window: close with Esc, changed window position to main form center
- OSD: show file name => 'i' key
- fluent fast forwarding - reading file length and current position
- added "Remove duplicates from list"
- added "Remove missing files from list"

Version 2.2
- Ctrl+D: delete selected files (if stopped) or currently playing file
- 'p' during playback: go to previous file (if position < 10 seconds) or go to beginning of the current file
- added "Mark" function
- added function marking files that are possible duplicates - by size
- added file length to playlists
- fixed issue with flickering at startup if multiple playlists are loaded
- delaying load of playlists until user switches to particular playlist
- remembering last playlist selected on restart
- added common.lib
- hotkeys are now configurable; previous fixed hotkeys are now default configuration
- added "show log" action type for hotkeys
- mplayer: use pausing_keep when changing volume and for OSD commands
- treat mplayer configuration path as relative to VideoPlayer.exe by default, as absolute if mplayer executable was not found there

Version 2.3
- added "Hide playlist" function
- added "Go to file" menu item 
- settings: added option to add extra parameters for mplayer (added for every file after default parameters)
- playlists can hold extra mplayer parameters for each file, added at the end of mplayer command line
- log: show mplayer command line
- default configuration: "L" as shortcut to "show log"
- default configuration: try also relative mplayer\mplayer.exe path if this file exists
- add setting for control panel position: top (default) or bottom
- changed default softvol amplification to 200% and volume to 50

Version 2.4
- FIXED default playlist selection when playlist are hidden
- FIXED "Rename playlist" for newly created playlist (not saved yet)
- added option to store separate volume setting for each file from playlist
- store and show video and audio bitrates for playlist entries (files can be softed by bitrate)
- FIXED Access Violation when unhiding playlist
- saving modified playlists with 120 s timer event (not only on exit) to prevent possible playlist loss in case of e.g. crash/reboot
- files dropped to playlist are now not played automatically (to avoid unintentional loss of previously played file bookmark)

VideoPlayerMpv:
- storing/showing minimum and maximum video and audio bitrate values for each file
- values displayed in main window as video/audio bitrates should be now treated as momentary
- volume is shown as OSD text for 1500 ms when changed using mouse wheel

Version 3.1
- added volume up/down hotkey action types
- fixed global hotkeys
- converting file names to UTF-8 when opening and for OSD
- added Lua scripting

Version 3.2
- focusing playlist on startup
- added new hotkey action types:
	- toggling subtitles on/off
	- pause / frame step
- ignoring files added to playlist twice when searching for duplicated files by size

Vesion 3.3
- added tool window for testing mpv properties (setting name + value as string)
	- added example removing black bars
- added 'Ignore mouse/cursor movement when playing in full screen mode' setting
- options to skip file intro and outro (configurable times per playlist entry)
- forcing video output / OSD also for audio-only files 
- added tray icon (opt-out)

Version 3.4
- added option to start minimized to tray
- URLs (like HTTP/HTTPS radio stations) can be added to playlist
- added option to show ballon tooltip with file/URL name on play start (Win7)
- renamed play/stop action to stop, added actual play/stop (start/stop) action
- added media hotkeys (Play/Pause, Previous Track and Next Track) to default configuration, by default not global
- added "Stop" to tray menu
- added bitmaps (icons) to tray menu items
- added option to ignore "Prev" command/hotkey when playing first item from the playlist
- added option to ignore "Next" command/hotkey when playing last item from the playlist
- settings window height can be resized (allows displaying more hotkeys same time)

Version 3.5
- added option (opt-out) to pause when suspending to RAM or hibernating
- added option to exit fullscreen when suspending to RAM or hibernating
- player asks if new playlist should be created if single folder is dropped into application
- files from dropped folders are added recursively to playlist
- added "Previous" and "Next" to tray menu
- added "Move to playlist" to playlist context menu (moves selected entries with their playback info)
- added option to start with Windows (current user, HKCU Run registry key)
- added option to resume playback of last file on application start
- added option to set video background color (video panel and mpv), e.g. for seamless transitions between files
- added option to loop playlist (Next after last item goes to first one, Prev on first item goes to last one)
- Shift + mouse wheel seeks +/-3 s (wheel up = forward), with option to swap it with volume control (plain wheel)
- added "Paths" to playlist context menu: converting paths of all or selected entries to relative (to playlist file directory) or absolute
	- note: paths are relative to playlist file directory (common convention, e.g. m3u), not to executable directory;
	  as playlists are always stored in "playlists" subfolder both would differ only by one level ("..\Videos\a.mp4" vs "Videos\a.mp4")
	  and both survive moving whole application folder
- relative paths in playlist are resolved against playlist file directory (if file exists there) when playing and for file operations (go to file, rename, delete, remove missing); paths starting with drive letter, backslash/slash (UNC) or URLs are treated as absolute
- FIXED: "Remove missing files from list" was removing URL entries
- FIXED: last playback position could be assigned to wrong file when playlist was saved with active filter (file name with last position is now stored in playlist)
- FIXED: main window position was limited to primary monitor (multi-monitor support, window is moved to first monitor only if it is not visible on any monitor)
- FIXED: dragging playlist tab to new position was not working correctly with hidden playlists
- FIXED: "Default" playlist protection (rename/delete/hide) depended on tab position instead of playlist name
- FIXED: default playlist file with different letter case (e.g. default.json) was loaded as second playlist
- FIXED: "Unhide playlist" menu could point to wrong playlist after deleting or moving tabs
- FIXED: hidden playlist could be selected at startup if it was the last used one
- FIXED: new playlist with name of already existing one was opened twice on the same file
- FIXED: playlist selected after deleting active playlist was not loaded
- FIXED: playlist tab was removed even if deleting playlist file failed
- FIXED: application could hang forever when stopping playback (stop timeout was not counting)
- FIXED: crashes when starting minimized to tray (playlist autosave, tray Exit/Play before showing window); also "Resume playback on start" works now in this mode
- FIXED: possible memory corruption when changing volume with playlist filter active
- FIXED: crash when log file could not be created
- FIXED: crash on play when mpv failed to initialize
- FIXED: Lua: FindWindowByCaptionAndExeName could use freed memory; _ALERT with non-string argument crashed
- FIXED: Lua: ShowMessage, Sleep, SetClipboardText, ForceDirectories used last instead of first argument; Sleep with non-integer value did not wait
- Lua: WinAPI module renamed from "jsonedit_winapi" to "vp_winapi" - scripts need to use local winapi = require("vp_winapi")
- FIXED: Lua: missing locks for queues and ClearAllVariables (possible data corruption with plugins)
- FIXED: Lua: FindWindowByCaptionAndExeName could freeze on not responding window
- FIXED: Lua: exception from function called by script could leave script window in "running" state; error message for non-string error objects
- FIXED: Lua: ForceDirectories with empty path
- FIXED: script window: "Save As" with "All files" filter did not save; "Save As" did not clear modified flag; cancelling save after "Save current file?" discarded text
- FIXED: script window: handle leaks when running luacheck
- FIXED: quick Next/Prev while file was still loading could skip additional playlist entry (next file is started only if previous one ended by itself)
- FIXED: resume position from file that failed to load could be applied to next file
- FIXED: skipping outro could be triggered by length of previous file; outro skip no longer stops playback from inside mpv event handler
- FIXED: volume changed before first playback was lost
- FIXED: UI stayed in playing state if playback failed to start
- FIXED: "Delete file" action did not stop playback of last file when "ignore Next on last item" was enabled
- FIXED: possible crash on global fullscreen/minimize hotkey when application was not active
- FIXED: with playlist filter active: "Set mpv extra params" showed values of other entry, opening file already present in playlist selected wrong entry
- FIXED: file already present in playlist as relative path was added again when opened
- FIXED: "Mark duplicates by size" could mark wrong entry if playlist contained duplicated file names
- FIXED: intro/outro skip dialog showed old values after change
- FIXED: audio bitrate from older playlist format could be reset to 0
- FIXED: script editor search ignored "Whole word" when "Match case" was also set
- FIXED: file added or opened with playlist filter active could be not selected (or other entry selected and played)
- FIXED: renamed active playlist was not restored after restart
- FIXED: restoring maximized (fullscreen) window from tray could move it to first monitor
- FIXED: renaming file with relative path without directory part moved it to drive root; renaming URL entry is ignored
- FIXED: last playback position was lost after renaming file
- FIXED: "Add remaining files from last folder" and "Add file(s)" dialog did not handle relative paths and URL as last entry; files already in playlist (relative paths, different letter case) were added again; uppercase extensions were ignored
- playlist names are checked for empty value and characters not allowed in file names
- playlist name duplicate check on rename is case-insensitive
*/

