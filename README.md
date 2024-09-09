# MaterialBinLoader

A Frida module let Minecraft read the .material.bin file from the resource pack, just like how we load third-party shaders before.

> [!NOTE]
> Release 10 supports 1.21.1, 1.21.2, 1.21.20 release and 1.21.10.24 preview.

<!--maybe check latest preview support?-->

## How to use

1. Extract libminecraftpe.so from the Minecraft APK.
2. Download or manually compile the .so files from the [releases](https://github.com/ddf8196/MaterialBinLoader/releases/latest).
3. Download [patchelf](https://github.com/NixOS/patchelf/releases/latest).
4. For 32 bit (armeabi-v7a) Minecraft run the following command:
```
patchelf --add-needed libmaterialbinloader-arm.so libminecraftpe.so
```
5. For 64 bit (arm-v8a) Minecraft run the following command:
```
patchelf --add-needed libmaterialbinloader-arm64.so libminecraftpe.so
```
7. Put the modified **libminecraftpe.so** and **libmaterialbinloader.so** file back into the apk
8. Sign and install.

## How to load the shader

Put the .material.bin file into the renderer/materials/ directory of the resource pack (the same directory structure as in assets)

```
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
