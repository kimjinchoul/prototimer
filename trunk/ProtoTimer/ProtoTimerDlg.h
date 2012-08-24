// ProtoTimerDlg.h : header file
//

#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"


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
	//CMonthCalCtrl m_calctrl;
	//CEdit m_timevalue;
	//CButton m_button;
	const int SECONDS_MAX;
	const int SECONDS_NOTIFY;

	int m_seconds_cur;
	int m_min_x0;
	int m_min_0x;
	int m_sec_x0;
	int m_sec_0x;
	
	bool m_bstart;

	//Draw
	CBrush  m_whitebrush;
	CStatic m_picbox_min_x0;
	CStatic m_picbox_min_0x;
	CStatic m_picbox_sec_x0;
	CStatic m_picbox_sec_0x;

	//Digit number pictures
	CBitmap m_bmp[10];
	CBitmap m_bmp_none;

	//Taskbar notify
	NOTIFYICONDATA m_notify_icon;
	bool m_benable_notify;

	//Configure
	CString m_config_mp3path;
	CString m_config_cmdline;
	CString m_config_argument;
	CButton m_button_close;

	//Start or stop timer.
	void ChangeTimerStatus();
	void UpdateDigitClock();

	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nID);

	//Let all transparent
	afx_msg HBRUSH OnCtlColor(CDC* pDC,	CWnd* pWnd,	UINT nCtlColor);

	//Reseverd
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	//Mouse indicate
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonClose();

	//System lock or unlock
	afx_msg LRESULT OnSessionChange(WPARAM wParam, LPARAM lParam);

	//Taskbar notify
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam,LPARAM IParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InitNotify();
	void CheckNotify();
	void FiniNotify();

	//Timeout
	void CheckTimout();
};
