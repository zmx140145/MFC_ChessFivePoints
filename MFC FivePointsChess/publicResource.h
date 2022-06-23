#pragma once
enum Skill{noUse,skill1,skill2,skill3,skill4};
enum PaintType {  Empty, Black, White };
enum Direction { Left, Up, RightUp, LeftUp,Right, Down, LeftDown,RightDown};
enum Station{sad,happy,think,await,welcome};
//一个格子有多大
#define ChessBlockSide 40
//一行或者一列有多少个格子
#define ChessBlockNum 16
#define SPACE_Y 20
#define SPACE_X 20
#define TIME_SELECT 1
#define CANTCHESS 3
#define WHITECHESS 2
#define BLACKCHESS 1
#define NULLCHESS 0
//取胜的棋子数目
#define WINCOUNT 5
//CD时长
#define CDTIME 3