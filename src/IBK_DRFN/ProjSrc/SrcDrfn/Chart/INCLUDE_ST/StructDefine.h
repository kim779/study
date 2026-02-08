#if !defined(AFX_STRUCTDEFINE_H__18754815_9512_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_STRUCTDEFINE_H__18754815_9512_11D2_AC26_00104B9E52FF__INCLUDED_

#ifndef _USESTD_OJTASO

#define _USESTD_OJTASO
#include <vector>
#include <algorithm>
using namespace std;

#endif // #ifndef _USESTD_OJTASO

class CScriptInfomation
{
public:
	CScriptInfomation(LPCSTR lpszScriptName, UINT nUniqueID, UINT nFileType, UINT nCompiled)
	{
		m_szFunctionNameOrg = ::strdup(lpszScriptName);
		m_szFunctionName = ::strdup(lpszScriptName);
		::strupr(m_szFunctionName);

		m_nUniqueID = nUniqueID;
		m_nFileType = nFileType;
		m_nChecked = nCompiled;
	}

	~CScriptInfomation()
	{
		::free(m_szFunctionName);
		::free(m_szFunctionNameOrg);
	}
	
	LPCSTR GetFunctionName()
	{
		return m_szFunctionName;
	}

	UINT GetUniqueID()
	{
		return m_nUniqueID;
	}

	UINT GetCompiledFlag()
	{
		return m_nChecked;
	}

	void SetCompileCheck(UINT nComplied)
	{
		m_nChecked = nComplied;
	}

//	UINT GetParemeterCount()
//	{
//		return m_nCntParameter;
//	}

private:
	LPSTR	m_szFunctionName;
	LPSTR	m_szFunctionNameOrg;
//	UINT	m_nCntParameter;
	UINT	m_nChecked;
//	LPSTR	m_szComment;

	UINT	m_nUniqueID;
	UINT	m_nFileType;
};

static void DeleteScriptInfo(CScriptInfomation* pScriptInfomation)
{
	delete pScriptInfomation;
}

#define VT_SCRIPTINFO vector<CScriptInfomation*>	

class CScriptFind
{
public:
	CScriptFind(UINT p_nID)
	{
		m_nID = p_nID;
		m_bFindType = 0;
	}
	
	CScriptFind(LPCSTR p_lpszName)
	{
		::lstrcpy(m_szName, p_lpszName);
		::strupr(m_szName);
		m_bFindType = 1;
	}

	CScriptFind(LPSTR p_lpszName, int nLength)
	{
		::memset(m_szName, 0x00, sizeof(m_szName));
		::strncpy(m_szName, p_lpszName, nLength);
		::strupr(m_szName);
		m_bFindType = 1;
	}
	bool operator()(CScriptInfomation* rhs) const
	{
		if(m_bFindType == 0)
		{
			if(m_nID == rhs->GetUniqueID())
				return true;
			else
				return false;
		}
		else if(m_bFindType == 1)
		{
			if(::lstrcmp(m_szName, rhs->GetFunctionName()))
				return false;
			else
				return true;
		}

		return false;
	}

private:
	UINT	m_nID;
	char	m_szName[65];

	BOOL	m_bFindType;
};

// 등록된 사용자 함수 리스트
// 함수이름,인풋갯수,검증여부,설명 으로 구성된 문자열을 파싱하여
// 사용자 함수, 내부함수 리스트를 관리한다.
class CFunctionInfo
{
public:
	CFunctionInfo(LPSTR p_lpszFunctionName, UINT p_nCntParameter, LPSTR p_lpszComment, UINT nID)
	{
		m_szFunctionName = new char[::lstrlen(p_lpszFunctionName) + 1];
		::lstrcpy(m_szFunctionName, p_lpszFunctionName);

		m_nCntParameter = p_nCntParameter;

		m_szComment = new char[::lstrlen(p_lpszComment) + 1];
		::lstrcpy(m_szComment, p_lpszComment);

		m_nUniqueID = nID;
	}

