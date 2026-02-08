

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_WebView.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0626 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_CX_WebView,0x027A6920,0x5C7A,0x4BCC,0x88,0xA4,0x88,0x0C,0x15,0x3C,0x05,0x2D);


MIDL_DEFINE_GUID(IID, DIID_IMainWnd,0x67801C5C,0xEC43,0x4CDC,0x9D,0xBD,0x52,0x45,0xE0,0x2B,0x72,0x82);


MIDL_DEFINE_GUID(CLSID, CLSID_MainWnd,0xBF36433D,0xF775,0x4BF6,0x85,0xA5,0xB1,0xBC,0xB6,0x44,0x9D,0x34);


MIDL_DEFINE_GUID(IID, DIID_IIEWnd,0xB8018BF3,0x6CFA,0x460A,0xBA,0x46,0x59,0x1A,0x5C,0x5D,0x28,0xE8);


MIDL_DEFINE_GUID(CLSID, CLSID_IEWnd,0xABF789C7,0x0C42,0x4936,0x88,0x09,0x45,0x4D,0xA2,0x3B,0x2A,0xC4);


MIDL_DEFINE_GUID(IID, DIID_IEdgeWnd,0xFA4AE7FB,0x92A9,0x4452,0x9C,0x50,0xF0,0x8E,0xDA,0xB6,0xE9,0x85);


MIDL_DEFINE_GUID(CLSID, CLSID_EdgeWnd,0x9C4BDD26,0xFECD,0x4137,0x84,0xCF,0x49,0x3C,0x49,0x70,0xEE,0x04);


MIDL_DEFINE_GUID(IID, DIID_IChromeWnd,0xA2ABAE8D,0xE68E,0x4A66,0xB4,0xD4,0x92,0x71,0x1A,0xF4,0x32,0xC7);


MIDL_DEFINE_GUID(CLSID, CLSID_ChromeWnd,0x7775396C,0x4DBB,0x4E20,0x8B,0x93,0x0C,0x6D,0x55,0x33,0x35,0xD8);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



