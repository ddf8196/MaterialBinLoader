# MaterialBinLoader

A Frida module let Minecraft read the .material.bin file from the resource pack, just like how we load third-party shaders before.

## How to use

1. Extract libminecraftpe.so from the Minecraft APK.
2. Download or manually compile the .so files from the [Releases](https://github.com/ddf8196/MaterialBinLoader/releases/latest).
3. Download [patchelf](https://github.com/NixOS/patchelf/releases/latest).
4. For 32 bit Minecraft run the following command: `patchelf --add-needed libmaterialbinloader-arm.so libminecraftpe.so`. For 64 bit Minecraft run the following command: `patchelf --add-needed libmaterialbinloader-arm64.so libminecraftpe.so`
5. Put the modified **libminecraftpe.so** and **libmaterialbinloader.so** file back into the apk
6. Sign and install.

## How to load the shader

Put the .material.bin file into the renderer/materials/ directory of the resource pack (the same directory structure as in assets)

```json
     put your shader name here
         |-renderer
              |-materials
                   |-Put the shader .material.bin files at here
         |-manifest.json
         |-textures(if you want add)
         |-pack_icon.png
```

## How to compile

### Windows

1. Install [Android NDK](https://developer.android.com/ndk/downloads/index.html) and add `NDKfolder`\toolchains\llvm\prebuilt\windows-x86_64\bin to your PC's environment variables.
2. Run `.\build.bat` file and wait it build.
3. Find the compiled .so file in the `build` folder.

### Linux

1. Install [Android NDK](https://developer.android.com/ndk/downloads/index.html) and set `NDKfolder` as ANDROID_NDK_HOME
2. Run `./build.sh` file and wait it build.
3. Find the compiled .so file in the `build` folder.

## Note

**This Program is not affiliated with Mojang Studios**.
