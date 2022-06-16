
// MFC FivePointsChess.cpp: 定义应用程序的类行为。
//

#include <ctime>
#include "pch.h"
#include "framework.h"
#include "MFC FivePointsChess.h"
#include "MFC FivePointsChessDlg.h"
#include <locale>
#include<vector>
#include<sstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCFivePointsChessApp

BEGIN_MESSAGE_MAP(CMFCFivePointsChessApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_COMMAND(ID_32771, &CMFCFivePointsChessApp::OnSave)
	ON_COMMAND(ID_32772, &CMFCFivePointsChessApp::OnRead)
END_MESSAGE_MAP()


// CMFCFivePointsChessApp 构造



CMFCFivePointsChessApp::CMFCFivePointsChessApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	init();
	// 将所有重要的初始化放置在 InitInstance 中

}

CMFCFivePointsChessApp::~CMFCFivePointsChessApp()
{
	freeRecord();
}

void CMFCFivePointsChessApp::init()
{
	//对chesstag初始化
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			ChessTag[i][j] = 0;
		}
	}
	m_nFlag = 1;
	GameWiner = 0;
	pRecord = NULL;
}
//获得棋盘位置的信息
int CMFCFivePointsChessApp::GetChess(int x, int y)
{
	return ChessTag[x][y];
}
//存储历史记录
void CMFCFivePointsChessApp::CreateRecord(int x, int y, PaintType type)
{
	pRecord = new Record{x,y,type,pRecord};
}
//尝试删除并读取记录
//读取失败返回Empty
//成功就返回相应棋子的类型 黑还是白
PaintType CMFCFivePointsChessApp::DeleteAndLoadLastRecord(int*x, int*y)
{
	if (!pRecord)
	{
		return Empty;
	}
	//读取记录
	*x=pRecord->x;
	*y = pRecord->y;
	PaintType type = pRecord->type;
	Record* pPre=pRecord;
	//再指向前一条记录
	pRecord = pRecord->next;
	//释放已经读取历史记录
	free(pPre);
	return type;
}
//尝试增加棋子
bool CMFCFivePointsChessApp::TryAddChess(int PosX, int PosY, PaintType type)
{
	//下棋
	int i = PosX / ChessBlockSide - 1;
	int j = PosY / ChessBlockSide - 1;
	switch (type)
	{
	case Black:
	{
		if (GetChess(i, j)==NULLCHESS)
		{
			//把棋盘记录更改
			ChessTag[i][j] = BLACKCHESS;
			//生成历史记录
			CreateRecord(i, j, Black);
			break;
		}
		
		return false;
	}
	
	case Empty:
		return false;
	case White:
	{
		if (GetChess(i, j) == NULLCHESS)
		{
			ChessTag[i][j] = WHITECHESS;
			CreateRecord(i, j, White);
			break;
		}

		return false;
	}
		
	default:
		break;
	}
	switch (GameWiner=JudgeWin(i, j))
	{
	case NULLCHESS:
	{
		//没有人赢
		return true;
	}
	case BLACKCHESS:
	{
		//黑棋赢了
		//停止输入
		ChangeFlag(NULLCHESS);
		break;
	}
	case WHITECHESS:
	{
		//白棋赢了
		//停止输入
		ChangeFlag(NULLCHESS);
		break;
	}
	default:
	{
		break;
	}
	}
	CString str;
	str = "游戏结束！》》按空格";
	CMFCFivePointsChessDlg* pWnd = (CMFCFivePointsChessDlg*)AfxGetApp()->GetMainWnd();
	pWnd->MessageEdit.SetWindowTextW(str);
	return false;
}
void CMFCFivePointsChessApp::freeRecord()
{
	Record* pPre;
	while (pRecord)
	{
		pPre = pRecord;
		pRecord = pRecord->next;
		free(pPre);
	}
}
int CMFCFivePointsChessApp::GetFlag()
{
	return m_nFlag;
}

bool CMFCFivePointsChessApp::ChangeFlag(int x)
{
	if (x >= 0)
	{
		if (x <= 2)
		{
			m_nFlag = x;
			return true;
		}
	}
	
	return false;
}

int CMFCFivePointsChessApp::JudgeWin(int x, int y)
{
	int tag= GetChess(x, y);
	if (tag == 0)
	{
		return 0;
	}
	//判断4个方向 
	//左右方向
	if (JudgeWinDirection(&x, &y, &tag, Left, WINCOUNT) + JudgeWinDirection(&x, &y, &tag, Right, WINCOUNT) - 1 == WINCOUNT)
	{
		return tag;
	}
	//上下方向
	if (JudgeWinDirection(&x, &y, &tag, Up, WINCOUNT) + JudgeWinDirection(&x, &y, &tag, Down, WINCOUNT) - 1 == WINCOUNT)
	{
		return tag;
	}
	//左上右下
	if (JudgeWinDirection(&x, &y, &tag, LeftUp, WINCOUNT) + JudgeWinDirection(&x, &y, &tag, RightDown, WINCOUNT) - 1 == WINCOUNT)
	{
		return tag;
	}
	if (JudgeWinDirection(&x, &y, &tag, LeftDown, WINCOUNT) + JudgeWinDirection(&x, &y, &tag, RightUp, WINCOUNT) - 1 == WINCOUNT)
	{
		return tag;
	}
	return 0;
}
void CMFCFivePointsChessApp::InitAllScoreArray()
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			ChessScoreResult[i][j] = 0;
			ChessScoreUD[i][j] = 0;
			ChessScoreUR[i][j] = 0;
			ChessScoreUL[i][j] = 0;
			ChessScoreLR[i][j] = 0;
			Opp_ChessScoreResult[i][j] = 0;
			Opp_ChessScoreUD[i][j] = 0;
			Opp_ChessScoreUR[i][j] = 0;
			Opp_ChessScoreUL[i][j] = 0;
			Opp_ChessScoreLR[i][j] = 0;
		}
	}
	
}
void CMFCFivePointsChessApp::InitResultScoreArray()
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			ChessScoreResult[i][j] = 0;
			Opp_ChessScoreResult[i][j] = 0;
			
		}
	}

}
//不会清空总分的记录
void CMFCFivePointsChessApp::InitScoreArray()
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			
			ChessScoreUD[i][j] = 0;
			ChessScoreUR[i][j] = 0;
			ChessScoreUL[i][j] = 0;
			ChessScoreLR[i][j] = 0;
			
			Opp_ChessScoreUD[i][j] = 0;
			Opp_ChessScoreUR[i][j] = 0;
			Opp_ChessScoreUL[i][j] = 0;
			Opp_ChessScoreLR[i][j] = 0;
		}
	}

}
bool CMFCFivePointsChessApp::AI_FindBestPoint(int * x, int * y)
{
	//先初始化分数 设置为0
	InitAllScoreArray();
	BestPoints.clear();
	Opp_BestPoints.clear();
	//计算分数
	/*----------------第一轮筛选---------------*/
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			//遍历所有的点 找出在下棋点附近的所有空位
			//把空位点在ChessScore 里设置为1  这样可以避免重复
			AddEmptyPointsNearChessFromPointAllDirection(i, j);
		}
	}
	//把空的点位不重复加到bestPoint
	bool havePoint=false;
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			if (ChessScoreResult[i][j] == 1)
			{
				havePoint = true;
				PointPos* p = new PointPos{ i,j };
				BestPoints.push_back(*p);
			}
		}
	}
	if (!havePoint)
	{
		if (ChessTag[7][7] == NULLCHESS)
		{
			PointPos* p = new PointPos{ 7,7};
			BestPoints.push_back(*p);
		}
		else
		{
			return false;
		}
	}
	//初始化分数记录的数组们
	InitAllScoreArray();
	//剔除掉那些在边界的点 没有可能能够形成五子的地方
	FirstAssessResult(x, y);
	//不用清空分数 在2时要用到
	//剔除掉那些中间的空格太多的点 就是在一个深度上空格很多的
	SecondAssessResult(x, y);
	CalculateEmptyPointContainCount();
	//两轮筛选之后的到的点 符合以下条件：
	//1.首先有机会形成五个子 无论是敌方还是自己
	//2.留下了最多子或者第二多子的 进攻 以及 防守的空位点 前提要大于0就是有子
	JudgeFromChessCount();
	/*----------------第三轮筛选---------------*/
	//计算每个点的得分情况 并根据得分把最佳点输出到BestPoints
	
	/*----------------第四轮筛选---------------*/

	ReAssessResult(x, y);
	ReReAssessResult(x, y);
	int p;
	if (BestPoints.size()-1 >0)
	{
		p = randInt(0, BestPoints.size() - 1);
	}
	else
	{
		p = 0;
	}

	*x=BestPoints.at(p).x;
	*y = BestPoints.at(p).y;
	return true;
}
unsigned CMFCFivePointsChessApp::randInt(int min, int max)
{
	long t = GetTickCount();
	srand((int)t);
	return (rand() % (max - min + 1)) + min;
}

