@echo off

echo "Clean up the environment ..."
IF EXIST "Src\external_dependencies\vcpkg" (
	echo Deleting "Src\external_dependencies\vcpkg" ...
	rmdir /S /Q "Src\external_dependencies\vcpkg"
	echo "Src\external_dependencies\vcpkg was deleted!"
)

IF EXIST "%AppData%\..\local\vcpkg" (
	echo Deleting "%AppData%\..\local\vcpkg" ...
	rmdir /S /Q "%AppData%\..\local\vcpkg"
	echo "%AppData%\..\local\vcpkg was deleted!"
)

IF EXIST ".\vcpkg" (
	echo Deleting ".\vcpkg" ...
	rmdir /S /Q ".\vcpkg"
	echo ".\vcpkg was deleted!"
)

echo "Uncompress \Src\external_dependencies\CEF ..."
.\BuildTools\7-ZipPortable_22.01\App\7-Zip\7z.exe x .\Src\external_dependencies\CEF.7z.001 -y -o.\Src\external_dependencies


IF NOT EXIST .\vcpkg (
	echo First time setup. Downloading vcpkg
	git clone https://github.com/microsoft/vcpkg.git
	.\vcpkg\bootstrap-vcpkg.bat -disableMetrics
	.\vcpkg\vcpkg.exe integrate install
	.\vcpkg\vcpkg.exe integrate project
	
	echo Patching ports...
	xcopy /K /Y /H /C /I /E .\vcpkg-ports\* .\vcpkg\ports\*

	echo Installing packages...
	cd .\vcpkg
	.\vcpkg install alac:x86-windows-static-md
	.\vcpkg install expat:x86-windows-static-md expat:x86-windows-static
	.\vcpkg install freetype:x86-windows-static-md
	.\vcpkg install ijg-libjpeg:x86-windows-static-md
	.\vcpkg install libflac:x86-windows-static-md
	.\vcpkg install libogg:x86-windows-static-md
	.\vcpkg install libpng:x86-windows-static-md
	.\vcpkg install libsndfile:x86-windows-static-md
	.\vcpkg install libtheora:x86-windows-static-md
	.\vcpkg install libvorbis:x86-windows-static-md
	.\vcpkg install libvpx:x86-windows-static-md
	.\vcpkg install minizip:x86-windows-static-md
	.\vcpkg install mp3lame:x86-windows-static-md
	.\vcpkg install mpg123:x86-windows-static-md
	.\vcpkg install openssl:x86-windows-static-md openssl:x86-windows-static
	.\vcpkg install pthread:x86-windows-static-md pthread:x86-windows-static
	.\vcpkg install restclient-cpp:x86-windows-static-md restclient-cpp:x86-windows-static
	.\vcpkg install spdlog:x86-windows-static-md
	.\vcpkg install zlib:x86-windows-static-md zlib:x86-windows-static
	
	pause
) ELSE (
	echo vcpkg is available. Updating.
	cd .\vcpkg
	git pull
	.\bootstrap-vcpkg.bat -disableMetrics
	
	echo Patching ports...
	xcopy /K /Y /H /C /I /E ..\vcpkg-ports\* ..\vcpkg\ports\*
	
	pause
)