	CFunctionInfo(CString p_strLine, UINT nID)
	{
		int nIndex = 0;
		CString strTemp(_T(""));

		AfxExtractSubString(strTemp, p_strLine, nIndex++, ',');

		m_szFunctionName = new char[strTemp.GetLength() + 1];
		::lstrcpy(m_szFunctionName, strTemp);

		AfxExtractSubString(strTemp, p_strLine, nIndex++, ',');

		m_nCntParameter = ::atoi(strTemp);

		AfxExtractSubString(strTemp, p_strLine, nIndex++, ',');

		m_bChecked = (BOOL)::atoi(strTemp);

		AfxExtractSubString(strTemp, p_strLine, nIndex++, ',');

		m_szComment = new char[strTemp.GetLength() + 1];
		::lstrcpy(m_szComment, strTemp);

		m_nUniqueID = nID;
	}

	~CFunctionInfo()
	{
		delete [] m_szFunctionName;
		delete [] m_szComment;
	}

	LPCSTR GetFunctionName()
	{
		return m_szFunctionName;
	}

	UINT GetUniqueID()
	{
		return m_nUniqueID;
	}

	UINT GetParemeterCount()
	{
		return m_nCntParameter;
	}
	
private:
	LPSTR	m_szFunctionName;
	UINT	m_nCntParameter;
	BOOL	m_bChecked;
	LPSTR	m_szComment;

	UINT	m_nUniqueID;
};

static void DeleteUserFuncInfo(CFunctionInfo* p_pUserFuncInfo)
{
	delete p_pUserFuncInfo;
}

#define VT_USERFUNCINFO vector<CFunctionInfo*>	

// CFunctionInfo의 요소를 찾기 위한 Functor
class CFindFunction
{
public:
	CFindFunction(UINT p_nID)
	{
		m_nID = p_nID;
		m_bFindType = 0;
	}
	
	CFindFunction(LPSTR p_lpszName)
	{
		::lstrcpy(m_szName, p_lpszName);
		m_bFindType = 1;
	}

	CFindFunction(LPSTR p_lpszName, int nLength)
	{
		::memset(m_szName, 0x00, sizeof(m_szName));
		::strncpy(m_szName, p_lpszName, nLength);
		m_bFindType = 1;
	}

	bool operator()(CFunctionInfo* rhs) const
	{
		if(m_bFindType == 0)
		{
			if(m_nID == rhs->GetUniqueID())
				return true;
			else
				return false;
		}
		else if(m_bFindType == 1)
		{
			if(::lstrcmp(m_szName, rhs->GetFunctionName()))
				return false;
			else
				return true;
		}

		return false;
	}

private:
	UINT	m_nID;
	char	m_szName[65];

	BOOL	m_bFindType;
};

// 선언된 변수를 관리하기 위한 리스트
class CDeclareData
{
public:
	CDeclareData(int p_pIndex, LPCSTR p_lpszDeclare, LPSTR lpszData, LPSTR lpszOrgData = NULL)
	{
		m_nIndex = ::lstrlen(p_lpszDeclare) + 1;
		m_nArraySize = 0;

		m_lpszDeclare = new char[m_nIndex];
		m_lpszDeclareOrigin = new char[m_nIndex];

		m_nIndex = ::lstrlen(lpszData) + 1;
		m_lpszDeclareInit = new char[m_nIndex];

		::lstrcpy(m_lpszDeclare, p_lpszDeclare);
		::lstrcpy(m_lpszDeclareOrigin, p_lpszDeclare);
		::lstrcpy(m_lpszDeclareInit, lpszData);

		if(lpszOrgData && ::lstrlen(lpszOrgData))
		{
			m_lpszDeclareInitOrg = new char[m_nIndex];
			::lstrcpy(m_lpszDeclareInitOrg, lpszOrgData);
		}
		else
			m_lpszDeclareInitOrg = NULL;

		::strupr(m_lpszDeclare);

		m_nIndex = p_pIndex;
	}

