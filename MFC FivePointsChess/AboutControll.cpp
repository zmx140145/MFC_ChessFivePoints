// AboutControll.cpp: 实现文件
//

#include "pch.h"
#include "MFC FivePointsChess.h"
#include "AboutControll.h"
#include "afxdialogex.h"


// AboutControll 对话框

IMPLEMENT_DYNAMIC(AboutControll, CDialogEx)

AboutControll::AboutControll(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONTROLL, pParent)
{

}

AboutControll::~AboutControll()
{
}

void AboutControll::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AboutControll, CDialogEx)
END_MESSAGE_MAP()


// AboutControll 消息处理程序
