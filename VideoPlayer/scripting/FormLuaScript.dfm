object frmLuaScript: TfrmLuaScript
  Left = 0
  Top = 0
  Caption = 'Script'
  ClientHeight = 441
  ClientWidth = 639
  Color = clBtnFace
  Constraints.MinHeight = 300
  Constraints.MinWidth = 300
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlBottom: TPanel
    Left = 0
    Top = 309
    Width = 639
    Height = 132
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object pnlBottom2: TPanel
      Left = 0
      Top = 0
      Width = 639
      Height = 41
      Align = alTop
      TabOrder = 0
      object btnExecute: TButton
        Left = 8
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Execute'
        TabOrder = 0
        OnClick = btnExecuteClick
      end
      object btnClearOutputWindow: TButton
        Left = 186
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Clear output'
        TabOrder = 1
        OnClick = btnClearOutputWindowClick
      end
      object btnBreak: TButton
        Left = 98
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Break'
        TabOrder = 3
        OnClick = btnBreakClick
      end
      object btnLuacheck: TButton
        Left = 278
        Top = 8
        Width = 85
        Height = 25
        Caption = 'Run luacheck'
        TabOrder = 2
        OnClick = btnLuacheckClick
      end
    end
    object lvValidation: TListView
      Left = 0
      Top = 41
      Width = 639
      Height = 91
      Align = alClient
      Columns = <
        item
          Caption = 'File'
          Width = 0
        end
        item
          Caption = 'Line'
          Width = 35
        end
        item
          Caption = 'Pos'
          Width = 30
        end
        item
          Caption = 'Message'
          Width = 400
        end>
      HideSelection = False
      OwnerData = True
      ReadOnly = True
      RowSelect = True
      TabOrder = 1
      ViewStyle = vsReport
      OnData = lvValidationData
      OnDblClick = lvValidationDblClick
    end
  end
  object redOutput: TMemo
    Left = 0
    Top = 220
    Width = 639
    Height = 89
    Align = alBottom
    HideSelection = False
    MaxLength = 100000000
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 1
  end
  object MainMenu: TMainMenu
    Left = 8
    Top = 208
    object miFile: TMenuItem
      Caption = 'File'
      OnClick = miFileClick
      object Open1: TMenuItem
        Action = actFileOpen
      end
      object miOpenRecent: TMenuItem
        Caption = 'Open Recent'
      end
      object Save1: TMenuItem
        Action = actFileSave
      end
      object SaveAs1: TMenuItem
        Action = actFileSaveAs
      end
      object miCloseWindow: TMenuItem
        Caption = 'Close window'
        OnClick = miCloseWindowClick
      end
    end
    object miHelp: TMenuItem
      Caption = 'Help'
      object miHelpExamples: TMenuItem
        Caption = 'Examples'
      end
    end
  end
  object OpenDialog: TOpenDialog
    Filter = 
      'Lua files (*.lua)|*.lua|Text files (*.txt)|*.txt|All files (*.*)' +
      '|*.*'
    Left = 40
    Top = 208
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '*.lua'
    Filter = 
      'Lua files (*.lua)|*.lua|Text files (*.txt)|*.txt|All files (*.*)' +
      '|*.*'
    Options = [ofHideReadOnly, ofExtensionDifferent, ofEnableSizing]
    Left = 72
    Top = 208
  end
  object actionList: TActionList
    Left = 104
    Top = 208
    object actFileOpen: TAction
      Category = 'File'
      Caption = '&Open...'
      ShortCut = 16463
      OnExecute = actFileOpenExecute
    end
    object actFileSave: TAction
      Category = 'File'
      Caption = '&Save'
      ShortCut = 16467
      OnExecute = actFileSaveExecute
    end
    object actFileSaveAs: TAction
      Category = 'File'
      Caption = 'Save &As...'
      ShortCut = 24659
      OnExecute = actFileSaveAsExecute
    end
  end
end
