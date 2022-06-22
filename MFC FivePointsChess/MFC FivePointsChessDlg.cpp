
// MFC FivePointsChessDlg.cpp: 实现文件
//
#include <windows.h>
#include "pch.h"
#include "framework.h"
#include "MFC FivePointsChess.h"
#include "MFC FivePointsChessDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CMFCFivePointsChessDlg 对话框



CMFCFivePointsChessDlg::CMFCFivePointsChessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCFIVEPOINTSCHESS_DIALOG, pParent)
{
	
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	
	
}
	


void CMFCFivePointsChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BACKGROUND, ChessBoard);
	DDX_Control(pDX, IDC_POINT, text);

	
	DDX_Control(pDX, IDC_BUTTON1, button);
	DDX_Control(pDX, IDC_MESSAGE, MessageEdit);
}

BEGIN_MESSAGE_MAP(CMFCFivePointsChessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	
	ON_STN_CLICKED(IDC_BACKGROUND, &CMFCFivePointsChessDlg::OnStnClickedBackground)
	ON_WM_MOUSEMOVE()
	ON_STN_CLICKED(IDC_POINT, &CMFCFivePointsChessDlg::OnStnClickedPoint)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TIPS, &CMFCFivePointsChessDlg::OnBnClickedTips)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_REGRET, &CMFCFivePointsChessDlg::OnClickedRegret)
	ON_COMMAND(ID_32777, &CMFCFivePointsChessDlg::OnChanegeGameModeEasy)
	ON_COMMAND(ID_32775, &CMFCFivePointsChessDlg::OnChangeGameModePP)
	ON_BN_CLICKED(IDC_RESTART, &CMFCFivePointsChessDlg::OnBnClickedRestart)
END_MESSAGE_MAP()


// CMFCFivePointsChessDlg 消息处理程序

