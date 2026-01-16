# Qt-Threaded-FileSearcher 🔍

一个基于 **Qt 6 (C++)** 的高性能**多线程文件搜索器**。
采用现代化的 **Model/View 架构**，实现了数据与界面的完全分离。支持深色主题、流式搜索、随时启停，界面丝滑流畅。

## ✨ 主要功能 (Features)

*   **多线程架构**：采用 `Worker Object` + `moveToThread` 标准模式，将繁重的文件遍历任务移至后台，确保 UI 线程永不卡顿。
*   **Model/View 设计**：放弃传统的 `QListWidget`，使用 `QTableView` + 自定义 `QAbstractTableModel`，支持百万级数据的高性能展示。
*   **实时控制**：利用 `std::atomic<bool>` 实现无锁的线程同步，支持搜索过程中的**随时暂停/停止**及状态重置。
*   **现代化 UI**：
    *   内置精美的 **Dark Theme (深色主题)**，基于 QSS 定制。
    *   支持多列信息显示（文件名、路径、大小、时间）。
*   **高效遍历**：使用 `QDirIterator` 进行流式文件遍历，低内存占用。
*   **交互闭环**：完善的按钮状态流转（灰/亮切换），支持双击调用系统默认程序打开文件。

## 🛠 技术栈 (Tech Stack)

*   **语言**：C++ 17
*   **框架**：Qt 6 (Widgets)
*   **核心技术点**：
    *   **Architecture**: Model/View Programming (`QAbstractTableModel`)
    *   **Multi-threading**: `QThread`, `moveToThread`, Worker-Object Pattern
    *   **Synchronization**: `std::atomic<bool>` (Lock-free control)
    *   **Communication**: Signals & Slots (QueuedConnection for cross-thread)
    *   **Resource**: Qt Resource System (`.qrc`) & QSS
    *   **Build System**: CMake

## 📸 运行截图 (Screenshots)
<img width="616" height="439" alt="image" src="https://github.com/user-attachments/assets/bd13f634-6a1d-45b0-b69d-971266c12b86" />

<img width="835" height="439" alt="image" src="https://github.com/user-attachments/assets/f863d76e-5933-4e0f-9f96-4d62bef1120a" />

## 🚀 如何构建 (How to Build)

1.  确保已安装 **Qt 6.x** 和 C++ 编译器 (MSVC 或 MinGW)。
2.  克隆本仓库：
    ```bash
    git clone https://github.com/yumeicheng/Qt-Threaded-FileSearcher.git
    ```
3.  使用 **Qt Creator** 打开 `CMakeLists.txt`。
4.  点击 **构建并运行 (Run)**。

---
*Created by [Yumeicheng] - 2026*
