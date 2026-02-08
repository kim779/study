#pragma once



#include <afxtempl.h>
#include "axisfire.h"

class CAxisProfile
{
public:
    CAxisProfile(CWnd *pWizard) : m_pParent(pWizard) {
        m_User = Variant(nameCC);
        m_Home = Variant(homeCC);
        m_CheckPath.Format("%s\\USER\\%s", m_Home, m_User);
		m_CheckPath.MakeUpper();
		m_CheckPath.Replace("/", "\\");
    };

	CString GetEncryptPath(LPCSTR path) {
		CString tmp = path;
        tmp.MakeUpper();
        tmp.Replace("/", "\\");
        
        if (tmp.GetLength()>m_CheckPath.GetLength() && !m_CheckPath.IsEmpty())
        {
            if (tmp.Left(m_CheckPath.GetLength())==m_CheckPath)
            {
				tmp.Format("%s\\USER\\%s%s", m_Home, GetHashValue(m_User), tmp.Mid(m_CheckPath.GetLength()));
            }
        }
		return tmp;
	};

    BOOL WritePrivateProfileString(LPCSTR section, LPCSTR key, LPCSTR value, LPCSTR path) {
		return ::WritePrivateProfileString(section, key, value, GetEncryptPath(path));
    };

	DWORD GetPrivateProfileString(LPCSTR section, LPCSTR key, LPCSTR defval, LPSTR buff, DWORD size, LPCSTR path) {
		return ::GetPrivateProfileString(section, key, defval, buff, size, GetEncryptPath(path));
	};

	static CString GetHashValue(LPCSTR key) {
		char buff[32];
		sprintf(buff, "%08u", HashKey<LPCSTR>(key));
		return buff;
	};
    
protected:
    CWnd *m_pParent;
    CString m_User, m_Home, m_CheckPath;
    
protected:
    LPCSTR Variant(int cmd, LPCSTR data="") {
        return (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
    };
};

