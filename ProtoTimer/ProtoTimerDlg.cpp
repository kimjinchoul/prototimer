// ProtoTimerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoTimer.h"
#include "ProtoTimerDlg.h"

#include "WMPDailog.h"

#include <Wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProtoTimerDlg dialog

#define CONFIG_INI_NAME _T(".\\config.ini")


CProtoTimerDlg::CProtoTimerDlg(CWnd* pParent /*=NULL*/)
	:CDialog(CProtoTimerDlg::IDD, pParent),
	SECONDS_MAX(25 * 60),
	SECONDS_NOTIFY(3 * 60)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_bstart = false;
}

void CProtoTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_MONTHCALENDAR1, m_calctrl);
	//DDX_Control(pDX, IDC_EDIT1, m_timevalue);
	//DDX_Control(pDX, IDC_BUTTON1, m_button);
	DDX_Control(pDX, IDC_STATIC_MIN_X0, m_picbox_min_x0);
	DDX_Control(pDX, IDC_STATIC_MIN_0X, m_picbox_min_0x);
	DDX_Control(pDX, IDC_STATIC_SEC_X0, m_picbox_sec_x0);
	DDX_Control(pDX, IDC_STATIC_SEC_0X, m_picbox_sec_0x);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_button_close);
}

BEGIN_MESSAGE_MAP(CProtoTimerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_NC,OnNotifyIcon)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_WTSSESSION_CHANGE, OnSessionChange)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CProtoTimerDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CProtoTimerDlg message handlers

BOOL CProtoTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//m_timevalue.SetWindowText(_T("25"));
	//m_button.SetWindowText(_T("Start"));

	//Load bmp
	m_bmp[0].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_0));
	m_bmp[1].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_1));
	m_bmp[2].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_2));
	m_bmp[3].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_3));
	m_bmp[4].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_4));
	m_bmp[5].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_5));
	m_bmp[6].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_6));
	m_bmp[7].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_7));
	m_bmp[8].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_8));
	m_bmp[9].LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_9));
	m_bmp_none.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_NONE));

	//reset
	m_picbox_min_x0.SetBitmap((HBITMAP)m_bmp_none);
	m_picbox_min_0x.SetBitmap((HBITMAP)m_bmp_none);

	m_picbox_sec_x0.SetBitmap((HBITMAP)m_bmp_none);
	m_picbox_sec_0x.SetBitmap((HBITMAP)m_bmp_none);

	m_whitebrush.CreateSolidBrush(RGB(255,255,255));

	//reset
	m_benable_notify = false;

	//Alpha dialog
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE,	
		GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)^0x80000);

	//Color which will be alpha
	COLORREF clr(RGB(0, 0, 0));
	SetLayeredWindowAttributes(clr, 200, LWA_ALPHA);
	
	//Get move windows parameter
	int x = GetPrivateProfileInt(_T("ProtoTimer"),
		_T("X"),0,CONFIG_INI_NAME);
	int y = GetPrivateProfileInt(_T("ProtoTimer"),
		_T("Y"),0,CONFIG_INI_NAME);

	//Get parameter
	TCHAR return_strs[4096];
	GetPrivateProfileString(_T("ProtoTimer"),
		_T("mp3"),_T("alert.mp3"), return_strs, 4096,CONFIG_INI_NAME);
	m_config_mp3path = return_strs;

	GetPrivateProfileString(_T("ProtoTimer"),
		_T("cmd"),_T(""), return_strs, 4096,CONFIG_INI_NAME);
	m_config_cmdline = return_strs;

	GetPrivateProfileString(_T("ProtoTimer"),
		_T("arg"),_T(""), return_strs, 4096,CONFIG_INI_NAME);
	m_config_argument = return_strs;

	//Move to last place
	CRect rect;
	GetWindowRect(&rect);
	MoveWindow(x,y,rect.Width(),rect.Height());

	//reset value
	m_seconds_cur = -1;

	m_min_x0 = 0;
	m_min_0x = 0;
	m_sec_x0 = 0;
	m_sec_0x = 0;


	//////////////////////////////////////////////////////////////////////////
	BOOL bret = WTSRegisterSessionNotification(this->GetSafeHwnd(),NOTIFY_FOR_THIS_SESSION);
	if (!bret)
	{
		AfxMessageBox(_T("WTSRegisterSessionNotification Fail"));
	}

	//Start timer
	ChangeTimerStatus();

	//Add taskbar icon
	InitNotify();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProtoTimerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		UpdateDigitClock();
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProtoTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

