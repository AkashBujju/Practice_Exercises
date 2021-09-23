@echo off
cls
pushd source
echo -- Compiling
cl /EHsc /DBUILD_WITH_EASY_PROFILER /Fe"..\\bin\\app.exe" -I"..\\..\\include" ..\\..\\include\\glad.c *.cpp /link /LIBPATH:"..\\..\\libs" gdi32.lib opengl32.lib user32.lib glfw3dll.lib freetype.lib easy_profiler.lib
del *.obj
echo.
echo.
echo -- Done.
popd
