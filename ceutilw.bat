@REM To run the script set VCPKGDIR environmental variable for vcpkg root folder
if exist ./buildw/ (
rmdir /s /q ./buildw
)
cmake -B ./buildw -S ./ -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%VCPKGDIR%/scripts/buildsystems/vcpkg.cmake -D CMAKE_INSTALL_PREFIX=%VCPKGDIR%/installed/x64-windows/ &&^
cmake --build ./buildw --config Release &&^
cmake --install ./buildw --config Release
cd buildw
@REM cpack --verbose
cd ..
