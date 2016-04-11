# Microsoft Developer Studio Generated NMAKE File, Based on ART.dsp
!IF "$(CFG)" == ""
CFG=ART - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ART - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ART - Win32 Release" && "$(CFG)" != "ART - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ART.mak" CFG="ART - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ART - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ART - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ART - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ART.dll"


CLEAN :
	-@erase "$(INTDIR)\ART.obj"
	-@erase "$(INTDIR)\ART.sbr"
	-@erase "$(INTDIR)\ARTdataContainer.obj"
	-@erase "$(INTDIR)\ARTdataContainer.sbr"
	-@erase "$(INTDIR)\ARTerror.obj"
	-@erase "$(INTDIR)\ARTerror.sbr"
	-@erase "$(INTDIR)\ARTfunctionoid.obj"
	-@erase "$(INTDIR)\ARTfunctionoid.sbr"
	-@erase "$(INTDIR)\ARTlink.obj"
	-@erase "$(INTDIR)\ARTlink.sbr"
	-@erase "$(INTDIR)\cbabk2.obj"
	-@erase "$(INTDIR)\cbabk2.sbr"
	-@erase "$(INTDIR)\cbal.obj"
	-@erase "$(INTDIR)\cbal.sbr"
	-@erase "$(INTDIR)\cdiv.obj"
	-@erase "$(INTDIR)\cdiv.sbr"
	-@erase "$(INTDIR)\cg.obj"
	-@erase "$(INTDIR)\cg.sbr"
	-@erase "$(INTDIR)\comqr.obj"
	-@erase "$(INTDIR)\comqr.sbr"
	-@erase "$(INTDIR)\comqr2.obj"
	-@erase "$(INTDIR)\comqr2.sbr"
	-@erase "$(INTDIR)\corth.obj"
	-@erase "$(INTDIR)\corth.sbr"
	-@erase "$(INTDIR)\csroot.obj"
	-@erase "$(INTDIR)\csroot.sbr"
	-@erase "$(INTDIR)\hornelement.obj"
	-@erase "$(INTDIR)\hornelement.sbr"
	-@erase "$(INTDIR)\impedancecurve.obj"
	-@erase "$(INTDIR)\impedancecurve.sbr"
	-@erase "$(INTDIR)\interface.obj"
	-@erase "$(INTDIR)\interface.sbr"
	-@erase "$(INTDIR)\legquadrature.obj"
	-@erase "$(INTDIR)\legquadrature.sbr"
	-@erase "$(INTDIR)\mpARTValue.obj"
	-@erase "$(INTDIR)\mpARTValue.sbr"
	-@erase "$(INTDIR)\mpError.obj"
	-@erase "$(INTDIR)\mpError.sbr"
	-@erase "$(INTDIR)\mpFuncCmplx.obj"
	-@erase "$(INTDIR)\mpFuncCmplx.sbr"
	-@erase "$(INTDIR)\mpFuncCommon.obj"
	-@erase "$(INTDIR)\mpFuncCommon.sbr"
	-@erase "$(INTDIR)\mpFuncNonCmplx.obj"
	-@erase "$(INTDIR)\mpFuncNonCmplx.sbr"
	-@erase "$(INTDIR)\mpFuncStr.obj"
	-@erase "$(INTDIR)\mpFuncStr.sbr"
	-@erase "$(INTDIR)\mpICallback.obj"
	-@erase "$(INTDIR)\mpICallback.sbr"
	-@erase "$(INTDIR)\mpIfThenElse.obj"
	-@erase "$(INTDIR)\mpIfThenElse.sbr"
	-@erase "$(INTDIR)\mpIOprt.obj"
	-@erase "$(INTDIR)\mpIOprt.sbr"
	-@erase "$(INTDIR)\mpIPackage.obj"
	-@erase "$(INTDIR)\mpIPackage.sbr"
	-@erase "$(INTDIR)\mpIToken.obj"
	-@erase "$(INTDIR)\mpIToken.sbr"
	-@erase "$(INTDIR)\mpIValReader.obj"
	-@erase "$(INTDIR)\mpIValReader.sbr"
	-@erase "$(INTDIR)\mpIValue.obj"
	-@erase "$(INTDIR)\mpIValue.sbr"
	-@erase "$(INTDIR)\mpOprtBinAssign.obj"
	-@erase "$(INTDIR)\mpOprtBinAssign.sbr"
	-@erase "$(INTDIR)\mpOprtBinCommon.obj"
	-@erase "$(INTDIR)\mpOprtBinCommon.sbr"
	-@erase "$(INTDIR)\mpOprtCmplx.obj"
	-@erase "$(INTDIR)\mpOprtCmplx.sbr"
	-@erase "$(INTDIR)\mpOprtNonCmplx.obj"
	-@erase "$(INTDIR)\mpOprtNonCmplx.sbr"
	-@erase "$(INTDIR)\mpOverload.obj"
	-@erase "$(INTDIR)\mpOverload.sbr"
	-@erase "$(INTDIR)\mpPackageCmplx.obj"
	-@erase "$(INTDIR)\mpPackageCmplx.sbr"
	-@erase "$(INTDIR)\mpPackageCommon.obj"
	-@erase "$(INTDIR)\mpPackageCommon.sbr"
	-@erase "$(INTDIR)\mpPackageNonCmplx.obj"
	-@erase "$(INTDIR)\mpPackageNonCmplx.sbr"
	-@erase "$(INTDIR)\mpPackageStr.obj"
	-@erase "$(INTDIR)\mpPackageStr.sbr"
	-@erase "$(INTDIR)\mpPackageUnit.obj"
	-@erase "$(INTDIR)\mpPackageUnit.sbr"
	-@erase "$(INTDIR)\mpParser.obj"
	-@erase "$(INTDIR)\mpParser.sbr"
	-@erase "$(INTDIR)\mpParserBase.obj"
	-@erase "$(INTDIR)\mpParserBase.sbr"
	-@erase "$(INTDIR)\mpRPN.obj"
	-@erase "$(INTDIR)\mpRPN.sbr"
	-@erase "$(INTDIR)\mpTest.obj"
	-@erase "$(INTDIR)\mpTest.sbr"
	-@erase "$(INTDIR)\mpTokenReader.obj"
	-@erase "$(INTDIR)\mpTokenReader.sbr"
	-@erase "$(INTDIR)\mpValReader.obj"
	-@erase "$(INTDIR)\mpValReader.sbr"
	-@erase "$(INTDIR)\mpValue.obj"
	-@erase "$(INTDIR)\mpValue.sbr"
	-@erase "$(INTDIR)\mpValueCache.obj"
	-@erase "$(INTDIR)\mpValueCache.sbr"
	-@erase "$(INTDIR)\mpVariable.obj"
	-@erase "$(INTDIR)\mpVariable.sbr"
	-@erase "$(INTDIR)\pythag.obj"
	-@erase "$(INTDIR)\pythag.sbr"
	-@erase "$(INTDIR)\rosenbrock.obj"
	-@erase "$(INTDIR)\rosenbrock.sbr"
	-@erase "$(INTDIR)\runga.obj"
	-@erase "$(INTDIR)\runga.sbr"
	-@erase "$(INTDIR)\runrb.obj"
	-@erase "$(INTDIR)\runrb.sbr"
	-@erase "$(INTDIR)\simplematrix.obj"
	-@erase "$(INTDIR)\simplematrix.sbr"
	-@erase "$(INTDIR)\strparsing.obj"
	-@erase "$(INTDIR)\strparsing.sbr"
	-@erase "$(INTDIR)\test_main.obj"
	-@erase "$(INTDIR)\test_main.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\windinstrument.obj"
	-@erase "$(INTDIR)\windinstrument.sbr"
	-@erase "$(OUTDIR)\ART.bsc"
	-@erase "$(OUTDIR)\ART.exe"
	-@erase "$(OUTDIR)\ART.ilk"
	-@erase "$(OUTDIR)\ART.pdb"
	-@erase "$(OUTDIR)\ART.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

