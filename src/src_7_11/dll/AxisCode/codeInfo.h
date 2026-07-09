#if !defined(AFX_CODEINFO_H__984617D9_6931_4C35_A3BB_F38A21B0E850__INCLUDED_)
#define AFX_CODEINFO_H__984617D9_6931_4C35_A3BB_F38A21B0E850__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class _odata	{
public:
	bool		bUse;
	CString		code;
	CString		name;
	CString		price;
};

class _option	{
public:
	CString		month;
	CString		atm;
	CArray <_odata, _odata>	data;
};

class CCodeInfo  
{
public:
	CCodeInfo();
	virtual ~CCodeInfo();

public:
	bool	GetCommdtList(CString &list);
	bool	GetFutureList(CString &list);
	bool	GetCallList(CString &list);
	bool	GetPutList(CString &list);

	//추가
	bool	GetPCodeList(CString &list);
	bool	GetPCallList(CString &list, CString keys);
	bool	GetPPutList(CString &list, CString keys);
	bool	GetKFutureList(CString &list);
	bool	GetXFutureList(CString &list);
	//추가 2006.10
	bool	GetSFutureList(CString &list);
private:
	CAxisCodeApp*	pApp;
	CString		m_tabPath;

	struct	_opdata	{
		char	month[4];
		char	atm[6];
		int	idx;
		struct	_data {
			char	code[9];
			char	name[20];
			char	price[6];
		} data[50];
	};
	struct	_opdata	opdata[4];

	struct _ppdata {
		char	month[4];
		char	atm[8];
		int	idx;
		struct _data {
			char	code[9];
			char	name[30];
			char	price[8];
		} data[50];
	};
	struct	_ppdata ppdata[4];

};

#endif // !defined(AFX_CODEINFO_H__984617D9_6931_4C35_A3BB_F38A21B0E850__INCLUDED_)
