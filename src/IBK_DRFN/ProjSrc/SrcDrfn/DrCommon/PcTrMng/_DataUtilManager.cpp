#include "stdafx.h"
#include "PcTrMng.h"

// Manager class


////////////////////////////////////////////////////////////////////////////////
// 01. function명      : {IsNumber}
// 02. 작  성  자      : {이준옥}
// 03. 작 성 일 자     : {1998/07/03}
// 04. function기능    : {숫자인지 확인}
// 05. 입력인자        : {없음}
// 06. 출력인자        : {없음}
// 07. return 값       : {없음}
// 08. REMARKS 사항    : {}
////////////////////////////////////////////////////////////////////////////////
BOOL CDataUtilManager::IsNumber(UINT nChar)
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
int CDataUtilManager::Remove(CString &strIn, TCHAR ch, int nIndex)
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

int CDataUtilManager::RemoveAll(CString &strIn, TCHAR ch)
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

char* CDataUtilManager::RemoveFront(LPCSTR pData, TCHAR ch)
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

void CDataUtilManager::GetWord(LPSTR pWord, LPSTR pLine, TCHAR chStop)
{
	int x = 0,y;

	for(x=0;((pLine[x]) && (pLine[x] != chStop));x++)
		pWord[x] = pLine[x];

	pWord[x] = '\0';
	if(pLine[x]) ++x;
	y=0;

	while(pLine[y++] = pLine[x++]);
}

int CDataUtilManager::GetString( CString& szAB, CString& szCD, TCHAR chStop)
{
	int nIndex=-1;
	nIndex = szCD.Find(chStop);
	if(nIndex==-1) return nIndex;

	szAB = szCD.Mid(0, nIndex);
	szCD = szCD.Mid( nIndex+1);
	
	return nIndex;
}

BOOL CDataUtilManager::IsOnlyNumber( LPCTSTR szData)
{
	if(strlen(szData)==0) return TRUE;

	for(int x=0; (szData[x]); x++)
		if( szData[x] > '9' || szData[x] < '0')  return FALSE;

	return TRUE;
}


CString  CDataUtilManager::GetOnlyNumber( LPCSTR szData )
{
	CString szRet=_T("");

	for(int x=0; (szData[x]); x++) {
		if( szData[x] >= '0' && szData[x] <= '9')
			szRet += szData[x];
	}

	return szRet;
}



CString  CDataUtilManager::GetNumber( LPCSTR pData )
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


CString CDataUtilManager::GetComma(LPCSTR pData)
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
	if( nIndex >= 0 )
	{
		sTemp1 = sTemp.Mid(nIndex+1);
		szTmp = sTemp.Left(nIndex);
	}
	else
		szTmp = sTemp;		
	//

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

BOOL CDataUtilManager::IsSign(LPCSTR pData)
{
	if(pData[0]=='+' || pData[0]=='-') return TRUE;
	return FALSE;
}
/*
CString CDataUtilManager::GetComma(LPCSTR pData)
{
	CString szRet=_T("");
	CString szBefore = pData;
	CString szPoint=_T("");
	CString szTmp;
	char	ch;

	if(szBefore[0]=='+' || szBefore[0]=='-') {
		ch=szBefore[0];
		szBefore = szBefore.Mid(1);
	}

	int	nIndex = szBefore.Find('.');
	if(nIndex > 0 ) {
		szPoint = szBefore.Mid(nIndex);
		szBefore = szBefore.Mid(0, nIndex);
	}

	szBefore = GetOnlyNumber(szBefore);
	szPoint = GetOnlyNumber(szPoint);


	szBefore.MakeReverse();
	int	nLen = szBefore.GetLength();

 	for(int i=0;   i<nLen;   i++) {
		if(i!=0 && !(i%3))	szTmp += ',';
		szTmp += szBefore[i];
	}
	szTmp.MakeReverse();

	szRet.Empty();
	if( ch=='+' || ch=='-' ) szRet = ch;
	szRet += szTmp;
	if(szPoint.GetLength()>0) {
		szRet += ".";
		szRet += szPoint;
	}

	return szRet;
}

*/

CString CDataUtilManager::GetMakeText(TCHAR ch, int nCnt)
{
	CString szRet;
	for(int j=0; j<nCnt; j++)	
		szRet += ch ;
	return szRet;
}

