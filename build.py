import subprocess
import sys
import os



if (os.path.exists("build")):
	dl=[]
	for r,ndl,fl in os.walk("build"):
		r=r.replace("\\","/").strip("/")+"/"
		for d in ndl:
			dl.insert(0,r+d)
		for f in fl:
			os.remove(r+f)
	for k in dl:
		os.rmdir(k)
else:
	os.mkdir("build")
cd=os.getcwd()
os.chdir("build")
if (subprocess.run(["cl","/c","/permissive-","/GS","/utf-8","/W3","/Zc:wchar_t","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:forScope","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","../src/main.c","../src/lego_wireless_protocol/*.c","/I","../src/include"]).returncode!=0 or subprocess.run(["cl","/c","/permissive-","/GS","/utf-8","/W3","/ZW","/Zc:wchar_t","/await","/std:c++17","/Gm-","/sdl","/Zc:inline","/fp:precise","/D","_DEBUG","/D","_WINDOWS","/D","_UNICODE","/D","UNICODE","/errorReport:none","/WX","/Zc:twoPhase-","/Gd","/Oi","/FC","/EHsc","/nologo","/diagnostics:column","/ZI","/Od","/RTC1","/MDd","../src/lego_wireless_protocol/*.cpp","/I","../src/include"]).returncode!=0 or subprocess.run(["link","*.obj","/OUT:lego_wireless_protocol.exe","/DYNAMICBASE","kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib","shell32.lib","ole32.lib","oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib","/MACHINE:X64","/SUBSYSTEM:CONSOLE","/ERRORREPORT:none","/NOLOGO","/TLBID:1","/WX","/DEBUG","/INCREMENTAL"]).returncode!=0):
	os.chdir(cd)
	sys.exit(1)
os.chdir(cd)
if ("--run" in sys.argv):
	subprocess.run(["build/lego_wireless_protocol.exe"])
