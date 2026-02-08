#if !defined(AFX_ACCOUNT_H__F81234A1_D6B6_11D4_B31B_0048543A0D2E__INCLUDED_)
#define AFX_ACCOUNT_H__F81234A1_D6B6_11D4_B31B_0048543A0D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Account.h : header file
//

#include <afxTempl.h>
#define		SIZEONEACCT		51  // 계좌(11) + 계좌명(40)
                                // 서버에서 내려받을 때의 패킷 

#define		SIZEFULLACCT	64  // 계좌(11) + 계좌명(40) + 비밀번호(13)
                                // 공유처리는 SIZEFULLACCT 단위로

#define		SIZETOTALACCT	66  // 계좌(11) + 계좌명(40) + 비밀번호(13) + 자타구분(1) + 비밀번호저장(1)

class CAcct
{
public:
	CString m_acctA, m_acctB, m_acctC,
			m_acctName,
			m_acctEncPass,
			m_acctFlag;			// 자, 타 구분flag('I', 'Y')
	int		m_nSaveMode;		// 저장안함(0), 영구저장(1), 임시저장(2)
	
	// 데이터가 넘어올때는 리스트박스에 나타나는 형태대로 넘어온다.
	// 코드 체계 [3(지점번호)-2(상품구분)-6(계좌번호)]
	// 공유키는 상품구분코드로 구분
	CAcct(CString szData)
	{
		int nCount = szData.GetLength();
		if(nCount >= SIZETOTALACCT)
		{
			m_acctA = szData.Left(3);
			m_acctB = szData.Mid(3,2);
			m_acctC = szData.Mid(5, 6);
			m_acctName = szData.Mid(11, 40);
			m_acctEncPass = szData.Mid(51,13);
			m_acctFlag = szData.Mid(64,1);
			m_nSaveMode = atoi(szData.Mid(65,1));
			
			m_acctName.TrimLeft();	m_acctName.TrimRight();
			m_acctEncPass.TrimLeft();	m_acctEncPass.TrimRight();
		}
	}

	// * 계좌번호 64byte를 얻어온다.
	void GetString(CString &rString)
	{
		CString strPass = m_nSaveMode > 0 ? m_acctEncPass : _T("");

		rString.Empty();
		rString.Format("%3.3s%2.2s%6.6s%-40.40s%-13.13s", 
					m_acctA, 
					m_acctB,
					m_acctC,
					m_acctName, 
					strPass);
	}

	// * 계좌번호 64byte와 저장여부 1byte를 얻어온다. 
	void GetString2(CString &rString)
	{
		CString strPass = m_nSaveMode > 0 ? m_acctEncPass : _T("");

		rString.Empty();
		rString.Format("%3.3s%2.2s%6.6s%-40.40s%-13.13s%1.1d", 
					m_acctA, 
					m_acctB,
					m_acctC,
					m_acctName, 
					strPass,
					m_nSaveMode);
	}

	void GetString3(CString &rString)
	{
		CString strPass = m_nSaveMode > 0 ? m_acctEncPass : _T("");

		rString.Empty();
		rString.Format("%3.3s%2.2s%6.6s%-40.40s%-13.13s%-1.1s%1.1d", 
					m_acctA, 
					m_acctB,
					m_acctC,
					m_acctName, 
					strPass,
					m_acctFlag,
					m_nSaveMode);
	}

	// * 순수계좌번호만 얻어온다. 
	void GetPureAcct(CString& rString)
	{
		rString.Empty();
		rString.Format("%3.3s%2.2s%6.6s",
					m_acctA,
					m_acctB,
					m_acctC);
	}
};

class CAcctHelper
{
public:
	~CAcctHelper()
	{
		ClearAll();
	}

	void ClearAll()
	{
		for(POSITION pos=m_List.GetHeadPosition(); pos; )
		{
			CAcct* pData = m_List.GetNext(pos);
			delete pData;
		}
		m_List.RemoveAll();
	}

