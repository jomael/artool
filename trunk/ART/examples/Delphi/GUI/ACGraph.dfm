object Form3: TForm3
  Left = 206
  Top = 305
  Width = 771
  Height = 240
  Caption = 'Form3'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object PaintBox1: TPaintBox
    Left = 0
    Top = 0
    Width = 763
    Height = 196
    Cursor = crCross
    Align = alClient
    OnMouseMove = PaintBox1MouseMove
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 196
    Width = 763
    Height = 17
    Cursor = crArrow
    Panels = <
      item
        Text = 'Element'
        Width = 200
      end
      item
        Text = 'Frequency'
        Width = 100
      end
      item
        Text = 'Impedance'
        Width = 100
      end>
    SimplePanel = False
  end
end