void CMFCFivePointsChessApp::AddResultInResult()
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			ChessScoreResult[i][j]=ChessScoreUD[i][j] +ChessScoreUR[i][j]+ ChessScoreUL[i][j]+ChessScoreLR[i][j];
			Opp_ChessScoreResult[i][j] = Opp_ChessScoreUD[i][j] + Opp_ChessScoreUR[i][j] + Opp_ChessScoreUL[i][j] + Opp_ChessScoreLR[i][j];
		}
	}
}
void CMFCFivePointsChessApp::AddEmptyPointsNearChessFromPointAllDirection(int x, int y)
{
	int tag = GetChess(x, y);
	if (tag == NULLCHESS)
	{
		return;
	}

	//判断8个方向 
	AddEmptyPointsFromPoint(&x, &y, Left, 3);
	AddEmptyPointsFromPoint(&x, &y, Right, 3);
	AddEmptyPointsFromPoint(&x, &y, RightUp, 3);
	AddEmptyPointsFromPoint(&x, &y, RightDown, 3);
	AddEmptyPointsFromPoint(&x, &y, LeftUp,3);
	AddEmptyPointsFromPoint(&x, &y, LeftDown, 3);
	AddEmptyPointsFromPoint(&x, &y,Up, 3);
	AddEmptyPointsFromPoint(&x, &y, Down, 3);
	
	return ;
}
void CMFCFivePointsChessApp::AddEmptyPointsFromPoint(int* x, int* y, Direction direction, int Depth)
{
	int x1 = *x;
	int y1 = *y;
	if (ChessTag[x1][y1] == NULLCHESS)
	{
		ChessScoreResult[x1][y1] = 1;
		
	}
	if (Depth <= 1)
	{
		return;
	}
	if (CalculatePointDiretion(&x1, &y1, direction))
	{
	 AddEmptyPointsFromPoint(&x1, &y1, direction, --Depth);
	}
	else
	{
		return;
	}

}
//把不可能下到棋的方向剔除 因为最大分数是8  那么小于4的都设置为-5 其他设置为0
void CMFCFivePointsChessApp::SecondAssessResult(int* x, int* y)
{
	
	for each (PointPos it in BestPoints)
	{
		if (ChessScoreLR[it.x][it.y] < WINCOUNT-1)
		{
			ChessScoreLR[it.x][it.y] = -5;
		}
		else
		{
			ChessScoreLR[it.x][it.y] = 0;
		}
		if (ChessScoreUR[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUR[it.x][it.y] = -5;
		}
		else
		{
			ChessScoreUR[it.x][it.y] = 0;
		}
		if (ChessScoreUL[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUL[it.x][it.y] = -5;
		}
		else
		{
			ChessScoreUL[it.x][it.y] = 0;
		}
		if (ChessScoreUD[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUD[it.x][it.y] = -5;
		}
		else
		{
			ChessScoreUD[it.x][it.y] = 0;
		}
	}
	for each (PointPos it in Opp_BestPoints)
	{
		if (Opp_ChessScoreLR[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreLR[it.x][it.y] = -5;
		}
		else
		{
			Opp_ChessScoreLR[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUR[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUR[it.x][it.y] = -5;
		}
		else
		{
			Opp_ChessScoreUR[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUL[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUL[it.x][it.y] = -5;
		}
		else
		{
			Opp_ChessScoreUL[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUD[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUD[it.x][it.y] = -5;
		}
		else
		{
			Opp_ChessScoreUD[it.x][it.y] = 0;
		}
	}
	std::vector<PointPos> temp3;
	std::vector<PointPos> temp4;
	/*--------------求差集----------------*/
	//记录在我方而没有记录在敌方的
	for each (PointPos it in BestPoints)
	{
		bool flag = false;
		for each (PointPos dit in Opp_BestPoints)
		{
			if (it.x == dit.x&&it.y == dit.y)
			{
				flag = true;
			}
		}
		if (!flag)
		{
			temp3.push_back(it);
		}
	}
	//记录在敌方方而没有记录在我方的
	for each (PointPos it in Opp_BestPoints)
	{
		bool flag = false;
		for each (PointPos dit in BestPoints)
		{
			if (it.x == dit.x&&it.y == dit.y)
			{
				flag = true;
			}
		}
		if (!flag)
		{
			temp4.push_back(it);
		}
	}

	/*-------------求差集结束-------------*/
	for each (PointPos it in temp3)
	{
		if (Opp_ChessScoreLR[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreLR[it.x][it.y] = -5;
		}
		else
		{
			Opp_ChessScoreLR[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUR[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUR[it.x][it.y] = -5;
		}
		else
		{
			Opp_ChessScoreUR[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUL[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUL[it.x][it.y] = -5;
		}
		else
		{
			Opp_ChessScoreUL[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUD[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUD[it.x][it.y] = -5;
		}
		else
		{
			Opp_ChessScoreUD[it.x][it.y] = 0;
		}
	}
	for each (PointPos it in temp4)
	{
		if (ChessScoreLR[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreLR[it.x][it.y] = -5;
		}
		else
		{
			ChessScoreLR[it.x][it.y] = 0;
		}
		if (ChessScoreUR[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUR[it.x][it.y] = -5;
		}
		else
		{
			ChessScoreUR[it.x][it.y] = 0;
		}
		if (ChessScoreUL[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUL[it.x][it.y] = -5;
		}
		else
		{
			ChessScoreUL[it.x][it.y] = 0;
		}
		if (ChessScoreUD[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUD[it.x][it.y] = -5;
		}
		else
		{
			ChessScoreUD[it.x][it.y] = 0;
		}
	}
	temp3.clear();
	temp4.clear();
}
//初始筛选 把不可能五个子的位置剔除
void CMFCFivePointsChessApp::FirstAssessResult(int* x, int* y)
{
	std::vector<PointPos> temp;
	std::vector<PointPos> temp2;
	int tag = GetFlag();
	int tag1 = GetFlag();
	int depth = 0;
	int depth1 = 0;
	/*-------------------对于每一个空位点进行操作-----------------------*/
	for each (PointPos it in BestPoints)
	{

		//判断4个方向 
		//左右方向
		depth = 0;
		tag = NULLCHESS;
		int Count = JudgeChessCanWin(&it.x, &it.y, &tag, Right, WINCOUNT, &depth);

		if (tag == GetFlag())
		{
			//如果direction方向的第一个棋子信息是本方的
			//那么把分数加到本方的积分板上
			ChessScoreLR[it.x][it.y] += Count;
			Opp_ChessScoreLR[it.x][it.y] += depth;
		}
		else
		{
			if (tag != NULLCHESS)
			{
				//如果direction方向的第一个棋子信息是对方的
			//那么把分数加到对方的积分板上
				Opp_ChessScoreLR[it.x][it.y] += Count;
				ChessScoreLR[it.x][it.y] += depth;
			}
		}
		depth1 = 0;
		tag1 = NULLCHESS;
		int Count1 = JudgeChessCanWin(&it.x, &it.y, &tag1, Left, WINCOUNT, &depth1);
		if (tag1 == GetFlag())
		{
			ChessScoreLR[it.x][it.y] += Count1;
			Opp_ChessScoreLR[it.x][it.y] += depth1;
		}
		else
		{
			if (tag1 != NULLCHESS)
			{
				Opp_ChessScoreLR[it.x][it.y] += Count1;
				ChessScoreLR[it.x][it.y] += depth1;
			}

		}
		if (tag1 != NULLCHESS || tag != NULLCHESS)
		{
			if (tag == NULLCHESS)
			{
				ChessScoreLR[it.x][it.y] += Count;
				Opp_ChessScoreLR[it.x][it.y] += Count;
			}
			if (tag1 == NULLCHESS)
			{
				ChessScoreLR[it.x][it.y] += Count1;
				Opp_ChessScoreLR[it.x][it.y] += Count1;
			}
		}
		//上下方向
		depth = 0;
		tag = NULLCHESS;
		Count = JudgeChessCanWin(&it.x, &it.y, &tag, Up, WINCOUNT, &depth);

		if (tag == GetFlag())
		{
			//如果direction方向的第一个棋子信息是本方的
			//那么把分数加到本方的积分板上
			ChessScoreUD[it.x][it.y] += Count;
			Opp_ChessScoreUD[it.x][it.y] += depth;
		}
		else
		{
			if (tag != NULLCHESS)
			{
				//如果direction方向的第一个棋子信息是对方的
			//那么把分数加到对方的积分板上
				Opp_ChessScoreUD[it.x][it.y] += Count;
				ChessScoreUD[it.x][it.y] += depth;
			}
		}
		depth1 = 0;
		tag1 = NULLCHESS;
		Count1 = JudgeChessCanWin(&it.x, &it.y, &tag1, Down, WINCOUNT, &depth1);
		if (tag1 == GetFlag())
		{
			ChessScoreUD[it.x][it.y] += Count1;
			Opp_ChessScoreUD[it.x][it.y] += depth1;
		}
		else
		{
			if (tag1 != NULLCHESS)
			{
				Opp_ChessScoreUD[it.x][it.y] += Count1;
				ChessScoreUD[it.x][it.y] += depth1;
			}

		}
		if (tag1 != NULLCHESS || tag != NULLCHESS)
		{
			if (tag == NULLCHESS)
			{
				ChessScoreUD[it.x][it.y] += Count;
				Opp_ChessScoreUD[it.x][it.y] += Count;
			}
			if (tag1 == NULLCHESS)
			{
				ChessScoreUD[it.x][it.y] += Count1;
				Opp_ChessScoreUD[it.x][it.y] += Count1;
			}
		}
		//左上右下
		depth = 0;
		tag = NULLCHESS;
		Count = JudgeChessCanWin(&it.x, &it.y, &tag, LeftUp, WINCOUNT, &depth);

		if (tag == GetFlag())
		{
			//如果direction方向的第一个棋子信息是本方的
			//那么把分数加到本方的积分板上
			ChessScoreUL[it.x][it.y] += Count;
			Opp_ChessScoreUL[it.x][it.y] += depth;
		}
		else
		{
			if (tag != NULLCHESS)
			{
				//如果direction方向的第一个棋子信息是对方的
			//那么把分数加到对方的积分板上
				Opp_ChessScoreUL[it.x][it.y] += Count;
				ChessScoreUL[it.x][it.y] += depth;
			}
		}
		depth1 = 0;
		tag1 = NULLCHESS;
		Count1 = JudgeChessCanWin(&it.x, &it.y, &tag1, RightDown, WINCOUNT, &depth1);
		if (tag1 == GetFlag())
		{
			ChessScoreUL[it.x][it.y] += Count1;
			Opp_ChessScoreUL[it.x][it.y] += depth1;
		}
		else
		{
			if (tag1 != NULLCHESS)
			{
				Opp_ChessScoreUL[it.x][it.y] += Count1;
				ChessScoreUL[it.x][it.y] += depth1;
			}

		}
		if (tag1 != NULLCHESS || tag != NULLCHESS)
		{
			if (tag == NULLCHESS)
			{
				ChessScoreUL[it.x][it.y] += Count;
				Opp_ChessScoreUL[it.x][it.y] += Count;
			}
			if (tag1 == NULLCHESS)
			{
				ChessScoreUL[it.x][it.y] += Count1;
				Opp_ChessScoreUL[it.x][it.y] += Count1;
			}
		}
		//右上左下
		depth = 0;
		tag = NULLCHESS;
		Count = JudgeChessCanWin(&it.x, &it.y, &tag, RightUp, WINCOUNT, &depth);

		if (tag == GetFlag())
		{
			//如果direction方向的第一个棋子信息是本方的
			//那么把分数加到本方的积分板上
			ChessScoreUR[it.x][it.y] += Count;
			Opp_ChessScoreUR[it.x][it.y] += depth;
		}
		else
		{
			if (tag != NULLCHESS)
			{
				//如果direction方向的第一个棋子信息是对方的
			//那么把分数加到对方的积分板上
				Opp_ChessScoreUR[it.x][it.y] += Count;
				ChessScoreUR[it.x][it.y] += depth;
			}
		}
		depth1 = 0;
		tag1 = NULLCHESS;
		Count1 = JudgeChessCanWin(&it.x, &it.y, &tag1, LeftDown, WINCOUNT, &depth1);
		if (tag1 == GetFlag())
		{
			ChessScoreUR[it.x][it.y] += Count1;
			Opp_ChessScoreUR[it.x][it.y] += depth1;
		}
		else
		{
			if (tag1 != NULLCHESS)
			{
				Opp_ChessScoreUR[it.x][it.y] += Count1;
				ChessScoreUR[it.x][it.y] += depth1;
			}

		}
		if (tag1 != NULLCHESS || tag != NULLCHESS)
		{
			if (tag == NULLCHESS)
			{
				ChessScoreUR[it.x][it.y] += Count;
				Opp_ChessScoreUR[it.x][it.y] += Count;
			}
			if (tag1 == NULLCHESS)
			{
				ChessScoreUR[it.x][it.y] += Count1;
				Opp_ChessScoreUR[it.x][it.y] += Count1;
			}
		}
	}
	int count =(WINCOUNT - 1);
	for each  (PointPos it in BestPoints)
	{
		if (Opp_ChessScoreUR[it.x][it.y] >= count || Opp_ChessScoreUL[it.x][it.y] >= count || Opp_ChessScoreUD[it.x][it.y] >= count || Opp_ChessScoreLR[it.x][it.y] >= count)
		{
			temp2.push_back(it);
		}
			if(ChessScoreUR[it.x][it.y] >= count || ChessScoreUL[it.x][it.y] >= count || ChessScoreUD[it.x][it.y] >= count|| ChessScoreLR[it.x][it.y] >=count)
		{
				temp.push_back(it);
		}
	}
	
	if (!temp.empty()|| !temp2.empty())
	{
		BestPoints.clear();
		for each (PointPos var in temp)
		{
			BestPoints.push_back(var);
		}
		Opp_BestPoints.clear();
		for each (PointPos var in temp2)
		{
			Opp_BestPoints.push_back(var);
		}
	}
	
		temp.clear();
		temp2.clear();
		return;
}
void CMFCFivePointsChessApp::ReAssessResult(int* x, int* y)
{
	//上一次筛选会把4个子的直接内推 其他的删掉
	//如果没有四个子的那么再比较
	//方向棋子的信息 是1还是2 用于识别我方
	int MyTag = GetFlag();
	int OppTag = WHITECHESS;
	if (OppTag == MyTag)
	{
		OppTag = BLACKCHESS;
	}

	/*------------------------第三轮筛选------------------*/
	//上两轮的分数还在记录板里 把他们再进行处理
	/*-------------------分别对对手 和 自己的计分板进行操作-----------------------*/
	int count;
	int count1;
	int flag;
	int flag1;
	//先把点扩大16倍
	for each (PointPos it in BestPoints)
	{
		ChessScoreLR[it.x][it.y] *= 32;
		ChessScoreUL[it.x][it.y] *= 32;
		ChessScoreUR[it.x][it.y] *= 32;
		ChessScoreUD[it.x][it.y] *= 32;
	}
	for each (PointPos it in Opp_BestPoints)
	{
		Opp_ChessScoreLR[it.x][it.y] *= 32;
		Opp_ChessScoreUL[it.x][it.y] *= 32;
		Opp_ChessScoreUR[it.x][it.y] *= 32;
		Opp_ChessScoreUD[it.x][it.y] *= 32;
	}

	std::vector<PointPos> temp3;
	std::vector<PointPos> temp4;
	/*--------------求差集----------------*/
	//记录在我方而没有记录在敌方的
	for each (PointPos it in BestPoints)
	{
		bool flag = false;
		for each (PointPos dit in Opp_BestPoints)
		{
			if (it.x == dit.x&&it.y == dit.y)
			{
				flag = true;
			}
		}
		if (!flag)
		{
			temp3.push_back(it);
		}
	}
	//记录在敌方方而没有记录在我方的
	for each (PointPos it in Opp_BestPoints)
	{
		bool flag = false;
		for each (PointPos dit in BestPoints)
		{
			if (it.x == dit.x&&it.y == dit.y)
			{
				flag = true;
			}
		}
		if (!flag)
		{
			temp4.push_back(it);
		}
	}

	/*-------------求差集结束-------------*/
	/*--------------------把差集里的元素再处理--------------------*/
	for each (PointPos it in temp4)
	{
		ChessScoreLR[it.x][it.y] *= 32;
		ChessScoreUL[it.x][it.y] *= 32;
		ChessScoreUR[it.x][it.y] *= 32;
		ChessScoreUD[it.x][it.y] *= 32;
	}
	for each (PointPos it in temp3)
	{
		Opp_ChessScoreLR[it.x][it.y] *= 32;
		Opp_ChessScoreUL[it.x][it.y] *= 32;
		Opp_ChessScoreUR[it.x][it.y] *= 32;
		Opp_ChessScoreUD[it.x][it.y] *= 32;
	}


	for each (PointPos it in BestPoints)
	{
		//左右
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, Left, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, Right, WINCOUNT,false);
		
			ChessScoreLR[it.x][it.y] -= (count + count1);
		
		//左上右下
	
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, LeftUp, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, RightDown, WINCOUNT,false);
		
		
			ChessScoreUL[it.x][it.y] -= (count + count1);
		
		//上下
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, Up, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, Down, WINCOUNT,false);
	
			ChessScoreUD[it.x][it.y] -= (count + count1);
		
		//右上左下
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, RightUp, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, LeftDown, WINCOUNT,false);
		
			ChessScoreUR[it.x][it.y] -= (count + count1);
		
		/*------------------------------------------空格奖励-----------------------------------------------------------*/

		//左右
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, Left, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, Right, WINCOUNT, false);
		ChessScoreLR[it.x][it.y] += (count + count1);
		//上下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, Up, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, Down, WINCOUNT, false);
		ChessScoreUD[it.x][it.y] += (count + count1);
		//左上右下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, LeftUp, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, RightDown, WINCOUNT, false);
		ChessScoreUL[it.x][it.y] += (count + count1);
		//右上左下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, RightUp, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, LeftDown, WINCOUNT, false);
		ChessScoreUR[it.x][it.y] += (count + count1);
		/*-----------------------------在中间惩罚-----------------------*/

		//左右
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &MyTag, Left, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &MyTag, Right, WINCOUNT);
		if (flag == MyTag && flag1 == MyTag)
		{
			ChessScoreLR[it.x][it.y] -= (count + count1);
		}
		//上下
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &MyTag, Up, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &MyTag, Down, WINCOUNT);
		if (flag == MyTag && flag1 == MyTag)
		{
			ChessScoreUD[it.x][it.y] -= (count + count1);
		}
		//左上右下
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &MyTag, LeftUp, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &MyTag, RightDown, WINCOUNT);
		if (flag == MyTag && flag1 == MyTag)
		{
			ChessScoreUL[it.x][it.y] -= (count + count1);
		}
		//左下右上
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &MyTag, LeftDown, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &MyTag, RightUp, WINCOUNT);
		if (flag == MyTag && flag1 == MyTag)
		{
			ChessScoreUR[it.x][it.y] -= (count + count1);
		}



		/*-----------------------------在中间惩罚2-----------------------*/

		//左右
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, Left, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreLR[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, Right, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreLR[it.x][it.y] -= count;
		}
		//上下

		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, Up, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUD[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, Down, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUD[it.x][it.y] -= count;
		}
		//左上右下

		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, LeftUp, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUL[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, RightDown, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUL[it.x][it.y] -= count;
		}
		//左下右上
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, LeftDown, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUR[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, RightUp, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUR[it.x][it.y] -= count;
		}
	}
	for each (PointPos it in Opp_BestPoints)
	{
		//左右
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, Left, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, Right, WINCOUNT,false);
		
			Opp_ChessScoreLR[it.x][it.y] -= (count + count1);
		
		//左上右下
	
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, LeftUp, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, RightDown, WINCOUNT,false);
	
			Opp_ChessScoreUL[it.x][it.y] -= (count + count1);
		
		//上下
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, Up, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, Down, WINCOUNT,false);
	
			Opp_ChessScoreUD[it.x][it.y] -= (count + count1);
		
		//右上左下
	
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, RightUp, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, LeftDown, WINCOUNT,false);
	
			Opp_ChessScoreUR[it.x][it.y] -= (count + count1);
		
		/*------------------------------------------空格奖励-----------------------------------------------------------*/

		//左右
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, Left, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, Right, WINCOUNT, false);
		Opp_ChessScoreLR[it.x][it.y] += (count + count1);
		//上下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, Up, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, Down, WINCOUNT, false);
		Opp_ChessScoreUD[it.x][it.y] += (count + count1);
		//左上右下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, LeftUp, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, RightDown, WINCOUNT, false);
		Opp_ChessScoreUL[it.x][it.y] += (count + count1);
		//右上左下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, RightUp, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, LeftDown, WINCOUNT, false);
		Opp_ChessScoreUR[it.x][it.y] += (count + count1);

		/*-----------------------------在中间惩罚-----------------------*/

	//左右
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &OppTag, Left, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &OppTag, Right, WINCOUNT);
		if (flag == OppTag && flag1 == OppTag)
		{
			Opp_ChessScoreLR[it.x][it.y] -= (count + count1);
		}
		//上下
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &OppTag, Up, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &OppTag, Down, WINCOUNT);
		if (flag == OppTag && flag1 == OppTag)
		{
			Opp_ChessScoreUD[it.x][it.y] -= (count + count1);
		}
		//左上右下
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &OppTag, LeftUp, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &OppTag, RightDown, WINCOUNT);
		if (flag == OppTag && flag1 == OppTag)
		{
			Opp_ChessScoreUL[it.x][it.y] -= (count + count1);
		}
		//左下右上
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &OppTag, LeftDown, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &OppTag, RightUp, WINCOUNT);
		if (flag == OppTag && flag1 == OppTag)
		{
			Opp_ChessScoreUR[it.x][it.y] -= (count + count1);
		}

		/*-----------------------------在中间惩罚2-----------------------*/

		//左右
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, Left, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreLR[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, Right, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreLR[it.x][it.y] -= count;
		}
		//上下

		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, Up, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUD[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, Down, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUD[it.x][it.y] -= count;
		}
		//左上右下

		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, LeftUp, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUL[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, RightDown, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUL[it.x][it.y] -= count;
		}
		//左下右上
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, LeftDown, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUR[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, RightUp, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUR[it.x][it.y] -= count;
		}
	}


	for each (PointPos it in temp3)
	{
		//左右
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, Left, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, Right, WINCOUNT,false);
		
			Opp_ChessScoreLR[it.x][it.y] -= (count + count1);
		
		//左上右下
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, LeftUp, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, RightDown, WINCOUNT,false);
	
			Opp_ChessScoreUL[it.x][it.y] -= (count + count1);
		
		//上下
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, Up, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, Down, WINCOUNT,false);
	
			Opp_ChessScoreUD[it.x][it.y] -= (count + count1);
	
		//右上左下
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, RightUp, WINCOUNT,false);
		//只有不是两边都空时 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &OppTag, LeftDown, WINCOUNT,false);
		
			Opp_ChessScoreUR[it.x][it.y] -= (count + count1);
		
		/*-------------------------------*/
		//左右
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, Left, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, Right, WINCOUNT, false);
		Opp_ChessScoreLR[it.x][it.y] += (count + count1);
		//上下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, Up, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, Down, WINCOUNT, false);
		Opp_ChessScoreUD[it.x][it.y] += (count + count1);
		//左上右下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, LeftUp, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, RightDown, WINCOUNT, false);
		Opp_ChessScoreUL[it.x][it.y] += (count + count1);
		//右上左下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, RightUp, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &OppTag, LeftDown, WINCOUNT, false);
		Opp_ChessScoreUR[it.x][it.y] += (count + count1);

		/*-----------------------------在中间惩罚-----------------------*/

	//左右
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &OppTag, Left, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &OppTag, Right, WINCOUNT);
		if (flag == OppTag && flag1 == OppTag)
		{
			Opp_ChessScoreLR[it.x][it.y] -= (count + count1);
		}
		//上下
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &OppTag, Up, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &OppTag, Down, WINCOUNT);
		if (flag == OppTag && flag1 == OppTag)
		{
			Opp_ChessScoreUD[it.x][it.y] -= (count + count1);
		}
		//左上右下
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &OppTag, LeftUp, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &OppTag, RightDown, WINCOUNT);
		if (flag == OppTag && flag1 == OppTag)
		{
			Opp_ChessScoreUL[it.x][it.y] -= (count + count1);
		}
		//左下右上
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &OppTag, LeftDown, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &OppTag, RightUp, WINCOUNT);
		if (flag == OppTag && flag1 == OppTag)
		{
			Opp_ChessScoreUR[it.x][it.y] -= (count + count1);
		}
		/*-----------------------------在中间惩罚2-----------------------*/

		//左右
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, Left, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreLR[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, Right, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreLR[it.x][it.y] -= count;
		}
		//上下

		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, Up, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUD[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, Down, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUD[it.x][it.y] -= count;
		}
		//左上右下

		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, LeftUp, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUL[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, RightDown, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUL[it.x][it.y] -= count;
		}
		//左下右上
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, LeftDown, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUR[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &OppTag, false, RightUp, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == OppTag)
		{
			Opp_ChessScoreUR[it.x][it.y] -= count;
		}

	}
	for each (PointPos it in temp4)
	{
		//左右
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, Left, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, Right, WINCOUNT,false);
	
			ChessScoreLR[it.x][it.y] -= (count + count1);
		
		//左上右下
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, LeftUp, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, RightDown, WINCOUNT,false);
	
			ChessScoreUL[it.x][it.y] -= (count + count1);
		
		//上下
	
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, Up, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, Down, WINCOUNT,false);
	
			ChessScoreUD[it.x][it.y] -= (count + count1);
		
		//右上左下
		
		count = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, RightUp, WINCOUNT,false);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints(&it.x, &it.y, &MyTag, LeftDown, WINCOUNT,false);
	
			ChessScoreUR[it.x][it.y] -= (count + count1);
		

		/*------------------------------------------空格奖励-----------------------------------------------------------*/

