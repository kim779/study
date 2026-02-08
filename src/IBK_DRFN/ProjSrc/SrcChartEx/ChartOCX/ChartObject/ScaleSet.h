// ScaleSet.h: interface for the CScaleSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALESET_H__0CB2B045_ABF1_49AB_B2C9_ACF15F6924D9__INCLUDED_)
#define AFX_SCALESET_H__0CB2B045_ABF1_49AB_B2C9_ACF15F6924D9__INCLUDED_

#include "ColorButton.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColorButton;

class CScaleSet : public CDialog
{
public:
	CScaleSet(UINT nIDTemplate, CWnd* pParent);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnApplyBt();
	afx_msg void OnAllscalegroupCheck();
	afx_msg void OnSelchangeScalegroup();
	afx_msg void OnGridColor();
	afx_msg void OnTextColor();

protected:
	virtual void InitializeData() = 0;
	virtual void ApplyInEtcData() = 0;
	virtual void SetAllScale() = 0;
	virtual void SetScale(const int nIndex) = 0;
	virtual void SetScaleColor(const COLORREF& gridColor, const COLORREF& textColor) = 0;

	// 화면 변경
	virtual void ChangeScaleWindow(const int nGroupIndex) = 0;
	virtual void EnableWindowOnAllScaleGroup();

protected:
	void SetAllScaleGroup(const BOOL bAllScaleGroup);
	void SetScalePosition(const int nScalePosition);
	void SetScaleColorInButton(const COLORREF& gridColor, const COLORREF& textColor);

	CWnd* GetParent();
	BOOL IsAllScaleGroup() const;
	int GetScalePosition() const;
	int GetScaleGrid() const;

	void ChangeScaleWindow(const int nGroup, const int nPosition, const int nGrid);
	void EnableWindow(CWnd* pItem);
	void EnableWindow(CWnd* pItem, const bool bEnable);

private:
	CWnd* m_pParent;	
	BOOL m_bAllScaleGroup;
	int	m_nScaleGroup;
	int	m_nScalePosition;
	int	m_nScaleGrid;
	CColorButton	m_ctlGridColor;
	CColorButton	m_ctlTextColor;

	BOOL ApplyInAllData();
	BOOL SetScale();
	void SetScaleColorInButton(CColorButton* pColorButton, const COLORREF& color);

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND				m_hOcxWnd;
};

#endif // !defined(AFX_SCALESET_H__0CB2B045_ABF1_49AB_B2C9_ACF15F6924D9__INCLUDED_)
