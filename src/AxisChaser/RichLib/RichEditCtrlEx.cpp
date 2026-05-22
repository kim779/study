// RichEditCtrlEx.cpp : implementation file
//

#include "RichEditCtrlEx.h"
#include <mbstring.h>
/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx

CRichEditCtrlEx::CRichEditCtrlEx()
{
}

CRichEditCtrlEx::~CRichEditCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CRichEditCtrlEx, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditCtrlEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx message handlers



void CRichEditCtrlEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CRichEditCtrl::PreSubclassWindow();
	SetBackgroundColor( FALSE, ::GetSysColor( COLOR_BTNFACE ));// RGB(0,0,0) );
//	disable( ) ;
//	readOnly( ) ;
}


//
// GLOBAL STREAM MANIPULATORS
CRTFBuilder& 
doWrite(	CRTFBuilder&		r, 
					CRichEditCtrl&	c	 ) throw ( )
{																					 
	r.write( c ) ;
	return r ;
}

CRTFBuilder& 
doSize(		CRTFBuilder&		r, 
					int n	 ) throw ( )
{																					 
	r.size( n ) ;
	return r ;
}

CRTFBuilder& doBold(	CRTFBuilder& r,
											bool				 b )
{
	r.bold( b ) ;
	return r ;
}

CRTFBuilder& doStrike(	CRTFBuilder& r,
											bool				 b )
{
	r.strike( b ) ;
	return r ;
}

CRTFBuilder& doItalic(	CRTFBuilder& r,
												bool				 b )
{
	r.italic( b ) ;
	return r ;
}

CRTFBuilder& doUnderline(	CRTFBuilder& r,
													bool				 b )
{
	r.underline( b ) ;
	return r ;
}

CRTFBuilder& doFont(	CRTFBuilder&	r,
											int						n)
{
	r.font( _T("굴림"));
	return r ;
}

CRTFBuilder& doColor(	CRTFBuilder&	r,
											int						n)
{
	r.color( (COLORREF)n ) ;
	return r ;
}

CRTFBuilder& doBackColor(	CRTFBuilder&	r,
													int						n)
{
	r.backColor( (COLORREF)n ) ;
	return r ;
}

CRTFBuilder& doAddColor(	CRTFBuilder&	r,
													int						n)
{
	r.addColor( (COLORREF)n ) ;
	return r ;
}

CRTFBuilder& doFont(	CRTFBuilder&	r,
											CString&			s)
{
	r.font( s ) ;
	return r ;
}

CRTFBuilder& doAddFont(	CRTFBuilder&	r,
												CString&			s)
{
	r.addFont( s ) ;
	return r ;
}


CControlManip
write( CRichEditCtrl& c ) throw ( )
{
	return CControlManip(	&doWrite, 
											c	) ;
}

CIntManip
size( int n ) throw( )
{
	return CIntManip( &doSize, 
										n ) ;
}

CIntManip
font( int n ) throw( )
{
	return CIntManip( &doFont, 
										n ) ;
}

CIntManip
color( int n ) throw( )
{
	return CIntManip( &doColor, 
										n ) ;
}

CIntManip
backColor( int n ) throw( )
{
	return CIntManip( &doBackColor, 
										n ) ;
}

CIntManip
addColor( int n ) throw( )
{
	return CIntManip( &doAddColor, 
										n ) ;
}

CStringManip
font( LPCTSTR s ) throw( )
{
	return CStringManip(	&doFont, 
												s ) ;
}

CStringManip
addFont( LPCTSTR s ) throw( )
{
	return CStringManip(	&doAddFont, 
												s ) ;
}

CBoolManip
bold( bool b ) throw( )
{
	return CBoolManip(	&doBold, 
											b ) ;
}

CBoolManip
strike( bool b ) throw( )
{
	return CBoolManip(	&doStrike, 
											b ) ;
}

CBoolManip
italic( bool b ) throw( )
{
	return CBoolManip(	&doItalic, 
											b ) ;
}

CBoolManip
underline( bool b ) throw( )
{
	return CBoolManip(	&doUnderline, 
											b ) ;
}


CRTFBuilder& 
operator << (	CRTFBuilder&	b, 
							RTFSM_PFUNC			f ) 
{
	return f( b );
}


