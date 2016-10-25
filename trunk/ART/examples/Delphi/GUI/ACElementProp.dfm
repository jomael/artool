object Form2: TForm2
  Left = 218
  Top = 220
  BorderStyle = bsDialog
  Caption = 'Create new element'
  ClientHeight = 213
  ClientWidth = 150
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 0
    Width = 81
    Height = 13
    Caption = 'Name of Element'
  end
  object Label2: TLabel
    Left = 0
    Top = 40
    Width = 98
    Height = 13
    AutoSize = False
    Caption = 'Prototype of Element'
  end
  object Label3: TLabel
    Left = 0
    Top = 80
    Width = 113
    Height = 13
    AutoSize = False
    Caption = 'Parameters of Element'
  end
  object Button1: TButton
    Left = 83
    Top = 184
    Width = 65
    Height = 25
    Caption = 'Ok'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 0
    Top = 184
    Width = 65
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Edit1: TEdit
    Left = 0
    Top = 16
    Width = 145
    Height = 21
    TabOrder = 2
    OnChange = Edit1Change
  end
  object ComboBox1: TComboBox
    Left = 0
    Top = 56
    Width = 145
    Height = 21
    ItemHeight = 13
    TabOrder = 3
    Text = '(select form list)'
    OnChange = ComboBox1Change
  end
end
