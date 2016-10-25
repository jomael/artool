unit ARTDefinitions;

interface


const
  C_ART_str = 0;
	C_ART_enum = 1;
	C_ART_int = 2;
	C_ART_dbl = 3;
	C_ART_flo = 4;
	C_ART_cpx = 5;
	C_ART_tri = 6;
	C_ART_mat = 7;
	C_ART_matx = 8;
	C_ART_nstr = 9;
	C_ART_nint = 10;
	C_ART_nflo = 11;
	C_ART_ndbl = 12;
	C_ART_ncpx = 13;
	C_ART_ntri = 14;
	C_ART_nmat = 15;
	C_ART_nmatx = 16;
	C_ART_na    = 17;
	C_ART_undef = 18;

type 	T_ART_Type=C_ART_str..C_ART_undef;
	T_ART_Cmplx     =       record re,im: double; end;
	T_ART_Tripl     =	record f: double; c: T_ART_Cmplx end;
	T_ART_Matrix    =	record a11,a12,a21,a22: T_ART_Cmplx; end;

  // a "rebuild" of the inheritance relations of the objects
	P_ART_Variant = class(TObject);
	P_ART_Cell = class(TObject);
	P_ART_Object = class(P_ART_Cell);
	P_ART_Property = class(P_ART_Object);
	P_ART_DataProp = class(P_ART_Property);
	P_ART_ListProp = class(P_ART_Property);
	P_ART_Method = class(P_ART_Object);
	P_ART_ParList = class(TObject);
	P_ART_Prototype = class(P_ART_Object);
	P_ART_Element = class(P_ART_Prototype);
	P_ART_Circuit = class(P_ART_Element);
	P_ART_Simulator = class(P_ART_Object);

  TprogressFunction = function(var X: Double; const msg: Pchar): boolean; cdecl;
  PprogressFunction = ^TprogressFunction;
//...............................................................................................

// parameter is trace file name, returns always 0
function begin_trace ( filename : PChar ) : integer ;  cdecl; external 'ART.dll';
// parameter ignored, returns success(1) or 0
function end_trace ( dummy : integer ) : integer ;  cdecl; external 'ART.dll';

function ARTRootObject : P_ART_Object ;  cdecl; external 'ART.dll';
function ARTRootDestroy : boolean ;  cdecl; external 'ART.dll';

function ARTSetProgressFunction ( f : PprogressFunction ) : boolean ;  cdecl; external 'ART.dll';

function ARTCheckPropertyCapability ( propertycap, capability : PChar ) : boolean ;  cdecl; external 'ART.dll';

function ARTCreateSimulator(name, domain, wavetype: PChar) : P_ART_Simulator; cdecl; external 'ART.dll';
function ARTDestroySimulator ( simulator : P_ART_Simulator ) : boolean ;  cdecl; external 'ART.dll';
function ARTSetFrequencyRange ( sim : P_ART_Simulator ; f_min : double ; f_max : double ; f_step : double  ) : boolean ;  cdecl; external 'ART.dll';
function ARTSetNModes ( sim : P_ART_Simulator ; Nmodes : integer  ) : boolean ;  cdecl; external 'ART.dll';

function ARTCreateElement ( simulator : P_ART_Simulator ; name, typ : PChar ) : P_ART_Element ;  cdecl; external 'ART.dll';
function ARTChangeElementModel ( simulator : P_ART_Simulator ; element: P_ART_Element; typ : PChar ) : P_ART_Element ;  cdecl; external 'ART.dll';
function ARTDestroyElement ( simulator : P_ART_Simulator ; element : P_ART_Element  ) : boolean ;  cdecl; external 'ART.dll';
function ARTSetParameter ( simulator : P_ART_Simulator ; command : PChar ) : P_ART_DataProp ;  cdecl; external 'ART.dll';
function ARTChangeName ( element : P_ART_Element ; newname : PChar ) : P_ART_Element ;  cdecl; external 'ART.dll';

function ARTCreateCircuit ( simulator : P_ART_Simulator ; name : PChar ) : P_ART_Circuit ;  cdecl; external 'ART.dll';
function ARTDestroyCircuit ( simulator : P_ART_Simulator ; circuit : P_ART_Circuit  ) : boolean ;  cdecl; external 'ART.dll';

