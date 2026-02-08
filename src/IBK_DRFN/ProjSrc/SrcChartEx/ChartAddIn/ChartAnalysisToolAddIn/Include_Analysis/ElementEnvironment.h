// ElementEnvironment.h: interface for the CElementEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTENVIRONMENT_H__60C3FE74_3EC4_45A6_8D12_AFC3E9745073__INCLUDED_)
#define AFX_ELEMENTENVIRONMENT_H__60C3FE74_3EC4_45A6_8D12_AFC3E9745073__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"
#include "ToolBaseData.h"
#include "BlockBaseEnum2.h"		// for CAnalysis

class CPenEnvironment
{
public:
	CPenEnvironment& operator=(const CPenEnvironment& environment);

public:
	CPenEnvironment();
	CPenEnvironment(const COLORREF& color, const int nWidth, const CToolEnumData::PenStyle eStyle);
	CPenEnvironment(CPenEnvironment* pEnvironment);

	void CreatePen(const bool bIsElement, CPen& pen);
	void Set(const CPenEnvironment& environment);
	void SetColor(const COLORREF& color);
	void SetWidth(const int nWidth);
	void SetStyle(const CToolEnumData::PenStyle eStyle);

	COLORREF GetColor() const;
	int GetWidth() const;
	CToolEnumData::PenStyle GetStyle() const;
	CString GetSavedData() const;

private:
	COLORREF m_color;
	int m_nWidth;
	CToolEnumData::PenStyle m_eStyle;

	int GetStyle_Draw() const;
	CString GetSavedColor() const;
	CString GetSavedWidth() const;
	CString GetSavedStyle() const;
};

class CDataTypeEnvironment
{
public:
	CDataTypeEnvironment& operator=(const CDataTypeEnvironment& environment);

public:
	CDataTypeEnvironment();
	CDataTypeEnvironment(const CAnalysis::COORDINATETYPE eCoordinateType, const double& dVert, const CString& strHorz, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
	CDataTypeEnvironment(const CAnalysis::COORDINATETYPE eCoordinateType, const double& dVert, const CString& strHorz, const CString& strExtData, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);// 2008.08.20 김진순 피보나치 확장
	CDataTypeEnvironment(const CAnalysis::COORDINATETYPE eCoordinateType, const double& dVert, const CString& strHorz, const CString& strExtData, const CString& strStrVert, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);// 2008.08.20 김진순 피보나치 확장
	CDataTypeEnvironment(const CDataTypeEnvironment* pEnvironment);

	void Reset_AbsolutePoint();
	void SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType);
	void SetVert(const double& dDataType);
	void SetHorz(const CString& strDataType);
	void SetExtData(const CString& strExtData);
	void SetStrVert(const CString& strDataType);

	bool IsRelativePointType() const;
	bool IsDateType() const;
	CAnalysis::COORDINATETYPE GetCoordinateType() const;
	double GetVert() const;
	CString GetHorz() const;
	CString GetDateCompart() const;
	CString GetTimeCompart() const;
	CString GetExtData() const;
	CString GetStrVert() const;
	CString GetSavedData() const;

private:
	CAnalysis::COORDINATETYPE m_eCoordinateType;
	double m_dVert;		// ex) 1, 0.01
	CString m_strHorz;	// ex) YYYYMMDD or x0.01
	CString m_strExtData;		// Tool 별로 필요한 확장 데이타를 저장하기 위한 변수	// 2008.08.20 김진순 피보나치 확장
	CString m_strVert;

	CString GetSavedCoordinateType() const;
	CString GetSavedVert() const;
	CString GetSavedHorz() const;
	CString GetSavedExtData() const;
	CString GetSavedStrVert() const;

// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;
public:
	HWND	GetOcxHwnd( void) const		{	return m_hOcxWnd;	}

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
public:
	CChartInfo::CHART_MODE	GetChartMode( void) const		{	return m_eChartMode;	}
};

class CLineExtEnvironment
{
public:
	CLineExtEnvironment& operator=(const CLineExtEnvironment& environment);

public:
	CLineExtEnvironment();
	CLineExtEnvironment(const int& nLeft,const int& nRight);		//-1:NotExist,1:Exist&Check,0:Exist&UnCheck
	CLineExtEnvironment(const CLineExtEnvironment& environment);
	CLineExtEnvironment(CLineExtEnvironment* pEnvironment);

	void Set(const CLineExtEnvironment& environment);
	void SetLineExt(const int& nLeft,const int& nRight);

	int GetLeftExt() const;
	int GetRightExt() const;
	CString GetSavedData() const;

private:
	CString GetSavedLineExt() const;

private:
	int m_nLeftExt;
	int m_nRightExt;
};

class CElementCommonEnvironment
{
public:
	CElementCommonEnvironment& operator=(const CElementCommonEnvironment& environment);
public:
	CElementCommonEnvironment();
	CElementCommonEnvironment(	const bool bIsAutoPrice,
								const int& nEquiDiv,
								const int& nPriceRangeDiv,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
								const CPenEnvironment& pen, 
								const CDataTypeEnvironment& dataType,const CLineExtEnvironment& lineExt);

	void SetAutoPrice(const bool bIsAutoPrice);
	bool IsAutoPrice() const;
	
