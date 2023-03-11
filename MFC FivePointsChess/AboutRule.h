#pragma once


// AboutRule 对话框

class AboutRule : public CDialogEx
{
	DECLARE_DYNAMIC(AboutRule)

public:
	AboutRule(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AboutRule();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCFivePointsChessApp* theApp;
	CEdit Edit_SkillCD;
	CEdit Edit_Skill4CD;
	afx_msg void OnBnClickedConfirm();
};