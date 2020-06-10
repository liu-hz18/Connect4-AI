## 游戏简介

四子棋是一种两人对弈的游戏，任何一方先令自己的四个棋子在横、竖、斜方向上连成一条直线，即可获胜。  

## 游戏规则

基本游戏规则：

- 双方轮流落子
- 每次只能在列顶落子
- 先完成四连子（或四连子以上）者胜
- 若直到棋盘已满的时候尚未决出胜负，则为平局

但是由于四子棋的[必胜策略](http://www.informatik.uni-trier.de/~fernau/DSL0607/Masterthesis-Viergewinnt.pdf)已经于 1988 年被 Victor Allis 证明存在，所以本四子棋对战在基本游戏规则之上增添了一些扩展规则：

- 随机棋盘大小：棋盘的高度和宽度为 [9, 12] 范围内的随机数
- 随机生成不可落子点：在生成棋盘的时候，随机生成一个不可落子点；当不可落子点的下方填入棋子后，下次在该列落子时应当落在不可落子点上方

## 如何将课程代码提交到 Saiblo？

正常情况下，存放策略的 Strategy 目录应至少含有如下文件：

```plain
Strategy
├── Judge.cpp
├── Judge.h
├── Point.h
├── Strategy.cpp
└── Strategy.h
```

如果想要将文件提交到网站上，需要先点击「下载游戏包」按钮下载游戏 SDK，SDK 内的内容如下：

```plain
.
├── Makefile
└── sdk
    ├── ai_client.hpp
    ├── json
    └── main.cpp
```

选手需要将自己的策略与SDK 放在同一个文件夹下，如下是一个合理的目录结构：

```plain
.
├── Judge.cpp
├── Judge.h
├── Makefile
├── Point.h
├── Strategy.cpp
├── Strategy.h
└── sdk
```

`Makefile` 文件中已经写好了编译时 SDK 中必须要用到的文件，选手需要自行添加编译所需要的其他依赖文件和编译选项。之后将该目录打包成 `zip` 压缩包，提交至网站上。提交的时候选择提交语言类型为 `C++ with Makefile [.zip]`，若编译错误，则网站会显示编译错误信息；如果编译通过，则该 AI 可以进行对战。

### Windows 代码注意事项

Windows 代码需要额外注意两点：

首先，在 `Strategy.h` 和 `Strategy.cpp` 中，`getPoint` 和 `clearPoint` 定义如下：

```cpp
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, const int lastX, const int lastY, const int noX, const int noY);

extern "C" __declspec(dllexport) void clearPoint(Point* p);
```

在提交的时候需要去除 dll 相关内容，具体为将其改成

```cpp
Point* getPoint(const int M, const int N, const int* top, const int* _board, const int lastX, const int lastY, const int noX, const int noY);

void clearPoint(Point* p);
```

其次，由于服务器运行在 Linux 系统下，提交的代码请不要使用非 C++ 标准库，尤其是 `conio.h`、`atlstr.h` 等，否则会造成编译错误。

### 在线对战

除了排行榜和 AI 列表中的「快速人机对局」功能外。使用 Saiblo 平台的[房间系统](/rooms)也可以进行人机/机机/人人对战。

进入房间列表后，选择服务器 `monster`，点击创建房间即可创建一个房间。在任意空座位上，你可以

- 添加 AI：点击「添加 AI」，选择自己的 AI 或者使用 AI Token 加入 AI（Token 可以在我的 AI 中找到并复制，也可以在排行榜上复制其他选手的 AI Token）。
- 在线游玩：点击「加入游戏」并点击准备，即可以人类身份开始游戏。

当所有位置上的 AI 准备好后，房主点击「开始游戏」按钮即可开始游戏，游戏结束后，会自动跳转到对局结果页面。

## 大作业最终评测

AI 在运行时，限制每回合 3s 运行时间（以 CPU 时间为准）。此外，**与往年不同的是**，平台上限制峰值内存 512MB，由于**最终评测也在平台上进行**，请保证自己的程序运行时不会消耗过多内存。

为了便于同学监控 AI 的每回合的运行情况，在对局详情页面，我们提供了回合级别的的 AI 运行时间与内存监控。

此外，根据往年经验，由于 Linux 下内存访问较为严格，其余平台的代码在 Linux 下运行时容易出现运行时错误。为了避免最终评测中出现意外情况，请在平台上**充分测试后**再将作业提交。


## 关于

如果对平台有任何疑问，可以联系平台维护相关成员：曾奥涵（计86）、何广荣（计83）、饶淙元（计83）、李煜宸（计85）。