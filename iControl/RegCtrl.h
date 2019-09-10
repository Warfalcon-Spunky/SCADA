#ifndef __REG_CTRL_H__
#define __REG_CTRL_H__

#define __REGISTER_EXPORT__
#include "Register.h"

#include "Regedit.h"
#include "AES.h"

class REGISTER_API CRegCtrl
{
public:
	CRegCtrl();
	virtual ~CRegCtrl();

	// ��ñ����Ļ���������
 	int GetMachineEncode(LPCTSTR lpComputerCode, UINT uiComCodeSize, LPCTSTR lpRegisterCode, UINT uiRegCodeSize);
	// ��ý����Ļ���������
	int GetMachineDecode(LPCTSTR lpRegisterCode, UINT uiRegCodeSize, LPCTSTR lpComputerCode, UINT uiComCodeSize);
	// ��û�����������
 	BOOL GetMachineCode(LPCTSTR lpComputerCode);
	// ͨ�������Ļ��������뼤��ϵͳ
 	BOOL ActiveByComputerCode(LPCTSTR lpRegisterCode);
	// ��ü��ܱ���: �������������
	BOOL GenerateEncodeKey(LPCTSTR lpPublicKey, LPCTSTR lpEncode);

	BOOL CreateRegtrySubKey(struct HKEY__ *pRootKey, LPCTSTR lpSubKey);
	BOOL ReadRegistryValue(struct HKEY__ *pRootKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR lpValue);
	BOOL WriteRegtryStrValue(struct HKEY__ *pRootKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR lpValue);
	BOOL AESGetSeedProc(BYTE *lpSeedBuff);
	BOOL AESDecodeProc(BYTE *lpPwdBuff, BYTE *lpEthernetBuff, BYTE *lpRandomBuff, BYTE *lpAccessDayBuff);
	BOOL AESDecodeProc(BYTE *lpPwdBuff, BYTE *lpRandomBuff, BYTE *lpAccessDayBuff);

private:
	CRegedit m_Regedit;
	__int64 ChangeCharToInt(char ch);
	__int64 MPow(__int64 iPow, __int64 iCnt);

	BYTE FixCode[16];
	CAES m_AES;
};

#endif __REG_CTRL_H__