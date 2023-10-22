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
::1.19.60.03 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_60_03 ./src/*.c -o ./build/libmaterialbinloader-1.19.60.03-arm64.so
::1.19.62.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_62_01 ./src/*.c -o ./build/libmaterialbinloader-1.19.62.01-arm64.so
::1.19.63.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_63_01 ./src/*.c -o ./build/libmaterialbinloader-1.19.63.01-arm64.so
::1.19.70.22 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_70_22 ./src/*.c -o ./build/libmaterialbinloader-1.19.70.22-arm64.so
::1.19.70.02 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_70_02 ./src/*.c -o ./build/libmaterialbinloader-1.19.70.02-arm64.so
::1.19.71.02 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_71_02 ./src/*.c -o ./build/libmaterialbinloader-1.19.71.02-arm64.so
::1.19.73.02 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_73_02 ./src/*.c -o ./build/libmaterialbinloader-1.19.73.02-arm64.so
::1.19.80.02 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_80_02 ./src/*.c -o ./build/libmaterialbinloader-1.19.80.02-arm64.so
::1.19.81.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_81_01 ./src/*.c -o ./build/libmaterialbinloader-1.19.81.01-arm64.so
::1.19.83.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_19_83_01 ./src/*.c -o ./build/libmaterialbinloader-1.19.83.01-arm64.so
::1.20.0.20 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_0_20 ./src/*.c -o ./build/libmaterialbinloader-1.20.0.20-arm64.so
::1.20.0.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_0_01 ./src/*.c -o ./build/libmaterialbinloader-1.20.0.01-arm64.so
::1.20.1.02 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_1_02 ./src/*.c -o ./build/libmaterialbinloader-1.20.1.02-arm64.so
::1.20.10.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_10_01 ./src/*.c -o ./build/libmaterialbinloader-1.20.10.01-arm64.so
::1.20.12.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_12_01 ./src/*.c -o ./build/libmaterialbinloader-1.20.12.01-arm64.so
::1.20.30.20 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_30_20 ./src/*.c -o ./build/libmaterialbinloader-1.20.30.20-arm64.so
::1.20.30.21 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_30_21 ./src/*.c -o ./build/libmaterialbinloader-1.20.30.21-arm64.so
::1.20.15.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_15_01 ./src/*.c -o ./build/libmaterialbinloader-1.20.15.01-arm64.so
::1.20.30.22 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_30_22 ./src/*.c -o ./build/libmaterialbinloader-1.20.30.22-arm64.so
::1.20.30.25 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_30_25 ./src/*.c -o ./build/libmaterialbinloader-1.20.30.25-arm64.so
::1.20.40.20 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_40_20 ./src/*.c -o ./build/libmaterialbinloader-1.20.40.20-arm64.so
::1.20.30.02 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_30_02 ./src/*.c -o ./build/libmaterialbinloader-1.20.30.02-arm64.so
::1.20.40.22 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_40_22 ./src/*.c -o ./build/libmaterialbinloader-1.20.40.22-arm64.so
::1.20.31.01 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_31_01 ./src/*.c -o ./build/libmaterialbinloader-1.20.31.01-arm64.so
::1.20.32.03 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_32_03 ./src/*.c -o ./build/libmaterialbinloader-1.20.32.03-arm64.so
::1.20.40.24 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_40_24 ./src/*.c -o ./build/libmaterialbinloader-1.20.40.24-arm64.so
::1.20.50.20 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_50_20 ./src/*.c -o ./build/libmaterialbinloader-1.20.50.20-arm64.so
::1.20.50.21 arm64-v8a
clang.exe %CLANG_OPTIONS% -DV1_20_50_21 ./src/*.c -o ./build/libmaterialbinloader-1.20.50.21-arm64.so