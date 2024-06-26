cmake -G "MinGW Makefiles" -B build
cd .\build
cmake ..
mingw32-make
.\InOneWeekend.exe > image.ppm
@REM code image.ppm