	CDeclareData(int p_pIndex, LPCSTR p_lpszDeclare, UINT p_nArraySize, LPSTR lpszData, LPSTR lpszOrgData = NULL)
	{
		m_nIndex = ::lstrlen(p_lpszDeclare) + 1;
		m_nArraySize = p_nArraySize;

		m_lpszDeclare = new char[m_nIndex];
		m_lpszDeclareOrigin = new char[m_nIndex];

		m_nIndex = ::lstrlen(lpszData) + 1;
		m_lpszDeclareInit = new char[m_nIndex];

		::lstrcpy(m_lpszDeclare, p_lpszDeclare);
		::lstrcpy(m_lpszDeclareOrigin, p_lpszDeclare);
		::lstrcpy(m_lpszDeclareInit, lpszData);

		if(lpszOrgData && ::lstrlen(lpszOrgData))
		{
			m_lpszDeclareInitOrg = new char[m_nIndex];
			::lstrcpy(m_lpszDeclareInitOrg, lpszOrgData);
		}
		else
			m_lpszDeclareInitOrg = NULL;

		::strupr(m_lpszDeclare);

		m_nIndex = p_pIndex;
	}

	~CDeclareData()
	{
		delete [] m_lpszDeclare;
		delete [] m_lpszDeclareOrigin;
		delete [] m_lpszDeclareInit;
		if(m_lpszDeclareInitOrg)
			delete [] m_lpszDeclareInitOrg;
	}

	LPCSTR GetDeclareString()
	{
		return m_lpszDeclare;
	}

	LPCSTR GetDeclareOriginString()
	{
		return m_lpszDeclareOrigin;
	}

	int GetDeclareIndex()
	{
		return m_nIndex;
	}

	UINT GetArraySize()
	{
		return m_nArraySize;
	}

	LPCSTR GetDeclareInitString()
	{
		if(m_lpszDeclareInitOrg)
			return m_lpszDeclareInitOrg;
		else
			return m_lpszDeclareInit;
	}

	BOOL IsIndexInnerFunction()
	{
		char* szIndexFunction[] = {_T("PLOT1"), _T("PLOT2"), _T("PLOT3"), _T("PLOT4"), _T("PLOT5"), _T("PLOT6"), _T("PLOT7"), _T("PLOT8"), _T("PLOT9"), _T("PLOT10")};
		int nSize = sizeof(szIndexFunction) / sizeof(char*);
		
		for(int i = 0; i < nSize; i++)
		{
			if(::lstrcmp(szIndexFunction[i], m_lpszDeclare) == 0)
				return TRUE;
		}

		return FALSE;
	}

private:
	int		m_nIndex;
	UINT	m_nArraySize;
	LPSTR	m_lpszDeclare;
	LPSTR	m_lpszDeclareOrigin;
	LPSTR	m_lpszDeclareInit;
	LPSTR	m_lpszDeclareInitOrg;
};

#define VT_DECLDATA vector<CDeclareData*>	

// CDeclareData의 요소를 찾기위한 Functor
class CFindVariable
{
public:
	CFindVariable(LPSTR p_lpszName)
	{
		m_bFindType = TRUE;
		::memset(m_szName, 0x00, sizeof(m_szName));
		::strncpy(m_szName, p_lpszName, min(sizeof(m_szName) - 1, ::lstrlen(p_lpszName)));
		::strupr(m_szName);

		m_nIndex = -1;
	}

	CFindVariable(LPCSTR p_lpszName)
	{
		m_bFindType = TRUE;
		::memset(m_szName, 0x00, sizeof(m_szName));
		::strncpy(m_szName, p_lpszName, min(sizeof(m_szName) - 1, ::lstrlen(p_lpszName)));
		::strupr(m_szName);

		m_nIndex = -1;
	}

	CFindVariable(int p_nIndex)
	{
		m_bFindType = FALSE;
		m_nIndex = p_nIndex;

		::memset(m_szName, 0x00, sizeof(m_szName));
	}

	bool operator()(CDeclareData* rhs) const
	{
		if(m_bFindType)
		{
			if(!::lstrcmp(m_szName, rhs->GetDeclareString()))
				return true;
			else
				return false;
		}
		else
		{
			if(m_nIndex == rhs->GetDeclareIndex())
				return true;
			else
				return false;
		}
	}

private:
	int		m_nIndex;
	char	m_szName[65];

