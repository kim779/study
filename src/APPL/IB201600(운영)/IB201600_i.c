

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for IB201600.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
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

MIDL_DEFINE_GUID(IID, LIBID_IB201600,0x473DFF18,0x2C95,0x42D1,0x8D,0xB3,0x3F,0x33,0x02,0x61,0xC5,0x83);


MIDL_DEFINE_GUID(IID, DIID_IToolWnd,0x4ED2ADAD,0x0014,0x4441,0xBE,0xF6,0x6B,0xCE,0xC7,0xBC,0x6C,0x32);


MIDL_DEFINE_GUID(CLSID, CLSID_ToolWnd,0x6CFD36F2,0x38C4,0x4AF8,0x84,0x8A,0xD2,0x31,0x1B,0x74,0x7F,0xBE);


MIDL_DEFINE_GUID(IID, DIID_IViewWnd,0x0436CD91,0x84D3,0x41BC,0x97,0x7C,0xDC,0x0C,0x7E,0x87,0x59,0xB6);


MIDL_DEFINE_GUID(CLSID, CLSID_ViewWnd,0x7BDFD516,0xFEE5,0x4EEA,0x8F,0x36,0x37,0xE6,0x67,0x01,0x22,0x9E);


MIDL_DEFINE_GUID(IID, DIID_ISiseWnd,0x98CBF053,0xC703,0x4615,0xAB,0xA0,0x89,0xC5,0x94,0x3F,0x7C,0xBA);


MIDL_DEFINE_GUID(CLSID, CLSID_SiseWnd,0x7B3FBE3B,0xD247,0x43EE,0xB1,0xCC,0x48,0x90,0x5B,0x5A,0x8F,0x34);


MIDL_DEFINE_GUID(IID, DIID_IMatrixWnd,0xA76C7CE4,0x07DF,0x4E22,0xB5,0xED,0xC0,0xEA,0x4E,0xCB,0x71,0xFB);


MIDL_DEFINE_GUID(CLSID, CLSID_MatrixWnd,0xF5748DD6,0x26BF,0x4D79,0xA9,0x20,0xB3,0xF8,0xAD,0xC9,0x30,0x7E);


MIDL_DEFINE_GUID(IID, DIID_IHogaWnd,0xD3314442,0x9B73,0x48C6,0x8B,0x91,0x7F,0x4F,0xAA,0x7D,0x3C,0xF0);


MIDL_DEFINE_GUID(CLSID, CLSID_HogaWnd,0x9BF209B4,0xB57A,0x4E8B,0xBC,0x93,0x71,0xFA,0x55,0x9A,0x6C,0xD8);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



