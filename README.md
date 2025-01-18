# computer-graphics-from-scratch
《计算机图形学入门：3D渲染指南》中针对光线追踪和一部分的光栅化内容自己写的C++代码。

---

**《计算机图形学入门：3D渲染指南》**，加布里埃尔·甘贝塔（Gabriel Gambetta）著，书的条目见[豆瓣](https://book.douban.com/subject/35894599/?_dtcc=1)。

书主要讲了光线追踪和光栅化两部分的内容。本仓库中的代码是光线追踪的实现，代码用C++写成，只使用了`stb_image_write.h`这个外部头文件，其余.cpp文件和.h文件都是我自己编写的，也没有使用OpenGL、OpenCV等库。

文件结构：

```
章节名（如'2.5_first_spheres'）/
├── bin/
│   ├── Debug/
│   │   ├── .exe文件等
│   └── intermidiates（中间产生的文件）/
├── src/
│   ├── .cpp文件
├── 生成的图片.png文件
├── Visual Studio项目产生的文件
include/
├── show_image.h（自己写的头文件，主要包括`Vec3`类、创建图片的函数等等）
└── stb_image_write.h（网上知名的图片读写库）
ComputerGraphics.sln（Visual Studio解决方案文件）
```