BOOL CMFCFivePointsChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SelectY = SelectX = ChessBlockNum / 2 * ChessBlockSide;
	button.SetFocus();
	CWnd *pWind;
	pWind = GetDlgItem(IDC_BACKGROUND);
	pWind->SetWindowPos(NULL, SPACE_X, SPACE_Y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GameMode = -1;
	CString str;
	str = "请选择模式!!!";
	MessageEdit.SetWindowTextW(str);
	

	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCFivePointsChessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCFivePointsChessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCFivePointsChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CMFCFivePointsChessDlg::JudgeSelectPos(Direction dir)
{
	switch (dir)
	{
	case Left:
		SelectX -= ChessBlockSide;
		break;
	case Right:
		SelectX += ChessBlockSide;
		break;
	case Up:
		SelectY -= ChessBlockSide;
		break;
	case Down:
		SelectY += ChessBlockSide;
		break;
	default:
	{
		break;
	}
		
	}
	if (SelectX <ChessBlockSide)
	{
		SelectX = ChessBlockSide;
	}
	if (SelectX > ChessBlockSide*(ChessBlockNum - 1))
	{
		SelectX = ChessBlockSide * (ChessBlockNum - 1);
	}
	if (SelectY < ChessBlockSide)
	{
		SelectY = ChessBlockSide;
	}
	if (SelectY > ChessBlockSide*(ChessBlockNum - 1))
	{
		SelectY = ChessBlockSide * (ChessBlockNum - 1);
	}
	
}

void CMFCFivePointsChessDlg::DrawSelectRect()
{
	CWnd *pWind;
	pWind = GetDlgItem(IDC_BACKGROUND);

	CDC* pDC = pWind->GetDC();
	//定义一个内存设备描述表对象（即后备缓冲区）
	CDC MemDC;
	//定义一个位图对象
	CBitmap MemBitmap;
	CBitmap MemBitmap1;
	//建立与屏幕设备描述表（前端缓冲区）兼容的内存设备描述表句柄（后备缓冲区）
	
	MemDC.CreateCompatibleDC(NULL);
	//这时还不能绘图，因为没有位图的设备描述表是不能绘图的
	//下面建立一个与屏幕设备描述表（或者内存设备描述表）兼容的位图

	MemBitmap.LoadBitmapW(IDB_CHESSBOARD);
	MemBitmap1.LoadBitmapW(IDB_CHESSBOARD);
	//将位图选入到内存设备描述表
	//只有选入了位图的设备描述表才有地方绘图，画到指定的位图上
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	//先用背景色将位图清除干净，这里我用的是白色作为背景
	//你也可以用自己应该用的颜色
    
	//绘图
	CDC background;
	background.CreateCompatibleDC(NULL);
	background.SelectObject(&MemBitmap1);

	
	

	CRect *r1 = new CRect(SelectX - 20, SelectY - 20, SelectX + 20, SelectY + 20);
	switch (theApp.GetFlag())
	{
	case 0:
	{
		break;
	}
	case 1:
	{
		MemDC.FillSolidRect(*r1, RGB(50, 50, 50));
		break;
	}
	case 2:
	{
		MemDC.FillSolidRect(*r1, RGB(255, 255, 255));
		break;
	}
	}
	

	pDC->BitBlt(0, 0, 640, 640, &MemDC, 0, 0, SRCCOPY);
	
	pDC->BitBlt(SelectX - 15, SelectY - 15, 30, 30, &background, SelectX - 15, SelectY - 15, SRCCOPY);
	pDC->BitBlt(SelectX - 20, SelectY - 10, 40, 20, &background, SelectX - 20, SelectY - 10, SRCCOPY);
	pDC->BitBlt(SelectX - 10, SelectY - 20, 20, 40, &background, SelectX - 10, SelectY - 20, SRCCOPY);
	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemBitmap1.DeleteObject();
	MemDC.DeleteDC();
	background.DeleteDC();
	


	
	delete r1;
	ReleaseDC(pDC);
	
	//绘制所有棋子
	DrawAllChess(&theApp);
}

void CMFCFivePointsChessDlg::OnStnClickedBackground()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCFivePointsChessDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	
	/*if (point.x - SPACE_X < 600)
	{
		if (point.x - SPACE_X > 40)
		{
			SelectX = (point.x - SPACE_X);
		}
		else
		{
			SelectX = 40;
		}
	}
	else
	{
		SelectX = 600;

	}
	if (point.y - SPACE_Y < 600)
	{
		if (point.y - SPACE_Y > 40)
		{
			SelectY = point.y - SPACE_Y;
		}
		else
		{
			SelectY = 40;
		}
	}
	else
	{
		SelectY = 600;
	}*/
	
	//获得程序状态栏对象的指针,AFX_IDW_STATUS_BAR就是状态栏的ID
	//它的功能是通过指定的ID来获得子孙窗口
	
}


void CMFCFivePointsChessDlg::OnStnClickedPoint()
{
	// TODO: 在此添加控件通知处理程序代码
}




void CMFCFivePointsChessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIME_SELECT:
	{
		CWnd *pWind;
		pWind = GetDlgItem(IDC_BACKGROUND);
		break;
	}
	default:
		break;
	}
}
void CMFCFivePointsChessDlg::DrawAllChess(CMFCFivePointsChessApp* ChessApp)
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			switch (ChessApp->GetChess(i,j))
			{
				//0是没有棋子不用绘制
			case NULLCHESS:
			{

				break;
			}
			case BLACKCHESS:
			{
				//1是黑棋
				DrawChess(Black, (i + 1)*ChessBlockSide - ChessBlockSide / 2, (j + 1)*ChessBlockSide - ChessBlockSide / 2);
				break;
			}
			case WHITECHESS:
			{
				//2是白棋
				DrawChess(White, (i + 1)*ChessBlockSide - ChessBlockSide / 2, (j + 1)*ChessBlockSide - ChessBlockSide / 2);
				break;
			}
			}
		}
	}
}
void CMFCFivePointsChessDlg::DrawChess(PaintType type , int PosX ,int PosY)
{
	
	CBitmap bmp;
	switch (type)
	{
	case White:
	{
		bmp.LoadBitmapW(IDB_WHITE);//加载位图资源，将位图资源加载位图对象中
		break;
	}
	case Black:
	{
		bmp.LoadBitmapW(IDB_BLACK);//加载位图资源，将位图资源加载位图对象中
		break;
	}
	default:
		return;
	}
	CDC* pDC;
	CWnd *pWind;
	pWind = GetDlgItem(IDC_BACKGROUND);
	pDC = pWind->GetDC();
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(pDC);
	CBitmap *pOldImageBMP = ImageDC.SelectObject(&bmp);
	TransparentBlt(pDC->m_hDC, PosX, PosY, 40,40, ImageDC.m_hDC, 0, 0, 40, 40, RGB(0, 255, 0));
	ImageDC.SelectObject(pOldImageBMP);
	ImageDC.DeleteDC();
	bmp.DeleteObject();
	ReleaseDC(pDC);
	/*pDC->BitBlt(0, 0, 40, 40, &ImageDC, 0, 0, SRCCOPY);*/
	

}

