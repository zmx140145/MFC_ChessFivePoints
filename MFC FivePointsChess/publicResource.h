#pragma once
enum Skill{noUse,skill1,skill2,skill3,skill4};
enum PaintType {  Empty, Black, White };
enum Direction { Left, Up, RightUp, LeftUp,Right, Down, LeftDown,RightDown};
enum Station{sad,happy,think,await,welcome};
//һ�������ж��
#define ChessBlockSide 40
//һ�л���һ���ж��ٸ�����
#define ChessBlockNum 16
#define SPACE_Y 20
#define SPACE_X 20
#define TIME_SELECT 1
#define CANTCHESS 3
#define WHITECHESS 2
#define BLACKCHESS 1
#define NULLCHESS 0
//ȡʤ��������Ŀ
#define WINCOUNT 5
//CDʱ��
#define CDTIME 3