CString CDataUtilManager::SetLengthString(LPCSTR pData, int nCnt)
{
	CString strReturn;
	strReturn.Format("%-*.*s",nCnt,nCnt,pData);
	return strReturn;
	
}

CString CDataUtilManager::RemoveChar(LPCSTR pData, TCHAR ch)
{
	CString szNew;

	for( int i=0; *(pData+i); i++)
		if((pData[i])!=ch) szNew += pData[i];

	return szNew;
}

// Modify S (Add Function) 98.10.24 By JunOk Lee
BOOL CDataUtilManager::ValidNumber(LPCSTR strIn)
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

void CDataUtilManager::TrimRight(char* pIn)
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

void CDataUtilManager::RemoveCharAll(CString& szData, TCHAR ch)
{
	int nLen = szData.GetLength();
	char* pData = new char[nLen+1];
	int i = 0, j = 0;
	for( i=0, j=0; i<nLen; i++){
		if(szData[i]!=ch)
			pData[j++] = szData[i];
	}
	pData[j]='\0';
	szData = pData;
	delete pData;
}

CString CDataUtilManager::AdjustMask(CString mask,CString str)
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

CString CDataUtilManager::AdjustMask(CString mask, int nValue)
{
	CString szT;
	szT.Format("%d", nValue);
	return AdjustMask(mask, szT);		
}

CString CDataUtilManager::GetSignSimbol(int sign)
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

void CDataUtilManager::AddDashToAccount(LPCSTR _lpIn, CString& szOut)
{
	CString szIn = _lpIn;
	ASSERT(szIn.GetLength()==11);

	szIn.Insert(3, "-");
	szIn.Insert(6, "-");
	szOut = szIn;
}
	
#include <AtlBase.h>
BOOL CDataUtilManager::GetRegKey(LPCSTR szSection, LPCSTR szKey, CString& rData)
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

CString CDataUtilManager::GetFutureString(CString strData, BOOL bJisu)
{
	strData.TrimLeft();
	strData.TrimRight();
	
	CString strRet;
	CString strTemp;
	
	if( bJisu == TRUE )
	{
		BOOL bSign = FALSE;
		CString szRtn, szSign, szPoint, szNonPoint;
		szRtn = RemoveFront(strData,'0');		// 앞자리 0 제거.
		if(IsSign(szRtn))						// 앞 부호 저장 및 제거
		{
			szSign = szRtn[0];
			szRtn = szRtn.Mid(1);
			bSign = TRUE;
		}				
		
		if (szRtn.GetLength() < 6)
		{
			szPoint.Format("%6.6s", szRtn);
			szRtn = szPoint;
			szPoint.Empty();
		}
		else if (szRtn.GetLength() > 6)
		{
			szRtn = szRtn.Left(6);
		}		
		
		if(szRtn.GetLength() == 6)
		{			
			szRtn.Replace(0x20, '0');			
			CString szTail;
			CString szHead;				
			int is_dotindex = szRtn.Find('.');
			if (is_dotindex >= 0)
			{
				szHead = szRtn.Left(is_dotindex);
				szTail = szRtn.Mid(is_dotindex+1, 2);				
			}
			else
			{
				szHead = szRtn.Left(4);
				szTail = szRtn.Mid(4,2);
			}
			szRtn = szHead + "." + szTail;
			szRtn = RemoveFront(szRtn, '0');
			
			int index = szRtn.Find('.');
			if(index >0)
			{
				szPoint = szRtn.Mid(index, 3);
				szNonPoint = szRtn.Mid(0,index);
				szRtn = szNonPoint;
				
				if (szPoint.GetLength() < 3)
				{
					szPoint = szPoint + "00";
					szPoint.Format("%-03.03s", szPoint);
				}
				szRtn += szPoint;
			}
			else if(index == 0)
			{
				if (szRtn.GetLength() < 3)
				{
					szRtn = szRtn + "00";
					szRtn.Format("%-3.3s", szRtn);
				}
				
				szRtn = "0"+ szRtn;
			}
		}
		
		if( bSign == TRUE )  
			szRtn = szSign + szRtn;
		else
			szRtn = " " + szRtn;	
		
		strRet = szRtn;
	}
	else
	{
		BOOL bSign = FALSE;
		CString szRtn, szSign, szNonPoint;
		szRtn = RemoveFront(strData,'0');		// 앞자리 0 제거.
		if(IsSign(szRtn))						// 앞 부호 저장 및 제거
		{
			szSign = szRtn[0];
			szRtn = szRtn.Mid(1);
			bSign = TRUE;
		}						
		strTemp = RemoveFront(szRtn, '0');
		
		if( bSign == TRUE )  
			szRtn = szSign + strTemp;
		else
			szRtn = " " + strTemp;	
		
		strRet = GetComma(szRtn);
	}
	
	return strRet;
}


