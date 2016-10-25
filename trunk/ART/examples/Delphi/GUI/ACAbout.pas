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
unit ACAbout;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TForm5 = class(TForm)
    StaticText1: TStaticText;
    StaticText2: TStaticText;
    StaticText3: TStaticText;
    Label1: TLabel;
    Label2: TLabel;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form5: TForm5;

implementation

{$R *.DFM}

end.
