
// MFC FivePointsChess.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include"publicResource.h"
#include "resource.h"		// 主符号
#include <vector>
// CMFCFivePointsChessApp:
// 有关此类的实现，请参阅 MFC FivePointsChess.cpp
//
//这是下棋的操作记录
struct Record
{
	int x;
    int y;
	PaintType type;
	Record* next;
};
struct PointPos
{
	int x;
	int y;
};
class CMFCFivePointsChessApp : public CWinApp
{


public:
	std::vector<PointPos> Total_BestPoints;
	std::vector<PointPos> BestPoints;
	std::vector<PointPos> Opp_BestPoints;
	CPoint point; 
	float time;
	//历史记录链表的头指针 指向最后一次记录
	Record* pRecord;
	//临时的记录棋子的数组
	int ChessTag[ChessBlockNum - 1][ChessBlockNum - 1]; //0—无子,1—黑子,2—白子
	//记录每个格子分数的数值 用于AI判断
	int ChessScoreResult[ChessBlockNum - 1][ChessBlockNum - 1];
	int ChessScoreUD[ChessBlockNum - 1][ChessBlockNum - 1];//初始分为0分 越高分越优先考虑‘

	int ChessScoreUR[ChessBlockNum - 1][ChessBlockNum - 1];
	int ChessScoreUL[ChessBlockNum - 1][ChessBlockNum - 1];
	int ChessScoreLR[ChessBlockNum - 1][ChessBlockNum - 1];
	int Opp_ChessScoreResult[ChessBlockNum - 1][ChessBlockNum - 1];
	int Opp_ChessScoreUD[ChessBlockNum - 1][ChessBlockNum - 1];//初始分为0分 越高分越优先考虑
	int Opp_ChessScoreUR[ChessBlockNum - 1][ChessBlockNum - 1];
	int Opp_ChessScoreUL[ChessBlockNum - 1][ChessBlockNum - 1];
	int Opp_ChessScoreLR[ChessBlockNum - 1][ChessBlockNum - 1];
	int  m_nFlag;  //当前该哪方下子。1—黑,2—白,0—禁止下子
	void init();
	int GameWiner;//0是没有人胜利 //1是黑方赢了 //2是白方赢了
    //用下标查看
	int GetChess(int x, int y); //0 //1黑 //2白
	void CreateRecord(int x, int y, PaintType type);
	PaintType DeleteAndLoadLastRecord(int * x, int * y);
	//用坐标添加 //下子
	bool TryAddChess(int PosX, int PosY, PaintType type);
	void freeRecord();
	 //查看该谁下子
	int  GetFlag (); //1是黑色 //2是白色
	bool ChangeFlag(int x);//切换下棋方
	  //胜负判断 
	int  JudgeWin(int x,int y);  //0是没有赢  //1是黑色赢  //2是白色赢//输入坐标    
	void InitAllScoreArray();
	void InitResultScoreArray();
	void InitScoreArray();
	//寻找最优的下棋位置 如果找不到那么就说明可以认输了
	bool AI_FindBestPoint(int*x, int*y);
	unsigned randInt(int min, int max);
	int JudgeFromChessCount();
	void JudgeFromChessCount2();
	void AddResultInResult();
	void AddEmptyPointsNearChessFromPointAllDirection(int x, int y);
	void AddEmptyPointsFromPoint(int * x, int * y, Direction direction, int Depth);
	void SecondAssessResult();
	bool FirstAssessResult();
	void SixthAssessResult();
	void SetScoreArrayZero();
	void FifthAssessResult();
	void ThirdAssessResult();
	void FourthAssessResult();
	void AddToScore(const int & x, const int & y, const int & TargetCount, const Direction & direction, const int & count, const int & count1, const int & Choose, bool myself);
	int SearchOurPointsCount(int * x, int * y, int * TargetTag, Direction direction, int depth);
	int FindOpp_PointOfDirectionPoints2(int * x, int * y, int * TargetTag, int * MyTag, Direction direction, int depth);
	int FindOpp_PointOfDirectionPoints2_2(int * x, int * y, int * TargetTag, int * MyTag, bool Find, Direction direction, int depth);
	int FindOpp_PointOfDirectionPoints3(int * x, int * y, int * TargetTag, Direction direction, int depth, bool Find);
	int FindOpp_PointOfDirectionPoints(int * x, int * y, int * MyTag, Direction direction, int depth, bool Find);
	int JudgeChessCanWin2(int * x, int * y, int * tag, int * flag, Direction direction, int Depth);
	int JudgeChessCanWin(int * x, int * y, int * tag, Direction direction, int Depth, int * FindDepth);
	int CalCulateFromChessCountForSomeDetail(int * OurChessCount, int * FindDepth, int * x, int * y, int tempEmptyCount, bool FindEmptyLastRound, bool FindEmpty, const int & MyTag, const Direction & direction, bool FindMe, int Depth);



	

	




	

	

	


	

	



	

	



	int JudgeChessDirection2(int * x, int * y, int * tag, Direction direction, int Depth);

	int JudgeChessDirection(int * x, int * y, int * tag, Direction direction, int Depth);



	
	int JudgeWinDirection(int * x, int * y, int * tag, Direction direction, int Depth);

	PaintType Regret();

	bool CalculatePointDiretion(int * x, int * y, Direction direction);

	CMFCFivePointsChessApp();
	~CMFCFivePointsChessApp();
	
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSave();
	afx_msg void OnRead();
};

extern CMFCFivePointsChessApp theApp;


