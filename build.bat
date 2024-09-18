@echo off

set CLANG_OPTS_ARM=--target=armv7a-linux-androideabi21 -shared -s -O2 -fPIC -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -DANDROID -ffunction-sections -fdata-sections -mthumb -I./frida/arm/include -I./libmaterial/include -L./frida/arm/lib -lfrida-gum -llog -ldl -lm -pthread  ./libmaterial/armv7/lib/libmaterialbin.a  -Wl,--fix-cortex-a8,-z,relro,-z,noexecstack,--gc-sections
set CLANG_OPTS_ARM64=--target=aarch64-linux-android21 -shared -s -O2 -fPIC -DANDROID -ffunction-sections -fdata-sections -I./libmaterial/include -I./frida/arm64/include -L./frida/arm64/lib -lfrida-gum -llog -ldl -lm -pthread  ./libmaterial/arm64/lib/libmaterialbin.a -Wall -Wl,--gc-sections,-z,noexecstack,-z,relro,-z,now

if not exist "build" md "build"

:: armeabi-v7a
echo libmaterialbinloader-arm.so
clang++.exe %CLANG_OPTS_ARM% ./src/*.cpp -o ./build/libmaterialbinloader-arm.so

:: arm64-v8a
echo libmaterialbinloader-arm64.so
clang++.exe %CLANG_OPTS_ARM64% ./src/*.cpp -o ./build/libmaterialbinloader-arm64.so

pause
