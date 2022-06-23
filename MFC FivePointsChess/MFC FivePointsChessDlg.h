
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
	
	//选择落子的方块的矩形位置
	//相对于背景的相对坐标
	//x坐标
	int SelectX;
	//y坐标
	int SelectY;
	CStatic ChessBoard;

	void DrawBackground(const int & choose);
	//用来纠正选择点的位置防止出边界
	void JudgeSelectPos(Direction dir);
	void DrawSelectRect();
	CStatic select;
	CButton button;
	CStatic MessageText;

	void DrawSkill2SelectRect(CDC * pDC, COLORREF color);
	void DrawSkill1SelectRect(CDC * pDC, Direction direction, COLORREF color);
	void ReDrawMessage(const CString & str);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DrawAllChess(CMFCFivePointsChessApp* ChessApp);
	void DrawChess(PaintType type, int PosX, int PosY);
	afx_msg void OnBnClickedTips();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	/*afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);*/
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedRegret();
	
	afx_msg void OnChanegeGameModeEasy();
	afx_msg void OnChangeGameModePP();
	CEdit MessageEdit;
	afx_msg void OnBnClickedRestart();
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSkill1();
	afx_msg void OnSkill2();
	afx_msg void OnSkill3();
	afx_msg void OnSkill4();
	CButton RegretButton;
};