function ARTGetReferencePosition ( circuit : P_ART_Circuit ; element: P_ART_Element ) : integer ;  cdecl; external 'ART.dll';
function ARTAppendReference ( circuit : P_ART_Circuit ; element : P_ART_Element ) : P_ART_Object ;  cdecl; external 'ART.dll';
function ARTAppendReferenceBefore ( circuit : P_ART_Circuit ; elementAfter, element : P_ART_Element ) : P_ART_Object ;  cdecl; external 'ART.dll';
function ARTAppendReferenceAfter ( circuit : P_ART_Circuit ; elementBefore, element : P_ART_Element ) : P_ART_Object ;  cdecl; external 'ART.dll';
function ARTRemoveReference ( circuit : P_ART_Circuit ; element : P_ART_Element ) : integer ;  cdecl; external 'ART.dll';
function ARTReplaceReference ( circuit : P_ART_Circuit ; search, replace : P_ART_Element ) : integer ;  cdecl; external 'ART.dll';
function ARTRemoveAllReferences ( circuit : P_ART_Circuit ) : integer ;  cdecl; external 'ART.dll';
function ARTInputImpedance ( circuit : P_ART_Circuit ) : P_ART_DataProp ;  cdecl; external 'ART.dll';

function ARTGetName ( pobj : P_ART_Cell ) : PChar ;  cdecl; external 'ART.dll';
function ARTGetShortDescription ( pobj : P_ART_Cell ) : PChar ;  cdecl; external 'ART.dll';
function ARTGetLongDescription ( pobj : P_ART_Cell ) : PChar ;  cdecl; external 'ART.dll';
function ARTGetHelpFilename ( pobj : P_ART_Cell ) : PChar ;  cdecl; external 'ART.dll';
// there are listable and scalar properties
function ARTIsListable ( pprp : P_ART_Property ) : boolean ;  cdecl; external 'ART.dll';

function ARTIsDataProp ( pprp : P_ART_Property )  : boolean ;  cdecl; external 'ART.dll';
function ARTGetRange ( pprp : P_ART_DataProp )  : P_ART_Variant ;  cdecl; external 'ART.dll';
function ARTGetDefinitionString ( pprp : P_ART_DataProp )  : PChar ;  cdecl; external 'ART.dll';
// registered scalar properties have a value of any type, objects (and methods) have properties
// 						scalar properties receive their datatype at creation (readonly)
// some datatypes are array types, here you query their array size
function ARTGetLength ( pprp : P_ART_Variant ) : integer ;  cdecl; external 'ART.dll';
// get a pointer to the variant data structure
function ARTGetValue ( pprp : P_ART_DataProp ) : P_ART_Variant ;  cdecl; external 'ART.dll';

//TODO: sollte mit  P_ART_Variant sein
function ARTGetDatatype ( pprp : P_ART_Variant ) : T_ART_Type ;  cdecl; external 'ART.dll';
function ARTGetString ( pprp : P_ART_Variant ; idx : integer  ) : PChar ;  cdecl; external 'ART.dll';
function ARTSetString ( pprp : P_ART_Variant ; idx : integer ; s : PChar ) : boolean ;  cdecl; external 'ART.dll';
function ARTGetInteger ( pprp : P_ART_Variant ; idx : integer  ) : integer ;  cdecl; external 'ART.dll';
function ARTSetInteger ( pprp : P_ART_Variant ; idx : integer ; i : integer  ) : boolean ;  cdecl; external 'ART.dll';
function ARTGetFloat ( pprp : P_ART_Variant ; idx : integer  ) : single ;  cdecl; external 'ART.dll';
function ARTSetFloat ( pprp : P_ART_Variant ; idx : integer ; f : single  ) : boolean ;  cdecl; external 'ART.dll';
function ARTGetDouble ( pprp : P_ART_Variant ; idx : integer  ) : double ;  cdecl; external 'ART.dll';
function ARTSetDouble ( pprp : P_ART_Variant ; idx : integer ; d : double  ) : boolean ;  cdecl; external 'ART.dll';
function ARTGetComplex ( pprp : P_ART_Variant ; idx : integer  ) : T_ART_Cmplx ;  cdecl; external 'ART.dll';
function ARTSetComplex ( pprp : P_ART_Variant ; idx : integer ; c : T_ART_Cmplx  ) : boolean ;  cdecl; external 'ART.dll';
function ARTGetTriple ( pprp : P_ART_Variant ; idx : integer  ) : T_ART_Tripl ;  cdecl; external 'ART.dll';
function ARTSetTriple ( pprp : P_ART_Variant ; idx : integer ; t : T_ART_Tripl  ) : boolean ;  cdecl; external 'ART.dll';
function ARTGetMatrix ( pprp : P_ART_Variant ; idx : integer  ) : T_ART_Matrix ;  cdecl; external 'ART.dll';
function ARTSetMatrix ( pprp : P_ART_Variant ; idx : integer ; m : T_ART_Matrix  ) : boolean ;  cdecl; external 'ART.dll';

