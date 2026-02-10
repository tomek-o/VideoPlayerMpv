object frmAddUrl: TfrmAddUrl
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Add URL'
  ClientHeight = 96
  ClientWidth = 426
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblUrl: TLabel
    Left = 8
    Top = 8
    Width = 19
    Height = 13
    Caption = 'URL'
  end
  object lblName: TLabel
    Left = 8
    Top = 33
    Width = 83
    Height = 13
    Caption = 'Name (displayed)'
  end
  object pnlBottom: TPanel
    Left = 0
    Top = 59
    Width = 426
    Height = 37
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitTop = 62
    ExplicitWidth = 234
    DesignSize = (
      426
      37)
    object btnCancel: TButton
      Left = 343
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
      ExplicitLeft = 151
    end
    object btnApply: TButton
      Left = 262
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Apply'
      ModalResult = 1
      TabOrder = 0
      ExplicitLeft = 70
    end
  end
  object edUrl: TEdit
    Left = 97
    Top = 5
    Width = 321
    Height = 22
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = 'https://...'
  end
  object edName: TEdit
    Left = 97
    Top = 30
    Width = 321
    Height = 21
    TabOrder = 2
  end
end