// {{ -- 추가 2005.12.03(Sat)
int CDataUtilManager::Str2Int(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	
	if(str.GetLength()>0)
		return atoi(str);
	return 0;
}

long CDataUtilManager::Str2Long(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	
	if(str.GetLength()>0)
		return atol(str);
	return 0L;//
}

CRect CDataUtilManager::Str2Rect(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	
	int left,top,right,bottom;
	sscanf(str,"%d,%d,%d,%d", &left, &top, &right, &bottom);
	CRect tmpRect = CRect(left,top,right,bottom);
	return tmpRect;
}

COLORREF CDataUtilManager::Str2RGBColor(CString& str)
{
	int red,green, blue;
	sscanf(str, "%d,%d,%d", &red, &green, &blue);
	
	COLORREF tmpColor = RGB(red,green,blue);
	return tmpColor;
}

COLORREF CDataUtilManager::Str2RGBColor(char* pchData)
{
	int red,green, blue;
	sscanf(pchData, "%d,%d,%d", &red, &green, &blue);
	
	COLORREF tmpColor = RGB(red,green,blue);
	return tmpColor;
}

CString CDataUtilManager::Color2String(COLORREF color)
{
	CString szReturn;
	
	szReturn.Format("%d,%d,%d",GetRValue(color),GetGValue(color), GetBValue(color));
	
	return szReturn;
}

BOOL CDataUtilManager::Str2Boolean(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	
	if(str == _T("TRUE") || str == _T("1"))
		return TRUE;
	else
		return FALSE;
}

CSize CDataUtilManager::Str2Size(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	
	CSize size(0,0);
	
	int cx,cy;
	int nRet = sscanf(str, "%d,%d", &cx, &cy);
	
	if(nRet == 2)
		size = CSize(cx,cy);
	return size;
}

void CDataUtilManager::String2Olechar(LPCSTR _szFrom, OLECHAR& szTo, int nLen)
{
	MultiByteToWideChar(CP_ACP, 0, (char*)_szFrom, -1, &szTo, nLen);
}

LOGFONT CDataUtilManager::Str2LogFont(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	
	char szFontName[LF_FACESIZE] = {0,};
	double size = 10;
	int italic,underline,strikeout,weight;
	italic = underline = strikeout = weight = 0;
	
	//FontName/Size/Italic/UnderLine/StrikeOut/Weight
	int nFieldCount = sscanf(str,"%[^,],%lf,%d,%d,%d,%d", szFontName,&size,&italic,&underline,&strikeout,&weight);
	
	CClientDC dc(NULL);
	LOGFONT lf;
	lstrcpy(lf.lfFaceName, (LPCTSTR)szFontName);
	lf.lfHeight = MulDiv((int)size, dc.GetDeviceCaps(LOGPIXELSY), 72);
	lf.lfWidth = 0;
	lf.lfWeight = weight;
	
	lf.lfItalic = italic;
	lf.lfUnderline = underline;
	lf.lfStrikeOut = strikeout;
	
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfPitchAndFamily = DEFAULT_PITCH; 
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfCharSet = HANGEUL_CHARSET;                                                
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	
	return lf;	
}
// }} -- 추가 2005.12.03(Sat)

