object frmIntroOutroSkip: TfrmIntroOutroSkip
  Left = 0
  Top = 0
  Caption = 'Intro/outro skip'
  ClientHeight = 99
  ClientWidth = 234
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
  object lblIntroSkip: TLabel
    Left = 8
    Top = 8
    Width = 165
    Height = 13
    Caption = 'Intro length to skip                       s'
  end
  object lblOutroSkip: TLabel
    Left = 8
    Top = 32
    Width = 166
    Height = 13
    Caption = 'Outro length to skip                      s'
  end
  object cbIntroSkip: TComboBox
    Left = 112
    Top = 5
    Width = 49
    Height = 21
    ItemHeight = 13
    TabOrder = 0
  end
  object cbOutroSkip: TComboBox
    Left = 112
    Top = 29
    Width = 49
    Height = 21
    ItemHeight = 13
    TabOrder = 1
  end
  object pnlBottom: TPanel
    Left = 0
    Top = 62
    Width = 234
    Height = 37
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitLeft = -185
    ExplicitTop = 141
    ExplicitWidth = 621
    DesignSize = (
      234
      37)
    object btnCancel: TButton
      Left = 151
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
      ExplicitLeft = 538
    end
    object btnApply: TButton
      Left = 70
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Apply'
      ModalResult = 1
      TabOrder = 0
      ExplicitLeft = 457
    end
  end
end
