// IGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "iControl.h"
#include "IGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIGridCtrl

CIGridCtrl::CIGridCtrl()
{
}

CIGridCtrl::~CIGridCtrl()
{
}

void CIGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
	m_iEditRow = nRow;
	m_iEditCol = nCol;
	m_strEdit = str;

	CGridCtrl::OnEndEditCell(nRow, nCol, str);
}

CString CIGridCtrl::GetItemText(int nRow, int nCol) const
{
	if (m_iEditRow == nRow && m_iEditCol == nCol)
	{
		return m_strEdit;
	}
	else
	{
		return CGridCtrl::GetItemText(nRow, nCol);
	}
}

LRESULT CIGridCtrl::SendMessageToParent(int nRow, int nCol, int nMessage) const
{
	return CGridCtrl::SendMessageToParent(nRow, nCol, nMessage);
}