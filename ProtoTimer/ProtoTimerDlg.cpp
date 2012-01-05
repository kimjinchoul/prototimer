// ProtoTimerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoTimer.h"
#include "ProtoTimerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProtoTimerDlg dialog




CProtoTimerDlg::CProtoTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProtoTimerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_bstart = false;
}

void CProtoTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_calctrl);
	DDX_Control(pDX, IDC_EDIT1, m_timevalue);
	DDX_Control(pDX, IDC_BUTTON1, m_button);
	DDX_Control(pDX, IDC_STATIC_MIN_X0, m_picbox_min_x0);
	DDX_Control(pDX, IDC_STATIC_MIN_0X, m_picbox_min_0x);
	DDX_Control(pDX, IDC_STATIC_SEC_X0, m_picbox_sec_x0);
	DDX_Control(pDX, IDC_STATIC_SEC_0X, m_picbox_sec_0x);
	DDX_Control(pDX, IDC_OCX1, m_wmp);
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
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CProtoTimerDlg::OnBnClickedButton1)
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
	m_timevalue.SetWindowText(_T("25"));
	m_button.SetWindowText(_T("Start"));

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

	m_whitebrush.CreateSolidBrush(RGB(255,255,255));

	m_bminmal = false;

	//////////////////////////////////////////////////////////////////////////

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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProtoTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CProtoTimerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if (m_bstart)
	{
		KillTimer(0);
		m_button.SetWindowText(_T("Start"));
		m_timevalue.EnableWindow(TRUE);
		m_bstart = false;
	}
	else
	{
		CString strTmp;
		m_timevalue.GetWindowText(strTmp);
		if (strTmp.GetLength() > 0)
		{
			m_seconds_max = _wtoi(strTmp.GetBuffer()) * 60;
		}
		else
		{
			m_seconds_max = 25 * 60;
		}

		m_seconds_cur = 0;

		m_bstart = true;
		m_button.SetWindowText(_T("Stop"));
		m_timevalue.EnableWindow(FALSE);
		OnTimer(0);
		SetTimer(0, 1000, NULL);
	}
}

void CProtoTimerDlg::OnTimer(UINT_PTR nID)
{
	CString strTmp;
	int time_left = 0;
	int time_min = 0;
	int time_sec = 0;
	if (nID == 0)
	{
		m_seconds_cur++;

		//Calc
		time_left = m_seconds_max - m_seconds_cur;
		time_min = time_left / 60;
		time_sec = time_left % 60;

		m_picbox_min_x0.SetBitmap((HBITMAP)m_bmp[time_min / 10]);
		m_picbox_min_0x.SetBitmap((HBITMAP)m_bmp[time_min % 10]);

		m_picbox_sec_x0.SetBitmap((HBITMAP)m_bmp[time_sec / 10]);
		m_picbox_sec_0x.SetBitmap((HBITMAP)m_bmp[time_sec % 10]);

		strTmp.Format(_T("%d:%d left!"), time_min, time_sec);

		if (m_bminmal)
		{
			lstrcpy(m_notify_icon.szTip, strTmp.GetBuffer());

			//When only 1 min left, warning.
			if (time_left == 60)
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

		if (m_seconds_cur >= m_seconds_max)
		{
			m_wmp.put_URL(_T("alert.mp3"));
			KillTimer(0);
			m_button.SetWindowText(_T("Start"));
			m_timevalue.EnableWindow(TRUE);
			m_bstart = false;

			if (m_bminmal)
			{
				OnNotifyIcon(0, WM_LBUTTONDOWN);
			}			
		}
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

BOOL CProtoTimerDlg::OnMouseWheel(UINT nFlags,
				  short zDelta, CPoint pt)
{
	return TRUE;
}

LRESULT CProtoTimerDlg::OnNotifyIcon(WPARAM wParam,LPARAM IParam)
{
	if ((IParam == WM_LBUTTONDOWN) || (IParam == WM_RBUTTONDOWN))
	{ 
		ShowWindow(SW_SHOWNORMAL);
		BringWindowToTop();
		Shell_NotifyIcon(NIM_DELETE, &m_notify_icon);
		m_bminmal = false;
	}

	return 0;
}

void CProtoTimerDlg::OnSize(UINT nType, int cx, int cy)
{
	if (nType == SIZE_MINIMIZED)
	{
		//Icon to tray
		m_notify_icon.cbSize = sizeof(NOTIFYICONDATA);
		m_notify_icon.hIcon  = AfxGetApp()->LoadIcon(IDI_ICON1);
		m_notify_icon.hWnd   = m_hWnd;
		lstrcpy(m_notify_icon.szTip, _T("ProtoTimer!"));
		m_notify_icon.uCallbackMessage = WM_NC;
		m_notify_icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		Shell_NotifyIcon(NIM_ADD, &m_notify_icon);
		m_bminmal = true;

		ShowWindow(SW_HIDE);
	}

	CDialog::OnSize(nType, cx, cy);
}

void CProtoTimerDlg::OnDestroy()
{
	if (m_bminmal)
	{
		Shell_NotifyIcon(NIM_DELETE, &m_notify_icon);
	}	
}
