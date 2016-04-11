::set cfg="ART - Win32 Release"
::if "%2"=="" goto nodebug
::set cfg="ART - Win32 Debug"

set cfg="ART - Win32 Debug"
set cfg_rel="ART - Win32 Release"

:nodebug
call VCSetup.bat

if "%1"=="release" goto release
if "%1"=="clean" goto clean

:make
NMAKE -f ARTDLL.mak CFG=%cfg%
goto done

:clean
NMAKE -f ARTDLL.mak CFG=%cfg% clean
goto done

:release
NMAKE -f ARTDLL.mak CFG=%cfg_rel%
::copy Release\ART.dll ..\..\..\ArtSim\Source
goto done

:usage
echo usage: MakeARTdll clean|make|release [debug]

:done
