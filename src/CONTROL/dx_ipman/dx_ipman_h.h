

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for dx_ipman.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __dx_ipman_h_h__
#define __dx_ipman_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IIpMan_FWD_DEFINED__
#define __IIpMan_FWD_DEFINED__
typedef interface IIpMan IIpMan;

#endif 	/* __IIpMan_FWD_DEFINED__ */


#ifndef __IpMan_FWD_DEFINED__
#define __IpMan_FWD_DEFINED__

#ifdef __cplusplus
typedef class IpMan IpMan;
#else
typedef struct IpMan IpMan;
#endif /* __cplusplus */

#endif 	/* __IpMan_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Dx_ipman_LIBRARY_DEFINED__
#define __Dx_ipman_LIBRARY_DEFINED__

/* library Dx_ipman */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Dx_ipman,0xE70CB534,0x3A7F,0x443B,0x8E,0x91,0x34,0x4E,0x3C,0x72,0x51,0xE7);

#ifndef __IIpMan_DISPINTERFACE_DEFINED__
#define __IIpMan_DISPINTERFACE_DEFINED__

/* dispinterface IIpMan */
/* [uuid] */ 


DEFINE_GUID(DIID_IIpMan,0x44792F41,0x1FCD,0x47C1,0x95,0x93,0xC5,0xB7,0x20,0x91,0x61,0x4E);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("44792F41-1FCD-47C1-9593-C5B72091614E")
    IIpMan : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IIpManVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIpMan * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIpMan * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIpMan * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIpMan * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIpMan * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIpMan * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIpMan * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IIpManVtbl;

    interface IIpMan
    {
        CONST_VTBL struct IIpManVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIpMan_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IIpMan_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IIpMan_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IIpMan_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IIpMan_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IIpMan_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IIpMan_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IIpMan_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_IpMan,0x92E6F069,0x22E8,0x4F36,0xA8,0x46,0x90,0x45,0xDA,0xAB,0x2D,0x38);

#ifdef __cplusplus

class DECLSPEC_UUID("92E6F069-22E8-4F36-A846-9045DAAB2D38")
IpMan;
#endif
#endif /* __Dx_ipman_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


