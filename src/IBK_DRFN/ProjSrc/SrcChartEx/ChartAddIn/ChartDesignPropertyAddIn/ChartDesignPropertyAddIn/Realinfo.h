// Realinfo.h: interface for the CRealinfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALINFO_H__62D5CADB_FA42_408E_B7A3_CB9C9432632D__INCLUDED_)
#define AFX_REALINFO_H__62D5CADB_FA42_408E_B7A3_CB9C9432632D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRealinfo  
{
public:
	CRealinfo(const CString &packetName, const CString &serverpacketName, const CString &fieldname, const CString &KeyName);
	virtual ~CRealinfo();

	int GetIndex();
	CString GetKeyName();
	CString GetFieldName();
	CString GetServerPacketName();
	CString GetPacketName();

	void SetIndex(const int nIndex);
	void SetKeyName(const CString& keyname);
	void SetFieldName(const CString& fieldname);
	void SetPacketName(const CString& packetname);
	void SetServerPacketName(const CString& serverpacketName);

private:
	CString m_PacketName;
	CString m_SeverPacketName;
	CString m_fieldName;
	CString m_KeyName;

	int m_nIndex;

};

#endif // !defined(AFX_REALINFO_H__62D5CADB_FA42_408E_B7A3_CB9C9432632D__INCLUDED_)
