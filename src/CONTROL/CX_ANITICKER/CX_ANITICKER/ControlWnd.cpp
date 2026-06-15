#include "pch.h"
#include "CX_ANITICKER.h"
#include "ControlWnd.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

IMPLEMENT_DYNCREATE(ControlWnd, CWnd)

ControlWnd::ControlWnd()
	: m_dir(0), m_prevDir(0)
	, m_animProg(0.0f), m_bAnimating(false)
	, m_pD2DFactory(nullptr), m_pDWFactory(nullptr)
	, m_pFmtLabel(nullptr), m_pFmtValue(nullptr)
	, m_pRT(nullptr)
	, m_pBrushLabel(nullptr), m_pBrushUp(nullptr)
	, m_pBrushDown(nullptr), m_pBrushFlat(nullptr)
{
	EnableAutomation();
}

ControlWnd::ControlWnd(_param* pParam, CWnd* pWizard)
	: m_dir(0), m_prevDir(0)
	, m_animProg(0.0f), m_bAnimating(false)
	, m_pD2DFactory(nullptr), m_pDWFactory(nullptr)
	, m_pFmtLabel(nullptr), m_pFmtValue(nullptr)
	, m_pRT(nullptr)
	, m_pBrushLabel(nullptr), m_pBrushUp(nullptr)
	, m_pBrushDown(nullptr), m_pBrushFlat(nullptr)
{
	EnableAutomation();
	m_pWizard = pWizard;
}

ControlWnd::~ControlWnd()
{
	DiscardDeviceResources();
	ReleaseTextFormats();
	if (m_pDWFactory) { m_pDWFactory->Release(); m_pDWFactory = nullptr; }
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

	m_pRT->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &m_pBrushLabel);
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.22f, 0.22f), &m_pBrushUp);
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(0.28f, 0.50f, 1.0f), &m_pBrushDown);
	m_pRT->CreateSolidColorBrush(D2D1::ColorF(0.88f, 0.88f, 0.88f), &m_pBrushFlat);

	RecreateTextFormats((float)rc.bottom);
	return S_OK;
}

void ControlWnd::DiscardDeviceResources()
{
	if (m_pBrushLabel) { m_pBrushLabel->Release(); m_pBrushLabel = nullptr; }
	if (m_pBrushUp)    { m_pBrushUp->Release();    m_pBrushUp    = nullptr; }
	if (m_pBrushDown)  { m_pBrushDown->Release();  m_pBrushDown  = nullptr; }
	if (m_pBrushFlat)  { m_pBrushFlat->Release();  m_pBrushFlat  = nullptr; }
	if (m_pRT)         { m_pRT->Release();          m_pRT         = nullptr; }
}

void ControlWnd::RecreateTextFormats(float height)
{
	ReleaseTextFormats();
	if (!m_pDWFactory) return;

	float fontSize = height * 0.60f;
	if (fontSize < 8.0f) fontSize = 8.0f;

	m_pDWFactory->CreateTextFormat(
		L"Malgun Gothic", nullptr,
		DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"ko-KR", &m_pFmtLabel);

	m_pDWFactory->CreateTextFormat(
		L"Malgun Gothic", nullptr,
		DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"ko-KR", &m_pFmtValue);

	if (m_pFmtLabel)
	{
		m_pFmtLabel->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pFmtLabel->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	if (m_pFmtValue)
	{
		m_pFmtValue->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pFmtValue->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}

void ControlWnd::ReleaseTextFormats()
{
	if (m_pFmtLabel) { m_pFmtLabel->Release(); m_pFmtLabel = nullptr; }
	if (m_pFmtValue) { m_pFmtValue->Release(); m_pFmtValue = nullptr; }
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
	if (m_pDWFactory)   { m_pDWFactory->Release();   m_pDWFactory   = nullptr; }
	if (m_pD2DFactory)  { m_pD2DFactory->Release();  m_pD2DFactory  = nullptr; }
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
		m_animProg += 0.07f;
		if (m_animProg >= 1.0f)
		{
			m_animProg   = 1.0f;
			m_bAnimating = false;
			KillTimer(TIMER_ANIM);
		}
		Invalidate(FALSE);
	}
	CWnd::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------
// D2D rendering
// ---------------------------------------------------------------------------

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
		// Smoothstep easing: t = p*p*(3-2*p)
		float p = m_animProg;
		float t = p * p * (3.0f - 2.0f * p);

		float oldY, newY;
		if (m_dir > 0)
		{
			// Up: old exits top, new enters from bottom
			oldY = -t * H;
			newY = (1.0f - t) * H;
		}
		else
		{
			// Down: old exits bottom, new enters from top
			oldY =  t * H;
			newY = -(1.0f - t) * H;
		}
		DrawRow(oldY, m_prevPrice, m_prevChange, m_prevDir);
		DrawRow(newY, m_price, m_change, m_dir);
	}
	else
	{
		DrawRow(0.0f, m_price, m_change, m_dir);
	}

	m_pRT->PopAxisAlignedClip();
	HRESULT hr = m_pRT->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		DiscardDeviceResources();
		Invalidate(FALSE);
	}
}

