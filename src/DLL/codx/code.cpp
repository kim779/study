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

#define	N_CODE	6
#define	MASTERs		_T("  ")
#define	L_MASTERs	2

#define	Kospi		100	// kospi
#define	Kostar		101	// kostar
#define Future		110	// future
#define	Spread		111	// spread
#define MFuture		112	// Mini Future
#define MSpread		113	// Mini Spread
#define	CallOp		120	// call option
#define MCallOp		121	// Mini Call Option
#define	PutOp		130	// put option
#define MPutOp		131	// Mini Put Option
#define	ksOp		140	// kospi option
#define	FutureOp	150	// kospi, kostar, kospi option

#define	Kosdaq		200	// kosdaq
#define	kqFuture	210	// kosdaq future
#define	kqCallOp	220	// kosdaq call option
#define	kqPutOp		230	// kosdaq put option
#define	ETF		240	// ETF
#define ELW		250	// ELW

#define PigF		300 	// 돈육 선물 
#define PigF_SP		309
#define GoldF		310 	// 금 선물
#define GoldF_SP	319
#define KTB35		320 	// 3,5 년 국채
#define KTB35_SP	329 
#define KTB10		330 	// 10년 국채 
#define KTB10_SP	339
#define TSF		340 	// 통화 선물 
#define TSF_SP		349
#define BOND01		350 	//채권
#define BOND05		351 	//채권
#define BOND10		352 	//채권
// 2010.09.02 미니금선물 추가 by warship
#define MiniGoldF	360	// 미니금선물
#define MiniGoldF_SP	361	// 미니금선물 Spread
// 2011.09.06 주식선물 추가 by warship
#define StockF      	260	// 주식선물
#define StockF_SP	261 	// 주식선물 Spread
#define StockQF      	270	// 코스닥주식선물
#define StockQF_SP	271 	// 코스닥주식선물 Spread
#define KTB30 290  //30년 국채, 30년 국채 스프레드
#define STBOND 291   //단기사채
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CCode::GetMaster(char* root, int* ncode)
{
	*(int *)ncode = N_CODE;

	CFile	fileH;
	CString	code, name, slog;
	
	m_master.Empty();
	code.Format("%s/%s/%s", root, TABDIR, HJCODEx);
	if (!fileH.Open(code, CFile::modeRead|CFile::typeBinary))
		return false;

	int datL = gsl::narrow_cast<int>(fileH.GetLength());
	CString data;

	fileH.Read(data.GetBuffer(datL), datL);
	data.ReleaseBuffer();
	fileH.Close();

	datL /= sizeof(struct hjcodex);
	if (datL <= 0)
		return false;

	int kk{};
	std::vector<CString> vdata;
	struct	hjcodex	*hjc = (struct hjcodex *) data.GetString();

	for (int ii = 0; ii < datL; ii++, hjc++)
	{
		switch (hjc->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS /*|| hjc->ssgb == jmELW*/)
				continue;
			break;
		default:
			continue;
		}
		code = CString(hjc->code, sizeof(hjc->code));
		kk = code.Find('\0');
		if (kk != -1)
			code = code.Left(kk);
		code.TrimRight();
		code = code.Mid(1);
		kk = code.GetLength();
		if (kk > N_CODE)
			continue;	//code = code.Mid(kk - N_CODE);

		name = CString(hjc->hnam, sizeof(hjc->hnam));
		kk = name.Find('\0');
		if (kk != -1)
			name = name.Left(kk);
		name.TrimRight();
		name = name.Mid(1);

		code += MASTERs + name;
		vdata.emplace_back(code);
	}

	std::sort(vdata.rbegin(), vdata.rend(), [](CString aa, CString bb)->bool{ 
		constexpr int len = N_CODE + L_MASTERs;
		return aa.Mid(len).Compare(bb.Mid(len)) > 0 ? true : false;
	});

	for_each(vdata.begin(), vdata.end(), [&](auto item) { m_master += item + '\t'; });
	return true;
}