//左右
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, Left, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, Right, WINCOUNT, false);
		ChessScoreLR[it.x][it.y] += (count + count1);
		//上下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, Up, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, Down, WINCOUNT, false);
		ChessScoreUD[it.x][it.y] += (count + count1);
		//左上右下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, LeftUp, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, RightDown, WINCOUNT, false);
		ChessScoreUL[it.x][it.y] += (count + count1);
		//右上左下
		count = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, RightUp, WINCOUNT, false);
		count1 = FindOpp_PointOfDirectionPoints3(&it.x, &it.y, &MyTag, LeftDown, WINCOUNT, false);
		ChessScoreUR[it.x][it.y] += (count + count1);

		/*-----------------------------在中间惩罚-----------------------*/

		//左右
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &MyTag, Left, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &MyTag, Right, WINCOUNT);
		if (flag == MyTag && flag1 == MyTag)
		{
			ChessScoreLR[it.x][it.y] -= (count + count1);
		}
		//上下
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &MyTag, Up, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &MyTag, Down, WINCOUNT);
		if (flag == MyTag && flag1 == MyTag)
		{
			ChessScoreUD[it.x][it.y] -= (count + count1);
		}
		//左上右下
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &MyTag, LeftUp, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &MyTag, RightDown, WINCOUNT);
		if (flag == MyTag && flag1 == MyTag)
		{
			ChessScoreUL[it.x][it.y] -= (count + count1);
		}
		//左下右上
		flag = NULLCHESS;
		flag1 = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag, &MyTag, LeftDown, WINCOUNT);
		//只有不是两边都空时才减去 和第一次筛选的时候操作一样
		count1 = FindOpp_PointOfDirectionPoints2(&it.x, &it.y, &flag1, &MyTag, RightUp, WINCOUNT);
		if (flag == MyTag && flag1 == MyTag)
		{
			ChessScoreUR[it.x][it.y] -= (count + count1);
		}
		/*-----------------------------在中间惩罚2-----------------------*/

		//左右
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, Left, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreLR[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, Right, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreLR[it.x][it.y] -= count;
		}
		//上下

		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, Up, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUD[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, Down, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUD[it.x][it.y] -= count;
		}
		//左上右下

		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, LeftUp, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUL[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, RightDown, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUL[it.x][it.y] -= count;
		}
		//左下右上
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, LeftDown, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUR[it.x][it.y] -= count;
		}
		flag = NULLCHESS;
		count = FindOpp_PointOfDirectionPoints2_2(&it.x, &it.y, &flag, &MyTag, false, RightUp, WINCOUNT);
		//只要标记为自己说明找到了两次
		if (flag == MyTag)
		{
			ChessScoreUR[it.x][it.y] -= count;
		}
	}

	/*------------------------------处理结束--------------------------*/
	std::vector<PointPos> temp;
	std::vector<PointPos> temp2;

	int BestScore = 0;
	int Opp_BestScore = 0;
	for each (PointPos it in BestPoints)
	{
		BestScore = max(ChessScoreUD[it.x][it.y], max(ChessScoreUL[it.x][it.y], max(ChessScoreUR[it.x][it.y], max(ChessScoreLR[it.x][it.y], BestScore))));

	}
	for each (PointPos it in Opp_BestPoints)
	{
		Opp_BestScore = max(Opp_ChessScoreLR[it.x][it.y], max(Opp_ChessScoreUD[it.x][it.y], max(Opp_ChessScoreUL[it.x][it.y], max(Opp_ChessScoreUR[it.x][it.y], Opp_BestScore))));
	}
	if (BestScore >= Opp_BestScore)
	{
		for each (PointPos it in BestPoints)
		{
			if (ChessScoreLR[it.x][it.y] == BestScore || ChessScoreUR[it.x][it.y] == BestScore || ChessScoreUL[it.x][it.y] == BestScore || ChessScoreUD[it.x][it.y] == BestScore)
			{
				temp.push_back(it);
			}
		}
	}
	if (BestScore <= Opp_BestScore)
	{
		for each (PointPos it in Opp_BestPoints)
		{
			if (Opp_ChessScoreLR[it.x][it.y] == Opp_BestScore || Opp_ChessScoreUR[it.x][it.y] == Opp_BestScore || Opp_ChessScoreUL[it.x][it.y] == Opp_BestScore || Opp_ChessScoreUD[it.x][it.y] == Opp_BestScore)
			{
				temp2.push_back(it);
			}
		}
	}
	if (!temp.empty() || !temp2.empty())
	{
		BestPoints.clear();
		Opp_BestPoints.clear();
		
	}
	for each (PointPos it in temp)
		{
			BestPoints.push_back(it);
		}
		for each (PointPos it in temp2)
		{
			Opp_BestPoints.push_back(it);
		}
	temp.clear();
	temp2.clear();
	temp3.clear();
	temp4.clear();
	return;
}
//第四轮筛选
void CMFCFivePointsChessApp::ReReAssessResult(int* x, int* y)
{
	if (BestPoints.empty()||Opp_BestPoints.empty())
	{
		if (!Opp_BestPoints.empty())
		{
			for each (PointPos it in Opp_BestPoints)
			{
				BestPoints.push_back(it);
			}
		}
		return;
	}
	AddResultInResult();
	int BestScore=0;
	
	/*-------------删去重复元素-------------------------------------*/
	std::vector<PointPos> temp;
	std::vector<PointPos> temp2;
	for each (PointPos it in Opp_BestPoints)
	{
		temp2.push_back(it);
	}
	for each (PointPos it in BestPoints)
	{
		temp.push_back(it);
	}
	for each (PointPos it in temp)
	{
		bool flag = false;
		int i = -1;
		for each (PointPos dit in temp2)
		{
			if (!flag)
			{
				i++;
			}
			if (it.x == dit.x&&it.y == dit.y)
			{
				flag = true;
			}
		
		}

		if (!temp2.empty() && flag)
		{
			temp2.erase(temp2.begin() + i);
		}

	}

	/*------------- ------结束删去重复元素-------------------------------------*/
	for each (PointPos it in temp2)
	{
		temp.push_back(it);
	}
	temp2.clear();
	for each (PointPos it in temp)
	{
		if (BestScore < ChessScoreResult[it.x][it.y] + Opp_ChessScoreResult[it.x][it.y])
			BestScore = ChessScoreResult[it.x][it.y] + Opp_ChessScoreResult[it.x][it.y];
	}
	for each (PointPos it in temp)
	{
		if (ChessScoreResult[it.x][it.y] + Opp_ChessScoreResult[it.x][it.y] == BestScore)
		{
			temp2.push_back(it);
		}
	}
	if (temp2.empty())
	{
		return;
    }
	else
	{
		Opp_BestPoints.clear();
		BestPoints.clear();
		for each (PointPos it in temp2)
		{
			BestPoints.push_back(it);
		}
	}
	temp.clear();
	temp2.clear();
	return;
	
}


