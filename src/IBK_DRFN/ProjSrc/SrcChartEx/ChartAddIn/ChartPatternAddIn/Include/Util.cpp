
#include "stdafx.h"
#include <AtlBase.h>

#include "Util.h"
#include "MainDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDicStringEx class

IMPLEMENT_DYNAMIC(CUtil, CObject)

////////////////////////////////////////////////////////////////////////////////
// 01. function명      : {CUtil}
// 02. 작  성  자      : {이준옥}
// 03. 작 성 일 자     : {1998/07/03}
// 04. function기능    : {CUtil생성자}
// 05. 입력인자        : {없음}
// 06. 출력인자        : {없음}
// 07. return 값       : {없음}
// 08. REMARKS 사항    : {}
////////////////////////////////////////////////////////////////////////////////
BOOL CUtil::IsNumber(UINT nChar)
{

	if( nChar >= '0' && nChar <= '9')  return TRUE;
	else return FALSE;
}



////////////////////////////////////////////////////////////////////////////////
// 01. function명      : {}
// 02. 작  성  자      : {이준옥}
// 03. 작 성 일 자     : {}
// 04. function기능    : {}
// 05. 입력인자        : {}
// 06. 출력인자        : {}
// 07. return 값       : {제거된 단어수}
// 08. REMARKS 사항    : {}
////////////////////////////////////////////////////////////////////////////////
int CUtil::Remove(CString &strIn, TCHAR ch, int nIndex)
{
	if(nIndex==0)	return 0;
	if( nIndex == -1) return RemoveAll( strIn, ch );

	CString szTmp;
	int	nLen = strIn.GetLength();
	int	nFoundIndex=0;

	for(int i=0;   i<nLen;	i++) {
		if( strIn[i]==ch ) {
			nFoundIndex ++;
			if(nFoundIndex != nIndex) szTmp += strIn[i];
		}
		szTmp += strIn[i];
	}

	strIn.Empty();
	strIn = szTmp;
	szTmp.Empty();

	return (nFoundIndex>0)? 1:0 ;
}

int CUtil::RemoveAll(CString &strIn, TCHAR ch)
{
	CString	szTmp;
	int	nRemove=0;
	int	nLen = strIn.GetLength();

	for(int i=0;   i<nLen;  i++) {
		if(strIn[i]!=ch) {
			szTmp += strIn[i];
			nRemove++;
		}
	}

	strIn.Empty();
	strIn = szTmp;
	szTmp.Empty();

	return nRemove;
}

char* CUtil::RemoveFront(LPCSTR pData, TCHAR ch)
{
	int nLen = strlen(pData);
	int nSameCharCnt = 0;

	for(int i=0;  i<nLen ; i++) 
	{  
		if(pData[i]!=ch) return (char *)&pData[i];
		nSameCharCnt++;
	}

	//같은 길이를 가진 
	if(ch == '0' && nSameCharCnt == nLen)
	{
		return (char*)&pData[nLen-1] ;
	}

	return (char *)pData;
}

void CUtil::GetWord(LPSTR pWord, LPSTR pLine, TCHAR chStop)
{
	int x = 0,y;

	for(x=0;((pLine[x]) && (pLine[x] != chStop));x++)
		pWord[x] = pLine[x];

	pWord[x] = '\0';
	if(pLine[x]) ++x;
	y=0;

	while(pLine[y++] = pLine[x++]);
}

int CUtil::GetString( CString& szAB, CString& szCD, TCHAR chStop)
{
	int nIndex=-1;
	nIndex = szCD.Find(chStop);
	if(nIndex==-1) return nIndex;

	szAB = szCD.Mid(0, nIndex);
	szCD = szCD.Mid( nIndex+1);
	
	return nIndex;
}

BOOL CUtil::IsOnlyNumber( LPCTSTR szData)
{
	if(strlen(szData)==0) return TRUE;

	for(int x=0; (szData[x]); x++)
		if( szData[x] > '9' || szData[x] < '0')  return FALSE;

	return TRUE;
}


