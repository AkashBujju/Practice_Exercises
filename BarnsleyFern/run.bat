@echo off
cls
pushd bin
echo -- Running app.exe
echo.
app.exe %2 %3 %4
echo.
echo -- Exited app.exe
echo.
popd
