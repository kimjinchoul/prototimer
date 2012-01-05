// ProtoTimerDlg.h : header file
//

#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ocx1.h"


// CProtoTimerDlg dialog
class CProtoTimerDlg : public CDialog
{
// Construction
public:
	CProtoTimerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PROTOTIMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMonthCalCtrl m_calctrl;
	CEdit m_timevalue;
	CButton m_button;
	int m_seconds_max;
	int m_seconds_cur;
	bool m_bstart;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nID);
	afx_msg HBRUSH OnCtlColor(CDC* pDC,
		CWnd* pWnd,
		UINT nCtlColor);
	afx_msg BOOL OnMouseWheel(UINT nFlags,
		short zDelta, CPoint pt);
	CBrush  m_whitebrush;
	CStatic m_picbox_min_x0;
	CStatic m_picbox_min_0x;
	CBitmap m_bmp[10];
	CStatic m_picbox_sec_x0;
	CStatic m_picbox_sec_0x;
	COcx1 m_wmp;
	NOTIFYICONDATA m_notify_icon;
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam,LPARAM IParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	bool m_bminmal;
};
