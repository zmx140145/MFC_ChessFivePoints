#pragma once

enum PaintType {  Empty, Black, White };
enum Direction { Left, Right, Up, Down ,LeftUp,LeftDown,RightUp,RightDown};
//һ�������ж��
#define ChessBlockSide 40
//һ�л���һ���ж��ٸ�����
#define ChessBlockNum 16
#define SPACE_Y 20
#define SPACE_X 20
#define TIME_SELECT 1
#define WHITECHESS 2
#define BLACKCHESS 1
#define NULLCHESS 0
//ȡʤ��������Ŀ
#define WINCOUNT 5