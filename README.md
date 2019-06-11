# ImgCompressSDK-v1.0.0

## 1、简述
ImageCompressSdk是基于libjpeg-turbo实现的图片压缩工具集合，支持Bytes，File
，Bitmap，Stream，Resource，URI类型的数据源，并提供同步和异步处理单张图片或
批量图片压缩。
## 2、libjpeg-turbo
[libjpeg-turbo](https://libjpeg-turbo.org/)是一种JPEG图像编解码器，
它使用SIMD指令（MMX，SSE2，AVX2，NEON，AltiVec）加速x86，x86-64，ARM和
PowerPC系统上的基准JPEG压缩和解压缩。在这样的系统上， libjpeg - turbo的速度
通常是 libjpeg的 2-6倍，而其他的都是相等的。在其他类型的系统上，借助其高度优
化的Huffman编码例程，libjpeg-turbo仍然可以在性能上超过libjpeg。在许多情况下
，libjpeg - turbo的性能 与专有的高速JPEG编解码器相媲美。
## 3、基本使用
### 3.1、初始化
Reduce.with(Context);
### 3.2、加载图片
Reduce.with(Context).source(Bitmap);
### 3.3、图片压缩
Reduce.with(Context).source(Bitmap).compressSync();
## 4、异步压缩
Reduce.with(Context).source(Bitmap).compress(new Callback<>());
## 5、批量压缩
Reduce.with(Context).source(Bitmaps).compress();