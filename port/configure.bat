@echo off
if %1.==. goto :help
set CC=%1 %2 %3 %4 %5 %6 %7 %8 %9
set FILE=globconf.h

cd conftest

echo // %FILE% - generated file > %FILE%


REM =================================================================
REM
REM Check for standard headers
REM
REM =================================================================

%CC% iso646.c
if errorlevel 1 echo #define NO_ISO646 >> %FILE%

%CC% stdbool.c
if errorlevel 1 echo #define NO_STDBOOL >> %FILE%

%CC% stdint.c
if errorlevel 1 echo #define NO_STDINT >> %FILE%


REM =================================================================
REM
REM Check for language features
REM
REM =================================================================

%CC% restrict.c
if errorlevel 1 echo #define NO_RESTRICT >> %FILE%

%CC% restrct1.c
if errorlevel 1 echo #define NO_UNDER_RESTRICT >> %FILE%

%CC% inline.c
if errorlevel 1 echo #define NO_INLINE >> %FILE%

%CC% inline1.c
if errorlevel 1 echo #define NO_UNDER_INLINE >> %FILE%

%CC% longlong.c
if errorlevel 1 echo #define NO_LONGLONG >> %FILE%


REM =================================================================
REM
REM Check for functions and types
REM
REM =================================================================

%CC% isblank.c
if errorlevel 1 echo #define NO_ISBLANK >> %FILE%

%CC% lldiv.c
if errorlevel 1 echo #define NO_LLDIV >> %FILE%

%CC% va_copy.c
if errorlevel 1 echo #define NO_VA_COPY >> %FILE%

%CC% strtoull.c
if errorlevel 1 echo #define NO_STRTOULL >> %FILE%

%CC% strtoll.c
if errorlevel 1 echo #define NO_STRTOLL >> %FILE%

%CC% strtold.c
if errorlevel 1 echo #define NO_STRTOLD >> %FILE%

%CC% strtof.c
if errorlevel 1 echo #define NO_STRTOF >> %FILE%

%CC% atoll.c
if errorlevel 1 echo #define NO_ATOLL >> %FILE%

%CC% llabs.c
if errorlevel 1 echo #define NO_LLABS >> %FILE%

%CC% exit1.c
if errorlevel 1 echo #define NO_UNDER_EXIT >> %FILE%


copy %FILE% ..

cd ..

set CC=
set FILE=
goto :exit

:help
echo Must provide compiler and parameters

:exit
