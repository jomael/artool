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

unit ACMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, ToolWin, ComCtrls, StdCtrls, Buttons, Menus, ACCommon, artdefinitions,
  ACGraph, ACProgressDlg;

const
  BackgroundColor = 0;
  elNameFontSize = 10;
  elNameColor = $BBBBBB;
  elBorderColor = $FFFFFF;
  elFillColor = $303030;
  elHighlightNameColor = $BBBB;
  elHighlightBorderColor = $FFFF;
  elHighlightFillColor = $3030;
  elInputColor =  $DDDD00;
  eloutputColor = $DD00DD;
  elBoxLineWidth = 1;
  NewconnectionColor = $00FFFF;
  NewGoodconnectionColor = $00FF00;
  ConnectionColor = $BB6600;
  ConnectionLineWidth = 2;
  markColor = $00FF00;
{
  //white color scheme
  BackgroundColor = $FFFFFF;
  elHighlightNameColor = $BBBB;
  elHighlightBorderColor = $FFFF;
  elHighlightFillColor = $3030;
  elNameFontSize = 10;
  elNameColor = $0;
  elBorderColor = $0;
  elFillColor = $eeeeeee;
  elInputColor =  $DDDD00;
  eloutputColor = $DD00DD;
  elBoxLineWidth = 1;
  NewconnectionColor = $00EEEE;
  NewGoodconnectionColor = $FF;
  ConnectionColor = $444444;
  ConnectionLineWidth = 2;
  markColor = $FF;
}
  grid = 20; //only in use when useGrid is true (see var below)
  ConnectionCurveEdge = 20; //the bigger, the wider the curves

  ms_idle = 0;
  ms_dragelement = 1;
  ms_newconnection = 2;

  elementtouched = 1;
  inputtouched = 2;
  outputtouched = 3;

  maxConnectionsPerInput = 999;
  maxConnectionsPerOutput = 1;

  inputPortsPerElement = 1;
  outputPortsPerElement = 1;

  names : array[0..9] of string =
    ( 'Cylinder', 'Cone', 'Besselhorn', 'ToneHole', 'Zyl2', 'Kon2', 'Bes3', 'TonLoch', 'Zweig', 'Lulu' ) ;
