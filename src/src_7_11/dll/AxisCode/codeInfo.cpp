#include "stdafx.h"
#include "AxisCode.h"
#include "codeInfo.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	HJCODE		"hjcode.dat"	// 현물
#define	FJCODE		"fjcode.dat"	// 선물
#define	OJCODE		"opcode.dat"	// 옵션
#define	CJCODE		"cjcode.dat"
#define	UPCODE		"upcode.dat"
#define	KFCODE		"qjcode.dat"
#define	SFCODE		"sfcode.dat"	// 주식선물
#define	PJCODE		"pjcode.dat"	// 주식옵션
#define	AXISMSG		"axismsg.dat"
#define	MAPVER		"version.ini"
#define	VTCODE		"vtcode.ini"
#define KJCODE		"kjcode.dat"	// 스타지수추가
#define XJCODE		"xjcode.dat"	// KRX100 선물 추가 2006.05.23 _KSH
#define	COCODE		"cocode.dat"	// 상품선물

CCodeInfo::CCodeInfo()
{
	pApp = (CAxisCodeApp*) AfxGetApp();
	m_tabPath = pApp->m_RootPath + "\\tab\\";
}

CCodeInfo::~CCodeInfo()
{

}

bool CCodeInfo::GetFutureList(CString &list)
{
	list = _T("");
	
	CString tmpS;
	char	buf[512];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	fjcode	*pFjCode;
	CString code, name;
	
	CString path = m_tabPath + FJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return false;
	}

	bufsize = sizeof(struct fjcode);
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;
		pFjCode = (struct fjcode *)buf;
		code = CString(pFjCode->cod2, FCodeLen);
		code.TrimRight();
		name = CString(pFjCode->hnam, FNameLen);
		name.TrimRight();
		tmpS.Format("%s\t%s\t", code, name);
		list += tmpS;
	}
	CloseHandle(fh);
	if (list.IsEmpty())
		return false;
	return true;
}

bool CCodeInfo::GetCommdtList(CString &list)
{
	list = _T("");
	
	CString tmpS;
	char	buf[512];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	cocode	*pCoCode;
	CString code, name;
	
	CString path = m_tabPath + COCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return false;
	}

	bufsize = sizeof(struct cocode);
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;
		pCoCode = (struct cocode *)buf;
		code = CString(pCoCode->codx, FCodeLen);
		code.TrimRight();
		name = CString(pCoCode->hnam, FNameLen);
		name.TrimRight();
		tmpS.Format("%s\t%s\t", code, name);
		list += tmpS;
	}
	CloseHandle(fh);
	if (list.IsEmpty())
		return false;
	return true;
}

bool CCodeInfo::GetCallList(CString &list)
{
	list = _T("");

	CString tmpS;
	char	buf[512];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	ojcode	*pOjCode;
	struct	ojcodh	*pOpHeader;
	CString code, name, price, month, atm;
	int	ii = 0, jj = 0;

	CString path = m_tabPath + OJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return false;
	}

	bufsize = sizeof(struct ojcodh);
	ZeroMemory(buf, sizeof(buf));
	ReadFile(fh, buf, bufsize, &dw, NULL);
	if (bufsize != (int)dw)
	{
		::MessageBox(NULL, "옵션 화일을 읽을 수 없습니다.", "확인", MB_OK);
		CloseHandle(fh);
		return false;
	}

	memset(&opdata, 0x00, sizeof(opdata));
	pOpHeader = (struct ojcodh *)buf;
	for (ii = 3; ii >= 0; ii--)
	{
		month = CString(&(pOpHeader->cjym[ii][2]), 4);
		memcpy(&opdata[3-ii].month, month, 4);
	}
		
	bufsize = sizeof(struct ojcode);
	for (ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;
		pOjCode = (struct ojcode *)buf;

		for (jj = 3; jj >= 0; jj--)
		{
			if (pOjCode->call[jj].yorn == '0')
				continue;
			
			code = CString(pOjCode->call[jj].cod2, OCodeLen);
			name = CString(pOjCode->call[jj].hnam, ONameLen);
			res = name.Find('\0');
			if (res != -1)
				name = name.Left(res);
			name.TrimRight();
			code.TrimRight();
			tmpS = CString(pOjCode->price, OPriceLen);
			price.Format("%.2f", float(atoi(tmpS) / 100.0));
			
			memcpy(&opdata[3-jj].data[opdata[3-jj].idx].code, code, code.GetLength());
			memcpy(&opdata[3-jj].data[opdata[3-jj].idx].name, name, name.GetLength());
			memcpy(&opdata[3-jj].data[opdata[3-jj].idx].price, price, price.GetLength());
			opdata[3-jj].idx++;
			if (pOjCode->atmg == 1)
				memcpy(&opdata[3-jj].atm, price, price.GetLength());
		}
	}

	// {월물 \t ATM \t {code \t name \t 행사가 \t}\n}
	for (ii = 0; ii < 4; ii++)
	{
		month = CString(opdata[ii].month, 4);
		atm = CString(opdata[ii].atm, 5);
		tmpS.Format("%s\t%s\t", month, atm);
		list += tmpS;
		for (int jj = 0; jj < opdata[ii].idx; jj++)
		{
			code = CString(opdata[ii].data[jj].code, 9);
			code.TrimRight();
			name = CString(opdata[ii].data[jj].name, 20);
			name.TrimRight();
			if (code.IsEmpty() || name.IsEmpty())
				continue;
			price = CString(opdata[ii].data[jj].price, 6);
			price.TrimRight();
			tmpS.Format("%s\t%s\t%s\t", code, name, price);
			list += tmpS;
		}
		list += "\n";
	}

	CloseHandle(fh);

	if (list.IsEmpty())
		return false;
	return true;
}