CString  CUtil::GetOnlyNumber( LPCSTR szData )
{
	CString szRet=_T("");

	for(int x=0; (szData[x]); x++) {
		if( szData[x] >= '0' && szData[x] <= '9')
			szRet += szData[x];
	}

	return szRet;
}



CString  CUtil::GetNumber( LPCSTR pData )
{
	CString szRet=_T("");
	int nLen = strlen(pData);
	char	ch;

	for(int i=0;  i<nLen ; i++) {
		ch = pData[i];
		if(ch >= '0' && ch <= '9'||ch == '.' || ch == '-' || ch== '+') 
			szRet += ch;
	}
	return szRet;
}

////////////////////////////////////////////////////////////////////////////////
// 01. function명      : {GetComma}
// 02. 작  성  자      : {이준옥}
// 03. 작 성 일 자     : {1998. 6. 29}
// 04. function기능    : {숫자에 컴마를 붙임}
// 05. 입력인자        : {CString &D}
// 06. 출력인자        : {void}
// 07. return 값       : {}
// 08. REMARKS 사항    : {}
////////////////////////////////////////////////////////////////////////////////


CString CUtil::GetComma(LPCSTR pData)
{
	CString szRet=_T("");
	CString szTmp = pData;
	szTmp.TrimLeft();
	szTmp.TrimRight();

	szTmp = GetNumber(szTmp);

	if(szTmp.IsEmpty())	return "" ;
	TCHAR ch=' ';
	if(szTmp[0]=='+' ||szTmp[0]=='-') 
	{
		ch = szTmp[0];
		szTmp = szTmp.Mid(1);
	}

	// 앞의 0 제거
	CString sTemp;
	TCHAR chTemp = '0';
	sTemp = RemoveFront(szTmp, chTemp);
	
	// 소수점 이하 제거
	CString sTemp1;
	int nIndex = sTemp.Find(".");
	if (nIndex == 0)
	{
		sTemp1 = sTemp.Mid(nIndex+1);
		szTmp = "0";
	}
	else if( nIndex > 0 )
	{
		sTemp1 = sTemp.Mid(nIndex+1);
		szTmp = sTemp.Left(nIndex);
	}
	else
		szTmp = sTemp;		

	// 전부 0일 경우에 0을 추가
	if( szTmp.GetLength() == 0 )
		szTmp = "0";
	
	szTmp.MakeReverse();
	int	nLen = szTmp.GetLength();	
	szRet.Empty();
 	for(int i=0;   i<nLen;   i++) 
	{
		if(i!=0 && !(i%3))	szRet += ',';
		szRet += szTmp[i];
	}
	if(ch=='-') szRet += ch;
	szRet.MakeReverse();	

	// 소수점 이하 다시 붙임
	if( nIndex >= 0 )
	{
		szRet = szRet + "." +sTemp1;
	}

	return szRet;

}

////////////////////////////////////////////////////////////////////////////////
// 01. function명      : {GetComma}
// 02. 작  성  자      : {이준옥}
// 03. 작 성 일 자     : {1998. 6. 29}
// 04. function기능    : {첫번째 숫자값에 '+' 또는 '-' 표시가 있는지 검사한다.}
// 05. 입력인자        : {CString &D}
// 06. 출력인자        : {void}
// 07. return 값       : {표시가 있으면 TRUE, 아니면 FALSE}
// 08. REMARKS 사항    : {}
////////////////////////////////////////////////////////////////////////////////

BOOL CUtil::IsSign(LPCSTR pData)
{
	if(pData[0]=='+' || pData[0]=='-') return TRUE;
	return FALSE;
}

CString CUtil::GetMakeText(TCHAR ch, int nCnt)
{
	CString szRet;
	for(int j=0; j<nCnt; j++)	
		szRet += ch ;
	return szRet;
}

CString CUtil::SetLengthString(LPCSTR pData, int nCnt)
{
	CString strReturn;
	strReturn.Format("%-*.*s",nCnt,nCnt,pData);
	return strReturn;
	
}

CString CUtil::RemoveChar(LPCSTR pData, TCHAR ch)
{
	CString szNew;

	for( int i=0; *(pData+i); i++)
		if((pData[i])!=ch) szNew += pData[i];

	return szNew;
}

