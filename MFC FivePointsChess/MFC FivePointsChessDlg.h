
// MFC FivePointsChessDlg.h: 头文件
//

#pragma once
#include"publicResource.h"
// CMFCFivePointsChessDlg 对话框
class CMFCFivePointsChessDlg : public CDialogEx
{
// 构造
public:
	CMFCFivePointsChessDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCFIVEPOINTSCHESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	void DrawBaclgroundAcorrdingSituation();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	std::vector<CRect>ButtonPos;
	//文字输出暂时储存
	CString Public_Str;
	//屏幕缩放比例
	double size=1;
	double Old_Size = 1;
	//选择落子的方块的矩形位置
	//相对于棋盘的下标
	//x下标
	int SelectX;
	//y下标
	int SelectY;
	//用于记录计分板的参数
	int BlackWinCount = 0;
	int WhiteWinCount = 0;
	int GameRunCount = 0;
	void DrawBackground(const int & choose);
	//用来纠正选择点的位置防止出边界
	void JudgeSelectPos(Direction dir);
	void DrawSelectRectAndChess();
	CStatic select;
	CButton button;
	CStatic MessageText;
	CButton RegretButton;
	CEdit MessageEdit;
	void DrawSkill2SelectRect(CDC * pDC, COLORREF color);
	void DrawSkill1SelectRect(CDC * pDC, Direction direction, COLORREF color);
	void ReDrawMessage(const CString & str);
	void SizeChanged(int ClientX, int ClientY);
	void OnDraw();
	void DrawAllChess(CDC * pDC);
	void DrawChess(CDC * pDC, PaintType type, int PosX, int PosY);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//按下提示
	afx_msg void OnBnClickedTips();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClickedRegret();
	afx_msg void OnChanegeGameModeEasy();
	afx_msg void OnChangeGameModePP();
	afx_msg void OnBnClickedRestart();
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	bool UseSkill();
	afx_msg void OnSkill1();
	afx_msg void OnSkill2();
	afx_msg void OnSkill3();
	afx_msg void OnSkill4();
	afx_msg void OnRule();
	afx_msg void OnControll();
	afx_msg void OnChanegeGameModeMiddle();

	afx_msg void OnViewScore();
	void ShowScoreDlg();
	afx_msg void OnExitGame();
	afx_msg void OnDiff();
	afx_msg void OnCc();
};