void CMFCFivePointsChessApp::CalculateEmptyPointContainCount()
{
	int MyTag = GetFlag();
	int OppTag = BLACKCHESS;
	if (OppTag == MyTag)
	{
		OppTag = WHITECHESS;
	}
	int count=0;
	for each (PointPos it in BestPoints)
	{
	
		count = 0;
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 5), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 4), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 3), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 2), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 1), count);
		ChessScoreLR[it.x][it.y] += count;
		//上下
		count = 0;
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 5), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 4), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 3), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 2), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 1), count);
		ChessScoreUD[it.x][it.y] += count;
		//左上右下

		count = 0;
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 5), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 4), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 3), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 2), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 1), count);
		ChessScoreUL[it.x][it.y] += count;
		//左下右上
		count = 0;
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 5), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 4), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 3), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 2), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 1), count);
		ChessScoreUR[it.x][it.y] += count;
	}
	for each (PointPos it in Opp_BestPoints)
	{
		//左右
		count = 0;
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 5), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 4), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 3), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 2), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 1), count);
		Opp_ChessScoreLR[it.x][it.y] += count;
		//上下
		count = 0;
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 5), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 4), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 3), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 2), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 1), count);
		Opp_ChessScoreUD[it.x][it.y] += count;
		//左上右下

		count = 0;
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 5), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 4), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 3), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 2), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 1), count);
		Opp_ChessScoreUL[it.x][it.y] += count;
		//左下右上
		count = 0;
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 5), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 4), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 3), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 2), count);
		count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 1), count);
		Opp_ChessScoreUR[it.x][it.y] += count;
	}
}
//通过棋子数目来进行判断
void CMFCFivePointsChessApp::JudgeFromChessCount()
{
	std::vector<PointPos> temp;
	std::vector<PointPos> Opp_temp;
	int BestScore=0;
	int Opp_BestScore=0;
	//分别求最大值
	for each (PointPos it in BestPoints)
	{
		BestScore = max(ChessScoreUD[it.x][it.y], max(ChessScoreUL[it.x][it.y], max(ChessScoreUR[it.x][it.y], max(ChessScoreLR[it.x][it.y], BestScore))));

	}
	for each (PointPos it in Opp_BestPoints)
	{
		Opp_BestScore = max(Opp_ChessScoreLR[it.x][it.y], max(Opp_ChessScoreUD[it.x][it.y], max(Opp_ChessScoreUL[it.x][it.y], max(Opp_ChessScoreUR[it.x][it.y], Opp_BestScore))));
	}
	//如果是4和3 BestScore>=Opp_BestScore 用BestScore 
	//如果是小于3 那么只有BestScore=Opp时两者都采用 然后是哪个大用哪个
	
		if (BestScore >= Opp_BestScore)
		{
			for each (PointPos it in BestPoints)
			{
				//只要有一个边的子数和最好数目一样加入vector
				if (ChessScoreLR[it.x][it.y] == BestScore || ChessScoreUR[it.x][it.y] == BestScore || ChessScoreUL[it.x][it.y] == BestScore || ChessScoreUD[it.x][it.y] == BestScore)
				{
					temp.push_back(it);
				}
			}
		}
		else
		{
			for each (PointPos it in Opp_BestPoints)
			{
				//只要有一个边的子数和最好数目一样加入vector
				if (Opp_ChessScoreLR[it.x][it.y] == Opp_BestScore || Opp_ChessScoreUR[it.x][it.y] == Opp_BestScore || Opp_ChessScoreUL[it.x][it.y] == Opp_BestScore || Opp_ChessScoreUD[it.x][it.y] == Opp_BestScore)
				{
					Opp_temp.push_back(it);
				}
			}
			
		}
	
	if (!temp.empty()|| !Opp_temp.empty())
	{
		BestPoints.clear();
		Opp_BestPoints.clear();
	}
	else
	{
		return;
	}
	for each (PointPos it in temp)
	{
		BestPoints.push_back(it);
	}
	for each (PointPos it in Opp_temp)
	{
		Opp_BestPoints.push_back(it);
	}
	temp.clear();
	Opp_temp.clear();
	return;
}
int CMFCFivePointsChessApp::SearchOurPointsCount(int*x, int*y, int* TargetTag, Direction direction, int depth)
{
	int x1 = *x;
	int y1 = *y;
	if (depth <= 0)
	{
		return 0;
	}
	//如果是自己 那么加一
	if (ChessTag[x1][y1] == *TargetTag)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return SearchOurPointsCount(&x1, &y1, TargetTag, direction, depth-1) + 1;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if (ChessTag[x1][y1] == NULLCHESS)
		{
			//跳过返回下一层
			if (CalculatePointDiretion(&x1, &y1, direction))
			{
				return SearchOurPointsCount(&x1, &y1, TargetTag, direction, depth-1);
			}
			else
			{
				return 0;
			}
		}
		//碰到敌人直接返回
		return 0;
	}
}
//这个是在第四轮筛选
//减去两个自己棋子之间的棋子  自己在两个棋子里面
//MaxScore为减分的最大分  最后结果是 depth
int CMFCFivePointsChessApp::FindOpp_PointOfDirectionPoints2(int*x, int*y, int* TargetTag,int* MyTag,Direction direction, int depth)
{
	int x1 = *x;
	int y1 = *y;
	if (depth <= 0)
	{
		return 0;
	}
	if (depth == 5)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return FindOpp_PointOfDirectionPoints2(&x1, &y1, TargetTag, MyTag, direction, depth - 1);
		}
		else
		{
			return 0;
		}
	}
	if (*TargetTag !=*MyTag )
	{
		*TargetTag = ChessTag[x1][y1];
	}
	if (ChessTag[x1][y1] != NULLCHESS)
	{
		return 0;
	}
	else
	{
		//3分起步
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return FindOpp_PointOfDirectionPoints2(&x1,&y1,TargetTag,MyTag,direction,depth-1) +3;
		}
		else
		{
			
			return 0;
		}
		
	}
}
//减去两个自己棋子之间的空格分数 自己在两个棋子外面
int CMFCFivePointsChessApp::FindOpp_PointOfDirectionPoints2_2(int*x, int*y, int* TargetTag, int* MyTag,bool Find, Direction direction, int depth)
{
	int x1 = *x;
	int y1 = *y;
	if (depth <= 0)
	{
		return 0;
	}
	if (depth == 5)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return FindOpp_PointOfDirectionPoints2_2(&x1, &y1, TargetTag, MyTag,Find, direction, depth - 1);
		}
		else
		{
			return 0;
		}
	}
	if (*TargetTag != *MyTag)
	{
		if (ChessTag[x1][y1] == *MyTag)
		{
			if (!Find)
			{
				//Find标记第一次找到
				Find = true;
			}
			else
			{
				//第二次找到才会有标记
				*TargetTag = ChessTag[x1][y1];
				return 0;
			}
		}
	}
	if (ChessTag[x1][y1] == NULLCHESS)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			if (Find)
			{

				return FindOpp_PointOfDirectionPoints2_2(&x1, &y1, TargetTag, MyTag, Find, direction, depth - 1) + 5;
			}
			else
			{
				return FindOpp_PointOfDirectionPoints2_2(&x1, &y1, TargetTag, MyTag, Find, direction, depth - 1);
			}
		}
		else
		{
			if (Find)
			{
				
				return  5;
			}
			else
			{
				return 0;
			}
		}

	
	}
	else
	{
		//是别人就直接返回
		if (ChessTag[x1][y1] != *MyTag)
		{
			return 0;
		}
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return FindOpp_PointOfDirectionPoints2_2(&x1, &y1, TargetTag, MyTag, Find, direction, depth - 1);
		}
		else
		{
			return 0;
		}

	}
}
//空格奖励 这个可以改成动态深度 来变化分数
//在TargetTag不是空的前提下 找到Target前的count是2分 而找到Target后的分是1分
int CMFCFivePointsChessApp::FindOpp_PointOfDirectionPoints3(int*x, int*y, int* TargetTag, Direction direction, int depth,bool Find)
{
	int x1 = *x;
	int y1 = *y;
	if (depth <= 0)
	{
		return 0;
	}
	if (depth == 5)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return FindOpp_PointOfDirectionPoints3(&x1, &y1, TargetTag, direction, depth-1,Find);
		}
		else
		{
			return 0;
		}
	}
	if (ChessTag[x1][y1] != NULLCHESS)
	{
		if (ChessTag[x1][y1] == *TargetTag)
		{
			Find = true;
		}
		else
		{
			//找到了别人直接返回
			return 0;
		}
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			if (Find)
			{
				return FindOpp_PointOfDirectionPoints3(&x1, &y1, TargetTag, direction, depth - 1, Find)+4*depth;
			}
			else
			{
				return FindOpp_PointOfDirectionPoints3(&x1, &y1, TargetTag, direction, depth - 1, Find)+4*depth;
			}
			
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			if (Find)
			{
				return FindOpp_PointOfDirectionPoints3(&x1, &y1, TargetTag, direction, depth-1, Find)+1;
			}
			else
			{
				return FindOpp_PointOfDirectionPoints3(&x1, &y1, TargetTag, direction, depth-1, Find)+1;
			}
		}
		else
		{
			//用find来标记 区别找到过没有 找到过就多点奖励
			if (Find)
			{
				return 1;
			}
			else
			{
				return 1;
			}
			
		}
	}
  }

