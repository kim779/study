/****************************************************************************************************
* Copyright (C) 2013 by Kyung-jin Kim
* e-mail		: devmachine@naver.com
*
*
* Description	: Easy Ini Helper Class
* Created	: Sep-5,2013
* Last Updated	: Jul-15,2014
* Version	: EZIni v1.2
*/
#pragma once
#include <vector>
#include <map>
#pragma warning(disable:26487)
#pragma warning(disable:26489)
#pragma warning(disable:26486)
#pragma warning(disable:26481)
	
/************************************************************************************************/
// Class EzIni
/************************************************************************************************/
class EZIni
{
public:
	class Exception
	{
	public:
		Exception(DWORD dwErrorCode, LPCTSTR lpErrorMessage)
			: m_dwErrorCode(dwErrorCode), m_sErrorMessage(lpErrorMessage) {}

		CString GetErrorMessage() const { return m_sErrorMessage; }
		DWORD GetErrorCode() const noexcept { return m_dwErrorCode; }

	private:
		DWORD m_dwErrorCode{};
		CString m_sErrorMessage;
	};

	class Key;
	class Section
	{
		friend class EZIni;

	public:
		
		Key operator[](LPCSTR lpKeyName)
		{
			return Key(m_sFileName, m_sSectionName, CA2T(lpKeyName));
		}

		Key operator[](LPCWSTR lpKeyName)
		{
			return Key(m_sFileName, m_sSectionName, CW2T(lpKeyName));
		}

		BOOL Exists() const
		{
			std::vector<CString> listOfSections;

			EZIni dummy(m_sFileName);
			dummy.EnumSectionNames(listOfSections);

			return std::find(listOfSections.begin(), listOfSections.end(), m_sSectionName) != listOfSections.end();
		}

		BOOL Delete() noexcept
		{
			return WritePrivateProfileString(m_sSectionName, nullptr, _T(""), m_sFileName);
		}