bool CCodeInfo::GetPutList(CString &list)
{
	list = _T("");

	CString tmpS;
	char	buf[512];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	ojcode	*pOjCode;
	struct	ojcodh	*pOpHeader;
	CString code, name, price, month, atm;
	int	ii = 0, jj = 0;

	CString path = m_tabPath + OJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return false;
	}

	bufsize = sizeof(struct ojcodh);
	ZeroMemory(buf, sizeof(buf));
	ReadFile(fh, buf, bufsize, &dw, NULL);
	if (bufsize != (int)dw)
	{
		::MessageBox(NULL, "옵션 화일을 읽을 수 없습니다.", "확인", MB_OK);
		CloseHandle(fh);
		return false;
	}

	memset(&opdata, 0x00, sizeof(opdata));
	pOpHeader = (struct ojcodh *)buf;
	for (ii = 3; ii >= 0; ii--)
	{
		month = CString(&(pOpHeader->cjym[ii][2]), 4);
		memcpy(&opdata[3-ii].month, month, 4);
	}
		
	bufsize = sizeof(struct ojcode);
	for (ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;
		pOjCode = (struct ojcode *)buf;

		for (jj = 0; jj < 4; jj++)
		{
			if (pOjCode->put[jj].yorn == '0')
				continue;
			
			code = CString(pOjCode->put[jj].cod2, OCodeLen);
			name = CString(pOjCode->put[jj].hnam, ONameLen);
			res = name.Find('\0');
			if (res != -1)
				name = name.Left(res);
			name.TrimRight();
			code.TrimRight();
			tmpS = CString(pOjCode->price, OPriceLen);
			price.Format("%.2f", float(atoi(tmpS) / 100.0));
			
			memcpy(&opdata[jj].data[opdata[jj].idx].code, code, code.GetLength());
			memcpy(&opdata[jj].data[opdata[jj].idx].name, name, name.GetLength());
			memcpy(&opdata[jj].data[opdata[jj].idx].price, price, price.GetLength());
			opdata[jj].idx++;
			if (pOjCode->atmg == 1)
				memcpy(&opdata[jj].atm, price, price.GetLength());
		}
	}

	// {월물 \t ATM \t {code \t name \t 행사가 \t}\n}
	for (ii = 0; ii < 4; ii++)
	{
		month = CString(opdata[ii].month, 4);
		atm = CString(opdata[ii].atm, 5);
		tmpS.Format("%s\t%s\t", month, atm);
		list += tmpS;
		for (int jj = 0; jj < opdata[ii].idx; jj++)
		{
			code = CString(opdata[ii].data[jj].code, 9);
			code.TrimRight();
			name = CString(opdata[ii].data[jj].name, 20);
			name.TrimRight();
			if (code.IsEmpty() || name.IsEmpty())
				continue;
			price = CString(opdata[ii].data[jj].price, 6);
			price.TrimRight();
			tmpS.Format("%s\t%s\t%s\t", code, name, price);
			list += tmpS;
		}
		list += "\n";
	}

	CloseHandle(fh);

	if (list.IsEmpty())
		return false;
	return true;
}

// 스타지수
bool CCodeInfo::GetKFutureList(CString &list)
{
	list = _T("");
	
	CString tmpS;
	char	buf[512];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	fjcode	*pFjCode;
	CString code, name;
	
	CString path = m_tabPath + KJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return false;
	}

	bufsize = sizeof(struct fjcode);
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;
		pFjCode = (struct fjcode *)buf;
		code = CString(pFjCode->cod2, FCodeLen);
		code.TrimRight();
		name = CString(pFjCode->hnam, FNameLen);
		name.TrimRight();
		tmpS.Format("%s\t%s\t", code, name);
		list += tmpS;
	}
	CloseHandle(fh);
	if (list.IsEmpty())
		return false;
	return true;
}