#altes Release: CPP_PROJ=/nologo /MT /W3 /GR /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FFT_EXPORTS" /Fp"$(INTDIR)\ART.pch" /FAs /Fa"Release/" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
#neues Release (ML geht nicht -> Linker Fehlermeldung!)
#CPP_PROJ=/nologo /MLd /w /W0 /GR /GX /Ot /Og /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
#CPP_PROJ=/nologo /MLd /w /W0 /GR /GX /Ot /Og /I "..\..\include" /D "WIN32" /D "DLL" /D "_DEBUG" /D "_MBCS" /Fp"$(INTDIR)/ART.pch" /YX /Fo"$(INTDIR)" /Fd"$(INTDIR)" /FD /c 
#CPP_PROJ=/nologo /MT  /w /W0     /GR /GX     /Ot /Og /I "..\..\include" /D "WIN32" /D "DLL" /D "_DEBUG" /D "_MBCS" /YX /Fo"$(INTDIR)"                    /FD /c 
#*REL*CPP_PROJ=/nologo /MT  /w /W0     /GR /GX     /Ot /Og /I "..\..\include" /D "WIN32" /D "DLL" /D "_DEBUG" /D "_MBCS"                  /Fp"$(INTDIR)\ART.pch" /YX /Fo"$(INTDIR)"                    /FD /c 
#*REL*CPP_PROJ=/nologo /MT  /w /W0     /GR /GX     /Ot /Og /I "..\..\include" /D "WIN32" /D "DLL" /D "_DEBUG" /D "_MBCS"                  /Fp"$(INTDIR)\ART.pch" /YX /Fo"$(INTDIR)"                    /FD /c 
#*DEB*CPP_PROJ=/nologo /MTd /w /W0 /Gm /GR /GX /ZI /Od     /I "..\..\include" /D "WIN32" /D "DLL" /D "_DEBUG" /D "_MBCS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\ART.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

#Note: /Og (global optimizations) does NOT work!!! Compiling works fine, but the calculated results will differ slightly from what we get in the command line version and debug versions!
#old (see note above): CPP_PROJ=/nologo /MT  /w /W0     /GR /GX     /Ot /Og /I "..\..\include" /D "WIN32" /D "DLL" /D "NDEBUG" /D "_MBCS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\ART.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c
CPP_PROJ=/nologo /MT  /w /W0     /GR /GX     /Ot /Od /I "..\..\include" /D "WIN32" /D "DLL" /D "NDEBUG" /D "_MBCS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\ART.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c


.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