	int  GetEquiDiv() const;
	int& GetEquiDiv();
	void SetEquiDiv(int nEquiDiv);

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	int  GetPriceRangeDiv() const;
	int& GetPriceRangeDiv();
	void SetPriceRangeDiv(int nPriceRangeDiv);

	BOOL IsLeftLineExt() const;
	BOOL IsRightLineExt() const;
	void SetLineExt(int nLeftExt,int nRightExt);
	CLineExtEnvironment GetLineExt() const;
	CLineExtEnvironment& GetLineExt();

	CPenEnvironment GetPen() const;
	CDataTypeEnvironment GetDataType() const;
	CPenEnvironment& GetPen();
	CDataTypeEnvironment& GetDataType();
	CString GetSavedData() const;

private:
	bool m_bIsAutoPrice;	// y축 값따라가기.
	int m_nEquiDiv;
	int m_nPriceRangeDiv;			// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	CLineExtEnvironment m_lineExt;
	CPenEnvironment m_pen;
	CDataTypeEnvironment m_dataType;

	CString GetSaveData_AutoPrice() const;
	CString GetSaveData_EquiDiv() const;
	CString GetSaveData_PriceRangeDiv() const;		// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
};

// ----------------------------------------------------------------------------
class CTextEnvironment
{
public:
	CTextEnvironment& operator=(const CTextEnvironment& environment);

public:
	CTextEnvironment();
	CTextEnvironment(const COLORREF& color);
	CTextEnvironment(const CTextEnvironment& environment);
	CTextEnvironment(CTextEnvironment* pEnvironment);

	void Set(const CTextEnvironment& environment);
	void SetColor(const COLORREF& color);

	COLORREF GetColor() const;
	CString GetSavedData() const;

private:
	CString GetSavedColor() const;

private:
	COLORREF m_color;
};

class CTextPosition
{
public:
	CTextPosition& operator=(const CTextPosition& environment);

public:
	CTextPosition();
	CTextPosition(const CTextEnvironment& textEnvironment);

	virtual void Set(const CTextPosition* pTextPosition);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

	CTextEnvironment GetTextEnvironment() const;
	CTextEnvironment& GetTextEnvironment();

	CString GetSavedData() const;

protected:
	virtual CString GetSavedRealTextPositionData() const = 0;

	CString GetDataOfOneLine(const bool bData, const CString& strData) const;

private:
	CString GetSavedData_TextPosition() const;

private:
	CTextEnvironment m_textEnvironment;
};

class CTextShowPosition : public CTextPosition
{
public:
	CTextShowPosition();
	CTextShowPosition(const CTextEnvironment& environment, const bool bShow);
	CTextShowPosition(const CTextShowPosition& position);

	virtual void Set(const CTextPosition* pTextPosition);
	void SetShow(const bool bShow);

	bool IsShow() const;

protected:
	virtual CString GetSavedRealTextPositionData() const;

private:
	bool m_bShow;
};

class CTextHorzPosition : public CTextPosition
{
public:
	CTextHorzPosition();
	CTextHorzPosition(const CTextEnvironment& environment, const bool bLeft, const bool bRight, const bool bRightRate = TRUE);
	CTextHorzPosition(const CTextHorzPosition& position);

	virtual void Set(const CTextPosition* pTextPosition);
	void SetLeft(const bool bLeft);
	void SetRight(const bool bRight);
	void SetRightRate(const bool bValue);

	bool GetLeft() const;
	bool GetRight() const;
	bool GetRightRate() const;

protected:
	virtual CString GetSavedRealTextPositionData() const;

private:
	bool m_bLeft;
	bool m_bRight;
	bool m_bRightRate;
};

class CTextVertPosition : public CTextPosition
{
public:
	CTextVertPosition();
	CTextVertPosition(const CTextEnvironment& environment, const bool bTop, const bool bBottom);
	CTextVertPosition(const CTextVertPosition& position);

	virtual void Set(const CTextPosition* pTextPosition);
	void SetTop(const bool bTop);
	void SetBottom(const bool bBottom);

	bool GetTop() const;
	bool GetBottom() const;

protected:
	virtual CString GetSavedRealTextPositionData() const;

private:
	bool m_bTop;
	bool m_bBottom;
};

class CTextHorzVertPosition : public CTextPosition
{
public:
	CTextHorzVertPosition& operator=(const CTextHorzVertPosition& position);

public:
	CTextHorzVertPosition();
	CTextHorzVertPosition(const CTextEnvironment& environment, 
		const bool bLeft, const bool bRight, const bool bTop, const bool bBottom);

	virtual void Set(const CTextPosition* pTextPosition);
	void SetLeft(const bool bLeft);
	void SetRight(const bool bRight);
	void SetTop(const bool bTop);
	void SetBottom(const bool bBottom);

	bool GetLeft() const;
	bool GetRight() const;
	bool GetTop() const;
	bool GetBottom() const;

	CTextHorzPosition GetHorz() const;
	CTextVertPosition GetVert() const;

protected:
	virtual CString GetSavedRealTextPositionData() const;

private:
	bool m_bLeft;
	bool m_bRight;
	bool m_bTop;
	bool m_bBottom;
};

#endif // !defined(AFX_ELEMENTENVIRONMENT_H__60C3FE74_3EC4_45A6_8D12_AFC3E9745073__INCLUDED_)
