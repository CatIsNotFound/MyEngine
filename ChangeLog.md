# MyEngine ChangeLog

## v0.1.1-beta (2025/12/10)

### 🎉 新功能
- **核心引擎**: 基于SDL3的MyEngine 2D图形框架初始版本
- **渲染系统**: 基于命令的渲染器，支持批量渲染
- **窗口管理**: 多窗口支持，提供OpenGL和Vulkan后端选项
- **事件系统**: 全面的输入处理，支持键盘、鼠标和窗口事件
- **图形基元**: 支持点、线、矩形、三角形和椭圆的绘制
- **纹理系统**: 纹理加载、渲染和属性管理
- **文本渲染**: TTF字体支持，提供像素级完美的文本渲染
- **音频系统**: 支持背景音乐和音效，具备3D定位和混音功能
- **动画系统**: GIF动画支持，基于帧的播放控制
- **UI框架**: 基础UI控件，支持点击/按住区域检测
- **游戏组件**: 精灵和精灵表管理，专为2D游戏设计
- **碰撞系统**: 2D碰撞检测，支持触发器功能
- **多线程**: 多线程支持，提供线程池实现
- **工具类**: 文件系统、日志记录、随机数生成和系统内存监控

### 🔧 技术特性
- **现代C++20**: 利用最新C++特性，代码简洁高效
- **模块化架构**: 组织良好的模块系统，易于扩展
- **内存管理**: 智能指针使用和内存使用监控
- **跨平台**: 支持Windows和Linux，使用CMake构建系统
- **CMake集成**: 易于与现有CMake项目集成

### 📦 依赖项
- SDL3及其扩展（SDL_image、SDL_ttf、SDL_mixer）
- CMake 3.14+ 用于构建
- C++20兼容的编译器

### 📝 注意事项
- 这是用于测试和反馈的测试版本
- API可能在未来的版本中发生变化
- 文档正在开发中

-----

## v0.1.1-beta (2025/12/10)

### 🎉 New Features
- **Core Engine**: Initial release of MyEngine 2D graphics framework built on SDL3
- **Rendering System**: Command-based renderer with batch rendering support
- **Window Management**: Multi-window support with OpenGL and Vulkan backend options
- **Event System**: Comprehensive input handling for keyboard, mouse, and window events
- **Graphics Primitives**: Support for points, lines, rectangles, triangles, and ellipses
- **Texture System**: Texture loading, rendering, and property management
- **Text Rendering**: TTF font support with pixel-perfect text rendering
- **Audio System**: BGM and SFX support with 3D positioning and mixing
- **Animation System**: GIF animation support with frame-based playback
- **UI Framework**: Basic UI controls with click/hold area detection
- **Game Components**: Sprite and SpriteSheet management for 2D games
- **Collision System**: 2D collision detection with trigger support
- **Threading**: Multi-threading support with thread pool implementation
- **Utilities**: File system, logging, random number generation, and system memory monitoring

### 🔧 Technical Features
- **Modern C++20**: Leverages latest C++ features for clean and efficient code
- **Modular Architecture**: Well-organized module system for easy extension
- **Memory Management**: Smart pointer usage and memory usage monitoring
- **Cross-Platform**: Windows and Linux support with CMake build system
- **CMake Integration**: Easy integration with existing CMake projects

### 📦 Dependencies
- SDL3 and its extensions (SDL_image, SDL_ttf, SDL_mixer)
- CMake 3.14+ for building
- C++20 compatible compiler

### 📝 Notes
- This is a beta release for testing and feedback
- API may change in future versions
- Documentation is under development
