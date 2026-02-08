// OcxIDs.h: interface for the COcxIDs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OCXIDS_H__B27916CF_2513_43CE_B9AF_8ADDA78381AF__INCLUDED_)
#define AFX_OCXIDS_H__B27916CF_2513_43CE_B9AF_8ADDA78381AF__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// (2008/3/20 - Seung-Won, Bae) OCX Version ID 관리
/////////////////////////////////////////////////////////////////////////////

#define _OCX_VER						1		//IBK 배포버전
//#define _OCX_VER						99		//TEST용 버전

#if _OCX_VER == 1
#define _OCX_PROG_ID					"IBKCHART.IBKChartCtrl.1"
#define _OCX_PP_PROG_ID					"IBKCHART.IBKChartPropPage.1"

#define _OCX_TYPE_LIBRARY_ID			D64827B8-55F0-4399-BEDD-0D2AD9C65451
//8AFC054D-E96B-4227-A5F7-9AA4207C4A1C
#define _OCX_TYPE_LIBRARY_ID_VALUES		{ 0xd64827b8, 0x55f0, 0x4399, { 0xbe, 0xdd, 0xd, 0x2a, 0xd9, 0xc6, 0x54, 0x51 } }
//{ 0x8afc054d, 0xe96b, 0x4227, { 0xa5, 0xf7, 0x9a, 0xa4, 0x20, 0x7c, 0x4a, 0x1c } }

#define _OCX_PRIMARY_DISP_ID			5E2211A8-2AFB-4338-B40D-E900730A9DDF
//8AF588B7-9206-4eba-9057-C9351465120A
#define _OCX_PRIMARY_DISP_ID_VALUES		{ 0x5e2211a8, 0x2afb, 0x4338, { 0xb4, 0xd, 0xe9, 0x0, 0x73, 0xa, 0x9d, 0xdf } }
//{ 0x8af588b7, 0x9206, 0x4eba, { 0x90, 0x57, 0xc9, 0x35, 0x14, 0x65, 0x12, 0xa } }

#define _OCX_EVENT_DISP_ID				6390F0D0-6F41-4492-8B66-5022D05B8EF8
//DBD9623E-E113-497f-8FD4-316783920A72
#define _OCX_EVENT_DISP_ID_VALUES		{ 0x6390f0d0, 0x6f41, 0x4492, { 0x8b, 0x66, 0x50, 0x22, 0xd0, 0x5b, 0x8e, 0xf8 } }
//{ 0xdbd9623e, 0xe113, 0x497f, { 0x8f, 0xd4, 0x31, 0x67, 0x83, 0x92, 0xa, 0x72 } }

#define _OCX_CLASS_INFO_ID				B98261E1-B73D-4bb1-90D6-E6C8BAAEEBD5 
//F2914299-FB2C-4165-AEB4-8008514490D6
#define _OCX_CLASS_INFO_ID_VALUES				IMPLEMENT_OLECREATE_EX( CKTBChartCtrl, _OCX_PROG_ID,					\
										0xb98261e1, 0xb73d, 0x4bb1, 0x90, 0xd6, 0xe6, 0xc8, 0xba, 0xae, 0xeb, 0xd5		\
												)
										//0xf2914299, 0xfb2c, 0x4165, 0xae, 0xb4, 0x80, 0x8, 0x51, 0x44, 0x90, 0xd6		\
										//		)

// {C5715BAC-3826-4845-B082-4CB50D7EF383}
//IMPLEMENT_OLECREATE(<<class>>, <<external_name>>, 
//					0xc5715bac, 0x3826, 0x4845, 0xb0, 0x82, 0x4c, 0xb5, 0xd, 0x7e, 0xf3, 0x83);
#define _OCX_PP_CLASS_INFO_ID_VALUES			IMPLEMENT_OLECREATE_EX( CKTBChartPropPage, _OCX_PP_PROG_ID,			\
									0xc5715bac, 0x3826, 0x4845, 0xb0, 0x82, 0x4c, 0xb5, 0xd, 0x7e, 0xf3, 0x83		\
												)	
									//	0xb07a1acc, 0xf06, 0x4dd7, 0xba, 0x41, 0xc6, 0xeb, 0x4d, 0xe3, 0xc7, 0xd0		\
									//			)
#define _OCX_IMPLEMENT_OLECREATE_EX()		_OCX_CLASS_INFO_ID_VALUES
#define _OCX_PP_IMPLEMENT_OLECREATE_EX()	_OCX_PP_CLASS_INFO_ID_VALUES
#endif

#endif // !defined(AFX_OCXIDS_H__B27916CF_2513_43CE_B9AF_8ADDA78381AF__INCLUDED_)