	BOOL	m_bFindType;
};

static void DeleteVariable(CDeclareData* rhs)
{
	delete rhs;
}

// 스크립트 보안정보 클래스
class CScriptSecurity
{
public:
	CScriptSecurity() :
	m_nFileType(0), m_strScriptName(_T("")),
		m_bUsePassword(FALSE), m_bUseUserID(FALSE), m_bUseExpires(FALSE),
		m_strPassword(_T("")), m_strUserID(_T("")), m_strExpires(_T(""))
	{
	}

	CScriptSecurity(int nFileType, CString strScriptName, LPSTR lpszLine) :
	m_nFileType(nFileType), m_strScriptName(strScriptName),
		m_bUsePassword(FALSE), m_bUseUserID(FALSE), m_bUseExpires(FALSE),
		m_strPassword(_T("")), m_strUserID(_T("")), m_strExpires(_T(""))
	{		
		if(::lstrlen(lpszLine))
		{
			CString strTemp(_T(""));

			AfxExtractSubString(strTemp, lpszLine, 0, _T(','));
			m_bUsePassword = atoi(strTemp);

			AfxExtractSubString(m_strPassword, lpszLine, 1, _T(','));

			AfxExtractSubString(strTemp, lpszLine, 2, _T(','));
			m_bUseUserID = atoi(strTemp);

			AfxExtractSubString(m_strUserID, lpszLine, 3, _T(','));

			AfxExtractSubString(strTemp, lpszLine, 4, _T(','));
			m_bUseExpires = atoi(strTemp);

			AfxExtractSubString(m_strExpires, lpszLine, 5, _T(','));
		}
	}

	CScriptSecurity(int nFileType, CString strScriptName,
		BOOL bUsePassword, BOOL bUseUserID, BOOL bUseExpires,
		CString strPassword, CString strUserID, CString strExpires) :
	m_nFileType(nFileType), m_strScriptName(strScriptName),
		m_bUsePassword(bUsePassword), m_bUseUserID(bUseUserID), m_bUseExpires(bUseExpires),
		m_strPassword(strPassword), m_strUserID(strUserID), m_strExpires(strExpires)
	{
	}

	~CScriptSecurity()
	{
	}
	
	CScriptSecurity& operator=(const CScriptSecurity& info)
	{
		if(this == &info)
			return *this;

		return *this;
	}

	void SetScriptInfo(int nFileType, CString strScriptName)
	{
		m_nFileType = nFileType;
		m_strScriptName = strScriptName;
	}

	void GetScriptInfo(int& nFileType, CString& strScriptName)
	{
		nFileType = m_nFileType;
		strScriptName = m_strScriptName;
	}

	void SetUsePassword(BOOL bSet) { m_bUsePassword = bSet; }
	void SetUseUserID(BOOL bSet) { m_bUseUserID = bSet; }
	void SetUseExpires(BOOL bSet) { m_bUseExpires = bSet; }

	BOOL GetUsePassword() { return m_bUsePassword; }
	BOOL GetUseUserID() { return m_bUseUserID; }
	BOOL GetUseExpires() { return m_bUseExpires; }

	void SetPassword(CString strText) { m_strPassword = strText; }
	void SetUserID(CString strText) { m_strUserID = strText; }
	void SetExpires(CString strText) { m_strExpires = strText; }

	CString GetPassword() { return m_strPassword; }
	CString GetUserID() { return m_strUserID; }
	CString GetExpires() { return m_strExpires; }

private:
	BOOL	m_bUsePassword;
	BOOL	m_bUseUserID;
	BOOL	m_bUseExpires;

	CString m_strPassword;
	CString	m_strUserID;
	CString	m_strExpires;

	int		m_nFileType;
	CString	m_strScriptName;
};

