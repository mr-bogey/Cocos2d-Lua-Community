@echo off
if not exist .\win32 (
	mkdir .\win32
	cd .\win32
	cmake .. -G"Visual Studio 15 2017" -A Win32
) else (
	cd .\win32
	cmake --build .
)