// Modify S (Add Function) 98.10.24 By JunOk Lee
BOOL CUtil::ValidNumber(LPCSTR strIn)
{                                       
	CString strTmp = strIn;
	strTmp.TrimLeft();
	strTmp.TrimRight();

	if(strTmp[0]=='+' ||strTmp[0]=='-')
		strTmp = strTmp.Mid(1);

	int nIndex = strTmp.Find(".");
	if(nIndex!=-1) {
		CString chTmp=strTmp.Mid(nIndex+1);
		nIndex = chTmp.Find(".");
		if(nIndex!=-1) return FALSE;
	}

	for(int i=0;i<strTmp.GetLength();i++) {
		if(strTmp[i]=='.') continue;
		if(!((strTmp[i] >= '0' && strTmp[i] <= '9') || strTmp[i] == '-' || strTmp[i] == '+'	|| strTmp[i] == ',')) 
			return FALSE;
	}
	
	return TRUE;
}

// Modify E (Add Function) 98.10.24 By JunOk Lee

void CUtil::TrimRight(char* pIn)
{
	int nLen = strlen(pIn);
	int nOld = nLen;
	while( ::isspace(*(pIn+nLen-1)))
	{
		nLen--;
	}
	if(nLen==nOld)	return;
	pIn[nLen] = '\0';
}

void CUtil::RemoveCharAll(CString& szData, TCHAR ch)
{
	int nLen = szData.GetLength();
	char* pData = new char[nLen+1];
	int j = 0;
	for(int i=0, j=0; i<nLen; i++){
		if(szData[i]!=ch)
			pData[j++] = szData[i];
	}
	pData[j]='\0';
	szData = pData;
	delete pData;
}

CString CUtil::AdjustMask(CString mask,CString str)
{	
	// test
	if(str == "")
	{
		return "";
	}
	// end

	int nTemp = atoi(str);
	str.Format("%d",nTemp);	
	
	CString result=""; 
	
	int j=mask.GetLength()-1;
	int i=str.GetLength()-1;
	if(j<=0)
	{	
		return str;
	}
	
	while( j >= 0)
	{
		if(i>=0)
		{
			if((mask[j] == '9') || (mask[j] == 'Z') || (mask[j] == 'z'))
			{
				result=str[i]+result;
				j--; i--;
			}
			else
			{
				result=mask[j]+result;
				j--;
			}
			
		}
		else
		{
			CString str;
			if(mask[j] == '9')
			{
				str="0";
				result=str+result;
				j--;
			}
			else if((mask[j] == 'z') || (mask[j] == 'Z'))
			{
				str="";
				result=str+result;
				j--;
			}
			else
			{
				if((j+1< mask.GetLength()-1) && (j-1 >= 0))
				{
					if((mask[j-1] != 'z') && (mask[j-1] != 'Z'))
					{
						result=mask[j]+result;
					}
				}
				j--;
			}
		}
	}
	return result;
}

CString CUtil::AdjustMask(CString mask, int nValue)
{
	CString szT;
	szT.Format("%d", nValue);
	return AdjustMask(mask, szT);		
}

CString CUtil::GetSignSimbol(int sign)
{
	switch(sign)
	{
	case 1:
		return "↑";
	case 2: 
		return "▲";
	case 3:
		//return "■";
		return "";
	case 4:
		return "↓";
	case 5:
		return "▼";
	}
	return "";
}

void CUtil::AddDashToAccount(LPCSTR _lpIn, CString& szOut)
{
	CString szIn = _lpIn;
	ASSERT(szIn.GetLength()==11);

	szIn.Insert(3, "-");
	szIn.Insert(6, "-");
	szOut = szIn;
}

// 금액을 만, 억 단위로 표시(금액이 10원단위로 들어오는 경우)
CString CUtil::GetPriceUnit(LPCSTR pData)		// JSJ_Add_040528
{

	CString szPrice, szTemp, szTemp2;
	int  nLength;

	szTemp = pData;

	// 앞의 0 제거
	TCHAR chTemp = '0';
	szTemp2 = RemoveFront(szTemp, chTemp);	

	// 콤마 제거
	szTemp2.Remove(',');

	nLength = szTemp2.GetLength();

	if (nLength >= 9)		// 10억이상
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(7);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2 + "억원";

		return szPrice;
	}
	else if (nLength >= 4)	// 1만원 이상
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(3);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2 + "만원";

		return szPrice;
	}

	if (atoi(szTemp2) == 0)
		szTemp2 = "";
	szPrice = szTemp2 + "십원";
	
	return szPrice;
}