	void Add(CString szData)
	{
		if(szData.GetLength()<SIZEONEACCT)
			return;

		CAcct* pAcct = new CAcct(szData);
		m_List.AddTail(pAcct);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 최유강(Yoo-Kang, Choi)
	// 만든 날짜          : 2003/10/30
	// 리턴 타입          : int
	// 리턴 타입 설명     : 
	// 파라미터
	//		CString _szKey     : 상품코드 두자리
	//		CString& rString   : [out] 계좌 리스트
	// 함수 설명          : 특정 상품코드의 계좌정보를 얻는 함수
	//						리스트에 있는 순서대로 스트링을 구성한다. 
	///////////////////////////////////////////////////////////////////////////////
	int GetKeyString(CString _szKey, CString& rString)
	{
		int nCount = 0;
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				if(pAcct->m_acctB.Compare(_szKey)==0)
				{
					nCount++;
					CString szTemp;
					pAcct->GetString(szTemp);
					rString += szTemp;
				}
			}
		}
		return nCount;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 최유강(Yoo-Kang, Choi)
	// 만든 날짜          : 2003/10/30
	// 리턴 타입          : int
	// 리턴 타입 설명     : 
	// 파라미터
	//		CString _szKey     : 상품코드 두자리
	//		CString rOrigin	   : 현재 공유 세팅된 계좌 리스트
	//		CString& rString   : [out] 계좌 리스트
	// 함수 설명          : 특정 상품코드의 계좌정보를 얻는 함수
	//						현재 공유 세팅되어 있는 순서대로 스트링을 구성한다.
	//						☞ 계좌비번 저장후 다시 공유 세팅하기 위해 추가한 함수
	///////////////////////////////////////////////////////////////////////////////
	int GetKeyString2(CString _szKey, CString rOrigin, CString& rString)
	{
		int nSize = SIZEFULLACCT;	// 한 계좌의 길이.
		int nCount = 0;
		while(rOrigin.GetLength() >= nSize)
		{
			CString szPureAcct = rOrigin.Mid(5, 6);		// 순수 계좌번호만 추출
			rOrigin = rOrigin.Mid(nSize);	// 한번 비교한 계좌(맨앞계좌)는 제외

			for(POSITION pos = m_List.GetHeadPosition(); pos; )
			{
				CAcct* pAcct = m_List.GetNext(pos);
				if(pAcct)
				{
					if(pAcct->m_acctB.Compare(_szKey)==0 &&
						pAcct->m_acctC.Compare(szPureAcct)==0 )
					{
						nCount++;
						CString szTemp;
						pAcct->GetString(szTemp);
						rString += szTemp;
					}
				}
			}
		}
		
		return nCount;
	}

