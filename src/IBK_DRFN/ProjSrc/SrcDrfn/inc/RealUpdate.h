#if !defined(AFX__REALUPDATE__INCLUDED_)
#define AFX__REALUPDATE__INCLUDED_
// 벡트 테이블 정의 
#include <vector>
#include <algorithm>
// FIDLIST는 벡터 테이블을 이용한다.
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

// 화면에게 실시간데이터(PB)처리를 하도록하는 메세지. 
//#define WM_GET_BROD				WM_USER + 605		//@유진 601 -> 605 : WM_USER + 601

#define	STOCK_SISE_PID			21	//50		// 주식 실시간 시세 데이터
//#define	STOCK_HOGA_PID			3	//51		// 주식 실시간 종목호가 데이터
#define	UPJONG_SISE_PID			101				// 업종 실시간 시세 데이터
#define	FUTURE_SISE_PID			65	//65		// 선물 실시간 시세 데이터
#define	OPTION_SISE_PID			66	//67		// 옵션 실시간 시세 데이터
#define	STAR_SISE_PID			67				// 스타 실시간 시세 데이터
#define FOREIGN_SISE_PID		109
#define	ELW_SISE_PID			138				// ELW 실시간 시세 데이터
#define FX_SISE_PID				140				// FX 실시간 시세 데이터

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
#define DI_OPENPRICE		15009	//시가
#define DI_OFFER_HO1		14501	//매도호가1
#define DI_OFFER_REM1		14511	//매도호가잔량1
#define DI_BID_HO1			14531	//매수호가1
#define DI_BID_REM1			14541	//매수호가잔량1
#define DI_OFFER_TOT5		14621	//매도5단계호가잔량합
#define DI_BID_TOT5			14623	//매수5단계호가잔량합
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
	void			SetMainKey(WORD nFid, CString strVal){m_nMainKeyFid = nFid; m_strMainKeyVal = strVal; }
};

// 화면에게 실시간데이터(PB)처리를 하도록하는 메세지. 
// #define WM_GET_BROD				WM_USER + 601

#endif // AFX__REALUPDATE__INCLUDED_
