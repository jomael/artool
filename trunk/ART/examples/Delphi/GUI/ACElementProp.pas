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
unit ACElementProp;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ARTdefinitions, ACCommon, ACMain;

type
  TComboItemHint = Class(THintWindow)
  private
    DoHint : boolean;
    FControlName: string;
    procedure SetControlName(const Value: string);
    private
      property ControlName : string read FControlName write SetControlName;
    procedure DoActivateHint(ControlName : string; Text : string);
  end;

  TForm2 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Edit1: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    ComboBox1: TComboBox;
    procedure FormCreate(Sender: TObject);
    procedure StartDialog(x,y:integer; g: TgraphicElement);
//    procedure ListBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
  //    Y: Integer);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Edit1Change(Sender: TObject);
    procedure ComboBox1Exit(Sender: TObject);
    procedure ComboBox1Change(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    hw : TComboItemHint;
    x,y : integer;
    procedure ComboDropDown(sender : TObject);
    procedure ComboCloseUp(sender : TObject);
    procedure addEditFields;
    procedure EditKeyPress(Sender: TObject;  var Key: Word; Shift: TShiftState);
    procedure EditDone(Sender: TObject);
    procedure updateLabels;
  public
    procedure ApplicationIdle(sender: TObject; var Done: boolean);
  end;

var
  Form2: TForm2;
  editdef: string;

implementation

{$R *.DFM}
var
  oldHintIndex: integer = -1;
  freshElement: boolean;
  editGE: TgraphicElement;
  originalname: string;
  originalParams: string;
  originalPrototype: string;
  //editElement: P_ART_element;
  labels, edits: tlist;
//  originalParams: string;
  editStrings: TStringList;
 (*
function receiveValueAsString(prop: P_ART_DataProp) : string;
var
  s: string;
  text: array[0..100] of char;
  v:double;
begin
    receiveValueAsString := '';
    case ARTGetDatatype(prop) of
      C_ART_dbl: begin
        v := ARTGetDouble((prop),0);
        str(v:5:2, s);
        receiveValueAsString := s;
        end;
      C_ART_flo: begin
        v := ARTGetFloat((prop),0);
        str(v:5:2, s);
        receiveValueAsString := s;
        end;
      C_ART_int: begin
        v := ARTGetInteger((prop),0);
        str(v:5:2, s);
        receiveValueAsString := s;
        end;
      C_ART_str: begin
        s := String(ARTGetString((prop),0));
        receiveValueAsString := s;
        end;
    end;
end;    *)
 {
procedure receiveProperties();
var prop: P_ART_DataProp;
  elementName,name,s: string;
  text,text2: Pchar;
  i: integer;
  model: P_ART_Object;
begin
    form2.ListBox1.Items.Clear;
    if editGE.getARTElement <> nil then begin
      model := ARTGetModel(editGE.getARTElement);

      prop := ARTGetDataProperties(model,nil);
      while prop <> nil do
      begin
        text := ARTGetDefinitionString(P_ART_dataProp(prop));
        if not(text = nil) then form2.ListBox1.Items.AddObject(string(text), prop);

        //Get next property
        prop := ARTGetDataProperties(model,prop);
      end;
    end;
end;   }
{
procedure SetParameter(Edit: TEdit);
var
  text: array[0..100] of char;
begin
    if editGE.getARTElement <> nil then begin
      StrPCopy(text,Edit.text);
      if ARTSetParameter(simulator, text) = nil then begin //if unsuccessful
        //reset to original values
        Edit.text := editDef;
        StrPCopy(text,Edit.text);
        ARTSetParameter(simulator, text);
        //inform user about error
        messagebox(form2.handle,ARTGetLastErrorMessage,'Error when setting parameter', mb_ok);
      end;
    end;
end;
 }

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
  //writeln(paramstr);
  dp := ARTSetParameter(simulator, text);
  if (dp = nil) then begin //if unsuccessful
    messagebox(form2.handle,ARTGetLastErrorMessage,'Error when setting parameter', mb_ok);
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

procedure tform2.EditKeyPress(Sender: TObject;  var Key: Word; Shift: TShiftState);
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


procedure tform2.EditDone(Sender: TObject);
var
  ii : integer;
begin
  ii := edits.IndexOf(sender);
  SetParameter( ii ); 
end;

procedure TForm2.StartDialog(x,y:integer; g: TgraphicElement);
begin
{  listbox1.clear;
  listbox1.itemindex := -1;}
  freshElement := false;
  show;
  self.x := x;
  self.y := y;
  editGE := g;
  //Hide the edit field in the listbox
  // If no graphicelement to edit has been passed: create one
  if editGE = nil then
  begin
    originalName := CreateName('New');;
    editGE := TgraphicElement.create(originalName,1,1);
    editGE.place(graphicBGbuffer,point(x,y));
    ge.add(editGE);
    //remember we have created this element especially for this dialog!
    freshElement := true;
    caption:= 'Create new element';
    combobox1.ItemIndex := -1;
    //We have no ARTelement so far.
  end
  else //otherwise take data of the element passed
  begin
    originalName := editGE.getName;
    caption:= 'Element properties';
    combobox1.ItemIndex := combobox1.items.IndexOf(editGE.getPrototype);
    //Get properties and values of element
//    receiveProperties;
    //If the user presses cancel, we will revert to the original parameters,
    //so we have to save them now
//    originalParams := listbox1.items.Text;
    originalPrototype := editGE.getPrototype;
  end;
  edit1.text := originalName;
  //repaint to show newly placed element
  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
  form1.PaintBox1Paint(nil);

  addEditFields;

end;




procedure tform2.addEditFields;
var prop: P_ART_DataProp;
  elementName,name,s: string;
  text,text2: Pchar;
  i: integer;
  c: integer;
  model: P_ART_Object;
  strings: tstrings;
  h:integer;
  NewCombo : TComboBox;
  NewEdit : TEdit;
  NewLabel : TLabel;
  range: P_ART_Variant;
begin
  //remove all previously added controls
  for i := edits.count-1 downto 0 do
  begin
    RemoveComponent(Tedit(edits.Items[i]));
    Tedit(edits.Items[i]).Free;
  end;

  for i := labels.count-1 downto 0 do
  begin
    RemoveComponent(Tlabel(labels.Items[i]));
    Tlabel(labels.Items[i]).Free;
  end;

  edits.clear;
  labels.clear;

  editStrings.Clear;

  h := 20;
  i := 0;

  if not (editGE.getARTElement = nil) then
  begin
    model := ARTGetModel(editGE.getARTElement);
    prop := ARTGetDataProperties(model,nil);
    originalParams := '';
    while prop <> nil do
    begin
      //writeln(ARTGetName(prop));
      text := ARTGetDefinitionString(prop);
      if not(text = nil) then
      begin
        //save original parameter strings to restore original values...
        originalParams := originalParams + string(text) + '; ';
        strings := SplitString('=',string(text), nil);
        NewLabel := TLabel.create(self);

        //Check if a range is defined
        range := ARTGetRange( prop );
        if (range <> nil) AND (ARTGetDatatype(range) = C_ART_nstr) then //if the range is of type nstr, add the possible strings to a combobox, do not display the edit!
        begin
          NewCombo := TComboBox.create(self);
          with (NewCombo) do
          begin
            Width  := 75;
            Height  := h;

            Top    := 96 + h * i;

            Left   := 75;
            Parent := self;
            OnKeyDown := EditKeyPress;
            OnExit := EditDone;
            editStrings.Add( Trim(strings[1]) );
            Hint := ARTGetShortDescription( prop );
            ShowHint := true;
            for c := 0 to ARTGetLength((range))-1 do
              Items.Add( ARTGetString((range), c) );
            ItemIndex := Items.IndexOf(Trim(strings[1]));
          end;
          edits.add(NewCombo);
        end
        else
        begin
          NewEdit := TEdit.create(self);
          with (NewEdit) do
          begin
            Width  := 75;
            Height  := h;
            Top    := 96 + h * i;
            Left   := 75;
            Parent := self;
            OnKeyDown := EditKeyPress;
            OnExit := EditDone;
            editStrings.Add( Trim(strings[1]) );
            Hint := ARTGetShortDescription( prop );
            ShowHint := true;
            AutoSelect := false;
            text := Trim(strings[1]);
          end;
          edits.add(NewEdit);
        end;

        with NewLabel do
        begin
          Width  := 75;
          Height  := h;
          Top    := 96 + 3 + h * i;
          Left   := 2;
          Parent := self;
          Hint := ARTGetShortDescription( prop );
          ShowHint := true;
          Caption := Trim(strings[0]);
        end;


        labels.add(NewLabel);
        inc(i);
      end;

      //Get next property
      prop := ARTGetDataProperties(model,prop);
    end;
  end
  else //no prototype selected
  begin
    NewLabel := TLabel.create(self);
    with NewLabel do
    begin
      Width  := 75;
      Height  := h;
      Top    := 96 + 3 + h + h * i;
      Left   := 30;
      Parent := self;
      ShowHint := false;
      Caption := 'Select prototype';
      Font.Style := [fsItalic];
    end;
    labels.add(NewLabel);
    i := 3;
  end;
  button1.Top := 96 + 2 + h * (i);
  button2.Top := 96 + 2 + h * (i);

  form2.height := button1.Top + button1.height + 30;

  show;
  self.x := x;
  self.y := y;
end;

//update labels when element name is changed
procedure tform2.updateLabels;
var prop: P_ART_DataProp;
  elementName,name,s: string;
  text,text2: Pchar;
  i: integer;
  model: P_ART_Object;
  strings: tstrings;
  mylabel : TLabel;
begin
  if not (editGE.getARTElement = nil) then
  begin
    model := ARTGetModel(editGE.getARTElement);
    prop := ARTGetDataProperties(model,nil);
    i := 0;
    while prop <> nil do
    begin
      text := ARTGetDefinitionString(prop);
      if not(text = nil) then
      begin
        //labels must be in same order for this to work...
        strings := SplitString('=',string(text), nil);
        //get the label i of the list
        mylabel := TLabel(labels.items[i]);
        mylabel.Caption := Trim(strings[0]);
        inc(i);
      end;

      //Get next property
      prop := ARTGetDataProperties(model,prop);
    end;
  end;
end;


//This creates and prepares the form when the application is started
procedure TForm2.FormCreate(Sender: TObject);
var
  acElements : P_ART_Property;
  ele : P_ART_Object;
begin
  labels := tlist.create;
  edits := tlist.create;
  editstrings := TStringList.create;

  //get possible models
  acElements := ARTFindProperty(psim, 'prototypeModels');
  ele := ARTGetObjects(P_ART_ListProp(acElements),nil);
  while ele <> nil do begin
    name := ARTGetName(ele);
    comboBox1.Items.AddObject(name, ele); //add element as object to entries
    //get next element
    ele := ARTGetObjects(P_ART_ListProp(acElements), ele);
  end;
  //for hints
  ComboBox1.OnDropDown := ComboDropDown;
  ComboBox1.OnClick := ComboCloseUp;
  hw := TComboItemHint.Create(self);
  //editElement := nil;
end;
{
procedure TForm2.ListBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  APoint: TPoint;
  Index: integer;
  TheObject: TControl;
begin
  APoint.X := X;
  APoint.Y := Y;
  Index := ListBox1.ItemAtPos(APoint, true);
  if index <> oldHintIndex then Application.CancelHint;
  if not (index = -1) then
    ListBox1.Hint := ARTGetShortDescription( P_ART_DataProp(ListBox1.Items.Objects[Index]) )
  else
    ListBox1.Hint := '';
  oldHintIndex := index;
end;
 }
procedure TForm2.Button1Click(Sender: TObject);
var
  text: array[0..100] of char;
  i : integer;
begin
  if edit1.text = '' then begin
    messagebox(form2.Handle,'Please enter a name.', 'Error',mb_ok);
    exit;
  end;
  if combobox1.itemindex = -1 then begin
    messagebox(form2.Handle,'Please select a prototype.', 'Error',mb_ok);
    exit;
  end;


  StrPCopy(text,edit1.Text);
  ARTChangeName(editGE.getARTelement, text);//change element name
  editGE.recreate(edit1.text,1,1); //change display name

  editGE.changePrototype(combobox1.items[combobox1.itemindex]);
  updateLabels;
  
  //Set all the parameters
  for i:= 0 to edits.Count-1 do
    SetParameter( i );

  form2.Hide;
  //repaint all!
  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
  form1.PaintBox1Paint(Sender);
end;

procedure TForm2.Button2Click(Sender: TObject);
var
  text: array[0..1000] of char;
  i: integer;
  s: string;
begin
  //if we have created this element especially for this dialog we will remove it
  if freshElement then
  begin
    ge.extract(editGE);
    editGE.destroy;
    editGE := nil;
  end
  else //restore original name, restore original model and original parameters
  begin
    editGE.recreate(originalName,1,1);
    editGE.changePrototype(originalPrototype);
    // Reset parameters to original values
    StrPCopy(text,originalParams);
    ARTSetParameter(simulator, text);
  end;
  form2.Hide;
end;

procedure TForm2.Edit1Change(Sender: TObject);
begin
  //reflect name changes immidiately
  editGE.recreate(edit1.text,1,1);
  //repaint all!
  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
  form1.PaintBox1Paint(Sender);
end;

//for combobox item hints:
procedure TForm2.ApplicationIdle(sender: TObject; var Done: boolean);
var
  pt         : TPoint;
  w          : thandle;
  ItemBuffer : array[0..256] of Char;
  idx        : Integer;
  s          : string;
  c: tcontrol;
begin
  pt := Mouse.CursorPos;
  w := WindowFromPoint(pt);
  if w = 0 then Exit;
  GetClassName(w, ItemBuffer, SizeOf(ItemBuffer));
  if StrIComp(ItemBuffer, 'ComboLBox') = 0 then
  begin
    Windows.ScreenToClient(w, pt);
    idx := SendMessage(w,LB_ITEMFROMPOINT,0,LParam(PointToSmallPoint(pt)));
    if idx >= 0 then
    begin
      if LB_ERR <> SendMessage(w,LB_GETTEXT,idx,LParam(@ItemBuffer)) then
      begin
        //get the selected item by name
        idx := ComboBox1.Items.IndexOf(ItemBuffer);
        if idx >= 0 then
          s := ARTGetShortDescription( P_ART_DataProp(ComboBox1.Items.Objects[idx]) );
        hw.DoActivateHint(ActiveControl.Name + ItemBuffer,s);
      end;
    end;
  end
  else
  begin
    Application.HideHint;
    hw.ReleaseHandle;
  end;
end; (*ApplicationIdle*)

{ TComboItemHint }
procedure TComboItemHint.DoActivateHint(ControlName : string; Text: string);
var
  pt : TPoint;
  r : TRect;
  xoffset : integer ;
  yoffset : integer ;
begin
  self.ControlName := ControlName;
  if DoHint then
  begin
    pt := Mouse.CursorPos;
    //some hard-coding
    xoffset := 0;
    yoffset := 16;
    r := CalcHintRect(1000, text, nil);
    r := Rect(pt.X + xoffset ,pt.Y + yoffset,pt.X + r.right + xoffset ,pt.Y + r.Bottom + yoffset);
    ActivateHint(r,Text);
    DoHint := false;
  end;
end;

procedure TComboItemHint.SetControlName(const Value: string);
begin
  if FControlName <> Value then
  begin
    ReleaseHandle;
    //remove flicker
    DoHint := True;
    FControlName := Value;
  end;
end;


procedure TForm2.ComboCloseUp(sender: TObject);
begin
  if not Combobox1.DroppedDown then begin
    Application.OnIdle := nil;
    Application.HideHint;
    hw.ReleaseHandle;
  end;
end; (*ComboCloseUp*)

procedure TForm2.ComboDropDown(sender: TObject);
begin
  Application.OnIdle := ApplicationIdle;
end; (*ComboDropDown*)

procedure DoHint(AControl: TControl) ;
var
   pt: TPoint;
begin
   Application.Hint := '';
   Application.ActivateHint(pt) ;
end;

procedure TForm2.ComboBox1Exit(Sender: TObject);
begin
  if not Combobox1.DroppedDown then begin
    Application.OnIdle := nil;
    Application.HideHint;
    hw.ReleaseHandle;
  end;
end;

  {
//List Edit
procedure EditParameter(Listbox: TListbox);
var
  ii, PosReturn: integer;
  lRect: TRect;
begin
  ii := Listbox.itemindex;
  if ii = -1 then exit;

  lRect := ListBox.ItemRect(ii);
  form2.ListEdit.Parent := Listbox;
  form2.ListEdit.Top := lRect.Top ;
  form2.ListEdit.Left := lRect.Left + 0;
  form2.ListEdit.Height := (lRect.Bottom - lRect.Top) + 2;

  editdef := Listbox.Items[ii];
  form2.ListEdit.Text := Listbox.Items[ii];
  form2.ListEdit.Visible := True;
  form2.ListEdit.ShowHint := true;
  form2.ListEdit.Hint := ARTGetShortDescription( P_ART_DataProp(ListBox.Items.Objects[ii]) );
  //select number or text after =
  PosReturn := Pos('=',form2.ListEdit.Text);
  if PosReturn <> 0 then
  begin
    form2.ListEdit.SelStart := PosReturn + 1;
    form2.ListEdit.SelLength := Length(form2.ListEdit.Text);
  end;
  form2.ListEdit.SetFocus;
end;
}

{
procedure Tform2.ListBox1Click(Sender: TObject);
var
  ii, PosReturn: integer;
  lRect: TRect;
begin
  EditParameter(ListBox1);
end;
 }
      {
procedure Tform2.ListEditKeyPress(Sender: TObject;  var Key: Word; Shift: TShiftState);
var
  ii : integer;
  ListBox : TListBox;
begin
  //The listbox object is the parent of the edit, thats how we can access it
  listbox := ListEdit.Parent as TListBox;
  if Key = VK_RETURN then
  begin
    ii := ListBox.ItemIndex;
    ListBox.Items.Delete(ii);
    ListBox.Items.Insert(ii,ListEdit.text);
    ListEdit.Visible := false;
    SetParameter( ListEdit); //hier
    receiveProperties; //hier
    Key := 0;
  end;
  if Key = VK_UP then begin
    SetParameter( ListEdit); //hier
    ii := Listbox.itemindex -1;
    receiveProperties; //hier
    if ii >= 0 then begin
      Listbox1.itemindex := ii;
      EditParameter(ListBox);
    end;
    Key := 0;
  end;
  if Key = VK_DOWN then begin
    SetParameter( ListEdit); //hier
    ii := Listbox.itemindex + 1;
    receiveProperties; //hier
    if ii < Listbox1.Items.Capacity then begin
      Listbox.itemindex := ii;
      EditParameter(ListBox);
    end;
    Key := 0;
  end;
end;

procedure Tform2.ListEditDone(Sender: TObject);
var
  ii : integer;
  ListBox : TListBox;
begin
  //The listbox object is the parent of the edit, thats how we can access it
  listbox := ListEdit.Parent as TListBox;
  ii := ListBox.ItemIndex;
  ListBox.Items.Delete(ii);
  ListBox.Items.Insert(ii,ListEdit.text);
  ListEdit.Visible := false;
  SetParameter(ListEdit); //hier
  receiveProperties; //hier
end;
       }
procedure TForm2.ComboBox1Change(Sender: TObject);
var
  prop: P_ART_Property;
  elementName,name,s: string;
  text,text2: array[0..100] of char;
begin
  if combobox1.itemindex >= 0 then
  begin
    //change or create edit element
    editGE.changePrototype(combobox1.items[combobox1.itemindex]);
{    StrPCopy(text,combobox1.items[combobox1.itemindex]);
    if editElement = nil then
      editElement := ARTCreateElement(simulator,editGE.getname,text)
    else
      ARTChangeElementModel(simulator,editGE.getname,text);
}
//    receiveProperties;
    if not (editGE.getARTElement = nil) then
      addEditFields;
  end;
end;


procedure TForm2.FormDestroy(Sender: TObject);
begin
  labels.Free;
  edits.Free;
  editStrings.Free;
end;

end.