//寻找敌人扣分机制
int CMFCFivePointsChessApp::FindOpp_PointOfDirectionPoints(int*x,int*y, int* MyTag,Direction direction,int depth,bool Find)
{
	//连续有两个空格就直接结束 
	//遇到敌人就减分 遇到一个最低-16分 最高扣-48 2*depth+16
	int x1 = *x;
	int y1 = *y;
	if (depth <= 0)
	{
		return 0;
	}
	if (depth == 5)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return FindOpp_PointOfDirectionPoints(&x1, &y1, MyTag, direction, --depth,false);
		}
		else
		{
			return 2 * (depth - 1) + 16;
		}
	}
	//如果找到了对面的棋子返回0
	if ((ChessTag[x1][y1] != *MyTag && ChessTag[x1][y1] != NULLCHESS))
	{
		return 2 * depth + 16;
	}
	//如果碰到空的
	if (ChessTag[x1][y1] == NULLCHESS)
	{
		
			//只有碰到空
			//没记录就记录
			//记录了就直接退出
			if (Find)
			{
				return 0;
			}
			else
			{
				if (CalculatePointDiretion(&x1, &y1, direction))
				{
					if (depth <= 1)
					{
						if (ChessTag[x1][y1] != *MyTag&&ChessTag[x1][y1]!=NULLCHESS)
						{
							return 16;
						}
					}
					//碰到自己的棋子跳过
					return FindOpp_PointOfDirectionPoints(&x1, &y1, MyTag, direction, --depth, true);
				}
				else
				{
					
					return max(16, 2 * (depth - 1) + 16);
					
				}
			}
			
		
	
	}
	if (CalculatePointDiretion(&x1, &y1, direction))
	{
		if (depth <= 1)
		{
			if (ChessTag[x1][y1] != *MyTag&&ChessTag[x1][y1] != NULLCHESS)
			{
				return 16;
			}
		}
		//碰到自己的棋子跳过
		return FindOpp_PointOfDirectionPoints(&x1, &y1, MyTag, direction, --depth, false);
	}
	else
	{
		
			return max(16,2 * (depth - 1) + 16);
		
	}

}


