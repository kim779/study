// IhUtils.h: interface for the IhUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IHUTILS_H__5A28283F_90BE_4A53_A7CA_1DB8A754E480__INCLUDED_)
#define AFX_IHUTILS_H__5A28283F_90BE_4A53_A7CA_1DB8A754E480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "usedefine.h"
/**************************************************************************************/
// Template Functions
/**************************************************************************************/
// To Operate to Destroy Window and Free Window pointer 
template<class T>
void LIB_DeleteWnd(T*& pWnd)
{
	if (pWnd == NULL)
		return;

	if (::IsWindow(pWnd->m_hWnd))
		pWnd->DestroyWindow();

	delete pWnd;
	pWnd = NULL;
}

// To check the handle of this window
template<class T>
BOOL LIB_IsWndAlive(T*& pWnd)
{
	if (pWnd == NULL)
		return FALSE;

	return (::IsWindow(pWnd->m_hWnd));	
}

// to clear memory by some value
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

// to create memory like array
template <class T>
T* LIB_new(T* arg, int nSize)
{	
	arg = new T[nSize];
	return (arg);
}

// to delete memory like array
template <class T>
void LIB_delete(T*& arg)
{
	if (arg == NULL)
		return;

	delete [] arg;
	arg = NULL;
}

/**************************************************************************************/
// Template Information - Collection Classes
/**************************************************************************************/
// auto destroy pointer Maps / Array
// Warning
// 1. Don't Destroy by yourself
// 2. Don't Input not heap memory
template <class BASE_CLASS, class KEY, class VALUE>
class CIHMaps : public CTypedPtrMap<BASE_CLASS, KEY, VALUE>
{
public:
	CIHMaps(int nBlockSize = 10) : CTypedPtrMap<BASE_CLASS, KEY, VALUE>(nBlockSize) {}

	~CIHMaps() 
		{ RemoveAll(); }

	void RemoveAll() 
	{	
		VALUE		pObject = NULL;	
		POSITION	pos = NULL;
		KEY		string;
		pos = GetStartPosition();

		while (pos)
		{
			GetNextAssoc(pos, string, pObject);

			if (pObject != NULL)
			{
				delete pObject;
				pObject = NULL;	
			}
		}

		CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveAll();
	}

	BOOL RemoveKey(KEY key) 
	{
		VALUE		pObject = NULL;	

		if (this->Lookup(key, pObject))
		{
			if (pObject != NULL)
			{
				delete pObject;
				pObject = NULL;
			}

			return CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveKey(key);	
		}
		
		return FALSE;
	}

	BOOL IsExist(KEY key)
	{
		VALUE		pObject = NULL;	

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
		VALUE		pObject = NULL;
		POSITION	pos = NULL;
		
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
		VALUE		pObject = NULL;
		
		this->Lookup(key, pObject);
		return pObject;
	}
};

template <class BASE_CLASS, class KEY, class VALUE>
class CIHCharMaps : public CTypedPtrMap<BASE_CLASS, KEY, VALUE>
{
public:
	CIHCharMaps(int nBlockSize = 10) : CTypedPtrMap<BASE_CLASS, KEY, VALUE>(nBlockSize) {}
	~CIHCharMaps() { RemoveAll(); }

	void RemoveAll() 
	{	
		VALUE		pObject = NULL;	
		POSITION	pos = NULL;
		KEY		string;
		pos = GetStartPosition();

		while (pos)
		{
			GetNextAssoc(pos, string, pObject);

			if (pObject != NULL)
			{
				delete [] pObject;
				pObject = NULL;
			}
		}

		CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveAll();
	}

	BOOL RemoveKey(KEY key) 
	{
		VALUE		pObject = NULL;	

		if (this->Lookup(key, pObject))
		{
			if (pObject != NULL)
			{
				delete [] pObject;
				pObject = NULL;
			}

			return CTypedPtrMap<BASE_CLASS, KEY, VALUE>::RemoveKey(key);	
		}
		
		return FALSE;
	}

	BOOL IsExist(KEY key)
	{
		VALUE		pObject = NULL;	

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
		VALUE		pObject = NULL;
		POSITION	pos = NULL;
		
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
		VALUE		pObject = NULL;
		
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
		TYPE	pObject = NULL;

		pObject = GetAt(nIndex);

		if (pObject != NULL)
		{
			delete pObject;
		}

		CArray<TYPE, ARG_TYPE>::RemoveAt(nIndex, nCount);
	}

	void RemoveAll()
	{
		TYPE		pObject = NULL;
		POSITION	pos = NULL;
		int		ii = 0;

		for ( ii = 0 ; ii < GetSize() ; ii++ )
		{
			pObject = GetAt(ii);

			if (pObject != NULL)
			{
				delete pObject;
			}
				

		}	

		CArray<TYPE, ARG_TYPE>::RemoveAll();
	}
};