#MTL=midl.exe
#MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
#RSC=rc.exe
#BSC32=bscmake.exe
#BSC32_FLAGS=/nologo /o"$(OUTDIR)\ART.bsc" 
#BSC32_SBRS= \

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ART.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\cbabk2.sbr" \
	"$(INTDIR)\cbal.sbr" \
	"$(INTDIR)\cdiv.sbr" \
	"$(INTDIR)\cg.sbr" \
	"$(INTDIR)\comqr.sbr" \
	"$(INTDIR)\comqr2.sbr" \
	"$(INTDIR)\corth.sbr" \
	"$(INTDIR)\csroot.sbr" \
	"$(INTDIR)\legquadrature.sbr" \
	"$(INTDIR)\pythag.sbr" \
	"$(INTDIR)\mpARTValue.sbr" \
	"$(INTDIR)\mpError.sbr" \
	"$(INTDIR)\mpFuncCmplx.sbr" \
	"$(INTDIR)\mpFuncCommon.sbr" \
	"$(INTDIR)\mpFuncNonCmplx.sbr" \
	"$(INTDIR)\mpFuncStr.sbr" \
	"$(INTDIR)\mpICallback.sbr" \
	"$(INTDIR)\mpIfThenElse.sbr" \
	"$(INTDIR)\mpIOprt.sbr" \
	"$(INTDIR)\mpIPackage.sbr" \
	"$(INTDIR)\mpIToken.sbr" \
	"$(INTDIR)\mpIValReader.sbr" \
	"$(INTDIR)\mpIValue.sbr" \
	"$(INTDIR)\mpOprtBinAssign.sbr" \
	"$(INTDIR)\mpOprtBinCommon.sbr" \
	"$(INTDIR)\mpOprtCmplx.sbr" \
	"$(INTDIR)\mpOprtNonCmplx.sbr" \
	"$(INTDIR)\mpOverload.sbr" \
	"$(INTDIR)\mpPackageCmplx.sbr" \
	"$(INTDIR)\mpPackageCommon.sbr" \
	"$(INTDIR)\mpPackageNonCmplx.sbr" \
	"$(INTDIR)\mpPackageStr.sbr" \
	"$(INTDIR)\mpPackageUnit.sbr" \
	"$(INTDIR)\mpParser.sbr" \
	"$(INTDIR)\mpParserBase.sbr" \
	"$(INTDIR)\mpRPN.sbr" \
	"$(INTDIR)\mpTest.sbr" \
	"$(INTDIR)\mpTokenReader.sbr" \
	"$(INTDIR)\mpValReader.sbr" \
	"$(INTDIR)\mpValue.sbr" \
	"$(INTDIR)\mpValueCache.sbr" \
	"$(INTDIR)\mpVariable.sbr" \
	"$(INTDIR)\ART.sbr" \
	"$(INTDIR)\ARTdataContainer.sbr" \
	"$(INTDIR)\ARTerror.sbr" \
	"$(INTDIR)\ARTfunctionoid.sbr" \
	"$(INTDIR)\ARTlink.sbr" \
	"$(INTDIR)\hornelement.sbr" \
	"$(INTDIR)\impedancecurve.sbr" \
	"$(INTDIR)\interface.sbr" \
	"$(INTDIR)\rosenbrock.sbr" \
	"$(INTDIR)\runga.sbr" \
	"$(INTDIR)\runrb.sbr" \
	"$(INTDIR)\simplematrix.sbr" \
	"$(INTDIR)\strparsing.sbr" \
	"$(INTDIR)\test_main.sbr" \
	"$(INTDIR)\windinstrument.sbr"

"$(OUTDIR)\ART.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /nodefaultlib:"msvcprtd.lib" /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libc.lib" /incremental:yes /pdb:"$(OUTDIR)\ART.pdb" /machine:I386 /def:"..\ART.def" /out:"$(OUTDIR)\ART.dll" /implib:"$(OUTDIR)\ART.lib" 
#release LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /incremental:no /nodefaultlib:"libc.lib" /nodefaultlib:"libcd.lib" /MAP:"$(OUTDIR)\ART.map" /pdb:"$(OUTDIR)\ART.pdb" /machine:I386 /def:"..\ART.def" /out:"$(OUTDIR)\ART.dll" /implib:"$(OUTDIR)\ART.lib" 

DEF_FILE= \
	"..\ART.def"
LINK32_OBJS= \
	"$(INTDIR)\cbabk2.obj" \
	"$(INTDIR)\cbal.obj" \
	"$(INTDIR)\cdiv.obj" \
	"$(INTDIR)\cg.obj" \
	"$(INTDIR)\comqr.obj" \
	"$(INTDIR)\comqr2.obj" \
	"$(INTDIR)\corth.obj" \
	"$(INTDIR)\csroot.obj" \
	"$(INTDIR)\legquadrature.obj" \
	"$(INTDIR)\pythag.obj" \
	"$(INTDIR)\mpARTValue.obj" \
	"$(INTDIR)\mpError.obj" \
	"$(INTDIR)\mpFuncCmplx.obj" \
	"$(INTDIR)\mpFuncCommon.obj" \
	"$(INTDIR)\mpFuncNonCmplx.obj" \
	"$(INTDIR)\mpFuncStr.obj" \
	"$(INTDIR)\mpICallback.obj" \
	"$(INTDIR)\mpIfThenElse.obj" \
	"$(INTDIR)\mpIOprt.obj" \
	"$(INTDIR)\mpIPackage.obj" \
	"$(INTDIR)\mpIToken.obj" \
	"$(INTDIR)\mpIValReader.obj" \
	"$(INTDIR)\mpIValue.obj" \
	"$(INTDIR)\mpOprtBinAssign.obj" \
	"$(INTDIR)\mpOprtBinCommon.obj" \
	"$(INTDIR)\mpOprtCmplx.obj" \
	"$(INTDIR)\mpOprtNonCmplx.obj" \
	"$(INTDIR)\mpOverload.obj" \
	"$(INTDIR)\mpPackageCmplx.obj" \
	"$(INTDIR)\mpPackageCommon.obj" \
	"$(INTDIR)\mpPackageNonCmplx.obj" \
	"$(INTDIR)\mpPackageStr.obj" \
	"$(INTDIR)\mpPackageUnit.obj" \
	"$(INTDIR)\mpParser.obj" \
	"$(INTDIR)\mpParserBase.obj" \
	"$(INTDIR)\mpRPN.obj" \
	"$(INTDIR)\mpTest.obj" \
	"$(INTDIR)\mpTokenReader.obj" \
	"$(INTDIR)\mpValReader.obj" \
	"$(INTDIR)\mpValue.obj" \
	"$(INTDIR)\mpValueCache.obj" \
	"$(INTDIR)\mpVariable.obj" \
	"$(INTDIR)\ART.obj" \
	"$(INTDIR)\ARTdataContainer.obj" \
	"$(INTDIR)\ARTerror.obj" \
	"$(INTDIR)\ARTfunctionoid.obj" \
	"$(INTDIR)\ARTlink.obj" \
	"$(INTDIR)\hornelement.obj" \
	"$(INTDIR)\impedancecurve.obj" \
	"$(INTDIR)\interface.obj" \
	"$(INTDIR)\rosenbrock.obj" \
	"$(INTDIR)\runga.obj" \
	"$(INTDIR)\runrb.obj" \
	"$(INTDIR)\simplematrix.obj" \
	"$(INTDIR)\strparsing.obj" \
	"$(INTDIR)\test_main.obj" \
	"$(INTDIR)\windinstrument.obj" \
	"..\..\lib\library.lib"

