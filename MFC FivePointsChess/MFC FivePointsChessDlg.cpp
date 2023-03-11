
// MFC FivePointsChessDlg.cpp: 实现文件
//
#include <windows.h>
#include "pch.h"
#include "framework.h"
#include "MFC FivePointsChess.h"
#include "MFC FivePointsChessDlg.h"
#include "afxdialogex.h"
#include "AboutControll.h"
#include "AboutRule.h"
#include "Score.h"
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
	ON_COMMAND(ID_32784, &CMFCFivePointsChessDlg::OnRule)
	ON_COMMAND(ID_32785, &CMFCFivePointsChessDlg::OnControll)
	ON_COMMAND(ID_32776, &CMFCFivePointsChessDlg::OnChanegeGameModeMiddle)
//	ON_WM_DRAWITEM()
ON_COMMAND(ID_32786, &CMFCFivePointsChessDlg::OnViewScore)
ON_COMMAND(ID_32787, &CMFCFivePointsChessDlg::OnExitGame)
ON_COMMAND(ID_DIF, &CMFCFivePointsChessDlg::OnDiff)
ON_COMMAND(ID_CC, &CMFCFivePointsChessDlg::OnCc)
END_MESSAGE_MAP()


// CMFCFivePointsChessDlg 消息处理程序
//初始化时设置定时器 检测大小变化
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
	CRect   temprect(0, 0, 1370, 910);
	CWnd::SetWindowPos(NULL, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	CWnd::CenterWindow();
	SelectY = SelectX = 7;
	button.SetFocus();
	theApp.GameMode = -1;
	Public_Str = "请选择模式!!!";
	theApp.initViewChessTag();
	theApp.SetAI_Station(welcome);
	ReDrawMessage(Public_Str);
	
	SetTimer(1, 50, NULL);

	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCFivePointsChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
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
//相当与视图文档的ondraw 
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
		//重画整个窗口
		//并不是每次都调用OnPaint 如当移动选择框的时候只重画棋盘
		DrawBaclgroundAcorrdingSituation();

		CDialogEx::OnPaint();
	}
}
void CMFCFivePointsChessDlg::OnDraw()
{
	DrawSelectRectAndChess();
}
//定时器
void CMFCFivePointsChessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{

	case 1:
	{
		//判断窗口是否被改变
		Old_Size = size;
		CRect rect;
		GetClientRect(&rect);
		if (rect.Width() < 1370 && rect.Height() < 910)
		{
			if (1370 * size - rect.Width() > 910 * size - rect.Height())
			{
				size = rect.Width() / 1370.0;
				if (size != Old_Size)
				{
					SizeChanged(rect.Width(), rect.Height());
					Invalidate();
				}
				return;
			}
			else
			{
				size = rect.Height() / 910.0;
				if (size != Old_Size)
				{
					SizeChanged(rect.Width(), rect.Height());
					Invalidate();
				}
				return;
			}
		}
		else
		{
			if (rect.Width() < 1370)
			{
				size = rect.Width() / 1370.0;
				if (size != Old_Size)
				{
					SizeChanged(rect.Width(), rect.Height());
					Invalidate();
				}
				return;
			}
			if (rect.Height() < 910)
			{
				size = rect.Height() / 910.0;
				if (size != Old_Size)
				{
					SizeChanged(rect.Width(), rect.Height());
					Invalidate();
				}
				return;
			}
			size = 1;
			if (size != Old_Size)
			{
				SizeChanged(rect.Width(), rect.Height());
				Invalidate();

			}



		}
		break;
	}
	}
}
//重绘文字
void CMFCFivePointsChessDlg::ReDrawMessage(const CString& str)
{
	CWnd* pWnd = GetDlgItem(IDC_MESSAGE);
	CRect rc{ (int)(660 * size),0,(int)(1370 * size),(int)(910 * size) };
	CFont m_pFont;//创建新的字体
	m_pFont.CreateFont(32 * size,							 // 字体高度
		0,							 // 字体宽度
		0,							 // 字体倾斜角
		0,							 // 字体倾斜角
		600 * size,				         // 字体的粗细
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
	InvalidateRect(&rc);
	MessageEdit.SetWindowTextW(str);
	UpdateWindow();
	DrawBaclgroundAcorrdingSituation();
}
//屏幕大小发生改变
void CMFCFivePointsChessDlg::SizeChanged(int ClientX, int ClientY)
{
	CWnd *pWind;
	pWind = GetDlgItem(IDC_TIPS);
	CRect rect;
	pWind->GetWindowRect(&rect);
	pWind->SetWindowPos(NULL, 50 * size, 750 * size, 150 * size, 90 * size, SWP_NOZORDER);
	pWind = GetDlgItem(IDC_REGRET);
	pWind->SetWindowPos(NULL, 280 * size, 750 * size, 150 * size, 90 * size, SWP_NOZORDER);
	pWind = GetDlgItem(IDC_RESTART);
	pWind->SetWindowPos(NULL, 510 * size, 750 * size, 150 * size, 90 * size, SWP_NOZORDER);
	pWind = GetDlgItem(IDC_MESSAGE);
	pWind->SetWindowPos(NULL, 800 * size, 650 * size, 400 * size, 500 * size, SWP_NOZORDER);
	ReDrawMessage(Public_Str);
}
//根据情况选择背景并调用DrawBackGround
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
//画整个背景 那么就要先画背景再画棋盘 棋子 选择框
void CMFCFivePointsChessDlg::DrawBackground(const int& choose)
{
	
	CPaintDC   dc(this);
	CRect   rect;
	CRect   rect1{0,0,1370,910};
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
	dc.FillSolidRect(&rect1, RGB(240, 240, 240));
	dc.StretchBlt(640*size, 0, (1370-640)*size, 910*size, &dcMem, 640, 0,bitmap.bmWidth-640, bitmap.bmHeight, SRCCOPY);
	DrawSelectRectAndChess();
}
void CMFCFivePointsChessDlg::JudgeSelectPos(Direction dir)
{
	switch (dir)
	{
	case Left:
		SelectX -= 1;
		break;
	case Right:
		SelectX +=1 ;
		break;
	case Up:
		SelectY -=1 ;
		break;
	case Down:
		SelectY += 1;
		break;
	default:
	{
		break;
	}
		
	}
	if (SelectX <0)
	{
		SelectX = 0;
	}
	if (SelectX > ChessBlockNum-2)
	{
		SelectX = ChessBlockNum - 2;
	}
	if (SelectY < 0)
	{
		SelectY = 0;
	}
	if (SelectY > ChessBlockNum - 2)
	{
		SelectY = ChessBlockNum-2;
	}
	
}
//选择框和棋盘的绘制 里面统一调用了 棋盘绘制和 技能框选择框的绘制 棋子的绘制
void CMFCFivePointsChessDlg::DrawSelectRectAndChess()
{

	
	CDC* pDC = GetDC();
	CDC MemDC;
	CBitmap MemBitmap;
	/*MemBitmap.LoadBitmapW(IDB_CHESSBOARD);*/
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC, 640 * size, 640 * size);
	///////
	
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	CRect *r1 = new CRect(0, 0, ChessBlockNum*ChessBlockSide*size, ChessBlockNum*ChessBlockSide*size);
	/*-------------------------画棋盘底色-------------------------*/
	MemDC.FillSolidRect(*r1, RGB(234, 179, 99));
	CPen pen;
	//黑色笔
	pen.CreatePen(PS_DASH, max(1,3*size), RGB(10, 10, 10));
	MemDC.SelectObject(&pen);
	/*-------------------------画棋盘线-------------------------*/
	for (int i = 0; i < 15; i++)
	{
		int start = ChessBlockSide * size * (i + 1);

		MemDC.MoveTo(start, ChessBlockSide*size);
		MemDC.LineTo(start, (ChessBlockNum - 1)*ChessBlockSide*size);
		MemDC.MoveTo(ChessBlockSide*size, start);
		MemDC.LineTo((ChessBlockNum - 1)*ChessBlockSide*size, start);
	}
	MemDC.MoveTo((ChessBlockNum - 1)*ChessBlockSide*size, ChessBlockSide*size);
	MemDC.LineTo((ChessBlockNum - 1)*ChessBlockSide*size, (ChessBlockNum - 1)*ChessBlockSide*size);
	MemDC.MoveTo(ChessBlockSide*size, (ChessBlockNum - 1)*ChessBlockSide*size);
	MemDC.LineTo((ChessBlockNum - 1)*ChessBlockSide*size, (ChessBlockNum - 1)*ChessBlockSide*size);

	///////
	free(r1);
	pen.DeleteObject();
	CBrush MyBrush(RGB(10, 10, 10)), *pOldBrush;
	pOldBrush = MemDC.SelectObject(&MyBrush);
	/*-------------------------画棋盘标志圆点-------------------------*/
	//3 3
	MemDC.Ellipse(CRect(((3 + 1)*ChessBlockSide - 4)*size, ((3 + 1)*ChessBlockSide - 4)*size, ((3 + 1)*ChessBlockSide + 4)*size, ((3 + 1)*ChessBlockSide + 4)*size));
	//3 7
	MemDC.Ellipse(CRect(((3 + 1)*ChessBlockSide - 4)*size, ((7 + 1)*ChessBlockSide - 4)*size, ((3+ 1)*ChessBlockSide + 4)*size, ((7 + 1)*ChessBlockSide + 4)*size));
	//3 11
	MemDC.Ellipse(CRect(((3 + 1)*ChessBlockSide - 4)*size, ((11 + 1)*ChessBlockSide - 4)*size, ((3 + 1)*ChessBlockSide + 4)*size, ((11 + 1)*ChessBlockSide + 4)*size));
	//7 3
	MemDC.Ellipse(CRect(((7 + 1)*ChessBlockSide - 4)*size, ((3 + 1)*ChessBlockSide - 4)*size, ((7 + 1)*ChessBlockSide + 4)*size, ((3 + 1)*ChessBlockSide + 4)*size));
	//7 7
	MemDC.Ellipse(CRect(((7 + 1)*ChessBlockSide - 4)*size, ((7 + 1)*ChessBlockSide - 4)*size, ((7 + 1)*ChessBlockSide + 4)*size, ((7 + 1)*ChessBlockSide + 4)*size));
	//7 11
	MemDC.Ellipse(CRect(((7 + 1)*ChessBlockSide - 4)*size, ((11 + 1)*ChessBlockSide - 4)*size, ((7 + 1)*ChessBlockSide + 4)*size, ((11 + 1)*ChessBlockSide + 4)*size));
	//11 3
	MemDC.Ellipse(CRect(((11 + 1)*ChessBlockSide - 4)*size, ((3 + 1)*ChessBlockSide - 4)*size, ((11 + 1)*ChessBlockSide + 4)*size, ((3 + 1)*ChessBlockSide + 4)*size));
	//11 7
	MemDC.Ellipse(CRect(((11 + 1)*ChessBlockSide - 4)*size, ((7 + 1)*ChessBlockSide - 4)*size, ((11 + 1)*ChessBlockSide + 4)*size, ((7 + 1)*ChessBlockSide + 4)*size));
	//11 11
	MemDC.Ellipse(CRect(((11 + 1)*ChessBlockSide - 4)*size, ((11 + 1)*ChessBlockSide - 4)*size, ((11 + 1)*ChessBlockSide + 4)*size, ((11 + 1)*ChessBlockSide + 4)*size));
	MemDC.SelectObject(pOldBrush);
	MyBrush.DeleteObject();
	
	/*-------------------------绘制所有棋子-------------------------*/
	DrawAllChess(&MemDC);


	/*-------------------------根据下棋方选择颜色并把选择框绘制-------------------------*/
	COLORREF color;
	switch (theApp.GetFlag())
	{
	case 0:
	{
		color = color = RGB(255, 255, 255);
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

	
	switch (theApp.GetSkill())
	{
	case skill1:
		DrawSkill1SelectRect(&MemDC, theApp.GetSelectDirection(), color);

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
		DrawSkill2SelectRect(&MemDC, color);

		break;
	case skill4:
		DrawSkill2SelectRect(&MemDC, color);

		break;
	default:
		
		pen.CreatePen(PS_DASH, 5*size, color);
		MemDC.SelectObject(&pen);
		MemDC.MoveTo(((SelectX+1)*ChessBlockSide*size- 10*size), (SelectY+1)*ChessBlockSide*size- 20*size);
		MemDC.LineTo((SelectX+1)*ChessBlockSide*size- 20*size, (SelectY+1)*ChessBlockSide*size- 20*size);
		MemDC.LineTo((SelectX+1)*ChessBlockSide*size- 20*size, (SelectY+1)*ChessBlockSide*size- 10*size);
		MemDC.MoveTo((SelectX+1)*ChessBlockSide*size- 20*size, (SelectY+1)*ChessBlockSide*size+ 10*size);
		MemDC.LineTo((SelectX+1)*ChessBlockSide*size- 20*size, (SelectY+1)*ChessBlockSide*size+ 20*size);
		MemDC.LineTo((SelectX+1)*ChessBlockSide*size- 10*size, (SelectY+1)*ChessBlockSide*size+ 20*size);
		MemDC.MoveTo((SelectX+1)*ChessBlockSide*size+ 10*size, (SelectY+1)*ChessBlockSide*size+ 20*size);
		MemDC.LineTo((SelectX+1)*ChessBlockSide*size+ 20*size, (SelectY+1)*ChessBlockSide*size+ 20*size);
		MemDC.LineTo((SelectX+1)*ChessBlockSide*size+ 20*size, (SelectY+1)*ChessBlockSide*size+ 10*size);
		MemDC.MoveTo((SelectX+1)*ChessBlockSide*size+ 20*size, (SelectY+1)*ChessBlockSide*size- 10*size);
		MemDC.LineTo((SelectX+1)*ChessBlockSide*size+ 20*size, (SelectY+1)*ChessBlockSide*size- 20*size);
		MemDC.LineTo((SelectX+1)*ChessBlockSide*size+ 10*size, (SelectY+1)*ChessBlockSide*size- 20*size);
		break;
	}


	pDC->BitBlt(30, 30, 640, 640, &MemDC, 0, 0, SRCCOPY);

	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	ReleaseDC(pDC);
}
//技能234的选择框绘制
void CMFCFivePointsChessDlg::DrawSkill2SelectRect(CDC* pDC, COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_DASH, 5*size, color);
	pDC->SelectObject(&pen);
	int x = SelectX;
	int y = SelectY;
	int x1 = x;
	int y1 = y;
	theApp.CalculatePointDiretion(&x, &y, RightUp);
	theApp.CalculatePointDiretion(&x1, &y1, LeftDown);
	x = (x + 1)* ChessBlockSide*size;
	y = (y + 1) * ChessBlockSide*size;
	x1 = (x1 + 1)*ChessBlockSide*size;
	y1 = (y1 + 1)*ChessBlockSide*size;
	pDC->MoveTo(x+20*size,y-20*size);
	pDC->LineTo(x+20*size,y1+20*size);
	pDC->LineTo(x1-20*size, y1+20*size);
	pDC->LineTo(x1-20*size, y-20*size);
	pDC->LineTo(x+20*size, y-20*size);
}
//技能1的选择框绘制
void CMFCFivePointsChessDlg::DrawSkill1SelectRect(CDC* pDC, Direction direction,COLORREF color)
{
	CPen pen;
	pen.CreatePen(PS_DASH, 5*size, color);
	pDC->SelectObject(&pen);
	int x = SelectX;
	int y = SelectY;
	int x1 = x;
	int y1 = y;
	switch (direction)
	{
	case Left:
		pDC->MoveTo(20*size, (SelectY+1)*ChessBlockSide*size - 20*size);
		pDC->LineTo(620*size, (SelectY+1)*ChessBlockSide*size - 20*size);
		pDC->LineTo(620*size, (SelectY+1)*ChessBlockSide*size +20*size);
		pDC->LineTo(20*size, (SelectY+1)*ChessBlockSide*size + 20*size);
		pDC->LineTo(20*size, (SelectY+1)*ChessBlockSide*size -20*size);
		break;
	case RightUp:

		while (theApp.CalculatePointDiretion(&x,&y,RightUp)){}
		x = (x+1)* ChessBlockSide *size;
		y = (y+1) * ChessBlockSide*size ;
		while (theApp.CalculatePointDiretion(&x1, &y1, LeftDown)){}
		x1 = (x1+1)*ChessBlockSide*size;
		y1 = (y1 + 1)*ChessBlockSide*size;
		pDC->MoveTo(x+15*size, y-15*size);
		pDC->LineTo(x+15*size, y+15*size);
		pDC->LineTo(x1+15*size, y1 + 15*size);
		pDC->LineTo(x1 - 15*size, y1 + 15*size);
		pDC->LineTo(x1 - 15*size, y1 - 15*size);
		pDC->LineTo(x - 15*size, y - 15*size);
		pDC->LineTo(x + 15*size, y - 15*size);
		break;
	case LeftUp:
		while (theApp.CalculatePointDiretion(&x, &y, LeftUp)) {}
		x = (x + 1)* ChessBlockSide*size;
		y = (y + 1) * ChessBlockSide*size;
		while (theApp.CalculatePointDiretion(&x1, &y1, RightDown)) {}
		x1 = (x1 + 1)*ChessBlockSide*size;
		y1 = (y1 + 1)*ChessBlockSide*size;
		pDC->MoveTo(x + 15*size, y - 15*size);
		pDC->LineTo(x1 + 15*size, y1 - 15*size);
		pDC->LineTo(x1 + 15*size, y1 + 15*size);
		pDC->LineTo(x1 - 15*size, y1 + 15*size);
		pDC->LineTo(x - 15*size, y + 15*size);
		pDC->LineTo(x - 15*size, y - 15*size);
		pDC->LineTo(x + 15*size, y - 15*size);
		break;
	case Up:
		pDC->MoveTo((SelectX+1)*ChessBlockSide*size - 20*size, 20*size);
		pDC->LineTo((SelectX+1)*ChessBlockSide*size + 20*size, 20*size);
		pDC->LineTo((SelectX+1)*ChessBlockSide*size + 20*size, 620*size);
		pDC->LineTo((SelectX+1)*ChessBlockSide*size -20*size, 620*size);
		pDC->LineTo((SelectX+1)*ChessBlockSide*size - 20*size, 20*size);
		break;
	}
}

//绘制棋盘
void CMFCFivePointsChessDlg::DrawAllChess(CDC* pDC)
{
	for (int i = 0; i < ChessBlockNum - 1; i++)
	{
		for (int j = 0; j < ChessBlockNum - 1; j++)
		{
			if (theApp.ChessViewTag[i][j] != 0)
			{
				continue;
			}
			switch (theApp.GetChess(i,j))
			{
				//0是没有棋子不用绘制
			case NULLCHESS:
			{

				break;
			}
			case BLACKCHESS:
			{
				//1是黑棋
				DrawChess(pDC,Black, (i + 1)*ChessBlockSide, (j + 1)*ChessBlockSide );
				break;
			}
			case WHITECHESS:
			{
				//2是白棋
				DrawChess(pDC,White, (i + 1)*ChessBlockSide , (j + 1)*ChessBlockSide );
				break;
			}
			case CANTCHESS:
			{
				//3是坑
				DrawChess(pDC,Empty, (i + 1)*ChessBlockSide , (j + 1)*ChessBlockSide );
				break;
			}
			}
		}
	}
}
//绘制所有棋子
void CMFCFivePointsChessDlg::DrawChess(CDC* pDC,PaintType type , int PosX ,int PosY)
{
	
	COLORREF color = RGB(255, 255, 255);
	switch (type)
	{
	case White:
	{
		color = RGB(255, 255, 255);
		break;
	}
	case Black:
	{
		color = RGB(0, 0, 0);
		break;
	}
	case Empty:
	{
		color = RGB(200, 100, 20);
	}
	}
	CBrush MyBrush(color), *pOldBrush;
	pOldBrush = pDC->SelectObject(&MyBrush);
	pDC->Ellipse(CRect((PosX - 18)*size, (PosY - 18)*size, (PosX + 18)*size, (PosY + 18)*size));
	pDC->SelectObject(pOldBrush);
	MyBrush.DeleteObject();
	/*pDC->BitBlt(0, 0, 40, 40, &ImageDC, 0, 0, SRCCOPY);*/
	

}
//按下提示
void CMFCFivePointsChessDlg::OnBnClickedTips()
{
	// TODO: 在此添加控件通知处理程序代码
	int x, y;
	int gameMode = theApp.GameMode;
	int depth = theApp.AI_FindDepth;
	theApp.GameMode= 0;
	theApp.AI_FindDepth = 1;
	theApp.AI_FindBestPoint(&x, &y);
	SelectX = x;
		SelectY = y;
		theApp.AI_FindDepth = depth;
		theApp.GameMode = gameMode;
	DrawSelectRectAndChess();
}
BOOL CMFCFivePointsChessDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return FALSE;
	/*return CDialogEx::OnEraseBkgnd(pDC);*/
}

