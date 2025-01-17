@echo off

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua vs2022

mkdir Assets\
popd

pause