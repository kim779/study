#pragma once


// CMaketPickerWnd

class CMaketPickerWnd : public CWnd
{
	DECLARE_DYNAMIC(CMaketPickerWnd)

public:
	CMaketPickerWnd();
	virtual ~CMaketPickerWnd();

	CString m_slog{};
	CString m_sRoot{}, m_sImagePath{};;

	CWnd* m_pWizard{};
	CWnd* m_pContainerWnd{};   //DLL 에서 cx_marketpicker를 사용할때 
	COLORREF m_clrBack{};
	
	enum { TYPE_TOTAL = 0, TYPE_KRX, TYPE_NXT };
	int m_markettype{};
	int m_mapKey{};
	int m_sizetype{};

	void SetMarketType(int itype);
	void SetMarketType(CString stype);
	CString Parser(CString& srcstr, CString substr);
	CString Variant(int comm, CString data);
	CBitmap* getBitmap(CString path);
	COLORREF GetIndexColor(int index);
	CSize BitmapSize(HBITMAP hBitmap);

	CString m_strMarket{};   //통합, KRX, NXT
	CString m_sID{};			   //대표화면 번호 (빌더에서 지정)
	HBITMAP		m_hIBitmap, m_hKBitmap, m_hNBitmap;
	enum { TYPE_SMALL = 1, TYPE_NORMAL };
	void Init();
	void SetImages();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


