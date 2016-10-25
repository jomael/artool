object Form1: TForm1
  Left = 208
  Top = 140
  BorderStyle = bsSingle
  Caption = 'Impedance Calculator'
  ClientHeight = 403
  ClientWidth = 704
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 172
    Height = 13
    Caption = '1. Choose Models of your Instrument'
  end
  object PaintBox1: TPaintBox
    Left = 0
    Top = 176
    Width = 704
    Height = 227
    Align = alBottom
    OnPaint = PaintBox1Paint
  end
  object Label3: TLabel
    Left = 8
    Top = 56
    Width = 300
    Height = 13
    Caption = '2. Set Parameters of Models  (choose parameter and edit value)'
  end
  object Label4: TLabel
    Left = 536
    Top = 8
    Width = 136
    Height = 13
    Caption = '3. Choose Frequency Range'
  end
  object Label5: TLabel
    Left = 536
    Top = 43
    Width = 87
    Height = 13
    Caption = 'Lowest Frequency'
  end
  object Label6: TLabel
    Left = 536
    Top = 70
    Width = 89
    Height = 13
    Caption = 'Highest Frequency'
  end
  object Label7: TLabel
    Left = 536
    Top = 96
    Width = 75
    Height = 13
    Caption = 'Frequency Step'
  end
  object Label8: TLabel
    Left = 677
    Top = 96
    Width = 13
    Height = 13
    Caption = 'Hz'
  end
  object Label9: TLabel
    Left = 677
    Top = 70
    Width = 13
    Height = 13
    Caption = 'Hz'
  end
  object Label10: TLabel
    Left = 677
    Top = 43
    Width = 13
    Height = 13
    Caption = 'Hz'
  end
  object Label11: TLabel
    Left = 536
    Top = 123
    Width = 84
    Height = 13
    Caption = 'Number of Modes'
  end
  object ComboBox1: TComboBox
    Left = 32
    Top = 24
    Width = 97
    Height = 21
    ItemHeight = 13
    TabOrder = 0
    Text = '(none)'
    OnChange = ComboBox1Change
  end
  object ComboBox2: TComboBox
    Left = 160
    Top = 24
    Width = 97
    Height = 21
    ItemHeight = 13
    TabOrder = 1
    Text = '(none)'
    OnChange = ComboBox2Change
  end
  object ComboBox3: TComboBox
    Left = 288
    Top = 24
    Width = 97
    Height = 21
    ItemHeight = 13
    TabOrder = 2
    Text = '(none)'
    OnChange = ComboBox3Change
  end
  object ComboBox4: TComboBox
    Left = 416
    Top = 24
    Width = 97
    Height = 21
    ItemHeight = 13
    TabOrder = 3
    Text = '(none)'
    OnChange = ComboBox4Change
  end
  object ListBox1: TListBox
    Left = 32
    Top = 72
    Width = 97
    Height = 73
    ItemHeight = 13
    Items.Strings = (
      'Choose Model')
    TabOrder = 4
    OnClick = ListBox1Click
  end
  object ListBox2: TListBox
    Left = 160
    Top = 72
    Width = 97
    Height = 73
    ItemHeight = 13
    Items.Strings = (
      'Choose Model')
    TabOrder = 5
    OnClick = ListBox2Click
  end
  object ListBox3: TListBox
    Left = 288
    Top = 72
    Width = 97
    Height = 73
    ItemHeight = 13
    Items.Strings = (
      'Choose Model')
    TabOrder = 6
    OnClick = ListBox3Click
  end
  object ListBox4: TListBox
    Left = 416
    Top = 72
    Width = 97
    Height = 73
    ItemHeight = 13
    Items.Strings = (
      'Choose Model')
    TabOrder = 7
    OnClick = ListBox4Click
  end
  object Edit1: TEdit
    Left = 32
    Top = 152
    Width = 97
    Height = 21
    TabOrder = 8
    Text = 'Choose parameter'
    OnExit = Edit1Exit
  end
  object Edit2: TEdit
    Left = 160
    Top = 152
    Width = 97
    Height = 21
    TabOrder = 9
    Text = 'Choose parameter'
    OnExit = Edit2Exit
  end
  object Edit3: TEdit
    Left = 288
    Top = 152
    Width = 97
    Height = 21
    TabOrder = 10
    Text = 'Choose parameter'
    OnExit = Edit3Exit
  end
  object Edit4: TEdit
    Left = 416
    Top = 152
    Width = 97
    Height = 21
    TabOrder = 11
    Text = 'Choose parameter'
    OnExit = Edit4Exit
  end
  object Button5: TButton
    Left = 528
    Top = 152
    Width = 169
    Height = 21
    Caption = '4. Calculate Input Impedance'
    TabOrder = 12
    OnClick = Button5Click
  end
  object ProgressBar1: TProgressBar
    Left = 0
    Top = 176
    Width = 705
    Height = 13
    Min = 0
    Max = 100
    TabOrder = 13
    Visible = False
  end
  object Edit5: TEdit
    Left = 632
    Top = 40
    Width = 41
    Height = 21
    TabOrder = 14
    Text = '10'
    OnExit = Edit5Exit
  end
  object Edit6: TEdit
    Left = 632
    Top = 67
    Width = 41
    Height = 21
    TabOrder = 15
    Text = '1800'
    OnExit = Edit6Exit
  end
  object Edit7: TEdit
    Left = 632
    Top = 93
    Width = 41
    Height = 21
    TabOrder = 16
    Text = '10'
    OnExit = Edit7Exit
  end
  object Edit8: TEdit
    Left = 632
    Top = 120
    Width = 41
    Height = 21
    TabOrder = 17
    Text = '1'
    OnExit = Edit8Exit
  end
end
