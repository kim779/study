#include "pch.h"
#include "CX_ANITICKER.h"
#include "ControlWnd.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <cmath>

static CStringW FormatDisplay(const CStringW& text);

IMPLEMENT_DYNCREATE(ControlWnd, CWnd)

ControlWnd::ControlWnd()
	: m_dir(0), m_colorDir(0), m_prevColorDir(0)
	, m_animProg(0.0f), m_bAnimating(false)
	, m_currVal(0.0), m_targetVal(0.0), m_bCounting(false), m_decimalPlaces(0)
	, m_pD2DFactory(nullptr), m_pDWFactory(nullptr)
	, m_pFmt(nullptr)
	, m_pRT(nullptr)
	, m_pBrushUp(nullptr), m_pBrushDown(nullptr), m_pBrushFlat(nullptr)
{
	EnableAutomation();

}

ControlWnd::ControlWnd(_param* pParam, CWnd* pWizard)
	: m_dir(0), m_colorDir(0), m_prevColorDir(0)
	, m_animProg(0.0f), m_bAnimating(false)
	, m_currVal(0.0), m_targetVal(0.0), m_bCounting(false), m_decimalPlaces(0)
	, m_pD2DFactory(nullptr), m_pDWFactory(nullptr)
	, m_pFmt(nullptr)
	, m_pRT(nullptr)
	, m_pBrushUp(nullptr), m_pBrushDown(nullptr), m_pBrushFlat(nullptr)
{
	EnableAutomation();
	m_pWizard = pWizard;

}

ControlWnd::~ControlWnd()
{
	DiscardDeviceResources();
	ReleaseTextFormats();
	if (m_pDWFactory)  { m_pDWFactory->Release();  m_pDWFactory  = nullptr; }
	if (m_pD2DFactory) { m_pD2DFactory->Release(); m_pD2DFactory = nullptr; }
}

void ControlWnd::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}

// ---------------------------------------------------------------------------
// Message map
// ---------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(ControlWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(ControlWnd, CWnd)
	DISP_FUNCTION_ID(ControlWnd, "SetData", dispidSetData, SetData, VT_I2, VTS_I4 VTS_BSTR)
END_DISPATCH_MAP()

// {F461206F-C87B-452F-81FE-095D6C8A3900}
static const IID IID_IControlWnd =
{ 0xf461206f, 0xc87b, 0x452f, { 0x81, 0xfe, 0x09, 0x5d, 0x6c, 0x8a, 0x39, 0x00 } };

BEGIN_INTERFACE_MAP(ControlWnd, CWnd)
	INTERFACE_PART(ControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

// ---------------------------------------------------------------------------
// D2D initialization
// ---------------------------------------------------------------------------

HRESULT ControlWnd::InitD2D()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	if (FAILED(hr)) return hr;
	return DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pDWFactory));
}

HRESULT ControlWnd::CreateDeviceResources()
{
	if (m_pRT) return S_OK;
	if (!m_pD2DFactory) return E_FAIL;

	RECT rc;
	GetClientRect(&rc);
	if (rc.right == 0 || rc.bottom == 0) return S_OK;

	HRESULT hr = m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(GetSafeHwnd(), D2D1::SizeU(rc.right, rc.bottom)),
		&m_pRT);
	if (FAILED(hr)) return hr;

	m_pRT->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.22f, 0.22f), &m_pBrushUp);
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(0.28f, 0.50f, 1.0f), &m_pBrushDown);
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f),   &m_pBrushFlat);

	RecreateTextFormats((float)rc.bottom);
	return S_OK;
}

void ControlWnd::DiscardDeviceResources()
{
	if (m_pBrushUp)   { m_pBrushUp->Release();   m_pBrushUp   = nullptr; }
	if (m_pBrushDown) { m_pBrushDown->Release();  m_pBrushDown = nullptr; }
	if (m_pBrushFlat) { m_pBrushFlat->Release();  m_pBrushFlat = nullptr; }
	if (m_pRT)        { m_pRT->Release();          m_pRT        = nullptr; }
}

