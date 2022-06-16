
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
	int GameMode;//0人人//1是人机简单 //2是人机困难
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
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
	//用来纠正选择点的位置防止出边界
	void JudgeSelectPos(Direction dir);
	void DrawSelectRect();
	afx_msg void OnStnClickedBackground();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStnClickedPoint();
	CStatic select;
	CStatic text;
	CButton button;
	CStatic MessageText;
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
};
