object frmSettings: TfrmSettings
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Settings'
  ClientHeight = 356
  ClientWidth = 627
  Color = clBtnFace
  Constraints.MaxWidth = 643
  Constraints.MinHeight = 395
  Constraints.MinWidth = 635
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlBottom: TPanel
    Left = 0
    Top = 319
    Width = 627
    Height = 37
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 316
    DesignSize = (
      627
      37)
    object btnCancel: TButton
      Left = 544
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = btnCancelClick
    end
    object btnApply: TButton
      Left = 463
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Apply'
      TabOrder = 0
      OnClick = btnApplyClick
    end
  end
  object pcSettings: TPageControl
    Left = 0
    Top = 0
    Width = 627
    Height = 319
    ActivePage = tsGeneral
    Align = alClient
    TabOrder = 0
    ExplicitHeight = 316
    object tsGeneral: TTabSheet
      Caption = 'General'
      ExplicitHeight = 288
      object lblControlPanelPosition: TLabel
        Left = 3
        Top = 49
        Width = 104
        Height = 13
        Caption = 'Control panel position'
      end
      object chbAlwaysOnTop: TCheckBox
        Left = 3
        Top = 3
        Width = 325
        Height = 17
        Caption = 'Window always on top'
        TabOrder = 0
        OnClick = chbAlwaysOnTopClick
      end
      object btnShowLogWindow: TButton
        Left = 3
        Top = 260
        Width = 126
        Height = 25
        Caption = 'Show log window'
        TabOrder = 1
        OnClick = btnShowLogWindowClick
      end
      object chbExitFullscreenOnStop: TCheckBox
        Left = 3
        Top = 26
        Width = 325
        Height = 17
        Caption = 'Exit fullscreen on stop playing'
        TabOrder = 2
        OnClick = chbAlwaysOnTopClick
      end
      object btnAbout: TButton
        Left = 139
        Top = 260
        Width = 126
        Height = 25
        Caption = 'About...'
        TabOrder = 3
        OnClick = btnAboutClick
      end
      object cbControlPanelPosition: TComboBox
        Left = 128
        Top = 46
        Width = 65
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 4
        Text = 'top'
        Items.Strings = (
          'top'
          'bottom')
      end
      object chbIgnoreMouseMovementInFullScreenPlayback: TCheckBox
        Left = 3
        Top = 70
        Width = 358
        Height = 17
        Caption = 'Ignore mouse/cursor movement when playing in full screen mode'
        TabOrder = 5
      end
      object chbShowTrayIcon: TCheckBox
        Left = 3
        Top = 93
        Width = 358
        Height = 17
        Caption = 'Show icon in system tray'
        TabOrder = 6
      end
      object chbStartMinimizedToTray: TCheckBox
        Left = 22
        Top = 112
        Width = 243
        Height = 17
        Caption = 'Start minimized to tray'
        TabOrder = 7
      end
      object chbShowTrayBalloonOnPlayStart: TCheckBox
        Left = 22
        Top = 132
        Width = 475
        Height = 17
        Caption = 
          'Show tray notification balloon on play start (for Windows 7, pro' +
          'bably won'#39't work with Win 10)'
        TabOrder = 8
      end
      object chbIgnorePrevCommandWhenPlayingFirstItem: TCheckBox
        Left = 3
        Top = 155
        Width = 390
        Height = 17
        Caption = 'Ignore "Prev" command when playing first item from the playlist'
        TabOrder = 9
      end
      object chbIgnoreNextCommandWhenPlayingLastItem: TCheckBox
        Left = 3
        Top = 176
        Width = 390
        Height = 17
        Caption = 'Ignore "Next" command when playing last item from the playlist'
        TabOrder = 10
      end
      object chbPauseOnSuspend: TCheckBox
        Left = 3
        Top = 197
        Width = 390
        Height = 17
        Caption = 'Pause playback on suspend to RAM (S3) or hibernation (S4)'
        TabOrder = 11
      end
      object chbExitFullscreenOnSuspend: TCheckBox
        Left = 3
        Top = 217
        Width = 390
        Height = 17
        Caption = 'Exit fullscreen on suspend to RAM (S3) or hibernation (S4)'
        TabOrder = 12
      end
      object chbAutostart: TCheckBox
        Left = 3
        Top = 238
        Width = 358
        Height = 17
        Caption = 'Start with Windows (current user)'
        TabOrder = 13
      end
    end
    object tsLogging: TTabSheet
      Caption = 'Logging'
      ImageIndex = 1
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object lblUiCapacity: TLabel
        Left = 5
        Top = 26
        Width = 194
        Height = 13
        Caption = 'Visible (buffered) log lines in log window:'
      end
      object cmbMaxUiLogLines: TComboBox
        Left = 216
        Top = 23
        Width = 111
        Height = 21
        Style = csDropDownList
        ItemHeight = 0
        TabOrder = 0
        OnChange = cmbMaxUiLogLinesChange
        Items.Strings = (
          '100'
          '200'
          '500'
          '1000'
          '2000'
          '5000'
          '10000')
      end
      object chbLogToFile: TCheckBox
        Left = 5
        Top = 3
        Width = 325
        Height = 17
        Caption = 'Log to file'
        TabOrder = 1
        OnClick = chbAlwaysOnTopClick
      end
    end
    object tsMplayer: TTabSheet
      Caption = 'mpv'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object lblSoftVolMax: TLabel
        Left = 3
        Top = 3
        Width = 182
        Height = 13
        Caption = 'Maximum volume level (50 ... 1000%)'
      end
      object lblMplayerPercent: TLabel
        Left = 263
        Top = 3
        Width = 11
        Height = 13
        Caption = '%'
      end
      object lblBackgroundColor: TLabel
        Left = 3
        Top = 123
        Width = 111
        Height = 13
        Caption = 'Video background color'
      end
      object edMplayerSoftVolMax: TEdit
        Left = 198
        Top = 0
        Width = 51
        Height = 21
        TabOrder = 0
        Text = '200'
      end
      object chbMplayerShowFileNameOnPlayStart: TCheckBox
        Left = 3
        Top = 72
        Width = 318
        Height = 17
        Caption = 'Show file name (OSD) on playback start'
        TabOrder = 1
      end
      object chbUseSeparateVolumeForEachFile: TCheckBox
        Left = 3
        Top = 22
        Width = 318
        Height = 17
        Caption = 'Use separate volume for each file from playlist'
        TabOrder = 2
      end
      object chbMpvShowPropertyEditor: TCheckBox
        Left = 3
        Top = 95
        Width = 318
        Height = 17
        Caption = 'Show mpv property editor / test tool'
        TabOrder = 3
      end
      object pnlBackgroundColor: TPanel
        Left = 198
        Top = 118
        Width = 41
        Height = 23
        BevelOuter = bvLowered
        ParentBackground = False
        TabOrder = 4
        OnClick = btnBackgroundColorClick
      end
      object btnBackgroundColor: TButton
        Left = 245
        Top = 118
        Width = 75
        Height = 23
        Caption = 'Change...'
        TabOrder = 5
        OnClick = btnBackgroundColorClick
      end
      object btnBackgroundColorDefault: TButton
        Left = 326
        Top = 118
        Width = 75
        Height = 23
        Caption = 'Default'
        TabOrder = 6
        OnClick = btnBackgroundColorDefaultClick
      end
    end
    object tsPlayback: TTabSheet
      Caption = 'Playback'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object chbResumePlaybackOnStart: TCheckBox
        Left = 3
        Top = 3
        Width = 358
        Height = 17
        Caption = 'Resume playback of last file on application start'
        TabOrder = 0
      end
      object chbLoopPlaylist: TCheckBox
        Left = 3
        Top = 26
        Width = 358
        Height = 17
        Caption = 'Loop playlist (go to first item after last one)'
        TabOrder = 1
      end
      object chbMouseWheelSeekWithoutShift: TCheckBox
        Left = 3
        Top = 49
        Width = 600
        Height = 17
        Caption = 
          'Mouse wheel seeks (+/-3 s), Shift + mouse wheel changes volume (' +
          'swapped)'
        TabOrder = 2
      end
    end
    object tsHotkeys: TTabSheet
      Caption = 'Hotkeys'
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
    end
  end
  object ColorDialog: TColorDialog
    Options = [cdFullOpen]
    Left = 40
    Top = 320
  end
  object OpenDialog: TOpenDialog
    DefaultExt = '*.exe'
    FileName = 'mplayer.exe'
    Filter = 'Executables|*.exe'
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 8
    Top = 320
  end
end
