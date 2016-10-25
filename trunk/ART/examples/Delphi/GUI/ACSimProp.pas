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
unit ACSimProp;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ARTdefinitions, ACCommon, ACMain;

type
  TForm4 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    procedure FormCreate(Sender: TObject);
    procedure StartDialog(x,y:integer);
//    procedure ListBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
  //    Y: Integer);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
//    procedure ListBox1Click(Sender: TObject);
//    procedure Button3Click(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    x,y : integer;
    procedure EditKeyPress(Sender: TObject;  var Key: Word; Shift: TShiftState);
    procedure EditDone(Sender: TObject);
  end;


var
  Form4: TForm4;

  editdef: string;

implementation

uses ACElementProp;

{$R *.DFM}
var
  oldHintIndex: integer = -1;
  freshElement: boolean;
  //editElement: P_ART_element;
  labels, edits: tlist;
  originalParams: string;
  editStrings: TStringList;


procedure SetParameter(i: integer);
var
  text: array[0..100] of char;
  text2: Pchar;
  paramstr: string;
  dp: P_ART_DataProp;
  strings: TStrings;
begin
  paramstr := TLabel(labels.Items[i]).caption + ' = ' + TEdit(edits.Items[i]).text;
  StrPCopy(text,paramstr);
  dp := ARTSetParameter(simulator, text);
  if (dp = nil) then begin //if unsuccessful
    messagebox(form4.handle,ARTGetLastErrorMessage,'Error when setting parameter', mb_ok);
    //reset to saved value
    TEdit(edits.Items[i]).text := editStrings[i];
  end
  else
  begin // update field
    text2 := ARTGetDefinitionString(dp);
    if not(text2 = nil) then
    begin
      strings := SplitString('=',string(text2), nil);
      TEdit(edits.Items[i]).text := Trim(strings[1]);
    end;
  end;


end;

procedure tform4.StartDialog(x,y:integer);
var prop: P_ART_DataProp;
  elementName,name,s: string;
  text,text2: Pchar;
  i: integer;
  model: P_ART_Object;
  strings: tstrings;
  h:integer;
  NewEdit : TEdit;
  NewLabel : TLabel;
begin
  //remove all previously added controls
  for i := edits.count-1 downto 0 do
  begin
    RemoveComponent(Tedit(edits.Items[i]));
    RemoveComponent(Tlabel(labels.Items[i]));
    Tedit(edits.Items[i]).Free;
    Tlabel(labels.Items[i]).Free;
  end;

  edits.clear;
  labels.clear;

  editStrings.Clear;

  h := 20;
  i := 0;
  prop := ARTGetDataProperties(simulator,nil);
  originalParams := '';
  while prop <> nil do
  begin
    text := ARTGetDefinitionString(prop);
    if not(text = nil) then
    begin
      //save original parameter strings to restore original values...
      originalParams := originalParams + string(text) + '; ';
      strings := SplitString('=',string(text), nil);
      NewEdit := TEdit.create(self);
      NewLabel := TLabel.create(self);
      with NewLabel do
      begin
        Width  := 150;
        Height  := h;
        Top    := 2 + 3 + h * i;
        Left   := 2;
        Parent := self;
        Hint := ARTGetShortDescription( prop );
        ShowHint := true;
        Caption := Trim(strings[0]);
      end;
      with NewEdit do
      begin
        Width  := 65;
        Height  := h;
        Top    := 2 + h * i;
        Left   := 160;
        Parent := self;
        OnKeyDown := EditKeyPress;
        AutoSelect := false;
        OnExit := EditDone;
        text := Trim(strings[1]);
        editStrings.Add( Trim(strings[1]) );
        Hint := ARTGetShortDescription( prop );
        ShowHint := true;
      end;  //With

      labels.add(NewLabel);
      edits.add(NewEdit);
      inc(i);
    end;

    //Get next property
    prop := ARTGetDataProperties(simulator,prop);
  end;

  button1.Top := 10 + 2 + h * (i);
  button2.Top := 10 + 2 + h * (i);

  form4.height := button1.Top + button1.height + 30;

  show;
  self.x := x;
  self.y := y;
end;

//This creates and prepares the form when the application is started
procedure tform4.FormCreate(Sender: TObject);
var
  acElements : P_ART_Property;
  ele : P_ART_Object;
begin
  labels := tlist.create;
  edits := tlist.create;
  editstrings := TStringList.create;
end;


procedure tform4.Button1Click(Sender: TObject);
var
  i: integer;
begin
  //Set all the parameters
  for i:= 0 to edits.Count-1 do
    SetParameter( i );
  form4.Hide;
end;

procedure tform4.Button2Click(Sender: TObject);
var
  text: array[0..1000] of char;
  i: integer;
  s: string;
begin
  StrPCopy(text,originalParams);
  ARTSetParameter(simulator, text);
  //hide the form
  form4.Hide;
end;

procedure tform4.EditKeyPress(Sender: TObject;  var Key: Word; Shift: TShiftState);
var
  ii : integer;
  ListBox : TListBox;
begin
  if Key = VK_RETURN then
  begin
    ii := edits.IndexOf(sender);
    SetParameter( ii ); //hier
    Key := 0;
  end;
  if Key = VK_UP then begin
    ii := edits.IndexOf(sender);
    SetParameter( ii ); //hier
    dec(ii);
    if (ii<0) then ii := edits.Count - 1;
    TEdit(edits.Items[ii]).SetFocus;
    Key := 0;
  end;
  if Key = VK_DOWN then begin
    ii := edits.IndexOf(sender);
    SetParameter( ii ); //hier
    inc(ii);
    if (ii>=edits.Count) then ii := 0;
    TEdit(edits.Items[ii]).SetFocus;
    Key := 0;
  end;
end;


procedure tform4.EditDone(Sender: TObject);
var
  ii : integer;
begin
  ii := edits.IndexOf(sender);
  SetParameter( ii ); 
end;



procedure TForm4.FormDestroy(Sender: TObject);
begin
  labels.Free;
  edits.Free;
  editStrings.Free;
end;

end.

