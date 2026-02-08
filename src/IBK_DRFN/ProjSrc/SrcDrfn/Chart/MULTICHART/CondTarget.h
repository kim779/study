// CondTarget.h: interface for the CCondTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONDTARGET_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_CONDTARGET_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

class CGetBackData
{
public:
	CGetBackData(CString strPath, int nIndex)		{ m_strPath = strPath; m_nIndex = nIndex;	};
	virtual ~CGetBackData()	{};

	CString		m_strPath;
	int			m_nIndex;
};



#define		SZ_MONTHARRAY	12

class CCondTarget  
{
public:
	CCondTarget();
	virtual ~CCondTarget();

	void				RemoveAll();

	CString				m_strUserDir;
	CString				m_strRootDir;

	CString				m_strKRX100Code, m_strKSP200Code, m_strKSPCode, m_strKSQCode;
	CStringArray		m_arrKSPName, m_arrKSPCode, m_arrKSQName, m_arrKSQCode;
	IMasterDataManagerLast*	m_pManager;

	CStringArray		m_arTargetResult;				// 대상변경 설정 적용된 코드 리스트.

	// 대상변경 설정 값.

	// 마스타 데이터 분석값.
	CStringArray		m_saMonth[SZ_MONTHARRAY];		// 결산월별 arr[0] ~ arr[11] 종목코드 리스트.
	CMapStringToString	m_mapExeptItem;					// 종목별 제외종목 정보.


	void				ReceiveData(LPVOID aTRData, long dwTRDateLen);
	void				SetEditSubjectTarGetBit();

	int		m_nKey;
	CMap<int, int, CGetBackData*, CGetBackData*>		m_mapGetBackData;
	CGetBackData*		SetGetBackData(CString strPath);
	void				RemoveAtGetBackData(int nKey);
	void				LoadTdfData(CGetBackData *pData);

	int					GetProfileInt(CString strGKey, CString strKey, CString strSavePath);

	BOOL				GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey);
	void				GetItemCodeFromThemeGroup(CStringArray &arrCode, CString strThemeIndex);
	CString				Parser(CString &sSrc, CString sDelimiter);
	BOOL				GetStockMasterData(CStringArray& saCode, CStringArray& saName);
	CString				GetMarketCode(CString strType, CString strName);
	void				DelSpaceInName();
	BOOL				CheckTargetResult(CString strCode);


	BOOL	m_bIssuesForAdministration;
	BOOL	m_bDealStop;
	BOOL	m_bTidyDeal;
	BOOL	m_bInsincerityNotice;
	BOOL	m_bAttentionInvestment;
	BOOL	m_bNoticeAttentionInvestment;
	BOOL	m_bPreferenceStock;
	BOOL	m_bDeposit;
	BOOL	m_bDangerInvestment;
	BOOL	m_bWarningInvestment;
	BOOL	m_bInvestAttention;
	BOOL	m_bETF;
	int		m_nSelRadioMonth;
	int		m_nIndexMonthCombo;
	CString	m_strEditSubjectSelectItemText;
	CString	m_strEditSubjectSelectParentItemText;
	CString	m_strJangGubun;

};

#endif // !defined(AFX_CONDTARGET_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
