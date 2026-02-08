// FontKey.h: interface for the CFontKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTKEY_H__490C34C7_CA58_4CA6_AB46_D70812227A41__INCLUDED_)
#define AFX_FONTKEY_H__490C34C7_CA58_4CA6_AB46_D70812227A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFontKey : public CObject  
{
private:
//public:
	LONG m_nBold;
	LONG m_nSize;
	char m_szFnName[20];
	
public:
	LONG GetBold(){return m_nBold;};
	LONG GetSize(){return m_nSize;};
	char* GetName(){return m_szFnName;};
	void SetData(LONG Bold, LONG Size, char* Name)
	{
		m_nBold = Bold;
		m_nSize = Size;
		sprintf(m_szFnName, "%-19.19s", Name);
	};
	CFontKey(LONG Bold, LONG Size, char* szName)
	{
		m_nBold = Bold;
		m_nSize = Size;
		sprintf(m_szFnName, "%-19.19s", szName);
	};
	virtual ~CFontKey(){};

};

#endif // !defined(AFX_FONTKEY_H__490C34C7_CA58_4CA6_AB46_D70812227A41__INCLUDED_)