"$(OUTDIR)\ART.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ART - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ART.dll"


CLEAN :
	-@erase "$(INTDIR)\ART.obj"
	-@erase "$(INTDIR)\ART.sbr"
	-@erase "$(INTDIR)\ARTdataContainer.obj"
	-@erase "$(INTDIR)\ARTdataContainer.sbr"
	-@erase "$(INTDIR)\ARTerror.obj"
	-@erase "$(INTDIR)\ARTerror.sbr"
	-@erase "$(INTDIR)\ARTfunctionoid.obj"
	-@erase "$(INTDIR)\ARTfunctionoid.sbr"
	-@erase "$(INTDIR)\ARTlink.obj"
	-@erase "$(INTDIR)\ARTlink.sbr"
	-@erase "$(INTDIR)\cbabk2.obj"
	-@erase "$(INTDIR)\cbabk2.sbr"
	-@erase "$(INTDIR)\cbal.obj"
	-@erase "$(INTDIR)\cbal.sbr"
	-@erase "$(INTDIR)\cdiv.obj"
	-@erase "$(INTDIR)\cdiv.sbr"
	-@erase "$(INTDIR)\cg.obj"
	-@erase "$(INTDIR)\cg.sbr"
	-@erase "$(INTDIR)\comqr.obj"
	-@erase "$(INTDIR)\comqr.sbr"
	-@erase "$(INTDIR)\comqr2.obj"
	-@erase "$(INTDIR)\comqr2.sbr"
	-@erase "$(INTDIR)\corth.obj"
	-@erase "$(INTDIR)\corth.sbr"
	-@erase "$(INTDIR)\csroot.obj"
	-@erase "$(INTDIR)\csroot.sbr"
	-@erase "$(INTDIR)\hornelement.obj"
	-@erase "$(INTDIR)\hornelement.sbr"
	-@erase "$(INTDIR)\impedancecurve.obj"
	-@erase "$(INTDIR)\impedancecurve.sbr"
	-@erase "$(INTDIR)\interface.obj"
	-@erase "$(INTDIR)\interface.sbr"
	-@erase "$(INTDIR)\legquadrature.obj"
	-@erase "$(INTDIR)\legquadrature.sbr"
	-@erase "$(INTDIR)\mpARTValue.obj"
	-@erase "$(INTDIR)\mpARTValue.sbr"
	-@erase "$(INTDIR)\mpError.obj"
	-@erase "$(INTDIR)\mpError.sbr"
	-@erase "$(INTDIR)\mpFuncCmplx.obj"
	-@erase "$(INTDIR)\mpFuncCmplx.sbr"
	-@erase "$(INTDIR)\mpFuncCommon.obj"
	-@erase "$(INTDIR)\mpFuncCommon.sbr"
	-@erase "$(INTDIR)\mpFuncNonCmplx.obj"
	-@erase "$(INTDIR)\mpFuncNonCmplx.sbr"
	-@erase "$(INTDIR)\mpFuncStr.obj"
	-@erase "$(INTDIR)\mpFuncStr.sbr"
	-@erase "$(INTDIR)\mpICallback.obj"
	-@erase "$(INTDIR)\mpICallback.sbr"
	-@erase "$(INTDIR)\mpIfThenElse.obj"
	-@erase "$(INTDIR)\mpIfThenElse.sbr"
	-@erase "$(INTDIR)\mpIOprt.obj"
	-@erase "$(INTDIR)\mpIOprt.sbr"
	-@erase "$(INTDIR)\mpIPackage.obj"
	-@erase "$(INTDIR)\mpIPackage.sbr"
	-@erase "$(INTDIR)\mpIToken.obj"
	-@erase "$(INTDIR)\mpIToken.sbr"
	-@erase "$(INTDIR)\mpIValReader.obj"
	-@erase "$(INTDIR)\mpIValReader.sbr"
	-@erase "$(INTDIR)\mpIValue.obj"
	-@erase "$(INTDIR)\mpIValue.sbr"
	-@erase "$(INTDIR)\mpOprtBinAssign.obj"
	-@erase "$(INTDIR)\mpOprtBinAssign.sbr"
	-@erase "$(INTDIR)\mpOprtBinCommon.obj"
	-@erase "$(INTDIR)\mpOprtBinCommon.sbr"
	-@erase "$(INTDIR)\mpOprtCmplx.obj"
	-@erase "$(INTDIR)\mpOprtCmplx.sbr"
	-@erase "$(INTDIR)\mpOprtNonCmplx.obj"
	-@erase "$(INTDIR)\mpOprtNonCmplx.sbr"
	-@erase "$(INTDIR)\mpOverload.obj"
	-@erase "$(INTDIR)\mpOverload.sbr"
	-@erase "$(INTDIR)\mpPackageCmplx.obj"
	-@erase "$(INTDIR)\mpPackageCmplx.sbr"
	-@erase "$(INTDIR)\mpPackageCommon.obj"
	-@erase "$(INTDIR)\mpPackageCommon.sbr"
	-@erase "$(INTDIR)\mpPackageNonCmplx.obj"
	-@erase "$(INTDIR)\mpPackageNonCmplx.sbr"
	-@erase "$(INTDIR)\mpPackageStr.obj"
	-@erase "$(INTDIR)\mpPackageStr.sbr"
	-@erase "$(INTDIR)\mpPackageUnit.obj"
	-@erase "$(INTDIR)\mpPackageUnit.sbr"
	-@erase "$(INTDIR)\mpParser.obj"
	-@erase "$(INTDIR)\mpParser.sbr"
	-@erase "$(INTDIR)\mpParserBase.obj"
	-@erase "$(INTDIR)\mpParserBase.sbr"
	-@erase "$(INTDIR)\mpRPN.obj"
	-@erase "$(INTDIR)\mpRPN.sbr"
	-@erase "$(INTDIR)\mpTest.obj"
	-@erase "$(INTDIR)\mpTest.sbr"
	-@erase "$(INTDIR)\mpTokenReader.obj"
	-@erase "$(INTDIR)\mpTokenReader.sbr"
	-@erase "$(INTDIR)\mpValReader.obj"
	-@erase "$(INTDIR)\mpValReader.sbr"
	-@erase "$(INTDIR)\mpValue.obj"
	-@erase "$(INTDIR)\mpValue.sbr"
	-@erase "$(INTDIR)\mpValueCache.obj"
	-@erase "$(INTDIR)\mpValueCache.sbr"
	-@erase "$(INTDIR)\mpVariable.obj"
	-@erase "$(INTDIR)\mpVariable.sbr"
	-@erase "$(INTDIR)\pythag.obj"
	-@erase "$(INTDIR)\pythag.sbr"
	-@erase "$(INTDIR)\rosenbrock.obj"
	-@erase "$(INTDIR)\rosenbrock.sbr"
	-@erase "$(INTDIR)\runga.obj"
	-@erase "$(INTDIR)\runga.sbr"
	-@erase "$(INTDIR)\runrb.obj"
	-@erase "$(INTDIR)\runrb.sbr"
	-@erase "$(INTDIR)\simplematrix.obj"
	-@erase "$(INTDIR)\simplematrix.sbr"
	-@erase "$(INTDIR)\strparsing.obj"
	-@erase "$(INTDIR)\strparsing.sbr"
	-@erase "$(INTDIR)\test_main.obj"
	-@erase "$(INTDIR)\test_main.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\windinstrument.obj"
	-@erase "$(INTDIR)\windinstrument.sbr"
	-@erase "$(OUTDIR)\ART.bsc"
	-@erase "$(OUTDIR)\ART.exe"
	-@erase "$(OUTDIR)\ART.ilk"
	-@erase "$(OUTDIR)\ART.pdb"
	-@erase "$(OUTDIR)\ART.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
