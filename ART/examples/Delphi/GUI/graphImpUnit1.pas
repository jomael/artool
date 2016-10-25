unit graphImpUnit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls,ARTDefinitions, ComCtrls, Math;

type
  TForm1 = class(TForm)
    ComboBox1: TComboBox;
    ComboBox2: TComboBox;
    ComboBox3: TComboBox;
    ComboBox4: TComboBox;
    ListBox1: TListBox;
    ListBox2: TListBox;
    ListBox3: TListBox;
    ListBox4: TListBox;
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    Edit4: TEdit;
    Label1: TLabel;
    PaintBox1: TPaintBox;
    Button5: TButton;
    ProgressBar1: TProgressBar;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Edit5: TEdit;
    Edit6: TEdit;
    Edit7: TEdit;
    Label8: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Edit8: TEdit;
    procedure FormCreate(Sender: TObject);
    procedure ComboBox1Change(Sender: TObject);
    procedure ComboBox2Change(Sender: TObject);
    procedure ComboBox3Change(Sender: TObject);
    procedure ComboBox4Change(Sender: TObject);
    procedure ListBox1Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure ListBox2Click(Sender: TObject);
    procedure ListBox3Click(Sender: TObject);
    procedure ListBox4Click(Sender: TObject);
    procedure Edit1Exit(Sender: TObject);
    procedure Edit2Exit(Sender: TObject);
    procedure Edit3Exit(Sender: TObject);
    procedure Edit4Exit(Sender: TObject);
    procedure Edit5Exit(Sender: TObject);
    procedure Edit6Exit(Sender: TObject);
    procedure Edit7Exit(Sender: TObject);
    procedure Edit8Exit(Sender: TObject);
    procedure PaintBox1Paint(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormResize(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  psim : P_ART_Object;
  acElements: P_ART_Property;
  el: array[1..4] of P_ART_Object;
  model: array[1..4] of P_ART_Element;
  simulator: P_ART_simulator;
  f_min, f_max, f_step, Nmodes: integer;
  abortpressed : boolean;
  editdef: array[1..4] of string;
  pufferbild: tbitmap;
  myIns: P_ART_Circuit;

implementation
{$R *.DFM}

function progress(X: Double; const msg: Pchar): boolean; cdecl;
begin
  form1.progressbar1.position := round(X * 100);
  //progress := not (abortpressed); Calculation must be run in another thread for this to work
  progress := true;
end;

procedure prepareGraph(bitmap: TBitmap;mmin,mmax:real);
var
  zoom, dfrq, f, dmag, mag: real;
  s: string;
  i, x, y: integer;
begin
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
  zoom := Bitmap.Width / (f_max-f_min);
  repeat
    x := round(f * zoom);
    str(f:4:0,s);
    Bitmap.Canvas.MoveTo(x,0);
    Bitmap.Canvas.LineTo(x,Bitmap.Height-13);
    Bitmap.Canvas.TextOut(x-12,Bitmap.Height-13, s + ' Hz');
    f := f + dfrq;
  until (f >= f_max);

  dmag := 8 * power(10,trunc(log10(mmax - mmin)) - 1);
  mag := dmag*trunc(mmin/dmag);
  zoom := Bitmap.Height / (mmax-mmin);
  repeat
    y := Bitmap.Height - round(mag * zoom);
    if (y>7) and (y<(Bitmap.Height-7)) then begin
      str(mag:0:0,s);
      Bitmap.Canvas.MoveTo(0,y);
      Bitmap.Canvas.LineTo(Bitmap.Width,y);
      Bitmap.Canvas.TextOut(0,y-7, s+' MOhm');
    end;
    mag := mag + dmag;
  until (mag >= mmax);

end;

procedure TForm1.FormCreate(Sender: TObject);
var ele: P_ART_Object;
  name,s: string;
  i:integer;
  text,text2: array[0..100] of char;
begin

  simulator := ARTCreateSimulator('MySimulator', 'FrequencyDomain', 'MultiModal');
	myIns := ARTCreateCircuit(simulator, 'MyInstrument');
  f_min := 10;
  f_max := 1800;
  f_step := 10;
  Nmodes := 1;
  ARTSetProgressFunction(@progress);
  psim := ARTRootObject();
  Combobox1.Items.Add('(none)');
  Combobox2.Items.Add('(none)');
  Combobox3.Items.Add('(none)');
  Combobox4.Items.Add('(none)');
  //get possible models
  acElements := ARTFindProperty(psim, 'prototypeModels');
  ele := ARTGetObjects(P_ART_ListProp(acElements),nil);
  while ele <> nil do begin
    name := ARTGetName(ele);
    Combobox1.Items.Add(name);
    Combobox2.Items.Add(name);
    Combobox3.Items.Add(name);
    Combobox4.Items.Add(name);
    //get next element
    ele := ARTGetObjects(P_ART_ListProp(acElements), ele);
  end;
  //create image buffer
  pufferbild:=tbitmap.Create;
  pufferbild.canvas.create;
  pufferbild.height:=paintbox1.Height;
  pufferbild.width:=paintbox1.Width;
  pufferbild.Canvas.Brush.Create;
  pufferbild.canvas.Brush.Color:=clblack;
  pufferbild.canvas.Brush.Style:=bssolid;
  pufferbild.canvas.Pen.Color := clblack;
  pufferbild.canvas.Rectangle(0,0,paintbox1.width,paintbox1.Height);
  prepareGraph(pufferbild,0,34);

  //create elements for future use
  for i:= 1 to 4 do begin
    //create new model with the name ElementX, where X is a number 1-4
    str(i,s);
    s:='Element'+s;
    StrPCopy(text,s);
    model[i] := ARTCreateElement(simulator, text, 'Cylinder');
    //RecieveProperties(Combobox, Listbox,i);
  end;

end;

function RecieveValueAsString(prop: P_ART_DataProp) : string;
var
  s: string;
  text: array[0..100] of char;
  v:double;
  valid:boolean;
begin
    valid := false;

    case ARTGetDatatype(ARTGetValue(prop)) of
      C_ART_dbl: begin
        v := ARTGetDouble(ARTGetValue(prop),0);
        valid := true;
        end;
      C_ART_flo: begin
        v := ARTGetFloat(ARTGetValue(prop),0);
        valid := true;
        end;
    end;
    if valid then begin
      str(v:5:2, s);
      RecieveValueAsString := s;
    end
      else RecieveValueAsString := '';
end;

procedure RecieveProperties(Combobox: TCombobox; Listbox: TListbox; i:integer);
var prop: P_ART_DataProp;
  elementName,name,s: string;
  text,text2: array[0..100] of char;
begin
    ListBox.Items.Clear;
    str(i,s);
    StrPCopy(text,'Element'+s);
    el[i] := ARTFindObject(P_ART_ListProp(acElements),text);
    prop := ARTGetDataProperties(ARTGetModel(model[i]),nil);
    while prop <> nil do begin
      name := ARTGetName(prop);
      s := RecieveValueAsString(P_ART_dataProp(prop));
      if not(s = '') then
        ListBox.Items.Add(name + ' = ' + s);
      prop := ARTGetDataProperties(ARTGetModel(model[i]),prop);
    end;
end;

procedure CreateModel(Combobox: TCombobox; Listbox: TListbox; i:integer);
var prop: P_ART_Property;
  modelType,name,s: string;
  text,text2: array[0..100] of char;
  added: boolean;
  c:integer;

begin
  modelType := combobox.items[combobox.itemindex];
  //create element name string: ElementX, where X is a number 1-4
  str(i,s);
  s:='Element'+s;
  StrPCopy(text,s);
  StrPCopy(text2,modelType);
  //if a valid type is selected...
  if (combobox.itemindex>0) then begin
    //change model of element
    ARTChangeElementModel(simulator, model[i], text2);
    RecieveProperties(Combobox, Listbox,i);
    //make sure the element is in the circuit:
    //if it is not...
    if ARTGetReferencePosition( myIns,  model[i]) = -1 then
    begin
      //add it. Check which element is in the circuit
      added := false;
      for c:= (i+1) to 4 do
        if ARTGetReferencePosition( myIns,  model[c]) >= 0 then
        begin
          ARTGetReferencePosition( myIns,  model[c]);
          added := true;
          break;
        end;
        //if no element which should be after this one was found, then add this at the end.
        if not( added ) then
          ARTAppendReference( myIns,  model[i]);
    end;
  end
  else //if 'none' is selected make sure the element is not in the circuit
    ARTRemoveReference( myIns,  model[i]);
end;

procedure TForm1.ComboBox1Change(Sender: TObject);
begin
  CreateModel(Combobox1,Listbox1,1);
end;

procedure TForm1.ComboBox2Change(Sender: TObject);
begin
  CreateModel(Combobox2,Listbox2,2);
end;

procedure TForm1.ComboBox3Change(Sender: TObject);
begin
  CreateModel(Combobox3,Listbox3,3);
end;

procedure TForm1.ComboBox4Change(Sender: TObject);
begin
  CreateModel(Combobox4,Listbox4,4);
end;

procedure SetParameter(Listbox: TListbox; Edit: TEdit; i: Integer);
var
  s: string;
  text: array[0..100] of char;
begin
    if model[i] <> nil then begin
      str(i,s);
      s:='Element'+s+'.'+Edit.text;
      StrPCopy(text,s);
      if ARTSetParameter(simulator, text) = nil then begin //if unsuccessful
        //reset to original values
        Edit.text := editDef[i];
        str(i,s);
        s:='Element'+s+'.'+Edit.text;
        StrPCopy(text,s);
        ARTSetParameter(simulator, text);
        //inform user about error
        messagebox(form1.handle,ARTGetLastErrorMessage,'Error when setting parameter', mb_ok);
      end;
    end;
end;




procedure TForm1.Button1Click(Sender: TObject);
begin
  SetParameter(Listbox1, Edit1, 1);
end;


procedure TForm1.Button5Click(Sender: TObject);
var myImpCurve: P_ART_DataProp;
    val: P_ART_Variant;
    i,l: integer;
    tri: T_ART_Tripl;
    mag: array of real;
    xstep, zoom, min, max: real;
    valid: boolean;
    text: array[0..100] of char;
begin
  if (f_max <= f_min) then
    messagebox(form1.handle,'The maximum frequency must be higher than the lower frequency limit.','Invalid frequency range',mb_ok)
  else if (f_step = 0) then
    messagebox(form1.handle,'f_step must be greater than 0.','Invalid frequency range',mb_ok)
  else begin

  //Check if there's at least one element in the circuit
  valid := false;
  for i:= 1 to 4 do
    if (ARTGetReferencePosition(myIns, model[i]) >= 0) then valid := true;

    //if there was at least one element
    if valid then begin
      //calculate and get impedance curve
        progressbar1.visible := true;
        abortpressed := false;
        StrPCopy(text,'InputImpedanceCurve');
        myImpCurve := P_ART_DataProp(ARTFindProperty(myIns,text));
   	  	myImpCurve := ARTInputImpedance(myIns);
        if myImpCurve=nil then
          messagebox(form1.handle,ARTgetLastErrorMessage,'Error when calculating impedance',mb_ok)
        else begin
          val := ARTGetValue(myImpCurve);
          l := ARTGetLength(val);
          if l > 0 then begin
            Setlength(mag, l);

            tri := ARTGetTriple(val, 0);
            mag[0] := -sqrt(sqr(tri.c.re) + sqr(tri.c.im));
            //min := mag[0];
            max := mag[0];
            for i := 1 to l-1 do
            begin
              tri := ARTGetTriple(val, i);
              mag[i] := sqrt(sqr(tri.c.re) + sqr(tri.c.im));
              if mag[i] > max then max := mag[i];
              //if mag[i] < min then min := mag[i];
            end;
            min := 0; //always start at 0 Ohm
            prepareGraph(pufferbild, min / 1000000 , max / 1000000); //use MEGA Ohm for labels
            //draw impedance curve
            pufferbild.canvas.pen.color := rgb(128,255,0);
            i:=0;
            if (max - min) = 0 then zoom := 0 //just in case...
              else zoom := pufferbild.Height / (max - min);
            xstep := pufferbild.Width / l;
            pufferbild.canvas.moveTo(0, pufferbild.Height - round((mag[i]-min) * zoom));
            for i := 1 to l-1 do begin
              pufferbild.canvas.lineTo(round(i*xstep), pufferbild.Height - round((mag[i]-min) * zoom));
            end;
            //draw the picture on the screen
            paintbox1.canvas.draw(0,0,pufferbild);
          end;
        end;
    end;

    progressbar1.visible := false;
  end;
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  SetParameter(Listbox2, Edit2, 2);
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
  SetParameter(Listbox3, Edit3, 3);
end;

procedure TForm1.Button4Click(Sender: TObject);
begin
  SetParameter(Listbox4, Edit4, 4);
end;

procedure TForm1.ListBox1Click(Sender: TObject);
begin
  Edit1.Text := Listbox1.Items[Listbox1.itemindex];
  editdef[1] := edit1.text;
end;

procedure TForm1.ListBox2Click(Sender: TObject);
begin
  Edit2.Text := Listbox2.Items[Listbox2.itemindex];
  editdef[2] := edit1.text;
end;

procedure TForm1.ListBox3Click(Sender: TObject);
begin
  Edit3.Text := Listbox3.Items[Listbox3.itemindex];
  editdef[3] := edit1.text;
end;

procedure TForm1.ListBox4Click(Sender: TObject);
begin
  Edit4.Text := Listbox4.Items[Listbox4.itemindex];
  editdef[4] := edit1.text;
end;

procedure TForm1.Edit1Exit(Sender: TObject);
begin
  SetParameter(Listbox1, Edit1, 1);
  RecieveProperties(Combobox1, Listbox1,1);
end;

procedure TForm1.Edit2Exit(Sender: TObject);
begin
  SetParameter(Listbox2, Edit2, 2);
  RecieveProperties(Combobox2, Listbox2,2);
end;

procedure TForm1.Edit3Exit(Sender: TObject);
begin
  SetParameter(Listbox3, Edit3, 3);
  RecieveProperties(Combobox3, Listbox3,3);
end;

procedure TForm1.Edit4Exit(Sender: TObject);
begin
  SetParameter(Listbox4, Edit4, 4);
  RecieveProperties(Combobox4, Listbox4,4);
end;

procedure TForm1.Edit5Exit(Sender: TObject);
var i,c: integer;
begin
  val(Edit5.text,i,c);
  if (i <= 0) then messagebox(form1.handle,'Please enter a positive value.','Invalid Value',mb_ok)
  else begin
    f_min := i;
   	ARTSetFrequencyRange(simulator, f_min, f_max, f_step);
  end;
end;

procedure TForm1.Edit6Exit(Sender: TObject);
var i,c: integer;
begin
  val(Edit6.text,i,c);
  if (i < 0) then messagebox(form1.handle,'Please enter a positive value greater than 0.','Invalid Value',mb_ok)
  else begin
    f_max := i;
   	ARTSetFrequencyRange(simulator, f_min, f_max, f_step);
  end;
end;

procedure TForm1.Edit7Exit(Sender: TObject);
var i,c: integer;
begin
  val(Edit7.text,i,c);
  if (i < 0) then messagebox(form1.handle,'Please enter a positive value greater than 0.','Invalid Value',mb_ok)
  else begin
    f_step := i;
   	ARTSetFrequencyRange(simulator, f_min, f_max, f_step);
  end;
end;

procedure TForm1.Edit8Exit(Sender: TObject);
var i,c: integer;
begin
  val(Edit8.text,i,c);
  if (i < 0) then messagebox(form1.handle,'Please enter a positive value greater than 0.','Invalid Value',mb_ok)
  else begin
    Nmodes := i;
   	ARTSetNModes(simulator, Nmodes);
  end;
end;

procedure TForm1.PaintBox1Paint(Sender: TObject);
begin
  paintbox1.canvas.draw(0,0,pufferbild);
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  //destroy circuit
  ARTDestroyCircuit(simulator, myIns);
  ARTDestroySimulator(simulator);
  pufferbild.Destroy;
end;

procedure TForm1.FormResize(Sender: TObject);
begin
  paintbox1.canvas.draw(0,0,pufferbild);
end;

end.