//消息处理 处理键盘消息 相当于OnKeyDown
BOOL CMFCFivePointsChessDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (theApp.GameMode != -1)
		{


			switch (pMsg->wParam)
			{
			case VK_RETURN:
			{
				button.SetFocus();
				//落子 

			
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
							Public_Str = "请开始游戏";
							break;
						}
						case BLACKCHESS:
						{
							Public_Str = "黑方赢了！";
							ReDrawMessage(Public_Str);
							Sleep(500);
							ShowScoreDlg();
							return TRUE;
							
						}
						case WHITECHESS:
						{
							Public_Str = "白方赢了！";
							ReDrawMessage(Public_Str);
							Sleep(500);
							ShowScoreDlg();
							return TRUE;
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
								Public_Str = "请白方下子";
								theApp.ChangeFlag(2);
								break;
							}
							if (theApp.GameWiner == Empty)
							{
								return TRUE;
							}
							Public_Str = "游戏结束！》》按空格";
							GameRunCount++;
							BlackWinCount++;
							ReDrawMessage(Public_Str);
							OnDraw();
							return TRUE;
						}
						else
						{
							if (theApp.GameMode >= 1&& theApp.GameMode<=2)
							{
								if (theApp.TryAddChess(SelectX, SelectY, Black))
								{
									OnDraw();
									//如果成功下棋那么就切换下棋手
									Public_Str = "电脑正在下子";
									theApp.SetAI_Station(await);
									theApp.ChangeFlag(2);
									ReDrawMessage(Public_Str);
									if (theApp.AI_FindDepth <= 3)
									{
										Sleep(300);
									}
									int x, y;
									Public_Str = "思考中......";
									ReDrawMessage(Public_Str);
									if (theApp.AI_FindDepth <= 3)
									{
										Sleep(500);
									}
									
									if (theApp.AI_FindBestPoint(&x, &y))
									{
										//进行下棋
										SelectX = x;
										SelectY = y;
										OnDraw();
										Sleep(150);
										if (!theApp.TryAddChess(SelectX, SelectY, White))
										{
											if (theApp.GameWiner == Empty)
											{
												return TRUE;
											}
											Public_Str = "游戏结束！》》按空格";
											GameRunCount++;
											WhiteWinCount++;
											ReDrawMessage(Public_Str);
											OnDraw();
											return TRUE;
										}

										OnDraw();
										Public_Str = "请玩家下子";
										theApp.ChangeFlag(1);
										break;
									}
									else
									{
										SelectX = x;
											SelectY = y;
										Sleep(500);
										OnDraw();
										Sleep(150);
										if (!theApp.TryAddChess(SelectX, SelectY, White))
										{
											OnDraw();
											return TRUE;
										}
										OnDraw();
										//求和操作 //TODO:
										theApp.ChangeFlag(1);
										break;

									}

								}
								if (theApp.GameWiner == Empty)
								{
									//玩家下棋失败
									return TRUE;
								}
								//玩家胜利
								Public_Str = "游戏结束！》》按空格";
								GameRunCount++;
							    BlackWinCount++;
								ReDrawMessage(Public_Str);
								OnDraw();
								return TRUE;
							}
							else
							{
								//GameMode 3 机机对战 黑方
								int x, y;
								//更改电脑的算法
								theApp.AI_FindDepth = 1;
								if (theApp.AI_FindBestPoint(&x, &y))
									{
										//进行下棋
										SelectX = x;
										SelectY = y;
										if (!theApp.TryAddChess(SelectX, SelectY,Black))
										{
											if (theApp.GameWiner == Empty)
											{
												return TRUE;
											}
											Public_Str = "游戏结束！》》按空格";
											GameRunCount++;
										    BlackWinCount++;
											ReDrawMessage(Public_Str);
											OnDraw();
											return TRUE;
										}

										OnDraw();
										Public_Str = "请白棋下子";
										theApp.ChangeFlag(2);
										break;
									}
							}
						}

					}
					case 2:
					{
							return TRUE;

					}
					default:
						break;
					}
				}
				//下面一定不不包括Skill nouse的情况
				else
				{
					if (!UseSkill())
					{
						return TRUE;
					}

					theApp.ChangeFlag(3 - theApp.GetFlag());

				}
				ReDrawMessage(Public_Str);
				OnDraw();
				if (theApp.GetFlag() == 1)
				{
					theApp.CD_Skill_Black--;
				}
				else
				{
					theApp.CD_Skill_White--;
				}
				theApp.TimeRunCDOfViewChessTag();
				break;
			}
			case VK_SPACE:
			{
				button.SetFocus();
				//落子 

			
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
							Public_Str = "请开始游戏";
						
							break;
						}
						case BLACKCHESS:
						{
							Public_Str = "黑方赢了！";
							ReDrawMessage(Public_Str);
							Sleep(500);
							ShowScoreDlg();
							return TRUE;
						}
						case WHITECHESS:
						{
							Public_Str = "白方赢了！";
							ReDrawMessage(Public_Str);
							Sleep(500);
							ShowScoreDlg();
							return TRUE;
						}
						default:
							break;
						}

						break;
					}
					case 1:
					{
						return TRUE;
					}
					case 2:
					{
		
						//白色下棋
						if (theApp.GameMode == 0)
						{
							if (theApp.TryAddChess(SelectX, SelectY, White))
							{
								//如果成功下棋那么就切换下棋手
								Public_Str = "请黑方下子";
								theApp.ChangeFlag(1);
								break;
							}
							if (theApp.GameWiner == Empty)
							{
								return TRUE;
							}
							Public_Str = "游戏结束！》》按空格";
							GameRunCount++;
							WhiteWinCount++;
							ReDrawMessage(Public_Str);
							OnDraw();
							return TRUE;
						}
						else if(theApp.GameMode==3)
						{
							//GameMode 3 机机对战 白方
							int x, y;
							theApp.AI_FindDepth = 1;
							if (theApp.AI_FindBestPoint(&x, &y))
							{
								//进行下棋
								SelectX = x;
								SelectY = y;
								if (!theApp.TryAddChess(SelectX, SelectY, White))
								{
									if (theApp.GameWiner == Empty)
									{
										return TRUE;
									}
									Public_Str = "游戏结束！》》按空格";
									GameRunCount++;
									WhiteWinCount++;
									ReDrawMessage(Public_Str);
									OnDraw();
									return TRUE;
								}

								OnDraw();
								Public_Str = "请黑棋下子";
								theApp.ChangeFlag(1);
								break;
							}
						}

					}
					default:
						break;
					}
				}
				//下面一定不不包括Skill nouse的情况
				else
	            {
				if (!UseSkill())
				{
					return TRUE;
				}

				theApp.ChangeFlag(3 - theApp.GetFlag());
				
               	}
				ReDrawMessage(Public_Str);
				OnDraw();
				if (theApp.GetFlag() == 1)
				{
					theApp.CD_Skill_Black--;
				}
				else
				{
					theApp.CD_Skill_White--;
				}
				theApp.TimeRunCDOfViewChessTag();

				//SPACE结束
				break;
			}


			//W
			case 87:
			{
				if (theApp.GetFlag() == 2)
				{
					JudgeSelectPos(Up);
					OnDraw();
				}

				break;
			}

			//A
			case 65:
			{
				if (theApp.GetFlag() == 2)
				{
					JudgeSelectPos(Left);
					OnDraw();
				}

				break;
			}
			//S
			case 83:
			{
				if (theApp.GetFlag() == 2)
				{
					JudgeSelectPos(Down);
					OnDraw();
				}

				break;
			}
			//D
			case 68:
			{
				if (theApp.GetFlag() == 2)
				{
					JudgeSelectPos(Right);
					OnDraw();
				}
				break;
			}
			//Q
			case 81:
			{
				if (theApp.GetSkill() != noUse)
				{
					theApp.ChangeSelectDirection(true);
					OnDraw();
				}
				break;
			}
			//E
			case 69:
			{
				if (theApp.GetSkill() != noUse)
				{
					theApp.ChangeSelectDirection(false);
					OnDraw();
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
					OnDraw();
					
					Public_Str = "技能取消";
					ReDrawMessage(Public_Str);
					Sleep(400);
					if (theApp.GetFlag() == 2)
					{
						theApp.CD_Skill_White = -1;
						Public_Str = "请白方下子";
					}
					else
					{
						theApp.CD_Skill_Black = -1;
						Public_Str = "请黑方下子";
					}
					ReDrawMessage(Public_Str);
				}
				
				break;
			}
			//上
			case VK_UP:
			{
				if (theApp.GetFlag() == 1)
				{
					JudgeSelectPos(Up);
					OnDraw();
				}
				break;
			}

			//左
			case VK_LEFT:
			{
				if (theApp.GetFlag() == 1)
				{
					JudgeSelectPos(Left);
					OnDraw();
				}
				break;
			}
			//下
			case VK_DOWN:
			{
				if (theApp.GetFlag() == 1)
				{
					JudgeSelectPos(Down);
					OnDraw();
				}
				break;
			}
			//右
			case VK_RIGHT:
			{
				if (theApp.GetFlag() == 1)
				{
					JudgeSelectPos(Right);
					OnDraw();
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
			Public_Str = "请白方下子";
			theApp.ChangeFlag(2);
			break;
		}
		case Black:
		{
			Public_Str = "请黑方下子";
			theApp.ChangeFlag(1);
			break;
		}
		}

		ReDrawMessage(Public_Str);
		OnDraw();
	}
	else
	{
		if (theApp.GameMode > 0)
		{
			while (theApp.Regret()==White)
			{
			}
			theApp.SetAI_Station(sad);
			Public_Str = "哒咩！！";
			theApp.ChangeFlag(1);
			ReDrawMessage(Public_Str);
			OnDraw();
		}
	}
	
}
void CMFCFivePointsChessDlg::OnChanegeGameModeEasy()
{
	theApp.init();
	theApp.initViewChessTag();
	theApp.GameMode = 1;
	theApp.AI_FindDepth = 1;
	theApp.SetAI_Station(welcome); 
	OnDraw();
	
	Public_Str.Format(_T("人机模式（简单）：你是黑方，我是白方\r\n 按回车下子 方向键移动 \r\n请多多指教"));
	ReDrawMessage(Public_Str);

}
void CMFCFivePointsChessDlg::OnChanegeGameModeMiddle()
{
	// TODO: 在此添加命令处理程序代码
	theApp.init();
	theApp.initViewChessTag();
	theApp.GameMode = 2;
	theApp.AI_FindDepth = 2;
	theApp.SetAI_Station(welcome);
	OnDraw();
	
	Public_Str .Format(_T("人机模式（中等）：你是黑方，我是白方\r\n 按回车下子 方向键移动 \r\n请多多指教"));
	ReDrawMessage(Public_Str);
}
void CMFCFivePointsChessDlg::OnDiff()
{
	theApp.init();
	theApp.initViewChessTag();
	theApp.GameMode = 2;
	theApp.AI_FindDepth = 3;
	theApp.SetAI_Station(welcome);
	OnDraw();
	Public_Str.Format(_T("人机模式（困难）未完善：你是黑方，我是白方\r\n 按回车下子 方向键移动 \r\n请多多指教"));
	ReDrawMessage(Public_Str);
	// TODO: 在此添加命令处理程序代码
}
void CMFCFivePointsChessDlg::OnChangeGameModePP()
{
	theApp.init();
	theApp.initViewChessTag();
	theApp.GameMode = 0;
	RegretButton.EnableWindow(true);
	OnDraw();

	Public_Str.Format(_T("人人模式：黑方先手，白方后手  \r\n 黑方：方向键移动 按回车下子\r\n白方：WASD移动 按空格下子")) ;
	ReDrawMessage(Public_Str);
	// TODO: 在此添加命令处理程序代码
}
//机机对战
void CMFCFivePointsChessDlg::OnCc()
{
	theApp.init();
	theApp.initViewChessTag();
	theApp.GameMode = 3;
	RegretButton.EnableWindow(true);
	OnDraw();
	Public_Str.Format(_T("机机模式：黑方先手，白方后手  \r\n 黑方： 按回车下子\r\n白方 按空格下子"));
	ReDrawMessage(Public_Str);
	// TODO: 在此添加命令处理程序代码
}
void CMFCFivePointsChessDlg::OnBnClickedRestart()
{
	
	theApp.init();
	theApp.initViewChessTag();
	theApp.GameMode = -1;
	RegretButton.EnableWindow(true);

	Public_Str = "请选择模式!!!";
	theApp.SetAI_Station(welcome);
	ReDrawMessage(Public_Str);
	SelectY = SelectX = 7;
	OnDraw();

	// TODO: 在此添加控件通知处理程序代码
}
void CMFCFivePointsChessDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	theApp.freeRecord();
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	// TODO: 在此处添加消息处理程序代码
}
void CMFCFivePointsChessDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 640; // 设定最小跟踪宽度

	lpMMI->ptMinTrackSize.y = 520; // 设定最小跟踪高度

	lpMMI->ptMaxTrackSize.x = 1370; // 设定最大跟踪宽度

	lpMMI->ptMaxTrackSize.y = 910; // 设定最大跟踪高度
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

	
	switch(theApp.GameMode)
	{
	case -1:
		Public_Str = "请在人人模式下使用技能";
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
		if ((*CD)<=0)
		{
		
		RegretButton.EnableWindow(FALSE);
		Public_Str = "排山倒海\n任意一排或一列进行棋子反转 \n注意：太贪心会导致施法失败哦！ \n按Q或者E进行旋转 空格施法\n";
		theApp.SetSkill(skill1);
		OnDraw();
		*CD = theApp.SkillCDTime;
		}
		else
		{
			Public_Str.Format(_T("技能冷却中,%d回合后可用"), (*CD));
		}
		break;
	case 1: case 2:
		Public_Str = "该模式无法使用技能";
		break;
	}
	ReDrawMessage(Public_Str);
}
void CMFCFivePointsChessDlg::OnSkill2()
{

	
	switch (theApp.GameMode)
	{
	case -1:
		Public_Str = "请在人人模式下使用技能";
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
		if ((*CD) <=0)
		{
			RegretButton.EnableWindow(FALSE);
			Public_Str = "时空矩阵\n 对9格块进行时空传送\n注意：太贪心会导致施法失败\n传送距离太近会空间混乱哦\n未选择区域时选择方块与本方同色";
			theApp.SetSkill(skill2);
			OnDraw();
			*CD = theApp.SkillCDTime;
		}
		else
		{
			Public_Str.Format(_T("技能冷却中,%d回合后可用"), (*CD));
		}
		break;
	case 1: case 2:
		Public_Str = "该模式无法使用技能";
		break;
	}
	ReDrawMessage(Public_Str);
	// TODO: 在此添加命令处理程序代码
}
void CMFCFivePointsChessDlg::OnSkill3()
{
	
	// TODO: 在此添加命令处理程序代码


	switch (theApp.GameMode)
	{
	case -1:
		Public_Str = "请在人人模式下使用技能";
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
		if ((*CD) <=0)
		{
			RegretButton.EnableWindow(FALSE);
			Public_Str = "同归于尽\n 本方下一个棋子并对附近棋子造成爆炸伤害 并留下一个坑\n注意：不要炸死自己哦";
			theApp.SetSkill(skill3);
			OnDraw();
			*CD = theApp.SkillCDTime;
		}
		else
		{
			
			Public_Str.Format(_T("技能冷却中,%d回合后可用"), (*CD) );
		}
		break;
	case 1: case 2:
		Public_Str = "该模式无法使用技能";
		break;
	}
	ReDrawMessage(Public_Str);
}
void CMFCFivePointsChessDlg::OnSkill4()
{
	
	// TODO: 在此添加命令处理程序代码


	switch (theApp.GameMode)
	{
	case -1:
		Public_Str = "请在人人模式下使用技能";
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
		if ((*CD) <=0)
		{
			RegretButton.EnableWindow(FALSE);
			Public_Str = "信号屏蔽\n 在9格范围内发射天幕 使其与外界失去联系\n注意：被隐身的空格扔可以正常下棋 只是单纯看不见";
			theApp.SetSkill(skill4);
			OnDraw();
			*CD = theApp.SkillCDTime;
		}
		else
		{
			Public_Str.Format(_T("技能冷却中,%d回合后可用"), (*CD) );
		}
		break;
	case 1: case 2:
		Public_Str = "该模式无法使用技能";
		break;
	}
	ReDrawMessage(Public_Str);
}
//使用技能 通过这个方法统一调度所有的技能 设置冷却时间 实施效果
bool CMFCFivePointsChessDlg::UseSkill()
{

	switch (theApp.GetSkill())
	{
	case skill1:
		Public_Str = "排";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(100, 300));
		Public_Str = "山";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(100, 300));
		Public_Str = "倒";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(100, 300));
		Public_Str = "海";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(200, 300));
		theApp.Skill1Effect(SelectX , SelectY);
		Public_Str = "技能释放完毕！！";
		ReDrawMessage(Public_Str);
		Sleep(400);
		if (theApp.GetFlag() == 1)
		{
			Public_Str = "请白方下子";
		}
		else
		{
			Public_Str = "请黑方下子";
		}
		break;
	case skill2:
		if (theApp.isSelectTimeRect)
		{
			Public_Str = "时";
			ReDrawMessage(Public_Str);
			Sleep(200 + theApp.randInt(100, 300));
			Public_Str = "空";
			ReDrawMessage(Public_Str);
			Sleep(200 + theApp.randInt(100, 300));
			Public_Str = "据";
			ReDrawMessage(Public_Str);
			Sleep(200 + theApp.randInt(100, 300));
			Public_Str = "阵";
			ReDrawMessage(Public_Str);
			Sleep(200 + theApp.randInt(200, 300));
			theApp.TargetTimeRectPoint[0] = SelectX ;
			theApp.TargetTimeRectPoint[1] = SelectY ;
			theApp.Skill2Effect();
			theApp.isSelectTimeRect = false;
			Public_Str = "技能释放完毕！！";
			ReDrawMessage(Public_Str);
			Sleep(400);
			if (theApp.GetFlag() == 1)
			{
				Public_Str = "请白方下子";
			}
			else
			{
				Public_Str = "请黑方下子";
			}
			break;
		}
		else
		{

			theApp.TimeRectPoint[0] = SelectX ;
			theApp.TimeRectPoint[1] = SelectY ;
			theApp.Skill2Effect();
			theApp.isSelectTimeRect = true;
			Public_Str = "请选择需要移动到的位置！！";
			ReDrawMessage(Public_Str);
			return false;
		}

	case skill3:
		if (!theApp.TryAddChess(SelectX, SelectY, (PaintType)theApp.GetFlag()))
		{
			return false;
		}
		Public_Str = "同";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(100, 300));
		Public_Str = "归";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(100, 300));
		Public_Str = "于";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(100, 300));
		Public_Str = "尽";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(200, 300));
		theApp.Skill3Effect(SelectX , SelectY);
		Public_Str = "技能释放完毕！！";
		ReDrawMessage(Public_Str);
		OnDraw();
		Sleep(400);
		if (theApp.GetFlag() == 1)
		{
			Public_Str = "请白方下子";
		}
		else
		{
			Public_Str = "请黑方下子";
		}
		break;
	case skill4:

		Public_Str = "信";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(100, 300));
		Public_Str = "号";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(100, 300));
		Public_Str = "屏";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(100, 300));
		Public_Str = "蔽";
		ReDrawMessage(Public_Str);
		Sleep(200 + theApp.randInt(200, 300));
		theApp.Skill4Effect(SelectX , SelectY , theApp.Skill4ActiveTime);
		Public_Str = "技能释放完毕！！";
		OnDraw();
		ReDrawMessage(Public_Str);
		Sleep(400);
		if (theApp.GetFlag() == 1)
		{
			Public_Str = "请白方下子";
		}
		else
		{
			Public_Str = "请黑方下子";
		}
		break;

	}
	theApp.SetSkill(noUse);
	//到对方下子
	return true;
}
/*-----------------技能区结束---------------------------*/
//当按下查看规则 创建新的对话框
void CMFCFivePointsChessDlg::OnRule()
{
	// TODO: 在此添加命令处理程序代码
	AboutRule ruleDlg;
	ruleDlg.theApp = &theApp;
	ruleDlg.DoModal();
}
//当按下查看控制按键 创建新的对话框
void CMFCFivePointsChessDlg::OnControll()
{
	// TODO: 在此添加命令处理程序代码
	AboutControll controllDlg;
	controllDlg.DoModal();
}
//按退出游戏
void CMFCFivePointsChessDlg::OnExitGame()
{
	// TODO: 在此添加命令处理程序代码
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
}
//当按下查看成绩时
void CMFCFivePointsChessDlg::OnViewScore()
{
	ShowScoreDlg();
	// TODO: 在此添加命令处理程序代码
}
//查看成绩
void CMFCFivePointsChessDlg::ShowScoreDlg()
{
	Score ScoreDlg;
	ScoreDlg.BlackWinCount = BlackWinCount;
	ScoreDlg.WhiteWinCount = WhiteWinCount;
	ScoreDlg.GameRunCount = GameRunCount;
	ScoreDlg.DoModal();
}




