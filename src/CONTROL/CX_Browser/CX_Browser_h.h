

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_Browser.odl:
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


#ifndef __CX_Browser_h_h__
#define __CX_Browser_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWebLinkCtrl_FWD_DEFINED__
#define __IWebLinkCtrl_FWD_DEFINED__
typedef interface IWebLinkCtrl IWebLinkCtrl;

#endif 	/* __IWebLinkCtrl_FWD_DEFINED__ */


#ifndef __WebLinkCtrl_FWD_DEFINED__
#define __WebLinkCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class WebLinkCtrl WebLinkCtrl;
#else
typedef struct WebLinkCtrl WebLinkCtrl;
#endif /* __cplusplus */

#endif 	/* __WebLinkCtrl_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_Browser_LIBRARY_DEFINED__
#define __CX_Browser_LIBRARY_DEFINED__

/* library CX_Browser */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_Browser,0x5CE1C369,0x2CAA,0x41C0,0x93,0xB3,0xB8,0xE4,0xE3,0xD2,0xA4,0xEE);

#ifndef __IWebLinkCtrl_DISPINTERFACE_DEFINED__
#define __IWebLinkCtrl_DISPINTERFACE_DEFINED__

/* dispinterface IWebLinkCtrl */
/* [uuid] */ 


DEFINE_GUID(DIID_IWebLinkCtrl,0x878CA46C,0x2C1A,0x4BF3,0x97,0xDB,0xE6,0xBE,0x28,0xA9,0x1E,0x8E);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("878CA46C-2C1A-4BF3-97DB-E6BE28A91E8E")
    IWebLinkCtrl : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IWebLinkCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWebLinkCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWebLinkCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWebLinkCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWebLinkCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWebLinkCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWebLinkCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWebLinkCtrl * This,
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
    } IWebLinkCtrlVtbl;

    interface IWebLinkCtrl
    {
        CONST_VTBL struct IWebLinkCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebLinkCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWebLinkCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWebLinkCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWebLinkCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWebLinkCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWebLinkCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWebLinkCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IWebLinkCtrl_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_WebLinkCtrl,0xA9262258,0xEE28,0x4736,0xAC,0x34,0x91,0x32,0x1C,0x5E,0x24,0x4B);

#ifdef __cplusplus

class DECLSPEC_UUID("A9262258-EE28-4736-AC34-91321C5E244B")
WebLinkCtrl;
#endif
#endif /* __CX_Browser_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


