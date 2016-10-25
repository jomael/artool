program graphImpProject;

uses
  Forms,
  graphImpUnit1 in 'graphImpUnit1.pas' {Form1};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
