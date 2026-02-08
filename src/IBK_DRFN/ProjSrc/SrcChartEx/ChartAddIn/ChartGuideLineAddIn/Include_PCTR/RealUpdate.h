#if !defined(AFX__REALUPDATE__INCLUDED_)
#define AFX__REALUPDATE__INCLUDED_

#define DI_CODE				16013	//코드
#define DI_NAME				16002
#define DI_TIME				15019	//체결시간
#define DI_PRICE			15001	//현재가
#define DI_CHANGE			15472	//대비
#define DI_SIGN				15006	//등락부호
#define DI_CHGRATE			15004	//대비율
#define DI_HIGH				15010	//고가
#define DI_LOW				15011	//저가
#define DI_TRDVOL			15020	//체결량
#define DI_VOLUME			15015	//누적거래량
#define DI_AMOUNT			15023	//누적거래대금
#define DI_LAST_PRICE		15007	//전일종가
#define DI_OPENPRICE		15009	//시가
#define DI_OFFER_HO1		14501	//매도호가1	
#define DI_OFFER_REM1		14511	//매도호가잔량1
#define DI_BID_HO1			14531	//매수호가1
#define DI_BID_REM1			14541	//매수호가잔량1
#define DI_OFFER_TOTREM		14561	//매도호가잔량합
#define DI_BID_TOTREM		14563	//매수호가잔량합
#define DI_OFFER_OUT_TOT	14575	//시간외매도호가잔량
#define DI_BID_OUT_TOT		14577	//시간외매수호가


class CRealUpdateData
{
public:
	CRealUpdateData()
	{
		m_pFidList = NULL;
		m_pFidPtrValue = NULL;
		m_pFidTypeList = NULL;
	}
	~CRealUpdateData()
	{
	}

	BOOL GetDataString(WORD wID, CString &strData)
	{
		if(!m_pFidList)
			return FALSE;

		void* pData;
		WORD nType;
		for(int i=0; i<m_pFidList->GetSize(); i++)
		{
			if(wID == m_pFidList->GetAt(i))
			{
				pData = m_pFidPtrValue->GetAt(i);
				nType = m_pFidTypeList->GetAt(i);
				strData = ConvertToString(nType, (char *)pData);
				return TRUE;
			}
		}
		return FALSE;
	}

	CString ConvertToString(WORD wDataType, char *pBuf, int nLen=0)
	{
		CString strData="";

		int iIndex = 0;
		double fData;
		int nTemp;
		switch(wDataType)
		{
		case 1:
			if(nLen > 0)
				strData.Format("%*.*s", nLen, nLen, pBuf);
			else
				strData.Format("%s", pBuf);
			break;
		case 2:
			strData.Format("%d", pBuf[0]);
			break;
		case 3:
		case 4:
			if(nLen > 0)
				strData.Format("%*d", nLen, *(short *)pBuf);
			else
				strData.Format("%d", *(short *)pBuf);
			break;
		case 5:
			if(nLen > 0)
				strData.Format("%*d", nLen, *(long *)pBuf);
			else
				strData.Format("%d", *(long *)pBuf);
			break;
		case 6:
			if(nLen > 0)
				strData.Format("%*u", nLen, *(ULONG *)pBuf);
			else
				strData.Format("%u", *(ULONG *)pBuf);
			break;
		case 7:
			if(nLen > 0)
				strData.Format("%*u", nLen, *(ULONG *)pBuf);
			else
				strData.Format("%u", *(ULONG *)pBuf);
			nTemp = atoi(strData);
			fData = nTemp/100.;
			strData.Format("%*.2f", nLen, fData);
			break;
			//daetoo
		case 8:
		case 9:
			if(nLen > 0)
				strData.Format("%*ld", nLen, *(long *)pBuf);
			else
				strData.Format("%ld", *(long *)pBuf);
			break;
		default:
			break;
		}
		return strData;
	}

	CWordArray*		m_pFidList;				///> FID List
	CWordArray*		m_pFidTypeList;				///> FID Type List
	CPtrArray*		m_pFidPtrValue;				///> Value의 Pointer위치
	WORD			m_nMainKeyFid;
	CString			m_strMainKeyVal;
	WORD			m_nQAIndex;
	void			SetMainKey(WORD nFid, CString strVal){m_nMainKeyFid = nFid; m_strMainKeyVal = strVal; }

	// (2008/9/12 - Seung-Won, Bae) for GuideLine Interface
	const char *GetDataString2( WORD wID)
	{
		static CString strResult;
		if( !GetDataString( wID, strResult)) strResult.Empty();
		return strResult;
	}
};

#endif // AFX__REALUPDATE__INCLUDED_
