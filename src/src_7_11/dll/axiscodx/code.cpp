// code.cpp: implementation of the CCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axiscodx.h"
#include "code.h"
#include <math.h>

#include "../../h/jmcode.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	N_CODE	12
#define	MASTERs		_T("  ")
#define	L_MASTERs	2

#define	Kospi		100	// kospi
#define	Kostar		101	// kostar
#define Future		110	// future
#define	Spread		111	// spread
#define	CallOp		120	// call option
#define	PutOp		130	// put option
#define	ksOp		140	// kospi option
#define	FutureOp	150	// kospi, kostar, kospi option

#define	Kosdaq		200	// kosdaq
#define	kqFuture	210	// kosdaq future
#define	kqCallOp	220	// kosdaq call option
#define	kqPutOp		230	// kosdaq put option

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCode::CCode()
{
	m_master.Empty();
}

CCode::~CCode()
{
}

int compares(const void *arg1, const void *arg2)
{
	char*	src = (char *)*(int *)arg1;
	char*	des = (char *)*(int *)arg2;

	return strcmp(&src[N_CODE+L_MASTERs], &des[N_CODE+L_MASTERs]);
}

bool CCode::GetMaster(char* root, int* ncode)
{
	*(int *)ncode = N_CODE;

	CFile	fileH;
	CString	code, name, tmpS, temps;

	m_master.Empty();
	code.Format("%s/%s/%s", root, TABDIR, _T("IssBondMast.txt"));
	 
	if (!fileH.Open(code, CFile::modeRead/*|CFile::typeBinary*/))
		return false;

	int	datL = (int)fileH.GetLength();
	char	*data = new char[datL];

	fileH.Read(data, datL);
	fileH.Close();

	datL /= sizeof(struct CJcode);
	if (datL <= 0)
	{
		delete [] data;
		return false;
	}

	int	idx, kk;

	idx = 0;
	char*	(*sdata) = new char*[datL];
	struct	CJcode	*hjc = (struct CJcode *) data;

	int ii = 0;
	for (ii = 0; ii < datL; ii++, hjc++)
	{
		/*
		switch (hjc->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			//if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS || hjc->ssgb == jmELW)
			// ELW
			if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS )
				continue;
			break;
		default:
			continue;
		}
		*/
		code = CString(hjc->code, sizeof(hjc->code));
	/*	kk = code.Find('\0');
		if (kk != -1)
			code = code.Left(kk);
		code.TrimRight();

		if (code.IsEmpty()) continue;

		code = code.Mid(1);
		kk = code.GetLength();
		if (kk > N_CODE)
			code = code.Mid(kk - N_CODE);
	*/
		name = CString(hjc->name, sizeof(hjc->name));
		
		kk = name.Find('\0');
		if (kk != -1)
			name = name.Left(kk);
		//name.TrimRight();
		//name.TrimLeft();
		
		code += MASTERs + name;
		strcpy_s((char *)hjc, sizeof(struct	CJcode), code);
		
		sdata[idx++] = (char *)hjc;
	}

	qsort(sdata, idx, sizeof(char *), compares);

	for (ii = 0; ii < idx; ii++)
	{
		tmpS = sdata[ii];
	//	tmpS.Replace(_T(" "), _T(""));
	//	tmpS.Insert(12, _T("  "));
		tmpS = tmpS.Left(114);
		m_master += tmpS;
		m_master += '\t';
	}
	
	delete [] sdata;
	delete [] data;

	return true;
}