#DEBUG!
#war das hier: CPP_PROJ=/nologo /Gz /MTd /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FFT_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ART.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c  
#neues debug:
#aus alter make-datei
#CPP_PROJ=/nologo /Gz /MTd /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FFT_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ART.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

CPP_PROJ=/nologo /MTd /w /W0 /Gm /GR /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "DLL" /D "_DEBUG" /D "_MBCS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\ART.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ART.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\cbabk2.sbr" \
	"$(INTDIR)\cbal.sbr" \
	"$(INTDIR)\cdiv.sbr" \
	"$(INTDIR)\cg.sbr" \
	"$(INTDIR)\comqr.sbr" \
	"$(INTDIR)\comqr2.sbr" \
	"$(INTDIR)\corth.sbr" \
	"$(INTDIR)\csroot.sbr" \
	"$(INTDIR)\legquadrature.sbr" \
	"$(INTDIR)\pythag.sbr" \
	"$(INTDIR)\mpARTValue.sbr" \
	"$(INTDIR)\mpError.sbr" \
	"$(INTDIR)\mpFuncCmplx.sbr" \
	"$(INTDIR)\mpFuncCommon.sbr" \
	"$(INTDIR)\mpFuncNonCmplx.sbr" \
	"$(INTDIR)\mpFuncStr.sbr" \
	"$(INTDIR)\mpICallback.sbr" \
	"$(INTDIR)\mpIfThenElse.sbr" \
	"$(INTDIR)\mpIOprt.sbr" \
	"$(INTDIR)\mpIPackage.sbr" \
	"$(INTDIR)\mpIToken.sbr" \
	"$(INTDIR)\mpIValReader.sbr" \
	"$(INTDIR)\mpIValue.sbr" \
	"$(INTDIR)\mpOprtBinAssign.sbr" \
	"$(INTDIR)\mpOprtBinCommon.sbr" \
	"$(INTDIR)\mpOprtCmplx.sbr" \
	"$(INTDIR)\mpOprtNonCmplx.sbr" \
	"$(INTDIR)\mpOverload.sbr" \
	"$(INTDIR)\mpPackageCmplx.sbr" \
	"$(INTDIR)\mpPackageCommon.sbr" \
	"$(INTDIR)\mpPackageNonCmplx.sbr" \
	"$(INTDIR)\mpPackageStr.sbr" \
	"$(INTDIR)\mpPackageUnit.sbr" \
	"$(INTDIR)\mpParser.sbr" \
	"$(INTDIR)\mpParserBase.sbr" \
	"$(INTDIR)\mpRPN.sbr" \
	"$(INTDIR)\mpTest.sbr" \
	"$(INTDIR)\mpTokenReader.sbr" \
	"$(INTDIR)\mpValReader.sbr" \
	"$(INTDIR)\mpValue.sbr" \
	"$(INTDIR)\mpValueCache.sbr" \
	"$(INTDIR)\mpVariable.sbr" \
	"$(INTDIR)\ART.sbr" \
	"$(INTDIR)\ARTdataContainer.sbr" \
	"$(INTDIR)\ARTerror.sbr" \
	"$(INTDIR)\ARTfunctionoid.sbr" \
	"$(INTDIR)\ARTlink.sbr" \
	"$(INTDIR)\hornelement.sbr" \
	"$(INTDIR)\impedancecurve.sbr" \
	"$(INTDIR)\interface.sbr" \
	"$(INTDIR)\rosenbrock.sbr" \
	"$(INTDIR)\runga.sbr" \
	"$(INTDIR)\runrb.sbr" \
	"$(INTDIR)\simplematrix.sbr" \
	"$(INTDIR)\strparsing.sbr" \
	"$(INTDIR)\test_main.sbr" \
	"$(INTDIR)\windinstrument.sbr"

