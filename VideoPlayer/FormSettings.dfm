object frmSettings: TfrmSettings
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Settings'
  ClientHeight = 363
  ClientWidth = 621
  Color = clBtnFace
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
    Top = 326
    Width = 621
    Height = 37
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      621
      37)
    object btnCancel: TButton
      Left = 538
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = btnCancelClick
    end
    object btnApply: TButton
      Left = 457
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
    Width = 621
    Height = 326
    ActivePage = tsGeneral
    Align = alClient
    TabOrder = 0
    object tsGeneral: TTabSheet
      Caption = 'General'
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
        Top = 138
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
        Left = 3
        Top = 169
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
    end
    object tsLogging: TTabSheet
      Caption = 'Logging'
      ImageIndex = 1
      TabVisible = False
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
        ItemHeight = 13
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
    end
    object tsHotkeys: TTabSheet
      Caption = 'Hotkeys'
      ImageIndex = 3
    end
  end
  object OpenDialog: TOpenDialog
    DefaultExt = '*.exe'
    FileName = 'mplayer.exe'
    Filter = 'Executables|*.exe'
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Top = 224
  end
end
