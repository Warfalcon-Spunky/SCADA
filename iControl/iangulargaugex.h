#if !defined(AFX_IANGULARGAUGEX_H__90E7815B_2A0B_4245_ABEE_5B2B385157C8__INCLUDED_)
#define AFX_IANGULARGAUGEX_H__90E7815B_2A0B_4245_ABEE_5B2B385157C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class COleFont;
class CPicture;

/////////////////////////////////////////////////////////////////////////////
// CiAngularGaugeX wrapper class

class CiAngularGaugeX : public CWnd
{
protected:
	DECLARE_DYNCREATE(CiAngularGaugeX)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xc5412dd5, 0x2e2f, 0x11d3, { 0x85, 0xbf, 0x0, 0x10, 0x5a, 0xc8, 0xb7, 0x15 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	long GetArcRadius();
	void SetArcRadius(long nNewValue);
	long GetArcRangeDegrees();
	void SetArcRangeDegrees(long nNewValue);
	long GetArcStartDegrees();
	void SetArcStartDegrees(long nNewValue);
	CString GetLabel1Text();
	void SetLabel1Text(LPCTSTR lpszNewValue);
	COleFont GetLabel1Font();
	void SetLabel1Font(LPDISPATCH newValue);
	long GetLabel1AlignVertical();
	void SetLabel1AlignVertical(long nNewValue);
	long GetLabel1AlignHorizontal();
	void SetLabel1AlignHorizontal(long nNewValue);
	long GetLabel1OffsetX();
	void SetLabel1OffsetX(long nNewValue);
	long GetLabel1OffsetY();
	void SetLabel1OffsetY(long nNewValue);
	CString GetLabel2Text();
	void SetLabel2Text(LPCTSTR lpszNewValue);
	COleFont GetLabel2Font();
	void SetLabel2Font(LPDISPATCH newValue);
	long GetLabel2AlignVertical();
	void SetLabel2AlignVertical(long nNewValue);
	long GetLabel2AlignHorizontal();
	void SetLabel2AlignHorizontal(long nNewValue);
	long GetLabel2OffsetX();
	void SetLabel2OffsetX(long nNewValue);
	long GetLabel2OffsetY();
	void SetLabel2OffsetY(long nNewValue);
	long GetHubSize();
	void SetHubSize(long nNewValue);
	long GetMinMaxPointerStyle();
	void SetMinMaxPointerStyle(long nNewValue);
	long GetPointerMargin();
	void SetPointerMargin(long nNewValue);
	unsigned long GetPointerColor();
	void SetPointerColor(unsigned long newValue);
	long GetPointerSize();
	void SetPointerSize(long nNewValue);
	long GetPointerStyle();
	void SetPointerStyle(long nNewValue);
	BOOL GetShowInnerArcRadius();
	void SetShowInnerArcRadius(BOOL bNewValue);
	BOOL GetShowOuterArcRadius();
	void SetShowOuterArcRadius(BOOL bNewValue);
	BOOL GetShowHub();
	void SetShowHub(BOOL bNewValue);
	BOOL GetShowLabel1();
	void SetShowLabel1(BOOL bNewValue);
	BOOL GetShowLabel2();
	void SetShowLabel2(BOOL bNewValue);
	long GetOffsetX();
	void SetOffsetX(long nNewValue);
	long GetOffsetY();
	void SetOffsetY(long nNewValue);
	unsigned long GetSectionColor1();
	void SetSectionColor1(unsigned long newValue);
	unsigned long GetSectionColor2();
	void SetSectionColor2(unsigned long newValue);
	unsigned long GetSectionColor3();
	void SetSectionColor3(unsigned long newValue);
	unsigned long GetSectionColor4();
	void SetSectionColor4(unsigned long newValue);
	unsigned long GetSectionColor5();
	void SetSectionColor5(unsigned long newValue);
	long GetSectionCount();
	void SetSectionCount(long nNewValue);
	double GetSectionEnd1();
	void SetSectionEnd1(double newValue);
	double GetSectionEnd2();
	void SetSectionEnd2(double newValue);
	double GetSectionEnd3();
	void SetSectionEnd3(double newValue);
	double GetSectionEnd4();
	void SetSectionEnd4(double newValue);
	unsigned long GetBackGroundColor();
	void SetBackGroundColor(unsigned long newValue);
	long GetBorderStyle();
	void SetBorderStyle(long nNewValue);
	BOOL GetShowMaxPointer();
	void SetShowMaxPointer(BOOL bNewValue);
	BOOL GetShowMinPointer();
	void SetShowMinPointer(BOOL bNewValue);
	long GetMinMaxPointerSize();
	void SetMinMaxPointerSize(long nNewValue);
	long GetMinMaxPointerMargin();
	void SetMinMaxPointerMargin(long nNewValue);
	unsigned long GetMaxPointerColor();
	void SetMaxPointerColor(unsigned long newValue);
	unsigned long GetMinPointerColor();
	void SetMinPointerColor(unsigned long newValue);
	BOOL GetShowTicksMajor();
	void SetShowTicksMajor(BOOL bNewValue);
	BOOL GetShowTicksMinor();
	void SetShowTicksMinor(BOOL bNewValue);
	BOOL GetShowTickLabels();
	void SetShowTickLabels(BOOL bNewValue);
	long GetTickMajorCount();
	void SetTickMajorCount(long nNewValue);
	unsigned long GetTickMajorColor();
	void SetTickMajorColor(unsigned long newValue);
	long GetTickMajorLength();
	void SetTickMajorLength(long nNewValue);
	long GetTickMinorAlignment();
	void SetTickMinorAlignment(long nNewValue);
	long GetTickMinorCount();
	void SetTickMinorCount(long nNewValue);
	unsigned long GetTickMinorColor();
	void SetTickMinorColor(unsigned long newValue);
	long GetTickMinorLength();
	void SetTickMinorLength(long nNewValue);
	long GetTickMargin();
	void SetTickMargin(long nNewValue);
	long GetTickLabelMargin();
	void SetTickLabelMargin(long nNewValue);
	COleFont GetTickLabelFont();
	void SetTickLabelFont(LPDISPATCH newValue);
	long GetTickLabelPrecision();
	void SetTickLabelPrecision(long nNewValue);
	void ResetMinMax();
	double GetCurrentMax();
	void SetCurrentMax(double newValue);
	double GetCurrentMin();
	void SetCurrentMin(double newValue);
	double GetPositionPercent();
	void SetPositionPercent(double newValue);
	double GetPosition();
	void SetPosition(double newValue);
	double GetPositionMax();
	void SetPositionMax(double newValue);
	double GetPositionMin();
	void SetPositionMin(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	void AboutBox();
	unsigned long GetTickLabelFontColor();
	void SetTickLabelFontColor(unsigned long newValue);
	unsigned long GetLabel1FontColor();
	void SetLabel1FontColor(unsigned long newValue);
	unsigned long GetLabel2FontColor();
	void SetLabel2FontColor(unsigned long newValue);
	CPicture GetBackGroundPicture();
	void SetBackGroundPicture(LPDISPATCH newValue);
	void ShowPropertyEditor();
	void SetPositionNoEvent(double Value);
	BOOL GetMinMaxFixed();
	void SetMinMaxFixed(BOOL bNewValue);
	BOOL GetMinMaxUserCanMove();
	void SetMinMaxUserCanMove(BOOL bNewValue);
	BOOL GetReverseScale();
	void SetReverseScale(BOOL bNewValue);
	unsigned long GetHubColor();
	void SetHubColor(unsigned long newValue);
	long GetPrecisionStyle();
	void SetPrecisionStyle(long nNewValue);
	BOOL GetAutoScaleEnabled();
	void SetAutoScaleEnabled(BOOL bNewValue);
	long GetAutoScaleDesiredTicks();
	void SetAutoScaleDesiredTicks(long nNewValue);
	long GetAutoScaleMaxTicks();
	void SetAutoScaleMaxTicks(long nNewValue);
	long GetAutoScaleStyle();
	void SetAutoScaleStyle(long nNewValue);
	BOOL GetTransparent();
	void SetTransparent(BOOL bNewValue);
	void RepaintAll();
	void BeginUpdate();
	void EndUpdate();
	void AddLimit(long PointerSize, long PointerMargin, BOOL DrawScaleSide, double UpperValue, double LowerValue, unsigned long UpperPointerColor, unsigned long LowerPointerColor);
	void RemoveLimit(long Index);
	void RemoveAllLimits();
	long GetLimitCount();
	double GetLimitUpperValue(long Index);
	double GetLimitLowerValue(long Index);
	long GetLimitPointerSize(long Index);
	long GetLimitPointerMargin(long Index);
	unsigned long GetLimitUpperPointerColor(long Index);
	unsigned long GetLimitLowerPointerColor(long Index);
	BOOL GetLimitShowUpperPointer(long Index);
	BOOL GetLimitShowLowerPointer(long Index);
	BOOL GetLimitDrawScaleSide(long Index);
	void SetLimitUpperValue(long Index, double Value);
	void SetLimitLowerValue(long Index, double Value);
	void SetLimitPointerSize(long Index, long Value);
	void SetLimitPointerMargin(long Index, long Value);
	void SetLimitUpperPointerColor(long Index, unsigned long Value);
	void SetLimitLowerPointerColor(long Index, unsigned long Value);
	void SetLimitShowUpperPointer(long Index, BOOL Value);
	void SetLimitShowLowerPointer(long Index, BOOL Value);
	void SetLimitDrawScaleSide(long Index, BOOL Value);
	void Lock();
	void Unlock();
	BOOL GetAutoCenter();
	void SetAutoCenter(BOOL bNewValue);
	long GetUpdateFrameRate();
	void SetUpdateFrameRate(long nNewValue);
	long GetTickLabelAlignment();
	void SetTickLabelAlignment(long nNewValue);
	long GetComponentHandle();
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	long GetOuterMargin();
	void SetOuterMargin(long nNewValue);
	void SaveImageToBitmap(LPCTSTR FileName);
	void SaveImageToMetaFile(LPCTSTR FileName);
	void SaveImageToJPEG(LPCTSTR FileName, long Compression, BOOL Progressive);
	long OPCAddItem();
	long OPCItemCount();
	void OPCRemoveAllItems();
	void OPCItemActivate(long Index);
	void OPCItemDeactivate(long Index);
	CString GetOPCItemComputerName(long Index);
	CString GetOPCItemServerName(long Index);
	CString GetOPCItemItemName(long Index);
	CString GetOPCItemPropertyName(long Index);
	BOOL GetOPCItemAutoConnect(long Index);
	long GetOPCItemUpdateRate(long Index);
	void SetOPCItemComputerName(long Index, LPCTSTR Value);
	void SetOPCItemServerName(long Index, LPCTSTR Value);
	void SetOPCItemItemName(long Index, LPCTSTR Value);
	void SetOPCItemPropertyName(long Index, LPCTSTR Value);
	void SetOPCItemAutoConnect(long Index, BOOL Value);
	void SetOPCItemUpdateRate(long Index, long Value);
	void OPCItemLoaded(long Index);
	BOOL GetCachedDrawing();
	void SetCachedDrawing(BOOL bNewValue);
	long AddPointer();
	void RemoveAllPointers();
	void DeletePointer(long Index);
	long GetPointerCount();
	double GetPointersPosition(long Index);
	long GetPointersSize(long Index);
	unsigned long GetPointersColor(long Index);
	long GetPointersMargin(long Index);
	long GetPointersStyle(long Index);
	void SetPointersPosition(long Index, double Value);
	void SetPointersSize(long Index, long Value);
	void SetPointersColor(long Index, long Value);
	void SetPointersMargin(long Index, long Value);
	void SetPointersStyle(long Index, long Value);
	long GetObjectPointer();
	VARIANT GetBytesJPEG(long Compression, BOOL Progressive);
	void SavePropertiesToFile(LPCTSTR FileName);
	void LoadPropertiesFromFile(LPCTSTR FileName);
	BOOL GetOptionSaveAllProperties();
	void SetOptionSaveAllProperties(BOOL bNewValue);
	BOOL GetAutoFrameRate();
	void SetAutoFrameRate(BOOL bNewValue);
	void iPaintToDC(long X, long Y, BOOL Transparent, long DC);
	long GetWidth();
	void SetWidth(long nNewValue);
	long GetHeight();
	void SetHeight(long nNewValue);
	void OPCItemUpdateResume(long Index);
	void OPCItemUpdateSuspend(long Index);
	VARIANT GetOPCItemData(long Index);
	short GetOPCItemQuality(long Index);
	double GetOPCItemTimeStamp(long Index);
	void SetOPCItemData(long Index, const VARIANT& Value);
	BOOL GetOPCItemActive(long Index);
	BOOL GetPointersVisible(long Index);
	void SetPointersVisible(long Index, BOOL Value);
	CString GetOPCItemGroupName(long Index);
	void SetOPCItemGroupName(long Index, LPCTSTR Value);
	void OPCDeleteItem(long Index);
	void OPCItemActivateSuspend(long Index);
	VARIANT GetBytesPNG(long Compression);
	void SaveImageToPNG(LPCTSTR FileName, long Compression);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IANGULARGAUGEX_H__90E7815B_2A0B_4245_ABEE_5B2B385157C8__INCLUDED_)
