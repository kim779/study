// XMLparser.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "XMLparser.h"

// CXMLparser
CXMLparser::CXMLparser()
{
	m_pXmlList = NULL;
}

CXMLparser::~CXMLparser()
{
}

void CXMLparser::parserRun(CString path)
{
	//
	// Check the file specified exists.
	//
	if (path.IsEmpty())
		return;
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		IXMLDOMDocument *pDoc;
		if (SUCCEEDED(CoCreateInstance(CLSID_DOMDocument,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IXMLDOMDocument,
			reinterpret_cast<void**>(&pDoc))))
		{
			//
			// Tell the "doc" that we're not going to load asynchronously.
			//
			if (SUCCEEDED(pDoc->put_async(VARIANT_FALSE)))
			{
				CComVariant vFile(path);
				VARIANT_BOOL vBool;
				pDoc->load(vFile, &vBool);
				if (vBool == VARIANT_TRUE)
				{
					IXMLDOMNode *pNode;
					if (SUCCEEDED(pDoc->QueryInterface(IID_IXMLDOMNode,
						reinterpret_cast<void**>(&pNode))))
					{
						if(m_pXmlList) delete m_pXmlList;
						m_pXmlList = NULL;
						IterateChildNodes(pNode, m_pXmlList);
						pNode->Release();
						pNode = NULL;
					}
				}
				else
					CheckLoad(pDoc);
			}
			pDoc->Release();
			pDoc = NULL;
		}
		CoUninitialize();
	}
}

char* CXMLparser::convertUtf8ToAnsi(char* buff)
{
	BSTR    bstrStr;
	char*   buffOut;
	int     length;

	// Get length of wide char buffer
	length = MultiByteToWideChar(CP_UTF8, 0, buff, lstrlen(buff) + 1, NULL, NULL);

	// Change UTF-8 to unicode (UTF-16)
	bstrStr = SysAllocStringLen(NULL, length);
	MultiByteToWideChar(CP_UTF8, 0, buff, lstrlen(buff) + 1, bstrStr, length);

	// Get length of multi byte buffer
	length = WideCharToMultiByte(CP_ACP, 0, bstrStr, -1, NULL, 0, NULL, NULL);

	// Change from unicode to mult byte
	buffOut = (char*)malloc(length);
	WideCharToMultiByte(CP_ACP, 0, bstrStr, -1, buffOut, length, NULL, NULL);

	SysFreeString(bstrStr);
	return buffOut;
}

bool CXMLparser::IterateChildNodes(IXMLDOMNode *pNode, xmlList* pXmlList)
{
	BSTR bstrNodeName;

	if (pNode)
	{
		CString strOutput;
		pNode->get_nodeName(&bstrNodeName);

		//
		// Find out the node type (as a string).
		//
		BSTR bstrNodeType;
		pNode->get_nodeTypeString(&bstrNodeType);
		CString strType;
		strType.Format(_T("%S"), bstrNodeType);
		SysFreeString(bstrNodeType);

		DOMNodeType eEnum;
		pNode->get_nodeType(&eEnum);

		CString strValue;
		if (eEnum == NODE_TEXT)
		{
			BSTR bstrValue;
			pNode->get_text(&bstrValue);
			//strOutput.Format(_T("%S"), bstrValue);
			strOutput = (LPCWSTR)bstrValue;
			SysFreeString(bstrValue);
			//pXmlList->value = strOutput;
		}
		else if (eEnum == NODE_COMMENT)
		{
			VARIANT vValue;
			pNode->get_nodeValue(&vValue);

			CString strValue;
			if (vValue.vt == VT_BSTR)
				strOutput.Format(_T("%S"), V_BSTR(&vValue));
			else
				strOutput.Format(_T("Unknown comment type"));
			VariantClear(&vValue);
		}
		else if (eEnum == NODE_PROCESSING_INSTRUCTION)
		{
			strOutput.Format(_T("%S"), bstrNodeName);
		}
		else if (eEnum == NODE_ELEMENT)
		{
			strOutput.Format(_T("%S"), bstrNodeName);
			//pXmlList->elm = strOutput;
			//pXmlList->sub = new xmlList();
		}
		else
		{
			// 
			// Other types, include the type name.
			//
			strOutput.Format(_T("%S - %s"), bstrNodeName, strType);
			//pXmlList = new xmlList();
		}
		SysFreeString(bstrNodeName);
	}

	IXMLDOMNode *pNext = NULL;
	IXMLDOMNode *pChild;
	pNode->get_firstChild(&pChild);

	while (pChild)
	{
		IterateChildNodes(pChild, pXmlList);
		pChild->get_nextSibling(&pNext);
		pChild->Release();
		pChild = pNext;
	}
	return true;
}

HRESULT CXMLparser::CheckLoad(IXMLDOMDocument *pDoc)
{
	HRESULT hResult = E_FAIL;
	long lErrorCode = E_FAIL;
	IXMLDOMParseError *pXMLError = NULL;

	if (SUCCEEDED(pDoc->get_parseError(&pXMLError))
		&& SUCCEEDED(pXMLError->get_errorCode(&lErrorCode))
		&& (lErrorCode != 0))
		hResult = ReportError(pXMLError);

	//
	// Clean-up pointers used.
	//
	if (pXMLError)
	{
		pXMLError->Release();
		pXMLError = NULL;
	}

	//
	// Pass back the return code.
	//
	return lErrorCode;
}

HRESULT CXMLparser::ReportError(IXMLDOMParseError *pXMLError)
{
	long lLine;
	long lLinePos;
	long lErrorCode;
	BSTR bstrFile;
	BSTR bstrReason;

	//
	// Whilst these could all return errors, we'll assume that since the
	// COM was awake enough to report an error (and return the structure)
	// that it can work with these calls.
	//
	pXMLError->get_line(&lLine);
	pXMLError->get_linepos(&lLinePos);
	pXMLError->get_errorCode(&lErrorCode);
	pXMLError->get_reason(&bstrReason);
	pXMLError->get_url(&bstrFile);

	//
	// Format this message into a suitable output.
	//
	if (lLine > 0)
	{
		CString strError;
		strError.Format(_T("Error processing XML file: %S\nError on line %d, position %d\n")
			_T("Error (%x) text: %S"),
			bstrFile,
			lLine, lLinePos, lErrorCode,
			bstrReason);
		TRACE(_T("%s\n"), strError);
	}

	SysFreeString(bstrFile);
	SysFreeString(bstrReason);

	return NOERROR;
}