bool CCodeInfo::GetXFutureList(CString &list)
{
	list = _T("");
	
	CString tmpS;
	char	buf[512];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	fjcode	*pFjCode;
	CString code, name;
	
	CString path = m_tabPath + XJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return false;
	}

	bufsize = sizeof(struct fjcode);
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;
		pFjCode = (struct fjcode *)buf;
		code = CString(pFjCode->cod2, FCodeLen);
		code.TrimRight();
		name = CString(pFjCode->hnam, FNameLen);
		name.TrimRight();
		tmpS.Format("%s\t%s\t", code, name);
		list += tmpS;
	}
	CloseHandle(fh);
	if (list.IsEmpty())
		return false;
	return true;
}

// 주식선물 2006.10.27
bool CCodeInfo::GetSFutureList(CString &list)
{
// 	list = _T("");
// 	
// 	CString tmpS;
// 	char	buf[512];
// 	int	res;
// 	DWORD	dw;
// 	int	bufsize;
// 	struct	fjcode	*pFjCode;
// 	CString code, name;
// 	
// 	CString path = m_tabPath + SJCODE;
// 	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
// 		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 	if (fh == INVALID_HANDLE_VALUE)
// 	{
// 		CloseHandle(fh);
// 		return false;
// 	}
// 
// 	bufsize = sizeof(struct fjcode);
// 	for (int ii = 0; ; ii++)
// 	{
// 		res = ReadFile(fh, buf, bufsize, &dw, NULL);
// 		if (bufsize != (int)dw)
// 			break;
// 		pFjCode = (struct fjcode *)buf;
// 		code = CString(pFjCode->cod2, FCodeLen);
// 		code.TrimRight();
// 		name = CString(pFjCode->hnam, FNameLen);
// 		name.TrimRight();
// 		tmpS.Format("%s\t%s\t", code, name);
// 		list += tmpS;
// 	}
// 	CloseHandle(fh);
// 	if (list.IsEmpty())
// 		return false;
	return true;
}

// 주식옵션 list 거래대상종목구분 \t 거래대상종목명 \t
bool CCodeInfo::GetPCodeList(CString &list)
{
	list = _T("");
	
	CString tmpS;
	char	buf[512];
	int	res;
	DWORD	dw;
	int	bufsize;	
	CString tjgb, name, string;
	struct pjcode* pjcode;
		
	CString path = m_tabPath + PJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return false;
	}

	bufsize = L_pjcode;
	ZeroMemory(buf, sizeof(buf));
	ReadFile(fh, buf, bufsize, &dw, NULL);
	if (bufsize != (int)dw)
	{
		::MessageBox(NULL, "주식옵션 화일을 읽을 수 없습니다.", "확인", MB_OK);
		CloseHandle(fh);
		return false;
	}

	while (true)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pjcode = (struct pjcode *) buf;

		string = CString(pjcode->tjgb, sizeof(pjcode->tjgb));

		if (string != tjgb)
		{
			tjgb = string;
			name = CString(pjcode->snam, sizeof(pjcode->snam));
			name.TrimRight();
			tmpS.Format("%s\t%s\t", tjgb, name);
			list += tmpS;
		}
	}

	CloseHandle(fh);
	if (list.IsEmpty())
		return false;
	return true;
}