"$(OUTDIR)\ART.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
#LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /nodefaultlib:"msvcprtd.lib" /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libc.lib" /incremental:yes /pdb:"$(OUTDIR)\ART.pdb" /debug /machine:I386 /def:"..\ART.def" /out:"$(OUTDIR)\ART.dll" /implib:"$(OUTDIR)\ART.lib" 
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /nodefaultlib:"msvcprtd.lib" /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libc.lib" /incremental:yes /pdb:"$(OUTDIR)\ART.pdb" /debug /machine:I386 /def:"..\ART.def" /out:"$(OUTDIR)\ART.dll" /implib:"$(OUTDIR)\ART.lib" 
DEF_FILE= \
	"..\ART.def"
LINK32_OBJS= \
	"$(INTDIR)\cbabk2.obj" \
	"$(INTDIR)\cbal.obj" \
	"$(INTDIR)\cdiv.obj" \
	"$(INTDIR)\cg.obj" \
	"$(INTDIR)\comqr.obj" \
	"$(INTDIR)\comqr2.obj" \
	"$(INTDIR)\corth.obj" \
	"$(INTDIR)\csroot.obj" \
	"$(INTDIR)\legquadrature.obj" \
	"$(INTDIR)\pythag.obj" \
	"$(INTDIR)\mpARTValue.obj" \
	"$(INTDIR)\mpError.obj" \
	"$(INTDIR)\mpFuncCmplx.obj" \
	"$(INTDIR)\mpFuncCommon.obj" \
	"$(INTDIR)\mpFuncNonCmplx.obj" \
	"$(INTDIR)\mpFuncStr.obj" \
	"$(INTDIR)\mpICallback.obj" \
	"$(INTDIR)\mpIfThenElse.obj" \
	"$(INTDIR)\mpIOprt.obj" \
	"$(INTDIR)\mpIPackage.obj" \
	"$(INTDIR)\mpIToken.obj" \
	"$(INTDIR)\mpIValReader.obj" \
	"$(INTDIR)\mpIValue.obj" \
	"$(INTDIR)\mpOprtBinAssign.obj" \
	"$(INTDIR)\mpOprtBinCommon.obj" \
	"$(INTDIR)\mpOprtCmplx.obj" \
	"$(INTDIR)\mpOprtNonCmplx.obj" \
	"$(INTDIR)\mpOverload.obj" \
	"$(INTDIR)\mpPackageCmplx.obj" \
	"$(INTDIR)\mpPackageCommon.obj" \
	"$(INTDIR)\mpPackageNonCmplx.obj" \
	"$(INTDIR)\mpPackageStr.obj" \
	"$(INTDIR)\mpPackageUnit.obj" \
	"$(INTDIR)\mpParser.obj" \
	"$(INTDIR)\mpParserBase.obj" \
	"$(INTDIR)\mpRPN.obj" \
	"$(INTDIR)\mpTest.obj" \
	"$(INTDIR)\mpTokenReader.obj" \
	"$(INTDIR)\mpValReader.obj" \
	"$(INTDIR)\mpValue.obj" \
	"$(INTDIR)\mpValueCache.obj" \
	"$(INTDIR)\mpVariable.obj" \
	"$(INTDIR)\ART.obj" \
	"$(INTDIR)\ARTdataContainer.obj" \
	"$(INTDIR)\ARTerror.obj" \
	"$(INTDIR)\ARTfunctionoid.obj" \
	"$(INTDIR)\ARTlink.obj" \
	"$(INTDIR)\hornelement.obj" \
	"$(INTDIR)\impedancecurve.obj" \
	"$(INTDIR)\interface.obj" \
	"$(INTDIR)\rosenbrock.obj" \
	"$(INTDIR)\runga.obj" \
	"$(INTDIR)\runrb.obj" \
	"$(INTDIR)\simplematrix.obj" \
	"$(INTDIR)\strparsing.obj" \
	"$(INTDIR)\test_main.obj" \
	"$(INTDIR)\windinstrument.obj" \
	"..\..\lib\library.lib"

