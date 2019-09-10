#ifndef __REGEDIT_H__
#define __REGEDIT_H__

class CRegedit
{
public:
	CRegedit();
	~CRegedit();
	
public:
	int CreateSubKey(struct HKEY__ *pRootKey, TCHAR *pSubKey);
	int GetContent(char *pContent, int *pLen);
	int ShowContent(struct HKEY__ *pRootKey, TCHAR *pSubKey, TCHAR *pValueName);
	int SetStrContent(struct HKEY__ *pRootKey, TCHAR *pSubKey, TCHAR *pValueName, LPBYTE StrContent);
	int SetIntContent(struct HKEY__ *pRootKey, TCHAR *pSubKey, TCHAR *pValueName, int *pIntContent);
	int SetBinContent(struct HKEY__ *pRootKey, TCHAR *pSubKey, TCHAR *pValueName, BYTE *pBinContent);
	int DeleteKey(struct HKEY__ *pRootKey, TCHAR *pSubKey, TCHAR *pValueName);
	int DeleteValue(struct HKEY__ *pRootKey, TCHAR *pSubKey, TCHAR *pValueName);

private:
	HKEY m_hkey;					// 主键值变量
	char m_cContent[MAX_PATH];		// 所查注册表键值内容
	DWORD m_dwType;					// 读取数据类型
	DWORD m_dwLength;				// 读取数据长度
	struct HKEY__ *m_pRootKey;		// 注册表主键值名称
	TCHAR *m_pSubKey;				// 注册表项地址
	TCHAR *m_pKeyName;				// 设置项的名称
	TCHAR *m_pValueName;			// 设置值的名称
	LPBYTE m_StrContent;			// 字符串类型
	int    m_IntContent[MAX_PATH];	// 整型类型
	BYTE   m_BinContent[MAX_PATH];	// 二进制类型
};

#endif __REGEDIT_H__