// 금액을 만단위로 표시(금액이 1원단위로 들어오는 경우)
CString CUtil::GetPriceUnitMan(LPCSTR pData)		// JSJ_Add_040528
{

	CString szPrice, szTemp, szTemp2;
	int  nLength;

	szTemp = pData;

	// 앞의 0 제거
	TCHAR chTemp = '0';
	szTemp2 = RemoveFront(szTemp, chTemp);	

	// 콤마 제거
	szTemp2.Remove(',');

	nLength = szTemp2.GetLength();
	
	if (nLength >= 5)	// 1만원 이상
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(4);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2;

		return szPrice;
	}

	szPrice = "0";		// 만원 이하인 경우 "0" 리턴
		
	return szPrice;
}


// 거래량을 만, 억 단위로 표시(거래량이 1주 단위로 들어오는 경우)
CString CUtil::GetVolUnit(LPCSTR pData)			// JSJ_Add_040603
{

	CString szPrice, szTemp, szTemp2;
	int  nLength;

	szTemp = pData;

	// '-'부호제거
	szTemp.Remove('-');

	// 앞의 0 제거
	TCHAR chTemp = '0';
	szTemp2 = RemoveFront(szTemp, chTemp);	

	// 콤마 제거
	szTemp2.Remove(',');

	nLength = szTemp2.GetLength();

	if (nLength >= 10)		// 10억주 이상
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(8);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2 + "억주";

		return szPrice;
	}
	else if (nLength >= 6)	// 10만주 이상
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(4);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2 + "만주";

		return szPrice;
	}

	if (atoi(szTemp2) == 0)
	{
		szPrice = "주";
	}
	else
	{
		szTemp = GetComma(szTemp2);
		szPrice = szTemp + "주";
	}
		
	return szPrice;
}