bool CCode::GetCategory(char* root, int kind, int index, char* rcode, bool prev)
{
	switch (kind)
	{
	case 'A': kind = -1;		break;
	case 'K': 
		kind = jmKOSPI;	
		if (index/100 == 6)
			kind = jm3RD;
		break;
	case 'Q': kind = jmKOSDAQ;	break;
	case 'S': kind = jmSECTOR;	break;
	default:
		return false;
	}
	
	CFile	file;
	CString	string;

	string.Format("%s/%s/%s", root, TABDIR, HJCODE);
	if (!file.Open(string, CFile::modeRead|CFile::typeBinary))
		return false;

	int	datL = (int)file.GetLength();
	char*	datB = new char[datL];

	file.Read(datB, datL);
	file.Close();

	datL /= sizeof(struct hjcode);
	if (datL <= 0)
	{
		delete[] datB;
		return false;
	}

	int	pos, value;
	enum	{ csNormal=0, csFirst, csFind, csDone } state;
	CString	code, codex, codef;
	struct	hjcode	*hjc = (struct hjcode *)datB;
	unsigned short usStgb;

	state = csNormal;
	code  = rcode;
	codex = codef = _T("");
	usStgb = 0x01 << ((index % 100) - 1);
	for (int ii = 0, idx = 0; ii < datL; ii++, hjc++)
	{
		if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS /*|| hjc->ssgb == jmELW*/)
			continue;

		if (kind >= 0 && (index/100 != upSECTOR && index/100 != upINTEGRATE && kind != hjc->kosd))
			continue;

		value = index % 100;
		switch (kind)
		{
		case jmKOSPI:
			switch (index / 100)
			{
			case upKOSPI:
				switch (value)
				{
				case 1:	break;
				case 27:
					if (hjc->jjug == value)
						break;
					continue;
				case 42:
					if (hjc->wsgb == 1)
						break;
					continue;
				default:
					if (value < 5)
					{
						if (hjc->size == value)
							break;
					}
					else if (value < 38)
					{
						if (hjc->ucdm == value || hjc->ucds == value)
							break;
					}
					else if (value <= 41)
					{
						if (hjc->jsiz == value)
							break;
					}
					continue;
				}
				break;
			case upKOSPI200:
				if (hjc->kpgb == 1 || hjc->kpgb == 2 || hjc->kpgb == 3)
					break;
				continue;
			case upKOSPIx:
				switch (value)
				{
				case 1:
					if (hjc->kpgb == 2 || hjc->kpgb == 3)
						break;
					continue;
				case 2:
					if (hjc->kpgb == 3)
						break;
					continue;
				case 3:
					if (hjc->itgb == 1)
						break;
					continue;
				}
				break;
			case upINTEGRATE:
				if (hjc->unio == 1)
					break;
			default:
				continue;
			}
			break;

		case jmKOSDAQ:
			switch (index / 100)
			{
			case upKOSDAQ:
				switch (value)
				{
				case 1:	break;
				case 2:
					if (hjc->jjug == value)
						break;
					continue;
				case 5:
					if (hjc->size == 4 && hjc->jjug == 2)
						break;
					continue;
				default:
					if (value < 5)
					{
						if (hjc->size == value)
							break;
					}
					else if (value < 15 || value == 41)
					{
						if (hjc->ucdm == value)
							break;
					}
					else if (value < 38)
					{
						if (hjc->ucds == value)
							break;
					}
					else if (value < 41)
					{
						if (hjc->jsiz == value - 37)
							break;
					}
					continue;
				}
				break;
			case upKOSTAR:
				switch (value)
				{
				case 3:
					if (hjc->star == 1)
						break;
					continue;
				}
				break;
			}
			break;
		case jmSECTOR:
			{
				if (!(index % 100))
					continue;

				if (usStgb & *(unsigned short*)&hjc->stgb)
					break;
				continue;
			}
			break;
		case jm3RD:
			{
				switch (value)
				{
				case 1:	//종합
					if (hjc->kosd == jm3RD)
						break;
					continue;
				case 2:	//벤처
					if (hjc->jjug == 3)
						break;
					continue;
				case 3: //제조
					if (hjc->ucdm == 3)
						break;
					continue;
				case 4: //컴퓨터
					if (hjc->ucdm == 4)
						break;
					continue;
				}
			}
			break;
		default:
			if (hjc->kosd == jmKOSPI || hjc->kosd == jmKOSDAQ || hjc->kosd == jm3RD)
				break;
			continue;
		}

		string = CString(hjc->code, sizeof(hjc->code));
		pos = string.Find('\0');
		if (pos != -1)
			string = string.Left(pos);
		string.TrimRight();
// 		if (hjc->ssgb != jmELW)
// 			string = string.Mid(1);
		pos = string.GetLength();
		if (pos > N_CODE)
			string = string.Mid(pos - N_CODE);
		if (idx == 0)
			codef = string;
		if (state == csFind || (code.IsEmpty() && idx == 0))
		{
			code  = string;
			state = csDone;
			break;
		}
		
		if (!code.CompareNoCase(string))
		{
			if (prev)
			{
				if (idx == 0)
					state = csFirst;
				else
				{
					code  = codex;
					state = csDone;
					break;
				}
			}
			else
				state = csFind;
		}
		codex = string;
		idx++;
	}

	switch (state)
	{
	case csFirst:
		code = codex;	break;
	case csFind:
	case csNormal:
		code = codef;	break;
	default:
		break;
	}

	delete[] datB;
	strcpy_s(rcode, code.GetLength() + 1, code);
	return true;
}

