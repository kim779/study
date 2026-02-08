// DataOfPacketPage.h: interface for the CDataOfPacketPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAOFPACKETPAGE_H__97299F42_6833_48F9_898C_D7720ED21926__INCLUDED_)
#define AFX_DATAOFPACKETPAGE_H__97299F42_6833_48F9_898C_D7720ED21926__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPacketSuperGrid;
class CDataOfPacketPage
{
public:
	void Init(const CString& strReceiveDataFromOcx, CPacketSuperGrid* PacketList);
	CDataOfPacketPage();
	virtual ~CDataOfPacketPage();

	CString GetUseRealPacketItems(const CString& strSavedData);
	//get all data of listctrl. save to m_strInfoOfCheckPacketItems.
	CString GetAllDataFromListCtrl(CPacketSuperGrid* PacketList);

//	bool DoMustRealPageSetting(CListCtrl* pList);
private:
	CString m_strOldRealPacketCount;
	CString m_strRealPacketCount;

	//set received data From ocx
	void AddData(const int nCheck, const CString& szpacket, const CString& szField, const CString& szLength, const CString& szType, const CString& szReal, CPacketSuperGrid *PacketList); //ocx에서 받은 속성값 적용하기 위함..

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

#endif // !defined(AFX_DATAOFPACKETPAGE_H__97299F42_6833_48F9_898C_D7720ED21926__INCLUDED_)
