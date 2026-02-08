#pragma once

#include "stdAfx.h"
// CContentWnd

class CContentWnd : public CWnd
{
	DECLARE_DYNAMIC(CContentWnd)

public:
	CContentWnd();
	virtual ~CContentWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


