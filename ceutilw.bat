@REM To run the script set VCPKG_ROOT environmental variable for vcpkg root folder

xcopy ".\include" "%VCPKG_ROOT%\installed\x64-windows\include" /E /H /C /I /Y
