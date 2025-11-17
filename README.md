# S3GF - SDL3 Graphics Framework

S3GF 是一个基于 SDL3（Simple DirectMedia Layer 3）的轻量级图形框架，旨在为开发者提供简洁易用的2D图形渲染接口和游戏开发工具。

## 主要功能

- 窗口管理与事件处理
- 2D图形渲染（点、线、矩形、三角形、椭圆）
- 纹理加载与绘制
- 文本渲染系统
- 音频播放功能
- 基础数学库（向量、矩阵、几何计算）
- 多线程支持

## 技术栈

- C++
- SDL3
- CMake（构建系统）

## 快速开始
```cpp
#include <S3GF/Core.h>

int main(int argc, char* argv[]) {
    // 创建引擎实例
    S3GF::Engine engine;
    
    // 创建窗口
    auto window = new S3GF::Window("S3GF Demo", 800, 600);
    engine.newWindow(window);
    
    // 运行引擎
    return engine.exec();
}
```

## 构建与安装

本项目依赖于 SDL3 库及其扩展模块（SDL3_image, SDL3_mixer, SDL3_ttf）。请确保在构建前已安装这些库。

```bash
mkdir build
cd build
cmake .. -DSDL3_LIB=/path/to/SDL3 -DSDL3_IMAGE_LIB=/path/to/SDL3_image -DSDL3_MIXER_LIB=/path/to/SDL3_mixer -DSDL3_TTF_LIB=/path/to/SDL3_ttf
make
```

## 许可证
MIT License

## 贡献
欢迎提交Issue和Pull Request！