void ControlWnd::DrawRow(float yOff, const CStringW& price, const CStringW& change, int dir)
{
	if (!m_pFmtLabel || !m_pFmtValue) return;

	RECT rc;
	GetClientRect(&rc);
	float W = (float)rc.right;
	float H = (float)rc.bottom;

	ID2D1SolidColorBrush* pValBrush = (dir > 0) ? m_pBrushUp :
	                                  (dir < 0) ? m_pBrushDown : m_pBrushFlat;

	// Layout: [30% label][40% price][30% change%]
	float x1 = W * 0.30f;
	float x2 = W * 0.70f;

	if (!m_label.IsEmpty())
	{
		m_pRT->DrawText(m_label, m_label.GetLength(), m_pFmtLabel,
			D2D1::RectF(0.0f, yOff, x1, yOff + H), m_pBrushLabel);
	}
	if (!price.IsEmpty())
	{
		m_pRT->DrawText(price, price.GetLength(), m_pFmtValue,
			D2D1::RectF(x1, yOff, x2, yOff + H), pValBrush);
	}
	if (!change.IsEmpty())
	{
		m_pRT->DrawText(change, change.GetLength(), m_pFmtValue,
			D2D1::RectF(x2, yOff, W, yOff + H), pValBrush);
	}
}

// ---------------------------------------------------------------------------
// OLE dispatch
// ---------------------------------------------------------------------------

SHORT ControlWnd::SetData(int gubn, BSTR sVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!sVal) return 0;

	// Parse pipe-delimited string: "label|price|change|dir"
	// dir: "1"=up, "-1"=down, "0"=flat
	CStringW data(sVal);
	CStringW parts[5];
	int count = 0;
	int start = 0;
	for (int i = 0; i <= data.GetLength() && count < 5; i++)
	{
		if (i == data.GetLength() || data[i] == L'|')
		{
			parts[count++] = data.Mid(start, i - start);
			start = i + 1;
		}
	}

	if (count < 1) return 0;

	if (!parts[0].IsEmpty())
		m_label = parts[0];

	CStringW newPrice  = (count >= 2) ? parts[1] : L"";
	CStringW newChange = (count >= 3) ? parts[2] : L"";
	int      newDir    = (count >= 4) ? _wtoi(parts[3]) : 0;

	bool withAnim = (gubn == 1) && !m_price.IsEmpty() && (newPrice != m_price);

	if (withAnim)
	{
		m_prevPrice  = m_price;
		m_prevChange = m_change;
		m_prevDir    = m_dir;

		m_price  = newPrice;
		m_change = newChange;
		m_dir    = newDir;

		m_animProg   = 0.0f;
		m_bAnimating = true;
		SetTimer(TIMER_ANIM, 16, nullptr);
	}
	else
	{
		m_price  = newPrice;
		m_change = newChange;
		m_dir    = newDir;
		Invalidate(FALSE);
	}

	return 0;
}