void CMFCFivePointsChessDlg::OnBnClickedTips()
{
	// TODO: 在此添加控件通知处理程序代码
	int x, y;
	theApp.AI_FindBestPoint(&x, &y);
	SelectX = (x + 1)*ChessBlockSide;
	SelectY = (y + 1)*ChessBlockSide;
	DrawSelectRect();
}


BOOL CMFCFivePointsChessDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return FALSE;
	/*return CDialogEx::OnEraseBkgnd(pDC);*/
}




BOOL CMFCFivePointsChessDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GameMode != -1)
		{


			switch (pMsg->wParam)
			{
			case VK_SPACE:
			{
				button.SetFocus();
				//落子 

				CString str;
				switch (theApp.GetFlag())
				{
				case 0:
				{



					switch (theApp.GameWiner)
					{
					case NULLCHESS:
					{
						str = "请开始游戏";
						break;
					}
					case BLACKCHESS:
					{
						str = "黑方赢了！";
						break;
					}
					case WHITECHESS:
					{
						str = "白方赢了！";
						break;
					}
					default:
						break;
					}

					break;
				}
				case 1:
				{
					if (GameMode == 0)
					{
						//黑色下棋
						if (theApp.TryAddChess(SelectX, SelectY, Black))
						{
							//如果成功下棋那么就切换下棋手
							str = "请白方下子";
							theApp.ChangeFlag(2);
							break;
						}
						DrawSelectRect();
						return TRUE;
					}
					else
					{
						if (GameMode == 1)
						{
							if (theApp.TryAddChess(SelectX, SelectY, Black))
							{
								DrawSelectRect();
								//如果成功下棋那么就切换下棋手
								str = "电脑正在下子";
								theApp.ChangeFlag(2);
								MessageEdit.SetWindowTextW(str);
								UpdateWindow();
								int x, y;
								if (theApp.AI_FindBestPoint(&x, &y))
								{
									//进行下棋
									SelectX = (x + 1)*ChessBlockSide;
									SelectY = (y + 1)*ChessBlockSide;
									Sleep(500);
									DrawSelectRect();
									Sleep(150);
									if (!theApp.TryAddChess(SelectX, SelectY, White))
									{
										DrawSelectRect();
										return TRUE;
									}

									DrawSelectRect();

									str = "请黑方下子";
									theApp.ChangeFlag(1);
									break;
								}
								else
								{
									SelectX = (x + 1)*ChessBlockSide;
									SelectY = (y + 1)*ChessBlockSide;
									Sleep(500);
									DrawSelectRect();
									Sleep(150);
									if (!theApp.TryAddChess(SelectX, SelectY, White))
									{
										DrawSelectRect();
										return TRUE;
									}
									DrawSelectRect();
									//求和操作 //TODO:
									theApp.ChangeFlag(1);
									break;
							
								}
								
							}
							DrawSelectRect();
							return TRUE;
						}
					}

				}
				case 2:
				{
					//白色下棋
					if (GameMode == 0)
					{
						if (theApp.TryAddChess(SelectX, SelectY, White))
						{
							//如果成功下棋那么就切换下棋手
							str = "请黑方下子";
							theApp.ChangeFlag(1);
							break;
						}
						DrawSelectRect();
						return TRUE;
					}

				}
				default:
					break;
				}
				
				MessageEdit.SetWindowTextW(str);
				DrawSelectRect();
				//SPACE结束
				break;
			}


			//W
			case 87:
			{
				if (theApp.GetFlag() == 1)
				{
					JudgeSelectPos(Up);
					DrawSelectRect();
				}

				break;
			}

			//A
			case 65:
			{
				if (theApp.GetFlag() == 1)
				{
					JudgeSelectPos(Left);
					DrawSelectRect();
				}

				break;
			}
			//S
			case 83:
			{
				if (theApp.GetFlag() == 1)
				{
					JudgeSelectPos(Down);
					DrawSelectRect();
				}

				break;
			}
			//D
			case 68:
			{
				if (theApp.GetFlag() == 1)
				{
					JudgeSelectPos(Right);
					DrawSelectRect();
				}
				break;
			}



			//上
			case VK_UP:
			{
				if (theApp.GetFlag() == 2)
				{
					JudgeSelectPos(Up);
					DrawSelectRect();
				}
				break;
			}

			//左
			case VK_LEFT:
			{
				if (theApp.GetFlag() == 2)
				{
					JudgeSelectPos(Left);
					DrawSelectRect();
				}
				break;
			}
			//下
			case VK_DOWN:
			{
				if (theApp.GetFlag() == 2)
				{
					JudgeSelectPos(Down);
					DrawSelectRect();
				}
				break;
			}
			//右
			case VK_RIGHT:
			{
				if (theApp.GetFlag() == 2)
				{
					JudgeSelectPos(Right);
					DrawSelectRect();
				}
				break;
			}

			//白方操作结束
			default:
				break;
			}

			CString str;
			str.Format(_T("[%d,%d]  "), SelectX, SelectY);
			text.SetWindowTextW(str);

			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//当玩家按下悔棋
void CMFCFivePointsChessDlg::OnClickedRegret()
{

	CString str;
	if (GameMode== 0 )
	{
		switch (theApp.Regret())
		{
		case Empty:
		{
			break;
		}
		case White:
		{
			str = "请白方下子";
			theApp.ChangeFlag(2);
			break;
		}
		case Black:
		{
			str = "请黑方下子";
			theApp.ChangeFlag(1);
			break;
		}
		}

		MessageEdit.SetWindowTextW(str);
		DrawSelectRect();
	}
	else
	{
		if (GameMode > 0)
		{
			while (theApp.Regret()==White)
			{
			}
			str = "请黑方下子";
			theApp.ChangeFlag(1);
			MessageEdit.SetWindowTextW(str);
			DrawSelectRect();
		}
	}
	
}


void CMFCFivePointsChessDlg::OnChanegeGameModeEasy()
{

	// TODO: 在此添加命令处理程序代码
	GameMode = 1;
	DrawSelectRect();
	CString str;
	str = "人机模式：你是黑方，电脑白方\r\n 按空格下子 WASD移动";
	MessageEdit.SetWindowTextW(str);
}


void CMFCFivePointsChessDlg::OnChangeGameModePP()
{
	GameMode = 0;
	DrawSelectRect();
	CString str;
	str = "人人模式：黑方先手，白方后手 \r\n 按空格下子 \r\n 黑方：WASD移动 白方：方向键移动";
	MessageEdit.SetWindowTextW(str);
	// TODO: 在此添加命令处理程序代码
}


void CMFCFivePointsChessDlg::OnBnClickedRestart()
{
	
	theApp.init();
	GameMode = -1;
	CString str;
	str = "请选择模式!!!";
	MessageEdit.SetWindowTextW(str);
	SelectY = SelectX = ChessBlockNum / 2 * ChessBlockSide;
	DrawSelectRect();

	// TODO: 在此添加控件通知处理程序代码
}
