

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for cx_THtmlEx.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_cx_THtmlEx,0x3F424F76,0xDBA2,0x4836,0x83,0xC3,0x9A,0xAB,0xEF,0x5E,0xA9,0x57);


MIDL_DEFINE_GUID(IID, DIID_IControlWnd,0x7E741965,0x4318,0x4459,0xA8,0xF9,0x38,0x23,0x12,0x3B,0x9B,0x0E);


MIDL_DEFINE_GUID(CLSID, CLSID_ControlWnd,0xA6300030,0x038E,0x440D,0xBC,0x82,0xAA,0xF2,0x9D,0x75,0xBD,0x48);


MIDL_DEFINE_GUID(IID, DIID_IAxBrowser,0x1AECD67C,0xE92A,0x4ED6,0x84,0xB1,0xFC,0x48,0xE0,0x36,0x40,0x8D);


MIDL_DEFINE_GUID(CLSID, CLSID_AxBrowser,0x5DD71E6A,0x959E,0x4B0A,0xB9,0xB1,0x72,0x1F,0x4E,0xF9,0xAA,0x67);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



