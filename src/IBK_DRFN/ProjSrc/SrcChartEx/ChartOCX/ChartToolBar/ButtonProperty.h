// ButtonProperty.h: interface for the CButtonProperty class.
// 툴바에서 사용하는 버튼들의 State설정에 관한 정책을 규정한 클래스이다.
// 즉, 특정 버튼을 눌렀을때 실행되고 있는 다른 툴버튼을 해제할것인지를 규정하는 클래스.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUTTONPROPERTY_H__C62A5FC8_2FEF_417D_988B_B9440F30CD1B__INCLUDED_)
#define AFX_BUTTONPROPERTY_H__C62A5FC8_2FEF_417D_988B_B9440F30CD1B__INCLUDED_

#include <AFXTEMPL.H>
#include "ToolOption.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CButtonEx;
class CToolBarDlg;

typedef CList<CButtonEx*, CButtonEx*> CButtonList; // ToolBarDlg.h에도 정의되어 있음.

class CButtonProperty  
{
public:
	CButtonProperty(CWnd* pParent, CButtonList* pButtonList);
	virtual ~CButtonProperty();
	int ReleaseOtherButtonUI( CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam = _T(""));	// 눌려진 버튼 풀기 시작점.
private:
	void ReleaseCaseZoom(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam);
	void ReleaseCaseSpecialChart(CToolOptionInfo::TOOLOPTION enumToolOption);
	void ReleaseCaseDefault(CToolOptionInfo::TOOLOPTION enumToolOption);
	void ReleaseCaseCrossLine();
	
	void ReleaseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);

	bool IsModalessDialog(CToolOptionInfo::TOOLOPTION enumToolOption);

private:
	CButtonList* m_pButtonList;
	CToolBarDlg* m_pParent;
};

#endif // !defined(AFX_BUTTONPROPERTY_H__C62A5FC8_2FEF_417D_988B_B9440F30CD1B__INCLUDED_)