CRTFBuilder& 
operator << (	CRTFBuilder&		b, 
							CManip&			f ) 
{
	return f.go( b );
}

//CRTFBuilder& 
//operator << (	CRTFBuilder&		b, 
//							CControlManip&	f ) 
//{
//	return f.go( b );
//}

CRTFBuilder& 
normal( CRTFBuilder& b )
{
	b.normal( ) ;
	return b ;
}

CRTFBuilder& 
black( CRTFBuilder& b )
{
	b.black( ) ;
	return b ;
}

CRTFBuilder& 
push( CRTFBuilder& b )
{
	b.push( ) ;
	return b ;
}

CRTFBuilder& 
pull( CRTFBuilder& b )
{
	b.pull( ) ;
	return b ;
}

CRTFBuilder& 
red( CRTFBuilder& b )
{
	b.red( ) ;
	return b ;
}

CRTFBuilder& 
green( CRTFBuilder& b )
{
	b.green( ) ;
	return b ;
}

CRTFBuilder& 
blue( CRTFBuilder& b )
{
	b.blue( ) ;
	return b ;
}

CRTFBuilder& 
bold( CRTFBuilder& b )
{
	b.bold( ) ;
	return b ;
}

CRTFBuilder& 
strike( CRTFBuilder& b )
{
	b.strike( ) ;
	return b ;
}

CRTFBuilder& 
italic( CRTFBuilder& b )
{
	b.italic( ) ;
	return b ;
}

CRTFBuilder& 
underline( CRTFBuilder& b )
{
	b.underline( ) ;
	return b ;
}


CRTFBuilder::CRTFBuilder()
{
//	m_fontList.add( "Arial" ) ;
	m_colorList.add( RGB( 0,0,0 )  ) ;
	m_colorList.add( RGB( 255,0,0 )  ) ;
	m_colorList.add( RGB( 0,255,0 )  ) ;
	m_colorList.add( RGB( 0,0,255 )  ) ;
//	size(10);
}

CRTFBuilder::~CRTFBuilder()
{

}


void CRTFBuilder::push( )
{
	m_attrStack.push(m_attr);
}

void CRTFBuilder::pull( )
{
	m_attr = m_attrStack.top( ) ;
	m_attrStack.pop( ) ;
}

void CRTFBuilder::color( COLORREF c )
{
	int n ( m_colorList.find( c )  );
	m_attr.m_nColorFground= n<0 ? m_colorList.add( c )
															:	n ;
}

void CRTFBuilder::backColor( COLORREF c )
{
	int n ( m_colorList.find( c )  )	;
	m_attr.m_nColorBground= n<0 ? m_colorList.add( c )
															: n ;
}

void CRTFBuilder::black()
{
	m_attr.m_nColorFground = 0 ;
}

void CRTFBuilder::red()
{
	m_attr.m_nColorFground = 1 ;
}

void CRTFBuilder::green()
{
	m_attr.m_nColorFground = 2 ;

}

void CRTFBuilder::blue()
{
	m_attr.m_nColorFground = 3 ;
}

void CRTFBuilder::size(int n)
{
	m_attr.m_nFontSize = n*2 ;
}

void CRTFBuilder::font( const CString& strFont)
{

	int nCount =  0 ;
	for ( list<CString>::iterator i = m_fontList.begin( ) ; i != m_fontList.end( ) ; i++, nCount++ )
	{
		if ( (*i) == strFont )
		{
			m_attr.m_nFontNumber = nCount ;
			return ;
		}
	}
	//not there, lets add it
	m_fontList.add( strFont ) ;
	m_attr.m_nFontNumber = nCount ;
}

void CRTFBuilder::bold(bool b)
{
	m_attr.m_bsBold= b ;
}

void CRTFBuilder::strike(bool b)
{
	m_attr.m_bsStrike= b ;
}

void CRTFBuilder::italic(bool b)
{
	m_attr.m_bsItalic = b ;
}

void CRTFBuilder::underline(bool b)
{
	m_attr.m_bsUnderline = b ;
}

void CRTFBuilder::normal( )
{
	bold(false);
	italic(false);
	underline(false);
	strike(false);
}