		BOOL EnumKeyNames(std::vector<CString>& listOfKeyNames) const
		{
			if (!PathFileExists(m_sFileName))
				return FALSE;

			listOfKeyNames.clear();

			DWORD dwLen = READ_BUFFER_SIZE;

			CString sBuffer;
			DWORD dwCopied = GetPrivateProfileSection(m_sSectionName, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
			sBuffer.ReleaseBuffer();			
			while (dwCopied + 2 >= dwLen)
			{
				dwLen += READ_BUFFER_SIZE;				
				dwCopied = GetPrivateProfileSection(m_sSectionName, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
				sBuffer.ReleaseBuffer();
			}

			CString sLine, sKeyName;
			LPCTSTR pLines = sBuffer.GetBuffer();
			if (pLines)
			{
				while (*pLines)
				{
					sLine = pLines;
					AfxExtractSubString(sKeyName, pLines, 0, _T('='));
					sKeyName.Trim();

					listOfKeyNames.push_back(sKeyName);
					pLines += sLine.GetLength() + 1;
				}
			}
			return TRUE;
		}

		DWORD readAll(CString& sData)
		{
			if (!PathFileExists(m_sFileName))
				return 0;

			DWORD dwLen = READ_BUFFER_SIZE;

			CString sBuffer;
			DWORD dwCopied = GetPrivateProfileSection(m_sSectionName, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
			sBuffer.ReleaseBuffer();
			while (dwCopied + 2 >= dwLen)
			{
				dwLen += READ_BUFFER_SIZE;
				dwCopied = GetPrivateProfileSection(m_sSectionName, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
				sBuffer.ReleaseBuffer();
			}

			CString sLine, sKeyName;
			LPCTSTR pLines = sBuffer.GetString();
			if (pLines)
			{
				while (*pLines)
				{
					sLine = pLines;
					sData += sLine + _T("\r\n");
					pLines += sLine.GetLength() + 1;
				}
			}
			sData.TrimRight(_T("\r\n"));
			sData.AppendChar(0);
			return dwCopied;
		}

		DWORD readKey(std::map<CString, CString> &map)
		{
			if (!PathFileExists(m_sFileName))
				return 0;

			DWORD dwLen = READ_BUFFER_SIZE;

			CString sBuffer;
			DWORD dwCopied = GetPrivateProfileSection(m_sSectionName, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
			sBuffer.ReleaseBuffer();
			while (dwCopied + 2 >= dwLen)
			{
				dwLen += READ_BUFFER_SIZE;
				dwCopied = GetPrivateProfileSection(m_sSectionName, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
				sBuffer.ReleaseBuffer();
			}

			CString sLine, sKey, sData;
			LPCTSTR pLines = sBuffer.GetString();
			if (pLines)
			{
				while (*pLines)
				{
					sLine = pLines;
					AfxExtractSubString(sKey, sLine, 0, '=');
					AfxExtractSubString(sData, sLine, 1, '=');

					//sKeyName = Parser(sData, _T("="));				
					map.insert(std::make_pair(sKey, sData));

					pLines += sLine.GetLength() + 1;
				}
			}
			return dwCopied;
		}

		DWORD readKey(std::vector<std::pair<CString, CString>>& vector)
		{
			if (!PathFileExists(m_sFileName))
				return 0;

			DWORD dwLen = READ_BUFFER_SIZE;

			CString sBuffer;
			DWORD dwCopied = GetPrivateProfileSection(m_sSectionName, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
			sBuffer.ReleaseBuffer();
			while (dwCopied + 2 >= dwLen)
			{
				dwLen += READ_BUFFER_SIZE;
				dwCopied = GetPrivateProfileSection(m_sSectionName, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
				sBuffer.ReleaseBuffer();
			}

			CString sLine, sKey, sData;
			LPCTSTR pLines = sBuffer.GetString();
			if (pLines)
			{
				while (*pLines)
				{
					sLine = pLines;
					AfxExtractSubString(sKey, sLine, 0, '=');
					AfxExtractSubString(sData, sLine, 1, '=');

					vector.emplace_back(std::make_pair(sKey, sData));

					pLines += sLine.GetLength() + 1;
				}
			}
			return vector.size();
		}


		BOOL writeAll(const CString& sData) noexcept
		{
			return WritePrivateProfileSection(m_sSectionName, sData, m_sFileName);
		}

	private:
		// EZIni::Section class
		Section(LPCTSTR lpFileName, LPCTSTR lpSectionName)
			: m_sFileName(lpFileName),
			m_sSectionName(lpSectionName)
		{
		}

	private:
		CString m_sFileName;
		CString m_sSectionName;
	};

	class Key
	{
		friend class EZIni;
		friend class EZIni::Section;

	public:
		Key& operator=(int nValue)
		{
			_SetKeyValue(nValue, _T("%d"));
			return *this;
		}

		Key& operator=(UINT uValue)
		{
			_SetKeyValue(uValue, _T("%u"));
			return *this;
		}

		Key& operator=(INT64 n64Value)
		{
			_SetKeyValue(n64Value, _T("%I64d"));
			return *this;
		}

		Key& operator=(UINT64 u64Value)
		{
			_SetKeyValue(u64Value, _T("%I64u"));
			return *this;
		}

		Key& operator=(bool bValue)
		{
			_SetKeyValue(bValue, _T("%d"));
			return *this;
		}

		Key& operator=(double dValue)
		{
			_SetKeyValue(dValue, _T("%f"));
			return *this;
		}

		Key& operator=(LPCTSTR lpValue)
		{
			_SetKeyValue(lpValue, _T("%s"));
			return *this;
		}

		Key& operator=(POINT ptValue)
		{
			_SetKeyValue(_Point2String(ptValue), _T("%s"));
			return *this;
		}

		Key& operator=(LPCRECT lpRectValue)
		{
			_SetKeyValue(_Rect2String(lpRectValue), _T("%s"));
			return *this;
		}

		Key& operator=(const COleDateTime& dtValue)
		{
			_SetKeyValue(_DateTime2String(dtValue), _T("%s"));
			return *this;
		}
		
		Key& operator=(const COLORREF colValue)
		{
			_SetKeyValue(colValue, _T("%u"));
			return *this;
		}

		operator int()
		{
			CString sValue = _GetKeyValue(m_nDefaultValue, _T("%d"));
			return _tcstol(sValue, nullptr, 10);
		}

		operator UINT()
		{
			CString sValue = _GetKeyValue(m_uDefaultValue, _T("%u"));			
			return _tcstoul(sValue, nullptr, 10);
		}

		operator INT64()
		{
			CString sValue = _GetKeyValue(m_n64DefaultValue, _T("%I64d"));
			return _tcstoi64(sValue, nullptr, 10);
		}

		operator UINT64()
		{
			CString sValue = _GetKeyValue(m_u64DefaultValue, _T("%I64u"));
			return _tcstoui64(sValue, nullptr, 10);
		}

		operator bool()
		{
			CString sValue = _GetKeyValue(m_bDefaultValue, _T("%d"));
			return (_tcstol(sValue, nullptr, 10)) == 0 ? false : true;
		}

		operator double()
		{
			CString sValue = _GetKeyValue(m_dDefaultValue, _T("%f"));
			return _tstof(sValue);
		}

		operator CString()
		{
			CString sValue = _GetKeyValue(m_sDefaultValue, _T("%s"));
			return sValue;
		}

		operator CPoint()
		{
			CString sValue = _GetKeyValue(_Point2String(m_ptDefaultValue), _T("%s"));
			return _String2Point(sValue);
		}

		operator CRect()
		{
			CString sValue = _GetKeyValue(_Rect2String(&m_rcDefaultValue), _T("%s"));
			return _String2Rect(sValue);
		}

		operator COleDateTime()
		{
			CString sValue = _GetKeyValue(_DateTime2String(m_dtDefaultValue), _T("%s"));
			return _String2DateTime(sValue);
		}

		operator COLORREF()
		{
			CString sValue = _GetKeyValue(m_uDefaultValue, _T("%u"));			
			return _tcstoul(sValue, nullptr, 10);
		}

		Key& operator++()
		{
			int nValue = *this;
			*this = ++nValue;

			return *this;
		}

		Key& operator--()
		{
			int nValue = *this;
			*this = --nValue;

			return *this;
		}

		Key& operator+=(int nValue)
		{
			const int nRead = *this;
			*this = nRead + nValue;

			return *this;
		}

		Key& operator-=(int nValue)
		{
			const int nRead = *this;
			*this = nRead - nValue;
			return *this;
		}

		Key& operator*=(int nValue)
		{
			const int nRead = *this;
			*this = nRead * nValue;

			return *this;
		}

		Key& operator/=(int nValue)
		{
			ASSERT(0 != nValue);

			const int nRead = *this;
			*this = nRead / nValue;

			return *this;
		}

		Key& operator%=(int nValue)
		{
			ASSERT(0 != nValue);

			const int nRead = *this;
			*this = nRead % nValue;

			return *this;
		}

		Key& operator+=(UINT uValue)
		{
			const UINT uRead = *this;
			*this = uRead + uValue;

			return *this;
		}

		Key& operator-=(UINT uValue)
		{
			const UINT uRead = *this;
			*this = uRead - uValue;

			return *this;
		}

		Key& operator*=(UINT uValue)
		{
			const UINT uRead = *this;
			*this = uRead * uValue;

			return *this;
		}

		Key& operator/=(UINT uValue)
		{
			ASSERT(0 != uValue);

			const UINT uRead = *this;
			*this = uRead / uValue;

			return *this;
		}

		Key& operator%=(UINT uValue)
		{
			ASSERT(0 != uValue);

			const UINT uRead = *this;
			*this = uRead + uValue;

			return *this;
		}

		Key& operator<<=(UINT uValue)
		{
			const UINT uRead = *this;
			*this = uRead << uValue;

			return *this;
		}

		Key& operator>>=(UINT uValue)
		{
			const UINT uRead = *this;
			*this = uRead >> uValue;

			return *this;
		}

		Key& operator&=(UINT uValue)
		{
			const UINT uRead = *this;
			*this = uRead & uValue;

			return *this;
		}

		Key& operator|=(UINT uValue)
		{
			const UINT uRead = *this;
			*this = uRead | uValue;

			return *this;
		}

		Key& operator^=(UINT uValue)
		{
			const UINT uRead = *this;
			*this = uRead ^ uValue;

			return *this;
		}

		Key& operator+=(INT64 n64Value)
		{
			const INT64 n64Read = *this;
			*this = n64Read + n64Value;

			return *this;
		}

		Key& operator-=(INT64 n64Value)
		{
			const INT64 n64Read = *this;
			*this = n64Read - n64Value;

			return *this;
		}

		Key& operator*=(INT64 n64Value)
		{
			const INT64 n64Read = *this;
			*this = n64Read * n64Value;

			return *this;
		}

		Key& operator/=(INT64 n64Value)
		{
			ASSERT(0 != n64Value);

			const INT64 n64Read = *this;
			*this = n64Read / n64Value;

			return *this;
		}

		Key& operator%=(INT64 n64Value)
		{
			ASSERT(0 != n64Value);

			const INT64 n64Read = *this;
			*this = n64Read % n64Value;

			return *this;
		}


		Key& operator+=(UINT64 u64Value)
		{
			const UINT64 u64Read = *this;
			*this = u64Read + u64Value;

			return *this;
		}

		Key& operator-=(UINT64 u64Value)
		{
			const UINT64 u64Read = *this;
			*this = u64Read - u64Value;

			return *this;
		}

		Key& operator*=(UINT64 u64Value)
		{
			const UINT64 u64Read = *this;
			*this = u64Read * u64Value;

			return *this;
		}

		Key& operator/=(UINT64 u64Value)
		{
			ASSERT(0 != u64Value);

			const UINT64 u64Read = *this;
			*this = u64Read / u64Value;

			return *this;
		}

		Key& operator%=(UINT64 u64Value)
		{
			ASSERT(0 != u64Value);

			const UINT64 u64Read = *this;
			*this = u64Read % u64Value;

			return *this;
		}

		Key& operator<<=(UINT64 u64Value)
		{
			const UINT64 u64Read = *this;
			*this = u64Read << u64Value;

			return *this;
		}

		Key& operator>>=(UINT64 u64Value)
		{
			const UINT64 u64Read = *this;
			*this = u64Read >> u64Value;

			return *this;
		}

		Key& operator&=(UINT64 u64Value)
		{
			const UINT64 u64Read = *this;
			*this = u64Read & u64Value;

			return *this;
		}

		Key& operator|=(UINT64 u64Value)
		{
			const UINT64 u64Read = *this;
			*this = u64Read | u64Value;

			return *this;
		}

		Key& operator^=(UINT64 u64Value)
		{
			const UINT64 u64Read = *this;
			*this = u64Read ^ u64Value;

			return *this;
		}

		Key& operator+=(double dValue)
		{
			const double dRead = *this;
			*this = dRead + dValue;

			return *this;
		}

		Key& operator-=(double dValue)
		{
			const double dRead = *this;
			*this = dRead - dValue;
			return *this;
		}

		Key& operator*=(double dValue)
		{
			const double dRead = *this;
			*this = dRead * dValue;

			return *this;
		}

		Key& operator/=(double dValue)
		{
			ASSERT(0.0 != dValue);

			const double dRead = *this;
			*this = dRead / dValue;

			return *this;
		}

		Key& operator+=(LPCTSTR lpText)
		{
			CString sRead = *this;
			*this = sRead + lpText;

			return (*this);
		}

		Key& operator+=(POINT ptValue)
		{
			CString sRead, sTemp;
			int nDelimiter;

			sTemp = sRead = *this;
			nDelimiter = sTemp.Remove(_T(','));

			if (1 == nDelimiter)
				*this = _String2Point(sRead) + ptValue;

			else if (3 == nDelimiter)
				*this = _String2Rect(sRead) + ptValue;

			else
				ASSERT(FALSE);

			return *this;
		}

		Key& operator-=(POINT ptValue)
		{
			*this += CPoint(-ptValue.x, -ptValue.y);
			return *this;
		}

		Key& operator+=(SIZE sizeValue)
		{
			CString sRead, sTemp;
			int nDelimiter;

			sTemp = sRead = *this;
			nDelimiter = sTemp.Remove(_T(','));

			if (1 == nDelimiter)
				*this = _String2Point(sRead) + sizeValue;

			else if (3 == nDelimiter)
				*this = _String2Rect(sRead) + sizeValue;

			else
				ASSERT(FALSE);

			return *this;
		}

		Key& operator-=(SIZE sizeValue)
		{
			*this += CSize(-sizeValue.cx, -sizeValue.cy);
			return *this;
		}

		Key& operator+=(LPCRECT lpRectValue)
		{
			const CRect rcRead = *this;
			*this = rcRead + lpRectValue;

			return *this;
		}

		Key& operator-=(LPCRECT lpRectValue)
		{
			const CRect rcRead = *this;
			*this = rcRead - lpRectValue;

			return *this;
		}

		Key& operator+=(COleDateTimeSpan dateSpan)
		{
			const COleDateTime dtRead = *this;
			*this = dtRead + dateSpan;

			return *this;
		}

		Key& operator-=(COleDateTimeSpan dateSpan)
		{
			const COleDateTime dtRead = *this;
			*this = dtRead - dateSpan;

			return *this;
		}

		Key& operator<<(int nDefaultValue) noexcept
		{
			m_nDefaultValue = nDefaultValue;
			return *this;
		}

		Key& operator<<(UINT uDefaultValue) noexcept
		{
			m_uDefaultValue = uDefaultValue;
			return *this;
		}

		Key& operator<<(COLORREF uDefaultValue) noexcept
		{
			m_uDefaultValue = uDefaultValue;
			return *this;
		}

		Key& operator<<(INT64 n64DefaultValue) noexcept
		{
			m_n64DefaultValue = n64DefaultValue;
			return *this;
		}

		Key& operator<<(UINT64 u64DefaultValue) noexcept
		{
			m_u64DefaultValue = u64DefaultValue;
			return *this;
		}

		Key& operator<<(bool bDefaultValue) noexcept
		{
			m_bDefaultValue = bDefaultValue;
			return *this;
		}

		Key& operator<<(double dDefaultValue) noexcept
		{
			m_dDefaultValue = dDefaultValue;
			return *this;
		}

		Key& operator<<(LPCTSTR lpDefaultValue)
		{
			m_sDefaultValue = lpDefaultValue;
			return *this;
		}

		Key& operator<<(POINT ptDefaultValue) noexcept
		{
			m_ptDefaultValue = ptDefaultValue;
			return *this;
		}

		Key& operator<<(LPCRECT lpDefaultValue) noexcept
		{
			m_rcDefaultValue = *lpDefaultValue;
			return *this;
		}

		Key& operator<<(const COleDateTime& dtDefaultValue) noexcept
		{
			m_dtDefaultValue = dtDefaultValue;
			return *this;
		}

		Key& operator>>(int nDefaultValue) noexcept
		{
			m_nDefaultValue = nDefaultValue;
			return *this;
		}

		Key& operator>>(UINT uDefaultValue) noexcept
		{
			m_uDefaultValue = uDefaultValue;
			return *this;
		}

		Key& operator>>(INT64 n64DefaultValue) noexcept
		{
			m_n64DefaultValue = n64DefaultValue;
			return *this;
		}

		Key& operator>>(UINT64 u64DefaultValue) noexcept
		{
			m_u64DefaultValue = u64DefaultValue;
			return *this;
		}

		Key& operator>>(bool bDefaultValue) noexcept
		{
			m_bDefaultValue = bDefaultValue;
			return *this;
		}

		Key& operator>>(double dDefaultValue) noexcept
		{
			m_dDefaultValue = dDefaultValue;
			return *this;
		}

		Key& operator>>(LPCTSTR lpDefaultValue)
		{
			m_sDefaultValue = lpDefaultValue;
			return *this;
		}

		Key& operator>>(POINT ptDefaultValue) noexcept
		{
			m_ptDefaultValue = ptDefaultValue;
			return *this;
		}

		Key& operator>>(LPCRECT lpDefaultValue) noexcept
		{
			m_rcDefaultValue = *lpDefaultValue;
			return *this;
		}

		Key& operator>>(const COleDateTime& dtDefaultValue) noexcept
		{
			m_dtDefaultValue = dtDefaultValue;
			return *this;
		}

		// Ini Key Helper function
		BOOL Exists() const
		{
			std::vector<CString> listOfKeyNames;

			EZIni dummy(m_sFileName);
			dummy[m_sSectionName].EnumKeyNames(listOfKeyNames);

			return std::find(listOfKeyNames.begin(), listOfKeyNames.end(), m_sKeyName) != listOfKeyNames.end();
		}

		BOOL Delete() noexcept
		{
			return WritePrivateProfileString(m_sSectionName, m_sKeyName, nullptr, m_sFileName);
		}

	private:
		// EzIni::Key class
		Key(LPCTSTR lpFileName, LPCTSTR lpSectionName, LPCTSTR lpKeyName)
			: m_sFileName(lpFileName),
			m_sSectionName(lpSectionName),
			m_sKeyName(lpKeyName),
			m_sDefaultValue(_T(""))
		{
			m_nDefaultValue = 0;
			m_uDefaultValue = 0;
			m_n64DefaultValue = 0;
			m_u64DefaultValue = 0;
			m_bDefaultValue = 0;
			m_dDefaultValue = 0;	
			m_rcDefaultValue = CRect(0, 0, 0, 0);
		}


		template <typename _T>
		void _SetKeyValue(_T value, LPCTSTR lpFormatSpec)
		{
			ASSERT(!m_sFileName.IsEmpty() && !m_sSectionName.IsEmpty() && !m_sKeyName.IsEmpty());

			CString sValue;
			sValue.Format(lpFormatSpec, value);
			const BOOL bResult = WritePrivateProfileString(m_sSectionName, m_sKeyName, sValue, m_sFileName);
			if (!bResult)
			{
				CString sErrMessage, sFormat;
				sFormat.Format(_T("Failed to write value(%s)."), lpFormatSpec);
				sFormat += _T(" %s");
				sErrMessage.Format(sFormat, value, EZIni::_GetErrorMessage());
			//	throw EZIni::Exception(GetLastError(), sErrMessage);
			}
		}

		template <typename _T>
		CString _GetKeyValue(const _T& defaultValue, LPCTSTR lpFormatSpec)
		{
			CString sDefaultValue;
			sDefaultValue.Format(lpFormatSpec, defaultValue);

			DWORD dwLen = READ_BUFFER_SIZE;
			CString sBuffer;

			DWORD dwCopied = GetPrivateProfileString(m_sSectionName, m_sKeyName, sDefaultValue, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
			sBuffer.ReleaseBuffer();
			while (dwCopied + 1 >= dwLen)
			{
				dwLen += READ_BUFFER_SIZE;
				dwCopied = GetPrivateProfileString(m_sSectionName, m_sKeyName, sDefaultValue, sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
				sBuffer.ReleaseBuffer();
			}
			return sBuffer;
		}



		CString _Point2String(const POINT& ptValue)
		{
			CString sResult;
			sResult.Format(_T("%d,%d"), ptValue.x, ptValue.y);
			return sResult;
		}

		CString _Rect2String(LPCRECT lpRectValue)
		{
			CString sResult;
			sResult.Format(_T("%d,%d,%d,%d"), lpRectValue->left, lpRectValue->top, lpRectValue->right, lpRectValue->bottom);
			return sResult;
		}

		CString _DateTime2String(const COleDateTime& dtValue)
		{
			return dtValue.Format(_T("%Y-%m-%d %H:%M:%S"));
		}

		CPoint _String2Point(LPCTSTR lpValue)
		{
			CString sPoint[2];
			for (int i = 0; i < 2; ++i)
				AfxExtractSubString(gsl::at(sPoint, i), lpValue, i, _T(','));

			return CPoint(_tstoi(sPoint[0]), _tstoi(sPoint[1]));
		}

		CRect _String2Rect(LPCTSTR lpValue)
		{
			CString sRect[4];
			for (int i = 0; i < 4; ++i)
				AfxExtractSubString(gsl::at(sRect, i), lpValue, i, _T(','));

			return CRect(_tstoi(sRect[0]), _tstoi(sRect[1]), _tstoi(sRect[2]), _tstoi(sRect[3]));
		}

		COleDateTime _String2DateTime(LPCTSTR lpValue)
		{
			COleDateTime dtResult;
			const bool bParsed = dtResult.ParseDateTime(lpValue);
			ASSERT(bParsed);
			return dtResult;
		}



	private:
		CString m_sFileName;
		CString m_sSectionName;
		CString m_sKeyName;

		CString m_sDefaultValue;
		union
		{
			int m_nDefaultValue;
			UINT m_uDefaultValue;
			INT64 m_n64DefaultValue;
			UINT64 m_u64DefaultValue;
			bool m_bDefaultValue;
			double m_dDefaultValue;
			POINT m_ptDefaultValue{};
			RECT m_rcDefaultValue;
			DATE m_dtDefaultValue;
		};
	};


	EZIni::EZIni(void) noexcept
	{
	}

	EZIni::EZIni(LPCTSTR lpFileName, BOOL bCreateAsUnicode = FALSE)
	{
		SetFileName(lpFileName, bCreateAsUnicode);
	}

	//EZIni::~EZIni(void)
	//{
	//}

	Section operator[](LPCSTR lpSectionName)
	{
		return Section(m_sFileName, CA2T(lpSectionName));
	}

	Section operator[](LPCWSTR lpSectionName)
	{
		return Section(m_sFileName, CW2T(lpSectionName));
	}

	void SetFileName(LPCTSTR lpFileName, BOOL bCreateAsUnicode = FALSE)
	{
		m_sFileName = lpFileName;
		if (bCreateAsUnicode && !PathFileExists(lpFileName))
		{
			const WORD BOM = 0xFEFF;
			HANDLE hFile = CreateFile(lpFileName, GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (INVALID_HANDLE_VALUE != hFile)
			{
				WriteFile(hFile, &BOM, sizeof(WORD), NULL, NULL);
				CloseHandle(hFile);
			}
		}
	}

	CString GetFileName(){ return m_sFileName; }

	bool EnumSectionNames(std::vector<CString>& listOfSectionNames) const
	{
		if (!PathFileExists(m_sFileName))
			return false;

		listOfSectionNames.clear();

		DWORD dwLen = READ_BUFFER_SIZE;
		CString sBuffer;
		DWORD dwCopied = GetPrivateProfileSectionNames(sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
		sBuffer.ReleaseBuffer();
		while (dwCopied + 2 >= dwLen)
		{
			dwLen += READ_BUFFER_SIZE;
			dwCopied = GetPrivateProfileSectionNames(sBuffer.GetBuffer(dwLen), dwLen, m_sFileName);
		}

		LPCTSTR pSections = sBuffer.GetBuffer();
		if (pSections)
		{
			while (*pSections)
			{
				listOfSectionNames.push_back(pSections);
				pSections += listOfSectionNames.back().GetLength() + 1;
			}
		}
		
		return listOfSectionNames.size() == 0 ? false : true;
	}

private:
	enum { READ_BUFFER_SIZE = 512 };

	static CString _GetErrorMessage()
	{
		CString sMsg;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), sMsg.GetBuffer(256), 256, nullptr);
		sMsg.ReleaseBuffer();
		sMsg.TrimRight(_T(" \t\r\n"));

		return sMsg;
	}

private:
	CString m_sFileName;
};





/**
* Sample code

************ Write Operation ************
try
{
EZIni ini(_T("D:\\ServerInfo.ini"));

ini["Primary"]["IP"] = _T("192.168.0.1");
ini["Primary"]["Port"] = 5678;
ini["Primary"]["Value"] = 89.97;
ini["Primary"]["HasSecondary"] = true;
ini["Primary"]["LastAccess"] = COleDateTime::GetCurrentTime();

EZIni::Section section = ini["Secondary"];
section["IP"] = _T("192.168.0.2");
section["Port"] = 6789;

EZIni::Key key = section["Value"];
key = 78.56;
key = 90.12;
}
catch (EZIni::Exception& e)
{
TRACE(_T("Ini Error: %s\n"), e.GetErrorMessage());
}

************ Read Operation ************
EZIni ini(_T("D:\\ServerInfo.ini"));

CString sIP = ini["Primary"]["IP"];
int nPort = ini["Primary"]["Port"];
double dValue = ini["Primary"]["Value"];
bool bHasSecondary = ini["Primary"]["HasSecondary"];
COleDateTime dtLastAccess = ini["Primary"]["LastAccess"];

TRACE(_T("Primary Server Info\n"));
TRACE(_T("IP: %s, Port: %d, Value: %.2f\n"), sIP, nPort, dValue);
TRACE(_T("HasSecondary: %s, LastAccess: %s\n\n"),
bHasSecondary ? _T("true") : _T("false"), dtLastAccess.Format(_T("%Y-%m-%d %H:%M:%S")));

// Use default value if it doesn't exist;
sIP = ini["Tertiary"]["IP"] << _T("192.168.0.3");
nPort = ini["Tertiary"]["Port"] << 7890;
dValue = ini["Tertiary"]["Value"] << 12.79;

TRACE(_T("Tertiary Server Info\n"));
TRACE(_T("IP: %s, Port: %d, Value: %.2f\n"), sIP, nPort, dValue);

************ Arithmetic/Logical Operation ************
try
{
EZIni ini(_T("D:\\ServerInfo.ini"));

ini["Primary"]["IP"] += _T(":5678");

++ini["Primary"]["Port"];
--ini["Primary"]["Port"];

ini["Primary"]["Value"] *= 10.0;
ini["Primary"]["LastAccess"] += COleDateTimeSpan(1, 0, 0, 0);
}
catch (EZIni::Exception& e)
{
TRACE(_T("Ini Error: %s\n"), e.GetErrorMessage());
}

************ Helper Functions ************
EZIni ini(_T("D:\\ServerInfo.ini"));

// Print all sections
std::vector<CString> listOfSectionNames;
ini.EnumSectionNames(listOfSectionNames);

TRACE(_T("List of Section names\n"));
for (auto it = listOfSectionNames.begin(); it != listOfSectionNames.end(); ++it)
TRACE(_T("%s\n"), *it);

// Print all keys in Primary Section
std::vector<CString> listOfKeyNames;
ini["Primary"].EnumKeyNames(listOfKeyNames);

TRACE(_T("\nList of Key names(Primary Section)\n"));
for (auto it = listOfKeyNames.begin(); it != listOfKeyNames.end(); ++it)
TRACE(_T("%s\n"), *it);

// Delete key if it exists
if (ini["Secondary"]["IP"].Exists())
ini["Secondary"]["IP"].Delete();

// Delete section if it exists
if (ini["Secondary"].Exists())
ini["Secondary"].Delete();

************ Unicode Support ************
EZIni ini(_T("D:\\ServerInfo.ini"), TRUE);
*/





// EZIni class
