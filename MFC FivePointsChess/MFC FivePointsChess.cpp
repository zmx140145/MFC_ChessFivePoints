
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
	ON_COMMAND(ID_32773, &CMFCFivePointsChessApp::OnRemoveRecord)
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

}
// 唯一的 CMFCFivePointsChessApp 对象

CMFCFivePointsChessApp theApp;



//初始化对话框
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
//设置AI的情绪 用于判断使用哪种图片
void CMFCFivePointsChessApp::SetAI_Station(Station st)
{
	AI_Station = st;
}
//获得AI的情绪
Station CMFCFivePointsChessApp::GetAI_Station()
{
	return AI_Station;
}
//设置技能 nouse就是没有用技能 其他就是正在用相应的技能
void CMFCFivePointsChessApp::SetSkill(Skill sk)
{
	PlayerSkill = sk;
}
//获得当前使用技能
Skill CMFCFivePointsChessApp::GetSkill()
{
	return PlayerSkill;
}
//游戏的初始化 包括初始化棋盘 初始化技能CD 初始化历史记录 等
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
	freeRecord();
	CD_Skill_Black = -1;
	CD_Skill_White = -1;
}
//获得棋盘位置的信息
int CMFCFivePointsChessApp::GetChess(int x, int y)
{
	return ChessTag[x][y];
}
//存储历史记录

//尝试删除并读取记录 读取失败返回Empty 成功就返回相应棋子的类型 黑还是白
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
	int i = PosX ;
	int j = PosY ;
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
	return false;
}

