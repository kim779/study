#if !defined(AFX_FINDDATA_GRID_UTIL_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_FINDDATA_GRID_UTIL_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

//#include "../Common/FindDataGridUtil.h"
// 2011.08.29 Created By Junok Lee

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ugctrl.h"

class CFindData_GridUtil
{
public:
	static int GetGridAllWidth(CUGCtrl*pGridCtrl, CMapStringToString *pUserColWidthsMap)
	{
		pUserColWidthsMap->RemoveAll();

		int nColCnt = pGridCtrl->GetNumberCols();
		CString szTmp;
		int nCol;
		for (nCol = 0; nCol < nColCnt; nCol++)
		{
			LPCSTR szCellText = pGridCtrl->QuickGetText(nCol, -1);
			szTmp.Format("%d", pGridCtrl->GetColWidth(nCol));
			
			if(szCellText!=NULL)
				pUserColWidthsMap->SetAt(szCellText, szTmp);
		}

		return pUserColWidthsMap->GetCount();
	}

	static void SetGridAllWidth(CUGCtrl*pGridCtrl, CMapStringToString *pUserColWidthsMap)
	{
		int nColCnt = pGridCtrl->GetNumberCols();
		CString szTmp;
		int nCol, nWidth;
		for (nCol = 0; nCol < nColCnt; nCol++)
		{
 			LPCSTR szCellText = pGridCtrl->QuickGetText(nCol, -1);
			if(pUserColWidthsMap->Lookup(szCellText, szTmp))
			{
				nWidth = atoi(szTmp);
				pGridCtrl->SetColWidth(nCol, nWidth);
			}
		}
	}

	static void SetBunSinFieldWidth(CUGCtrl* pGridCtrl, CMapStringToString* pUserColWidthsMap,
		CString strCellText, int nCol, int nWidth)
	{
		CString szTmp;
		szTmp.Format("%d", nWidth);
		
		CString strColWidth;
		if(pUserColWidthsMap->Lookup(strCellText, strColWidth))
		{
			pUserColWidthsMap->SetAt(strCellText, szTmp);
			pGridCtrl->SetColWidth(nCol, nWidth);
		}
	}

	static int LoadFromUserEnvFile(LPCSTR szINI, LPCSTR szSectKey, CMapStringToString *pUserColWidthsMap)
	{
		pUserColWidthsMap->RemoveAll();

		IniToEnvironmentTable(szINI, szSectKey, pUserColWidthsMap);

		return pUserColWidthsMap->GetCount();
	}

	static int SaveToUserEnvFile(LPCSTR szINI, LPCSTR szSectKey, CMapStringToString *pUserColWidthsMap)
	{
		POSITION pos = pUserColWidthsMap->GetStartPosition();
		CString rKey, rValue, sTmp, sBuffStr;
		//CStringList sKeyValue;
		while(pos)
		{
			pUserColWidthsMap->GetNextAssoc(pos, rKey, rValue);
			//sTmp = rKey + rValue;
			//sKeyValue.AddTail(sTmp);
			sTmp.Format("%s=%s\r\n", rKey ,rValue);			
			sBuffStr+=sTmp;
		}

		int nSize = sBuffStr.GetLength();
		if(nSize==0) return 0;

		char *pBuff = new char[nSize+2];
		memcpy(pBuff, (LPCSTR)sBuffStr, nSize);
		pBuff[nSize]=0;
		pBuff[nSize+1]=0;
		::WritePrivateProfileSection(szSectKey, pBuff, szINI);
		delete pBuff;

// 		int nCount = sKeyValue.GetCount();
// 		char aCount[3+1];
// 		CString szData
// 		for(int i=0; i<nCount; i++)
// 		{
// 			sprintf(aCount, "%d", i++);
// 
// 		}
// 		return nCount;
		return pUserColWidthsMap->GetCount();
	}

	static int IniToEnvironmentTable(LPCSTR szIniPath, LPCSTR szSect, CMapStringToString* pEnvDB)
	{
		int nAddCnt=0;
		
		char *pReturnedStrng=(char*)malloc(9600);	//Section MaxSize 9600.
		DWORD nSize = 9600;
		
		DWORD lReads = ::GetPrivateProfileSection(szSect, pReturnedStrng, 9600, szIniPath);
		if(lReads<0)
		{
			// 데이터가 많으면 64K까지 지원한다.
			free(pReturnedStrng);
			pReturnedStrng = (char*)malloc(64000);	//64K.
			nSize = 64000;
			
			lReads = ::GetPrivateProfileSection(szSect, pReturnedStrng, 9600, szIniPath);
			if(lReads<0)
			{
				free(pReturnedStrng);
				return nAddCnt;
			}
		}
		
		int nStart=0, nEnd=0;
		int nStrSize=0;
		//char aLine[MAX_PATH], aKey[100];//, aValue[MAX_PATH];
		CString szLine, szKey, szValue;
		szLine.GetBufferSetLength(MAX_PATH);
		for(int i=0; i<(int)lReads; i++)
		{
			if(pReturnedStrng[i] == 0x00)
			{
				memcpy((LPSTR)(LPCSTR)szLine, &pReturnedStrng[nStart], i-nStart+1);	//Null포함.
				nStart = i+1;
				
				if(AfxExtractSubString(szKey, szLine, 0, '=') == TRUE)
				{
					if(szLine!=szKey)
						szValue = szLine.Mid(szKey.GetLength()+1);
					else
						szValue.Empty();
					//theApp.m_DataUtilMng.GetWord(aKey, aLine, '=');
					
					//{SetEnvironmentVariable(aKey, aLine);}
					{
						szKey.TrimLeft();	szKey.TrimRight();
						szValue.TrimLeft(); szValue.TrimRight();
						pEnvDB->SetAt(szKey, szValue);
					}
					nAddCnt++;
				}
			}
		}
		szLine.ReleaseBuffer();
		free(pReturnedStrng);
		
		return nAddCnt;
	}
};

#endif // !defined(AFX_FINDDATA_GRID_UTIL_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)