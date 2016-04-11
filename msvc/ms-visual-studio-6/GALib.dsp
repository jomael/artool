# Microsoft Developer Studio Project File - Name="GALib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GALib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GALib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GALib.mak" CFG="GALib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GALib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GALib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GALib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\release"
# PROP Intermediate_Dir "GALib___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\include" /I "..\include\ga" /I "..\source" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"GALib.lib"

!ELSEIF  "$(CFG)" == "GALib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GALib___Win32_Debug"
# PROP BASE Intermediate_Dir "GALib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\debug"
# PROP Intermediate_Dir "GALib___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /Gm /GX /ZI /Od /I "..\include" /I "..\include\ga" /I "..\source" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"GALib.lib"

!ENDIF 

# Begin Target

# Name "GALib - Win32 Release"
# Name "GALib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\source\ga\GA1DArrayGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GA1DBinStrGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GA2DArrayGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GA2DBinStrGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GA3DArrayGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GA3DBinStrGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAAllele.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GABaseGA.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GABin2DecGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\gabincvt.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GABinStr.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GADCrowdingGA.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GADemeGA.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\gaerror.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAIncGA.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAList.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAListBASE.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAListGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAParameter.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAPopulation.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\garandom.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GARealGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAScaling.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GASelector.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GASimpleGA.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GASStateGA.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAStatistics.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GAStringGenome.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GATree.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GATreeBASE.cpp
# End Source File
# Begin Source File

SOURCE=..\source\ga\GATreeGenome.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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
# End Target
# End Project
