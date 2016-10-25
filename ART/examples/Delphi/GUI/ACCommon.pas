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
unit ACCommon;

interface

uses ARTdefinitions, graphics, classes, Sysutils;

var
  psim: P_ART_Object ;
  simulator: P_ART_simulator;
  mainCircuit: P_ART_Circuit;
  f_min, f_max, f_step: real;
  Nmodes: integer;
  impedancegraph: tbitmap;
  impedanceMagnitude: array of real;
  impedanceMin, impedanceMax: real;

function SplitString(const separator: string; const str: string; Strings: TStrings): TStrings;


implementation

function SplitString(const separator: string; const str: string; Strings: TStrings): TStrings;
  // Fills a string list with the parts of "str" separated by
  // "separator". If Nil is passed instead of a string list,
  // the function creates a TStringList object which has to
  // be freed by the caller
var
  n: integer;
  p, q, s: PChar;
  item: string;
begin
  if Strings = nil then
    Result := TStringList.Create
  else
    Result := Strings;
  try
    p := PChar(str);
    s := PChar(separator);
    n := Length(separator);
    repeat
      q := StrPos(p, s);
      if q = nil then q := StrScan(p, #0);
      SetString(item, p, q - p);
      Result.Add(item);
      p := q + n;
    until q^ = #0;
  except
    item := '';
    if Strings = nil then Result.Free;
    raise;
  end;
end;

end.
 