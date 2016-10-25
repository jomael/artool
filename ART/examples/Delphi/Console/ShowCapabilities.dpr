program ShowCapabilies;
// *********************************************************************************************************************
// This delphi program displays the complete information about library, models and parameters.
// Object properties, methods and method parameters are written like: name=value // short description
//
// Enumerating a list of objects associated with another object is always done in the same simple way:
//             ChildObjectPointer := SimGetSomething(ParentObjectPointer, ChildObjectPointer)
// Passing nil returns the pointer to the first listed object, passing a pointer to the last object returns nil.
// *********************************************************************************************************************

{$APPTYPE CONSOLE}
uses SysUtils, ARTDefinitions;

const TAB = #9; CRLF = #13#10; SPC = ' ';

type 	PInteger = ^Integer; PFloat = ^Double;

var psim : P_ART_Object;
    nam, sds, lds, htm: String;

procedure ListObjects(plst: P_ART_Property; s: string); forward;

// .............................................................................

// Retrieve general information about all objects
  procedure GetInfo(pobj: P_ART_Object; var nam,sds,lds,htm: string);
  begin
    nam := ARTGetName(pobj);
    sds := ARTGetShortDescription(pobj);
    lds := ARTGetLongDescription(pobj);
    htm := ARTGetHelpFilename(pobj);
  end;

// .............................................................................

// .............................................................................

procedure ListProperties(obj: P_ART_Object; s: string);
var pprp: P_ART_Property;
    v: P_ART_Variant;
    nam, sds, lds, htm: String;
    listable: boolean;
    c,l: integer;
    pc: pchar;
    val: P_ART_Variant;
begin
  // traverse property list of each model object
  // get first property
  pprp := ARTGetProperties(obj, nil);
  while pprp <> nil do begin
    //get name and other info
    GetInfo(pprp, nam,sds,lds,htm);
    nam := 'Prp: '+nam;
    listable := ARTIsListable(pprp);
    if listable then
    begin
        nam := nam+'<..>';
        writeln(s+nam, ' = // ', sds);
        ListObjects(pprp,s+SPC);
    end
    else //If it is a string data property, then print it
      if ARTIsDataProp(pprp) then begin
        val := ARTGetValue(P_ART_DataProp(pprp));
        if (ARTGetDatatype(val) = C_ART_str) or (ARTGetDatatype(val) = C_ART_nstr)
        then begin
          l:=ARTGetLength(val);
          if l>0 then begin
                writeln(s+nam, '[', ARTGetLength(val), ']= // ', sds);
                for c:= 0 to ARTGetLength(val)-1 do begin
                        pc := ARTGetString(val,c);
                        writeln(s+SPC+pc);
                end;
                end;
        end
        else writeln(s+nam + SPC + sds);

    end;
    //get next property
    pprp := ARTGetProperties(obj, pprp);
  end;
end;

// .............................................................................

procedure ListMethods(obj: P_ART_Object; s: string);
var pmet: P_ART_Method;
    nam, sds, lds, htm: String;
begin
  pmet := ARTGetMethods(obj, nil);
  while pmet <> nil do begin
    GetInfo(pmet, nam,sds,lds,htm); nam := 'Met: '+nam;
    writeln(s+nam, ' (Hint: ', sds,' Desc: ', lds,' Doku: ', htm, ')');
    ListProperties(pmet,s+SPC);
    pmet := ARTGetMethods(obj, pmet);
  end;
end;

// .............................................................................

procedure ListObjects(plst: P_ART_Property; s: string);
var pmod: P_ART_Object;
    nam, sds, lds, htm: String;
begin
  if assigned(plst) and ARTIsListable(plst) then
    pmod := ARTGetObjects(P_ART_ListProp(plst),nil)
  else pmod := nil;
  while pmod <> nil do begin

    GetInfo(pmod, nam,sds,lds,htm);  nam := 'Obj: '+nam;
    writeln(s+nam, ' (Hint: ', sds,' Desc: ', lds,' Doku: ', htm, ')');

    // Retrieve Information about all available properties (of model)

    // traverse property list of each object
    ListProperties(pmod, s+SPC);

    // Retrieve Information about all available methods (of model)

    // traverse method list of each object
    ListMethods(pmod, s+SPC);

    pmod := ARTGetObjects(P_ART_ListProp(plst), pmod);
  end;
end;

// …………………………………………………………................................................

begin
  writeln;
  writeln('########################################################');
  writeln('ART Capabilities');
  writeln;

  // Create Root Object
  try
    psim := ARTRootObject();
  except
  end;
  GetInfo(psim, nam,sds,lds,htm);
  writeln('********************************************************');
  writeln('Object: ', nam);
  writeln('  Hint: ', sds);
  writeln('  Desc: ', lds);
  writeln('  Doku: ', htm);
  writeln;

  // Retrieve Information about all available properties (of root object)
  writeln('--------------------------------------------------------');
  writeln('Object Properties');
  writeln;
  ListProperties(psim,SPC);

  // Retrieve Information about all available methods (of root object)
  writeln('--------------------------------------------------------');
  writeln('Object Methods');
  writeln;
  ListMethods(psim,SPC);

  writeln;
  writeln('########################################################');
  writeln('ART Menu Commands');
  writeln;
  ListObjects(ARTFindProperty(psim, 'MenuCommands'),SPC);

  // Retrieve Information about all available simulation models
  // e.g. Name=, Domain=, Level=, LWave=, RWave=, BWave=
  //        Hint=, Description=, Usage=, Helpfile=, Authors=, Version=, Build=, Date=, Copyright=, Url=, Email=, History=...
  writeln;
  writeln('########################################################');
  writeln('ART Native Models');
  writeln;
  ListObjects(ARTFindProperty(psim, 'prototypeModels'),SPC);

  writeln;
  write('Press ENTER to continue...');
  readln;

  ARTRootDestroy();
end.