// 선언된 함수의 인풋정보를 관리
class CFunctionInputInfo
{
public:
	CFunctionInputInfo(LPCSTR lpszInput)
	{
		m_bDefualtParameter = FALSE;

		int i = 0;
		while(lpszInput[i] != NULL)
		{
			if(lpszInput[i] == '=')
			{
				m_szInputType = new char[i + 1];
				::memset(m_szInputType, 0x00, i + 1);
				::strncpy(m_szInputType, lpszInput, i);

				int nLen = (::lstrlen(lpszInput) - i) * 2 + 10;
				m_szInputValue = new char[nLen]; 
				::memset(m_szInputValue, 0x00, nLen);
				::strcpy(m_szInputValue, lpszInput + i + 1);

				if(!::isdigit(*m_szInputValue) && *m_szInputValue != '-')
					ConversionReservedVariable(m_szInputValue, nLen);

				m_bDefualtParameter = TRUE;
				break;
			}

			i++;
		}

		if(!m_bDefualtParameter)
		{
			m_szInputType = ::strdup(lpszInput);
			m_szInputValue = NULL;
		}
	}

	~CFunctionInputInfo()
	{
		if(m_bDefualtParameter)
		{
			delete [] m_szInputType;
			delete [] m_szInputValue;
		}
		else
			::free(m_szInputType);
	}

	LPCSTR GetDefaultArgument()
	{
		return m_szInputValue;
	}

	void ConversionReservedVariable(LPSTR lpszValue, int nBufferSize)
	{
		LPSTR lpszTemp = new char[nBufferSize];
		::memset(lpszTemp, 0x00, nBufferSize);

		::strupr(lpszValue);

		int nCount = ::lstrlen(lpszValue);
		
		char* lpszReserved[] = {_T("OPEN"), _T("CLOSE"), _T("LOW"), _T("HIGH")};

		int nSize = sizeof(lpszReserved) / sizeof(char*);

		BOOL bChange = FALSE;
		for(int nIndex = 0; nIndex < nCount; nIndex++)
		{
			bChange = FALSE;
			for(int i = 0; i < nSize; i++)
			{
				if(::memcmp(lpszValue + nIndex, lpszReserved[i], ::lstrlen(lpszReserved[i])) == 0)
				{
					int nOffset = ::lstrlen(lpszReserved[i]);

					::strncat(lpszTemp, lpszValue + nIndex, nOffset);
					::lstrcat(lpszTemp, _T("DATA(0)"));

					nIndex += nOffset - 1;
					bChange = TRUE;

					break;
				}
			}

			if(!bChange)
				::strncat(lpszTemp, lpszValue + nIndex, 1);
		}

		::lstrcpy(lpszValue, lpszTemp);
		delete [] lpszTemp;
	}

private:
	BOOL	m_bDefualtParameter;
	LPSTR	m_szInputType;
	LPSTR	m_szInputValue;
};

static void DeleteFunctionInputInfo(CFunctionInputInfo* p_pInput)
{
	delete p_pInput;
}

// 각 내부함수의 정보를 관리
class CInnerFunctionInfo
{
public:
	CInnerFunctionInfo(LPCSTR lpszFunctionName, LPCSTR lpszFunctionInput, UINT nInputIndex, LPCSTR lpszSection, LPCSTR lpszKey, UINT nRefIndex)
	{	
		m_szFunctionName = ::strdup(lpszFunctionName);

		CString strArg(_T(""));
		int i = 0;
		while(AfxExtractSubString(strArg, lpszFunctionInput, i++, ',') && strArg.GetLength())
		{
			CFunctionInputInfo* pInput = new CFunctionInputInfo(strArg);
			m_vtInputs.push_back(pInput);
		}

		m_nInputIndex = nInputIndex;

		m_szSection = ::strdup(lpszSection);
		m_szKey = ::strdup(lpszKey);
		m_nRefIndex = nRefIndex;
	}

	~CInnerFunctionInfo()
	{
		::free(m_szFunctionName);
		::free(m_szSection);
		::free(m_szKey);

		std::for_each(m_vtInputs.begin(), m_vtInputs.end(), DeleteFunctionInputInfo);
		m_vtInputs.clear();
	}

	LPCSTR GetInnerFunctionName()
	{
		return m_szFunctionName;
	}

	UINT GetFunctionParameterCount()
	{
		return m_vtInputs.size();
	}
	
	UINT GetFunctionIndex()
	{
		return m_nInputIndex;
	}

