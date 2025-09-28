object frmMpvSetProperty: TfrmMpvSetProperty
  Left = 0
  Top = 0
  BorderIcons = [biMinimize, biMaximize]
  BorderStyle = bsSingle
  Caption = 'Set MPV property'
  ClientHeight = 115
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
    Top = 8
    Width = 27
    Height = 13
    Caption = 'Name'
  end
  object lblValue: TLabel
    Left = 8
    Top = 32
    Width = 26
    Height = 13
    Caption = 'Value'
  end
  object lblStatus: TLabel
    Left = 44
    Top = 57
    Width = 3
    Height = 13
  end
  object edName: TEdit
    Left = 44
    Top = 5
    Width = 421
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
    Left = 44
    Top = 29
    Width = 421
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
    Left = 390
    Top = 82
    Width = 75
    Height = 25
    Caption = 'Set'
    TabOrder = 2
    OnClick = btnSetClick
  end
end
