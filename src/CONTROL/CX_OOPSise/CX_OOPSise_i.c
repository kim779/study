

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_OOPSise.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_CX_OOPSise,0xB64CB01F,0x97A8,0x41E2,0xA1,0x98,0x95,0x52,0x33,0xB1,0xFA,0x14);


MIDL_DEFINE_GUID(IID, DIID_IControlWnd,0xA9BEF83E,0x7126,0x47B4,0x86,0x8D,0xFF,0xF9,0x50,0xDB,0xA2,0x37);


MIDL_DEFINE_GUID(CLSID, CLSID_ControlWnd,0x95A01D60,0xC8A3,0x4ADE,0xAE,0x3E,0xB9,0xF3,0xA5,0xD2,0xBF,0xF6);


MIDL_DEFINE_GUID(IID, DIID_IAxisWnd,0xD9F419BD,0x6114,0x4ECD,0x94,0x3A,0xB5,0xCA,0xF7,0xC1,0xA6,0x05);


MIDL_DEFINE_GUID(CLSID, CLSID_AxisWnd,0xAE4A6561,0x0182,0x464D,0xB0,0xBE,0x41,0xD3,0xEB,0xB1,0xF9,0xE7);


MIDL_DEFINE_GUID(IID, DIID_IAxTest,0x95B275FB,0xD00C,0x465B,0xA1,0x19,0xA0,0x25,0x39,0xD1,0xC0,0x0C);


MIDL_DEFINE_GUID(CLSID, CLSID_AxTest,0x96D7775D,0x2730,0x46E7,0xB3,0x93,0x09,0x1F,0xD8,0xBB,0x1A,0xB6);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