////////////////////////////////////////////////////////////////////////////////
void CProtoTimerDlg::ChangeTimerStatus()
{
	// TODO: Add your control notification handler code here
	if (m_bstart)
	{
		//Stop......
		KillTimer(0);

		m_picbox_min_x0.SetBitmap((HBITMAP)m_bmp_none);
		m_picbox_min_0x.SetBitmap((HBITMAP)m_bmp_none);

		m_picbox_sec_x0.SetBitmap((HBITMAP)m_bmp_none);
		m_picbox_sec_0x.SetBitmap((HBITMAP)m_bmp_none);
		m_bstart = false;

		//Reset
		m_seconds_cur = -1;
		m_min_x0 = -1;
		m_min_0x = -1;
		m_sec_x0 = -1;
		m_sec_0x = -1;		
	}
	else
	{
		//Start......
		m_seconds_cur = 0;

		m_min_x0 = -1;
		m_min_0x = -1;
		m_sec_x0 = -1;
		m_sec_0x = -1;

		m_bstart = true;

		UpdateDigitClock();
		SetTimer(0, 1000, NULL);
	}
}

void CProtoTimerDlg::UpdateDigitClock()
{
	//Calc
	int time_left = 0;
	int time_min = 0;
	int time_sec = 0;

	//get time value
	time_left = this->SECONDS_MAX - this->m_seconds_cur;
	time_min = time_left / 60;
	time_sec = time_left % 60;

	//return when timer not start
	if (this->m_seconds_cur == -1)
	{
		return;
	}

	//refresh picture when digit changed!
	if (m_min_x0 != (time_min / 10))
	{
		m_picbox_min_x0.SetBitmap((HBITMAP)m_bmp[time_min / 10]);
	}
	if (m_min_0x != (time_min % 10))
	{
		m_picbox_min_0x.SetBitmap((HBITMAP)m_bmp[time_min % 10]);
	}
	if (m_sec_x0 != (time_sec / 10))
	{
		m_picbox_sec_x0.SetBitmap((HBITMAP)m_bmp[time_sec / 10]);
	}
	if (m_sec_0x != (time_sec % 10))
	{
		m_picbox_sec_0x.SetBitmap((HBITMAP)m_bmp[time_sec % 10]);
	}

	//Save current time value
	m_min_x0 = time_min / 10;
	m_min_0x = time_min % 10;
	m_sec_x0 = time_sec / 10;
	m_sec_0x = time_sec % 10;
}

void CProtoTimerDlg::OnDestroy()
{
	CRect rect;
	CString strTmp;
	FiniNotify();

	GetWindowRect(&rect);

	strTmp.Format(_T("%d"), rect.left);
	WritePrivateProfileString(_T("ProtoTimer"),
		_T("X"), strTmp.GetBuffer(), CONFIG_INI_NAME);

	strTmp.Format(_T("%d"), rect.top);
	WritePrivateProfileString(_T("ProtoTimer"),
		_T("Y"), strTmp.GetBuffer(), CONFIG_INI_NAME);

	WritePrivateProfileString(_T("ProtoTimer"),
		_T("mp3"), m_config_mp3path.GetBuffer(), CONFIG_INI_NAME);

	WritePrivateProfileString(_T("ProtoTimer"),
		_T("cmd"), m_config_cmdline.GetBuffer(), CONFIG_INI_NAME);

	WritePrivateProfileString(_T("ProtoTimer"),
		_T("arg"), m_config_argument.GetBuffer(), CONFIG_INI_NAME);

	WTSUnRegisterSessionNotification(this->GetSafeHwnd());
}

void CProtoTimerDlg::OnTimer(UINT_PTR nID)
{

	if (nID == 0)
	{
		//Update time counter
		m_seconds_cur++;

		UpdateDigitClock();
		CheckNotify();
		//Alert when 5 min left
		//if (m_seconds_max - m_seconds_cur == 5 * 60)
		//{
		//	AfxMessageBox(_T("System will lock after 5 min!"));
		//}
		CheckTimout();
	}
}

HBRUSH CProtoTimerDlg::OnCtlColor(CDC* pDC,
		CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor);

	if((nCtlColor == CTLCOLOR_DLG) || (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC -> SetBkMode(TRANSPARENT);
		return m_whitebrush;
	}
	return hbr;
}

BOOL CProtoTimerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return TRUE;
}

void CProtoTimerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CProtoTimerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	ChangeTimerStatus();
	CDialog::OnRButtonDown(nFlags, point);
}

void CProtoTimerDlg::OnBnClickedButtonClose()
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CLOSE);
}

