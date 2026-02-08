#pragma once

#include "Remain.h"
#include "RemainFuture.h"
#include "../../h/ledger.h"
#include "../../h/Grid.h"
#include <afxmt.h>
#include "ShMemory.h"

//==============================================================================
class CAccn
{
public:
  bool      m_bFuture;
  CString   m_accn;
  CString   m_pswd;
  CString   m_errmsg;
  bool      m_bCredit;        // true:신용포함, false:신용제외
  int       m_dFee;           // 수수료제세금 적용 1 미적용 0
  double    m_dMass;          // 매체구분 0:hts 1:영업점/콜 2.ARS
  double    m_dSave;          // 매체구분 0:hts 1:영업점/콜 2.ARS
  int       m_dCalcType;      // 0:융자금제외, 1:융자금포함
  int		m_iMcgb;		  // 2015.04.22 KSJ 매체구분 0:HTS/MTS/홈페이지, 1:영업점/콜센터, 2:ARS
  bool		m_bReceive;			//2015.03.04 KSJ true:Recevie받음, false:Recevie받지 않음
  CMapStringToPtr m_CodeMap;  // Ptr: F->class CRemainFuture, C->class CRemain

public:
  CAccn()
  {
    m_bFuture = false;
    m_bCredit = false;
    m_accn = "";
    m_pswd = m_errmsg = "";
    m_dFee = 0; //수수료제세금 적용 1 미적용 0
    m_dMass = 0;
    m_dSave = 0;
	m_dCalcType = 0;
	m_iMcgb = 0;

    m_CodeMap.RemoveAll();
  }
};
//==============================================================================
class CMapWnd : public CWnd
{
public:
  CMapWnd();
  virtual ~CMapWnd();

  //{{AFX_VIRTUAL(CMapWnd)
  //}}AFX_VIRTUAL

protected:
  //{{AFX_MSG(CMapWnd)
  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnDestroy();
  //}}AFX_MSG
  afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnRequestOfCtrl(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnRequestLawCheg(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnCloseOfCtrl(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnReceiveRemainData(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

public:
	BOOL  CreateMap(CWnd *pParent);

private:
	void  sendRemainTR(CString sAccn, CString sPswd, bool bFuture, bool bCredit, int dFee, double dMass ,double dSave, int dCalcType, int iMcgb,HWND sHwnd);	//2015.04.22 KSJ 매체구분 추가
	void  sendLawCheg(CString acc_gb, CString mng_no, CString mk_cd, CString grp_no, HWND sHwnd);
	CString parsingRemainData(char* pData, int len, CString sErrmsg, int trkey);
	void  sendToControl(CString sAccn);
	void  sendToControl(CString sAccn, CAccn *pAccn);
	void  attachCB(HWND hWnd, CString accn);
	void  detachCB(CString accn, CString sData, CString emsg = _T(""));

	void  sendNContTR(CString sLedger = "");
	void  parsingNContData(char* pData);

	void  parsingRTS(CString sRtsData);
	void  parsingRTSx(struct _alertR* alertR);
	CString parsingNotice(CString str);

	BOOL  getRemainFee();
	CString getKey(bool bFuture, CString codeS, CString dateS, CString sygbS, CString sJggb, int trkey=-1);

	CString ledgerTR(CString sTran, CString sSvcn, CString sGubn);
	int openView(int type, CString data);

	CString parser(CString &srcstr, CString substr);

	CString getTime();
	CString variant(int comm, CString data);	
	void  removeAccn(CString accn);
	void  removeRemain(CAccn *pAccn);

public:
	CMapStringToPtr m_hname;
	BOOL LoadCode();
	bool Set3RDCode();					//2014.10.06 KSJ K-OTC 종목 세팅
	bool Is3RDCode(CString sCode);		//2014.10.06 KSJ K-OTC 종목 찾기
	CPtrArray m_3RDcode;				//2014.10.06 KSJ K-OTC 종목코드
	CPtrArray m_hcode;

	CString GetRoot();
	CString GetUser();
	CString GetName();
	BOOL  sendTR(CString sTR, CString sData, int nKey, int nStat, CString Acnt);

	CString GetHandleByKey(CString key);
	int GetNewAccHandleKey();
	int GetNewLawHandleKey();
	CWnd  *m_pParent;

private:
	std::unique_ptr<char[]> m_hcB;
	void RemoveCB(CString accn);
	HWND GetRightHwnd(CString key);	
	void SendLawNotice(CString sData);
	CString GetEncPassword(CString sPswd);	//2014.09.18 KSJ 일방향암호화

	HINSTANCE	m_hiSha256;	//2014.09.18 KSJ 일방향암호화

  int m_hcnt;
  std::unique_ptr<class CShMemory> m_ShMemory;
  CCriticalSection  m_sync;
  char      m_system;   // 대투, 하나 구분

  CMapStringToPtr   m_AccnMap;    // Ptr : class CAccn

  CMapStringToString  m_multicb;
  CMapStringToString  m_AccnKey;    // 계좌별 키관리
  CMapStringToString  m_LawKey;   // 법인영업 키관리
  CStringArray    m_arKey;

  CString     m_sCurrAccn;    // 조회중인 accn
  CString     m_sCurrPswd;    // 조회중인 pswd
  CString     m_sCurrKey;   // 조회중인 key
  bool      m_bAllAccn;   // 복수계좌조회

  std::vector<std::unique_ptr<CAccn>> _vList;
};

