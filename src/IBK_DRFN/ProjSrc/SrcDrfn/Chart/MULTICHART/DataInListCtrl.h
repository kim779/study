class CDataInListCtrl
{
public:
	CDataInListCtrl() 
	{

	}
	const CDataInListCtrl& CDataInListCtrl::operator=(const CDataInListCtrl &ci)
	{
		m_strJongCode = ci.m_strJongCode;
		m_strCodeName = ci.m_strCodeName;
		m_strTemp	= ci.m_strTemp;
		m_strTemp2	= ci.m_strTemp2;
		return *this;
	}
	CString m_strJongCode;
	CString m_strCodeName;
	CString m_strTemp;
	CString m_strTemp2;
};