CString CDataUtilManager::ConvertIntToPoint(CString strData, int nPointPosition, int nPointCurPosition/*=-1*/)
{
	int i;
	CString strResult;
	BOOL bMinus = FALSE;
	strData.Remove(',');
	strData.TrimLeft();
	strData.TrimRight();

	if(strData.Left(1) == "-")
	{
		bMinus = TRUE;
		strData = strData.Mid(1);
	}	

	if(nPointCurPosition == -1)
	{
		if(strData.GetLength() > nPointPosition)
		{
			CString strBefore , strAfter;
			strBefore = strData.Left(strData.GetLength() - nPointPosition);
			strAfter  = strData.Right(nPointPosition);
			//InsertComma(strBefore);
			if(bMinus) strBefore = "-" + strBefore;
			strResult   = strBefore + "." + strAfter;
		}
		else
		{
			int nDif = nPointPosition - strData.GetLength();
			CString strBefore = "0.";
			for(i = 0; i < nDif ; i++)			
				strBefore = strBefore + "0";
			
			if(bMinus) strBefore = "-" + strBefore;
			strResult = strBefore + strData;
		}
	}
	else
	{
		if(strData.GetLength() > nPointCurPosition)
		{
			CString strBefore, strAfter;
			strBefore = strData.Left(strData.GetLength() - nPointCurPosition);
			strAfter  = strData.Right(nPointCurPosition);
			for( i = 0; i < nPointPosition - nPointCurPosition ; i++)
				strAfter = strAfter + "0";
			InsertComma(strBefore);
			if(bMinus) strBefore = "-" + strBefore;
			strResult   = strBefore + "." + strAfter;
		}
		else
		{
			int nDif = nPointCurPosition - strData.GetLength();
			CString strBefore = "0.", strAfter;
			for(i = 0 ; i < nDif; i++)
				strBefore = strBefore + "0";

			if(bMinus) strBefore = "-" + strBefore;
			for( i = 0; i < nPointPosition - nPointCurPosition ; i++)
				strAfter = strAfter + "0";
			strResult = strBefore + strData + strAfter;
		}
	}
	return strResult;
}

CString CDataUtilManager::ConvertPointToInt(CString strData,int nPointPositoin)
{
	int nDif;
	int i,j,nSize = strData.GetLength();
	for(i=0 ; i < nSize ; i++)
	{
		if(strData.GetAt(i) == '.')
		{
			int nPointSize = (nSize-1) - i;
			if((nDif = nPointPositoin - nPointSize) >=0)
			{
				strData = strData.Left(i) + strData.Mid(i+1);
				for(j = 0 ; j < nDif ; j++)
				{
					strData = strData + "0";
				}
			}
			else
			{
				strData = strData.Left(i) + strData.Mid(i+1,nPointPositoin);
			}
			break;
		}
	}

	if(i==nSize)
	{
		for(i = 0 ;i < nPointPositoin ; i++)
		strData = strData + "0";
	}
	
	return strData;
}

void CDataUtilManager::InsertComma(CString & str1)
{
		CString str,strTemp1,strTemp2,strTemp3;
		str = str1;
		//부호 처리.
		if(str.GetLength() > 0)
		{
			if(str.GetAt(0) == '-' || str.GetAt(0) == '+')
			{
				strTemp1 = str.Left(1);
				str = str.Mid(1);//부호제거한 문자열.
			}
		}

		int pos ;
		int x,y,i;
		//소수점 부분을 제거한다.
		if((pos = str.Find(".")) != -1)
		{
			strTemp3 = str.Mid(pos);//소수점부터 이하값을 가지고 있는 변수.
			str = str.Left(pos);//소수점 위의 내용을 가지고 있다.
		}
		//문자열의 길이를 구한다.
		int nLength = str.GetLength();
		int max;//콤마의 최대갯수

		//정수 3자리 이상의 값일 경우.
		if((x = nLength / 3) > 0)//몫->x
		{
			if((y = nLength % 3) > 0) //정확히 나누어 떨어지지 않을 때.
				max = x;
			else if(y == 0)
				max = x-1;
			
			for(i = 0 ; i < max ; i++)//실제로 콤마를 삽입하는 부분.
			{
				if((pos = str.Find(','))== -1)//콤마가 문자열에 없을 경우.
				{
					str = str.Left(nLength-3)+","+str.Mid(nLength-3);					
				}
				else
				{
					str = str.Left(pos-3)+","+str.Mid(pos-3);					
				}
			}
			str1 = strTemp1 + str + strTemp3;		
		}
}

int CDataUtilManager::chars_atoi(LPSTR lpstrSrc, UINT uSize)
{
    char tmp[20];
	memcpy(tmp, lpstrSrc, uSize);
	tmp[uSize] = 0;
	int nRetValue = atoi(tmp);
	return nRetValue;
}

double CDataUtilManager::chars_atof(LPSTR lpstrSrc, UINT uSize)
{
    char tmp[20];
	memcpy(tmp, lpstrSrc, uSize);
	tmp[uSize] = 0;
	double dRetValue = atof(tmp);
	return dRetValue;
}