//判断 是否自己和自己连续的空位太多
int CMFCFivePointsChessApp::JudgeChessCanWin2(int *x, int *y, int* tag,int* flag, Direction direction, int Depth)
{
	int x1 = *x;
	int y1 = *y;
	if (Depth <= 0)
	{
		return 0;
	}
	if (Depth == 5)
	{
		//这个算法开始的格子是空的 所以tag是NULL 直接跳过进去下一层迭代
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return JudgeChessCanWin2(&x1, &y1, tag,flag, direction, --Depth);
		}
		else
		{
			return 0;
		}
	}
	if (*flag == NULLCHESS)
	{
		//如果是空的那么就继续为空直到有不为空的为止
		*flag = GetChess(x1, y1);
	}
	//如果找到了对面的棋子返回0
	if ((ChessTag[x1][y1] != *tag && ChessTag[x1][y1] != NULLCHESS))
	{
		return 0;
	}
	if (ChessTag[x1][y1] == NULLCHESS)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			//只有碰到空的棋子才加分
			return JudgeChessCanWin2(&x1, &y1, tag,flag, direction, --Depth) + 1;
		}
		else
		{
			return 1;
		}
	}
	if (CalculatePointDiretion(&x1, &y1, direction))
	{
		//碰到自己的棋子跳过
		return JudgeChessCanWin2(&x1, &y1, tag,flag, direction, --Depth);
	}
	else
	{
		return 0;
	}

}
//这个是判断非对手的连续空位
int CMFCFivePointsChessApp::JudgeChessCanWin(int *x, int *y, int* tag, Direction direction, int Depth,int* FindDepth)
{
	int x1 = *x;
	int y1 = *y;
	if (Depth <= 0)
	{
		return 0;
	}
	if (Depth == 5)
	{
		//这个算法开始的格子是空的 所以tag是NULL 直接跳过进去下一层迭代
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return JudgeChessCanWin(&x1, &y1, tag, direction, --Depth, FindDepth);
		}
		else
		{
			return 0;
		}

	}
	if (*tag == NULLCHESS)
	{
		//如果是空的那么就继续为空直到有不为空的为止
		
		*tag = GetChess(x1, y1);
		if (*tag == NULLCHESS)
		{
			(*FindDepth)++;
			//如果这一个是空的就直接跳过
			if (CalculatePointDiretion(&x1, &y1, direction))
			{
				
				return JudgeChessCanWin(&x1, &y1, tag, direction, --Depth,FindDepth)+1;
			}
			else
			{
				
				//如果已经是最后一层了不
				return 1;
			}


		}
	}
	//如果是碰到了对面的 直接返回
	if ((ChessTag[x1][y1] != *tag && ChessTag[x1][y1] != NULLCHESS))
	{
		return 0;
	}
	if (CalculatePointDiretion(&x1, &y1, direction))
	{
		//只有开始碰到自己的棋子才加分
		return JudgeChessCanWin(&x1, &y1, tag, direction, --Depth, FindDepth)+1;
	}
	else
	{
		return 1;
	}
	
	
}

