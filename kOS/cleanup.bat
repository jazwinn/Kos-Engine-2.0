@echo off
for %%d in (.vs x64 bin out CMakeFiles build debug release CMakeCache.txt test-results) do if exist "%%d" rmdir /Q /S "%%d"
for /d %%d in (*.dir *.vcxproj *.filters *.user) do if exist "%%d" rmdir /Q /S "%%d"
echo Cleanup done!