# Microsoft Developer Studio Generated NMAKE File, Based on main.dsp
!IF "$(CFG)" == ""
CFG=main - Win32 Debug
!MESSAGE No configuration specified. Defaulting to main - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "main - Win32 Release" && "$(CFG)" != "main - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "main.mak" CFG="main - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "main - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "main - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "main - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\MMSimul.dll"


CLEAN :
	-@erase "$(INTDIR)\cbabk2.obj"
	-@erase "$(INTDIR)\cbal.obj"
	-@erase "$(INTDIR)\cdiv.obj"
	-@erase "$(INTDIR)\cg.obj"
	-@erase "$(INTDIR)\comqr.obj"
	-@erase "$(INTDIR)\comqr2.obj"
	-@erase "$(INTDIR)\corth.obj"
	-@erase "$(INTDIR)\csroot.obj"
	-@erase "$(INTDIR)\hornelement.obj"
	-@erase "$(INTDIR)\impedancecurve.obj"
	-@erase "$(INTDIR)\legquadrature.obj"
	-@erase "$(INTDIR)\pythag.obj"
	-@erase "$(INTDIR)\rosenbrock.obj"
	-@erase "$(INTDIR)\simplematrix.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\windinstrument.obj"
	-@erase "$(OUTDIR)\MMSimul.obj"
	-@erase "$(OUTDIR)\MMSimul.dll"
	-@erase "$(OUTDIR)\MMSimul.exp"
	-@erase "$(OUTDIR)\MMSimul.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GR /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FFT_EXPORTS" /Fp"$(INTDIR)\MMSimul.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MMSimul.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /incremental:no /nodefaultlib:"libcd.lib" /pdb:"$(OUTDIR)\MMSimul.pdb" /machine:I386 /def:".\MMSimul.def" /out:"$(OUTDIR)\MMSimul.dll" /implib:"$(OUTDIR)\MMSimul.lib" 
DEF_FILE= \
	".\MMSimul.def"
LINK32_OBJS= \
	"$(INTDIR)\cbabk2.obj" \
	"$(INTDIR)\cbal.obj" \
	"$(INTDIR)\cdiv.obj" \
	"$(INTDIR)\cg.obj" \
	"$(INTDIR)\comqr.obj" \
	"$(INTDIR)\comqr2.obj" \
	"$(INTDIR)\corth.obj" \
	"$(INTDIR)\csroot.obj" \
	"$(INTDIR)\hornelement.obj" \
	"$(INTDIR)\impedancecurve.obj" \
	"$(INTDIR)\legquadrature.obj" \
	"$(INTDIR)\MMSimul.obj" \
	"$(INTDIR)\pythag.obj" \
	"$(INTDIR)\rosenbrock.obj" \
	"$(INTDIR)\simplematrix.obj" \
	"$(INTDIR)\windinstrument.obj" \
	"..\lib\library.lib"

"$(OUTDIR)\MMSimul.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "main - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\MMSimul.dll"


CLEAN :
	-@erase "$(INTDIR)\cbabk2.obj"
	-@erase "$(INTDIR)\cbal.obj"
	-@erase "$(INTDIR)\cdiv.obj"
	-@erase "$(INTDIR)\cg.obj"
	-@erase "$(INTDIR)\comqr.obj"
	-@erase "$(INTDIR)\comqr2.obj"
	-@erase "$(INTDIR)\corth.obj"
	-@erase "$(INTDIR)\csroot.obj"
	-@erase "$(INTDIR)\hornelement.obj"
	-@erase "$(INTDIR)\impedancecurve.obj"
	-@erase "$(INTDIR)\legquadrature.obj"
	-@erase "$(INTDIR)\MMSimul.obj"
	-@erase "$(INTDIR)\pythag.obj"
	-@erase "$(INTDIR)\rosenbrock.obj"
	-@erase "$(INTDIR)\simplematrix.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\windinstrument.obj"
	-@erase "$(OUTDIR)\MMSimul.dll"
	-@erase "$(OUTDIR)\MMSimul.exp"
	-@erase "$(OUTDIR)\MMSimul.ilk"
	-@erase "$(OUTDIR)\MMSimul.lib"
	-@erase "$(OUTDIR)\MMSimul.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Gz /MTd /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FFT_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MMSimul.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MMSimul.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\MMSimul.sbr"
	
"$(OUTDIR)\MMSimul.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /nodefaultlib:"libcmtd.lib" /nodefaultlib:"libcpmtd.lib" /incremental:yes /pdb:"$(OUTDIR)\MMSimul.pdb" /debug /machine:I386 /def:".\MMSimul.def" /out:"$(OUTDIR)\MMSimul.dll" /implib:"$(OUTDIR)\MMSimul.lib" 
DEF_FILE= \
	".\MMSimul.def"
LINK32_OBJS= \
	"$(INTDIR)\cbabk2.obj" \
	"$(INTDIR)\cbal.obj" \
	"$(INTDIR)\cdiv.obj" \
	"$(INTDIR)\cg.obj" \
	"$(INTDIR)\comqr.obj" \
	"$(INTDIR)\comqr2.obj" \
	"$(INTDIR)\corth.obj" \
	"$(INTDIR)\csroot.obj" \
	"$(INTDIR)\hornelement.obj" \
	"$(INTDIR)\impedancecurve.obj" \
	"$(INTDIR)\legquadrature.obj" \
	"$(INTDIR)\MMSimul.obj" \
	"$(INTDIR)\pythag.obj" \
	"$(INTDIR)\rosenbrock.obj" \
	"$(INTDIR)\simplematrix.obj" \
	"$(INTDIR)\windinstrument.obj" \
	"..\lib\libraryd.lib"

"$(OUTDIR)\MMSimul.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("MMSimul.dep")
!INCLUDE "MMSimul.dep"
!ELSE 
!MESSAGE Warning: cannot find "MMSimul.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "main - Win32 Release" || "$(CFG)" == "main - Win32 Debug"
SOURCE=.\fortran\cbabk2.c

"$(INTDIR)\cbabk2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\fortran\cbal.c

"$(INTDIR)\cbal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\fortran\cdiv.c

"$(INTDIR)\cdiv.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\fortran\cg.c

"$(INTDIR)\cg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\fortran\comqr.c

"$(INTDIR)\comqr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\fortran\comqr2.c

"$(INTDIR)\comqr2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\fortran\corth.c

"$(INTDIR)\corth.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\fortran\csroot.c

"$(INTDIR)\csroot.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\hornelement.cpp

"$(INTDIR)\hornelement.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\impedancecurve.cpp

"$(INTDIR)\impedancecurve.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\legquadrature.cpp

"$(INTDIR)\legquadrature.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MMSimul.cpp

"$(INTDIR)\MMSimul.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\fortran\pythag.c

"$(INTDIR)\pythag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\rosenbrock.cpp

"$(INTDIR)\rosenbrock.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\simplematrix.cpp

"$(INTDIR)\simplematrix.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\windinstrument.cpp

"$(INTDIR)\windinstrument.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

