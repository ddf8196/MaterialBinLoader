#!/bin/bash

CLANG_OPTS_ARM="--target=armv7a-linux-androideabi21 -shared -s -O2 -fPIC -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -DANDROID -ffunction-sections -fdata-sections -mthumb -I./frida/arm/include -I./libmaterial/include -L./frida/arm/lib -lfrida-gum -llog -ldl -lm -pthread ./libmaterial/armv7/lib/libmaterialbin.a -Wl,--fix-cortex-a8,-z,relro,-z,noexecstack,--gc-sections"
CLANG_OPTS_ARM64="--target=aarch64-linux-android21 -shared -s -O2 -fPIC -DANDROID -ffunction-sections -fdata-sections -I./libmaterial/include -I./frida/arm64/include -L./frida/arm64/lib -lfrida-gum -llog -ldl -lm -pthread ./libmaterial/arm64/lib/libmaterialbin.a -Wall -Wl,--gc-sections,-z,noexecstack,-z,relro,-z,now"

mkdir -p build

# armeabi-v7a
$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ $CLANG_OPTS_ARM ./src/*.cpp -o ./build/libmaterialbinloader-arm.so

# arm64-v8a
$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ $CLANG_OPTS_ARM64 ./src/*.cpp -o ./build/libmaterialbinloader-arm64.so