bool CCode::GetCategory(char* root, int kind, int index, char* rcode, bool prev)
{
	switch (kind)
	{
	case 'A': kind = -1;		break;
	case 'K': kind = jmKOSPI;	break;
	case 'Q': kind = jmKOSDAQ;	break;
	default:
		return false;
	}
	
	CFile	file;
	CString	string;

	string.Format("%s/%s/%s", root, TABDIR, HJCODEx);
	if (!file.Open(string, CFile::modeRead|CFile::typeBinary))
		return false;

	int datL = gsl::narrow_cast<int>(file.GetLength()) + 1;
	CString data;

	file.Read(data.GetBuffer(datL), datL);
	data.ReleaseBuffer();
	file.Close();

	datL /= sizeof(struct hjcodex);
	if (datL <= 0)
		return false;

	int	pos{}, value{};
	enum { csNormal = 0, csFirst, csFind, csDone } state{};
	CString	code, codex, codef;
	struct	hjcodex* hjc = (struct hjcodex*)data.GetString();

	state = csNormal;
	code  = rcode;
	codex = codef = _T("");
	for (int ii = 0, idx = 0; ii < datL; ii++, hjc++)
	{
		if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS /*|| hjc->ssgb == jmELW*/)
			continue;

		if (kind >= 0 && (index/100 != upINTEGRATE && kind != hjc->kosd))
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
						//if (hjc->ucdm == value || hjc->ucds == value)
						//	break;
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
					//	if (hjc->ucdm == value)
					//		break;
					}
					else if (value < 38)
					{
					//	if (hjc->ucds == value)
					//		break;
					}
					else if (value < 41)
					{
						if (hjc->jsiz == value - 37)
							break;
					}
					continue;
				}
				break;
			case upKOSDAQ50:
				switch (value)
				{
				case 1:
					if (hjc->kpgb == 1)
						break;
					continue;
				case 2:
					if (hjc->itgb == 1)
						break;
					continue;
				case 3:
					if (hjc->star == 1)
						break;
					continue;
				}
				break;
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
		string = string.Mid(1);
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

	strcpy_s(rcode, 128, code);
	return true;
}

