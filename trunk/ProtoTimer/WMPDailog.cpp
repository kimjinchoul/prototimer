// WMPDailog.cpp : implementation file
//

#include "stdafx.h"
#include "ProtoTimer.h"
#include "WMPDailog.h"


// CWMPDailog dialog

IMPLEMENT_DYNAMIC(CWMPDailog, CDialog)

CWMPDailog::CWMPDailog(CWnd* pParent /*=NULL*/)
	: CDialog(CWMPDailog::IDD, pParent)
{

}

CWMPDailog::~CWMPDailog()
{
}

void CWMPDailog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCX1, m_wmp);
}

BOOL CWMPDailog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_wmp.put_URL(_T("alert.mp3"));
	return TRUE;
}


BEGIN_MESSAGE_MAP(CWMPDailog, CDialog)
END_MESSAGE_MAP()


// CWMPDailog message handlers