int CCode::GetSpinData(DWORD type, int nfloat, int xfloat, char* data, double value, bool prev)
{
	int	idx, length = 0;
	CString	tmps, string;
	char	sign = '\0';
	bool	has_sign = false;

	string = data;
	idx  = string.FindOneOf("+-");
	if (idx != -1)
		has_sign = true;
	if (xfloat > 0)
		length = string.GetLength() - xfloat;

	if (prev)
	{
		string.Format("%f", value);
		idx = string.FindOneOf("+-");
		if (idx != -1)
		{
			sign     = string.GetAt(idx);
			string = string.Mid(idx+1);
			if (!has_sign && xfloat > 0)
				xfloat++;
		}
		else if (has_sign && xfloat > 0)
			xfloat--;
		idx = string.Find('.');
		switch (type)
		{
		case Future:
		case Kostar:
		case Spread:
		case CallOp:
		case PutOp:
		case kqFuture:
		case kqCallOp:
		case kqPutOp:
		case FutureOp:
			if (nfloat > 0 && value != 0)
			{
				xfloat = idx;
				value *= pow((double)10, nfloat);
				string.Format("%f", value);
				idx = string.FindOneOf("+-");
				if (idx != -1)
					string = string.Mid(idx+1);
				idx = string.Find('.');
				if (idx != -1)
					string = string.Left(idx);
				string.Insert(0, CString('0', nfloat-string.GetLength()+xfloat));
				if (atof(string) == 0)
					sign = '\0';
				if (sign != '\0')
				{
					xfloat++;
					string.Insert(0, sign);
				}
				break;
			}
		default:
			if (idx != -1)
				string = string.Left(idx);
			if (atof(string) == 0)
			{
				xfloat = -1;
				sign = '\0';
			}
			if (xfloat > 0 && length >= string.GetLength())
			{
				tmps  = CString('0', length - string.GetLength() +1);
				tmps += string;
				string = tmps;
			}
			if (sign != '\0')
				string.Insert(0, sign);
			if (xfloat > 0 && length < string.GetLength() - xfloat)
				xfloat--;
			break;
		}
	}
	else
	{
		string.Format("%f", value);
		idx = string.FindOneOf("+-");
		if (idx != -1)
		{
			sign   = string.GetAt(idx);
			string = string.Mid(idx+1);
			if (!has_sign && xfloat > 0)
				xfloat++;
		}
		else if (has_sign && xfloat > 0)
			xfloat--;
		idx = string.Find('.');
		switch (type)
		{
		case Future:
		case Kostar:
		case Spread:
		case CallOp:
		case PutOp:
		case kqFuture:
		case kqCallOp:
		case kqPutOp:
		case FutureOp:
			if (nfloat > 0 && value != 0)
			{
				xfloat = idx;
				value *= pow((double)10, nfloat);
				string.Format("%f", value);
				idx = string.FindOneOf("+-");
				if (idx != -1)
					string = string.Mid(idx+1);
				idx = string.Find('.');
				if (idx != -1)
					string = string.Left(idx);
				string.Insert(0, CString('0', nfloat-string.GetLength()+xfloat));
				if (atof(string) == 0)
					sign = '\0';
				if (sign != '\0')
				{
					xfloat++;
					string.Insert(0, sign);
				}
				break;
			}
		default:
			if (idx != -1)
				string = string.Left(idx);
			if (atof(string) == 0)
			{
				xfloat = -1;
				sign = '\0';
			}
			if (xfloat > 0 && length >= string.GetLength())
			{
				tmps  = CString('0', length - string.GetLength() +1);
				tmps += string;
				string = tmps;
			}
			if (sign != '\0')
				string.Insert(0, sign);
			if (xfloat > 0 && length > string.GetLength() - xfloat)
				xfloat--;
			break;
		}
	}

	strcpy_s(data, string.GetLength() + 1, string);
	return xfloat;
}

double CCode::GetDelta(DWORD type, double value, bool prev)
{
	double	delta = 0;

	switch (type)
	{
	case Kostar:	// kostar
		delta = 0.5;	break;

	case FutureOp:	// kospi, kostar, kospi option
		if (value > 500)
			delta = 0.5;
		else if (value == 500)
			delta = prev ? 0.5 : 0.05;
		else if (value > 3.00)
			delta = 0.05;
		else if (value == 3.00)
			delta = prev ? 0.05 : 0.01;
		else	delta = 0.01;
		break;

	case Future:	// future
	case Spread:	// spread
	case kqFuture:	// kosdaq future
		delta = 0.05;	break;

	case CallOp:	// call option
	case PutOp:	// put option
		if (value > 3.00)
			delta = 0.05;
		else if (value == 3.00)
			delta = prev ? 0.05 : 0.01;
		else	delta = 0.01;
		break;

	case kqCallOp:	// kosdaq call option
	case kqPutOp:	// kosdaq put option
		if (value > 5.00)
			delta = 0.05;
		else if (value == 5.00)
			delta = prev ? 0.05 : 0.01;
		else	delta = 0.01;
		break;

	case Kospi:	// kospi
		if (value > 500000)
			delta = 1000;
		else if (value == 500000)
			delta = prev ? 1000 : 500;
		else if (value > 100000)
			delta = 500;
		else if (value == 100000)
			delta = prev ? 500 : 100;
		if (delta > 0)
			break;
	case Kosdaq:	// kosdaq
		if (value > 50000)
			delta = 100;
		else if (value == 50000)
			delta = prev ? 100 : 50;
		else if (value > 10000)
			delta = 50;
		else if (value == 10000)
			delta = prev ? 50 : 10;
		else if (value > 5000)
			delta = 10;
		else if (value == 5000)
			delta = prev ? 10 : 5;
		else	delta = 5;
		break;

	case ksOp:	// kospi option
		if (value > 10000)
			delta = 200;
		else if (value == 10000)
			delta = prev ? 200 : 100;
		else if (value > 5000)
			delta = 100;
		else if (value == 5000)
			delta = prev ? 100 : 50;
		else if (value > 2000)
			delta = 50;
		else if (value == 2000)
			delta = prev ? 50 : 20;
		else if (value > 1000)
			delta = 20;
		else if (value == 1000)
			delta = prev ? 20 : 10;
		else	delta = 10;
		break;

	default:
		break;
	}

	return delta;
}
