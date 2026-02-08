

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_PORTFOLIO.odl:
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


#ifndef __CX_PORTFOLIO_h_h__
#define __CX_PORTFOLIO_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPortfolio_FWD_DEFINED__
#define __IPortfolio_FWD_DEFINED__
typedef interface IPortfolio IPortfolio;

#endif 	/* __IPortfolio_FWD_DEFINED__ */


#ifndef __Portfolio_FWD_DEFINED__
#define __Portfolio_FWD_DEFINED__

#ifdef __cplusplus
typedef class Portfolio Portfolio;
#else
typedef struct Portfolio Portfolio;
#endif /* __cplusplus */

#endif 	/* __Portfolio_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_PORTFOLIO_LIBRARY_DEFINED__
#define __CX_PORTFOLIO_LIBRARY_DEFINED__

/* library CX_PORTFOLIO */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_PORTFOLIO,0xC69ADCBC,0x8DA9,0x4595,0x91,0x53,0x27,0x3A,0x64,0xB8,0xFB,0xD7);

#ifndef __IPortfolio_DISPINTERFACE_DEFINED__
#define __IPortfolio_DISPINTERFACE_DEFINED__

/* dispinterface IPortfolio */
/* [uuid] */ 


DEFINE_GUID(DIID_IPortfolio,0xA2BB4FBC,0x4EBF,0x49A4,0x98,0xE2,0x92,0xB8,0x6C,0x56,0xB9,0x1F);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A2BB4FBC-4EBF-49A4-98E2-92B86C56B91F")
    IPortfolio : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IPortfolioVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPortfolio * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPortfolio * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPortfolio * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPortfolio * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPortfolio * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPortfolio * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPortfolio * This,
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
    } IPortfolioVtbl;

    interface IPortfolio
    {
        CONST_VTBL struct IPortfolioVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPortfolio_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPortfolio_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPortfolio_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPortfolio_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPortfolio_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPortfolio_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPortfolio_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IPortfolio_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Portfolio,0xF324975F,0x8130,0x460E,0x99,0x0F,0x36,0x10,0xD4,0x87,0x05,0xF0);

#ifdef __cplusplus

class DECLSPEC_UUID("F324975F-8130-460E-990F-3610D48705F0")
Portfolio;
#endif
#endif /* __CX_PORTFOLIO_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


