@echo off

@REM echo "Clean up the environment ..."
@REM IF EXIST "Src\external_dependencies\vcpkg" (
@REM 	echo Deleting "Src\external_dependencies\vcpkg" ...
@REM 	rmdir /S /Q "Src\external_dependencies\vcpkg"
@REM 	echo "Src\external_dependencies\vcpkg was deleted!"
@REM )

@REM IF EXIST "%AppData%\..\local\vcpkg" (
@REM 	echo Deleting "%AppData%\..\local\vcpkg" ...
@REM 	rmdir /S /Q "%AppData%\..\local\vcpkg"
@REM 	echo "%AppData%\..\local\vcpkg was deleted!"
@REM )

@REM echo "Uncompress \Src\external_dependencies\CEF ..."
.\BuildTools\7-ZipPortable_22.01\App\7-Zip\7z.exe x .\Src\external_dependencies\CEF.7z.001 -y -o.\Src\external_dependencies

@REM Make vcpkg is installed in the users machine
WHERE /q vcpkg
IF %ERRORLEVEL% NEQ 0 (
	ECHO vcpkg is not installed, please install
	EXIT /b
)

ECHO Installing packages...
@REM find the current script directory and
@REM set the  --overlay-ports to the ports
set vcpkg_ports_dir=%~dp0

vcpkg install --overlay-ports="%vcpkg_ports_dir%vcpkg-ports" alac:x86-windows-static-md ^
  expat:x86-windows-static-md expat:x86-windows-static ^
  freetype:x86-windows-static-md ^
  ijg-libjpeg:x86-windows-static-md ^
  libflac:x86-windows-static-md ^
  libogg:x86-windows-static-md ^
  libpng:x86-windows-static-md ^
  libsndfile:x86-windows-static-md ^
  libtheora:x86-windows-static-md ^
  libvorbis:x86-windows-static-md ^
  libvpx:x86-windows-static-md ^
  minizip:x86-windows-static-md ^
  mp3lame:x86-windows-static-md ^
  mpg123:x86-windows-static-md ^
  openssl:x86-windows-static-md openssl:x86-windows-static ^
  pthread:x86-windows-static-md pthread:x86-windows-static ^
  restclient-cpp:x86-windows-static-md restclient-cpp:x86-windows-static ^
  spdlog:x86-windows-static-md ^
  zlib:x86-windows-static-md zlib:x86-windows-static
