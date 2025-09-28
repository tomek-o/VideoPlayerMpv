object frmMpvSetProperty: TfrmMpvSetProperty
  Left = 0
  Top = 0
  BorderIcons = [biMinimize, biMaximize]
  BorderStyle = bsSingle
  Caption = 'Set MPV property'
  ClientHeight = 157
  ClientWidth = 479
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object lblName: TLabel
    Left = 8
    Top = 48
    Width = 27
    Height = 13
    Caption = 'Name'
  end
  object lblValue: TLabel
    Left = 8
    Top = 72
    Width = 26
    Height = 13
    Caption = 'Value'
  end
  object lblStatus: TLabel
    Left = 44
    Top = 97
    Width = 3
    Height = 13
  end
  object lblExamples: TLabel
    Left = 8
    Top = 8
    Width = 45
    Height = 13
    Caption = 'Examples'
  end
  object edName: TEdit
    Left = 64
    Top = 45
    Width = 401
    Height = 22
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object edValue: TEdit
    Left = 64
    Top = 69
    Width = 401
    Height = 22
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object btnSet: TButton
    Left = 344
    Top = 122
    Width = 121
    Height = 25
    Caption = 'Set property'
    TabOrder = 2
    OnClick = btnSetClick
  end
  object cbExamples: TComboBox
    Left = 64
    Top = 5
    Width = 265
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 3
  end
  object btnLoadExample: TButton
    Left = 344
    Top = 3
    Width = 121
    Height = 25
    Caption = 'Load example'
    TabOrder = 4
    OnClick = btnLoadExampleClick
  end
end