static DWORD CALLBACK EditStreamCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString *pstr = (CString *)dwCookie;

	if( pstr->GetLength() < cb )
	{
		*pcb = pstr->GetLength();
		memcpy(pbBuff, (LPCSTR)*pstr, *pcb );
		pstr->Empty();
	}
	else
	{
		*pcb = cb;
		memcpy(	pbBuff, 
						(LPCSTR)*pstr, 
						*pcb ) ;
		*pstr = pstr->Right( pstr->GetLength() - cb ) ;
	}
	return 0;
}

void CRTFBuilder::write( CRichEditCtrl& c )
{
	m_string += '}';

	int	len = m_string.GetLength();
//	CRtfStringDlg d( m_string ) ;

	EDITSTREAM es = {(DWORD)&m_string, 0, EditStreamCallBack };

	// richEd is the rich edit control
	c.StreamIn( SF_RTF | SFF_SELECTION , es);
	m_string.Empty( ) ;
}

#ifdef DF_MBCS
CRTFBuilder& CRTFBuilder::operator+=(LPCTSTR p)
{
	CString s(p), s2;

	int len = s.GetLength();
	for (int i = 0; i < len; i++)
	{
		unsigned char ch = (unsigned char)s[i];

		if (ch == '\n')
		{
			s2 += "\r\n\\par ";
		}
		else if (ch == '\\' || ch == '{' || ch == '}')
		{
			// RTF 메타문자 escape
			CString esc;
			esc.Format("\\%c", ch);
			s2 += esc;
		}
		else if (_ismbblead(ch) && (i + 1) < len)
		{
			// CP949 2바이트 문자(한글 등) → \'XX\'XX
			unsigned char hi = ch;
			unsigned char lo = (unsigned char)s[i + 1];
			CString esc;
			esc.Format("\\'%02x\\'%02x", hi, lo);
			s2 += esc;
			i++; // trail byte 건너뜀
		}
		else if (ch > 0x7F)
		{
			// ASCII 범위 밖의 단일 바이트도 안전하게 escape
			CString esc;
			esc.Format("\\'%02x", ch);
			s2 += esc;
		}
		else
		{
			s2 += (char)ch;
		}
	}

	WORD langID = PRIMARYLANGID(GetSystemDefaultLangID());
	switch (langID)
	{
	case LANG_KOREAN:
		m_string += (CString)
			"{\\rtf1\\ansi\\ansicpg949\\deff0\\deftab720";
		break;
	case LANG_CHINESE:
		m_string += (CString)
			"{\\rtf1\\ansi\\ansicpg950\\deff0\\deftab720";
		break;
	case LANG_ENGLISH:
	default:
		m_string += (CString)
			"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deftab720";
		break;
	}

	m_string += (CString)m_fontList;
	m_string += (CString)m_colorList;
	m_string += (CString)m_attr;
	m_string += s2;

	return *this;
}
#else
CRTFBuilder& CRTFBuilder::operator+=( LPCTSTR p )
{
	CString s( p ) , s2 , tmps;
	for ( int i = 0 ; i < s.GetLength( ) ; i ++ )
		if ( s[i]=='\n' )
			s2+= (CString)"\r\n" += "\\par ";//\\par " ;
		else
			s2+= s[i] ;

	WORD	langID = PRIMARYLANGID(GetSystemDefaultLangID());
	switch (langID)
	{
	case LANG_KOREAN:
		m_string += (CString)
		"{\\rtf1\\ansi\\ansicpg949\\deff0\\deftab720";
		break;
	case LANG_CHINESE:
		m_string += (CString)
		"{\\rtf1\\ansi\\ansicpg950\\deff0\\deftab720";
		break;
	case LANG_ENGLISH:
	default:
		m_string += (CString)
		"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deftab720";
		break;
	}

	m_string += (CString)m_fontList +=
	(CString)m_colorList += (CString)m_attr += s2;
	return *this ;

}
#endif

CRTFBuilder&
CRTFBuilder::operator << ( LPCTSTR p )
{
	*this+=p ;
	return *this ;
}

CRTFBuilder&
CRTFBuilder::operator << ( int n )
{
	CString s ;
	s.Format( "%d", n ) ;
	*this += (LPCTSTR)s ;
	return *this ;
}


CRTFBuilder&
CRTFBuilder::operator >> ( CRichEditCtrl& e )
{
	write( e ) ;
	return *this ;
}


