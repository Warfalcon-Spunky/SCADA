// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "isevensegmentanalogx.h"

/////////////////////////////////////////////////////////////////////////////
// CiSevenSegmentAnalogX

IMPLEMENT_DYNCREATE(CiSevenSegmentAnalogX, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CiSevenSegmentAnalogX properties

/////////////////////////////////////////////////////////////////////////////
// CiSevenSegmentAnalogX operations

long CiSevenSegmentAnalogX::GetPrecision()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetPrecision(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

double CiSevenSegmentAnalogX::GetValue()
{
	double result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetValue(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL CiSevenSegmentAnalogX::GetShowSign()
{
	BOOL result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetShowSign(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiSevenSegmentAnalogX::GetDigitCount()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetDigitCount(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiSevenSegmentAnalogX::GetLeadingStyle()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetLeadingStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CiSevenSegmentAnalogX::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiSevenSegmentAnalogX::GetDigitSpacing()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetDigitSpacing(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiSevenSegmentAnalogX::GetSegmentMargin()
{
	long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetSegmentMargin(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CiSevenSegmentAnalogX::GetSegmentColor()
{
	unsigned long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetSegmentColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CiSevenSegmentAnalogX::GetSegmentSeperation()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetSegmentSeperation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiSevenSegmentAnalogX::GetSegmentSize()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetSegmentSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CiSevenSegmentAnalogX::GetShowOffSegments()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetShowOffSegments(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CiSevenSegmentAnalogX::GetPowerOff()
{
	BOOL result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetPowerOff(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long CiSevenSegmentAnalogX::GetBackGroundColor()
{
	unsigned long result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetBackGroundColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CiSevenSegmentAnalogX::GetBorderStyle()
{
	long result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetBorderStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CiSevenSegmentAnalogX::GetVisible()
{
	BOOL result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CiSevenSegmentAnalogX::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSevenSegmentAnalogX::ShowPropertyEditor()
{
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSevenSegmentAnalogX::SetValueNoEvent(double Value)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Value);
}

unsigned long CiSevenSegmentAnalogX::GetSegmentOffColor()
{
	unsigned long result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetSegmentOffColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL CiSevenSegmentAnalogX::GetAutoSegmentOffColor()
{
	BOOL result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetAutoSegmentOffColor(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CiSevenSegmentAnalogX::GetTransparent()
{
	BOOL result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetTransparent(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CiSevenSegmentAnalogX::RepaintAll()
{
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSevenSegmentAnalogX::BeginUpdate()
{
	InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSevenSegmentAnalogX::EndUpdate()
{
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSevenSegmentAnalogX::Lock()
{
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSevenSegmentAnalogX::Unlock()
{
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CiSevenSegmentAnalogX::GetUpdateFrameRate()
{
	long result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetUpdateFrameRate(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiSevenSegmentAnalogX::GetComponentHandle()
{
	long result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SaveImageToBitmap(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CiSevenSegmentAnalogX::SaveImageToMetaFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CiSevenSegmentAnalogX::SaveImageToJPEG(LPCTSTR FileName, long Compression, BOOL Progressive)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BOOL;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, Compression, Progressive);
}

long CiSevenSegmentAnalogX::OPCAddItem()
{
	long result;
	InvokeHelper(0x1001, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CiSevenSegmentAnalogX::OPCItemCount()
{
	long result;
	InvokeHelper(0x1002, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::OPCRemoveAllItems()
{
	InvokeHelper(0x1003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSevenSegmentAnalogX::OPCItemActivate(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void CiSevenSegmentAnalogX::OPCItemDeactivate(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

CString CiSevenSegmentAnalogX::GetOPCItemComputerName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1006, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

CString CiSevenSegmentAnalogX::GetOPCItemServerName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1007, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

CString CiSevenSegmentAnalogX::GetOPCItemItemName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1008, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

CString CiSevenSegmentAnalogX::GetOPCItemPropertyName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1009, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

BOOL CiSevenSegmentAnalogX::GetOPCItemAutoConnect(long Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x100a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Index);
	return result;
}

long CiSevenSegmentAnalogX::GetOPCItemUpdateRate(long Index)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x100b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Index);
	return result;
}

void CiSevenSegmentAnalogX::SetOPCItemComputerName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSevenSegmentAnalogX::SetOPCItemServerName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSevenSegmentAnalogX::SetOPCItemItemName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSevenSegmentAnalogX::SetOPCItemPropertyName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSevenSegmentAnalogX::SetOPCItemAutoConnect(long Index, BOOL Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x1010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSevenSegmentAnalogX::SetOPCItemUpdateRate(long Index, long Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x1011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSevenSegmentAnalogX::OPCItemLoaded(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

long CiSevenSegmentAnalogX::GetObjectPointer()
{
	long result;
	InvokeHelper(0x1068, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT CiSevenSegmentAnalogX::GetBytesJPEG(long Compression, BOOL Progressive)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x1069, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Compression, Progressive);
	return result;
}

void CiSevenSegmentAnalogX::SavePropertiesToFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x106a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CiSevenSegmentAnalogX::LoadPropertiesFromFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x106b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

BOOL CiSevenSegmentAnalogX::GetOptionSaveAllProperties()
{
	BOOL result;
	InvokeHelper(0x106d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetOptionSaveAllProperties(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x106d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CiSevenSegmentAnalogX::GetAutoFrameRate()
{
	BOOL result;
	InvokeHelper(0x106e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetAutoFrameRate(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x106e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiSevenSegmentAnalogX::GetWidth()
{
	long result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiSevenSegmentAnalogX::GetHeight()
{
	long result;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void CiSevenSegmentAnalogX::iPaintToDC(long X, long Y, BOOL Transparent, long DC)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BOOL VTS_I4;
	InvokeHelper(0x10cc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y, Transparent, DC);
}

BOOL CiSevenSegmentAnalogX::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSevenSegmentAnalogX::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CiSevenSegmentAnalogX::OPCItemUpdateResume(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void CiSevenSegmentAnalogX::OPCItemUpdateSuspend(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

VARIANT CiSevenSegmentAnalogX::GetOPCItemData(long Index)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1015, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Index);
	return result;
}

short CiSevenSegmentAnalogX::GetOPCItemQuality(long Index)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1016, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Index);
	return result;
}

double CiSevenSegmentAnalogX::GetOPCItemTimeStamp(long Index)
{
	double result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1017, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		Index);
	return result;
}

void CiSevenSegmentAnalogX::SetOPCItemData(long Index, const VARIANT& Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x1018, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, &Value);
}

BOOL CiSevenSegmentAnalogX::GetOPCItemActive(long Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1019, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Index);
	return result;
}

CString CiSevenSegmentAnalogX::GetOPCItemGroupName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x101c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

void CiSevenSegmentAnalogX::SetOPCItemGroupName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x101d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSevenSegmentAnalogX::OPCDeleteItem(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x101e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void CiSevenSegmentAnalogX::OPCItemActivateSuspend(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x101f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

VARIANT CiSevenSegmentAnalogX::GetBytesPNG(long Compression)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1020, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Compression);
	return result;
}

void CiSevenSegmentAnalogX::SaveImageToPNG(LPCTSTR FileName, long Compression)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x1021, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, Compression);
}
