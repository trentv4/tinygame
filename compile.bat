@echo off
For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c-%%a-%%b)
For /f "tokens=1-2 delims=/:" %%a in ("%TIME%") do (set mytime=%%a%%b)
set filename=build_%mydate%_%mytime%
echo.
echo -------------- GCC --------------
gcc -Wall -o build/%filename% src/*.c -L ".\libs" -lglfw3dll
echo.
echo ----- %filename% -----
.\build\%filename%
echo.
echo ---------------------------------
