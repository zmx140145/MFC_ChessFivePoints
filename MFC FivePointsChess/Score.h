#pragma once


// Score 对话框

class Score : public CDialogEx
{
	DECLARE_DYNAMIC(Score)

public:
	Score(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Score();
	int BlackWinCount=0;
	int WhiteWinCount=0;
	int GameRunCount=0;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCORE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic BlackScoreText;
	CStatic WhiteScoreText;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