	LPCSTR GetSection()
	{
		return m_szSection;
	}

	LPCSTR GetKey()
	{
		return m_szKey;
	}

	UINT GetReferenceIndex()
	{
		return m_nRefIndex;
	}

	LPCSTR GetDefaultArgument(UINT nIndex)
	{
		vector<CFunctionInputInfo*>::iterator it = m_vtInputs.begin();
		it += nIndex;

		CFunctionInputInfo* pInputInfo = (*it);
		return pInputInfo->GetDefaultArgument();
	}

	vector<CFunctionInputInfo*>* GetInputInfo()
	{
		return &m_vtInputs;
	}

private:
	LPSTR	m_szFunctionName;
	UINT	m_nInputIndex;

	LPSTR	m_szSection;
	LPSTR	m_szKey;
	UINT	m_nRefIndex;

	vector<CFunctionInputInfo*>	m_vtInputs;
};

// CInnerFunctionInfo의 요소를 찾기위한 Functor
class CFindInnerFunction
{
public:
	CFindInnerFunction(LPSTR p_lpszName)
	{
		m_szFunctionName = ::strdup(p_lpszName);
		::strupr(m_szFunctionName);
	}

	CFindInnerFunction(LPCSTR p_lpszName)
	{
		m_szFunctionName = ::strdup(p_lpszName);
		::strupr(m_szFunctionName);
	}

	CFindInnerFunction(LPSTR p_lpszName, int nLength)
	{
		m_szFunctionName = (LPSTR)::malloc(nLength + 1);
		::memset(m_szFunctionName, 0x00, nLength + 1);
		::strncpy(m_szFunctionName, p_lpszName, nLength);
		::strupr(m_szFunctionName);
	}

	void DeleteString()
	{
		::free(m_szFunctionName);
	}

	bool operator()(CInnerFunctionInfo* rhs) const
	{
		if(!::lstrcmp(m_szFunctionName, rhs->GetInnerFunctionName()))
			return true;
		else
			return false;
	}

private:
	LPSTR m_szFunctionName;
};

static void DeleteInnerFuncInfo(CInnerFunctionInfo* p_pInnerFuncInfo)
{
	delete p_pInnerFuncInfo;
}

#define VT_INNERFUNCINFO vector<CInnerFunctionInfo*>

typedef struct _ST_MAKEFILEDLL
{
	char szName[MAX_PATH];
	UINT nFileType;
} ST_MAKEFILEDLL;

class CErrorInfomation
{
public:
	CErrorInfomation(UINT nFileType, UINT nScriptType)
	{
		m_lpszError = NULL;
		m_lpszFilePath = NULL;
		m_nLineNumber = 0;
		m_nFileType = nFileType;
		m_nErrorIndex = 0;
		m_nScriptType = nScriptType;
		m_nStrategyType = 0; // 5자리 속성값 구성 (지표:신호:강세:예약:예약)
	}

	~CErrorInfomation()
	{
		if(m_lpszError)
		{
			::free(m_lpszError);
			m_lpszError = NULL;
		}

		if(m_lpszFilePath)
		{
			::free(m_lpszFilePath);
			m_lpszFilePath = NULL;
		}
	}

	UINT GetFileType()
	{
		return m_nFileType;
	}

	UINT GetScriptType()
	{
		return m_nScriptType;
	}

	void SetFileType(UINT nFileType)
	{
		m_nFileType = nFileType;
	}

	void SetScriptType(UINT nScriptType)
	{
		m_nScriptType = nScriptType;
	}

	void SetErrorFileName(LPCSTR lpszFilePath)
	{
		m_lpszFilePath = ::strdup(lpszFilePath);
	}

	void SetErrorLineInfomation(LPCSTR lpszError, UINT nLineNumber, UINT nErrorIndex)
	{
		if(lpszError)
			m_lpszError = ::strdup(lpszError);

		m_nLineNumber = nLineNumber;
		m_nErrorIndex = nErrorIndex;
	}

