#if !defined(AFX_ADK_DATAPARSER_OF_DIR_H__DEAC0189__8C9176A965AA__INCLUDED_)
#define AFX_ADK_DATAPARSER_OF_DIR_H__DEAC0189__8C9176A965AA__INCLUDED_
// 2009-10-06 By JunokLee(alzioyes)(For ADK)
// ADKDataParser.h

/* sample
#include "../../inc/ADKDataParser.h"
	{
		CString szData;
		szData = _T("123\t456\t987");
		CADKDataParser parser;
		parser.DoParse(szData, szData.GetLength());
	}
*/

#include <vector>

using namespace std;

#if !defined(_WIN32_WCE)
#pragma pack(push, 1)
#endif

#if !defined(_WIN32_WCE)
typedef std::vector<char*> CHARVECTOR;
typedef std::vector<std::vector<char*> > CHARVECTOR2;
typedef CHARVECTOR::iterator CHARVECTORIT;
typedef CHARVECTOR2::iterator CHARVECTOR2IT;
#else
typedef std::vector<TCHAR*> CHARVECTOR;
typedef std::vector<std::vector<TCHAR*> > CHARVECTOR2;
typedef CHARVECTOR::iterator CHARVECTORIT;
typedef CHARVECTOR2::iterator CHARVECTOR2IT;
#endif

typedef std::vector<string> VTSTRING;
typedef std::vector<std::vector<string> > VTSTRING2;

typedef std::vector<unsigned short> SHORTVECTOR;
typedef std::vector<std::vector<unsigned short> > SHORTVECTOR2;
// 컨트롤 윈도우핸들에 사용한다. 
typedef std::vector<HWND> HWNDVECTOR;
typedef std::vector<long> LONGVECTOR;

typedef HWNDVECTOR::iterator HWNDVECTORIT;
typedef LONGVECTOR::iterator LONGVECTORIT;

// #include "../../../inc/Util_DataParser.h"
class CADKDataParser
{
public:
	char	m_cTrim;

	CADKDataParser()
	{
		m_cTrim = '\t';
	}
	~CADKDataParser()
	{
		DoClear();
	}

	CHARVECTOR m_DataVt;

	void DoParse(LPCSTR szDataString, int nDataLen)
	{
		if(nDataLen<=0) return;

		bool bEndSame = true;
		if(szDataString[nDataLen-1]!=m_cTrim) bEndSame=false;

		//char pSendBuf[MAX_PACKET_SIZE]={0,};
		int nPos=0;
		char pSendBuf[MAX_PATH]={0,};
		for(int i=0; i<nDataLen; i++)
		{			
			if(szDataString[i]==m_cTrim)
			{
				pSendBuf[nPos] = NULL;
				nPos = 0;
				_DoAdd(pSendBuf);
				continue;
			}
			pSendBuf[nPos] = szDataString[i];
			nPos++;
		}
		if(!bEndSame)
		{
			pSendBuf[nPos] = NULL;
			_DoAdd(pSendBuf);
		}
	}

	void DoClear()
	{
		m_DataVt.clear();
	}

protected:
	void _DoAdd(char* rDataString)
	{
		int nLen = strlen(rDataString);
		if(nLen>0)
		{
			//char* pszItem = new char[nLen+1];
			//memcpy(pszItem, rDataString, nLen);
			//pszItem[nLen] = 0;
			//m_DataVt.push_back(pszItem);
			m_DataVt.push_back(rDataString);
		}
	}
};

#if !defined(_WIN32_WCE)
#pragma pack(pop)
#endif

#endif // AFX_ADK_DATAPARSER_OF_DIR_H__DEAC0189__8C9176A965AA__INCLUDED_