//获得当前下棋手
int CMFCFivePointsChessApp::GetFlag()
{
	return m_nFlag;
}
//改变下棋方
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
//判断输赢 0没有人赢 1黑方赢 2白方赢
int CMFCFivePointsChessApp::JudgeWin(int x, int y)
{
	int tag= GetChess(x, y);
	if (tag == 0)
	{
		return 0;
	}
	//判断4个方向 
	//左右方向
	if (JudgeWinDirection(&x, &y, &tag, Left, WINCOUNT) + JudgeWinDirection(&x, &y, &tag, Right, WINCOUNT) - 1 >= WINCOUNT)
	{
		return tag;
	}
	//上下方向
	if (JudgeWinDirection(&x, &y, &tag, Up, WINCOUNT) + JudgeWinDirection(&x, &y, &tag, Down, WINCOUNT) - 1 >= WINCOUNT)
	{
		return tag;
	}
	//左上右下
	if (JudgeWinDirection(&x, &y, &tag, LeftUp, WINCOUNT) + JudgeWinDirection(&x, &y, &tag, RightDown, WINCOUNT) - 1 >= WINCOUNT)
	{
		return tag;
	}
	if (JudgeWinDirection(&x, &y, &tag, LeftDown, WINCOUNT) + JudgeWinDirection(&x, &y, &tag, RightUp, WINCOUNT) - 1 >= WINCOUNT)
	{
		return tag;
	}
	return 0;
}
//清空所有的积分板
void CMFCFivePointsChessApp::InitAllScoreArray()
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			ALLChessScoreResult[i][j] = 0;
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
void CMFCFivePointsChessApp::InitAllScoreArray1()
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
//只清空总分积分板
void CMFCFivePointsChessApp::InitResultScoreArray()
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			ChessScoreResult[i][j] = 0;
			Opp_ChessScoreResult[i][j] = 0;
			ALLChessScoreResult[i][j] = 0;
		}
	}

}
//清空除总分外的积分记录
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
//AI调用的总入口
bool CMFCFivePointsChessApp::AI_FindBestPoint(int * x, int * y)
{
	
	std::vector<PointPos*> temp;
	//先初始化分数 设置为0
	InitAllScoreArray();
	BestPoints.clear();
	Opp_BestPoints.clear();
	Total_BestPoints.clear();
	//计算分数
	/*----------------前置筛选---------------*/
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
	bool NoRequirePeace=true;
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			if (ChessScoreResult[i][j] == 1)
			{
				havePoint = true;
				PointPos* p = new PointPos{ i,j };
				temp.push_back(p);
				BestPoints.push_back(*p);
			}
		}
	}
	//这个是完全没有棋子 让AI把位置放在屏幕中间
	if (!havePoint)
	{
		if (ChessTag[7][7] == NULLCHESS)
		{
			PointPos* p = new PointPos{ 7,7};
			temp.push_back(p);
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
	if (!FirstAssessResult())
	{
		NoRequirePeace = false;
	}
	//把能五子但局部不能五子的地方标记 -1
	SecondAssessResult();
	                                          /*-------------------两轮筛选之后的到的点 符合以下条件：-------------------------*/
				                                          //1.首先有机会形成五个子 无论是敌方还是自己
				                                         //2.把有些积分是负数说明是不能可能形成5子的
	                                          /*-------------------------------------------------------------------------------*/
 /*----------------第三轮筛选---------------*/
	//得到真实的棋子数
/*-------------------------------------------*/
	ThirdAssessResult();

	//如果有大于3 也就是可以决胜的棋子直接跳过这一步 会返回1 
	if (JudgeFromChessCount() == 0)
	{
		             /*===============改变情绪================*/
		               AI_Station = think;
		             /*--------------------势均力敌---------------*/
	/*----------------第四轮筛选---------------*/
		  //细分1 2 3棋子数 映射为 0~5
		  FourthAssessResult();
		
	/*-----------弱智电脑---------------------*/
 
		  if (GameMode <= 1||AI_FindDepth<=1)
		 {
			 //选择得分最大的点 保存到最优点里面
			 JudgeFromChessCount2(1, 0);
			 /*----------------第五轮筛选---------------*/
	 //根据敌人远近 自己棋子多少 空格位多少来算单个方向的的分
	/*-----------------------------------------*/
			 FifthAssessResult();

			 /*----------------第六轮筛选---------------*/
			//把所有方向的结果加起来 得到综合分最高的点
			 /*-----------------------------------------*/
			 // 把分数为负数的积分设置为0
			 SetScoreArrayZero();
			 //开始计算
			 SixthAssessResult();
		 }
		 /*-----------聪明电脑---------------------*/
		 else
		 {

			 //选择得分大的5个点 
			 int i = JudgeFromChessCount2(2, 5);
			 //先清空总分计分板
			 Total_BestPoints.clear();
			 InitResultScoreArray();
			 std::vector<PointPos> temp1;
			 std::vector<PointPos> temp2;
			 for each (PointPos it in Opp_BestPoints)
			 {
				 temp1.push_back(it);
				
			 }
			 for each (PointPos it in BestPoints)
			 {
				 temp2.push_back(it);
				
			 }
			 for each (PointPos it in temp1)
			 {
				 SPAI_CalculateScoreWithChessPoint(it.x, it.y, 3 - GetFlag());
			 }
			 for each (PointPos it in temp2)
			 {
				 SPAI_CalculateScoreWithChessPoint(it.x, it.y, GetFlag());
			 }
			 int BestOfAllScore=INT_MIN;
			 BestPoints.clear();
			 for each (PointScore* it in Total_BestScoreVec)
			 {
				 if (*it->TotalScore > BestOfAllScore)
				 {
					 BestOfAllScore = *it->TotalScore;
				 }
			 }
			 for each (PointScore* it in Total_BestScoreVec)
			 {
				 if (*it->TotalScore == BestOfAllScore)
				 {
					 PointPos dit{ it->x,it->y };
					 BestPoints.push_back(dit);
					
				 }
				
			 }
		 }
	}
	
	
	int p;
	//对剩余的点取随机
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
	PointPos* pre;
	for each  (PointPos* it in temp)
	{
		pre = it;
		it = NULL;
		free(pre);
	}
	PointScore* pS;

	while (RubbishPS.size() > 0)
	{
		pS = RubbishPS.back();
		RubbishPS.pop_back();
		pS->TotalScore = NULL;
		pS->PointScoreChilds.clear();
		delete(pS);
		
	}
	Total_BestScoreVec.clear();
	return NoRequirePeace;
}
//用当前时间获得随机数
unsigned CMFCFivePointsChessApp::randInt(int min, int max)
{
	long t = GetTickCount();
	srand((int)t);
	return (rand() % (max - min + 1)) + min;
}
//把所有方向的结果加到总积分板上
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
//把有棋子的格子周围的空格子加到最优节点中
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
//把某方向的空格子加到最优节点中
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
//初始筛选 把不可能五个子的位置剔除
//第一轮是用于去除不可能下到5个棋的点
bool CMFCFivePointsChessApp::FirstAssessResult()
{
	std::vector<PointPos> temp;
	std::vector<PointPos> temp2;
	int MyTag = GetFlag();
	int OppTag = WHITECHESS;
	if (OppTag == MyTag)
	{
		OppTag = BLACKCHESS;
	}

	
	/*-------------------对于每一个空位点进行操作-----------------------*/
	for each (PointPos it in BestPoints)
	{
		//左右
		ChessScoreLR[it.x][it.y]=JudgeChessCanWin(&it.x, &it.y, MyTag, Right, WINCOUNT) +JudgeChessCanWin(&it.x, &it.y, MyTag, Left, WINCOUNT);
		Opp_ChessScoreLR[it.x][it.y] = JudgeChessCanWin(&it.x, &it.y, OppTag, Right, WINCOUNT) + JudgeChessCanWin(&it.x, &it.y, OppTag, Left, WINCOUNT);
		//上下
		ChessScoreUD[it.x][it.y] = JudgeChessCanWin(&it.x, &it.y, MyTag, Up, WINCOUNT) + JudgeChessCanWin(&it.x, &it.y, MyTag, Down, WINCOUNT);
		Opp_ChessScoreUD[it.x][it.y] = JudgeChessCanWin(&it.x, &it.y, OppTag, Up, WINCOUNT) + JudgeChessCanWin(&it.x, &it.y, OppTag, Down, WINCOUNT);
		//左上右下
		ChessScoreUL[it.x][it.y]=JudgeChessCanWin(&it.x, &it.y, MyTag, LeftUp, WINCOUNT) +JudgeChessCanWin(&it.x, &it.y, MyTag, RightDown, WINCOUNT);
		Opp_ChessScoreUL[it.x][it.y] = JudgeChessCanWin(&it.x, &it.y, OppTag, LeftUp, WINCOUNT) + JudgeChessCanWin(&it.x, &it.y, OppTag, RightDown, WINCOUNT);
		//左下右上
		ChessScoreUR[it.x][it.y] = JudgeChessCanWin(&it.x, &it.y, MyTag, RightUp, WINCOUNT) + JudgeChessCanWin(&it.x, &it.y, MyTag, LeftDown, WINCOUNT);
		Opp_ChessScoreUR[it.x][it.y] = JudgeChessCanWin(&it.x, &it.y, OppTag, RightUp, WINCOUNT) + JudgeChessCanWin(&it.x, &it.y, OppTag, LeftDown, WINCOUNT);
	}
	int count = (WINCOUNT - 1);
	for each  (PointPos it in BestPoints)
	{
		if (Opp_ChessScoreUR[it.x][it.y] >= count || Opp_ChessScoreUL[it.x][it.y] >= count || Opp_ChessScoreUD[it.x][it.y] >= count || Opp_ChessScoreLR[it.x][it.y] >= count)
		{
			temp2.push_back(it);
		}
		if (ChessScoreUR[it.x][it.y] >= count || ChessScoreUL[it.x][it.y] >= count || ChessScoreUD[it.x][it.y] >= count || ChessScoreLR[it.x][it.y] >= count)
		{
			temp.push_back(it);
		}
	}
	bool deal = true;
	if (!temp.empty() || !temp2.empty())
	{
		if (temp.empty())
		{
			deal = false;
		}
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
	return deal;
}
//把不可能下到棋的方向剔除 因为最大分数是8  那么小于4的都设置为-1 其他设置为0 
//第二轮是用于识别第一轮中虽然有可能下到5个子但是有些方向是不能下5个子的点
void CMFCFivePointsChessApp::SecondAssessResult()
{
	
	for each (PointPos it in BestPoints)
	{
		if (ChessScoreLR[it.x][it.y] < WINCOUNT-1)
		{
			ChessScoreLR[it.x][it.y] = -1;
		}
		else
		{
			ChessScoreLR[it.x][it.y] = 0;
		}
		if (ChessScoreUR[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUR[it.x][it.y] = -1;
		}
		else
		{
			ChessScoreUR[it.x][it.y] = 0;
		}
		if (ChessScoreUL[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUL[it.x][it.y] = -1;
		}
		else
		{
			ChessScoreUL[it.x][it.y] = 0;
		}
		if (ChessScoreUD[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUD[it.x][it.y] = -1;
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
			Opp_ChessScoreLR[it.x][it.y] = -1;
		}
		else
		{
			Opp_ChessScoreLR[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUR[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUR[it.x][it.y] = -1;
		}
		else
		{
			Opp_ChessScoreUR[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUL[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUL[it.x][it.y] = -1;
		}
		else
		{
			Opp_ChessScoreUL[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUD[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUD[it.x][it.y] = -1;
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
			Opp_ChessScoreLR[it.x][it.y] = -1;
		}
		else
		{
			Opp_ChessScoreLR[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUR[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUR[it.x][it.y] = -1;
		}
		else
		{
			Opp_ChessScoreUR[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUL[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUL[it.x][it.y] = -1;
		}
		else
		{
			Opp_ChessScoreUL[it.x][it.y] = 0;
		}
		if (Opp_ChessScoreUD[it.x][it.y] < WINCOUNT - 1)
		{
			Opp_ChessScoreUD[it.x][it.y] = -1;
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
			ChessScoreLR[it.x][it.y] = -1;
		}
		else
		{
			ChessScoreLR[it.x][it.y] = 0;
		}
		if (ChessScoreUR[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUR[it.x][it.y] = -1;
		}
		else
		{
			ChessScoreUR[it.x][it.y] = 0;
		}
		if (ChessScoreUL[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUL[it.x][it.y] = -1;
		}
		else
		{
			ChessScoreUL[it.x][it.y] = 0;
		}
		if (ChessScoreUD[it.x][it.y] < WINCOUNT - 1)
		{
			ChessScoreUD[it.x][it.y] = -1;
		}
		else
		{
			ChessScoreUD[it.x][it.y] = 0;
		}
	}
	temp3.clear();
	temp4.clear();
}
//第三轮是用来得到空格位在每个方向的真实最大棋子数
void CMFCFivePointsChessApp::ThirdAssessResult()
{
	int MyTag = GetFlag();
	int OppTag = BLACKCHESS;
	if (OppTag == MyTag)
	{
		OppTag = WHITECHESS;
	}
	int count = 0;
	for each (PointPos it in BestPoints)
	{
		Total_BestPoints.push_back(it);
		if (ChessScoreLR[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 1), count);
			ChessScoreLR[it.x][it.y] += count;
		}
		//上下
		if (ChessScoreUD[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 1), count);
			ChessScoreUD[it.x][it.y] += count;
		}
		//左上右下
		if (ChessScoreUL[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 1), count);
			ChessScoreUL[it.x][it.y] += count;
		}
		//左下右上
		if (ChessScoreUR[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 1), count);
			ChessScoreUR[it.x][it.y] += count;
		}

	}
	for each (PointPos it in Opp_BestPoints)
	{
		//左右
		if (Opp_ChessScoreLR[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 1), count);
			Opp_ChessScoreLR[it.x][it.y] += count;
		}
		//上下
		if (Opp_ChessScoreUD[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 1), count);
			Opp_ChessScoreUD[it.x][it.y] += count;
		}

		//左上右下
		if (Opp_ChessScoreUL[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 1), count);
			Opp_ChessScoreUL[it.x][it.y] += count;
		}

		//左下右上
		if (Opp_ChessScoreUR[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 1), count);
			Opp_ChessScoreUR[it.x][it.y] += count;
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
	for each (PointPos it in temp3)
	{
		//左右
		if (Opp_ChessScoreLR[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Left, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Right, 1), count);
			Opp_ChessScoreLR[it.x][it.y] += count;
		}
		//上下
		if (Opp_ChessScoreUD[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, Up, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, Down, 1), count);
			Opp_ChessScoreUD[it.x][it.y] += count;
		}

		//左上右下
		if (Opp_ChessScoreUL[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftUp, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightDown, 1), count);
			Opp_ChessScoreUL[it.x][it.y] += count;
		}

		//左下右上
		if (Opp_ChessScoreUR[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 1) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 2) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 3) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 4) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &OppTag, LeftDown, 5) + SearchOurPointsCount(&it.x, &it.y, &OppTag, RightUp, 1), count);
			Opp_ChessScoreUR[it.x][it.y] += count;
		}

	}
	for each (PointPos it in temp4)
	{
		Total_BestPoints.push_back(it);
		if (ChessScoreLR[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Left, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Right, 1), count);
			ChessScoreLR[it.x][it.y] += count;
		}
		//上下
		if (ChessScoreUD[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, Up, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, Down, 1), count);
			ChessScoreUD[it.x][it.y] += count;
		}
		//左上右下
		if (ChessScoreUL[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftUp, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightDown, 1), count);
			ChessScoreUL[it.x][it.y] += count;
		}
		//左下右上
		if (ChessScoreUR[it.x][it.y] >= 0)
		{
			count = 0;
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 1) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 5), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 2) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 4), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 3) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 3), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 4) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 2), count);
			count = max(SearchOurPointsCount(&it.x, &it.y, &MyTag, LeftDown, 5) + SearchOurPointsCount(&it.x, &it.y, &MyTag, RightUp, 1), count);
			ChessScoreUR[it.x][it.y] += count;
		}
	}
	temp3.clear();
	temp4.clear();
}
//第四轮是针对棋子数1 2 3这三种情况进行更进一步的甄别 比如通过这个方法可以把 2能变活3 和 3能冲4 设置为同一个优先级的
void CMFCFivePointsChessApp::FourthAssessResult()
{

	int TargetCount;//需要寻找到的棋子数 
	int MyTag;//标记当前寻找自己方的棋子类型
	int OurChessCount;//记录同一大方向的单方向寻找到的自己的棋子数 
	int FindDepth;//单方向寻找的深度 用于判断是否提前返回
	int count;//记录单向的得分
	int OurChessCount1;//记录同一大方向的单方向寻找到的自己的棋子数 
	int FindDepth1;//单方向寻找的深度 用于判断是否提前返回
	int count1;//记录单向的得分
	int SwitchCase;//用于决定分数处理方法 不同数字对应不同情况   如：两个分数相加 只取一个方向的分数 加分同时额外扣分
	for each (PointPos it in Total_BestPoints)
	{

		MyTag = GetFlag();
		//左上右下
		TargetCount = ChessScoreUL[it.x][it.y];
		if (TargetCount >= 1)
		{
			OurChessCount = 0;
			FindDepth = 5;
			OurChessCount1 = 0;
			FindDepth1 = 5;
			SwitchCase = 0;
			count = CalCulateFromChessCountForSomeDetail(&OurChessCount, &FindDepth, &it.x, &it.y, 0, false, false, MyTag, LeftUp, false, 5);
			count1 = CalCulateFromChessCountForSomeDetail(&OurChessCount1, &FindDepth1, &it.x, &it.y, 0, false, false, MyTag, RightDown, false, 5);
			count = min(1, count);
			count1 = min(1, count1);
			//一个方向就能够找到全部的目标棋子
			if (OurChessCount >= TargetCount || OurChessCount1 >= TargetCount)
			{
				//如果任意一边碰到墙 ps：不可能两边都碰墙
				if ((FindDepth >= 4 && OurChessCount == 0) || (FindDepth1 >= 4 && OurChessCount1 == 0))
				{
					//如果是前者碰到墙
					if (FindDepth >= 4 && OurChessCount == 0)
					{
						SwitchCase = 1;
					}
					//如果是后者碰到墙
					else
					{
						SwitchCase = 2;
					}
				}
				else
				{
					//有可能会找多 在找多的情况下依照多棋子的一个方向加分
					if (OurChessCount >= TargetCount)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
			}
			else
			{
				//能够找到全部的目标棋子
				if (OurChessCount + OurChessCount1 == TargetCount)
				{
					SwitchCase = 3;
				}
				//不能能够找到全部的目标棋子
				else
				{
					SwitchCase = 4;
				}
			}
			//根据SwitchCase来处理
			AddToScore(it.x, it.y, TargetCount, LeftUp, count, count1, SwitchCase, true);
		}
		//左右
		TargetCount = ChessScoreLR[it.x][it.y];
		if (TargetCount >= 1)
		{
			OurChessCount = 0;
			FindDepth = 5;
			OurChessCount1 = 0;
			FindDepth1 = 5;
			SwitchCase = 0;
			count = CalCulateFromChessCountForSomeDetail(&OurChessCount, &FindDepth, &it.x, &it.y, 0, false, false, MyTag, Left, false, 5);
			count1 = CalCulateFromChessCountForSomeDetail(&OurChessCount1, &FindDepth1, &it.x, &it.y, 0, false, false, MyTag, Right, false, 5);
			count = min(1, count);
			count1 = min(1, count1);
			if (OurChessCount >= TargetCount || OurChessCount1 >= TargetCount)
			{
				if ((FindDepth >= 4 && OurChessCount == 0) || (FindDepth1 >= 4 && OurChessCount1 == 0))
				{
					if (FindDepth >= 4 && OurChessCount == 0)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
				else
				{
					if (OurChessCount >= TargetCount)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
			}
			else
			{
				if (OurChessCount + OurChessCount1 == TargetCount)
				{
					SwitchCase = 3;
				}
				else
				{
					SwitchCase = 4;
				}
			}
			AddToScore(it.x, it.y, TargetCount, Left, count, count1, SwitchCase, true);
		}

		//右上左下
		TargetCount = ChessScoreUR[it.x][it.y];
		if (TargetCount >= 1)
		{
			OurChessCount = 0;
			FindDepth = 5;
			OurChessCount1 = 0;
			FindDepth1 = 5;
			SwitchCase = 0;
			count = CalCulateFromChessCountForSomeDetail(&OurChessCount, &FindDepth, &it.x, &it.y, 0, false, false, MyTag, LeftDown, false, 5);
			count1 = CalCulateFromChessCountForSomeDetail(&OurChessCount1, &FindDepth1, &it.x, &it.y, 0, false, false, MyTag, RightUp, false, 5);
			count = min(1, count);
			count1 = min(1, count1);
			if (OurChessCount >= TargetCount || OurChessCount1 >= TargetCount)
			{
				if ((FindDepth >= 4 && OurChessCount == 0) || (FindDepth1 >= 4 && OurChessCount1 == 0))
				{
					if (FindDepth >= 4 && OurChessCount == 0)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
				else
				{
					if (OurChessCount >= TargetCount)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
			}
			else
			{
				if (OurChessCount + OurChessCount1 == TargetCount)
				{
					SwitchCase = 3;
				}
				else
				{
					SwitchCase = 4;
				}
			}
			AddToScore(it.x, it.y, TargetCount, RightUp, count, count1, SwitchCase, true);
		}
		
		//上下
		TargetCount = ChessScoreUD[it.x][it.y];
		if (TargetCount >= 1)
		{
			OurChessCount = 0;
			FindDepth = 5;
			OurChessCount1 = 0;
			FindDepth1 = 5;
			SwitchCase = 0;
			count = CalCulateFromChessCountForSomeDetail(&OurChessCount, &FindDepth, &it.x, &it.y, 0, false, false, MyTag, Up, false, 5);
			count1 = CalCulateFromChessCountForSomeDetail(&OurChessCount1, &FindDepth1, &it.x, &it.y, 0, false, false, MyTag, Down, false, 5);
			count = min(1, count);
			count1 = min(1, count1);
			if (OurChessCount >= TargetCount || OurChessCount1 >= TargetCount)
			{
				if ((FindDepth >= 4 && OurChessCount == 0) || (FindDepth1 >= 4 && OurChessCount1 == 0))
				{
					if (FindDepth >= 4 && OurChessCount == 0)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
				else
				{
					if (OurChessCount >= TargetCount)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
			}
			else
			{
				if (OurChessCount + OurChessCount1 == TargetCount)
				{
					SwitchCase = 3;
				}
				else
				{
					SwitchCase = 4;
				}
			}
			AddToScore(it.x, it.y, TargetCount, Up, count, count1, SwitchCase, true);
		}
	
		/*--------------------处理敌方-------------------------*/
		MyTag = BLACKCHESS;
		if (MyTag == GetFlag())
		{
			MyTag = WHITECHESS;
		}
		//左右
		TargetCount = Opp_ChessScoreLR[it.x][it.y];
		if (TargetCount >= 1)
		{
			OurChessCount = 0;
			FindDepth = 5;
			OurChessCount1 = 0;
			FindDepth1 = 5;
			SwitchCase = 0;
			count = CalCulateFromChessCountForSomeDetail(&OurChessCount, &FindDepth, &it.x, &it.y, 0, false, false, MyTag, Left, false, 5);
			count1 = CalCulateFromChessCountForSomeDetail(&OurChessCount1, &FindDepth1, &it.x, &it.y, 0, false, false, MyTag, Right, false, 5);
			count = min(1, count);
			count1 = min(1, count1);
			if (OurChessCount >= TargetCount || OurChessCount1 >= TargetCount)
			{
				if ((FindDepth >= 4 && OurChessCount == 0) || (FindDepth1 >= 4 && OurChessCount1 == 0))
				{
					if (FindDepth >= 4 && OurChessCount == 0)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
				else
				{
					if (OurChessCount >= TargetCount)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
			}
			else
			{
				if (OurChessCount + OurChessCount1 == TargetCount)
				{
					SwitchCase = 3;
				}
				else
				{
					SwitchCase = 4;
				}
			}
			AddToScore(it.x, it.y, TargetCount, Left, count, count1, SwitchCase, false);
		}
	
		//左上右下
		TargetCount = Opp_ChessScoreUL[it.x][it.y];
		if (TargetCount >= 1)
		{
			
			OurChessCount = 0;
			FindDepth = 5;
			OurChessCount1 = 0;
			FindDepth1 = 5;
			SwitchCase = 0;
			count = CalCulateFromChessCountForSomeDetail(&OurChessCount, &FindDepth, &it.x, &it.y, 0, false, false, MyTag, LeftUp, false, 5);
			count1 = CalCulateFromChessCountForSomeDetail(&OurChessCount1, &FindDepth1, &it.x, &it.y, 0, false, false, MyTag, RightDown, false, 5);
			count = min(1, count);
			count1 = min(1, count1);
			if (OurChessCount >= TargetCount || OurChessCount1 >= TargetCount)
			{
				if ((FindDepth >= 4 && OurChessCount == 0) || (FindDepth1 >= 4 && OurChessCount1 == 0))
				{
					if (FindDepth >= 4 && OurChessCount == 0)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
				else
				{
					if (OurChessCount >= TargetCount)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
			}
			else
			{
				if (OurChessCount + OurChessCount1 == TargetCount)
				{
					SwitchCase = 3;
				}
				else
				{
					SwitchCase = 4;
				}
			}
			AddToScore(it.x, it.y, TargetCount, LeftUp, count, count1, SwitchCase, false);
		}
		
		//右上左下
		TargetCount = Opp_ChessScoreUR[it.x][it.y];
		if (TargetCount >= 1)
		{
			OurChessCount = 0;
			FindDepth = 5;
			OurChessCount1 = 0;
			FindDepth1 = 5;
			SwitchCase = 0;
			count = CalCulateFromChessCountForSomeDetail(&OurChessCount, &FindDepth, &it.x, &it.y, 0, false, false, MyTag, LeftDown, false, 5);
			count1 = CalCulateFromChessCountForSomeDetail(&OurChessCount1, &FindDepth1, &it.x, &it.y, 0, false, false, MyTag, RightUp, false, 5);
			count = min(1, count);
			count1 = min(1, count1);
			if (OurChessCount >= TargetCount || OurChessCount1 >= TargetCount)
			{
				if ((FindDepth >= 4 && OurChessCount == 0) || (FindDepth1 >= 4 && OurChessCount1 == 0))
				{
					if (FindDepth >= 4 && OurChessCount == 0)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
				else
				{
					if (OurChessCount >= TargetCount)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
			}
			else
			{
				if (OurChessCount + OurChessCount1 == TargetCount)
				{
					SwitchCase = 3;
				}
				else
				{
					SwitchCase = 4;
				}
			}
			AddToScore(it.x, it.y, TargetCount, RightUp, count, count1, SwitchCase, false);
		}
		
		//上下
		TargetCount = Opp_ChessScoreUD[it.x][it.y];
		if (TargetCount >= 1)
		{
			OurChessCount = 0;
			FindDepth = 5;
			OurChessCount1 = 0;
			FindDepth1 = 5;
			SwitchCase = 0;
			count = CalCulateFromChessCountForSomeDetail(&OurChessCount, &FindDepth, &it.x, &it.y, 0, false, false, MyTag, Up, false, 5);
			count1 = CalCulateFromChessCountForSomeDetail(&OurChessCount1, &FindDepth1, &it.x, &it.y, 0, false, false, MyTag, Down, false, 5);
			count = min(1, count);
			count1 = min(1, count1);
			if (OurChessCount >= TargetCount || OurChessCount1 >= TargetCount)
			{
				if ((FindDepth >= 4 && OurChessCount == 0) || (FindDepth1 >= 4 && OurChessCount1 == 0))
				{
					if (FindDepth >= 4 && OurChessCount == 0)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
				else
				{
					if (OurChessCount >= TargetCount)
					{
						SwitchCase = 1;
					}
					else
					{
						SwitchCase = 2;
					}
				}
			}
			else
			{
				if (OurChessCount + OurChessCount1 == TargetCount)
				{
					SwitchCase = 3;
				}
				else
				{
					SwitchCase = 4;
				}
			}
			AddToScore(it.x, it.y, TargetCount, Up, count, count1, SwitchCase, false);
		}
	
	}


}

//第五轮对剩余节点进行积分筛选 就是加空格分 加连续分 减碰壁碰别人的分 让单个方向对自己更有利的分数高
void CMFCFivePointsChessApp::FifthAssessResult()
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
	//先把点扩大32倍
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
	if (BestScore >= Opp_BestScore-2)
	{
		for each (PointPos it in BestPoints)
		{
			if (ChessScoreLR[it.x][it.y] >= BestScore-2 || ChessScoreUR[it.x][it.y] >= BestScore-2 || ChessScoreUL[it.x][it.y] >= BestScore-2 || ChessScoreUD[it.x][it.y] >= BestScore-2)
			{
				temp.push_back(it);
			}
		}
	}
	if (BestScore-2 <= Opp_BestScore)
	{
		for each (PointPos it in Opp_BestPoints)
		{
			if (Opp_ChessScoreLR[it.x][it.y] >= Opp_BestScore-2 || Opp_ChessScoreUR[it.x][it.y] >= Opp_BestScore-2 || Opp_ChessScoreUL[it.x][it.y] >= Opp_BestScore-2 || Opp_ChessScoreUD[it.x][it.y] >= Opp_BestScore-2)
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
//第六轮对所有的结果进行求和 并且算出总分最高的点
void CMFCFivePointsChessApp::SixthAssessResult()
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
	}
	AddResultInResult();
	int BestScore=0;
	
	/*-------------删去重复元素-------------------------------*/
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

	/*-------------------结束删去重复元素-------------------------------------*/
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
//第六轮的前置工作 把单个方向中负数的设置为0 防止太大造成干扰
void CMFCFivePointsChessApp::SetScoreArrayZero()
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			if(ChessScoreUD[i][j]<0)
			ChessScoreUD[i][j] = 0;
			if(ChessScoreUR[i][j]<0)
			ChessScoreUR[i][j] = 0;
			if (ChessScoreUL[i][j] < 0)
			ChessScoreUL[i][j] = 0;
			if(ChessScoreLR[i][j]<0)
			ChessScoreLR[i][j] = 0;

			if (Opp_ChessScoreUD[i][j] < 0)
				Opp_ChessScoreUD[i][j] = 0;
			if (Opp_ChessScoreUR[i][j] < 0)
				Opp_ChessScoreUR[i][j] = 0;
			if (Opp_ChessScoreUL[i][j] < 0)
				Opp_ChessScoreUL[i][j] = 0;
			if (Opp_ChessScoreLR[i][j] < 0)
				Opp_ChessScoreLR[i][j] = 0;
		}
	}
}
//这是第四轮的相关加分操作 太多了把它封装到函数里
void CMFCFivePointsChessApp::AddToScore(const int& x,const int& y,const int& TargetCount, const Direction& direction, const int& count, const int& count1,const int& Choose,bool myself)
{
	if (myself)
	{
		switch (direction)
		{
		case Left:
			switch (Choose)
			{
			case 1:
				if (TargetCount == 3)
				{

					ChessScoreLR[x][y] = ChessScoreLR[x][y] * 2 - count - 1;
				}
				else
				{
					ChessScoreLR[x][y] = ChessScoreLR[x][y] * 2 - count;
				}
				break;

			case 2:
				if (TargetCount == 3)
				{
					ChessScoreLR[x][y] = ChessScoreLR[x][y] * 2 - count1 - 1;
				}
				else
				{
					ChessScoreLR[x][y] = ChessScoreLR[x][y] * 2 - count1;
				}

				break;
			case 3:
				if (TargetCount == 3)
				{
					ChessScoreLR[x][y] = ChessScoreLR[x][y] * 2 - count1 - count - 1;
				}
				else
				{
					ChessScoreLR[x][y] = ChessScoreLR[x][y] * 2  -count - count1;
				}
				break;
			case 4:
				if (TargetCount == 3)
				{
					ChessScoreLR[x][y] = ChessScoreLR[x][y] * 2 + max(-2, -count1 - count - 1) - 1;
				}
				else
				{
					ChessScoreLR[x][y] = ChessScoreLR[x][y] * 2 + max(-2, -count1 - count - 1);
				}
				break;
			}
			break;
		case LeftUp:
			switch (Choose)
			{
			case 1:
				if (TargetCount == 3)
				{

					ChessScoreUL[x][y] = ChessScoreUL[x][y] * 2 - count - 1;
				}
				else
				{
					ChessScoreUL[x][y] = ChessScoreUL[x][y] * 2 - count;
				}
				break;

			case 2:
				if (TargetCount == 3)
				{
					ChessScoreUL[x][y] = ChessScoreUL[x][y] * 2 - count1 - 1;
				}
				else
				{
					ChessScoreUL[x][y] = ChessScoreUL[x][y] * 2 - count1;
				}

				break;
			case 3:
				if (TargetCount == 3)
				{
					ChessScoreUL[x][y] = ChessScoreUL[x][y] * 2 - count1 - count - 1;
				}
				else
				{
					ChessScoreUL[x][y] = ChessScoreUL[x][y] * 2  -count - count1;
				}
				break;
			case 4:
				if (TargetCount == 3)
				{
					ChessScoreUL[x][y] = ChessScoreUL[x][y] * 2 + max(-2, -count1 - count - 1) - 1;
				}
				else
				{
					ChessScoreUL[x][y] = ChessScoreUL[x][y] * 2 + max(-2, -count1 - count - 1);
				}
				break;
			}
			break;
		case RightUp:
			switch (Choose)
			{
			case 1:
				if (TargetCount == 3)
				{

					ChessScoreUR[x][y] = ChessScoreUR[x][y] * 2 - count - 1;
				}
				else
				{
					ChessScoreUR[x][y] = ChessScoreUR[x][y] * 2 - count;
				}
				break;

			case 2:
				if (TargetCount == 3)
				{
					ChessScoreUR[x][y] = ChessScoreUR[x][y] * 2 - count1 - 1;
				}
				else
				{
					ChessScoreUR[x][y] = ChessScoreUR[x][y] * 2 - count1;
				}

				break;
			case 3:
				if (TargetCount == 3)
				{
					ChessScoreUR[x][y] = ChessScoreUR[x][y] * 2 - count1 - count - 1;
				}
				else
				{
					ChessScoreUR[x][y] = ChessScoreUR[x][y] * 2 -count - count1;
				}
				break;
			case 4:
				if (TargetCount == 3)
				{
					ChessScoreUR[x][y] = ChessScoreUR[x][y] * 2 + max(-2, -count1 - count - 1) - 1;
				}
				else
				{
					ChessScoreUR[x][y] = ChessScoreUR[x][y] * 2 + max(-2, -count1 - count - 1);
				}
				break;
			}
			break;
		case Up:
			switch (Choose)
			{
			case 1:
				if (TargetCount == 3)
				{

					ChessScoreUD[x][y] = ChessScoreUD[x][y] * 2 - count - 1;
				}
				else
				{
					ChessScoreUD[x][y] = ChessScoreUD[x][y] * 2 - count;
				}
				break;

			case 2:
				if (TargetCount == 3)
				{
					ChessScoreUD[x][y] = ChessScoreUD[x][y] * 2 - count1 - 1;
				}
				else
				{
					ChessScoreUD[x][y] = ChessScoreUD[x][y] * 2 - count1;
				}

				break;
			case 3:
				if (TargetCount == 3)
				{
					ChessScoreUD[x][y] = ChessScoreUD[x][y] * 2 - count1 - count - 1;
				}
				else
				{
					ChessScoreUD[x][y] = ChessScoreUD[x][y] * 2 -count - count1;
				}
				break;
			case 4:
				if (TargetCount == 3)
				{
					ChessScoreUD[x][y] = ChessScoreUD[x][y] * 2 +max(-2,- count1 - count -1) - 1;
				}
				else
				{
					ChessScoreUD[x][y] = ChessScoreUD[x][y] * 2+ max(-2, -count1 - count - 1);
				}
				break;
			}
			break;
		}
	}
	else
	{
	switch (direction)
	{
	case Left:
		switch (Choose)
		{
		case 1:
			if (TargetCount == 3)
			{

				Opp_ChessScoreLR[x][y] = Opp_ChessScoreLR[x][y] * 2 - count - 1;
			}
			else
			{
				Opp_ChessScoreLR[x][y] = Opp_ChessScoreLR[x][y] * 2 - count;
			}
			break;

		case 2:
			if (TargetCount == 3)
			{
				Opp_ChessScoreLR[x][y] = Opp_ChessScoreLR[x][y] * 2 - count1 - 1;
			}
			else
			{
				Opp_ChessScoreLR[x][y] = Opp_ChessScoreLR[x][y] * 2 - count1;
			}

			break;
		case 3:
			if (TargetCount == 3)
			{
				Opp_ChessScoreLR[x][y] = Opp_ChessScoreLR[x][y] * 2 - count1 - count - 1;
			}
			else
			{
				Opp_ChessScoreLR[x][y] = Opp_ChessScoreLR[x][y] * 2 -count - count1;
			}
			break;
		case 4:
			if (TargetCount == 3)
			{
				Opp_ChessScoreLR[x][y] = Opp_ChessScoreLR[x][y] * 2 + max(-2, -count1 - count - 1) - 1;
			}
			else
			{
				Opp_ChessScoreLR[x][y] = Opp_ChessScoreLR[x][y] * 2 + max(-2, -count1 - count - 1);
			}
			break;
		}
		break;
	case LeftUp:
		switch (Choose)
		{
		case 1:
			if (TargetCount == 3)
			{

				Opp_ChessScoreUL[x][y] = Opp_ChessScoreUL[x][y] * 2 - count - 1;
			}
			else
			{
				Opp_ChessScoreUL[x][y] = Opp_ChessScoreUL[x][y] * 2 - count;
			}
			break;

		case 2:
			if (TargetCount == 3)
			{
				Opp_ChessScoreUL[x][y] = Opp_ChessScoreUL[x][y] * 2 - count1 - 1;
			}
			else
			{
				Opp_ChessScoreUL[x][y] = Opp_ChessScoreUL[x][y] * 2 - count1;
			}

			break;
		case 3:
			if (TargetCount == 3)
			{
				Opp_ChessScoreUL[x][y] = Opp_ChessScoreUL[x][y] * 2 - count1 - count - 1;
			}
			else
			{
				Opp_ChessScoreUL[x][y] = Opp_ChessScoreUL[x][y] * 2 -count - count1;
			}
			break;
		case 4:
			if (TargetCount == 3)
			{
				Opp_ChessScoreUL[x][y] = Opp_ChessScoreUL[x][y] * 2 + max(-2, -count1 - count - 1) - 1;
			}
			else
			{
				Opp_ChessScoreUL[x][y] = Opp_ChessScoreUL[x][y] * 2 + max(-2, -count1 - count - 1);
			}
			break;
		}
		break;
	case RightUp:
		switch (Choose)
		{
		case 1:
			if (TargetCount == 3)
			{

				Opp_ChessScoreUR[x][y] = Opp_ChessScoreUR[x][y] * 2 - count - 1;
			}
			else
			{
				Opp_ChessScoreUR[x][y] = Opp_ChessScoreUR[x][y] * 2 - count;
			}
			break;

		case 2:
			if (TargetCount == 3)
			{
				Opp_ChessScoreUR[x][y] = Opp_ChessScoreUR[x][y] * 2 - count1 - 1;
			}
			else
			{
				Opp_ChessScoreUR[x][y] = Opp_ChessScoreUR[x][y] * 2 - count1;
			}

			break;
		case 3:
			if (TargetCount == 3)
			{
				Opp_ChessScoreUR[x][y] = Opp_ChessScoreUR[x][y] * 2 - count1 - count - 1;
			}
			else
			{
				Opp_ChessScoreUR[x][y] = Opp_ChessScoreUR[x][y] * 2 -count - count1;
			}
			break;
		case 4:
			if (TargetCount == 3)
			{
				Opp_ChessScoreUR[x][y] = Opp_ChessScoreUR[x][y] * 2 + max(-2, -count1 - count - 1) - 1;
			}
			else
			{
				Opp_ChessScoreUR[x][y] = Opp_ChessScoreUR[x][y] * 2 + max(-2, -count1 - count - 1);
			}
			break;
		}
		break;
	case Up:
		switch (Choose)
		{
		case 1:
			if (TargetCount == 3)
			{

				Opp_ChessScoreUD[x][y] = Opp_ChessScoreUD[x][y] * 2 - count - 1;
			}
			else
			{
				Opp_ChessScoreUD[x][y] = Opp_ChessScoreUD[x][y] * 2 - count;
			}
			break;

		case 2:
			if (TargetCount == 3)
			{
				Opp_ChessScoreUD[x][y] = Opp_ChessScoreUD[x][y] * 2 - count1 - 1;
			}
			else
			{
				Opp_ChessScoreUD[x][y] = Opp_ChessScoreUD[x][y] * 2 - count1;
			}

			break;
		case 3:
			if (TargetCount == 3)
			{
				Opp_ChessScoreUD[x][y] = Opp_ChessScoreUD[x][y] * 2 - count1 - count - 1;
			}
			else
			{
				Opp_ChessScoreUD[x][y] = Opp_ChessScoreUD[x][y] * 2 -count - count1;
			}
			break;
		case 4:
			if (TargetCount == 3)
			{
				Opp_ChessScoreUD[x][y] = Opp_ChessScoreUD[x][y] * 2 + max(-2, -count1 - count - 1) - 1;
			}
			else
			{
				Opp_ChessScoreUD[x][y] = Opp_ChessScoreUD[x][y] * 2 + max(-2, -count1 - count - 1);
			}
			break;
		}
		break;
	}
	}
	
	
}


//通过棋子数目来进行判断
//1说明大于等于4 直接下一步
//0说明最大小于4 要继续判断
int CMFCFivePointsChessApp::JudgeFromChessCount()
{
	int result=0;
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
	//随便一个大于4
	if (BestScore >= 4 || Opp_BestScore >= 4)
	{
		result = 1;
		if (BestScore >= Opp_BestScore)
		{
			/*===============改变情绪================*/
			AI_Station = happy;
			/*--------------------我方大优势---------------*/
			
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
			/*===============改变情绪================*/
			AI_Station = sad;
			/*--------------------敌人大优势---------------*/
			for each (PointPos it in Opp_BestPoints)
			{
				//只要有一个边的子数和最好数目一样加入vector
				if (Opp_ChessScoreLR[it.x][it.y] == Opp_BestScore || Opp_ChessScoreUR[it.x][it.y] == Opp_BestScore || Opp_ChessScoreUL[it.x][it.y] == Opp_BestScore || Opp_ChessScoreUD[it.x][it.y] == Opp_BestScore)
				{
					Opp_temp.push_back(it);
				}
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
		return result;
	}
	for each (PointPos it in temp)
	{
		BestPoints.push_back(it);
	}
	for each (PointPos it in Opp_temp)
	{
		BestPoints.push_back(it);
	}
	temp.clear();
	Opp_temp.clear();
	return result;
}
//在棋子为1 2 3个时 如果我方最大棋子大于等于对方 优先采取我方点 就是优先进攻
int CMFCFivePointsChessApp::JudgeFromChessCount2(const int& choose,const int& count)
{
	std::vector<PointPos> temp;
	std::vector<PointPos> Opp_temp;
	int BestScore = 0;
	int Opp_BestScore = 0;
	int ChooseCount;
	int BMax = max(BestScore, Opp_BestScore);
	switch (BMax)
	{
	case 0:case 1:
		ChooseCount = 8;
		break;
	case 2:
		ChooseCount = 7;
		break;
	case 3:
		ChooseCount = 5;
		break;
	case 4:
		ChooseCount = 4;
		break;
	case 5:
		ChooseCount = 2;
		break;
	default:
		ChooseCount = 2;
		break;
	}
	//分别求最大值
	for each (PointPos it in BestPoints)
	{
		BestScore = max(ChessScoreUD[it.x][it.y], max(ChessScoreUL[it.x][it.y], max(ChessScoreUR[it.x][it.y], max(ChessScoreLR[it.x][it.y], BestScore))));

	}
	for each (PointPos it in Opp_BestPoints)
	{
		Opp_BestScore = max(Opp_ChessScoreLR[it.x][it.y], max(Opp_ChessScoreUD[it.x][it.y], max(Opp_ChessScoreUL[it.x][it.y], max(Opp_ChessScoreUR[it.x][it.y], Opp_BestScore))));
	}
	
	if (choose <= 1)
	{
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
	}
	//高级电脑内容
	//选择count个棋子数较大的空位点 分开敌我保存到temp temp1
	if (choose == 2)
	{

		int haveSelectCount = 0;//记录已经选择的数目
		int tempBest = BestScore;
		int Opp_tempBest = Opp_BestScore;
		while (tempBest>0||Opp_tempBest>0)
		{
			if (tempBest >= Opp_tempBest)
			{
				for each (PointPos it in BestPoints)
				{
					if (haveSelectCount >= ChooseCount)
					{
						break;
					}
						//只要没够数量就一直加
						if (ChessScoreLR[it.x][it.y] == tempBest || ChessScoreUR[it.x][it.y] == tempBest || ChessScoreUL[it.x][it.y] == tempBest || ChessScoreUD[it.x][it.y] == tempBest)
						{
							temp.push_back(it);
							haveSelectCount++;
						}
					

				}
				tempBest--;
			}
			else
			{
				for each (PointPos it in Opp_BestPoints)
				{
					if (haveSelectCount >= ChooseCount)
					{
						break;
					}

						if (Opp_ChessScoreLR[it.x][it.y] == Opp_tempBest || Opp_ChessScoreUR[it.x][it.y] == Opp_tempBest || Opp_ChessScoreUL[it.x][it.y] == Opp_tempBest || Opp_ChessScoreUD[it.x][it.y] == Opp_BestScore)
						{
							Opp_temp.push_back(it);
							haveSelectCount++;
						}
					
				}
				Opp_tempBest--;
			}
			if (haveSelectCount >= ChooseCount)
			{
				break;
			}
		}
		//while结束
	}

	if (!temp.empty() || !Opp_temp.empty())
	{
		BestPoints.clear();
		Opp_BestPoints.clear();
	}
	else
	{
		return 0;
	}
	int haveSelectCount = 0;
	for each (PointPos it in temp)
	{
		BestPoints.push_back(it);
		haveSelectCount++;
	}
	for each (PointPos it in Opp_temp)
	{
		Opp_BestPoints.push_back(it);
		haveSelectCount++;
	}
	temp.clear();
	Opp_temp.clear();
	
	return haveSelectCount;
}
//这是第三轮配套的算法 算出单个方向的同类型棋子
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
/*--------------------------------------------第五轮筛选的配套算法-------------------------------------------*/
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



/*--------------------------------------------第一轮筛选的配套算法-------------------------------------------*/
//这个是判断单个方向非对手的连续空位 遇到对手就停止
int CMFCFivePointsChessApp::JudgeChessCanWin(int *x, int *y, const int& tag, const Direction& direction, int Depth)
{
	int x1 = *x;
	int y1 = *y;
	if (Depth <= 0)
	{
		return 0;
	}
	if (Depth == 5)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return JudgeChessCanWin(&x1, &y1, tag, direction, --Depth);
		}
		else
		{
			return 0;
		}
	}
	if (ChessTag[x1][y1] == tag || ChessTag[x1][y1] == NULLCHESS)
	{
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			return JudgeChessCanWin(&x1, &y1, tag, direction, --Depth) + 1;
		}
		else
		{
			return 1;
		}

	}
	else
	{
		return 0;
	}

}


/*--------------------------------------------第四轮筛选的配套算法-------------------------------------------*/
//这个用于得到空格附近真实棋子数目并扩大两倍后进行进一步判断 把1格、2格和3格细化成0 1 2 3 4 5分 
//如果一边的OurChessCount已经等于TargetCount 那么只要另外一边不是下面碰到墙和敌人的情况都可以不作惩罚
//如果OurChessCount==0 FindDepth=4 那么就说明碰到敌人或者墙 要在外部手动进行减分 
//如果OurChessCount==0 FindDepth<4 那么就是碰到一个空格后碰墙或者敌人返回 算出来的分是不需要减去的 （算法里碰墙那里设置了把层数后移）
int CMFCFivePointsChessApp::CalCulateFromChessCountForSomeDetail(int * OurChessCount ,int * FindDepth, int * x, int * y,int tempEmptyCount,bool FindEmptyLastRound,bool FindEmpty,const int& MyTag,const Direction& direction,bool FindMe ,int Depth)
{
	*FindDepth = Depth;
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
			return CalCulateFromChessCountForSomeDetail(OurChessCount,FindDepth,&x1,&y1,0,false,false,MyTag,direction,false,--Depth);
		}
		else
		{
			(*FindDepth)=4;
			*OurChessCount = 0;
			return 1;
		}

	}
	if (ChessTag[x1][y1]== MyTag)
	{
		//遇到自己
		(*OurChessCount) += 1;
		if (CalculatePointDiretion(&x1, &y1, direction))
		{
			if (FindMe)
			{
				return CalCulateFromChessCountForSomeDetail(OurChessCount, FindDepth, &x1, &y1, 0, false,FindEmpty, MyTag, direction, true, --Depth)+tempEmptyCount;
			}
			else
			{
				return CalCulateFromChessCountForSomeDetail(OurChessCount, FindDepth, &x1, &y1, 0, false, FindEmpty,MyTag, direction, true, --Depth);
			}
		
		}
		else
		{
			if (FindMe)
			{
				if (Depth > 1)
				{
					(*FindDepth) -= 1;
					//扣一分
					return 1+tempEmptyCount;
				}
				else
				{
					return tempEmptyCount;
				}
			}
			else
			{
				if (Depth > 1)
				{
					(*FindDepth) -= 1;
					//扣一分
					return 1;
				}
				else
				{
					return 0;
				}
			}
			
			
		}
		
	}
	else
	{
		if (ChessTag[x1][y1] == NULLCHESS)
		{
			//空格
			if (CalculatePointDiretion(&x1, &y1, direction))
			{
				if (FindEmpty)
				{
					return 0;
				}
				else
				{
					if (FindMe)
					{
						tempEmptyCount += 1;
						return CalCulateFromChessCountForSomeDetail(OurChessCount, FindDepth, &x1, &y1,tempEmptyCount,true,true,  MyTag, direction, FindMe, --Depth);
					}
					else
					{
						return CalCulateFromChessCountForSomeDetail(OurChessCount, FindDepth, &x1, &y1, 0,true,true, MyTag, direction, FindMe, --Depth)+1;
					}
				}
			}
			else
			{
				(*FindDepth) -= 1;
				return 0;
			}
		}
		else
		{
			//遇到对手
			if (FindEmptyLastRound)
			{
				return 0;
			}
			else
			{
				return 1;
			}

		}
	}

}

//这种是除非遇到敌人棋子不然就一直递归
//这个是判断5格子之内自己的棋子数 中途不能遇到对手棋子
//就是获得5格内连续的自己的棋子
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

//判断选择框前进一格是否会出界
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
//技能1 实现
void CMFCFivePointsChessApp::Skill1Effect(int x,int y)
{
	int x1 = x;
	int y1 = y;
	if (ChessTag[x][y] == BLACKCHESS)
	{
		ChessTag[x][y] = WHITECHESS;
	}
	else
	{
		if (ChessTag[x][y] == WHITECHESS)
		{
			ChessTag[x][y] = BLACKCHESS;
		}
	}
	if (JudgeWin(x, y))
	{
		ChessTag[x][y] = NULLCHESS;
	}
	while (CalculatePointDiretion(&x, &y, selectDirection))
	{
	
		if (ChessTag[x][y] == BLACKCHESS)
		{
			ChessTag[x][y] = WHITECHESS;
		}
		else
		{
			if (ChessTag[x][y] == WHITECHESS)
			{
				ChessTag[x][y] = BLACKCHESS;
			}
		}
		if (JudgeWin(x, y))
		{
			ChessTag[x][y] = NULLCHESS;
		}
	}
	x = x1;
	y = y1;
	while (CalculatePointDiretion(&x, &y, (Direction)(selectDirection+4)))
	{
		if (ChessTag[x][y] == BLACKCHESS)
		{
			ChessTag[x][y] = WHITECHESS;
		}
		else
		{
			if (ChessTag[x][y] == WHITECHESS)
			{
				ChessTag[x][y] = BLACKCHESS;
			}
		}
		if (JudgeWin(x, y))
		{
			ChessTag[x][y] = NULLCHESS;
		}
	}
}
//技能2 实现
void CMFCFivePointsChessApp::Skill2Effect()
{
	int x, y;
	if (!isSelectTimeRect)
	{
		x = TimeRectPoint[0];
		y = TimeRectPoint[1];
		int x1, y1;
		//0
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, LeftUp))
		{
			SelectChessTimeRect[0] = ChessTag[x1][y1];
		}
		else
		{
			SelectChessTimeRect[0] = NULLCHESS;
		}
		//1
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Up))
		{
			SelectChessTimeRect[1] = ChessTag[x1][y1];
		}
		else
		{
			SelectChessTimeRect[1] = NULLCHESS;
		}
		//2
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, RightUp))
		{
			SelectChessTimeRect[2] = ChessTag[x1][y1];
		}
		else
		{
			SelectChessTimeRect[2] = NULLCHESS;
		}
		//3
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Left))
		{
			SelectChessTimeRect[3] = ChessTag[x1][y1];
		}
		else
		{
			SelectChessTimeRect[3] = NULLCHESS;
		}
		//4

		SelectChessTimeRect[4] = ChessTag[x][y];

		//5
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Right))
		{
			SelectChessTimeRect[5] = ChessTag[x1][y1];
		}
		else
		{
			SelectChessTimeRect[5] = NULLCHESS;
		}
		//6
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, LeftDown))
		{
			SelectChessTimeRect[6] = ChessTag[x1][y1];
		}
		else
		{
			SelectChessTimeRect[6] = NULLCHESS;

		}
		//7
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Down))
		{
			SelectChessTimeRect[7] = ChessTag[x1][y1];
		}
		else
		{
			SelectChessTimeRect[7] = NULLCHESS;
		}
		//8
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, RightDown))
		{
			SelectChessTimeRect[8] = ChessTag[x1][y1];
		}
		else
		{
			SelectChessTimeRect[8] = NULLCHESS;
		}
	}
	else
	{
		x = TargetTimeRectPoint[0];
		y = TargetTimeRectPoint[1];
		int x1, y1;
		//0
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, LeftUp))
		{
			TargetSelectChessTimeRect[0] = ChessTag[x1][y1];
			ChessTag[x1][y1] = SelectChessTimeRect[0];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		else
		{
			TargetSelectChessTimeRect[0] = NULLCHESS;
		}
		
		//1
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Up))
		{
			TargetSelectChessTimeRect[1] = ChessTag[x1][y1];
			ChessTag[x1][y1] = SelectChessTimeRect[1];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		else
		{
			TargetSelectChessTimeRect[1] = NULLCHESS;
		}
		
		//2
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, RightUp))
		{
			TargetSelectChessTimeRect[2] = ChessTag[x1][y1];
			ChessTag[x1][y1] = SelectChessTimeRect[2];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		else
		{
			TargetSelectChessTimeRect[2] = NULLCHESS;
		}
		
		//3
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Left))
		{
			TargetSelectChessTimeRect[3] = ChessTag[x1][y1];
			ChessTag[x1][y1] = SelectChessTimeRect[3];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		else
		{
			TargetSelectChessTimeRect[3] = NULLCHESS;
		}
		
		//4

		TargetSelectChessTimeRect[4] = ChessTag[x][y];

		ChessTag[x][y] = SelectChessTimeRect[4];
		if (JudgeWin(x, y))
		{
			ChessTag[x][y] = NULLCHESS;
		}
		//5
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Right))
		{
			TargetSelectChessTimeRect[5] = ChessTag[x1][y1];
			ChessTag[x1][y1] = SelectChessTimeRect[5];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		else
		{
			TargetSelectChessTimeRect[5] = NULLCHESS;
		}
		
		//6
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, LeftDown))
		{
			TargetSelectChessTimeRect[6] = ChessTag[x1][y1];
			ChessTag[x1][y1] = SelectChessTimeRect[6];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		else
		{
			TargetSelectChessTimeRect[6] = NULLCHESS;

		}
		
		//7
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Down))
		{
			TargetSelectChessTimeRect[7] = ChessTag[x1][y1];
		}
		else
		{
			TargetSelectChessTimeRect[7] = NULLCHESS;
		}
		ChessTag[x1][y1] = SelectChessTimeRect[7];
		if (JudgeWin(x1, y1))
		{
			ChessTag[x1][y1] = NULLCHESS;
		}
		//8
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, RightDown))
		{
			TargetSelectChessTimeRect[8] = ChessTag[x1][y1];
		}
		else
		{
			TargetSelectChessTimeRect[8] = NULLCHESS;
		}
		ChessTag[x1][y1] = SelectChessTimeRect[8];
		if (JudgeWin(x1, y1))
		{
			ChessTag[x1][y1] = NULLCHESS;
		}

		x = TimeRectPoint[0];
		y = TimeRectPoint[1];
		//写回去原来的位置
		//0
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, LeftUp))
		{
			 ChessTag[x1][y1]=TargetSelectChessTimeRect[0];
			 if (JudgeWin(x1, y1))
			 {
				 ChessTag[x1][y1] = NULLCHESS;
			 }
		}
		
		//1
		
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Up))
		{
			 ChessTag[x1][y1]=TargetSelectChessTimeRect[1];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		

	
		//2
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, RightUp))
		{
			ChessTag[x1][y1] = TargetSelectChessTimeRect[2];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		//3
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Left))
		{
			ChessTag[x1][y1] = TargetSelectChessTimeRect[3];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		
		
		//4

		 ChessTag[x][y]= TargetSelectChessTimeRect[4];
		if (JudgeWin(x, y))
		{
			ChessTag[x][y] = NULLCHESS;
		}
		//5
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Right))
		{
			ChessTag[x1][y1] = TargetSelectChessTimeRect[5];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		
	
		//6
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, LeftDown))
		{
			ChessTag[x1][y1] = TargetSelectChessTimeRect[6];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		
		
		//7
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, Down))
		{
			ChessTag[x1][y1] = TargetSelectChessTimeRect[7];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		
		
		//8
		x1 = x, y1 = y;
		if (CalculatePointDiretion(&x1, &y1, RightDown))
		{
			ChessTag[x1][y1] = TargetSelectChessTimeRect[8];
			if (JudgeWin(x1, y1))
			{
				ChessTag[x1][y1] = NULLCHESS;
			}
		}
		
		
	}
}
//技能3 实现
void CMFCFivePointsChessApp::Skill3Effect(int x, int y)
{
	int x1, y1;
	//0
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, LeftUp))
	{
		ChessTag[x1][y1] = NULLCHESS;
	}

	//1
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, Up))
	{
		ChessTag[x1][y1] = NULLCHESS;
	}
	
	//2
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, RightUp))
	{
		ChessTag[x1][y1] = NULLCHESS;
	}
	
	//3
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, Left))
	{
		ChessTag[x1][y1] = NULLCHESS;
	}
	
	//4

	ChessTag[x][y]=CANTCHESS;

	//5
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, Right))
	{
		ChessTag[x1][y1] = NULLCHESS;
	}
	
	//6
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, LeftDown))
	{
		ChessTag[x1][y1] = NULLCHESS;
	}
	
	//7
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, Down))
	{
		ChessTag[x1][y1] = NULLCHESS;
	}
	
	//8
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, RightDown))
	{
		ChessTag[x1][y1] = NULLCHESS;
	}
}
//技能4 实现
void CMFCFivePointsChessApp::Skill4Effect(int x, int y,int CD)
{
	int x1, y1;
	//0
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, LeftUp))
	{
		ChessViewTag[x1][y1] +=CD;
	}

	//1
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, Up))
	{
		ChessViewTag[x1][y1] +=CD;
	}

	//2
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, RightUp))
	{
		ChessViewTag[x1][y1] +=CD;
	}

	//3
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, Left))
	{
		ChessViewTag[x1][y1] +=CD;
	}

	//4

	ChessViewTag[x][y] += CD;

	//5
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, Right))
	{
		ChessViewTag[x1][y1] +=CD;
	}

	//6
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, LeftDown))
	{
		ChessViewTag[x1][y1] +=CD;
	}

	//7
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, Down))
	{
		ChessViewTag[x1][y1] +=CD;
	}

	//8
	x1 = x, y1 = y;
	if (CalculatePointDiretion(&x1, &y1, RightDown))
	{
		ChessViewTag[x1][y1] +=CD;
	}

}
//初始化与技能4配套的二维数组
void CMFCFivePointsChessApp::initViewChessTag()
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			ChessViewTag[i][j] = 0;
		}
	}
}
//技能4效果时长的消耗
void CMFCFivePointsChessApp::TimeRunCDOfViewChessTag()
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			if (ChessViewTag[i][j] > 0)
			{
				ChessViewTag[i][j] -= 1;
			}
		}
	}
}
 //根据棋盘的点计算最浅层就是开始那层的初始分数  
