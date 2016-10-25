{
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                         *
*                                                                         *
*              A c o u s t i c   C i r c u i t   v   1 . 0                *
*                                                                         *
*                                                                         *
*  A Simulation tool for Acoustic Systems using the ART library           *
*                                                                         *
*  Copyright (C) 2011 by the author and his organisation:                 *
*       Sadjad Siddiq                  sadjad.siddiq@gmx.at               *
*                                                                         *
*         Inst. of Music Acoustics, Univ. of Music, Vienna, AT            *
*         (http://iwk.mdw.ac.at)                                          *
*                                                                         *
*                                                                         *
*  The ART library:                                                       *
*                                                                         *
*  Copyright (C) 2011 by the authors and their organisations:             *
*                                                                         *
*       Alistair Braden          1)                                       *
*       Wilfried Kausel          2)    kausel(at)mdw.ac.at                *
*       Delphine Chadefaux       2)                                       *
*       Vasileios Chatziioannou  2)                                       *
*       Sadjad Siddiq            2)                                       *
*                                                                         *
*       1) School of Physics and Astronomy, Univ. of Edinburgh, GB        *
*          (http://www.ph.ed.ac.uk/)                                      *
*       2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT           *
*          (http://iwk.mdw.ac.at)                                         *
*                                                                         *
*  This program is free software: you can redistribute it and/or modify   *
*  it under the terms of the GNU General Public License as published by   *
*  the Free Software Foundation, either version 3 of the License, or      *
*  any later version.                                                     *
*                                                                         *
*  This program is distributed in the hope that it will be useful,        *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*  GNU General Public License for more details.                           *
*                                                                         *
*  You should have received a copy of the GNU General Public License      *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
*                                                                         *
*  more information on the internet: http://artool.sf.net                 *
*                                                                         *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
}
unit ACGraph;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, math, ACCommon, ComCtrls, ARTDefinitions;

type
  TForm3 = class(TForm)
    PaintBox1: TPaintBox;
    StatusBar1: TStatusBar;
    procedure FormPaint(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form3 : TForm3;
  zoomx : real;
  zoomy : real;
  caption : string;

procedure prepareGraph(bitmap: TBitmap;mmin,mmax:real);

implementation

{$R *.DFM}

procedure prepareGraph(bitmap: TBitmap;mmin,mmax:real);
var
  dfrq, f, dmag, mag: real;
  s: string;
  i, x, y: integer;
begin
  f_min := ARTGetDouble( ARTGetValue( ARTFindDataProperty(simulator,'LowerFrequencyLimit') ), 0 );
  f_max := ARTGetDouble( ARTGetValue( ARTFindDataProperty(simulator,'HigherFrequencyLimit') ), 0 );
  f_step := ARTGetDouble( ARTGetValue( ARTFindDataProperty(simulator,'FrequencyStep') ), 0 );

  Bitmap.Canvas.brush.color := 0;
  Bitmap.Canvas.pen.color := 0;
  Bitmap.Canvas.Rectangle(0,0,Bitmap.Width,Bitmap.Height);
  //frequency grid
  Bitmap.Canvas.pen.color := rgb(145,145,145);
  Bitmap.Canvas.Font.Color := rgb(145,145,145);
  //the distance between the frequency lines is smth. like (frequency range /10), but a number 2*10^n, where n e N
  dfrq := 4 * power(10,trunc(log10(f_max - f_min)) - 1);
  //don't start at f_min, but at a dfrq multiple equal or lower than f_min
  f := dfrq*trunc(f_min/dfrq) + dfrq;
  zoomx := Bitmap.Width / (f_max-f_min);
  repeat
    x := round((f-f_min) * zoomx);
    str(f:4:0,s);
    Bitmap.Canvas.MoveTo(x,0);
    Bitmap.Canvas.LineTo(x,Bitmap.Height-13);
    Bitmap.Canvas.TextOut(x-12,Bitmap.Height-13, s + ' Hz');
    f := f + dfrq;
  until (f >= f_max);

  dmag := 8 * power(10,trunc(log10(mmax - mmin)) - 1);
  mag := dmag*trunc(mmin/dmag);
  zoomy := Bitmap.Height / (mmax-mmin);
  repeat
    y := Bitmap.Height - round(mag * zoomy);
    if (y>7) and (y<(Bitmap.Height-7)) then begin
      str(mag:0:0,s);
      Bitmap.Canvas.MoveTo(0,y);
      Bitmap.Canvas.LineTo(Bitmap.Width,y);
      Bitmap.Canvas.TextOut(0,y-7, s+' MOhm');
    end;
    mag := mag + dmag;
  until (mag >= mmax);

end;

procedure TForm3.FormPaint(Sender: TObject);
begin
  paintbox1.canvas.draw(0,0,impedancegraph);
end;

procedure TForm3.FormResize(Sender: TObject);
var i,l: integer;
    //mag: array of real;
    xstep, zoom:real; //min, max: real;
begin
  //resize bitmap
  impedanceGraph.height:=paintbox1.Height;
  impedanceGraph.width:=paintbox1.Width;

  prepareGraph(impedancegraph, impedanceMin / 1000000 , impedanceMax / 1000000); //use MEGA Ohm for labels
  //draw impedance curve
  l := Length(impedanceMagnitude);
  if l = 0 then exit;

  impedancegraph.canvas.pen.color := rgb(128,255,0);
  i:=0;
  if (impedanceMax - impedanceMin) = 0 then zoom := 0 //just in case...
    else zoom := impedancegraph.Height / (impedanceMax - impedanceMin);
  xstep := impedancegraph.Width / l;
  impedancegraph.canvas.moveTo(0, impedancegraph.Height - round((impedanceMagnitude[i]-impedanceMin) * zoom));
  for i := 1 to l-1 do begin
    impedancegraph.canvas.lineTo(round(i*xstep), impedancegraph.Height - round((impedanceMagnitude[i]-impedanceMin) * zoom));
  end;

  //Todo: recreate with new size, repaint curve ?
  paintbox1.canvas.draw(0,0,impedancegraph);
end;

procedure TForm3.FormCreate(Sender: TObject);
begin
  impedanceGraph:=tbitmap.Create;
  impedanceGraph.canvas.create;
  impedanceGraph.height:=paintbox1.Height;
  impedanceGraph.width:=paintbox1.Width;
  impedanceGraph.Canvas.Brush.Create;
  impedanceGraph.canvas.Brush.Color:=clblack;
  impedanceGraph.canvas.Brush.Style:=bssolid;
  impedanceGraph.canvas.Pen.Color := clblack;
  impedanceGraph.canvas.Rectangle(0,0,paintbox1.width,paintbox1.Height);
  prepareGraph(impedanceGraph,0,34);

end;

procedure TForm3.FormDestroy(Sender: TObject);
begin
  impedanceGraph.destroy;
end;

procedure TForm3.PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  f:real;
  mag:real;
  s : string;
begin
  f := (x / zoomx) + f_min;
  mag := -(y - impedanceGraph.Height) / zoomy ;//- impedanceMax / 1000000;

  str(f:4:0,s);
  statusbar1.Panels.Items[1].Text := s + ' Hz';

  str(mag:0:3,s);
  statusbar1.Panels.Items[2].Text := s + ' MOhm';

end;

end.
