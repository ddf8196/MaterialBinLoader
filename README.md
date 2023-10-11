# MaterialBinLoader
Let Minecraft read the .material.bin file from the resource pack

## How to use
1. Extract the libminecraftpe.so from the apk
2. Download from [Releases](https://github.com/ENDERMANYK/MaterialBinLoader/releases/latest) or manually compile the .so file
3. Download [patchelf](https://github.com/NixOS/patchelf/releases/latest) and execute the command : `patchelf --add-needed libmaterialbinloader-version-arm64.so libminecraftpe.so`
4. Put the modified **libminecraftpe.so** and **libmaterialbinloader-version-arm64.so** back into the apk
5. Signing, installing

## How to load the shader
Put the .material.bin file into the renderer/materials/ directory of the resource pack (the same directory structure as in assets)

```
     yourshadername
         |-renderer
              |-materials
                   |-Put the shader .material.bin files at here
         |-manifest.json
         |-textures(if you want add)
         |-pack_icon_png
```

## How to compile
#### Windows:
1. Install [Android NDK](https://developer.android.com/ndk/downloads/index.html) and add `NDKfolder`\toolchains\llvm\prebuilt\windows-x86_64\bin to your PC's environment variables.
2. Run `.\build.bat` file and wait it build.
3. Find the compiled .so file in the `build` folder.
#### Linux:
1. Install [Android NDK](https://developer.android.com/ndk/downloads/index.html) and set `NDKfolder` as ANDROID_NDK_HOME
2. Run `./build.sh` file and wait it build.
3. Find the compiled .so file in the `build` folder.

## Note
**This Program is not affiliated with Mojang Studios**.