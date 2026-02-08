
#pragma once

class CNoCont  
{
public:
	CNoCont();
	virtual ~CNoCont();
public:
	void	Clear();
	int	GetNoCont(CString sJCode, CString sMemeGubn);
	void	SetNoCont(CString sJCode, CString sMemeGubn, int nAmount);
	void	ParsingNotice(CString sJCode, CString sMemeGubn, CString sStatus, int nAmount);
protected:
private:
	CMapStringToPtr	m_NoContMap;

};

