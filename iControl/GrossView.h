#if !defined(AFX_GROSSVIEW_H__E443F1DF_CAAA_40AD_95A0_6F38DDCFADA1__INCLUDED_)
#define AFX_GROSSVIEW_H__E443F1DF_CAAA_40AD_95A0_6F38DDCFADA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrossView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrossView view
#include "MainFrm.h"
#include "HisTbl.h"
#include "iControlDoc.h"
#include "IGridCtrl.h"

#include <vector>
#include <map>
using namespace std;

typedef struct tagGROSSDAYMAP
{
	int  KlinIdx;
	char DateTime[30];
	int  CodeCnt;
} GROSSDAYMAP, FAR *LGROSSDAYMAP, PGROSSDAYMAP;

typedef struct tagGROSSMINMAP
{
	int  KlinIdx;
	int  Internal;
} GROSSMINMAP, FAR *LGROSSMINMAP, PGROSSMINMAP;

class CGrossView : public CView
{
protected:
	CGrossView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGrossView)

// Attributes
public:
	int      m_iActiveFurnaceNum;
	BOOL     m_bDayDisp;

	CIGridCtrl *m_pGrid;
	LOGFONT     m_lfFixCellFont;
	LOGFONT     m_lfCellFont;

	CHisTbl    *m_pGrossTbl;

	vector<GROSSDAYMAP> m_GrossDayMap;
	vector<GROSSMINMAP> m_GrossMinMap;

// Operations
public:
	void SetGrossActiveView(int iFurnace);

protected:
	void SetGrossGrid(void);
	void CollectBaseDayCnt(void);
	void CollectDayCnt(void);
	void SetGrossGridItem(GV_DISPINFO *pDispInfo);
	void SetGrossDayGridItem(GV_DISPINFO *pDispInfo);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrossView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGrossView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrossView)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROSSVIEW_H__E443F1DF_CAAA_40AD_95A0_6F38DDCFADA1__INCLUDED_)