//这种是除非遇到敌人棋子不然就一直递归
//这个是判断5格子之内自己的棋子数 中途不能遇到对手棋子
//就是获得5格内连续的自己的棋子
int CMFCFivePointsChessApp::JudgeChessDirection2(int *x, int *y, int* tag, Direction direction, int Depth)
{
	int x1 = *x;
	int y1 = *y;
	if (Depth == 5)
	{
		//这个算法开始的格子是空的 所以tag是NULL 直接跳过进去下一层迭代
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return JudgeChessDirection2(&x1, &y1, tag, direction, --Depth);
		}
		else
		{
			return 0;
		}

	}
	if (*tag == NULLCHESS)
	{
		//如果是空的那么就继续为空直到有不为空的为止

		*tag = GetChess(x1, y1);
		if (*tag == NULLCHESS)
		{
			//如果这一个是空的就直接跳过
			if (CalculatePointDiretion(&x1, &y1, direction))
			{
				return JudgeChessDirection2(&x1, &y1, tag, direction, --Depth);
			}
			else
			{
				return 0;
			}


		}
	}
	if (Depth <= 0 || (ChessTag[x1][y1] != *tag && ChessTag[x1][y1] != NULLCHESS))
	{
		return 0;
	}
	if (ChessTag[x1][y1] == NULLCHESS)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return JudgeChessDirection2(&x1, &y1, tag, direction, --Depth);
		}
		else
		{
			//不能再向附近移动就直接返回这层的数量
			return 0;
		}

	}
	if (CalculatePointDiretion(&x1, &y1, direction))
	{
		return JudgeChessDirection2(&x1, &y1, tag, direction, --Depth) + 1;
	}
	else
	{
		//不能再向附近移动就直接返回这层的数量
		return 1;
	}
}

