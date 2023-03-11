// Score.cpp: 实现文件
//

#include "pch.h"
#include "MFC FivePointsChess.h"
#include "Score.h"
#include "afxdialogex.h"


// Score 对话框

IMPLEMENT_DYNAMIC(Score, CDialogEx)

Score::Score(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCORE, pParent)
{
	
}

Score::~Score()
{
}

void Score::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTB, BlackScoreText);
	DDX_Control(pDX, IDC_TEXTW, WhiteScoreText);
}


BEGIN_MESSAGE_MAP(Score, CDialogEx)
	ON_BN_CLICKED(IDOK, &Score::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Score::OnBnClickedCancel)
END_MESSAGE_MAP()


// Score 消息处理程序


void Score::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnOK();
}


void Score::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();
}


BOOL Score::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	CString str;
	if (GameRunCount == 0)
	{
		str.Format(_T("白方胜率:%d%% \n 胜局：%d "), WhiteWinCount * 100 / 1, WhiteWinCount);
		WhiteScoreText.SetWindowTextW(str);
		str.Format(_T("黑方胜率:%d%% \n 胜局：%d "), BlackWinCount * 100 / 1, BlackWinCount);
		BlackScoreText.SetWindowTextW(str);
	}
	else
	{
		str.Format(_T("白方胜率:%d%% \n 胜局：%d "), WhiteWinCount * 100 / (GameRunCount), WhiteWinCount);
		WhiteScoreText.SetWindowTextW(str);
		str.Format(_T("黑方胜率:%d%% \n 胜局：%d "), 100 - WhiteWinCount * 100 / (GameRunCount), BlackWinCount);
		BlackScoreText.SetWindowTextW(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
