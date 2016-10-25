program SimpleImpedance;

{$APPTYPE CONSOLE}
uses ARTDefinitions;

var
  psim : P_ART_Object;
  myCyl : P_ART_Element;
  myCon : P_ART_Element;
  myBes : P_ART_Element;
  mySim: P_ART_Simulator;
  myIns: P_ART_Circuit;
  myImpCurve: P_ART_Variant;
  time,i: integer;
  tri: T_ART_Tripl;
  mag: real;

begin
  // Create Root Object
  psim := ARTRootObject();

  // Create Simulator
  mySim := ARTCreateSimulator('MySimulator', 'FrequencyDomain', 'MultiModal');

  // Create elements
  myCyl := ARTCreateElement(mySim, 'Cyl', 'Cylinder');
  myCon := ARTCreateElement(mySim, 'Con', 'Cone');
  myBes := ARTCreateElement(mySim, 'Bes', 'Besselhorn');

  // Set parameters
  ARTSetParameter(mySim, 'Cyl.r = 1; Cyl.length = 100;');

  ARTSetParameter(mySim, 'Con.length = 50;');
  ARTSetParameter(mySim, 'Con.r1 = Cyl.r; Con.r2 = 1.5 * Con.r1;');

  ARTSetParameter(mySim, 'Bes.length = 20; Bes.r1 = Con.r2;');
  ARTSetParameter(mySim, 'Bes.r2 = 12.5;');
  ARTSetParameter(mySim, 'Bes.flare = 0.4;');

  // Create circuit and add elements to circuit
  myIns := ARTCreateCircuit(mySim, 'MyInstrument');

  ARTAppendReference( myIns, myCyl);
  ARTAppendReference( myIns, myCon);
  ARTAppendReference( myIns, myBes);

  // Set simulation properties
  ARTSetFrequencyRange(mySim, 10, 1800, 10);
  ARTSetNModes(mySim, 1);

  write('calculating...');
  // Get impedance!
  myImpCurve := ARTGetValue(ARTInputImpedance(myIns));
  writeln('done');

  // Print impedance (frequency + magnitude)
  writeln;
  writeln('Frequency     Impedance');
  writeln;
  for i := 0 to ARTGetLength(myImpCurve)-1 do
  begin
    // get data structure
    tri := ARTGetTriple(myImpCurve, i);
    // compute magnitude
    mag := sqrt(sqr(tri.c.re) + sqr(tri.c.im));
    writeln(tri.f:5:0,' Hz   ', mag:12:2, ' Ohm');
    // writeln(tri.f:5:0,' Hz   ', tri.c.re:12:2, ' + ', tri.c.im:12:2);
  end;

  // Wait for user to dismiss
  writeln;
  write('Press ENTER to continue...');
  readln;

  // clean up
	ARTDestroyCircuit(mySim, myIns);

  ARTDestroyElement(mySim, myCyl);
  ARTDestroyElement(mySim, myCon);
  ARTDestroyElement(mySim, myBes);

  ARTDestroySimulator(mySim);

  ARTRootDestroy();
end.

