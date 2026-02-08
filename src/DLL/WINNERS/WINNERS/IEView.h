#pragma once
#include <afxhtml.h>
class IEView : public CHtmlView
{
public:
	IEView(void);
	~IEView(void);

public:
	virtual void OnInitialUpdate(CString strurl);
};

