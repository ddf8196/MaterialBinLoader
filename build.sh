#!/bin/bash

# usage:
# ./build.sh
# ./build.sh -v 1.20.40.22 1.20.50.21

CLANG_OPTS_ARM64="-target aarch64-linux-android21 -shared -s -O2 -fPIC -Wno-void-pointer-to-enum-cast -DANDROID -ffunction-sections -fdata-sections -I./frida/arm64/include -L./frida/arm64/lib -lfrida-gum -llog -ldl -lm -pthread -Wl,--gc-sections,-z,noexecstack,-z,relro,-z,now"

# arm64-v8a
VERSIONS_ARM64=(
 "1.19.20.02"
 "1.19.21.01"
 "1.19.40.02" # also compatible with 1.19.41.01
 "1.19.51.01"
 "1.19.60.03"
 "1.19.62.01"
 "1.19.63.01"
 "1.19.70.22"
 "1.19.70.02"
 "1.19.71.02"
 "1.19.73.02"
 "1.19.80.02"
 "1.19.81.01"
 "1.19.83.01"
 "1.20.0.20"
 "1.20.0.01"
 "1.20.1.02"
 "1.20.10.01"
 "1.20.12.01"
 "1.20.30.20"
 "1.20.30.21"
 "1.20.15.01"
 "1.20.30.22"
 "1.20.30.25"
 "1.20.40.20"
 "1.20.30.02"
 "1.20.40.22"
 "1.20.31.01"
 "1.20.32.03"
 "1.20.40.24"
 "1.20.50.20"
 "1.20.50.21"
)

VERSIONS_ARM64=${VERSIONS_ARM64[@]}

ARG_MODE=""
for t in "$@"; do
  if [ "$t" == "-v" ]; then
    ARG_MODE="v"
    VERSIONS_ARM64=""
  elif [ "$ARG_MODE" == "v" ]; then
    VERSIONS_ARM64+="$t"
  fi
  shift
done

mkdir -p build

for v in $VERSIONS_ARM64; do
  echo "libmaterialbinloader-$v-arm64.so"
  DFLAG=${v//./_}
  $ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/clang $CLANG_OPTS_ARM64 -DV$DFLAG ./src/*.c -o ./build/libmaterialbinloader-$v-arm64.so
done
