// UChar.cpp: implementation of the UChar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UChar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma	warning(disable:4018)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UChar::UChar()
{

}

UChar::~UChar()
{

}

double UChar::atoff(LPSTR src, UINT uSize) 
{
//	char	*tmp = new char[uSize+1];								// Convert Temp buffer
    char tmp[20];
	memcpy(tmp, src, uSize);
	tmp[uSize] = 0;
	double fRetValue = atof(tmp);
//	delete tmp;
	return fRetValue;
}

int UChar::atoif(LPSTR src, UINT uSize) 
{
    char tmp[20];
	memcpy(tmp, src, uSize);
	tmp[uSize] = 0;
	int nRetValue = atoi(tmp);
	return nRetValue;
}

// char형으로 데이터 추출, return값: 문자길이
int UChar::atoaf(LPSTR src, UINT uSize, LPSTR dest)
{
	memcpy(dest, src, uSize);
	dest[uSize] = 0x00;

	return uSize;
}

// CString형으로 데이터 추출, return값: 문자길이
CString UChar::atoaf(LPSTR src, UINT uSize)
{
    char tmp[512];
	memcpy(tmp, src, uSize);
	tmp[uSize] = 0x00;

	CString strTmp(tmp);	strTmp.TrimLeft();	strTmp.TrimRight();
	return strTmp;
}

int UChar::RemoveChar(LPSTR pData, char ch, UINT uSize)
{
	CString szNew;

	for( int i=0; i < uSize; i++)
		if((pData[i])!=ch) szNew += pData[i];
	
	return atoif((LPSTR)(LPCTSTR)szNew,szNew.GetLength());	 
}

int UChar::RemoveChar(CString strData, char ch)
{
	strData.Remove(ch);
	
	return atoif((LPSTR)(LPCTSTR)strData,strData.GetLength());	 
}

int UChar::Str2CommaStr(LPSTR dest, LPSTR src,UINT uSize)
{
    if(uSize <=0 || uSize > 20) return -1;

	int nValue = atoif(src,uSize);

    char tmp[20];
	sprintf(tmp,"%d",nValue);

	uSize = strlen(tmp);

	if (uSize <=3 )
	{
		sprintf(dest,"%s",tmp);
		return 1;
	}
    short i,p,index=0;

    for(i=0,p=uSize; i < uSize;i++,p--){
        dest[index++] = tmp[i];
        if(!((p-1)%3) && p > 3) {
            dest[index++]=',';
        }
    }
    dest[index]=0;
    return 1;
}

int UChar::Double2CommaStr(LPSTR dest, double src)
{
    char tmp[20];

	sprintf(tmp,"%.0f",src);

	int uSize = strlen(tmp);

	if (uSize <=3 )
	{
		sprintf(dest,"%s",tmp);
		return 1;
	}

    short i,p,index=0;
    for(i=0,p=uSize; i < uSize;i++,p--){
        dest[index++] = tmp[i];
        if(!((p-1)%3) && p > 3 && tmp[i] != '-') {
            dest[index++]=',';
        }
    }

    dest[index]=0;
    return 1;
}
int UChar::Int2CommaStr(LPSTR dest, int src)
{
    char tmp[20];

	sprintf(tmp,"%d",src);

	int uSize = strlen(tmp);

	if (uSize <=3 )
	{
		sprintf(dest,"%s",tmp);
		return 1;
	}

    short i,p,index=0;
    for(i=0,p=uSize; i < uSize;i++,p--){
        dest[index++] = tmp[i];
        if(!((p-1)%3) && p > 3 && tmp[i] != '-') {
            dest[index++]=',';
        }
    }

    dest[index]=0;
    return 1;
}



// TEXT 값을 STRING으로 치환한다
void UChar::SetTextItem(char *str, int len, char *src)
{
	char buff[8192];

	sprintf(buff, "%-*.*s", len, len, src);
	memcpy(str, buff, len);

	return ;
}

// DOUBLE 값을 STRING으로 치환한다
void UChar::SetDoubleItem(char *str, int len, double value, int point/*=2*/)
{
	char tmp[32];

	sprintf(tmp, "%0*.*f", len, point, value);
	memcpy(str, tmp, len);

	return ;
}

// DOUBLE 값을 STRING으로 치환한다
void UChar::SetIntItem(char *str, int len, int value)
{
	char tmp[32];

	sprintf(tmp, "%0*d", len, value);
	memcpy(str, tmp, len);

	return ;
}


// double 값을 long값으로 치환한다(기본 100(10^2) 곱하기)
long UChar::dtol(const double value, int point/*=2*/)
{
	CString strTmp;

	double tmp = (point <= 0) ? value : (value * pow(10, point));

	strTmp.Format("%.*f", point, tmp);

	return atol(strTmp);
}

// long 값을 doulbe 값으로 치환한다(기본 100(10^2) 나누기)
double UChar::ltod(const long   value, int point/*=2*/)
{
	CString strTmp;

	double tmp = (point <= 0) ? value : (value / pow(10, point));

	strTmp.Format("%.*f", point, tmp);

	return atof(strTmp);
}