	int GetAllString(CString& rString)
	{
		int nCount = 0;
		for(POSITION pos = m_List.GetHeadPosition(); pos; nCount++)
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				CString szTemp;
				pAcct->GetString(szTemp);
				rString += szTemp;
			}
		}
		return nCount;
	}

	// GetString2() 함수를 써서 모든 계좌정보를 얻어옴
	int GetAllString2(CString& rString)
	{
		int nCount = 0;
		for(POSITION pos = m_List.GetHeadPosition(); pos; nCount++)
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				CString szTemp;
				pAcct->GetString2(szTemp);
				rString += szTemp;
			}
		}
		return nCount;	
	}

	CString GetKey(CString szAcct)
	{
		char* aPre = "!NP_Acct";
		CString szKey = szAcct.Mid(3,2);
		szKey = aPre + szKey;
		return szKey;
	}

	// * 인자로 넘어온 계좌(szAcct)를 _szKey에 해당하는 공유값의 맨 처음 항목으로 보낸다. 
	// * 멀티 스타트 공유를 위해 추가한 사항..
	// * MainGlobalSharedData.cpp 에서 사용.. 
	// * _szKey : (!NP_Acctxx - 'xx'는 상품코드) 형식으로..
	// * szAcct : 64 Byte (계좌번호+계좌명+비밀번호)
	void MoveToHead(CString _szKey, CString szAcct, CString& rString, BOOL bAddValue)
	{
		CString szExist;			// 인자로 넘어온 계좌가 계좌 리스트에 존재하는 경우 임시 저장
		CString szOriginAcct;		// Trim() 하지 않은 계좌 원본 데이타 (뒤에서 사용하기 위해)
		int		bExist = FALSE;
		
		szOriginAcct = szAcct;
		szAcct.TrimRight();

		CString szKey = _szKey.Right(2);
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				if(pAcct->m_acctB.Compare(szKey)==0)
				{
					CString szTemp;
					pAcct->GetString(szTemp);
					//if(szAcct.Compare(szTemp)==0)	// 맨 앞으로 넣기 위해 우선 빼놓는다.
					if(szTemp.Find(szAcct)>=0)		// 계좌만 넘어온 경우를 위해 Compare대신 Find사용
					{
						bExist = TRUE;
						szExist = szTemp;
					}
					else				
						rString += szTemp;
				}
			}
		}
		
		// * 넘겨받은 계좌가 리스트에 들어 있는 경우, 
		// * 혹은 bAddValue가 True인 경우(스크립트로 main.NotifyGShareData() 호출한 경우)에는 
		// * 그 계좌를 재구성된 리스트의 맨 앞에 붙여주고
		// * 그렇지 않은 경우 재구성된 리스트만 리턴한다. 
		if(bExist || bAddValue)
			rString = szExist + rString;
		// * 넘겨받은 계좌가 리스트에 존재하지 않을 경우, 
		// * 계좌원본(64바이트)을 재구성된 리스트의 맨 앞에 붙여준다. 
		else if(!bExist)
			rString = szOriginAcct + rString;
	}

	// * 계좌번호에 해당하는 비밀번호를 넘겨준다. 
	// * szAcct : 11 Byte(계좌번호)
	CString GetAcctPasswd(CString szAcct)
	{
		CString szKey = szAcct.Mid(3,2);
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				if(pAcct->m_acctB.Compare(szKey)==0)
				{
					CString szTemp;
					if(pAcct->m_acctC.Compare(szAcct.Right(6))==0)
						return pAcct->m_acctEncPass;
				}
			}
		}
		return "";
	}

	// * 계좌번호에 해당하는 비밀번호를 세팅한다. 
	// * szAcct : 11 Byte(계좌번호)
	// * szPasswd : szAcct에 해당하는 비밀번호 
	void SetAcctPasswd(CString szAcct, CString szPasswd)
	{
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				CString szTemp;
				pAcct->GetPureAcct(szTemp);
				if(!szTemp.Compare(szAcct))
				{
					pAcct->m_acctEncPass = szPasswd;
					return;
				}
			}
		}		
	}

	void Copy(CAcctHelper* pOrig)
	{
		// 리스트 Clear
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			delete m_List.GetNext(pos);			
		}
		m_List.RemoveAll();

		// Copy.. 
		for(pos = pOrig->m_List.GetHeadPosition(); pos; )
		{
			CAcct* pOld = pOrig->m_List.GetNext(pos);
			if(pOld)
			{
				CString szAcct;
				szAcct.Format("%3.3s%2.2s%6.6s%-40.40s%-13.13s%-1.1s%1.1d", pOld->m_acctA, 
																	pOld->m_acctB, 
																	pOld->m_acctC, 
																	pOld->m_acctName,
																	pOld->m_acctEncPass,
																	pOld->m_acctFlag,
																	pOld->m_nSaveMode);
				CAcct* pAcct = new CAcct(szAcct);
				m_List.AddTail(pAcct);
			}
		}
	}

	CList<CAcct*, CAcct*> m_List;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNT_H__F81234A1_D6B6_11D4_B31B_0048543A0D2E__INCLUDED_)