"$(OUTDIR)\ART.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ART.dep")
!INCLUDE "ART.dep"
!ELSE 
!MESSAGE Warning: cannot find "ART.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ART - Win32 Debug" || "$(CFG)" == "ART - Win32 Release" || "$(CFG)" == "ART - Win32 Debug ARTInterface"
SOURCE=..\fortran\cbabk2.c

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\cbabk2.obj"	"$(INTDIR)\cbabk2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\cbabk2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\cbabk2.obj"	"$(INTDIR)\cbabk2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\fortran\cbal.c

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\cbal.obj"	"$(INTDIR)\cbal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\cbal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\cbal.obj"	"$(INTDIR)\cbal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\fortran\cdiv.c

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\cdiv.obj"	"$(INTDIR)\cdiv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\cdiv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\cdiv.obj"	"$(INTDIR)\cdiv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\fortran\cg.c

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\cg.obj"	"$(INTDIR)\cg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\cg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\cg.obj"	"$(INTDIR)\cg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\fortran\comqr.c

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\comqr.obj"	"$(INTDIR)\comqr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\comqr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\comqr.obj"	"$(INTDIR)\comqr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\fortran\comqr2.c

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\comqr2.obj"	"$(INTDIR)\comqr2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\comqr2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\comqr2.obj"	"$(INTDIR)\comqr2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\fortran\corth.c

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\corth.obj"	"$(INTDIR)\corth.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\corth.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\corth.obj"	"$(INTDIR)\corth.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\fortran\csroot.c

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\csroot.obj"	"$(INTDIR)\csroot.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\csroot.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\csroot.obj"	"$(INTDIR)\csroot.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\legquadrature.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\legquadrature.obj"	"$(INTDIR)\legquadrature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\legquadrature.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\legquadrature.obj"	"$(INTDIR)\legquadrature.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\fortran\pythag.c

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\pythag.obj"	"$(INTDIR)\pythag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\pythag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\pythag.obj"	"$(INTDIR)\pythag.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpARTValue.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpARTValue.obj"	"$(INTDIR)\mpARTValue.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpARTValue.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpARTValue.obj"	"$(INTDIR)\mpARTValue.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpError.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpError.obj"	"$(INTDIR)\mpError.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpError.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpError.obj"	"$(INTDIR)\mpError.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpFuncCmplx.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpFuncCmplx.obj"	"$(INTDIR)\mpFuncCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpFuncCmplx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpFuncCmplx.obj"	"$(INTDIR)\mpFuncCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpFuncCommon.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpFuncCommon.obj"	"$(INTDIR)\mpFuncCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpFuncCommon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpFuncCommon.obj"	"$(INTDIR)\mpFuncCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpFuncNonCmplx.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpFuncNonCmplx.obj"	"$(INTDIR)\mpFuncNonCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpFuncNonCmplx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpFuncNonCmplx.obj"	"$(INTDIR)\mpFuncNonCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpFuncStr.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpFuncStr.obj"	"$(INTDIR)\mpFuncStr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpFuncStr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpFuncStr.obj"	"$(INTDIR)\mpFuncStr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpICallback.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpICallback.obj"	"$(INTDIR)\mpICallback.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpICallback.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpICallback.obj"	"$(INTDIR)\mpICallback.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpIfThenElse.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpIfThenElse.obj"	"$(INTDIR)\mpIfThenElse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpIfThenElse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpIfThenElse.obj"	"$(INTDIR)\mpIfThenElse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpIOprt.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpIOprt.obj"	"$(INTDIR)\mpIOprt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpIOprt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpIOprt.obj"	"$(INTDIR)\mpIOprt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpIPackage.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpIPackage.obj"	"$(INTDIR)\mpIPackage.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpIPackage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpIPackage.obj"	"$(INTDIR)\mpIPackage.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpIToken.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpIToken.obj"	"$(INTDIR)\mpIToken.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpIToken.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpIToken.obj"	"$(INTDIR)\mpIToken.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpIValReader.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpIValReader.obj"	"$(INTDIR)\mpIValReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpIValReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpIValReader.obj"	"$(INTDIR)\mpIValReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpIValue.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpIValue.obj"	"$(INTDIR)\mpIValue.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpIValue.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpIValue.obj"	"$(INTDIR)\mpIValue.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpOprtBinAssign.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpOprtBinAssign.obj"	"$(INTDIR)\mpOprtBinAssign.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpOprtBinAssign.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpOprtBinAssign.obj"	"$(INTDIR)\mpOprtBinAssign.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpOprtBinCommon.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpOprtBinCommon.obj"	"$(INTDIR)\mpOprtBinCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpOprtBinCommon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpOprtBinCommon.obj"	"$(INTDIR)\mpOprtBinCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpOprtCmplx.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpOprtCmplx.obj"	"$(INTDIR)\mpOprtCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpOprtCmplx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpOprtCmplx.obj"	"$(INTDIR)\mpOprtCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpOprtNonCmplx.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpOprtNonCmplx.obj"	"$(INTDIR)\mpOprtNonCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpOprtNonCmplx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpOprtNonCmplx.obj"	"$(INTDIR)\mpOprtNonCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpOverload.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpOverload.obj"	"$(INTDIR)\mpOverload.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpOverload.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpOverload.obj"	"$(INTDIR)\mpOverload.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpPackageCmplx.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpPackageCmplx.obj"	"$(INTDIR)\mpPackageCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpPackageCmplx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpPackageCmplx.obj"	"$(INTDIR)\mpPackageCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpPackageCommon.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpPackageCommon.obj"	"$(INTDIR)\mpPackageCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpPackageCommon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpPackageCommon.obj"	"$(INTDIR)\mpPackageCommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpPackageNonCmplx.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpPackageNonCmplx.obj"	"$(INTDIR)\mpPackageNonCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpPackageNonCmplx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpPackageNonCmplx.obj"	"$(INTDIR)\mpPackageNonCmplx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpPackageStr.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpPackageStr.obj"	"$(INTDIR)\mpPackageStr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpPackageStr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpPackageStr.obj"	"$(INTDIR)\mpPackageStr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpPackageUnit.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpPackageUnit.obj"	"$(INTDIR)\mpPackageUnit.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpPackageUnit.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpPackageUnit.obj"	"$(INTDIR)\mpPackageUnit.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpParser.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpParser.obj"	"$(INTDIR)\mpParser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpParser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpParser.obj"	"$(INTDIR)\mpParser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpParserBase.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpParserBase.obj"	"$(INTDIR)\mpParserBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpParserBase.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpParserBase.obj"	"$(INTDIR)\mpParserBase.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpRPN.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpRPN.obj"	"$(INTDIR)\mpRPN.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpRPN.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpRPN.obj"	"$(INTDIR)\mpRPN.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpTest.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpTest.obj"	"$(INTDIR)\mpTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpTest.obj"	"$(INTDIR)\mpTest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpTokenReader.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpTokenReader.obj"	"$(INTDIR)\mpTokenReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpTokenReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpTokenReader.obj"	"$(INTDIR)\mpTokenReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpValReader.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpValReader.obj"	"$(INTDIR)\mpValReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpValReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpValReader.obj"	"$(INTDIR)\mpValReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpValue.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpValue.obj"	"$(INTDIR)\mpValue.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpValue.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpValue.obj"	"$(INTDIR)\mpValue.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpValueCache.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpValueCache.obj"	"$(INTDIR)\mpValueCache.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpValueCache.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpValueCache.obj"	"$(INTDIR)\mpValueCache.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\include\muParser\mpVariable.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\mpVariable.obj"	"$(INTDIR)\mpVariable.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\mpVariable.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\mpVariable.obj"	"$(INTDIR)\mpVariable.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ART.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\ART.obj"	"$(INTDIR)\ART.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\ART.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\ART.obj"	"$(INTDIR)\ART.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ARTdataContainer.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\ARTdataContainer.obj"	"$(INTDIR)\ARTdataContainer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\ARTdataContainer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\ARTdataContainer.obj"	"$(INTDIR)\ARTdataContainer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ARTerror.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\ARTerror.obj"	"$(INTDIR)\ARTerror.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\ARTerror.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\ARTerror.obj"	"$(INTDIR)\ARTerror.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ARTfunctionoid.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\ARTfunctionoid.obj"	"$(INTDIR)\ARTfunctionoid.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\ARTfunctionoid.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\ARTfunctionoid.obj"	"$(INTDIR)\ARTfunctionoid.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\ARTlink.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\ARTlink.obj"	"$(INTDIR)\ARTlink.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\ARTlink.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\ARTlink.obj"	"$(INTDIR)\ARTlink.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\hornelement.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\hornelement.obj"	"$(INTDIR)\hornelement.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\hornelement.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\hornelement.obj"	"$(INTDIR)\hornelement.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\impedancecurve.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\impedancecurve.obj"	"$(INTDIR)\impedancecurve.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\impedancecurve.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\impedancecurve.obj"	"$(INTDIR)\impedancecurve.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\interface.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\interface.obj"	"$(INTDIR)\interface.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\interface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\interface.obj"	"$(INTDIR)\interface.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\rosenbrock.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\rosenbrock.obj"	"$(INTDIR)\rosenbrock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\rosenbrock.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\rosenbrock.obj"	"$(INTDIR)\rosenbrock.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\runga.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\runga.obj"	"$(INTDIR)\runga.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\runga.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\runga.obj"	"$(INTDIR)\runga.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\runrb.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\runrb.obj"	"$(INTDIR)\runrb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\runrb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\runrb.obj"	"$(INTDIR)\runrb.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\simplematrix.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\simplematrix.obj"	"$(INTDIR)\simplematrix.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\simplematrix.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\simplematrix.obj"	"$(INTDIR)\simplematrix.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\strparsing.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\strparsing.obj"	"$(INTDIR)\strparsing.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\strparsing.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\strparsing.obj"	"$(INTDIR)\strparsing.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\test_main.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\test_main.obj"	"$(INTDIR)\test_main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\test_main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\test_main.obj"	"$(INTDIR)\test_main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\windinstrument.cpp

!IF  "$(CFG)" == "ART - Win32 Debug"


"$(INTDIR)\windinstrument.obj"	"$(INTDIR)\windinstrument.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Release"


"$(INTDIR)\windinstrument.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"


"$(INTDIR)\windinstrument.obj"	"$(INTDIR)\windinstrument.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

