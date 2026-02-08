// PropertiesData.h: interface for the CPropertiesData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTIESDATA_H__1690F66F_1721_4210_A550_EC605AD3D1CD__INCLUDED_)
#define AFX_PROPERTIESDATA_H__1690F66F_1721_4210_A550_EC605AD3D1CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2004.04.13, 배승원) Global Variable을 Static을 이용하지 않고, 별도의 CPP에 정의하여
//		Module의 크기를 줄이도록 한다. (또한 중복 정의되지 않도록 Header Include Condition을 둔다.)
// (2004.06.08, 배승원) Global 공유 속성을 추가한다.

#define	INIT_PROPERTIES_NUM		24	//	Initailize Property Page의 Properties 수
#define	PPG_PROPERTIES_NUM		50	//	Property Page의 Properties 수

#define	TYPE_INIT_PROPERTIES	0	// Initailize Property Page
#define	TYPE_PPG_PROPERTIES		1	// Property Page
#define TYPE_GET_LINE_COLOR		2	// Reset Line Color

//	Propertis Style
#define	STYLE_INT				0
#define STYLE_COLOR				1
#define STYLE_BOOL				2
#define STYLE_STRING			3

//	Initialize Properties Name
extern char *pInitPropertiesName[];

//	Property Page Properties Name
extern char *pPropertiesListName[];

//	Initialize Properties Default Value
extern char *pInitPropertiesValue[];

//	Property Page Properties Default Value
extern char *pPropertiesListValue[];

//	Initialize Properties Style
extern int pInitPropertiesStyle[];

//	Property Page Properties Style
extern int pPropertiesListStyle[];

extern char *pInitPropertiesShortKey[];

extern char *pPropertiesShortKey[];

extern char *pEtcPropertiesShortKey[];

#endif // !defined(AFX_PROPERTIESDATA_H__1690F66F_1721_4210_A550_EC605AD3D1CD__INCLUDED_)
