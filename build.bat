@echo off
cls
set _INCLUDE=%INCLUDE%
set INCLUDE=../src/include;%INCLUDE%
set LIBPATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.28.29333\lib\x86\store\references;C:\Program Files (x86)\Windows Kits\10\UnionMetadata\10.0.19041.0
if exist build rmdir /s /q build
mkdir build
cd build
if %1.==. goto dbg
if %1==-r (
	cl /c /permissive- /GS /utf-8 /W3 /Zc:wchar_t /Gm- /sdl /Zc:inline /fp:precise /D "NDEBUG"  /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /errorReport:none /WX /Zc:forScope /Gd /Oi /FC /EHsc /nologo /diagnostics:column /GL /Gy /Zi /O2 /Oi /MD ../src/main.c ../src/lego_wireless_protocol/*.c&&link *.obj /OUT:lego_wireless_protocol.exe /DYNAMICBASE "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MACHINE:X64 /SUBSYSTEM:CONSOLE /ERRORREPORT:none /NOLOGO /TLBID:1 /WX /LTCG /OPT:REF /INCREMENTAL:NO /OPT:ICF&&goto run
	goto end
)
:dbg
cl /c /permissive- /GS /utf-8 /W3 /Zc:wchar_t /Gm- /sdl /Zc:inline /fp:precise /D "_DEBUG"  /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /errorReport:none /WX /Zc:forScope /Gd /Oi /FC /EHsc /nologo /diagnostics:column /ZI /Od /RTC1 /MDd ../src/main.c ../src/lego_wireless_protocol/*.c&&cl /c /permissive- /GS /utf-8 /W3 /ZW /Zc:wchar_t /await /std:c++17 /Gm- /sdl /Zc:inline /fp:precise /D "_DEBUG"  /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /errorReport:none /WX /Zc:twoPhase- /Gd /Oi /FC /EHsc /nologo /diagnostics:column /ZI /Od /RTC1 /MDd ../src/lego_wireless_protocol/*.cpp&&link *.obj /OUT:lego_wireless_protocol.exe /DYNAMICBASE "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MACHINE:X64 /SUBSYSTEM:CONSOLE /ERRORREPORT:none /NOLOGO /TLBID:1 /WX /DEBUG /INCREMENTAL&&goto run
goto end
:run
rem del *.obj
rem del *.pdb
rem del *.exp
rem del *.ilk
rem del *.idb
cls
lego_wireless_protocol.exe
:end
cd ..
set INCLUDE=%_INCLUDE%
