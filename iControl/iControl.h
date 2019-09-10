// iControl.h : main header file for the ICONTROL application
//

#if !defined(AFX_ICONTROL_H__33E3567C_0B3B_419D_B353_2B3263494F9C__INCLUDED_)
#define AFX_ICONTROL_H__33E3567C_0B3B_419D_B353_2B3263494F9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIControlApp:
// See iControl.cpp for the implementation of this class
//

class CIControlApp : public CWinApp
{
public:
	CIControlApp();

	int   m_iAccessDays;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIControlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CIControlApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONTROL_H__33E3567C_0B3B_419D_B353_2B3263494F9C__INCLUDED_)
