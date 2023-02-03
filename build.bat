set CLANG_OPTIONS=--target=aarch64-linux-android21 -shared -s -O2 -fPIC -Wno-void-pointer-to-enum-cast -DANDROID -ffunction-sections -fdata-sections -I./frida/arm64/include -L./frida/arm64/lib -lfrida-gum -llog -ldl -lm -pthread -Wl,--gc-sections,-z,noexecstack,-z,relro,-z,now

mkdir build
::1.19.20.02 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_20_02 ./src/*.c -o ./build/libmaterialbinloader-1.19.20.02-arm64.so
::1.19.21.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_21_01 ./src/*.c -o ./build/libmaterialbinloader-1.19.21.01-arm64.so 
::1.19.40.02 arm64-v8a
::also compatible with 1.19.41.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_40_02 ./src/*.c -o ./build/libmaterialbinloader-1.19.40.02-arm64.so
::1.19.51.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_51_01 ./src/*.c -o ./build/libmaterialbinloader-1.19.51.01-arm64.so
