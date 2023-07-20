# MaterialBinLoader
Let Minecraft read the .material.bin file from the resource pack

## How to use
1. Extract the libminecraftpe.so from the apk
2. Download from [Releases](https://github.com/ddf8196/MaterialBinLoader/releases/latest) or manually compile the .so file
3. Download [patchelf](https://github.com/NixOS/patchelf) and execute the command : `patchelf --add-needed libmaterialbinloader-version-architecture.so libminecraftpe.so`
4. Put the modified libminecraftpe.so and libmaterialbinloader-version-architecture.so back into the apk
5. Signing, installing

## How to load the shader
Put the .material.bin file into the renderer/materials/ directory of the resource pack (the same directory structure as in assets)

## How to compile
1. Install [Android NDK](https://developer.android.com/ndk/downloads/index.html)
2. `.\build.bat`