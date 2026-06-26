#pragma once

#include "CCommonWnd.h"
// CMKPopWnd

class CMKPopWnd : public CWnd, public CCommonWnd
{
	DECLARE_DYNAMIC(CMKPopWnd)

public:
	CMKPopWnd(CWnd* pParent);
	virtual ~CMKPopWnd();

protected:
	DECLARE_MESSAGE_MAP()
};