//这里的MyTag是用来标记传进来的点是防守点还是进攻点 
//因为最一开始这层需要计算到敌人的优势分也就是自己需要防守的高分 需要记录
//而其他层只需要计算相对与下棋者的分数！！！//使用之前要对ChessScoreResult进行清空
void CMFCFivePointsChessApp::SPAI_CalculateScoreWithChessPoint(const int &x,const int &y,const int&MyTag)
{
	int result;
	if (MyTag == GetFlag())
	{
		result=CalculatePow10(CalculatePow2(ChessScoreLR[x][y]) + CalculatePow2(ChessScoreUD[x][y]) + CalculatePow2(ChessScoreUR[x][y]) + CalculatePow2(ChessScoreUL[x][y]));
		if (ALLChessScoreResult[x][y] < result)
		{
			ALLChessScoreResult[x][y] = result;
		}
	}
	else
	{
		result=CalculatePow10(CalculatePow2(Opp_ChessScoreLR[x][y]) + CalculatePow2(Opp_ChessScoreUD[x][y]) + CalculatePow2(Opp_ChessScoreUR[x][y]) + CalculatePow2(Opp_ChessScoreUL[x][y]));
		if (ALLChessScoreResult[x][y] < result)
		{
			ALLChessScoreResult[x][y] = result;
		}
	
	}
	//////////////////////////----------------- //AI的探索深度
	SPAI_CalculateStartRound(x, y, GetFlag(), AI_FindDepth);
}
//深度探索的AI最开始探索的地方 待选点x，y棋盘下标，MyTag是标记从谁的角度算分 Round是AI的探索深度
int CMFCFivePointsChessApp::SPAI_CalculateStartRound(const int & x, const int & y, const int & MyTag, int Round)
{
	//修改
	PointScore* point = new PointScore{ x,y,&ALLChessScoreResult[x][y],MyTag,{} };
	RubbishPS.push_back(point);
	//容器存入指针 
	Total_BestScoreVec.push_back(point);
	SPAI_CalculateThisRound(point, 1,Round);
	return 0;
	
}
//深度探索的AI 计算当前层级最优的10个点 //计算这一轮最好的前n个点并存储到 PointScore中
void CMFCFivePointsChessApp::SPAI_CalculateThisRoundNicePoints(PointScore* LastChoosePoint)
{
	std::vector<PointPos*> temp;
	//先初始化分数 设置为0
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
	Total_BestPoints.clear();
	BestPoints.clear();
	Opp_BestPoints.clear();
	//计算分数
	/*----------------前置筛选---------------*/
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
	bool havePoint = false;
	bool NoRequirePeace = true;
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			if (ChessScoreResult[i][j] == 1)
			{
				havePoint = true;
				PointPos* p = new PointPos{ i,j };
				temp.push_back(p);
				BestPoints.push_back(*p);
			}
		}
	}
	//这个是完全没有棋子 让AI把位置放在屏幕中间
	if (!havePoint)
	{
		if (ChessTag[7][7] == NULLCHESS)
		{
			PointPos* p = new PointPos{ 7,7 };
			temp.push_back(p);
			BestPoints.push_back(*p);
		}
		else
		{
			return;
		}
	}
	//初始化分数记录的数组们
	InitAllScoreArray1();
	//剔除掉那些在边界的点 没有可能能够形成五子的地方
	if (!FirstAssessResult())
	{
		NoRequirePeace = false;
	}
	//把能五子但局部不能五子的地方标记 -1
	SecondAssessResult();
	/*-------------------两轮筛选之后的到的点 符合以下条件：-------------------------*/
				//1.首先有机会形成五个子 无论是敌方还是自己
			   //2.把有些积分是负数说明是不能可能形成5子的
	/*-------------------------------------------------------------------------------*/
