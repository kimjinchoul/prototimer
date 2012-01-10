#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "ocx1.h"


// CWMPDailog dialog

class CWMPDailog : public CDialog
{
	DECLARE_DYNAMIC(CWMPDailog)

public:
	CWMPDailog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWMPDailog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	COcx1 m_wmp;
};