// ******************************************************************
//						이전 및 다음 호가
// ******************************************************************
void CUtil::GetPre_N_NextHoga(int nJongMokGubun, long lPrice, long* lUpNextValue, long* lDownNextValue) 
{
	int _lUpBid = 5;
	int _lDownBid = 5;

	if(nJongMokGubun == KOSPI)			//거래소
	{	
		if(lPrice<5000)							{ _lUpBid = 5;		_lDownBid = 5;		}
		else if(lPrice == 5000)					{ _lUpBid = 10;		_lDownBid = 5;		}
		else if(lPrice>5000 && lPrice<10000)	{ _lUpBid = 10;		_lDownBid = 10;		}
		else if(lPrice == 10000)				{ _lUpBid = 50;		_lDownBid = 10;		}
		else if(lPrice>10000 && lPrice<50000)	{ _lUpBid = 50;		_lDownBid = 50;		}
		else if(lPrice == 50000)				{ _lUpBid = 100;	_lDownBid = 50;		}
		else if(lPrice>50000 && lPrice<100000)	{ _lUpBid = 100;	_lDownBid = 100;	}
		else if(lPrice == 100000)				{ _lUpBid = 500;	_lDownBid = 100;	}
		else if(lPrice>100000 && lPrice<500000)	{ _lUpBid = 500;	_lDownBid = 500;	}
		else if(lPrice == 500000)				{ _lUpBid = 1000;	_lDownBid = 500;	}
		else if(lPrice>500000)					{ _lUpBid = 1000;	_lDownBid = 1000;	}
	}
	else if(nJongMokGubun == KOSDAQ)  	//코스닥
	{
		if(lPrice<5000)							{ _lUpBid = 5;		_lDownBid = 5;		}
		else if(lPrice == 5000)					{ _lUpBid = 10;		_lDownBid = 5;		}
		else if(lPrice>5000 && lPrice<10000)	{ _lUpBid = 10;		_lDownBid = 10;		}
		else if(lPrice == 10000)				{ _lUpBid = 50;		_lDownBid = 10;		}
		else if(lPrice>10000 && lPrice<50000)	{ _lUpBid = 50;		_lDownBid = 50;		}
		else if(lPrice == 50000)				{ _lUpBid = 100;	_lDownBid = 50;		}
		else if(lPrice>50000)					{ _lUpBid = 100;	_lDownBid = 100;	}
	}
	else if(nJongMokGubun == ETF_KOSPI)	//ETF 거래소
	{
		if(lPrice<5000)							{ _lUpBid = 5;		_lDownBid = 5;		}
		else if(lPrice == 5000)					{ _lUpBid = 10;		_lDownBid = 5;		}
		else if(lPrice>5000 && lPrice<50000)	{ _lUpBid = 10;		_lDownBid = 10;		}
		else if(lPrice == 50000)				{ _lUpBid = 100;	_lDownBid = 10;		}
		else if(lPrice>50000 && lPrice<100000)	{ _lUpBid = 100;	_lDownBid = 100;	}
		else if(lPrice == 100000)				{ _lUpBid = 500;	_lDownBid = 100;	}
		else if(lPrice>100000 && lPrice<500000)	{ _lUpBid = 500;	_lDownBid = 500;	}
		else if(lPrice == 500000)				{ _lUpBid = 1000;	_lDownBid = 500;	}
		else if(lPrice>500000)					{ _lUpBid = 1000;	_lDownBid = 1000;	}
	}
	else if(nJongMokGubun == ETF_KOSDAQ)	//코스닥  지수종목	
	{
		//ETF 코스닥 - ETF 코스닥인 경우는 무조건 5단위로 변경되었답니다. 오늘(2004. 08.30) 부터 시행..
		_lUpBid = 5;	_lDownBid = 5;
	}

	*lUpNextValue = lPrice+_lUpBid;
	*lDownNextValue = lPrice-_lDownBid;
}


int CUtil::GetHogaTick(int nJongMokGubun, int lPrice)
{
	if(nJongMokGubun == KOSPI)			//거래소
	{		
		if(lPrice<5000)					return 5;
		else if(lPrice<10000)			return 10;
		else if(lPrice<50000)			return 50;
		else if(lPrice<100000)			return 100;
		else if(lPrice<500000)			return 500;
		else							return 1000;
	}
	else if(nJongMokGubun == KOSDAQ)		//코스닥
	{	
		if(lPrice<5000)					return 5;
		else if(lPrice<10000)			return 10;
		else if(lPrice<50000)			return 50;
		else 							return 100;

	}
	else if(nJongMokGubun == ETF_KOSPI)	//거래소 지수종목
	{ 
		//ETF 거래소
		if(lPrice<5000)								return 5; 
		else if(lPrice>=5000 && lPrice<10000)		return 10; 
		else if(lPrice>=10000 && lPrice<50000)		return 10; 
		else if(lPrice>=50000 && lPrice<100000)		return 100; 
		else if(lPrice>=100000 && lPrice<500000)	return 500; 
		else if(lPrice>=500000)						return 1000; 
	}
	else if(nJongMokGubun == ETF_KOSDAQ)	//코스닥  지수종목 
	{
		//ETF 코스닥 - ETF 코스닥인 경우는 무조건 5단위로 변경되었답니다. 오늘(2004. 08.30) 부터 시행..
		return 5;
	}

	return -99999999;
}


BOOL CUtil::GetRegKey(LPCSTR szSection, LPCSTR szKey, CString& rData)
{
	CRegKey regkey;
	if(regkey.Open(HKEY_CURRENT_USER, szSection)!=ERROR_SUCCESS)
		return FALSE;
	
	rData.GetBuffer(MAX_PATH);
	DWORD	dwCount = MAX_PATH;
	BOOL	bRet = TRUE;
	if(regkey.QueryValue((LPSTR)(LPCSTR)rData, szKey, &dwCount)!=ERROR_SUCCESS)
		bRet = FALSE;
	rData.ReleaseBuffer();

	return bRet;
}