int CMFCFivePointsChessApp::JudgeChessDirection(int *x, int *y, int* tag, Direction direction, int Depth)//depth为探索深度
{
	int x1 = *x;
	int y1 = *y;
	if (Depth == 5)
	{
		//这个算法开始的格子是空的 所以tag是NULL 直接跳过进去下一层迭代
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return JudgeChessDirection(&x1, &y1, tag, direction, --Depth);
		}
		else
		{
			return 0;
		}
		
	}
	if (Depth == 4)
	{
		*tag = GetChess(x1, y1);
		if (*tag == NULLCHESS)
		{
			//如果这一个是空的就直接返回
			return 0;
		}
	}
	if (Depth <= 0 || ChessTag[x1][y1] != *tag)
	{
		return 0;
	}
	if (CalculatePointDiretion(&x1, &y1, direction))
	{
		return JudgeChessDirection(&x1, &y1, tag, direction, --Depth) + 1;
	}
	else
	{
		//不能再向附近移动就直接返回这层的数量
		return 1;
	}

}
//把自己也计算了
int CMFCFivePointsChessApp::JudgeWinDirection(int *x,int *y,int* tag,Direction direction,int Depth)//depth为探索深度
{
	int x1=*x;
	int y1=*y;
	if (Depth<=0||ChessTag[x1][y1]!=*tag)
	{
		return 0;
	}
	if (CalculatePointDiretion(&x1, &y1, direction))
	{
		return JudgeWinDirection(&x1, &y1, tag, direction, --Depth) + 1;
	}
	else
	{
		return 1;
	}
	
}
PaintType CMFCFivePointsChessApp::Regret()
{
	int x, y;
	PaintType type =DeleteAndLoadLastRecord(&x, &y);
	switch (type)
	{
	case Empty:
	{
		return Empty;
	}
	
	case Black:
	{
		ChessTag[x][y] = NULLCHESS;
		return Black;
	}
	case White:
	{
		ChessTag[x][y] = NULLCHESS;
		return White;
	}
	default:
		return Empty;
	}
}

bool CMFCFivePointsChessApp::CalculatePointDiretion(int* x, int* y,Direction direction)
{
	switch (direction)
	{
	case Left:
	{
		if (*x > 0)
		{
			(*x)--;
			return true;
		}
		break;
	}
	case Right:
	{
		if (*x < ChessBlockNum - 2)
		{
			(*x)++;
			return true;
		}
		break;
	}
	case Up:
	{
		if (*y > 0)
		{
			(*y)--;
			return true;
		}
		break;
	}
	case Down:
	{
		if (*y < ChessBlockNum - 2)
		{
			(*y)++;
			return true;
		}
		break;
	
	}
	case LeftUp:
	{
		if (*y > 0 && *x > 0)
		{
			(*y)--;
			(*x)--;
			return true;
		}
		break;
	}
	
	case LeftDown:
	{
		if (*x > 0 && *y < ChessBlockNum - 2)
		{
			(*y)++;
			(*x)--;
			return true;
		}
		break;
	}
		
	case RightUp:
	{
		if (*x < ChessBlockNum - 2 && *y>0)
		{
			(*y)--;
			(*x)++;
			return true;
		}
		break;
	}
	
	case RightDown:
	{
		if (*x < ChessBlockNum - 2 && *y < ChessBlockNum - 2)
		{
			(*y)++;
			(*x)++;
			return true;
		}
		break;
	}
		
	default:
		break;
	}
	return false;
}


// 唯一的 CMFCFivePointsChessApp 对象

CMFCFivePointsChessApp theApp;


// CMFCFivePointsChessApp 初始化

BOOL CMFCFivePointsChessApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("铭铭制作"));

	CMFCFivePointsChessDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



void CMFCFivePointsChessApp::OnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename = _T("Test.txt");
	CString strWriteData;
	CString str;
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			if (j == ChessBlockNum - 2)
			{
				str.Format(_T("%d\n"), ChessTag[i][j]);
			}
			else
			{
				str.Format(_T("%d "), ChessTag[i][j]);
			}
			strWriteData += str;
		}
	}
	CStdioFile csFile;
	CFileException cfException;
	if (csFile.Open(filename, CFile::typeText | CFile::modeCreate | CFile::modeReadWrite ))//以txt方式读取|若没有文件则创建该文件|文件打开时不清除
	{
		csFile.SeekToEnd();
		setlocale(LC_CTYPE, "chs");
		csFile.WriteString(strWriteData);
	}
	csFile.Close();
}


void CMFCFivePointsChessApp::OnRead()
{
	CStdioFile FileRead;
	if (!(FileRead.Open(_T("test.txt"), CFile::modeRead | CFile::typeText)))
	{
		return;
	}
	vector<CString>vecReadText;
	CString cstrValue;
	while (FileRead.ReadString(cstrValue))
	{
		vecReadText.push_back(cstrValue);
	}

	FileRead.Close();


	CString str;
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		int j = 0;
		int count=vecReadText[i].GetLength();
		str = _T(" ");
		for (int k = 0; k < count; k++)
		{
			if (vecReadText[i][k] != str)
			{
				char a = vecReadText[i][k];
				ChessTag[i][j] = (int)a-48;
				j++;
			}
		}
		
	}
	
}
