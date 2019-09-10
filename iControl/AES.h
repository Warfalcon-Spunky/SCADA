#if !defined(AFX_AES_H__7AF1701B_57B2_40C2_9609_EDA11E186F8F__INCLUDED_)
#define AFX_AES_H__7AF1701B_57B2_40C2_9609_EDA11E186F8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AES.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAES window

class CAES : public CWnd
{
// Construction
public:
	CAES();

public:
	void AES128_Encrypt(BYTE *ucInput_Chal, BYTE *ucInputKey);

private:
	BYTE ucC[16];
	BYTE ucK[16];
	BYTE ucChallenge[4][4];
	BYTE ucKey[4][4];
	BYTE RCON_Count;

	void AddRoundKey(void);
	void SubBytes(void);
	void ShiftRows(void);
	BYTE modMult02(BYTE x);
	BYTE modMult03(BYTE x);
	void MixColumns(void);
	void KeySchedule(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAES)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAES();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAES)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AES_H__7AF1701B_57B2_40C2_9609_EDA11E186F8F__INCLUDED_)
