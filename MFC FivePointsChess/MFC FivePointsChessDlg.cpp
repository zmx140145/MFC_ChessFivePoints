
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
	
	
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LOGO);
	
	
	
}
	


void CMFCFivePointsChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BACKGROUND, ChessBoard);
	DDX_Control(pDX, IDC_BUTTON1, button);
	DDX_Control(pDX, IDC_MESSAGE, MessageEdit);
	DDX_Control(pDX, IDC_REGRET, RegretButton);
}

BEGIN_MESSAGE_MAP(CMFCFivePointsChessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TIPS, &CMFCFivePointsChessDlg::OnBnClickedTips)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_REGRET, &CMFCFivePointsChessDlg::OnClickedRegret)
	ON_COMMAND(ID_32777, &CMFCFivePointsChessDlg::OnChanegeGameModeEasy)
	ON_COMMAND(ID_32775, &CMFCFivePointsChessDlg::OnChangeGameModePP)
	ON_BN_CLICKED(IDC_RESTART, &CMFCFivePointsChessDlg::OnBnClickedRestart)
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_32779, &CMFCFivePointsChessDlg::OnSkill1)
	ON_COMMAND(ID_32780, &CMFCFivePointsChessDlg::OnSkill2)
	ON_COMMAND(ID_32781, &CMFCFivePointsChessDlg::OnSkill3)
	ON_COMMAND(ID_32782, &CMFCFivePointsChessDlg::OnSkill4)
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
	theApp.GameMode = -1;
	CString str;
	str = "请选择模式!!!";
	theApp.initViewChessTag();
	theApp.SetAI_Station(welcome);
	ReDrawMessage(str);
	SetTimer(1, 1000, NULL);

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
		
		DrawBaclgroundAcorrdingSituation();
		CDialogEx::OnPaint();
	}
}
void CMFCFivePointsChessDlg::DrawBaclgroundAcorrdingSituation()
{
	switch (theApp.GameMode)
	{
	case -1:
		switch (theApp.GetAI_Station())
		{
		case welcome:
			DrawBackground(2);
			break;
		case sad:
			DrawBackground(4);
			break;
		case await:
			DrawBackground(3);
			break;
		case think:
			DrawBackground(5);
			break;
		case happy:
			DrawBackground(6);
			break;
		default:
			break;
		}
		break;
	case 0:
		DrawBackground(2);
		break;
	case 1 :case 2:
		switch (theApp.GetAI_Station())
		{
		case welcome:
			DrawBackground(2);
			break;
		case sad:
			DrawBackground(4);
			break;
		case await:
			DrawBackground(3);
			break;
		case think:
			DrawBackground(5);
			break;
		case happy:
			DrawBackground(6);
			break;
		default:
			break;
		}
		break;
	
	default:
		break;
	}
	
	
		
	
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCFivePointsChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMFCFivePointsChessDlg::DrawBackground(const int& choose)
{
	CRect rect1{ChessBlockNum*ChessBlockSide+30,0,1470,910};
	InvalidateRect(&rect1);
	CPaintDC   dc(this);
	CRect   rect;
	GetClientRect(&rect);
	CDC   dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap   bmpBackground;
	switch (choose)
	{
	case 1:
		bmpBackground.LoadBitmap(IDB_1);
		break;
	case 2:
		bmpBackground.LoadBitmap(IDB_2);
		break;
	case 3:
		bmpBackground.LoadBitmap(IDB_3);
		break;
	case 4:
		bmpBackground.LoadBitmap(IDB_4);
		break;
	case 5:
		bmpBackground.LoadBitmap(IDB_5);
		break;
	case 6:
		bmpBackground.LoadBitmap(IDB_6);
		break;
	default:
		return;
	}
	
	BITMAP   bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap   *pbmpOld = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0,
		bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	DrawSelectRect();
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

	
	

	
	COLORREF color;
	switch (theApp.GetFlag())
	{
	case 0:
	{
		color= color = RGB(255, 255, 255);
		break;
	}
	case 1:
	{
		color = RGB(50, 50, 50);
		
		break;
	}
	case 2:
	{
		color = RGB(255, 255, 255);
		
		break;
	}
	}
	CRect *r1 = new CRect(SelectX - 20, SelectY - 20, SelectX + 20, SelectY + 20);
	switch (theApp.GetSkill())
	{
	case skill1:
		DrawSkill1SelectRect(&background, theApp.GetSelectDirection(), color);
		pDC->BitBlt(0, 0, 640, 640, &background, 0, 0, SRCCOPY);
		break;
	case skill2:
		if (theApp.isSelectTimeRect)
		{

			if (RGB(255, 255, 255) == color)
			{
				color = RGB(50, 50, 50);
			}
			else
			{
				color = RGB(255, 255, 255);
			}
		}
		DrawSkill2SelectRect(&background, color);
		pDC->BitBlt(0, 0, 640, 640, &background, 0, 0, SRCCOPY);
		break;
	case skill4:
	     DrawSkill2SelectRect(&background, color); 
		pDC->BitBlt(0, 0, 640, 640, &background, 0, 0, SRCCOPY);
		break;
	default:
		MemDC.FillSolidRect(*r1, color);
		pDC->BitBlt(0, 0, 640, 640, &MemDC, 0, 0, SRCCOPY);
		pDC->BitBlt(SelectX - 15, SelectY - 15, 30, 30, &background, SelectX - 15, SelectY - 15, SRCCOPY);
		pDC->BitBlt(SelectX - 20, SelectY - 10, 40, 20, &background, SelectX - 20, SelectY - 10, SRCCOPY);
		pDC->BitBlt(SelectX - 10, SelectY - 20, 20, 40, &background, SelectX - 10, SelectY - 20, SRCCOPY);
		break;
	}

	
	
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


void CMFCFivePointsChessDlg::DrawSkill2SelectRect(CDC* pDC, COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_DASH, 5, color);
	pDC->SelectObject(&pen);
	int x = SelectX / ChessBlockSide - 1;
	int y = SelectY / ChessBlockSide - 1;
	int x1 = x;
	int y1 = y;
	theApp.CalculatePointDiretion(&x, &y, RightUp);
	theApp.CalculatePointDiretion(&x1, &y1, LeftDown);
	x = (x + 1)* ChessBlockSide;
	y = (y + 1) * ChessBlockSide;
	x1 = (x1 + 1)*ChessBlockSide;
	y1 = (y1 + 1)*ChessBlockSide;
	pDC->MoveTo(x+20,y-20);
	pDC->LineTo(x+20,y1+20);
	pDC->LineTo(x1-20, y1+20);
	pDC->LineTo(x1-20, y-20);
	pDC->LineTo(x+20, y-20);
}

void CMFCFivePointsChessDlg::DrawSkill1SelectRect(CDC* pDC, Direction direction,COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_DASH, 5, color);
	pDC->SelectObject(&pen);
	int x = SelectX / ChessBlockSide - 1;
	int y = SelectY / ChessBlockSide - 1;
	int x1 = x;
	int y1 = y;
	switch (direction)
	{
	case Left:
		pDC->MoveTo(20, SelectY - 20);
		pDC->LineTo(620, SelectY - 20);
		pDC->LineTo(620, SelectY +20);
		pDC->LineTo(20, SelectY + 20);
		pDC->LineTo(20, SelectY -20);
		break;
	case RightUp:

		while (theApp.CalculatePointDiretion(&x,&y,RightUp)){}
		x = (x+1)* ChessBlockSide ;
		y = (y+1) * ChessBlockSide ;
		while (theApp.CalculatePointDiretion(&x1, &y1, LeftDown)){}
		x1 = (x1+1)*ChessBlockSide;
		y1 = (y1 + 1)*ChessBlockSide;
		pDC->MoveTo(x+15, y-15);
		pDC->LineTo(x+15, y+15);
		pDC->LineTo(x1+15, y1 + 15);
		pDC->LineTo(x1 - 15, y1 + 15);
		pDC->LineTo(x1 - 15, y1 - 15);
		pDC->LineTo(x - 15, y - 15);
		pDC->LineTo(x + 15, y - 15);
		break;
	case LeftUp:
		while (theApp.CalculatePointDiretion(&x, &y, LeftUp)) {}
		x = (x + 1)* ChessBlockSide;
		y = (y + 1) * ChessBlockSide;
		while (theApp.CalculatePointDiretion(&x1, &y1, RightDown)) {}
		x1 = (x1 + 1)*ChessBlockSide;
		y1 = (y1 + 1)*ChessBlockSide;
		pDC->MoveTo(x + 15, y - 15);
		pDC->LineTo(x1 + 15, y1 - 15);
		pDC->LineTo(x1 + 15, y1 + 15);
		pDC->LineTo(x1 - 15, y1 + 15);
		pDC->LineTo(x - 15, y + 15);
		pDC->LineTo(x - 15, y - 15);
		pDC->LineTo(x + 15, y - 15);
		break;
	case Up:
		pDC->MoveTo(SelectX - 20, 20);
		pDC->LineTo(SelectX + 20, 20);
		pDC->LineTo(SelectX + 20, 620);
		pDC->LineTo(SelectX -20, 620);
		pDC->LineTo(SelectX - 20, 20);
		break;
	}
}
void CMFCFivePointsChessDlg::ReDrawMessage(const CString& str)
{
	CWnd* pWnd = GetDlgItem(IDC_MESSAGE);
	CRect rc;
	CFont m_pFont;//创建新的字体
    m_pFont.CreateFont(32,							 // 字体高度
		0,							 // 字体宽度
		0,							 // 字体倾斜角
		0,							 // 字体倾斜角
		600,				         // 字体的粗细
		FALSE,						 // 字体是否为斜体
		FALSE,						 // 字体是否有下划线
		0,							 // 字体是否有删除线
		ANSI_CHARSET,				         // 字体使用的字符集
		OUT_DEFAULT_PRECIS,		     	                 // 指定如何选择合适的字体
		CLIP_DEFAULT_PRECIS,    		                 // 确定裁剪的精度
		DEFAULT_QUALITY,			                 // 怎么样跟选择的字体相符合
		DEFAULT_PITCH | FF_SWISS,	                         // 间距标志和属性标志
		_T("楷体"));
	MessageEdit.SetFont(&m_pFont);
	pWnd->GetWindowRect(&rc);
	ScreenToClient(&rc);
	InvalidateRect(&rc);
	MessageEdit.SetWindowTextW(str);
	UpdateWindow();
}
void CMFCFivePointsChessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
		
		case 1:
		{
			
			break;
		}
	}
}
void CMFCFivePointsChessDlg::DrawAllChess(CMFCFivePointsChessApp* ChessApp)
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			if (ChessApp->ChessViewTag[i][j] != 0)
			{
				continue;
			}
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
			case CANTCHESS:
			{
				//3是坑
				DrawChess(Empty, (i + 1)*ChessBlockSide - ChessBlockSide / 2, (j + 1)*ChessBlockSide - ChessBlockSide / 2);
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
	case Empty:
	{
		bmp.LoadBitmapW(IDB_EMPTY);//加载位图资源，将位图资源加载位图对象中
		break;
	}
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
		if (theApp.GameMode != -1)
		{


			switch (pMsg->wParam)
			{
			case VK_SPACE:
			{
				button.SetFocus();
				//落子 

				CString str;
				if (theApp.GetSkill() == noUse)
				{


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
						if (theApp.GameMode == 0)
						{
							//黑色下棋
							if (theApp.TryAddChess(SelectX, SelectY, Black))
							{
								//如果成功下棋那么就切换下棋手
								str = "请白方下子";
								theApp.ChangeFlag(2);
								break;
							}
							if (theApp.GameWiner == Empty)
							{
								return TRUE;
							}
							str = "游戏结束！》》按空格";
							ReDrawMessage(str);
							DrawSelectRect();
							return TRUE;
						}
						else
						{
							if (theApp.GameMode == 1)
							{
								if (theApp.TryAddChess(SelectX, SelectY, Black))
								{
									DrawSelectRect();
									//如果成功下棋那么就切换下棋手
									str = "电脑正在下子";
									theApp.SetAI_Station(await);
									theApp.ChangeFlag(2);
									ReDrawMessage(str);
									Sleep(300);
									int x, y;
									str = "思考中......";
									ReDrawMessage(str);
									Sleep(500);
									if (theApp.AI_FindBestPoint(&x, &y))
									{
										//进行下棋
										SelectX = (x + 1)*ChessBlockSide;
										SelectY = (y + 1)*ChessBlockSide;
										DrawSelectRect();
										Sleep(150);
										if (!theApp.TryAddChess(SelectX, SelectY, White))
										{
											if (theApp.GameWiner == Empty)
											{
												return TRUE;
											}
											str = "游戏结束！》》按空格";
											ReDrawMessage(str);
											DrawSelectRect();
											return TRUE;
										}

										DrawSelectRect();
										str = "请玩家下子";
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
						if (theApp.GameMode == 0)
						{
							if (theApp.TryAddChess(SelectX, SelectY, White))
							{
								//如果成功下棋那么就切换下棋手
								str = "请黑方下子";
								theApp.ChangeFlag(1);
								break;
							}
							if (theApp.GameWiner == Empty)
							{
								return TRUE;
							}
							str = "游戏结束！》》按空格";
							ReDrawMessage(str);
							DrawSelectRect();
							return TRUE;
						}

					}
					default:
						break;
					}
				}
				//下面一定不不包括Skill nouse的情况
				else
	            {
				switch (theApp.GetSkill())
				{
				case skill1:
					str = "排";
					ReDrawMessage(str);
					Sleep(200+theApp.randInt(100,300));
					str = "山";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(100, 300));
					str = "倒";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(100, 300));
					str = "海";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(200, 300));
					theApp.Skill1Effect(SelectX/ChessBlockSide-1, SelectY / ChessBlockSide - 1);
					str = "技能释放完毕！！";
					ReDrawMessage(str);
					Sleep(400);
					if (theApp.GetFlag() == 1)
					{
						str = "请白方下子";
					}
					else
					{
						str = "请黑方下子";
					}
					break;
				case skill2:
					if (theApp.isSelectTimeRect)
					{
						str = "时";
						ReDrawMessage(str);
						Sleep(200 + theApp.randInt(100, 300));
						str = "空";
						ReDrawMessage(str);
						Sleep(200 + theApp.randInt(100, 300));
						str = "据";
						ReDrawMessage(str);
						Sleep(200 + theApp.randInt(100, 300));
						str = "阵";
						ReDrawMessage(str);
						Sleep(200 + theApp.randInt(200, 300));
						theApp.TargetTimeRectPoint[0] = SelectX / ChessBlockSide - 1;
						theApp.TargetTimeRectPoint[1] = SelectY / ChessBlockSide - 1;
						theApp.Skill2Effect();
						theApp.isSelectTimeRect = false;
						str = "技能释放完毕！！";
						ReDrawMessage(str);
						Sleep(400);
						if (theApp.GetFlag() == 1)
						{
							str = "请白方下子";
						}
						else
						{
							str = "请黑方下子";
						}
						break;
					}
					else
					{
						
							theApp.TimeRectPoint[0] = SelectX / ChessBlockSide - 1;
							theApp.TimeRectPoint[1] = SelectY / ChessBlockSide - 1;
							theApp.Skill2Effect();
							theApp.isSelectTimeRect = true;
							str = "请选择需要移动到的位置！！";
							ReDrawMessage(str);
						return TRUE;
					}
					
				case skill3:
					if (!theApp.TryAddChess(SelectX, SelectY, (PaintType)theApp.GetFlag()))
					{
						return TRUE;
					}
					str = "同";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(100, 300));
					str = "归";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(100, 300));
					str = "于";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(100, 300));
					str = "尽";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(200, 300));
					theApp.Skill3Effect(SelectX / ChessBlockSide - 1, SelectY / ChessBlockSide - 1);
					str = "技能释放完毕！！";
					ReDrawMessage(str);
					DrawSelectRect();
					Sleep(400);
					if (theApp.GetFlag() == 1)
					{
						str = "请白方下子";
					}
					else
					{
						str = "请黑方下子";
					}
					break;
				case skill4:
					
					str = "信";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(100, 300));
					str = "号";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(100, 300));
					str = "屏";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(100, 300));
					str = "蔽";
					ReDrawMessage(str);
					Sleep(200 + theApp.randInt(200, 300));
					theApp.Skill4Effect(SelectX / ChessBlockSide - 1, SelectY / ChessBlockSide - 1,6);
					str = "技能释放完毕！！";
					DrawSelectRect();
					ReDrawMessage(str);
					Sleep(400);
					if (theApp.GetFlag() == 1)
					{
						str = "请白方下子";
					}
					else
					{
						str = "请黑方下子";
					}
					break;
				
				}
				theApp.SetSkill(noUse);
				//到对方下子
				
				theApp.ChangeFlag(3 - theApp.GetFlag());
				
               	}
				ReDrawMessage(str);
				DrawSelectRect();
				theApp.CD_Skill_Black--;
				theApp.CD_Skill_White--;
				theApp.TimeRunCDOfViewChessTag();

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
			//Q
			case 81:
			{
				if (theApp.GetSkill() != noUse)
				{
					theApp.ChangeSelectDirection(true);
					DrawSelectRect();
				}
				break;
			}
			//E
			case 69:
			{
				if (theApp.GetSkill() != noUse)
				{
					theApp.ChangeSelectDirection(false);
					DrawSelectRect();
				}
				break;
			}
			//C
			case 67:
			{
				if (theApp.GetSkill() != noUse)
				{
					theApp.SetSkill(noUse);
					theApp.isSelectTimeRect = false;
					DrawSelectRect();
					CString str;
					str = "技能取消";
					ReDrawMessage(str);
					Sleep(400);
					if (theApp.GetFlag() == 1)
					{
						str = "请白方下子";
					}
					else
					{
						str = "请黑方下子";
					}
					ReDrawMessage(str);
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

		

			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//当玩家按下悔棋
void CMFCFivePointsChessDlg::OnClickedRegret()
{

	CString str;
	if (theApp.GameMode== 0 )
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

		ReDrawMessage(str);
		DrawSelectRect();
	}
	else
	{
		if (theApp.GameMode > 0)
		{
			while (theApp.Regret()==White)
			{
			}
			theApp.SetAI_Station(sad);
			str = "哒咩！！";
			theApp.ChangeFlag(1);
			ReDrawMessage(str);
			DrawSelectRect();
		}
	}
	
}


void CMFCFivePointsChessDlg::OnChanegeGameModeEasy()
{

	// TODO: 在此添加命令处理程序代码
	theApp.GameMode = 1;
	theApp.SetAI_Station(welcome); 
	CString str;
	str = "人机模式：你是黑方，我是白方\r\n 按空格下子 WASD移动 请多多指教";
	ReDrawMessage(str);

}


void CMFCFivePointsChessDlg::OnChangeGameModePP()
{
	theApp.GameMode = 0;
	DrawSelectRect();
	CString str;
	str = "人人模式：黑方先手，白方后手 \r\n 按空格下子 \r\n 黑方：WASD移动 白方：方向键移动";
	ReDrawMessage(str);
	// TODO: 在此添加命令处理程序代码
}


void CMFCFivePointsChessDlg::OnBnClickedRestart()
{
	
	theApp.init();
	theApp.initViewChessTag();
	theApp.GameMode = -1;
	RegretButton.EnableWindow(true);
	CString str;
	str = "请选择模式!!!";
	theApp.SetAI_Station(welcome);
	ReDrawMessage(str);
	SelectY = SelectX = ChessBlockNum / 2 * ChessBlockSide;
	DrawSelectRect();

	// TODO: 在此添加控件通知处理程序代码
}


void CMFCFivePointsChessDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	theApp.freeRecord();
	// TODO: 在此处添加消息处理程序代码
}


void CMFCFivePointsChessDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 1370; // 设定最小跟踪宽度

	lpMMI->ptMinTrackSize.y = 914; // 设定最小跟踪高度

	lpMMI->ptMaxTrackSize.x = 1370; // 设定最大跟踪宽度

	lpMMI->ptMaxTrackSize.y = 914; // 设定最大跟踪高度
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


HBRUSH CMFCFivePointsChessDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_MESSAGE)
	{
		//设置背景透明
		/*pDC->SetBkMode(TRANSPARENT); */
		pDC->SetTextColor(RGB(240, 50, 50));
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

/*-----------------技能区---------------------------*/
void CMFCFivePointsChessDlg::OnSkill1()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	
	switch(theApp.GameMode)
	{
	case -1:
		str = "请在人人模式下使用技能";
		break;
	case 0:
		int* CD;
		if (theApp.GetFlag()==1)
		{
			CD = &theApp.CD_Skill_Black;
		}
		else
		{
			CD = &theApp.CD_Skill_White;
		}
		if ((*CD)<0)
		{
		
		RegretButton.EnableWindow(FALSE);
		str = "排山倒海\n任意一排或一列进行棋子反转 \n注意：太贪心会导致施法失败哦！ \n按Q或者E进行旋转 空格施法\n";
		theApp.SetSkill(skill1);
		DrawSelectRect();
		*CD = CDTIME;
		}
		else
		{
			str = "技能冷却中";
		}
		break;
	case 1: case 2:
		str = "该模式无法使用技能";
		break;
	}
	ReDrawMessage(str);
}


