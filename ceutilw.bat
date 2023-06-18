@REM To run the script set VCPKGDIR environmental variable for vcpkg root folder
if exist buildw (
echo deleting
rmdir /s /q buildw
)
cmake -G "Visual Studio 17 2022" -B ./buildw -S ./ -DCMAKE_TOOLCHAIN_FILE=%VCPKGDIR%/scripts/buildsystems/vcpkg.cmake -DCMAKE_INSTALL_PREFIX=%VCPKGDIR%/installed/x64-windows/ -DCMAKE_DEBUG_POSTFIX=d
cd buildw
cmake --build . --config Release
cmake --install . --config Release
cmake --build . --config Debug
cmake --install . --config Debug

cd ..