bool CCodeInfo::GetPCallList(CString &list, CString keys)
{	
	list = _T("");

	if (keys.IsEmpty())
		return false;
	
	CString tmpS, tmpSS;
	char	buf[512];
	int	res;
	DWORD	dw;
	int	bufsize;	
	CString name, code, price, mont, atm, tjgb, string;
	struct pjcode* pjcode;
	bool	flag = false;
	int index = -1;
		
	CString path = m_tabPath + PJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return false;
	}

	bufsize = L_pjcode;
	ZeroMemory(buf, sizeof(buf));
	ReadFile(fh, buf, bufsize, &dw, NULL);
	if (bufsize != (int)dw)
	{
		::MessageBox(NULL, "주식옵션 화일을 읽을 수 없습니다.", "확인", MB_OK);
		CloseHandle(fh);
		return false;
	}

	memset(&ppdata, 0x00, sizeof(ppdata));
	INT_PTR ii = 0;
	for (ii = 0; ;ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pjcode = (struct pjcode *) buf;

		tjgb = CString(pjcode->tjgb, sizeof(pjcode->tjgb));
		code = CString(pjcode->codx, sizeof(pjcode->codx));
		
		if (tjgb != keys && flag)
			break;

		if (tjgb != keys)
			continue;
		
		if (tjgb == keys && !code.IsEmpty() && code.GetAt(0) == '2')
		{
			if (!flag)
				flag = true;

			string = CString(pjcode->mont, sizeof(pjcode->mont));			
			if (mont != string)
			{
				mont = string;
				index++;
				memcpy(&ppdata[index].month, mont, mont.GetLength());
			}

			price = CString(pjcode->hsga, sizeof(pjcode->hsga));
			name = CString(pjcode->hnam, sizeof(pjcode->hnam));			
			memcpy(&ppdata[index].data[ppdata[index].idx].code, code, code.GetLength());
			memcpy(&ppdata[index].data[ppdata[index].idx].name, name, name.GetLength());
			memcpy(&ppdata[index].data[ppdata[index].idx].price, price, price.GetLength());

			ppdata[index].idx++;

			if (pjcode->atmx == 1)
				memcpy(&ppdata[index].atm, price, price.GetLength());
		}
	}
	// {월물 \t ATM \t {code \t name \t 행사가 \t}\n}
	
	for (ii = 0; ii < 4; ii++)
	{
		mont = CString(ppdata[ii].month, 4);
		atm = CString(ppdata[ii].atm, 8);

		tmpS.Format("%s\t%s\t", mont, atm);
		list += tmpS;
		for (int jj = 0; jj < ppdata[ii].idx; jj++)
		{
			code = CString(ppdata[ii].data[jj].code, 9);
			code.TrimRight();
			name = CString(ppdata[ii].data[jj].name, 30);
			name.TrimRight();
			if (code.IsEmpty() || name.IsEmpty())
				continue;
			price = CString(ppdata[ii].data[jj].price, 8);
			price.TrimRight();
			tmpS.Format("%s\t%s\t%s\t", code, name, price);
			list += tmpS;
		}
		list += "\n";
	}

	CloseHandle(fh);
	if (list.IsEmpty())
		return false;
	return true;
}

bool CCodeInfo::GetPPutList(CString &list, CString keys)
{
	list = _T("");

	if (keys.IsEmpty())
		return false;	
	
	CString tmpS, tmpSS;
	char	buf[512];
	int	res;
	DWORD	dw;
	int	bufsize;	
	CString name, code, price, mont, atm, tjgb, string;
	struct pjcode* pjcode;
	bool	flag = false;
	int index = -1;

	CString path = m_tabPath + PJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		return false;
	}

	bufsize = L_pjcode;
	ZeroMemory(buf, sizeof(buf));
	ReadFile(fh, buf, bufsize, &dw, NULL);
	if (bufsize != (int)dw)
	{
		::MessageBox(NULL, "주식옵션 화일을 읽을 수 없습니다.", "확인", MB_OK);
		CloseHandle(fh);
		return false;
	}

	memset(&ppdata, 0x00, sizeof(ppdata));
	INT_PTR ii = 0;
	for (ii = 0; ;ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pjcode = (struct pjcode *) buf;

		tjgb = CString(pjcode->tjgb, sizeof(pjcode->tjgb));
		code = CString(pjcode->codx, sizeof(pjcode->codx));
		
		if (tjgb != keys && flag)
			break;

		if (tjgb != keys)
			continue;
		
		if (tjgb == keys && !code.IsEmpty() && code.GetAt(0) == '3')
		{
			if (!flag)
				flag = true;

			string = CString(pjcode->mont, sizeof(pjcode->mont));
			if (mont != string)
			{
				mont = string;
				index++;
				memcpy(&ppdata[index].month, mont, mont.GetLength());
			}

			price = CString(pjcode->hsga, sizeof(pjcode->hsga));
			name = CString(pjcode->hnam, sizeof(pjcode->hnam));				
			memcpy(&ppdata[index].data[ppdata[index].idx].code, code, code.GetLength());
			memcpy(&ppdata[index].data[ppdata[index].idx].name, name, name.GetLength());
			memcpy(&ppdata[index].data[ppdata[index].idx].price, price, price.GetLength());

			ppdata[index].idx++;

			if (pjcode->atmx == 1)
				memcpy(&ppdata[index].atm, price, price.GetLength());
		}		
	}

	// {월물 \t ATM \t {code \t name \t 행사가 \t}\n}
	for (ii = 0; ii < 4; ii++)
	{
		mont = CString(ppdata[ii].month, 4);
		atm = CString(ppdata[ii].atm, 8);

		tmpS.Format("%s\t%s\t", mont, atm);
		list += tmpS;
		for (int jj = 0; jj < ppdata[ii].idx; jj++)
		{
			code = CString(ppdata[ii].data[jj].code, 9);
			code.TrimRight();
			name = CString(ppdata[ii].data[jj].name, 30);
			name.TrimRight();
			if (code.IsEmpty() || name.IsEmpty())
				continue;
			price = CString(ppdata[ii].data[jj].price, 8);
			price.TrimRight();
			tmpS.Format("%s\t%s\t%s\t", code, name, price);
			list += tmpS;
		}
		list += "\n";
	}

	CloseHandle(fh);
	if (list.IsEmpty())
		return false;
	return true;
}
