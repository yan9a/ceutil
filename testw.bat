@REM To run the script set VCPKG_ROOT environmental variable for vcpkg root folder
cd test
if exist ./buildw/ (
rmdir /s /q ./buildw
)
cmake -B ./buildw -S ./ -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake &&^
cmake --build ./buildw --config Release 
cd buildw
ctest -N 
ctest --verbose -C Release
cd ../..
