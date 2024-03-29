##### 使用蒙特卡罗树搜索+信心上限树算法，实现重力四子棋的AI

## Connect4简介

重力四子棋是一种两人对弈的游戏，任何一方先令自己的四个棋子在横、竖、斜方向上连成一条直线，即可获胜。  

## 游戏规则

基本游戏规则：

- 双方轮流落子
- 每次只能在列顶落子
- 先完成四连子（或四连子以上）者胜
- 若直到棋盘已满的时候尚未决出胜负，则为平局

但是由于四子棋的[必胜策略](http://www.informatik.uni-trier.de/~fernau/DSL0607/Masterthesis-Viergewinnt.pdf)已经于 1988 年被 Victor Allis 证明存在，所以本四子棋对战在基本游戏规则之上增添了一些扩展规则：

- 随机棋盘大小：棋盘的高度和宽度为 [9, 12] 范围内的随机数
- 随机生成不可落子点：在生成棋盘的时候，随机生成一个不可落子点；当不可落子点的下方填入棋子后，下次在该列落子时应当落在不可落子点上方
