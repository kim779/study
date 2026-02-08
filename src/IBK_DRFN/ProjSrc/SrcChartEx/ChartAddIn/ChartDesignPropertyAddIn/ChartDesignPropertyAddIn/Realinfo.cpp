// Realinfo.cpp: implementation of the CRealinfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "Realinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRealinfo::CRealinfo(const CString &packetName, const CString &serverpacketName, const CString &fieldname, const CString &KeyName)
{
	SetServerPacketName(serverpacketName);
	SetPacketName(packetName);
	SetFieldName(fieldname);
	SetKeyName(KeyName);
//	SetIndex(nIndex);
}

CRealinfo::~CRealinfo()
{

}

CString CRealinfo::GetPacketName()
{
	return m_PacketName;
}

CString CRealinfo::GetServerPacketName()
{
	return m_SeverPacketName;
}

CString CRealinfo::GetFieldName()
{
	return m_fieldName;
}

CString CRealinfo::GetKeyName()
{
	return m_KeyName;
}

void CRealinfo::SetServerPacketName(const CString &serverpacketName)
{
	m_SeverPacketName = serverpacketName;
}

void CRealinfo::SetPacketName(const CString &packetname)
{
	m_PacketName = packetname;
}

void CRealinfo::SetFieldName(const CString &fieldname)
{
	m_fieldName = fieldname;
}

void CRealinfo::SetKeyName(const CString &keyname)
{
	m_KeyName = keyname;
}

void CRealinfo::SetIndex(const int nIndex)
{
	m_nIndex = nIndex;
}


int CRealinfo::GetIndex()
{
	return m_nIndex;
}