	void SetErrorLineInfomation2(LPCSTR lpszFilePath, UINT nLineNumber)
	{
		m_lpszFilePath = ::strdup(lpszFilePath);
		m_nLineNumber = nLineNumber;
	}

	UINT GetLineNumber()
	{
		return m_nLineNumber;
	}

	void SetLineNumber(UINT nLineNumber, UINT nErrorIndex)
	{
		m_nLineNumber = nLineNumber;
		m_nErrorIndex = nErrorIndex;
	}

	void SetErrorIntomation(LPCSTR lpszFilePath, LPCSTR lpszError, UINT nLineNumber)
	{
		m_lpszFilePath = ::strdup(lpszFilePath);
		m_lpszError = ::strdup(lpszError);
		m_nLineNumber = nLineNumber;
	}

	UINT GetErrorIndex()
	{
		return m_nErrorIndex;
	}

	LPCSTR GetFileName()
	{
		if(m_lpszFilePath)
			return m_lpszFilePath;
		else
			return _T("");
	}

	LPCSTR GetErrorString()
	{
		if(m_lpszError)
			return m_lpszError;
		else
			return _T("");
	}

	void SetStrategyType(UINT nType)
	{
		m_nStrategyType |= nType;
	}

	BOOL GetStrategyType(UINT nType)
	{
		return (m_nStrategyType & nType);
	}

	UINT GetStrategyType()
	{
		return m_nStrategyType;
	}

private:
	LPSTR m_lpszError;
	LPSTR m_lpszFilePath;

	UINT m_nLineNumber;
	UINT m_nFileType;
	UINT m_nErrorIndex;
	UINT m_nScriptType;
	UINT m_nStrategyType;
};

typedef struct {
	int									nCommand;								// 4 byte 명령
	char								sData[1];								// 데이터
} TELCOMMAND, *LPTELCOMMAND;

typedef struct {
	int									nPort;									// 포트
	char								sID[8];									// 아이디
	char								sIP[15];								// 아이피
} USER_INFO, *LPUSER_INFO;

typedef struct {
	int									nPort;									// 포트
	char								sID[8];									// 아이디
	char								sIP[15];								// 아이피
	char								sData[1];								// 채팅 데이터
} _CHAT_DATA, *LPCHAT_DATA;

typedef struct _ST_SCRIPTINFO
{
	int		m_nLanguageType;	// 사용된 언어 0 : TS, 1 : CT, 2 : YES
	LPSTR	m_lpszName;			// 파일명은 확장자 .sd만 붙이면 됨
}ST_SCRIPTINFO;

class CStartDrag
{
public:
	CStartDrag(CWnd* pWnd, CRect* pRcClient, CPoint* pPtMouse) :
	  m_pWndStartDrag(pWnd),
	  m_pRcStartDrag(pRcClient),
	  m_pPtStartDrag(pPtMouse)
	{
	}

public:
	CWnd*	m_pWndStartDrag;
	CRect*	m_pRcStartDrag;
	CPoint*	m_pPtStartDrag;
};

typedef struct _ST_SCRIPTFILEINFO
{
	char szUserID[20];
	char szPassword[20];
	char szExpires[10];
}ST_SCRIPTFILEINFO;

typedef struct _ST_SENDDATA
{
	int		m_nCommand;
	UINT	m_nStrategyType;
	int		m_nDataSize;
	char	m_szData[1];		// Data 제일 뒤에 ETX
} ST_SENDDATA;

class CConnectionIndex
{
public:
	CConnectionIndex(UINT nIndex) :
	  m_nConnectionIndex(nIndex)
	  , m_bUsed(FALSE)
	{
	}
	
	~CConnectionIndex()
	{
	}

	void ChangeUsingFlag(BOOL bUsed)
	{
		m_bUsed = bUsed;
	}

	BOOL IsUsedIndex()
	{
		return m_bUsed;
	}

	UINT GetConnectionIndex()
	{
		return m_nConnectionIndex;
	}

private:
	UINT	m_nConnectionIndex;
	BOOL	m_bUsed;
};

#endif // !defined(AFX_STRUCTDEFINE_H__18754815_9512_11D2_AC26_00104B9E52FF__INCLUDED_)
