echo off
if "%MSVCDir%"=="" goto doit
goto done

:doit
call "C:\programme\Microsoft Visual Studio\VC98\Bin\VCvars32.bat" 

:done
