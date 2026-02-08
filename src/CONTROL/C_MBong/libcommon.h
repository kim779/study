#ifndef __LIB_COMMON_H__
#define __LIB_COMMON_H__



/**************************************************************************************/
// Template Information - Function
/**************************************************************************************/
#include <afxtempl.h>
#include <math.h>

template<class T>
void LIB_DeleteWnd(T*& pWnd)
{
	if (pWnd == nullptr)
		return;

	if (::IsWindow(pWnd->m_hWnd))
		pWnd->DestroyWindow();

	delete pWnd;
	pWnd = nullptr;
}

template<class T>
BOOL LIB_IsWndAlive(T*& pWnd)
{
	if (pWnd == nullptr)
		return FALSE;

	return (::IsWindow(pWnd->m_hWnd));	
}

template <class T>
void LIB_memset(T* array, int nSize, double val = 0.0)
{
	if (nSize <= 0)
		return;

	for ( int ii = 0 ; ii < nSize ; ii++ )
	{
		array[ii] = (T)val;
	}
}

template <class T>
T* LIB_new(T* arg, int nSize)
{	
	arg = new T[nSize];
	return (arg);
}

template <class T>
void LIB_delete(T*& arg)
{
	if (arg == nullptr)
		return;

	delete [] arg;
	arg = nullptr;
}

/**************************************************************************************/
// Template Information - Collection Classes
/**************************************************************************************/
template <class BASE_CLASS, class KEY, class VALUE>
class CIHApartMaps : public CTypedPtrMap<BASE_CLASS, KEY, VALUE>
{
public:
	CIHApartMaps(int nBlockSize = 10) : CTypedPtrMap<BASE_CLASS, KEY, VALUE>(nBlockSize) {}
	~CIHApartMaps() { RemoveAll(); }

	void RemoveAll() 
	{	
		VALUE		pObject = nullptr;	
		POSITION	pos = nullptr;
		KEY		string;
		pos = GetStartPosition();

		while (pos)
		{
			GetNextAssoc(pos, string, pObject);

			if (pObject != nullptr)
			{
				delete [] pObject;
				pObject = nullptr;
			}
		}

		CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveAll();
	}

	BOOL RemoveKey(KEY key) 
	{
		VALUE		pObject = nullptr;	

		if (this->Lookup(key, pObject))
		{
			if (pObject != nullptr)
			{
				delete [] pObject;
				pObject = nullptr;
			}

			return CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveKey(key);	
		}
		
		return FALSE;
	}

	BOOL IsExist(KEY key)
	{
		VALUE		pObject = nullptr;	

		if (Lookup(key, pObject))
		{
			return TRUE;		
		}
		return FALSE;
	}

	VALUE GetAt(int nIndex)
	{
		int		ii = 0;
		KEY		key;
		VALUE		pObject = nullptr;
		POSITION	pos = nullptr;
		
		pos = this->GetStartPosition();
		
		while (pos)
		{
			this->GetNextAssoc(pos, key, pObject);
			if (nIndex == ii)
				break;
			ii++;
		}

		return pObject;
	}	

	VALUE GetItem(KEY key)
	{
		VALUE		pObject = nullptr;
		
		this->Lookup(key, pObject);
		return pObject;
	}
};

template<class TYPE, class ARG_TYPE>
class CIHArray : public CArray<TYPE, ARG_TYPE>
{
public:
	CIHArray() : CArray<TYPE, ARG_TYPE>() {};
	~CIHArray() { RemoveAll(); }

	void RemoveAt(int nIndex, int nCount = 1)
	{
		TYPE	pObject = nullptr;

		pObject = GetAt(nIndex);

		if (pObject != nullptr)
		{
			delete pObject;
			pObject = nullptr;
		}

		CArray<TYPE, ARG_TYPE>::RemoveAt(nIndex, nCount);
	}

	void RemoveAll()
	{
		TYPE		pObject = nullptr;
		POSITION	pos = nullptr;
		int		ii = 0;

		for ( ii = 0 ; ii < GetSize() ; ii++ )
		{
			pObject = GetAt(ii);

			if (pObject != nullptr)
			{
				delete pObject;
				pObject = nullptr;
			}
				

		}	

		CArray<TYPE, ARG_TYPE>::RemoveAll();
	}
};

template<class TYPE, class ARG_TYPE>
class CIHApartArray : public CArray<TYPE, ARG_TYPE>
{
public:
	CIHApartArray() : CArray<TYPE, ARG_TYPE>() {};
	~CIHApartArray() { RemoveAll(); }

	void RemoveAt(int nIndex, int nCount = 1)
	{
		TYPE	pObject = nullptr;

		pObject = GetAt(nIndex);

		if (pObject != nullptr)
		{
			delete [] pObject;
			pObject = nullptr;
		}

		CArray<TYPE, ARG_TYPE>::RemoveAt(nIndex, nCount);
	}

	void RemoveAll()
	{
		TYPE		pObject = nullptr;
		POSITION	pos = nullptr;
		int		ii = 0;

		for ( ii = 0 ; ii < GetSize() ; ii++ )
		{
			pObject = GetAt(ii);

			if (pObject != nullptr)
			{
				delete [] pObject;
				pObject = nullptr;
			}
		
		}	

		CArray<TYPE, ARG_TYPE>::RemoveAll();
	}
};


/**************************************************************************************/
// structure
/**************************************************************************************/
struct	__createinfo {
	CRect	rect;
	char	param[128];
};

struct jbong
{
	char	date[8];
	char	siga[8];
	char	koga[8];
	char	jega[8];
	char	jgga[8];
	char	gvol[12];
};

struct  jbong30 {
        char    digit[2];		/* 소수점자리수		*/
        char    shan[8];		/* 상한가		*/
        char    hhan[8];		/* 하한가		*/
        struct  jbong jbong[30];	/* 30일 봉		*/
};

/**************************************************************************************/
// class
/**************************************************************************************/

class CBongData
{
public:
	CBongData()	{ Reset(); }
	~CBongData()	{ Reset(); }
	void Reset()
	{
		m_date = _T("");
		m_siga = 0.0;
		m_koga = 0.0;
		m_jega = 0.0;
		m_jgga = 0.0;
		m_gvol = 0;
	}

	void SetData(struct jbong* data)
	{
		CString	tmpstr;
		
		m_date.Format("%.*s", sizeof(data->date), data->date);

		tmpstr.Format("%.*s", sizeof(data->siga), data->siga);
		m_siga = atof(tmpstr);
		
		tmpstr.Format("%.*s", sizeof(data->koga), data->koga);
		m_koga = atof(tmpstr);
		
		tmpstr.Format("%.*s", sizeof(data->jega), data->jega);
		m_jega = atof(tmpstr);
		
		tmpstr.Format("%.*s", sizeof(data->jgga), data->jgga);
		m_jgga = atof(tmpstr);

		tmpstr.Format("%.*s", sizeof(data->gvol), data->gvol);
		m_gvol = atof(tmpstr);
	}

	CString		m_date;
	double		m_siga;
	double		m_koga;
	double		m_jega;
	double		m_jgga;
	double		m_gvol;
};
/**************************************************************************************/
// Define
/**************************************************************************************/

/**************************************************************************************/
// Normal Fuction
/**************************************************************************************/
CRect	LIB_MoveWindow(CWnd* pWnd, CPoint pt);
CString LIB_GetStrToken(int nIndex, CString data, char* token);
void	LIB_MessageBox(char* fmt, ...);
CString LIB_MakeComma(CString data);
COLORREF LIB_GetReverseColor(COLORREF color);
void LIB_FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit = 12);
#endif

		