int CCode::GetSpinData(DWORD type, int nfloat, int xfloat, char* data, double value, bool prev)
{
	int	idx{}, length = 0;
	CString	tmps, string;
	char	sign = '\0';
	bool	has_sign = false;

	string = data;
	idx  = string.FindOneOf("+-");
	if (idx != -1)
		has_sign = true; 
	if (xfloat > 0)
		length = string.GetLength() - xfloat;

	//NXT 중간가 틱처리
	if ((type == Kospi || type == Kosdaq) && (value != 0))
	{
		CString sOneVal, sCaledVal;  //원래 에디트값 저장
		sOneVal = string;
		sCaledVal.Format("%f", value);  //플랫폼이 계산한 값이다
		int itick{};
		if (prev) //true  
			itick = atoi(sCaledVal) - atoi(sOneVal);
		else
			itick = atoi(sOneVal) - atoi(sCaledVal);

		int nuint = itick;

		int imod = (int)value % (int)nuint;
		//prev -> false 내리기  prev->true 올리기
		if (imod != 0)
		{
			if (prev)
				nuint = nuint - imod;
			else
				nuint = imod;

			if (prev)
				value = value + nuint;
			else
				value = value - nuint;

			sCaledVal.Format("%f", value);
			strcpy_s(data, 128, sCaledVal);
		}
	}

	CString str;
 	str.Format("1:nfloat(%d) xfloat(%d) data(%s) value(%f) type(%d)\n", nfloat, xfloat, data, value, type );
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
		case MFuture:
		case Kostar:
		case Spread:
		case MSpread:
		case CallOp:
		case MCallOp:
		case PutOp:
		case MPutOp:
		case kqFuture:
		case kqCallOp:
		case kqPutOp:
		case FutureOp:	
		case ETF:
		case ELW:
			if (nfloat > 0 && value != 0) 
			{
				xfloat = idx;
				value *= pow(10, nfloat);
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
		case PigF_SP:
		case PigF:		
		case GoldF_SP:
		case GoldF:
		case MiniGoldF:
		case MiniGoldF_SP:
		case StockF:
		case StockF_SP:
			{			
				string.Format("%.0f",value);
				const int iFind = string.Find('.');
				xfloat = iFind;				

				nfloat = string.GetLength()-iFind-1;
				string.Replace(".","");	

			}		
			break;		
		case KTB10_SP:		
		case KTB35_SP:
		case TSF_SP:
		case TSF:			
		case KTB35:			
		case KTB10:	
		case BOND01:
		case BOND05:
		case BOND10:
		case KTB30:  //30년국채선물, 30년 국채선물스프레드
			{			
				string.Format("%.02f",value);

				const int iFind = string.Find('.');
				xfloat = iFind;
				
				//AfxMessageBox( string );

				nfloat = string.GetLength()-iFind-1;
				string.Replace(".","");	

			}			
			break;
		case 99:   //modi  new type 99 <-- normal
		{
			if (idx != -1)
				string = string.Left(idx);
		}
		break;
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
		case MFuture:
		case Kostar:
		case Spread:
		case MSpread:
		case CallOp:
		case MCallOp:
		case PutOp:
		case MPutOp:
		case kqFuture:
		case kqCallOp:
		case kqPutOp:
		case FutureOp:	
		case ETF:
			if (nfloat > 0 && value != 0)
			{
				xfloat = idx;
				value *= pow(10, nfloat);
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
		case PigF:
		case PigF_SP:			
		case GoldF_SP:
		case GoldF:
		case MiniGoldF:
		case MiniGoldF_SP:
		case StockF:
		case StockF_SP:
			{			
				string.Format("%.0f",value);

				const int iFind = string.Find('.');
				xfloat = iFind;
				
				nfloat = string.GetLength()-iFind-1;
				string.Replace(".","");	
			}		
			break;
		case KTB10_SP:		
		case KTB35_SP:
		case TSF_SP:
		case TSF:				
		case KTB35:				
		case KTB10:	
		case BOND01:
		case BOND05:
		case BOND10:
		case KTB30:  //30년국채선물, 30년 국채선물스프레드
			{			
				string.Format("%.02f",value);

				const int iFind = string.Find('.');
				xfloat = iFind;

				nfloat = string.GetLength()-iFind-1;
				string.Replace(".","");	
			}			
			break;	
			case 99:   //modi  new type 99 <-- normal
			{
				if (idx != -1)
					string = string.Left(idx);
			}
			break;
// 		case BOND01:
// 		case BOND05:
// 		case BOND10:
// 			{			
// 				string.Format("%.02f",value);
// 
// 				int iFind = string.Find('.');
// 				xfloat = iFind;
// 				
// 
// 				nfloat = string.GetLength()-iFind-1;
// 				string.Replace(".","");	
// 
// 			}			
// 			break;
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

	strcpy_s(data, 128, string);
	return xfloat;
}

double CCode::GetDelta(DWORD type, double value, bool prev)
{
	double	delta = 0; 	
	int val = 1;

	CString sBuffer;
	GetCurrentDirectory(256, sBuffer.GetBuffer(256));
	sBuffer.ReleaseBuffer();

	const int find = sBuffer.Find("\\exe");
	if (find != -1)
	{
		sBuffer = sBuffer.Left(find);
		EZIni ini(AxStd::FORMAT("%s/%s/%s", sBuffer, TABDIR, "axis.ini"));
		val = ini["K2023SyStem"]["Newhgunit"] << 0;
	}
	else
	{
		val = 1;
	}

	if (val)
	{
		switch (type)
		{
		case Kospi:
		case Kosdaq:
		case StockF:
		case StockF_SP:
		case StockQF:
		case StockQF_SP:
			type = 0;
		default:
			break;
		}
	}


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
	
	case MFuture: //Mini Future
	case MSpread: //Mini Spread
		delta = 0.02; break;

	case CallOp:	// call option
	case PutOp:	// put option
		if (value > 10.00)
			delta = 0.05;
		else if (value == 10.00)
			delta = prev ? 0.05 : 0.01;
		else	delta = 0.01;
		break;

	case MCallOp: //Mini Call Option
	case MPutOp: //Mini Put Option
		if (value > 10.00)
			delta = 0.05;
		else if (value == 10.00)
			delta = prev ? 0.05 : 0.02;
		else	delta = 0.02;
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
		else if (value > 1000)
			delta = 5;
		else if (value == 1000)
			delta = prev ? 5 : 1;
		else	delta = 1;
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
	case ETF:	// 20071030
		if (value > 2000)
			delta = 5;
		else if (value == 2000)
			delta = prev ? 5 : 1;
		else
			delta = 1;
		break;
	case ELW:	// 2010.08.02
		delta = 5; break;
	case PigF:
	case PigF_SP:
		delta = 5;
		break;	
	case GoldF:
	case GoldF_SP:
	case MiniGoldF:
	case MiniGoldF_SP:
		delta = 10;
		break;				
	case TSF:
	case TSF_SP:
		delta = 0.1;
		break;
	case KTB35:
	case KTB35_SP:
		delta = 0.01;
		break;
	case KTB30:  //국채선물30년,   국채선물30년 스프레드
	case KTB10:
	case KTB10_SP:
		delta = 0.02;
		break;
	case StockF:
	case StockF_SP:
		if (value > 500000)
			delta = 1000;
		else if (value == 500000)
			delta = prev ? 1000 : 500;
		else if (value > 100000)
			delta = 500;
		else if (value == 100000)
			delta = prev ? 500 : 100;
		else if (value > 50000)
			delta = 100;
		else if (value == 50000)
			delta = prev ? 100 : 50;
		else if (value > 10000)
			delta = 50;
		else if (value == 10000)
			delta = prev ? 50 : 10;
		else
			delta = 10;
		break;
	case StockQF:
	case StockQF_SP:
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
		else if (value > 1000)
			delta = 5;
		else if (value == 1000)
			delta = prev ? 5 : 1;
		else 
			delta = 1;
		break;
	case BOND01:
		delta = 0.1;
		break;
	case BOND05:
		delta = 0.5;
		break;
	case BOND10:
		delta = 1;
		break;
	//modi  new type 99 <-- normal
	case STBOND:
		delta = 100000000;
		break;
	case 99:
		delta = 1;
		break;
	default:
		if (val == 0)
		{
			if (value > 500000)
				delta = 1000;
			else if (value == 500000)
				delta = prev ? 1000 : 500;
			else if (value > 100000)
				delta = 500;
			else if (value == 100000)
				delta = prev ? 500 : 100;
			else if (value > 50000)
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
			else if (value > 1000)
				delta = 5;
			else if (value == 1000)
				delta = prev ? 5 : 1;
			else	delta = 1;
		}
		else
		{
			if (value > 500000)
				delta = 1000;
			else if (value == 500000)
				delta = prev ? 1000 : 500;
			else if (value > 200000)
				delta = 500;
			else if (value == 200000)
				delta = prev ? 500 : 100;
			else if (value > 50000)
				delta = 100;
			else if (value == 50000)
				delta = prev ? 100 : 50;
			else if (value > 20000)
				delta = 50;
			else if (value == 20000)
				delta = prev ? 50 : 10;
			else if (value > 5000)
				delta = 10;
			else if (value == 5000)
				delta = prev ? 10 : 5;
			else if (value > 2000)
				delta = 5;
			else if (value == 2000)
				delta = prev ? 5 : 1;
			else	delta = 1;

		}
		break;
	}

	return delta;
}
