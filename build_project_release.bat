REM A mini-script to build game binaries and pack the data into a single directory
@echo off
rmdir build_game
mkdir build_game
REM mkdir build
REM cd build
REM cmake .. -DCMAKE_BUILD_TYPE=Release
REM cmake --build . --config Release --parallel=12
REM cd ..
Build\bin\Release\PackageTool Build\bin\CoreData build_game\CoreData.pak -c;
Build\bin\Release\PackageTool Build\bin\Data build_game\Data.pak -c;

copy Build\bin\Release\MyGame.exe build_game\MyGame.exe
copy Build\bin\Release\MyGamePlugin.dll build_game\MyGamePlugin.dll
copy Build\bin\Release\Urho3D.dll build_game\Urho3D.dll

pause