LRESULT CProtoTimerDlg::OnSessionChange(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case WTS_CONSOLE_CONNECT:
		break;
	case WTS_CONSOLE_DISCONNECT:
		break;
	case WTS_REMOTE_CONNECT:
		break;
	case WTS_REMOTE_DISCONNECT:
		break;
	case WTS_SESSION_LOGON:
		break;
	case WTS_SESSION_LOGOFF:
		break;
	case WTS_SESSION_LOCK:
		if (m_bstart)
		{
			ChangeTimerStatus();
		}
		break;
	case WTS_SESSION_UNLOCK:
		if (!m_bstart)
		{
			ChangeTimerStatus();
		}
		break;
	case WTS_SESSION_REMOTE_CONTROL:
		break;
	default:
		break;
	}

	return 0;
}

LRESULT CProtoTimerDlg::OnNotifyIcon(WPARAM wParam,LPARAM IParam)
{
	if ((IParam == WM_LBUTTONDOWN) || (IParam == WM_RBUTTONDOWN))
	{ 
		ShowWindow(SW_SHOWNORMAL);
		SetForegroundWindow();
		//Shell_NotifyIcon(NIM_DELETE, &m_notify_icon);
		m_benable_notify = false;
	}

	return 0;
}

void CProtoTimerDlg::OnSize(UINT nType, int cx, int cy)
{
	if (nType == SIZE_MINIMIZED)
	{
		////Icon to tray
		//m_notify_icon.cbSize = sizeof(NOTIFYICONDATA);
		//m_notify_icon.hIcon  = AfxGetApp()->LoadIcon(IDI_ICON1);
		//m_notify_icon.hWnd   = m_hWnd;
		//lstrcpy(m_notify_icon.szTip, _T("ProtoTimer!"));
		//m_notify_icon.uCallbackMessage = WM_NC;
		//m_notify_icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		//Shell_NotifyIcon(NIM_ADD, &m_notify_icon);
		//m_bminmal = true;

		//ShowWindow(SW_HIDE);
	}

	CDialog::OnSize(nType, cx, cy);
}

void CProtoTimerDlg::InitNotify()
{
	m_notify_icon.cbSize = sizeof(NOTIFYICONDATA);
	m_notify_icon.hIcon  = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_notify_icon.hWnd   = m_hWnd;
	lstrcpy(m_notify_icon.szTip, _T("ProtoTimer!"));
	m_notify_icon.uCallbackMessage = WM_NC;
	m_notify_icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &m_notify_icon);
	m_benable_notify = true;
}

void CProtoTimerDlg::CheckNotify()
{
	int time_left = SECONDS_MAX - m_seconds_cur;
	CString strTmp;

	if (m_benable_notify)
	{
		//Build left notify
		strTmp.Format(_T(" %d:%d left! "), time_left / 60, time_left % 60);
		lstrcpy(m_notify_icon.szTip, strTmp.GetBuffer());

		if (time_left == SECONDS_NOTIFY)
		{
			m_notify_icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
			m_notify_icon.dwInfoFlags = NIF_INFO;

			lstrcpy(m_notify_icon.szInfo, strTmp.GetBuffer());
			lstrcpy(m_notify_icon.szInfoTitle, _T("Nofify"));
			m_notify_icon.uTimeout = 0;
		}
		else
		{
			m_notify_icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		}

		m_notify_icon.uCallbackMessage = WM_NC;
		//m_notify_icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		Shell_NotifyIcon(NIM_MODIFY, &m_notify_icon);
	}
}

void CProtoTimerDlg::FiniNotify()
{
	if (m_benable_notify)
	{
		Shell_NotifyIcon(NIM_DELETE, &m_notify_icon);
	}
}

void CProtoTimerDlg::CheckTimout()
{
	if (m_seconds_cur >= SECONDS_MAX)
	{
		if (this->m_config_cmdline.GetLength() > 0)
		{
			ShellExecute(NULL, _T("open"), m_config_cmdline, m_config_argument, NULL, SW_SHOWNORMAL);
		}
		else 
		{
			//CWMPDailog dlg;
			//KillTimer(0);
			//dlg.DoModal();
			AfxMessageBox(_T("Timeout!"));
		}

		//Stop timer, the timer will auto enable when shellexecute is lock.
		//When user unlock session, timer will enable!
		ChangeTimerStatus();

		//if (m_bminmal)
		//{
		//	OnNotifyIcon(0, WM_LBUTTONDOWN);
		//}			
	}
}