/*----------------第三轮筛选---------------*/
   //得到真实的棋子数
/*-------------------------------------------*/
	ThirdAssessResult();

	//如果有大于3 也就是可以决胜的棋子直接跳过这一步 会返回1 
	if (JudgeFromChessCount() == 0)
	{
		/*----------------第四轮筛选---------------*/
		//细分1 2 3棋子数 映射为 0~5
		FourthAssessResult();
		//选择得分最大的10个点 保存到最优点里面分别在 bestpoints opp_bestpoints里
		JudgeFromChessCount2(2, 10);
	}
	else
	{
		//把大于4映射为8
		for each (PointPos it in BestPoints)
		{
			if (ChessScoreLR[it.x][it.y] >= 4)
			{
				ChessScoreLR[it.x][it.y] = 2 * ChessScoreLR[it.x][it.y];
			}
			if (ChessScoreUD[it.x][it.y] >= 4)
			{
				ChessScoreUD[it.x][it.y] = 2 * ChessScoreUD[it.x][it.y];
			}
			if (ChessScoreUL[it.x][it.y] >= 4)
			{
				ChessScoreUL[it.x][it.y] = 2 * ChessScoreUL[it.x][it.y];
			}
			if (ChessScoreUR[it.x][it.y] >= 4)
			{
				ChessScoreUR[it.x][it.y] = 2 * ChessScoreUR[it.x][it.y];
			}
		}

	}
	/*算敌我两个的交集*/
	std::vector<PointPos> temp1;
			for each (PointPos it in BestPoints)
			{
				temp1.push_back(it);
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
					temp1.push_back(it);
				}
			}
			//交集计算完成
			
	//经过上面的处理之后 选择的点都存在temp1中
			for each  (PointPos it in temp1)
			{
				PointScore* ps = new PointScore{ it.x,it.y,LastChoosePoint->TotalScore,{}};
				LastChoosePoint->PointScoreChilds.push_back(ps);
				RubbishPS.push_back(ps);
			}
	PointPos* pre;
	for each  (PointPos* it in temp)
	{
		pre = it;
		it = NULL;
		free(pre);
	}
	return;

}
//深度探索的AI 计算当前层级
int CMFCFivePointsChessApp::SPAI_CalculateThisRound(PointScore* LastChoosePoint,int Round,const int& TargetRound)
{
	//假设下一个MyTag类型棋子了在这个位置
	if (Round > TargetRound)
	{
		return 0;
	}
	int tempTag = ChessTag[LastChoosePoint->x][LastChoosePoint->x];
	ChessTag[LastChoosePoint->x][LastChoosePoint->x] = LastChoosePoint->Tag;
	SPAI_CalculateThisRoundNicePoints(LastChoosePoint);
	if (LastChoosePoint->PointScoreChilds.empty())
	{
		return 0;
	}

	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			ChessScoreResult[i][j] = 0;
		}
	}
	
	for each (PointScore* it in LastChoosePoint->PointScoreChilds)
	{
		int result;
		if (LastChoosePoint->Tag == GetFlag())
		{
			//上一轮的tag是我 那么这轮计算的是敌人的分数
			result = CalculatePow10(CalculatePow2(Opp_ChessScoreLR[it->x][it->y]) + CalculatePow2(Opp_ChessScoreUD[it->x][it->y]) + CalculatePow2(Opp_ChessScoreUR[it->x][it->y]) + CalculatePow2(Opp_ChessScoreUL[it->x][it->y]));
			if (ChessScoreResult[it->x][it->y] < result)
			{
				ChessScoreResult[it->x][it->y] = result;
			}
		}
		else
		{
		
			result = CalculatePow10(CalculatePow2(ChessScoreLR[it->x][it->y]) + CalculatePow2(ChessScoreUD[it->x][it->y]) + CalculatePow2(ChessScoreUR[it->x][it->y]) + CalculatePow2(ChessScoreUL[it->x][it->y]));
			if (ChessScoreResult[it->x][it->y] < result)
			{
				ChessScoreResult[it->x][it->y] = result;
			}
			
		}
	}
	for each (PointScore* it in LastChoosePoint->PointScoreChilds)
	{
		//下与上一层相反的棋子 并加减分
		it->Tag = 3 - LastChoosePoint->Tag;
		if (LastChoosePoint->Tag == GetFlag())
		{
			//round+/2
			(*LastChoosePoint->TotalScore) -= ChessScoreResult[it->x][it->y]/ LastChoosePoint->PointScoreChilds.size()/((Round+1)/2);
		}
		else
		{
			(*LastChoosePoint->TotalScore) += ChessScoreResult[it->x][it->y]/ LastChoosePoint->PointScoreChilds.size()/((Round+1)/2);
		}
		
	}
	Round = Round + 1;
	//向下一层进入
	for each (PointScore* it in LastChoosePoint->PointScoreChilds)
	{
		SPAI_CalculateThisRound(it, Round, TargetRound);
	}
	//还原假设
	ChessTag[LastChoosePoint->x][LastChoosePoint->x] = tempTag;
	return 0;
}
//根据棋子映射分 再映射到2的次方
int CMFCFivePointsChessApp::CalculatePow2(int i)
{

	switch (i)
	{
	case 0:
		return 1;
		//单死一 远一
	case 1:
		return 2;
		//活一 死远二
	case 2:
		return 4;
		//单死二 活远二
	case 3:
		return 8;

	case 4:
		//死三 活二 远活三
		return 16;

	case 5:
		//活三
		return 32;
	default:
		break;
	}
	if (i > 0)
	{
		return 32;
	}
	else
	{
		return 0;
	}


}
//根据棋子映射分 再映射到10的次方
int CMFCFivePointsChessApp::CalculatePow10(int i)
{
	if (i >= 1 && i < 2)
	{
		return 1;
	}
	if (i >= 2 && i < 4)
	{
		//单死一 远一
		return 10;
	}
	if (i >= 4 && i < 8)
	{
		//活一 死远二 双死二
		return 100;
	}
	if (i >= 8 && i < 16)
	{
		//单死二 活远二
		return 1000;
	}
	if (i >= 16 && i < 32)
	{
		//死三 活二 远活三
		return 10000;
	}
	if (i >= 32)
	{
		//活三
		return 100000;
	}
}
//关于Q和E的旋转按键
void CMFCFivePointsChessApp::ChangeSelectDirection(bool Q)
{
	switch (selectDirection)
	{
	case Left:
		if (Q)
		{
			selectDirection = RightUp;
		}
		else
		{
			selectDirection = LeftUp;
		}
		break;
	case Up:
		if (Q)
		{
			selectDirection = LeftUp;
		}
		else
		{
			selectDirection = RightUp;
		}
		break;
	case LeftUp:
		if (Q)
		{
			selectDirection = Left;
		}
		else
		{
			selectDirection = Up;
		}
		break;
	case RightUp:
		if (Q)
		{
			selectDirection = Up;
		}
		else
		{
			selectDirection = Left;
		}
		break;
	}
}
//获得旋转选择框选择的方向 
Direction CMFCFivePointsChessApp::GetSelectDirection()
{
	return selectDirection;
}
//悔棋的实现
PaintType CMFCFivePointsChessApp::Regret()
{
	int x, y;
	PaintType type = DeleteAndLoadLastRecord(&x, &y);
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
//创建历史记录
void CMFCFivePointsChessApp::CreateRecord(int x, int y, PaintType type)
{
	pRecord = new Record{ x,y,type,pRecord };
}
//删除历史记录 用于防止内存泄漏
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
//根据历史记录来添加棋子
void CMFCFivePointsChessApp::AddChessFromRecord()
{
	if (pRecord)
	{
		for (int i = 0; i < ChessBlockNum - 1; i++)
	   {
		  for (int j = 0; j < ChessBlockNum - 1; j++)
		  {
			ChessTag[i][j] = 0;
		  }
	   }
	}
	Record* pCur = pRecord;
	while (pCur)
	{
		ChessTag[pCur->x][pCur->y] = pCur->type;
		pCur = pCur->next;
	}
}
//对下棋历史记录 进行反转 因为记录是个链表 这里就是反转链表的操作
void CMFCFivePointsChessApp::ReverseRecord()
{
	if (!pRecord)
	{
		return;
	}
	Record* pCur = pRecord;
	Record* pPre = NULL; 
	Record *pNext = NULL;
	while (pCur->next)
	{
		pNext = pCur->next;
		pCur->next = pPre;
		pPre = pCur;
		pCur = pNext;
	}
	pCur->next = pPre;
	pRecord = pCur;
}
//当按下保存
void CMFCFivePointsChessApp::OnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilters[] ="Text File(*.txt)\0*.txt\0"\
		"\0";
	CFileDialog FileDlg(FALSE,_T("txt"), _T("chess"), OFN_OVERWRITEPROMPT, _T("text Files(*.txt) | *.txt||"),NULL);
	FileDlg.DoModal();
	CString filename = FileDlg.GetPathName();
	CString strWriteData;
	CString str;
	Record* pCur = pRecord;
	str.Format(_T("%d \n"), GameMode);
	strWriteData += str;
	while (pCur)
	{
		str.Format(_T("%d %d "), pCur->x, pCur->y);
		strWriteData += str;
		switch (pCur->type)
		{
		case White:
			str.Format(_T("%d \n"), WHITECHESS);
			break;
		case Black:
			str.Format(_T("%d \n"), BLACKCHESS);
			break;
		default:
			break;
		}
		strWriteData += str;
		pCur = pCur->next;
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
//当按下读取
void CMFCFivePointsChessApp::OnRead()
{
	char szFilters[] = "Text File(*.txt)\0*.txt\0"\
		"\0";
	CFileDialog FileDlg(TRUE, _T("txt"), _T("chess"), OFN_HIDEREADONLY, _T("text Files(*.txt) | *.txt||"), NULL);
	FileDlg.DoModal();
	CStdioFile FileRead;
	if (!(FileRead.Open(FileDlg.GetPathName(), CFile::modeRead | CFile::typeText)))
	{
		return;
	}
	freeRecord();
	vector<CString>vecReadText;
	CString cstrValue;
	int h = 0;
	while (FileRead.ReadString(cstrValue))
	{
		h++;
		vecReadText.push_back(cstrValue);
	}

	FileRead.Close();


	CString str;
	CString str1;
	std::vector<int> temp;
	for (int k = 0; k < vecReadText[0].GetLength(); k++)
	{
		str = _T(" ");
		str1= _T("\n");
		if (vecReadText[0][k] != str&& vecReadText[0][k] != str1)
		{
			char a = vecReadText[0][k];
			GameMode = (int)a - 48;
		}
	}
	
	for (int i = 1; i < h; i++)
	{
		
		int count=vecReadText[i].GetLength();
		str = _T(" ");
		int num=0;
		char a;
		for (int k = 0; k < count; k++)
		{
			if (vecReadText[i][k] != str)
			{
				 a = vecReadText[i][k];
				 num=num*10+(int)a-48;
			}
			else
			{
				temp.push_back(num);
				num = 0;
			}
		}
		pRecord = new Record{ temp.at(0), temp.at(1), (PaintType)temp.at(2), pRecord };
		temp.clear();
	}
	if (!pRecord)
	{
		return;
	}
	ReverseRecord();
	AddChessFromRecord();
	switch (pRecord->type)
	{
	case White:
		ChangeFlag(BLACKCHESS);
		str = "请黑方下子";
		break;
	case Black:
		ChangeFlag(WHITECHESS);
		str = "请白方下子";
		break;
	case Empty:
		ChangeFlag(BLACKCHESS);
		str = "请黑方下子";
		break;
	}
	CMFCFivePointsChessDlg* pWnd = (CMFCFivePointsChessDlg*)AfxGetApp()->GetMainWnd();
	pWnd->DrawSelectRectAndChess();
	pWnd->MessageEdit.SetWindowTextW(str);
}

//按下清除存档
void CMFCFivePointsChessApp::OnRemoveRecord()
{
	char szFilters[] = "Text File(*.txt)\0*.txt\0"\
		"\0";
	CFileDialog FileDlg(TRUE, _T("txt"), _T("chess"), NULL , _T("text Files(*.txt) | *.txt||"), NULL);
	FileDlg.DoModal();
	DeleteFile(FileDlg.GetPathName());

	// TODO: 在此添加命令处理程序代码
}