function ARTFindProperty ( host : P_ART_Object ; nam : PChar ) : P_ART_Property ;  cdecl; external 'ART.dll';
function ARTFindDataProperty ( host : P_ART_Object ; nam : PChar ) : P_ART_DataProp ;  cdecl; external 'ART.dll';
function ARTFindMethod ( host : P_ART_Object ; nam : PChar ) : P_ART_Method ;  cdecl; external 'ART.dll';
function ARTFindObject ( host : P_ART_ListProp ; nam : PChar ) : P_ART_Object ;  cdecl; external 'ART.dll';
function ARTGetModel (element : P_ART_Element) : P_ART_Object;   cdecl; external 'ART.dll';

//function ARTGetModelProperties ( host : P_ART_Element ; pos : P_ART_Property  ) : P_ART_Property ;  cdecl; external 'ART.dll';
// Traverse property list of host object
function ARTGetProperties ( host : P_ART_Object ; pos : P_ART_Property  ) : P_ART_Property ;  cdecl; external 'ART.dll';

function ARTGetDataProperties ( host : P_ART_Object ; pos : P_ART_DataProp  ) : P_ART_DataProp ;  cdecl; external 'ART.dll';
// Traverse method list of host object
function ARTGetMethods ( host : P_ART_Object ; pos : P_ART_Method  ) : P_ART_Method ;  cdecl; external 'ART.dll';
// Traverse object list of host object
function ARTGetObjects ( host : P_ART_ListProp ; pos : P_ART_Object  ) : P_ART_Object ;  cdecl; external 'ART.dll';

function ARTAppendDataProp ( host : P_ART_Object ; val : P_ART_Variant ; nam, sds, lds, htm : PChar ) : P_ART_DataProp ;  cdecl; external 'ART.dll';
function ARTAppendListProp ( host : P_ART_Object ; nam, sds, lds, htm  : PChar ) : P_ART_ListProp ;  cdecl; external 'ART.dll';
function ARTAppendMethod ( host : P_ART_Object ; nam, sds, lds, htm  : PChar ) : P_ART_Method ;  cdecl; external 'ART.dll';
function ARTAppendObject ( host : P_ART_ListProp ; nam, sds, lds, htm  : PChar ) : P_ART_Object ;  cdecl; external 'ART.dll';
function ARTDeleteProperty ( host : P_ART_Object ; pprp : P_ART_Property  ) : boolean ;  cdecl; external 'ART.dll';
function ARTDeleteMethod ( host : P_ART_Object ; pmet : P_ART_Method  ) : boolean ;  cdecl; external 'ART.dll';
function ARTDeleteObject ( host : P_ART_ListProp ; pobj : P_ART_Object  ) : boolean ;  cdecl; external 'ART.dll';

function ARTGetLastErrorMessage : PChar ;  cdecl; external 'ART.dll';

function ARTGetDependencyTree	(pprp:P_ART_DataProp; linebreak: PChar): PChar; cdecl; external 'ART.dll';

function ARTFindElement (simulator: P_ART_Simulator; name: PChar): P_ART_Element; cdecl; external 'ART.dll';
function ARTFindCircuit (simulator: P_ART_Simulator; name: PChar): P_ART_Circuit; cdecl; external 'ART.dll';

implementation

end.