CString CDataUtilManager::DoubleToStrComma(double dVal, int nSosu, BOOL bShowZero/*=TRUE*/)
{
	CString strData, strDecimal, strNonDecimal, strReturn = _T("");
	strData.Format("%.*f", nSosu, dVal);

	if(nSosu == 0)
	{
		StrToStrComma(strData, strReturn);
	}
	else
	{
		int nLen = strData.GetLength();
		strNonDecimal = strData.Left(nLen-nSosu-1);
		strDecimal = strData.Right(nSosu+1);
		StrToStrComma(strNonDecimal, strNonDecimal);
		strReturn = strNonDecimal+strDecimal;
	}

	if(bShowZero == FALSE)
	{
		if(atof(strReturn) == 0.)	strReturn = _T(" ");
	}

	return strReturn;
}

// int -> comma string
CString CDataUtilManager::IntToStrComma(int nVal, BOOL bShowZero/*=TRUE*/)
{
	CString strReturn = _T("");
	if(nVal == 0)
	{
		if(bShowZero == FALSE)	strReturn = _T("");
		else					strReturn = _T("0");
	}
	else
	{
		CString strData; 
		strData.Format("%d", nVal);
		StrToStrComma(strData, strReturn);
	}
	return strReturn;
}


int CDataUtilManager::Str2CommaStr(LPSTR dest, LPSTR src, UINT uSize)
{
    if(uSize <=0 || uSize > 20) return -1;

	int nValue = chars_atoi(src,uSize);

    char tmp[20];
	sprintf(tmp,"%d",nValue);

	uSize = strlen(tmp);

	if (uSize <=3 )
	{
		sprintf(dest,"%s",tmp);
		return 1;
	}
    int i,p,index=0, nSize = uSize;
    for(i=0,p=nSize; i < nSize;i++,p--){
        dest[index++] = tmp[i];
        if(!((p-1)%3) && p > 3) {
            dest[index++]=',';
        }
    }
    dest[index]=0;
    return 1;
}

void CDataUtilManager::StrToStrComma(LPCTSTR lpstrSrc, CString & strReturn)
{
	CString szTmp = GetNumber(lpstrSrc);

	if(szTmp.IsEmpty())	
	{
		strReturn = "0";
	}
	else
	{
		TCHAR ch = ' ';
		if(szTmp[0]=='+' ||szTmp[0]=='-') 
		{
			ch = szTmp[0];
			szTmp = szTmp.Mid(1);
		}

		szTmp.MakeReverse();
		int	nLen = szTmp.GetLength();	
		strReturn = _T("");
 		for(int i = 0; i < nLen; i++) 
		{
			if(i!=0 && !(i%3))	strReturn += ',';
			strReturn += szTmp[i];
		}
		if(ch=='-') strReturn += ch;
		strReturn.MakeReverse();	
	}
}

CString CDataUtilManager::GetStringData(CString& data, const CString& trimData, const bool bIsInside)
{
	if(data.IsEmpty())
		return "";

	int count = data.Find(trimData);
	if(count < 0)
		return "";

	int length = data.GetLength();
	CString retData;
	if(count > 0)
		retData = data.Left(count);
	data = data.Mid(count+trimData.GetLength(), length - count);

	if(bIsInside)
		return retData;
	return (retData + trimData);
}

int CDataUtilManager::GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize)
{
	int nLen = nEnd-nStart+1;
	if(nLen<=0) return 0;

	_szGetStr.Empty();

	char *szGetStr = (char*)malloc((nLen*nOneWordSize)+1);
	char *szOneWord = (char*)malloc(nOneWordSize+1);
	memset(szOneWord, 0, nOneWordSize+1);

	for(int i=nStart; i<=nEnd; i++)
	{
		sprintf(szOneWord, "%*.*d", nOneWordSize, nOneWordSize, i);
		memcpy(&szGetStr[nOneWordSize*i], szOneWord, nOneWordSize);
	}

	szGetStr[nLen*nOneWordSize] = 0;
	_szGetStr = szGetStr;
	free(szOneWord);
	free(szGetStr);

	return _szGetStr.GetLength();
}

int CDataUtilManager::DoParsing(char cParser, LPTSTR pData, int &nPos, int nEndPos, char* aString)
{
	int nDataIdx=0;
	while (pData[nPos] != 0x00)
	{
		if (pData[nPos] == cParser || nPos>=nEndPos)
		{
			nPos++;
			aString[nDataIdx] = 0;
			return nDataIdx;
		}
		else
			aString[nDataIdx++] = pData[nPos++];
	}
	return -1;
}
