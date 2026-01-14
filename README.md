# Qt-Threaded-FileSearcher 🔍

一个基于 Qt/C++ 的高性能**多线程文件搜索器**。
实现了 UI 与逻辑分离，支持随时开始、停止、重置搜索，搜索过程界面丝滑流畅。

## ✨ 主要功能 (Features)

*   **多线程架构**：采用 `Worker Object` + `moveToThread` 模式，确保 UI 线程永不卡顿。
*   **实时控制**：支持搜索过程中的**随时暂停/停止** (基于 `std::atomic` 原子标志位)。
*   **高效遍历**：使用 `QDirIterator` 进行流式文件遍历，低内存占用。
*   **交互闭环**：完善的按钮状态流转（灰/亮切换），防止误操作。
*   **快速打开**：双击列表项即可调用系统默认程序打开目标文件。

## 🛠 技术栈 (Tech Stack)

*   **语言**：C++ 17
*   **框架**：Qt 6 (Widgets)
*   **核心技术点**：
    *   Multi-threading (`QThread`)
    *   Thread Synchronization (`std::atomic<bool>`)
    *   Signals & Slots (QueuedConnection)
    *   Memory Management (Parent-Child Ownership)
    *   Custom C++ UI Layout (No .ui files)

## 📸 运行截图 (Screenshots)

<!-- 你以后可以在这里放一张截图，现在先留空 -->
> *待补充：运行界面截图*

## 🚀 如何构建 (How to Build)

1.  确保已安装 Qt 6.x 和 C++ 编译器 (MSVC/MinGW)。
2.  使用 Qt Creator 打开 `CMakeLists.txt` 或 `.pro` 文件。
3.  点击构建并运行。

---
*Created by [Yumeicheng] - 2026*
