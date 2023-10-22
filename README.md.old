简体中文 | [English](README_US.md) 

# MaterialBinLoader
让渲染龙从资源包读取.material.bin文件

## 使用
1. 从apk中解压出libminecraftpe.so
2. 从[Releases](https://github.com/ddf8196/MaterialBinLoader/releases/latest)中下载或手动编译对应版本和架构的so
3. `patchelf --add-needed libmaterialbinloader-版本-架构.so libminecraftpe.so`
4. 将修改完的libminecraftpe.so和libmaterialbinloader-版本-架构.so打包回apk中
5. 签名，安装

## 文件位置
将.material.bin文件放入资源包的renderer/materials/目录中即可（与assets中目录结构相同）

## 编译
1. 安装[Android NDK](https://developer.android.google.cn/ndk/downloads/index.html)
2. `./build.bat`
