object Form1: TForm1
  Left = 208
  Top = 252
  AutoScroll = False
  Caption = 'Circuit'
  ClientHeight = 242
  ClientWidth = 697
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnMouseMove = FormMouseMove
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object PaintBox1: TPaintBox
    Left = 0
    Top = 0
    Width = 697
    Height = 242
    Align = alClient
    OnMouseDown = PaintBox1MouseDown
    OnMouseMove = PaintBox1MouseMove
    OnMouseUp = PaintBox1MouseUp
    OnPaint = PaintBox1Paint
  end
  object paintboxPopup: TPopupMenu
    Left = 32
    object AddElement1: TMenuItem
      Caption = 'Add Element'
      OnClick = AddElement1Click
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object Simulator1: TMenuItem
      Caption = 'Simulator properties'
      OnClick = Simulator1Click
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object Simulatorproperties1: TMenuItem
      Caption = 'Remove all'
      OnClick = New1Click
    end
  end
  object elementPopup: TPopupMenu
    object Properties1: TMenuItem
      Caption = 'Properties'
      OnClick = Properties1Click
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object CalcImpedance: TMenuItem
      Caption = 'Calculate Impedance'
      OnClick = CalcImpedanceClick
    end
    object Simulatorproperties3: TMenuItem
      Caption = 'Simulator properties'
      OnClick = Simulator1Click
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object RemoveElement1: TMenuItem
      Caption = 'Remove Element'
      OnClick = RemoveElement1Click
    end
    object Removeall1: TMenuItem
      Caption = 'Remove all'
      OnClick = New1Click
    end
  end
  object MainMenu1: TMainMenu
    Left = 88
    Top = 16
    object File1: TMenuItem
      Caption = '&File'
      object New1: TMenuItem
        Caption = '&New'
        OnClick = New1Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object Open1: TMenuItem
        Caption = 'Open &instrument file...'
        OnClick = Open1Click
      end
      object Openborelist1: TMenuItem
        Caption = 'Open &bore list...'
        OnClick = Openborelist1Click
      end
      object N7: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object Options1: TMenuItem
      Caption = '&Options'
      object SnaptoGrid1: TMenuItem
        Caption = '&Snap to grid'
        OnClick = SnaptoGrid1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      object About1: TMenuItem
        Caption = '&About...'
        OnClick = About1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Instrument files (*.ins)|*.ins|Instrument files (*.*)|*.*'
    Left = 192
    Top = 24
  end
  object OpenDialog2: TOpenDialog
    Filter = 'Bore list files (*.txt)|*.txt|Bore list files (*.*)|*.*'
    Left = 232
    Top = 24
  end
end
