# Renderua

## v1




TGA（Truevision Graphics Adapter）文件格式是一种常见的图像文件格式，它支持各种图像数据类型和压缩方式。以下是 TGA 文件格式中常见的参数及其解释：

1. **TGA Header (文件头部)：**
   - **idlength：** 图像信息字段的长度，通常为 0。
   - **colormaptype：** 调色板类型，通常为 0 表示没有调色板。
   - **datatypecode：** 图像数据类型码，定义了图像的颜色模式和压缩方式。
   - **colormaporigin：** 调色板的起始索引。
   - **colormaplength：** 调色板的颜色数目。
   - **colormapdepth：** 调色板中每个颜色的位深度。
   - **x_origin, y_origin：** 图像的原点坐标。
   - **width, height：** 图像的宽度和高度。
   - **bitsperpixel：** 每个像素的位数，表示图像的颜色深度。
   - **imagedescriptor：** 图像描述符，包含一些标志位，用于描述图像的属性。
2. **Data Type Codes (`datatypecode`)：**
   - **0：** 无图像数据。
   - **1：** 索引颜色，无调色板。
   - **2：** 真彩色图像，无调色板。
   - **3：** 灰度图像。
   - **9：** 索引颜色，带调色板。
   - **10：** 真彩色图像，带调色板。
   - **11：** 灰度图像，带调色板。
   - **32：** 真彩色图像，Alpha 通道。
3. **Image Descriptor (`imagedescriptor`)：**
   - **Bit 3-0：** 属性位。
   - **Bit 4：** 左对齐或右对齐。
   - **Bit 5：** 上对齐或下对齐。
4. **Bits Per Pixel (`bitsperpixel`)：**
   - **8 bits：** 灰度图像。
   - **24 bits：** 真彩色图像，无 Alpha 通道。
   - **32 bits：** 真彩色图像，带 Alpha 通道。

## reference

[ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer)