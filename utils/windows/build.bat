@echo off

set configuration = Debug
set tests = OFF
set clean = false
set check = false
set buildSystem = 

:loop
    if %1 == -d set configuration=Debug
    if %1 == -r set configuration=Release
    if %1 == -t set tests=ON
    if %1 == -c set clean=true
    if %1 == --check set check=true
    shift

if not "%~1" == "" goto loop

if %check% == true (
    set buildSystem=-GNinja
    set clean=true
)

if %clean% == true (
    @rd /s /q build\%configuration%
    echo Removed %cd%\build\%configuration%!
)

mkdir build\%configuration%
cd build\%configuration%

cmake %buildSystem% -DBUILD_TESTS=%tests% -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=%configuration% -DCMAKE_INSTALL_PREFIX=./bin ../..
if %check% == false cmake --build . --target install

cd ../..
