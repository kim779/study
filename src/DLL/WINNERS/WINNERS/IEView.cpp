#include "pch.h"
#include "IEView.h"

IEView::IEView(void)
{
}

IEView::~IEView(void)
{
}


void IEView::OnInitialUpdate(CString strurl)
{
	Navigate2(strurl, NULL, NULL);
}