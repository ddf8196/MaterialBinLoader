# MaterialBinLoader
让渲染龙从资源包读取.material.bin文件

## 使用
1. 从apk中解压出libminecraftpe.so
2. `patchelf --add-needed libmaterialbinloader-版本-架构.so libminecraftpe.so`
3. 将修改完的libminecraftpe.so和libmaterialbinloader-版本-架构.so打包回apk中
4. 签名，安装

## 编译
1. 安装[Android NDK](https://developer.android.google.cn/ndk/downloads/index.html)
2. `./build.bat`