void ControlWnd::RecreateTextFormats(float height)
{
	ReleaseTextFormats();
	if (!m_pDWFactory) return;

	float fontSize = height * 0.92f;
	if (fontSize < 8.0f) fontSize = 8.0f;

	m_pDWFactory->CreateTextFormat(
		L"Malgun Gothic", nullptr,
		DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"ko-KR", &m_pFmt);

	if (m_pFmt)
	{
		m_pFmt->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pFmt->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_pFmt->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	}
}

void ControlWnd::ReleaseTextFormats()
{
	if (m_pFmt) { m_pFmt->Release(); m_pFmt = nullptr; }
}

// ---------------------------------------------------------------------------
// Window messages
// ---------------------------------------------------------------------------

int ControlWnd::OnCreate(LPCREATESTRUCT lpCS)
{
	if (CWnd::OnCreate(lpCS) == -1) return -1;
	InitD2D();
	return 0;
}

void ControlWnd::OnDestroy()
{
	KillTimer(TIMER_ANIM);
	DiscardDeviceResources();
	ReleaseTextFormats();
	if (m_pDWFactory)  { m_pDWFactory->Release();  m_pDWFactory  = nullptr; }
	if (m_pD2DFactory) { m_pD2DFactory->Release(); m_pD2DFactory = nullptr; }
	CWnd::OnDestroy();
}

void ControlWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (cx <= 0 || cy <= 0) return;

	if (m_pRT)
	{
		m_pRT->Resize(D2D1::SizeU(cx, cy));
		RecreateTextFormats((float)cy);
	}
	Invalidate(FALSE);
}

BOOL ControlWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void ControlWnd::OnPaint()
{
	CPaintDC dc(this);
	if (!m_pRT)
		CreateDeviceResources();
	if (m_pRT)
		RenderD2D();
}

void ControlWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ANIM)
	{
		if (m_bCounting)
		{
			double diff = m_targetVal - m_currVal;
			if (fabs(diff) < 0.5)
			{
				m_currVal   = m_targetVal;
				m_bCounting = false;
				KillTimer(TIMER_ANIM);
			}
			else
			{
				m_currVal += diff * 0.15;
			}
		}
		else
		{
			m_animProg += m_iInterval;// 0.04f;
			if (m_animProg >= 1.0f)
			{
				m_animProg   = 1.0f;
				m_bAnimating = false;
				KillTimer(TIMER_ANIM);
			}
		}
		Invalidate(FALSE);
	}
	CWnd::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------
// D2D rendering
// ---------------------------------------------------------------------------

