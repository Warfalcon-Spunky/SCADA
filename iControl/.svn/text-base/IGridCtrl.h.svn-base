#if !defined(AFX_IGRIDCTRL_H__0B09C9F8_5407_4A4B_82AA_2C843605B1BC__INCLUDED_)
#define AFX_IGRIDCTRL_H__0B09C9F8_5407_4A4B_82AA_2C843605B1BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IGridCtrl.h : header file
//

#include "GridCtrl_src/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CIGridCtrl window

class CIGridCtrl : public CGridCtrl
{
// Construction
public:
	CIGridCtrl();
	virtual ~CIGridCtrl();

	virtual void OnEndEditCell(int nRow, int nCol, CString str);
	virtual CString GetItemText(int nRow, int nCol) const;
	virtual LRESULT SendMessageToParent(int nRow, int nCol, int nMessage) const;

public:
	int m_iEditRow, m_iEditCol;
	CString m_strEdit;
};

#endif // !defined(AFX_IGRIDCTRL_H__0B09C9F8_5407_4A4B_82AA_2C843605B1BC__INCLUDED_)
