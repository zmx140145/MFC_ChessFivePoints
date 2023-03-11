#pragma once


// AboutControll 对话框

class AboutControll : public CDialogEx
{
	DECLARE_DYNAMIC(AboutControll)

public:
	AboutControll(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AboutControll();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROLL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