void ControlWnd::RenderOdometer(float W, float H)
{
	float t = m_animProg * m_animProg * (3.0f - 2.0f * m_animProg); // smoothstep

	CStringW oldDisplay = FormatDisplay(m_prevText);
	CStringW newDisplay = FormatDisplay(m_text);

	while (oldDisplay.GetLength() < newDisplay.GetLength()) oldDisplay = L" " + oldDisplay;
	while (newDisplay.GetLength() < oldDisplay.GetLength()) newDisplay = L" " + newDisplay;

	int len = newDisplay.GetLength();

	IDWriteTextLayout* pLayout = nullptr;
	if (FAILED(m_pDWFactory->CreateTextLayout(newDisplay, len, m_pFmt, W, H, &pLayout)) || !pLayout)
	{
		float yOld = (m_dir > 0) ? -t * H :  t * H;
		float yNew = (m_dir > 0) ? (1.0f - t) * H : -(1.0f - t) * H;
		DrawSlice(yOld, m_prevText, m_prevColorDir);
		DrawSlice(yNew, m_text,     m_colorDir);
		return;
	}

	int oldDigit{};
	for (int i = 0; i < len; i++)
	{
		wchar_t oldCh = oldDisplay[i];
		wchar_t newCh = newDisplay[i];

		float x0 = 0.0f, x1 = W, dummy = 0.0f;
		DWRITE_HIT_TEST_METRICS metrics{};
		if (i > 0)
			pLayout->HitTestTextPosition(i,     FALSE, &x0, &dummy, &metrics);
		if (i < len - 1)
			pLayout->HitTestTextPosition(i + 1, FALSE, &x1, &dummy, &metrics);

		m_pRT->PushAxisAlignedClip(D2D1::RectF(x0, 0.0f, x1, H), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

		bool oldIsDigit = (oldCh >= L'0' && oldCh <= L'9');
		bool newIsDigit = (newCh >= L'0' && newCh <= L'9');

		if (oldCh == newCh || !oldIsDigit || !newIsDigit)
		{
			// 변화 없거나 숫자가 아닌 자릿수(콤마 등): 정지
			DrawSlice(0.0f, m_text, m_colorDir);
		}
		else
		{
			// 숫자 자릿수 변화: 중간 숫자들 스크롤
			oldDigit = oldCh - L'0';
			int newDigit = newCh - L'0';
			int steps    = newDigit - oldDigit;
			// 전체 방향(m_dir)에 맞게 wrap 경로 선택 (예: 감소 시 0→9는 +9가 아닌 -1)
			if (m_dir > 0 && steps < 0) steps += 10;
			if (m_dir < 0 && steps > 0) steps -= 10;
			int digitDir = (steps >= 0) ? 1 : -1;
			int absSteps = abs(steps);

			float pos  = t * absSteps;
			int   step = (int)pos;
			if (step >= absSteps) step = absSteps - 1;
			float frac = pos - step;

			int d0 = oldDigit + digitDir * step;
			int d1 = oldDigit + digitDir * (step + 1);

			// 증가: 위로 나가고 아래서 들어옴 / 감소: 아래로 나가고 위에서 들어옴
			float yOut = (m_dir > 0) ? -frac * H :  frac * H;
			float yIn  = (m_dir > 0) ? (1.0f - frac) * H : -(1.0f - frac) * H;

			CStringW strOut = newDisplay;
			CStringW strIn  = newDisplay;
			strOut.SetAt(i, L'0' + (wchar_t)((d0 % 10 + 10) % 10));
			strIn .SetAt(i, L'0' + (wchar_t)((d1 % 10 + 10) % 10));

			DrawSlice(yOut, strOut, m_colorDir);
			DrawSlice(yIn,  strIn,  m_colorDir);
		}

		m_pRT->PopAxisAlignedClip();
	}

	pLayout->Release();
}

void ControlWnd::RenderD2D()
{
	if (!m_pRT) return;

	RECT rc;
	GetClientRect(&rc);
	float W = (float)rc.right;
	float H = (float)rc.bottom;

	m_pRT->BeginDraw();
	m_pRT->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f));
	m_pRT->PushAxisAlignedClip(D2D1::RectF(0.0f, 0.0f, W, H), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	if (m_bAnimating && m_dir != 0)
	{
		if (m_type == ANIM_ODOMETER)
		{
			RenderOdometer(W, H);
		}
		else
		{
			float p = m_animProg;
			float t = p * p * (3.0f - 2.0f * p); // smoothstep

			float oldY, newY;
			if (m_dir > 0) { oldY = -t * H; newY = (1.0f - t) * H; }
			else           { oldY =  t * H; newY = -(1.0f - t) * H; }

			CStringW oldDisplay = FormatDisplay(m_prevText);
			CStringW newDisplay = FormatDisplay(m_text);

			int commonLen = 0;
			int minLen = min(oldDisplay.GetLength(), newDisplay.GetLength());
			for (int i = 0; i < minLen; i++)
			{
				if (oldDisplay[i] == newDisplay[i]) commonLen++;
				else break;
			}

			if (commonLen > 0 && oldDisplay.GetLength() == newDisplay.GetLength())
			{
				float splitX = MeasurePrefixX(newDisplay, commonLen);

				m_pRT->PushAxisAlignedClip(D2D1::RectF(0.0f, 0.0f, splitX, H), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
				DrawSlice(0.0f, m_text, m_colorDir);
				m_pRT->PopAxisAlignedClip();

				m_pRT->PushAxisAlignedClip(D2D1::RectF(splitX, 0.0f, W, H), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
				DrawSlice(oldY, m_prevText, m_prevColorDir);
				DrawSlice(newY, m_text, m_colorDir);
				m_pRT->PopAxisAlignedClip();
			}
			else
			{
				DrawSlice(oldY, m_prevText, m_prevColorDir);
				DrawSlice(newY, m_text, m_colorDir);
			}
		}
	}
	else
	{
		CStringW drawText = m_bCounting ? CountDisplayText() : m_text;
		DrawSlice(0.0f, drawText, m_colorDir);
	}

	m_pRT->PopAxisAlignedClip();
	HRESULT hr = m_pRT->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		DiscardDeviceResources();
		Invalidate(FALSE);
	}
}

float ControlWnd::MeasurePrefixX(const CStringW& display, int prefixLen)
{
	if (prefixLen <= 0 || !m_pDWFactory || !m_pFmt || display.IsEmpty())
		return 0.0f;

	RECT rc;
	GetClientRect(&rc);
	float W = (float)rc.right;
	float H = (float)rc.bottom;

	IDWriteTextLayout* pLayout = nullptr;
	if (FAILED(m_pDWFactory->CreateTextLayout(
		display, display.GetLength(), m_pFmt, W, H, &pLayout)) || !pLayout)
		return 0.0f;

	float x = 0.0f, y = 0.0f;
	DWRITE_HIT_TEST_METRICS metrics = {};
	pLayout->HitTestTextPosition(prefixLen, FALSE, &x, &y, &metrics);
	pLayout->Release();
	return x;
}

static CStringW FormatDisplay(const CStringW& text)
{
	if (text.IsEmpty()) return text;

	CStringW s = text;
	if (s[0] == L'+' || s[0] == L'-')
		s = s.Mid(1);

	// Check if numeric (digits and at most one '.')
	bool isNum = !s.IsEmpty();
	for (int i = 0; i < s.GetLength() && isNum; i++)
	{
		if (s[i] != L'.' && (s[i] < L'0' || s[i] > L'9'))
			isNum = false;
	}

	if (!isNum) return s;

	int dotPos = s.Find(L'.');
	CStringW intPart = (dotPos >= 0) ? s.Left(dotPos) : s;
	CStringW decPart = (dotPos >= 0) ? s.Mid(dotPos) : L"";

	CStringW result;
	int len = intPart.GetLength();
	for (int i = 0; i < len; i++)
	{
		if (i > 0 && (len - i) % 3 == 0)
			result += L',';
		result += intPart[i];
	}

	return result + decPart;
}

void ControlWnd::DrawSlice(float yOff, const CStringW& text, int dir)
{
	if (!m_pFmt || text.IsEmpty()) return;

	RECT rc;
	GetClientRect(&rc);
	float W = (float)rc.right;
	float H = (float)rc.bottom;

	ID2D1SolidColorBrush* pBrush = (dir > 0) ? m_pBrushUp :
	                               (dir < 0) ? m_pBrushDown : m_pBrushFlat;

	CStringW display = FormatDisplay(text);
	D2D1_RECT_F layoutRect = D2D1::RectF(0.0f, yOff, W, yOff + H);
	m_pRT->DrawText(display, display.GetLength(), m_pFmt, layoutRect, pBrush);
}

CStringW ControlWnd::CountDisplayText() const
{
	CStringW prefix = (m_colorDir > 0) ? L"+" : (m_colorDir < 0) ? L"-" : L"";
	CStringW num;
	if (m_decimalPlaces > 0)
		num.Format(L"%.*f", m_decimalPlaces, m_currVal);
	else
		num.Format(L"%d", (int)round(m_currVal));
	return prefix + num;
}

// ---------------------------------------------------------------------------
// OLE dispatch
// ---------------------------------------------------------------------------

SHORT ControlWnd::SetData(int gubn, LPCTSTR sVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!sVal || sVal[0] == _T('\0')) return 0;

	CStringW newText(sVal);
	newText.Trim();

	int newColorDir = 0;
	if (!newText.IsEmpty())
	{
		if      (newText[0] == L'+') newColorDir =  1;
		else if (newText[0] == L'-') newColorDir = -1;
	}

	// Strip prefix and check if numeric (allow comma-formatted input)
	CStringW stripped = newText;
	if (!stripped.IsEmpty() && (stripped[0] == L'+' || stripped[0] == L'-'))
		stripped = stripped.Mid(1);
	stripped.Remove(L',');

	bool isNum = !stripped.IsEmpty();
	for (int i = 0; i < stripped.GetLength() && isNum; i++)
		if (stripped[i] != L'.' && (stripped[i] < L'0' || stripped[i] > L'9'))
			isNum = false;

	double newVal = isNum ? _wtof(stripped) : 0.0;

	if (isNum)
	{
		int dotPos = stripped.Find(L'.');
		m_decimalPlaces = (dotPos >= 0) ? (stripped.GetLength() - dotPos - 1) : 0;
	}

	// Init (gubn==0 or first value)
	if (m_text.IsEmpty() || gubn == 0)
	{
		m_text       = newText;
		m_colorDir   = newColorDir;
		m_dir        = 0;
		m_bAnimating = false;
		m_bCounting  = false;
		if (isNum) m_currVal = m_targetVal = newVal;
		KillTimer(TIMER_ANIM);
		Invalidate(FALSE);
		return 0;
	}

	if (isNum && m_type == ANIM_COUNTUP)
	{
		if (!m_bCounting)
			m_currVal = m_targetVal;

		m_targetVal  = newVal;
		m_text       = newText;
		m_colorDir   = newColorDir;
		m_bAnimating = false;

		if (fabs(m_targetVal - m_currVal) < 0.5)
		{
			m_currVal   = m_targetVal;
			m_bCounting = false;
			KillTimer(TIMER_ANIM);
			Invalidate(FALSE);
			return 0;
		}

		if (!m_bCounting)
		{
			m_bCounting = true;
			SetTimer(TIMER_ANIM, 100, nullptr);
		}
		return 0;
	}

	// Slide / Odometer (numeric direction 비교, 비숫자는 direction=0 -> instant)
	{
		CStringW prevStripped = m_text;
		if (!prevStripped.IsEmpty() && (prevStripped[0] == L'+' || prevStripped[0] == L'-'))
			prevStripped = prevStripped.Mid(1);
		prevStripped.Remove(L',');
		double prevVal = _wtof(prevStripped);

		// +/- 접두사는 색상 표시용(전일대비 방향), 슬라이드 방향은 절대값으로 비교
		int newAnimDir = 0;
		if (newVal > prevVal)      newAnimDir =  1;
		else if (newVal < prevVal) newAnimDir = -1;

		if (newAnimDir == 0)
			return 0;

		m_prevText     = m_text;
		m_prevColorDir = m_colorDir;
		m_text         = newText;
		m_colorDir     = newColorDir;
		m_dir          = newAnimDir;
		m_animProg     = 0.0f;
		m_bAnimating   = true;
		m_bCounting    = false;
		SetTimer(TIMER_ANIM, 16, nullptr);
		return 0;
	}
}

void ControlWnd::SetParam(_param* pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	//m_clrForeColor = m_Param.tRGB = pParam->tRGB;
	//m_clrBackColor = m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
	parseOptions();
}

void ControlWnd::parseOptions()
{
	int	idx = 0, pos = 0;
	CString	keys, text, tmps, strtemp;
	CString sOption{};
	sOption = m_Param.options;

	tmps = _T("/ ");
	keys = _T("dpbschlutoargmxwfyvk");

	for (int ii = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = sOption.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = sOption.Find('/', idx);

		text = (pos < 0) ? sOption.Mid(idx) : sOption.Mid(idx, pos - idx);
		text.TrimLeft();

		switch (keys[ii])
		{
			case 's':
			{
				if (text == "0")
					m_type = ANIM_COUNTUP;
				else if(text == "1")
					m_type = ANIM_ODOMETER;
			}
			break;
			case 'u':
			{
		
			}
			break;
			case 't':
			{
				m_iInterval = (float)atof(text);
			}
			break;
			case 'k':
			{
		
			}
			break;
			case 'd':
			{
		
			}
			break;
			case 'v':
			{
		
			}
			break;
			case 'y':
			{
		
			}
			break;
			case 'f':
			{

			}
			break;
		}
	}
}