type
  TForm1 = class(TForm)
    PaintBox1: TPaintBox;
    paintboxPopup: TPopupMenu;
    AddElement1: TMenuItem;
    elementPopup: TPopupMenu;
    CalcImpedance: TMenuItem;
    RemoveElement1: TMenuItem;
    Properties1: TMenuItem;
    N1: TMenuItem;
    N2: TMenuItem;
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    New1: TMenuItem;
    Open1: TMenuItem;
    Simulatorproperties1: TMenuItem;
    Simulator1: TMenuItem;
    Removeall1: TMenuItem;
    Simulatorproperties3: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
    N5: TMenuItem;
    Exit1: TMenuItem;
    N7: TMenuItem;
    Openborelist1: TMenuItem;
    Options1: TMenuItem;
    SnaptoGrid1: TMenuItem;
    Help1: TMenuItem;
    About1: TMenuItem;
    OpenDialog1: TOpenDialog;
    OpenDialog2: TOpenDialog;
    procedure FormCreate(Sender: TObject);
    procedure PaintBox1Paint(Sender: TObject);
    procedure PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure BitBtn1Click(Sender: TObject);
    procedure RemoveElement1Click(Sender: TObject);
    procedure CalcImpedanceClick(Sender: TObject);
    procedure AddElement1Click(Sender: TObject);
    procedure Properties1Click(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Open1Click(Sender: TObject);
    procedure Simulator1Click(Sender: TObject);
    procedure New1Click(Sender: TObject);
    procedure SnaptoGrid1Click(Sender: TObject);
    procedure Openborelist1Click(Sender: TObject);
    procedure HowtoUseHelp1Click(Sender: TObject);
    procedure About1Click(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
  private
    PaintboxClicked: tpoint; //used to remember where the user clicked when calling submenus
  public
    { Public declarations }
  end;

  //forward decl
  TgraphicElement = class;
  Tport = class;
  Tconnection = class;

  Tporttype = (input, output);

  Tport = class
      element: tgraphicElement; //pointer to element this port belongs to
      number: integer; //number of this port in the element
      porttype: Tporttype; //the type of this port
      constructor create(el:tgraphicElement; portnumber: integer; ptype: Tporttype);
      function getScreenCoords: tpoint;
      procedure placeWithElement(p: tpoint);
      function getBitmapCoords: tpoint;
      procedure setBitmapCoords(p: tpoint);
      function getEdgeCoords: tpoint; //coordinates used to control curvature of bezier line, they are relative
      procedure setEdgeCoords(p: tpoint);
      function IsTouched(cursor: tpoint): boolean;
      function Isconnected: boolean; //returns true if connected
      function getConnection: tConnection; overload;
      function getConnection(number: integer): tConnection; overload;
      procedure establishConnection(con: tConnection); //the ports connection
      procedure removeConnection(con: tConnection); //the ports connection
      procedure drawConnections(dest: tbitmap); //the ports connection
      function getNumberOfConnections: integer;
      destructor destroy; //also destroy connection -> must free other port
    private
      connections: tlist; //tConnection;
      screencoordinates: tpoint; //the coordinates of this port on the screen
      bitmapcoordinates: tpoint; //the coordinates of this port on the element's bitmap
      edgecoordinates: tpoint; //the coordinates of this port on the element's bitmap
    end;

  TgraphicElement = class
      constructor create(name: string; nIn: integer = 1; nOut: integer = 1; prototype: string = ''); //this is not the constructor, because we need self to point to the objects location in memory
      procedure reCreate(name: string; nIn: integer = 1; nOut: integer = 1; prototype: string = '');
      class procedure highlightAll(highlighted: boolean);
      class procedure highlightGroup(list: tlist;highlighted: boolean);
      procedure highlight(highlighted: boolean);
      function changePrototype(newprototype: string): boolean;
      function getPrototype: string;
      function getARTElement: P_ART_Element;
      function getName: string;
      procedure place(dest: tbitmap; coord: tpoint);
      procedure replace(dest: tbitmap); //just recalculate position, do not draw anything! dest is used for bounds
      procedure repaint(canvas: tcanvas);
      function IsTouched(cursor: tpoint): tobject; //returns touched element or port or nil if nothing is touched
      function getInputPoint(index: integer): tpoint; //weg!
      function getOutputPoint(index: integer): tpoint;//weg!
      function getInputPort(index: integer): tport;
      function getOutputPort(index: integer): tport;
      procedure markOutput(index: integer);
      procedure markInput(index: integer);
      procedure unmark;
      procedure addToCircuit(mainbranch: tlist);
      destructor destroy;
      procedure IsInCircuit(ic: boolean);
    private
      name: string;
      prototype: string;
      box: trect; //the box around the name (screen coordinates)
      iput: array of tport; //the points representing input and output ports
      oput: array of tport;
      nInput: integer; //number of input and output ports
      nOutput: integer;
      bitmap: tbitmap; //once the graphical representation is created, it is stored here
      ARTelement: P_ART_Element;
      highlighted: boolean; //true if the element is highlighted
      inCircuit: boolean;
  end;

  Tconnection = class
      //procedure connect(startel:PgraphicElement; oport: integer; endel: PgraphicElement; iport:integer);
      constructor create;
      class function IsValidConnection(port1, port2: tport): boolean;//checks if connection between two given ports is valid
      function IsValidSecondPort(port:tport): boolean;
      procedure connect(port1,port2: tport); //add reference to ports
      procedure tempConnect(port: tPort);
      procedure draw(canvas: tcanvas);
      procedure drawToPoint(canvas: tcanvas; p:tpoint);
      function getInputElement: tgraphicElement;
      function getOutputElement: tgraphicElement;
      function getInputport: tport;
      function getOutputport: tport;
      destructor destroy; //remove reference to this connection from ports!
    private
      oport : tport;
      iport : tport;
      registeredConnection: boolean;
      validconnection: boolean;
      //pstartelement: PgraphicElement; //points to the graphicelement where the connection starts (from output)
      //pendelement: PgraphicElement;  //points to the graphicelement where the connection ends (at input)
      //startport: integer;  //index of output point
      //endport: integer; //index of input point
  end;

  //Calculation is executed in seperate thread
  Tcalculation = class(TThread)
  private
  protected
    procedure Execute; override;
    procedure Done(sender: TObject);
  public
    result: P_ART_DataProp;
    circuit: P_ART_Circuit;
  end;

procedure repaintgraphicActivebuffer;
procedure repaintgraphicBGbuffer(omitElement: tgraphicElement );



var
  Form1: TForm1;
  elementImages: array of timage;
  ge: tlist;// array[0..10] of TgraphicElement;
  connections: tlist;
  newconnection: Tconnection;
  mousestate: integer = ms_idle;
  activeElement: TgraphicElement = nil;
  connect_start: tpoint;
  markport: tpoint;
  graphicBGbuffer: tbitmap;
  graphicActiveBuffer: tbitmap;
  calculation: tcalculation;
  branchCircuits: tlist;
  useGrid: boolean = false;
  curveCaption: string;
  
procedure readInstrumentFile(filename: string; var branch: tlist);
function createName(proposed: string): string;

implementation

uses ACElementProp, ACSimProp, ACAbout;


{$R *.DFM}

constructor Tport.create(el:tgraphicElement; portnumber: integer; ptype: Tporttype);
begin
  connections := tlist.create; //by default a port is not connected
  connections.Clear;
  element := el;
  number := portnumber;
  porttype := ptype;
end;

function Tport.Isconnected: boolean; //returns true if connected
begin
  if connections.Count = 0 then IsConnected := false
    else IsConnected := true;
end;

function Tport.getConnection: tConnection;
begin
  if connections.Count <> 0 then
    getConnection := connections.items[0]
  else getConnection := nil;
end;

function Tport.getConnection(number: integer): tConnection;
begin
  if connections.Count > number then
    getConnection := connections.items[number]
  else getConnection := nil;
end;

procedure Tport.establishConnection(con: tConnection); //the ports connection
begin
  if con = nil then raise Exception.Create('Error: connection is nil');
  connections.Add(con);
end;

procedure Tport.removeConnection(con: tConnection); //the ports connection
begin
  if con = nil then raise Exception.Create('Error: connection is nil');
  connections.Extract(con);
end;

destructor Tport.destroy; //also destroy connection -> must free other port
var
  cn : tconnection;
  i: integer;
begin
  //note that this loop has to run backwards. it seems elements are automatically removed from a list when they are destroyed...
  for i := self.connections.count -1 downto 0 do
  begin
    cn := self.connections.items[i];
    //remove from global connections
    ACMain.connections.extract(cn);
    cn.destroy;
  end;
  connections.Clear;
  connections.destroy;
end;

function Tport.getScreenCoords: tpoint;
begin
  getScreenCoords := screencoordinates;
end;

procedure Tport.placeWithElement(p: tpoint);
begin
  screencoordinates.x := p.x + Bitmapcoordinates.x;
  screencoordinates.y := p.y + Bitmapcoordinates.y;
end;

function Tport.getBitmapCoords: tpoint;
begin
  getBitmapCoords := Bitmapcoordinates;
end;

procedure Tport.setBitmapCoords(p: tpoint);
begin
  Bitmapcoordinates := p;
end;

function Tport.getEdgeCoords: tpoint;
begin
  getEdgeCoords := Edgecoordinates;
end;

procedure Tport.setEdgeCoords(p: tpoint);
begin
  Edgecoordinates := p;
end;

function Tport.IsTouched(cursor: tpoint): boolean; //-> not placed, input, output, box, nottuched
begin
  IsTouched := false;
  if (screencoordinates.x-4 < cursor.x) AND (cursor.x < screencoordinates.x+4) AND
     (screencoordinates.y-4 < cursor.y) AND (cursor.y < screencoordinates.y+4) then IsTouched := true;
end;

procedure Tport.drawConnections(dest: tbitmap);
var
  i: integer;
  c: tconnection;
begin
  for i:= 0 to connections.Count - 1 do begin
    c := connections.Items[i];
    c.draw(dest.canvas);
  end;
end;

function Tport.getNumberOfConnections: integer;
begin
  getNumberOfConnections := connections.count;
end;

constructor TConnection.Create;
begin
  oport := nil;
  iport := nil;
  registeredConnection := false;
  validconnection := false;
end;

//checks if connection between two given ports is valid
class function TConnection.IsValidConnection(port1, port2: tport): boolean;
begin
  IsValidConnection := false; //lets assume its not valid
  //both ports must not be nil
  if (port1 = nil) OR (port2 = nil) then exit; //right, its not valid
  //One port must be input, the other output
  if port1.porttype = port2.porttype then exit; //right, its not valid
  //Both ports must belong to different elements
  if port1.element = port2.element then exit; //right, its not valid
  //Both ports must be free
  //if port1.Isconnected OR port2.Isconnected then begin writeln('4');exit;
  //The output port must be free, the input can have more than one connection
  if port1.porttype = output then
  begin
    if port1.getNumberOfConnections >= maxConnectionsPerOutput then exit;
    if port2.getNumberOfConnections >= maxConnectionsPerInput then exit;
  end;
  if port2.porttype = output then
  begin
    if port1.getNumberOfConnections >= maxConnectionsPerInput then exit;
    if port2.getNumberOfConnections >= maxConnectionsPerOutput then exit;
  end;
  //if none of the above conditions was true, the connection is valid
  IsValidConnection := true;
end;

//checks if connection of already set port to given port is valid
function TConnection.IsValidSecondPort(port: tport): boolean;
begin
  validconnection := false;
  IsValidSecondPort := false; //lets assume its not valid
  //exactly one port must be nil
  if not((oport = nil) XOR (iport = nil)) then begin {writeln('1');}exit; end; //right, its not valid
  //find valid port and check connection validity
  if (oport <> nil) then
    validconnection := TConnection.IsValidConnection(oport, port)
  else
    validconnection := TConnection.IsValidConnection(iport, port);
  IsValidSecondPort := validconnection;
end;

procedure Tconnection.connect(port1, port2: tport);
begin
  //die verbindung bei ports registrieren. aber vorsicht, die verbindungen dürfen
  //sich nicht überschreiben, weil sonst löscht destroyer später falsche verbindung
  //bzw. wenn verbindung überschrieben wird, dann muss sie überall gelöscht werden!!!
  if (port1 <> nil) AND (port2 <> nil) then
  begin
    if (port1.porttype = output) AND (port2.porttype = input) then
    begin
      oport := port1;
      iport := port2;
    end else
    if (port1.porttype = input) AND (port2.porttype = output) then
    begin
      oport := port2;
      iport := port1;
    end
    else begin
      //writeln('ERROR: Both Ports of same type?');
      exit;
    end;
    oport.establishconnection(self);
    iport.establishconnection((self));
    registeredconnection := true;
  end;
  //else
    //writeln('ERROR: One port was nil');
  validconnection := true;
end;

procedure Tconnection.tempConnect(port: tPort);
begin
    if (port.porttype = output) then
    begin
      oport := port;
      iport := nil;
      registeredconnection := false;
    end else
    if (port.porttype = input) then
    begin
      oport := nil;
      iport := port;
      registeredconnection := false;
    end
    else begin
      //writeln('ERROR: Unknown porttype');
      exit;
    end;
  validconnection := false;
end;

procedure Tconnection.draw(canvas: tcanvas);
var
  p1: tpoint;
  p2: tpoint;
  pp: array[1..4] of tpoint;
begin
  canvas.pen.color := ConnectionColor;
  canvas.pen.width := ConnectionLineWidth;
  if oport = nil then exit; //both must be valid for this function
  if iport = nil then exit;
  p1 := oport.getScreenCoords;
  p2 := iport.getScreenCoords;
  pp[1]:=p1;
  pp[2].x := p1.x + oport.getEdgeCoords.x;
  pp[2].y := p1.y + oport.getEdgeCoords.y;
  pp[3].x := p2.x + iport.getEdgeCoords.x;
  pp[3].y := p2.y + iport.getEdgeCoords.y;
  pp[4]:=p2;
{  pp[1]:=p;
  pp[2]:=p; pp[2].x := (p.x + p1.x) div 2;
  pp[3]:=p1;pp[3].x := (p.x + p1.x) div 2;
  pp[4]:=p1;}
  canvas.PolyBezier(pp);
end;

{
procedure Tconnection.draw(canvas: tcanvas);
var
  p1: tpoint;
  p2: tpoint;
begin
  canvas.pen.color := ConnectionColor;
  canvas.pen.width := ConnectionLineWidth;
  if oport = nil then exit; //both must be valid for this function
  if iport = nil then exit;
  p1 := oport.getScreenCoords;
  p2 := iport.getScreenCoords;
  canvas.moveto(p1.x,p1.y);
  canvas.lineto(p2.x,p2.y);
end;

procedure Tconnection.drawToPoint(canvas: tcanvas; p:tpoint); //this must be a new connection, because one point is missing
var
  p1: tpoint;
begin
  if validconnection then
    canvas.pen.color := NewGoodconnectionColor
  else
    canvas.pen.color := NewconnectionColor;
  validconnection := false; //this connection has to be re evaluated before the next drawing
  canvas.pen.width := ConnectionLineWidth;
  if (oport = nil) AND (iport = nil) then exit; //exactly one must be invalid!
  if (oport <> nil) AND (iport <> nil) then exit;
  if oport = nil then begin
    p1 := iport.getScreenCoords;
    canvas.moveto(p.x,p.y);
    canvas.lineto(p1.x,p1.y);
  end else begin
    p1 := oport.getScreenCoords;
    canvas.moveto(p.x,p.y);
    canvas.lineto(p1.x,p1.y);
  end;
end;
}

procedure Tconnection.drawToPoint(canvas: tcanvas; p:tpoint); //this must be a new connection, because one point is missing
var
  p1: tpoint;
  pp: array[1..4] of tpoint;
begin
  if validconnection then
    canvas.pen.color := NewGoodconnectionColor
  else
    canvas.pen.color := NewconnectionColor;
  validconnection := false; //this connection has to be re evaluated before the next drawing
  canvas.pen.width := ConnectionLineWidth;
  if (oport = nil) AND (iport = nil) then exit; //exactly one must be invalid!
  if (oport <> nil) AND (iport <> nil) then exit;
  if oport = nil then begin
    p1 := iport.getScreenCoords;
    pp[1]:=p1;
    pp[2].x := p1.x + iport.getEdgeCoords.x;
    pp[2].y := p1.y + iport.getEdgeCoords.y;
    pp[3]:=p;
    pp[4]:=p;
  end else begin
    p1 := oport.getScreenCoords;
    pp[1]:=p1;
    pp[2].x := p1.x + oport.getEdgeCoords.x;
    pp[2].y := p1.y + oport.getEdgeCoords.y;
    pp[3]:=p;
    pp[4]:=p;
  end;
{  pp[1]:=p;
  pp[2]:=p; pp[2].x := (p.x + p1.x) div 2;
  pp[3]:=p1;pp[3].x := (p.x + p1.x) div 2;
  pp[4]:=p1;}
  canvas.PolyBezier(pp);
end;

function Tconnection.getInputElement: tgraphicElement;
begin
  if iport = nil then getInputElement := nil else
  getInputElement := iport.element;
end;

function Tconnection.getOutputElement: tgraphicElement;
begin
  if oport = nil then getOutputElement := nil else
  getOutputElement := oport.element;
end;

function Tconnection.getInputport: tPort;
begin
  getInputport := iport;
end;

function Tconnection.getOutputport: tPort;
begin
  getOutputport := oport;
end;

destructor Tconnection.destroy;
begin
  //remove reference to this connection from ports, if it was registered
  if registeredConnection then begin
    if oport <> nil then oport.removeConnection(self);
    if iport <> nil then iport.removeConnection(self);
  end;
end;

procedure repaintgraphicBGbuffer(omitElement: tgraphicElement );
var
  i: integer;
  c: tconnection;
  g: tgraphicElement;
  dim: TSize;
begin
  //clear
  graphicBGbuffer.canvas.brush.color := BackgroundColor;
  graphicBGbuffer.canvas.brush.style := bsSolid;
  graphicBGbuffer.canvas.fillrect(graphicBGbuffer.canvas.cliprect);
  //paint elements (all, except omitElement)...
  if ge.count > 0 then begin
    for i := 0 to ge.count-1 do
    begin
      g := tobject(ge.items[i]) as tgraphicElement;
      if not(omitElement = g) then g.repaint(graphicBGbuffer.canvas);
    end;
    //... and all connections, except those to omit element
    for i:= 0 to connections.Count - 1 do begin
      c := connections.Items[i];
      if (c.getInputElement <> omitElement) AND
         (c.getOutputElement <> omitElement) then
          c.draw(graphicBGBuffer.Canvas);
    end;
  end
  else //if empty
  begin
    graphicBGbuffer.Canvas.font.Color := elNameColor;
    graphicBGbuffer.Canvas.font.Style := [ fsItalic ];
    dim := graphicBGbuffer.Canvas.TextExtent('Right click to add elements');
    graphicBGbuffer.canvas.textOut((graphicBGbuffer.Width - dim.cx) div 2, (graphicBGbuffer.height - dim.cy) div 2,
                                   'Right click to add elements');
  end;

end;

procedure repaintgraphicActivebuffer;
var
  i: integer;
begin
  graphicActivebuffer.canvas.Draw(0,0,graphicBGbuffer);
  if markport.x > -1 then
  begin
    graphicActiveBuffer.Canvas.pen.width := 1;
    graphicActiveBuffer.Canvas.pen.color := markColor;
    graphicActiveBuffer.Canvas.brush.style := bsClear;
    graphicActiveBuffer.Canvas.rectangle(markport.x-5,markport.y-5,markport.x+5,markport.y+5);
  end;
end;

procedure TForm1.FormCreate(Sender: TObject);
var
  i, x, y: integer;
  bitmap: tBitmap;
  g: tgraphicelement;
begin
  psim := ARTRootObject;
  ARTSetProgressFunction(@progress);
  simulator := ARTCreateSimulator('MySimulator', 'FrequencyDomain', 'MultiModal');

  f_min := ARTGetDouble( ARTGetValue( ARTFindDataProperty(simulator,'LowerFrequencyLimit') ), 0 );
  f_max := ARTGetDouble( ARTGetValue( ARTFindDataProperty(simulator,'HigherFrequencyLimit') ), 0 );
  f_step := ARTGetDouble( ARTGetValue( ARTFindDataProperty(simulator,'FrequencyStep') ), 0 );
  Nmodes := ARTGetInteger( ARTGetValue( ARTFindDataProperty(simulator,'NumberOfModes') ), 0 );

  //Create main circuit and add it to the circuits list (it will be at index 0)
  mainCircuit := ARTCreateCircuit(simulator, 'MainCircuit');
  branchCircuits :=  TList.Create;

  connections := TList.Create;
  graphicBGbuffer := tbitmap.create;
  graphicBGbuffer.Width := paintbox1.Width;
  graphicBGbuffer.height := paintbox1.height;
  graphicActivebuffer := tbitmap.create;
  graphicActivebuffer.Width := paintbox1.Width;
  graphicActivebuffer.height := paintbox1.height;
  markport := point(-1,-1);
  ge := tlist.create;
{  for i := 0 to 3 do begin
    x:=random(paintbox1.width);
    y:=random(paintbox1.height);
    g := TgraphicElement.create(names[i],inputPortsPerElement,outputPortsPerElement);
    g.changePrototype(names[i]);
    g.place(graphicBGbuffer,point(x,y));
    ge.add(g);
  end;        }

{  //Debug: add elements
  g := TgraphicElement.create('Cyl1');
  g.changePrototype('Cylinder');
  g.place(graphicBGbuffer,point(100,100));
  ge.add(g);
  g := TgraphicElement.create('Cyl2');
  g.changePrototype('Cylinder');
  g.place(graphicBGbuffer,point(200,100));
  ge.add(g);
  g := TgraphicElement.create('Cyl3');
  g.changePrototype('Cylinder');
  g.place(graphicBGbuffer,point(300,100));
  ge.add(g);    }

  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
end;

procedure TForm1.PaintBox1Paint(Sender: TObject);
var
  i: integer;
  dim: TSize;
begin
  paintbox1.Canvas.Draw(0,0,graphicActivebuffer);
end;




constructor TgraphicElement.create(name: string; nIn, nOut: integer; prototype: string);
var
  i: integer;
begin
  self.inCircuit := false;
  self.prototype := '';
  self.name := name;
  bitmap := tbitmap.Create;
  nInput:=nIn;
  nOutput:=nOut;
  setlength(oput,nOutput);
  setlength(iput,nInput);
  //writeln(IntToHex(integer((self)),1));
  for i := 0 to nOutput-1 do oput[i] := tport.create((self),i,output);
  for i := 0 to nInput-1 do iput[i] := tport.create((self),i,input);
  reCreate(name, nIn, nOut, prototype);
end;

function TgraphicElement.changePrototype(newprototype: string): boolean;
var
  nam: array[0..100] of char;
  pro: array[0..100] of char;
begin
  if (newprototype <> prototype) then
    begin
    StrPCopy(nam,name);
    StrPCopy(pro,newprototype);

    if ARTElement = nil then
      ARTElement := ARTCreateElement(simulator,nam,pro)
    else
      ARTElement := ARTChangeElementModel(simulator,ARTElement,pro);
    //check if there was an error
    if ARTElement = nil then
    begin
      messagebox(getactivewindow, ARTGetLastErrorMessage, 'Error', mb_ok);
      changePrototype := false;
    end
    else
      prototype := newprototype;
  end;
end;

function TgraphicElement.getARTElement: P_ART_Element;
begin
  getARTElement := ARTElement;
end;

procedure TgraphicElement.reCreate(name: string; nIn, nOut: integer; prototype: string);
var
  i: integer;
  dim: TSize;
  rect: trect;
  padding_x: integer;
  padding_y: integer;
  margin_x: integer;
  margin_y: integer;
  screencoord: tpoint;
begin
  self.name := name;
  if self.name = '' then self.name := ' ';
  nInput:=nIn;
  nOutput:=nOut;
  setlength(oput,nOutput);
  setlength(iput,nInput);
  bitmap.Canvas.font.Size:=elNameFontSize;
  bitmap.Canvas.font.Style := [ ];
  dim := bitmap.Canvas.TextExtent(self.name);
  margin_x := 2;
  margin_y := 2;
  padding_x := 5;
  padding_y := 3;
  rect.left := margin_x;
  rect.top  := margin_y;
  rect.right := margin_x + padding_x + dim.cx + padding_x + 1;
  rect.bottom:= margin_y + padding_y + dim.cy + padding_y + 1;
  //set dimensions of bitmap
  bitmap.width := margin_x + rect.right;
  bitmap.height := margin_y + rect.bottom;
  //clear bitmap
  bitmap.Canvas.pen.color := BackgroundColor;
  bitmap.Canvas.brush.color := BackgroundColor;
  bitmap.Canvas.brush.style := bsSolid;
  bitmap.Canvas.pen.width := elBoxLineWidth;
  bitmap.Canvas.Rectangle(0,0,bitmap.width,bitmap.height);
  bitmap.Canvas.TextOut(margin_x + padding_x,margin_y + padding_y,self.name);
  //draw element
  if not(highlighted) then begin
    bitmap.Canvas.font.Color:=elNameColor;
    bitmap.Canvas.brush.color := elFillColor;
    bitmap.Canvas.pen.Color := elBorderColor;
  end
  else
  begin
    bitmap.Canvas.font.Color:=elHighlightNameColor;
    bitmap.Canvas.brush.color := elHighlightFillColor;
    bitmap.Canvas.pen.Color := elHighlightBorderColor;
  end;
  bitmap.Canvas.RoundRect(rect.left,rect.top, rect.right,rect.bottom, (rect.right-rect.left) div 5, (rect.bottom-rect.top) div 5);
  //bitmap.Canvas.Rectangle(rect);
  bitmap.Canvas.TextOut(margin_x + padding_x,margin_y + padding_y,self.name);
  //input and output
  bitmap.Canvas.brush.color := elInputColor;
  bitmap.Canvas.brush.style := bsSolid;
  bitmap.Canvas.pen.Color := elInputColor;
  for i:=0 to nInput-1 do begin
    if i = 0 then begin
      iput[i].setBitmapCoords(Point(rect.right,rect.top+(rect.bottom-rect.top) div 2));
      iput[i].setEdgeCoords(Point(ConnectionCurveEdge,0));
    end;
    if i = 1 then begin
      iput[i].setBitmapCoords(Point(rect.left+(rect.right-rect.left) div 2,rect.top));
      iput[i].setEdgeCoords(Point(0,-ConnectionCurveEdge));
    end;
    //if i > 1 then writeln('TRYING TO ADD MORE THAN TWO INPUTS');
    bitmap.Canvas.Ellipse(iput[i].getBitmapCoords.x-2,iput[i].getBitmapCoords.y-2,
                          iput[i].getBitmapCoords.x+2,iput[i].getBitmapCoords.y+2);
  end;


  bitmap.Canvas.brush.color := eloutputColor;
  bitmap.Canvas.brush.style := bsSolid;
  bitmap.Canvas.pen.Color := elOutputColor;
  for i:=0 to nOutput-1 do begin
    if i = 0 then begin
      oput[i].setBitmapCoords(Point(rect.left,rect.top+(rect.bottom-rect.top) div 2));
      oput[i].setEdgeCoords(Point(-ConnectionCurveEdge,0));
    end;
    if i = 1 then begin
      oput[i].setBitmapCoords(Point(rect.left+(rect.right-rect.left) div 2,rect.bottom));
      oput[i].setEdgeCoords(Point(0,ConnectionCurveEdge));
    end;
    //if i > 1 then writeln('TRYING TO ADD MORE THAN TWO OUTPUTS');
    bitmap.Canvas.Ellipse(oput[i].getBitmapCoords.x-2,oput[i].getBitmapCoords.y-2,
                          oput[i].getBitmapCoords.x+2,oput[i].getBitmapCoords.y+2);
  end;

  //if box dimensions have changed, the port coordinates need to be recalculated
  screencoord.x := box.left;
  screencoord.y := box.top;

  box.right := box.left + bitmap.width;
  box.bottom:= box.top  + bitmap.height;
  //update screencoordinates of ports and draw connections, if ports are connected
  for i := 0 to nOutput-1 do begin
    oput[i].placeWithElement(screencoord);
  end;
  for i := 0 to nInput-1 do begin
    iput[i].placeWithElement(screencoord);
  end;

  //create ARTelement if prototype is provided
  if not(prototype = '') then
    changePrototype( prototype );
end;

function TgraphicElement.getName;
begin
  getName := name;
end;

procedure TgraphicElement.place(dest: tbitmap; coord: tpoint);
var
  i: integer;
  screencoord: tpoint;
begin
  if useGrid then begin
    screencoord.x := grid div 2 + (coord.x div grid)*grid - bitmap.width div 2;
    screencoord.y := grid div 2 + (coord.y div grid)*grid - bitmap.height div 2;
  end else begin
    screencoord.x := coord.x - bitmap.width div 2;
    screencoord.y := coord.y - bitmap.height div 2;
  end;
  if screencoord.x < 0 then screencoord.x := 0;
  if screencoord.y < 0 then screencoord.y := 0;
  //writeln('test',(screencoord.x + bitmap.width),' > ', dest.canvas.ClipRect.right);
  if ((screencoord.x + bitmap.width) > dest.width) then screencoord.x := dest.width - bitmap.width;
  if (screencoord.y + bitmap.height) > dest.height then screencoord.y := dest.height - bitmap.height;

  box.left := screencoord.x;
  box.top  := screencoord.y;
  box.right := box.left + bitmap.width;
  box.bottom:= box.top  + bitmap.height;

  dest.canvas.Draw(box.left,box.top, bitmap);
  //update screencoordinates of ports and draw connections, if ports are connected
  for i := 0 to nOutput-1 do begin
    oput[i].placeWithElement(screencoord);
    oput[i].drawConnections(dest);
  end;

  for i := 0 to nInput-1 do begin
    iput[i].placeWithElement(screencoord);
    iput[i].drawConnections(dest);
  end;
end;

procedure TgraphicElement.replace(dest: tbitmap);
var
  i: integer;
  screencoord: tpoint;
begin
  screencoord.x := box.left;
  screencoord.y := box.top;
  if useGrid then begin
    screencoord.x := grid div 2 + (screencoord.x div grid)*grid ;
    screencoord.y := grid div 2 + (screencoord.y div grid)*grid ;
  end;
  if screencoord.x < 0 then screencoord.x := 0;
  if screencoord.y < 0 then screencoord.y := 0;
  //writeln('test',(screencoord.x + bitmap.width),' > ', dest.canvas.ClipRect.right);
  if (screencoord.x + bitmap.width) > dest.width then screencoord.x := dest.width - bitmap.width;
  if (screencoord.y + bitmap.height) > dest.height then screencoord.y := dest.height - bitmap.height;

  box.left := screencoord.x;
  box.top  := screencoord.y;
  box.right := box.left + bitmap.width;
  box.bottom:= box.top  + bitmap.height;

  //update screencoordinates of ports and draw connections, if ports are connected
  for i := 0 to nOutput-1 do begin
    oput[i].placeWithElement(screencoord);
  end;

  for i := 0 to nInput-1 do begin
    iput[i].placeWithElement(screencoord);
  end;
end;

procedure TgraphicElement.repaint(canvas: tcanvas);
begin
  canvas.Draw(box.left,box.top, bitmap);
end;

function TgraphicElement.IsTouched(cursor: tpoint): tobject; //-> not placed, input, output, box, nottuched
var
  i: integer;
  x: integer;
  y: integer;
begin
  IsTouched := nil;
  if (box.left < cursor.x) AND (cursor.x < box.right) AND
     (box.top  < cursor.y) AND (cursor.y < box.bottom) then IsTouched := self;
  for i:=0 to nOutput-1 do if oput[i].IsTouched(cursor) then IsTouched := oput[i];
  for i:=0 to nInput -1 do if iput[i].IsTouched(cursor) then IsTouched := iput[i];
end;

function TgraphicElement.getInputPoint(index: integer): tpoint;
begin
//  getInputPoint.x := iput[index].x + box.Left;
//  getInputPoint.y := iput[index].y + box.Top;
  getInputPoint := iput[index].getscreencoords;
end;

function TgraphicElement.getOutputPoint(index: integer): tpoint;
begin
//  getOutputPoint.x := oput[index].x + box.Left;
//  getOutputPoint.y := oput[index].y + box.Top;
  getOutputPoint := oput[index].getscreencoords;
end;

function TgraphicElement.getInputPort(index: integer): tport;
begin
  getInputPort := (iput[index]);
end;

function TgraphicElement.getOutputPort(index: integer): tport;
begin
  getOutputPort := (oput[index]);
end;

function TgraphicElement.getPrototype: string;
begin
  getPrototype := prototype;
end;

procedure TgraphicElement.markOutput(index: integer);
begin
end;

procedure TgraphicElement.markInput(index: integer);
begin
end;

procedure TgraphicElement.unmark;
begin
end;

destructor TgraphicElement.destroy;
var i: integer;
begin
  for i:=0 to nInput-1 do iput[i].destroy;
  for i:=0 to nOutput-1 do oput[i].destroy;
  if ARTelement <> nil then ARTDestroyElement(simulator, ARTelement);
  bitmap.destroy;
end;

function findTouchedElement(point: tpoint): tobject;
var
  i: integer;
  touchedObject: tobject;
  g: tgraphicelement;
begin
  touchedObject := nil;

  for i := 0 to ge.count-1 do
  begin
    g := tobject(ge.items[i]) as tgraphicElement;
    touchedObject := g.IsTouched(point);
    if touchedObject <> nil then break;
  end;

  findTouchedElement := touchedObject
end;

procedure TForm1.PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  i: integer;
  rx: integer;
  ry: integer;
  o:tobject;
  port:tport;
  port2:tport;
  el:TgraphicElement;
begin
  o := findTouchedElement(point(x,y));
  if (o is TgraphicElement) then begin
    if Button = mbLeft then begin
      el := o as TgraphicElement;
      mousestate := ms_dragelement;
      activeElement := el;
      repaintgraphicBGbuffer( el);
    end
    else
    if button = mbRight then
    begin
      el := o as TgraphicElement;
      activeElement := el;
      cursor := crArrow;
      elementPopup.Popup(mouse.CursorPos.x,mouse.CursorPos.y);
    end;
  end;
  if (o is Tport) then begin
    port := (o as tport);
    if not port.Isconnected then begin
      mousestate := ms_newconnection;
      newconnection := tconnection.create;
      newconnection.tempConnect(o as tport);
      repaintgraphicBGbuffer( nil);
    end else
    begin
      //delete this connection -> find it in the list
      connections.extract(port.getConnection);
      //get this other port, not the one that the user clicked
      if port <> port.getConnection.getInputport then
        port2 := port.getConnection.getInputport
      else
        port2 := port.getConnection.getOutputport;
      //now delete the connection...
      port.getConnection.destroy;
      //...and create a new one, using the other port
      mousestate := ms_newconnection;
      newconnection := tconnection.create;
      newconnection.tempConnect(port2);
      repaintgraphicBGbuffer( nil);
    end;
  end;
  if (o = nil)and(button = mbRight) then begin
    Paintboxclicked.x := x;
    Paintboxclicked.y := y;
    paintboxPopup.Popup(mouse.CursorPos.x,mouse.CursorPos.y);
  end;

  PaintBox1MouseMove(Sender, Shift, X, Y);
end;

procedure TForm1.PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  touching: integer;
  i:integer;
  o:tobject;
  port:tport;
begin
  case mousestate of
    ms_idle:
      begin
        o := findTouchedElement(Point(x,y));
        if o is TgraphicElement then cursor := crDrag else
        if o is TPort then cursor := crCross else
          cursor := crArrow;
      end;

    ms_dragelement:
      begin
        repaintgraphicActivebuffer;
        activeElement.place(graphicActiveBuffer,point(x,y));
        PaintBox1Paint(Sender);
      end;

    ms_newconnection:
      begin
        o := findTouchedElement(Point(x,y));
        markport := Point(-1,-1); //don't mark anything, per default
        if o is TPort then begin
          port := o as tport;
          if newconnection.IsValidSecondPort((port)) then
              markport := port.getScreenCoords; //only when a matching port is being touched, mark it
        end;
        //if element is touched, check if output or input port would be a valid match
        if o is TgraphicElement then begin
          port := (o as TgraphicElement).getOutputPort(0);
          if newconnection.IsValidSecondPort((port)) then
              markport := port.getScreenCoords
          else begin
            port := (o as TgraphicElement).getInputPort(0);
            if newconnection.IsValidSecondPort((port)) then
                markport := port.getScreenCoords
          end;
        end;
        repaintgraphicActivebuffer;
        if markport.x > -1 then
          newconnection.drawToPoint(graphicActivebuffer.canvas,markport)
        else
          newconnection.drawToPoint(graphicActivebuffer.canvas,Point(x,y));
        PaintBox1Paint(Sender);
      end;
  end;
end;

procedure TForm1.PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  touching: integer;
  i:integer;
  port1: tPort;
  port2: tPort;
  o: tobject;
  nc: tconnection;
begin
  case mousestate of
    ms_newconnection:
      begin
        port1 := nil;
        markport := Point(-1,-1); //don't mark anything anymore
        //get touched element or port
        o := findTouchedElement(Point(x,y));

        //if element is touched, check if output or input port would be a valid match and get port
        if o is TgraphicElement then begin
          port1 := (o as TgraphicElement).getOutputPort(0);
          if not(newconnection.IsValidSecondPort(port1)) then
          begin
            port1 := (o as TgraphicElement).getInputPort(0);
            if not(newconnection.IsValidSecondPort(port1)) then port1 := nil;
          end;
        end;

        //if a port is touched, use the port directly if it is a valid second port
        if o is TPort then
          if newconnection.IsValidSecondPort(o as tport) then port1 := o as tport;

        //now make the connection, if a valid port was found
        if not(port1 = nil) then begin
            //get the first port of the new connection
            if (newconnection.getOutputPort <> nil) then port2 := newconnection.getOutputPort;
            if (newconnection.getInputPort <> nil) then port2 := newconnection.getInputPort;
            //and create a (non-temporary + registered) new connection
            //Nconnections := Nconnections + 1;
            //SetLength(connections,Nconnections);
            nc := tconnection.create;
            nc.connect(port1,port2);
            connections.add(nc);
        end;
        //dispose of the temp connection
        newconnection.destroy;
      end;
  end;
  mousestate := ms_idle;
  activeElement := nil;
  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
  PaintBox1Paint(Sender);
end;

procedure TForm1.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  cursor := crArrow;
end;

class procedure TgraphicElement.highlightAll(highlighted: boolean);
begin
  highlightGroup(ge, highlighted);
end;

class procedure TgraphicElement.highlightGroup(list: tlist; highlighted: boolean);
var
  g: tgraphicElement;
  sidebranch: tlist;
  i : integer;
  o : tobject;
begin
  for i := 0 to list.count-1 do
  begin
    o := list.items[i];
    if (o is TgraphicElement) then
    begin
      g := o as TgraphicElement;
      g.highlight(highlighted);
    end
    else
    if o is TList then
    begin
      sidebranch := o as Tlist;
      highlightGroup(sidebranch, highlighted);
    end
  end;
end;

procedure TgraphicElement.highlight(highlighted: boolean);
begin
  self.highlighted := highlighted;
  self.reCreate(name,nInput,nOutput,prototype);
end;

procedure TgraphicElement.IsInCircuit(ic: boolean);
begin
  inCircuit := ic;
end;

procedure TgraphicElement.addToCircuit(mainbranch: tlist);
var
  first : boolean;
  i : integer;
  j : integer;
  total : integer;
  count : integer;
  sidebranch : tlist;
begin
  if inCircuit then begin
    messagebox(GetActiveWindow,'Circular reference!','Error when building circuit',mb_ok);
    exit;
  end;
  inCircuit := true;
  //add itself to circuit
  mainbranch.add(self);
  //check for elements connected on inputs
  //count them
  total := 0;
  for i := 0 to nInput-1 do
    for j := 0 to iput[i].getNumberOfConnections - 1 do inc(total);


  //if more than one element, add side branches *first* (to have them at this point)
  //the last connection continues the main branch
  count := 0;
  for i := 0 to nInput-1 do
    for j := 0 to iput[i].getNumberOfConnections - 1 do
    begin
      inc(count);
      if count = total then //if this is the last element, continue main branch...
      begin
        iput[i].getConnection(j).getOutputElement.addToCircuit(mainbranch);
      end
      else //... but if it is not the last, first add the side branches
      begin
        //create a new side branch
        sidebranch := tlist.Create;
        //add the side branch to the main branch
        mainbranch.add(sidebranch);
        //and add the element to the side brnach
        iput[i].getConnection(j).getOutputElement.addToCircuit(sidebranch);
      end;
    end;
end;

function printElements(intend: string; mainbranch: tlist): string;
var
  g: tgraphicElement;
  sidebranch: tlist;
  i : integer;
  o : tobject;
  s: string;
begin
  for i := 0 to mainbranch.count-1 do
  begin
    write(intend);
    o := mainbranch.items[i];
    if (o is TgraphicElement) then
    begin
      //writeln(' object');
      g := o as TgraphicElement;
      s := s + intend + g.getName + #13#10;
      writeln(intend + g.getName);
    end
    else
    if o is TList then
    begin
      //writeln(' list');
      sidebranch := o as Tlist;
      s := s + printElements('*',sidebranch);
    end
  end;
  printElements := s;
end;

function createCircuit(circuit: P_ART_circuit; mainbranch: tlist): string;
var
  g: tgraphicElement;
  sidebranch: tlist;
  i : integer;
  o : tobject;
  s: string;
  name : array[0..100] of char ;
  branch: P_ART_Circuit;
begin
  for i := 0 to mainbranch.count-1 do
  begin
    //write(i);
    o := mainbranch.items[i];
    if (o is TgraphicElement) then
    begin
      //writeln(' object');
      g := o as TgraphicElement;
      //writeln(g.getName);
      StrPCopy(name,g.getName);
      ARTAppendReference(circuit, g.getARTElement);
    end
    else
    if o is TList then
    begin
      //writeln(' list');
      sidebranch := o as Tlist;

      //Create branch circuit
      //we need name... so give names like 'BranchCircuitXX'
      str(branchCircuits.Count,s);
      StrPCopy(name,'BranchCircuit' + s);
      branch := ARTCreateCircuit(simulator, name);
      //register branch circuit (for deleting!!!)
      branchCircuits.add(branch);
      //append branch circuit
      ARTAppendReference(circuit, branch);
      //append all elements in branch list to branch circuit
      createCircuit(branch, sidebranch);
    end
  end;
end;

procedure TForm1.BitBtn1Click(Sender: TObject);
var
  mainbranch: tlist;
  s : string;
begin

end;

procedure TForm1.RemoveElement1Click(Sender: TObject);
begin
  //remove form element list
  ge.extract(activeElement);
  if activeElement <> nil then
    activeElement.destroy;
  activeElement := nil;
  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
  PaintBox1Paint(Sender);
end;

procedure Tcalculation.Execute;
var
  text: array[0..99] of char;
  myImpCurve: P_ART_DataProp;
begin
  //run the calculation
 	result := ARTInputImpedance(circuit);
  StrPCopy(text,'InputImpedanceCurve');
  myImpCurve := P_ART_DataProp(ARTFindProperty(circuit,text));
end;

procedure generateMagImpedance(myImpCurve: P_ART_DataProp);
var
    i,l: integer;
    tri: T_ART_Tripl;
    //mag: array of real;
    xstep, zoom:real; //min, max: real;
    valid: boolean;
    text: array[0..100] of char;
    val : P_ART_Variant;
begin
      val := ARTGetValue(myImpCurve);
      l := ARTGetLength((val));
      if l > 0 then begin
            Setlength(impedanceMagnitude, l);

            tri := ARTGetTriple(val, 0);
            impedanceMagnitude[0] := sqrt(sqr(tri.c.re) + sqr(tri.c.im));
            //impedance_min := impedanceMagnitude[0];
            impedanceMax := impedanceMagnitude[0];
            for i := 1 to l-1 do
            begin
              tri := ARTGetTriple(val, i);
              impedanceMagnitude[i] := sqrt(sqr(tri.c.re) + sqr(tri.c.im));
              if impedanceMagnitude[i] > impedanceMax then impedanceMax := impedanceMagnitude[i];
              //if impedanceMagnitude[i] < impedance_min then impedance_min := impedanceMagnitude[i];
            end;
            impedanceMin := 0; //always start at 0 Ohm

            prepareGraph(impedancegraph, impedanceMin / 1000000 , impedanceMax / 1000000); //use MEGA Ohm for labels
            //draw impedance curve
            impedancegraph.canvas.pen.color := rgb(128,255,0);
            i:=0;
            if (impedanceMax - impedanceMin) = 0 then zoom := 0 //just in case...
              else zoom := impedancegraph.Height / (impedanceMax - impedanceMin);
            xstep := impedancegraph.Width / l;
            impedancegraph.canvas.moveTo(0, impedancegraph.Height - round((impedanceMagnitude[i]-impedanceMin) * zoom));
            for i := 1 to l-1 do begin
              impedancegraph.canvas.lineTo(round(i*xstep), impedancegraph.Height - round((impedanceMagnitude[i]-impedanceMin) * zoom));
            end;
            //show the picture on the screen
            form3.show;
            form3.StatusBar1.Panels.items[0].Text := curveCaption;
            form3.FormPaint(nil);
    end;
end;

procedure Tcalculation.Done(sender: TObject);
var myImpCurve: P_ART_DataProp;
begin
  ProgressDialog.hide;
  myImpCurve := calculation.result;
  if myImpCurve=nil then
    messagebox(form1.handle,ARTgetLastErrorMessage,'Error when calculating impedance',mb_ok)
  else begin
    generateMagImpedance(myImpCurve);
  end;
  calculation := nil;
end;

procedure ComputeImpedance(circuit: P_ART_Circuit);
var myImpCurve: P_ART_DataProp;
    i,l: integer;
    tri: T_ART_Tripl;
    //mag: array of real;
    xstep, zoom:real; //min, max: real;
    valid: boolean;
    text: array[0..100] of char;
begin
  if (f_max <= f_min) then
    messagebox(form1.handle,'The maximum frequency must be higher than the lower frequency limit.','Invalid frequency range',mb_ok)
  else if (f_step = 0) then
    messagebox(form1.handle,'f_step must be greater than 0.','Invalid frequency range',mb_ok)
  else begin

    //calculate and get impedance curve
    //progressbar1.visible := true;
    //abortpressed := false;
    StrPCopy(text,'InputImpedanceCurve');
    calculation :=  Tcalculation.Create(True); { create but don't run }
    //The thread will be destroyed automatically
    calculation.FreeOnTerminate := true;
    calculation.circuit := circuit;
    calculation.Resume;
    //specify which function to call when thread is done
    calculation.OnTerminate := calculation.Done;
    //progressbar1.visible := false;
  end;
end;



procedure TForm1.CalcImpedanceClick(Sender: TObject);
var
  mainbranch: tlist;
  s : string;
  nam: array[0..100] of char;
  i: integer;
  g: tgraphicElement;
begin
  if ((calculation <> nil) and not(calculation.Terminated)) then exit;
  if (activeElement <> nil) then
  begin
    //no element is in any circuit so far...
    for i := 0 to ge.count-1 do
    begin
      g := tobject(ge.items[i]) as tgraphicElement;
      g.IsInCircuit(false);
    end;
    //create mainbranch
    mainbranch := tlist.create;
    //go back along connections of ele, when there are branches, go along them as well
    activeElement.addToCircuit(mainbranch);
    //Create a caption for the curve we are going to get
    curveCaption := 'Input impedance of ' + activeElement.getName;

    //mark the elements belonging to circuit
    //TgraphicElement.highlightAll(false);
    //TgraphicElement.highlightGroup(mainbranch, true);

    //repaint
    repaintgraphicBGbuffer(nil);
    repaintgraphicActivebuffer;
    form1.PaintBox1Paint(nil);


    //print the structure
    //s := printElements('* ',mainbranch);

    //remove all previously added references from circuit
    ARTRemoveAllReferences(mainCircuit);
    //some of these references may have been branches, we will delete branches now!
    for i := 0 to branchCircuits.Count -1 do ARTDestroyCircuit(simulator, (branchCircuits[i]));
    createCircuit(mainCircuit, mainbranch);
    //nun mehrere elemente und auch ART neu kompilieren, so dass ich auch circuits referenzieren kann!

    //show progress dialog
    ProgressDialog.show;
    ComputeImpedance(mainCircuit);

    //DrawImpedanceCurve;
    mainbranch.destroy;
  end;
end;


procedure TForm1.AddElement1Click(Sender: TObject);
begin
  form2.startdialog(Paintboxclicked.x,Paintboxclicked.y, nil);
end;

procedure TForm1.Properties1Click(Sender: TObject);
begin
  form2.startdialog(Paintboxclicked.x,Paintboxclicked.y, activeElement);
//  weiter hier (prototyp übertragen usw...
end;



procedure TForm1.FormDestroy(Sender: TObject);
var i: integer;
begin
  ARTDestroyCircuit(simulator, maincircuit);
  //destroy all branch circuits
  for i := 0 to branchCircuits.Count -1 do ARTDestroyCircuit(simulator, (branchCircuits[i]));
  ARTDestroySimulator(simulator);
  ARTRootdestroy;
  //TODO : Listen tserstören
end;

//Checks if a proposed name is already in use and creates a similar name
//the algorithm used is very inefficient, but will be good enough for our needs
function createName(proposed: string): string;
var
  addNumber: integer;
  s: string;
  proposedName: string;
  found: boolean;
  i: integer;
  g: tgraphicElement;
begin
  addNumber := 0;
  proposedName := proposed;
  repeat
    found := false;
    for i := 0 to ge.count-1 do
    begin
      g := tobject(ge.items[i]) as tgraphicElement;
      if g.getName = proposedName then begin
        //writeln(g.getName, ' bei ', i);
        found := true;
        inc(addNumber);
        str(addNumber, s);
        proposedName := proposed + s;
        break;
      end;
    end;
  until (found=false);
  createName := proposedName;
end;

procedure readInstrumentFile(filename: string; var branch: tlist);
var
  f: text;
  s:string;
  elName:string;
  slist:tstrings;
  nameRead : boolean;
  prototype : string;
  paramString : string;
  typ: integer;
  c: integer;
  g: tgraphicElement;
  ps: array[0..999] of char;
  newBranch: tlist;
begin
  slist := tstringlist.create;
  system.assign(f,filename);
  system.reset(f);
  //Read the name of the instrument... we don't do anything with it so far
  nameRead := false;
  repeat
    readln(f, s);
    s := Trim(s);
    if not((s = '') or (s[1] = ';')) then nameRead := true;
  until nameRead or eof(f);

  try
  repeat
    readln(f, s);
    s := Trim(s);
    if not((s = '') or (s[1] = ';')) then
    begin
      //use strings-object to seperate strings on ,
      slist.commaText := s;
      paramString := '';
      elName := '';
      if slist.Count > 0 then begin
        val(slist[0],typ,c);
        case typ of
          0: //bore discontinuity (input radius, output radius)
               begin
                 prototype := 'BoreJump';
                 elName := CreateName('BJ');
                 if slist.Count > 1 then paramString := paramString + elName + '.r1=' + slist[1] + ';';
                 if slist.Count > 2 then paramString := paramString + elName + '.r2=' + slist[2] + ';';
                 //writeln(elName, ' ', paramString);
               end;
          1: //cylinder (length, radius)
               begin
                 //Check if name 'Cyl' is free, have other name proposed if not
                 prototype := 'Cylinder';
                 elName := CreateName('Cyl');
                 if slist.Count > 1 then paramString := paramString + elName + '.length=' + slist[1] + ';';
                 if slist.Count > 2 then paramString := paramString + elName + '.radius=' + slist[2] + ';';
               end;
          2: //cone section (length, input radius, output radius)
               begin
                 prototype := 'Cone';
                 elName := CreateName('Con');
                 if slist.Count > 1 then paramString := paramString + elName + '.length=' + slist[1] + ';';
                 if slist.Count > 2 then paramString := paramString + elName + '.r1=' + slist[2] + ';';
                 if slist.Count > 3 then paramString := paramString + elName + '.r2=' + slist[3] + ';';
                 //writeln(elName, ' ', paramString);
               end;
          3: //Bessel horn section (length, input radius, output radius, flare coefficient)
               begin
                 prototype := 'Besselhorn';
                 elName := CreateName('Bes');
                 if slist.Count > 1 then paramString := paramString + elName + '.length=' + slist[1] + ';';
                 if slist.Count > 2 then paramString := paramString + elName + '.r1=' + slist[2] + ';';
                 if slist.Count > 3 then paramString := paramString + elName + '.r2=' + slist[3] + ';';
                 if slist.Count > 4 then paramString := paramString + elName + '.flare=' + slist[4] + ';';
                 //writeln(elName, ' ', paramString);
               end;
          4: //list element (filename)
               begin
                  //writeln('file:',slist[1]);
                  newBranch := tlist.Create;
                  readInstrumentFile(slist[1], newBranch);
                  branch.add(newBranch);
               end;
          5: //cylindrical bend (length, radius, bend radius)
               begin
                 prototype := 'BentCylinder';
                 elName := CreateName('BCyl');
                 if slist.Count > 1 then paramString := paramString + elName + '.length=' + slist[1] + ';';
                 if slist.Count > 2 then paramString := paramString + elName + '.radius=' + slist[2] + ';';
                 if slist.Count > 3 then paramString := paramString + elName + '.bendradius=' + slist[3] + ';';
                 //writeln(elName, ' ', paramString);
               end;
          6: //conical bend (length, input radius, output radius, bend radius)
               begin
                 prototype := 'BentCone';
                 elName := CreateName('BCon');
                 if slist.Count > 1 then paramString := paramString + elName + '.length=' + slist[1] + ';';
                 if slist.Count > 2 then paramString := paramString + elName + '.r1=' + slist[2] + ';';
                 if slist.Count > 3 then paramString := paramString + elName + '.r2=' + slist[3] + ';';
                 if slist.Count > 4 then paramString := paramString + elName + '.bendradius=' + slist[4] + ';';
               end;
          7: //termination element (type)
               begin
                 prototype := 'TerminationModel';
                 elName := CreateName('Term');
                 if slist.Count > 1 then begin
                    if slist[1] = '0' then paramString := paramString + elName + '.termination=Reflecting;';
                    if slist[1] = '1' then paramString := paramString + elName + '.termination=Zorumski;';
                 end;
               end;
//          8: //tone hole with specifications in new file?? element (type)
          end;
          if not(elName = '') then begin
            //create Element
            g := TgraphicElement.create(elName,inputPortsPerElement,outputPortsPerElement, prototype);
            //Set parameters
            StrPCopy(ps,paramString);
            ARTSetParameter(simulator, ps);
            //Add the Element to the list
            ge.add(g);
            //Add Element to branch
            branch.add(g);
          end;
        end;
    end;
  until eof(f);
  finally slist.free;
  end;

  system.close(f);
end;


procedure readBoreList(filename: string; var branch: tlist);
var
  f: text;
  x: real;
  y: real;
  x1: real;
  y1: real;
  s : string;
  n : string;
  nam : string;
  ps: array[0..999] of char;
  r1: real;
  r2: real;
  length: real;
  g: tgraphicElement;
begin
  system.assign(f,filename);
  system.reset(f);

  try
    // read distance from beginning (x) and bore perimeter (y) of first measurement
    read(f, x1);
    read(f, y1);
    repeat
      //read next measurement
      read(f, x);
      read(f, y);
      if (x > x1) and (y > 0) then
      begin
        // now create a cone
        nam := CreateName('C');
        g := TgraphicElement.create(nam,inputPortsPerElement,outputPortsPerElement, 'Cone');
        //Set parameters
        s := '';
        length := x - x1;
        r1 := y1 / 2;
        r2 := y  / 2;
        str(length, n); s := s + nam + '.length = ' + n + ';';
        str(r1, n); s := s + nam + '.r1 = ' + n + ';';
        str(r2, n); s := s + nam + '.r2 = ' + n + ';';
        StrPCopy(ps,s);
        ARTSetParameter(simulator, ps);
        //writeln(nam,' ',s);
        //Add the cone to the list
        ge.add(g);
        //Add cone to branch
        branch.add(g);
        x1 := x;
        y1 := y;
      end;
    until eof(f);
    except begin end;
  end;

  system.close(f);
end;


procedure saveInstrumentFile(filename: string);
begin
end;


procedure placebranch(branch: tlist; origin,dir:tpoint; var max: tpoint; lastElement: TgraphicElement = nil);
var i: integer;
  port1: tport;
  port2: tport;
  nc: tconnection;
  o: tobject;
  p: tpoint;
  bp: tpoint;
  newdir: tpoint;
  last: TgraphicElement;
  branchdistance: real;
begin
  branchdistance:=1; // the first branch will start at p = p + 1 * dir, the second branch at p = p + 0.5 * dir and so on, so that we don't get overlapping branches
  last := lastElement;
  p.x := origin.x;
  p.y := origin.y;
  for i := 0 to (branch.count-1) do begin
    o := branch.items[i];
    if (o is TgraphicElement) then
    begin
      //connect with last element
      if (last<>nil) then
      begin
        port1 := TgraphicElement(o).getOutputPort(0);
        port2 := last.getInputPort(0);
        nc := tconnection.create;
        nc.connect(port1,port2);
        connections.add(nc);
      end;
      //place element
      //coordinates = origin + direction * i
      p.x := p.x + dir.x * 1;
      p.y := p.y + dir.y * 1;
      TgraphicElement(o).place(graphicBGbuffer,p);
      last := TgraphicElement(o);
    end
    else
    if (o is Tlist) then
    begin
      //the new branch is next to this branch, so move the writing position in a vector turned by 90 degrees
      bp.x := p.x + round(dir.y * branchdistance);
      bp.y := p.y + round(dir.x * branchdistance);
      placebranch(tlist(o), bp, dir, max, last);
      branchdistance := branchdistance / 2;
    end;
  end;
end;

procedure placeLongbranch(branch: tlist; origin,dir:tpoint; var max: tpoint; lastElement: TgraphicElement = nil);
var i: integer;
  port1: tport;
  port2: tport;
  nc: tconnection;
  o: tobject;
  p: tpoint;
  bp: tpoint;
  newdir: tpoint;
  last: TgraphicElement;
  branchdistance: real;
begin
  branchdistance:=1; // the first branch will start at p = p + 1 * dir, the second branch at p = p + 0.5 * dir and so on, so that we don't get overlapping branches
  last := lastElement;
  p.x := origin.x;
  p.y := origin.y;
  for i := 0 to (branch.count-1) do begin
    o := branch.items[i];
    if (o is TgraphicElement) then
    begin
      //connect with last element
      if (last<>nil) then
      begin
        port1 := TgraphicElement(o).getOutputPort(0);
        port2 := last.getInputPort(0);
        nc := tconnection.create;
        nc.connect(port1,port2);
        connections.add(nc);
      end;
      //place element
      //coordinates = origin + direction * i
      p.x := p.x + dir.x * 1;
      p.y := p.y + dir.y * 1;
      //continue branch below added elements in reverse direction, if end of bitmap is reached
      if (p.y > graphicBGbuffer.height) then begin
        //dir.x := - dir.x;
        //go 2 steps, to undo the step which went to far and to go into the right direction now
        p.y := origin.y;
        //continue on x (by dir.y!!)
        p.x := p.x + dir.y;
      end;
      TgraphicElement(o).place(graphicBGbuffer,p);
      last := TgraphicElement(o);
    end
    else
    if (o is Tlist) then
    begin
      //the new branch is next to this branch, so move the writing position in a vector turned by 90 degrees
      bp.x := p.x + round(dir.y * branchdistance);
      bp.y := p.y + round(dir.x * branchdistance);
      placebranch(tlist(o), bp, dir, max, last);
      branchdistance := branchdistance / 2;
    end;
  end;
end;

procedure TForm1.Open1Click(Sender: TObject);
var
  i: integer;
  branch: tlist;
  d: integer;
  port1: tport;
  port2: tport;
  nc: tconnection;


  origin: tpoint;
  dir: tpoint;
  max: tpoint;
  filename: string;
begin
  //get instrument file name
  opendialog1.Execute;
  filename := opendialog1.FileName;

  branch := tlist.create;
  readInstrumentFile(filename, branch);
  //check in which distance to place the elements
  d := round(graphicBGbuffer.width / (branch.count+1));

  placebranch(branch,point(0,50),point(100,0),max);

  branch.free;
  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
  PaintBox1Paint(Sender);
end;

procedure TForm1.Simulator1Click(Sender: TObject);
begin
  form4.startdialog(Paintboxclicked.x,Paintboxclicked.y);
end;


procedure TForm1.New1Click(Sender: TObject);
var i: integer;
  c: tconnection;
  g: tgraphicElement;
begin
 //clear connections
  for i := connections.count-1 downto 0 do
  begin
    c := connections.Items[i];
    connections.extract(c);
    c.destroy;
  end;

  //clear elements
  for i := ge.count-1 downto 0 do
  begin
    g := tobject(ge.items[i]) as tgraphicElement;
    ge.extract(g);
    g.destroy;
  end;

  //redraw
  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
  PaintBox1Paint(Sender);
end;

procedure TForm1.SnaptoGrid1Click(Sender: TObject);
begin
  snaptogrid1.checked := not snaptogrid1.checked;
  useGrid := snaptogrid1.checked;
end;

procedure TForm1.Openborelist1Click(Sender: TObject);
var
  branch: tlist;
  d: integer;
  max: tpoint;
  filename: string;
begin
  //get bore list file name
  opendialog2.Execute;
  filename := opendialog2.FileName;

  branch := tlist.create;
  readBoreList(filename, branch);
  //check in which distance to place the elements
  d := round(graphicBGbuffer.width / (branch.count+1));

  placelongbranch(branch,point(50,50),point(0,50),max);

  branch.free;
  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
  PaintBox1Paint(Sender);
end;

procedure TForm1.HowtoUseHelp1Click(Sender: TObject);
begin
  form5.show;
end;

procedure TForm1.About1Click(Sender: TObject);
begin
  form5.show;
end;

procedure TForm1.FormResize(Sender: TObject);
var
  g: tgraphicElement;
  i:integer;
begin
  graphicBGbuffer.Width := paintbox1.Width;
  graphicBGbuffer.height := paintbox1.height;
  graphicActivebuffer.Width := paintbox1.Width;
  graphicActivebuffer.height := paintbox1.height;
  //replace all elements (so that they are on the screen)
  if ge.count > 0 then begin
    for i := 0 to ge.count-1 do
    begin
      g := tobject(ge.items[i]) as tgraphicElement;
      g.replace(graphicBGbuffer);
    end;
  end;
  repaintgraphicBGbuffer( nil);
  repaintgraphicActivebuffer;
end;

procedure TForm1.Exit1Click(Sender: TObject);
begin
  form1.Close  ;
end;

end.












