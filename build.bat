@echo off
:: usage:
:: .\build.bat
:: .\bulid.bat -v 1.20.40.22 1.20.50.21

set CLANG_OPTS_ARM64=--target=aarch64-linux-android21 -shared -s -O2 -fPIC -Wno-void-pointer-to-enum-cast -DANDROID -ffunction-sections -fdata-sections -I./frida/arm64/include -L./frida/arm64/lib -lfrida-gum -llog -ldl -lm -pthread -Wl,--gc-sections,-z,noexecstack,-z,relro,-z,now

:: arm64-v8a
:: 1.19.40.02 also compatible with 1.19.41.01
set VERSIONS_ARM64=^
    1.19.20.02^
    1.19.21.01^
    1.19.40.02^
    1.19.51.01^
    1.19.60.03^
    1.19.62.01^
    1.19.63.01^
    1.19.70.22^
    1.19.70.02^
    1.19.71.02^
    1.19.73.02^
    1.19.80.02^
    1.19.81.01^
    1.19.83.01^
    1.20.0.20^
    1.20.0.01^
    1.20.1.02^
    1.20.10.01^
    1.20.12.01^
    1.20.30.20^
    1.20.30.21^
    1.20.15.01^
    1.20.30.22^
    1.20.30.25^
    1.20.40.20^
    1.20.30.02^
    1.20.40.22^
    1.20.31.01^
    1.20.32.03^
    1.20.40.24^
    1.20.50.20^
    1.20.50.21
:: version list end

set ARG_MODE=
:loop_args
  if "%1" == "" goto :end_args
  if "%1" == "-v" (
    set VERSIONS_ARM64=
    set ARG_MODE=%1
    goto :next_arg
  )
  if "%ARG_MODE%" == "-v" (
    set VERSIONS_ARM64=%VERSIONS_ARM64% %1
  )
:next_arg
    shift
    goto :loop_args
:end_args

if not exist "build" md "build"

setlocal enabledelayedexpansion
for %%a in (%VERSIONS_ARM64%) do (
    set DFLAG=%%a
    echo libmaterialbinloader-%%a-arm64.so
    clang.exe %CLANG_OPTS_ARM64% -DV!DFLAG:.=_! ./src/*.c -o ./build/libmaterialbinloader-%%a-arm64.so
)