template<class TYPE, class ARG_TYPE>
class CIHCharArray : public CArray<TYPE, ARG_TYPE>
{
public:
	CIHCharArray() : CArray<TYPE, ARG_TYPE>() {};
	~CIHCharArray() { RemoveAll(); }

	void RemoveAt(int nIndex, int nCount = 1)
	{	
		TYPE	pObject = NULL;

		pObject = GetAt(nIndex);

		if (pObject != NULL)
		{
			delete [] pObject;
		}

		CArray<TYPE, ARG_TYPE>::RemoveAt(nIndex, nCount);
	}

	void RemoveAll()
	{
		TYPE		pObject = NULL;
		POSITION	pos = NULL;
		int		ii = 0;

		for ( ii = 0 ; ii < GetSize() ; ii++ )
		{
			pObject = GetAt(ii);

			if (pObject != NULL)
			{
				delete [] pObject;
			}
		
		}	

		CArray<TYPE, ARG_TYPE>::RemoveAll();
	}
};



/**************************************************************************************/
// Define
/**************************************************************************************/

#define BOX_LEFT	0x01
#define BOX_RIGHT	0x02
#define BOX_TOP		0x04
#define BOX_BOTTOM	0x08
#define BOX_ALL		(BOX_LEFT|BOX_RIGHT|BOX_TOP|BOX_BOTTOM)
#define UNUSED_VALUE	-999999999.9999
/**************************************************************************************/
// Normal Fuction
/**************************************************************************************/


/**************************************************************************************/
// Library Class
/**************************************************************************************/
class IH 
{
public:
	IH();
	virtual ~IH();
	
	// string size
	static	CSize	GetStringSize(CWnd* pWnd, CString string);
	static	int	GetStringWidth(CWnd* pWnd, CString string);
	static	int	GetStringHeight(CWnd* pWnd, CString string);

	// parsing functions
	static	CString StrToken(int nIndex, CString data, char* token);
	static	CString	Parser(CString &srcstr, CString substr);
	static	CString	MakeComma(CString data);

	// for string
	static	double	CharToDouble(char* data, int len);
	static	double	CharToDouble(CString data);
	
	// window functinos
	static	CRect	MoveWindow(CWnd* pWnd, CPoint pt);
	static	void	ShowHide(CWnd* pParent, UINT shflag, UINT first, ...);
	static	CString GetComboText(CComboBox* pCombo);
	
	// trace
	static	void	MessageBox(char* fmt, ...);

	// for drawing
	static	void	DrawBox(CDC* pDC, CRect rect, COLORREF clrLine = RGB(0, 0, 0), int opt = BOX_ALL, COLORREF clrInner = RGB(255, 255, 255), bool bFill = false);
	static	void	DrawText(CDC* pDC, CRect rect, CString string, COLORREF color, int option = DT_LEFT);
	static	COLORREF GetReverseColor(COLORREF color);
	static double CalcFutureOptionJiSu(double val, int nTick, BOOL bOption); // 선물/옵션 가격을 지정한 												// Tick 만큼 가감 처리하여 돌려주는 함수 
										// Tick 만큼 가감 처리하여 돌려주는 함수
	static int CalcStockPrice(LPCTSTR pszPrice, int nTick, BOOL bKospi);		// 주식(KOSPI, KOSDAQ) 가격을 지정한 
										// Tick 만큼 가감 처리하여 돌려주는 함수
	
	// for abroad
	static	UINT	SetCheckInfo(CWnd* pWnd, UINT id, UINT option, UINT value);
	static	void	CreateFont(CFont& font, int height, CString fname, int weight);
	static	void	CreateFont(CFont& font, struct _font* fInfo);
	static	void	CreatePointFont(CFont& font, struct _font* fInfo);
	static	void	CreatePointFont(CFont& font, CString fname, int fpoint);
	static	void	HEXDUMP(char* msg, char* buf, int len, bool bFile = false);
	static	double	Sum(CArray<double*, double*>* pArray, int nday, int nIndex = 0);
	static	double	CalculateSMA(CArray<double*, double*>* pArray, int nday, int nIndex = 0);
	static	int	CalculateSMA(CArray<double, double>& pOutput, CArray<double*, double*>* pArray, int nday);
	static	BOOL	IsFileExist(CString filename);
	static	int	GetUnitInList(CMapStringToString& mapList, CString dat, CString deli = ",", CString start = "", CString end = "");
};

#endif // !defined(AFX_IHUTILS_H__5A28283F_90BE_4A53_A7CA_1DB8A754E480__INCLUDED_)
