# Microsoft Developer Studio Project File - Name="ART" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ART - Win32 Debug ARTInterface
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ART.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ART.mak" CFG="ART - Win32 Debug ARTInterface"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ART - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ART - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ART - Win32 Debug ARTInterface" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ART - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\debug"
# PROP Intermediate_Dir "ART___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\include\art" /I "..\include\muParser" /I "..\include\ga" /I "..\include\fortran" /I "..\source" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fortran.lib GALib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ART - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ART___Win32_Release"
# PROP BASE Intermediate_Dir "ART___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\release"
# PROP Intermediate_Dir "ART___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /GR /GX /Ot /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Ox /Og /Fr
# ADD CPP /nologo /w /W0 /GR /GX /Ot /I "..\include" /I "..\include\art" /I "..\include\muParser" /I "..\include\ga" /I "..\include\fortran" /I "..\source" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "WIN32" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:none /machine:I386
# SUBTRACT BASE LINK32 /profile /debug
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fortran.lib GALib.lib /nologo /subsystem:console /pdb:none /machine:I386 /nodefaultlib:"libc.lib"
# SUBTRACT LINK32 /profile /debug

!ELSEIF  "$(CFG)" == "ART - Win32 Debug ARTInterface"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ART___Win32_Debug_ARTInterface"
# PROP BASE Intermediate_Dir "ART___Win32_Debug_ARTInterface"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\debug_artinterface"
# PROP Intermediate_Dir "ART___Win32_Debug_ARTInterface"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /Gm /GR /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "ARTinterfaceTest" /D "_MBCS" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\include\art" /I "..\include\muParser" /I "..\include\ga" /I "..\include\fortran" /I "..\source" /D "WIN32" /D "_DEBUG" /D "ARTinterfaceTest" /D "_MBCS" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fortran.lib GALib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /profile /incremental:no

!ENDIF 

# Begin Target

# Name "ART - Win32 Debug"
# Name "ART - Win32 Release"
# Name "ART - Win32 Debug ARTInterface"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "art_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\source\art\Art.clp
# End Source File
# Begin Source File

SOURCE=..\source\art\ART.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\ARTdataContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\ARTerror.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\ARTfunctionoid.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\ARTlink.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\hornelement.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\impedancecurve.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\interface.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\legquadrature.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\rosenbrock.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\runga.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\runrb.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\simplematrix.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\strparsing.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\test_main.cpp
# End Source File
# Begin Source File

SOURCE=..\source\art\windinstrument.cpp
# End Source File
# End Group
# Begin Group "muparser_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\source\muParser\mpARTValue.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpError.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpFuncCmplx.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpFuncCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpFuncNonCmplx.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpFuncStr.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpICallback.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpIfThenElse.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpIOprt.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpIPackage.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpIToken.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpIValReader.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpIValue.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpOprtBinAssign.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpOprtBinCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpOprtCmplx.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpOprtNonCmplx.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpOverload.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpPackageCmplx.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpPackageCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpPackageNonCmplx.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpPackageStr.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpPackageUnit.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpParser.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpParserBase.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpRPN.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpTest.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpTokenReader.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpValReader.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpValue.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpValueCache.cpp
# End Source File
# Begin Source File

SOURCE=..\source\muParser\mpVariable.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "art_h"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\art\ART.h
# End Source File
# Begin Source File

SOURCE=..\include\art\ARTdataContainer.h
# End Source File
# Begin Source File

SOURCE=..\include\art\ARTerror.h
# End Source File
# Begin Source File

SOURCE=..\include\art\ARTfunctionoid.h
# End Source File
# Begin Source File

SOURCE=..\include\art\ARTlink.h
# End Source File
# Begin Source File

SOURCE=..\include\art\ARTmodel.h
# End Source File
# Begin Source File

SOURCE=..\include\art\ARTmodelinterface.h
# End Source File
# Begin Source File

SOURCE=..\include\art\ARTwaveObject.h
# End Source File
# Begin Source File

SOURCE=..\include\art\autoinit.hpp
# End Source File
# Begin Source File

SOURCE=..\include\art\common_vs6.hpp
# End Source File
# Begin Source File

SOURCE=..\include\art\error_vs6.hpp
# End Source File
# Begin Source File

SOURCE=..\include\art\hornelement.h
# End Source File
# Begin Source File

SOURCE=..\include\art\impedancecurve.h
# End Source File
# Begin Source File

SOURCE=..\include\art\interface.h
# End Source File
# Begin Source File

SOURCE=..\include\art\legquadrature.h
# End Source File
# Begin Source File

SOURCE=..\include\art\matrix.h
# End Source File
# Begin Source File

SOURCE=..\include\art\MMSimul.h
# End Source File
# Begin Source File

SOURCE=..\include\art\rosenbrock.h
# End Source File
# Begin Source File

SOURCE=..\include\art\simplematrix.h
# End Source File
# Begin Source File

SOURCE=..\include\art\strparsing.h
# End Source File
# Begin Source File

SOURCE=..\include\art\windinstrument.h
# End Source File
# End Group
# Begin Group "fortran_h"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\fortran\f2c.h
# End Source File
# End Group
# Begin Group "ga_h"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\ga\ga.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GA1DArrayGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GA1DBinStrGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GA2DArrayGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GA2DBinStrGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GA3DArrayGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GA3DBinStrGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAAllele.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAArray.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GABaseGA.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GABin2DecGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\gabincvt.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GABinStr.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\gaconfig.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GADCrowdingGA.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GADemeGA.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\gaerror.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAEvalData.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\gaid.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAIncGA.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAList.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAListBASE.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAListGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAMask.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GANode.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAParameter.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAPopulation.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\garandom.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GARealGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAScaling.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GASelector.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GASimpleGA.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GASStateGA.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAStatistics.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GAStringGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GATree.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GATreeBASE.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\GATreeGenome.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\gatypes.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\gaversion.h
# End Source File
# Begin Source File

SOURCE=..\include\ga\std_stream.h
# End Source File
# End Group
# Begin Group "muparser_h"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\muParser\memory.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpARTValue.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpBuffer.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpDefines.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpError.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpFuncCmplx.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpFuncCommon.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpFuncNonCmplx.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpFuncStr.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpFwdDecl.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpICallback.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpIfThenElse.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpIOprt.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpIPackage.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpIToken.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpIValReader.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpIValue.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpOprtBinAssign.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpOprtBinCommon.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpOprtCmplx.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpOprtNonCmplx.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpOverload.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpPackageCmplx.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpPackageCommon.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpPackageNonCmplx.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpPackageStr.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpPackageUnit.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpParser.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpParserBase.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpRPN.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpStack.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpTest.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpTokenReader.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpTypes.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpValReader.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpValue.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpValueCache.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\mpVariable.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\suSortPred.h
# End Source File
# Begin Source File

SOURCE=..\include\muParser\utGeneric.h
# End Source File
# End Group
# End Group
# End Target
# End Project