void CMFCFivePointsChessDlg::OnSkill2()
{
	CString str;
	
	switch (theApp.GameMode)
	{
	case -1:
		str = "请在人人模式下使用技能";
		break;
	case 0:
		int* CD;
		if (theApp.GetFlag() == 1)
		{
			CD = &theApp.CD_Skill_Black;
		}
		else
		{
			CD = &theApp.CD_Skill_White;
		}
		if ((*CD) < 0)
		{
			RegretButton.EnableWindow(FALSE);
			str = "时空矩阵\n 对9格块进行时空传送\n注意：太贪心会导致施法失败\n传送距离太近会空间混乱哦\n未选择区域时选择方块与本方同色";
			theApp.SetSkill(skill2);
			DrawSelectRect();
			*CD = CDTIME;
		}
		else
		{
			str = "技能冷却中";
		}
		break;
	case 1: case 2:
		str = "该模式无法使用技能";
		break;
	}
	ReDrawMessage(str);
	// TODO: 在此添加命令处理程序代码
}


void CMFCFivePointsChessDlg::OnSkill3()
{
	
	// TODO: 在此添加命令处理程序代码
	CString str;

	switch (theApp.GameMode)
	{
	case -1:
		str = "请在人人模式下使用技能";
		break;
	case 0:
		int* CD;
		if (theApp.GetFlag() == 1)
		{
			CD = &theApp.CD_Skill_Black;
		}
		else
		{
			CD = &theApp.CD_Skill_White;
		}
		if ((*CD) < 0)
		{
			RegretButton.EnableWindow(FALSE);
			str = "同归于尽\n 本方下一个棋子并对附近棋子造成爆炸伤害 并留下一个坑\n注意：不要炸死自己哦";
			theApp.SetSkill(skill3);
			DrawSelectRect();
			*CD = CDTIME;
		}
		else
		{
			str = "技能冷却中";
		}
		break;
	case 1: case 2:
		str = "该模式无法使用技能";
		break;
	}
	ReDrawMessage(str);
}


void CMFCFivePointsChessDlg::OnSkill4()
{
	
	// TODO: 在此添加命令处理程序代码
	CString str;

	switch (theApp.GameMode)
	{
	case -1:
		str = "请在人人模式下使用技能";
		break;
	case 0:
		int* CD;
		if (theApp.GetFlag() == 1)
		{
			CD = &theApp.CD_Skill_Black;
		}
		else
		{
			CD = &theApp.CD_Skill_White;
		}
		if ((*CD) < 0)
		{
			RegretButton.EnableWindow(FALSE);
			str = "信号屏蔽\n 在9格范围内发射天幕 使其与外界失去联系\n注意：被隐身的空格扔可以正常下棋 只是单纯看不见";
			theApp.SetSkill(skill4);
			DrawSelectRect();
			*CD = CDTIME;
		}
		else
		{
			str = "技能冷却中";
		}
		break;
	case 1: case 2:
		str = "该模式无法使用技能";
		break;
	}
	ReDrawMessage(str);
}
/*-----------------技能区结束---------------------------*/