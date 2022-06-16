#pragma once

enum PaintType {  Empty, Black, White };
enum Direction { Left, Right, Up, Down ,LeftUp,LeftDown,RightUp,RightDown};
//一个格子有多大
#define ChessBlockSide 40
//一行或者一列有多少个格子
#define ChessBlockNum 16
#define SPACE_Y 20
#define SPACE_X 20
#define TIME_SELECT 1
#define WHITECHESS 2
#define BLACKCHESS 1
#define NULLCHESS 0
//取胜的棋子数目
#define WINCOUNT 5