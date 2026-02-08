
/************************************************/
/*																		          */
/*							Line Chart Control							*/
/*                                              */
/*																		          */
/*							累己老 : 2006. 07 ~							*/
/*						累己磊 : 快悼快 (Doori co.)		    */
/*																		          */
/************************************************/

typedef struct tagXData
{
  CString strXTitle;
  double dXData;
} XData;

typedef struct tagSeries
{
  BOOL    bView;
  int     nSeriesID;

  CString   strLengend;
  COLORREF	clrLine;

  CString strDataType;    //# int, double
  CString strMask;
  CUIntArray *psiYIndex;
  CStringArray *psaYData;
} Series;

class CDrLnChartCtrl : public CStatic
{
public:
	CDrLnChartCtrl();
	~CDrLnChartCtrl();

  int  m_nXMask_OffSet;
  int  m_nLegend_OffSet;

  double m_dXOffset, m_dYOffset;

private:
  double m_dYMax, m_dYMin;
  CList <Series, Series&> m_listSeries;

  CList <XData, XData&> m_listXData;

  CPoint m_ptBase;
  CRect m_reCanvas;
  COLORREF m_clrCanvas;

  CString m_strTitle;
  CRect m_reTitle;

  CRect m_reLegend;
  
  int m_nTitleInterval_Y, m_nTitleInterval_X;


  int m_nZeroPos;

public:
	void CopyToClipboard();

  void SetInit();
  
  BOOL SetAdd_Series(int nSeriesID, CString strLengend, COLORREF clrLine);
  BOOL SetModify_Series(int nSeriesID, CString strLengend);
  BOOL SetSeriesShow(int nSeriesID, BOOL bShow);
  
  BOOL SetDelete_Series(int nSeriesID);

  void SetClear_XData();
  BOOL SetClear_YData(int nSeriesID);

  POSITION GetSeries(int nSeriesID, Series &stSeries);

  void SetAdd_XData(CString strXTitle, double dXData);

  BOOL SetAdd_YData(int nSeriesID, int nDataIndex, double dYData);
  void SetMaxMin_YVal(double dYData);
  void SetMaxMin_YVal();

  void SetClearAll();
  void SetDeleteAll();

  void SetDraw_Chart(HDC hDC = NULL);
  void SetDraw_Chart(HDC hDC, CRect reClient);

  void Draw3dRect(HDC hDC, int x, int y, int cx, int cy, int nLH,
					COLORREF clrTopLeft, COLORREF clrBottomRight);
  void FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clr);

  void SetDrawBox(HDC hDC, int x, int y, int cx, int cy, COLORREF clr=0xFF, int nWidth = 1);

  void SetDrawYGrid(HDC hDC);
  void SetDrawXGrid(HDC hDC);
  void SetDrawLine(HDC hDC);
  
  void SetDrawTitle(HDC hDC);
  void SetDrawLegend(HDC hDC);
  
  void SetDrawCanvers(HDC hDC, CRect reClient, COLORREF crColor = RGB(0, 0, 1));

  HFONT SetMakeFont(CString strFontName, int nFontSize, BOOL bRotate=FALSE);
  

protected:
	//{{AFX_MSG(CDrLnChartCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//#endif //__LINECHART_INCLUDE__