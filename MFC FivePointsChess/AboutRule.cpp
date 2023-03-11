// AboutRule.cpp: 实现文件
//

#include "pch.h"
#include "MFC FivePointsChess.h"
#include "AboutRule.h"
#include "afxdialogex.h"


// AboutRule 对话框

IMPLEMENT_DYNAMIC(AboutRule, CDialogEx)

AboutRule::AboutRule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RULE, pParent)
{

}

AboutRule::~AboutRule()
{
}

void AboutRule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKILLCD, Edit_SkillCD);
	DDX_Control(pDX, IDC_SKILL4CD, Edit_Skill4CD);
}


BEGIN_MESSAGE_MAP(AboutRule, CDialogEx)
	ON_BN_CLICKED(IDC_CONFIRM, &AboutRule::OnBnClickedConfirm)
END_MESSAGE_MAP()


// AboutRule 消息处理程序


void AboutRule::OnBnClickedConfirm()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	Edit_Skill4CD.GetWindowTextW(str);
	char c;

	if (!str.IsEmpty())
	{
		int m_Skill4CD = 0;
		for (int i = 0; i < str.GetLength(); i++)
		{
			c = str[i];
			m_Skill4CD = 10 * m_Skill4CD + int(c) - 48;
		}
		theApp->Skill4ActiveTime = 2*m_Skill4CD + 1;
	}
	Edit_SkillCD.GetWindowTextW(str);
	if (!str.IsEmpty())
	{
		int m_SkillCD = 0;
		for (int i = 0; i < str.GetLength(); i++)
		{
			c = str[i];
			m_SkillCD = 10 * m_SkillCD + int(c) - 48;
		}
	
		theApp->SkillCDTime = m_SkillCD + 1;
	}
	
	EndDialog(0);
}
