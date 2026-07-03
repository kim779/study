#include "stdafx.h"
#include "CMarketTimeManager.h"

#include "stdafx.h"
#include "CMarketTimeManager.h"

namespace
{
    const TCHAR* const VERIFIED_TIME_URL = _T("http://www.ibks.com/");
    const DWORD VERIFIED_TIME_TIMEOUT_MS = 3000;
}

CMarketTimeManager::CMarketTimeManager()
{
}

CMarketTimeManager::~CMarketTimeManager()
{
}

int CMarketTimeManager::ToMinute(int hour, int minute) const
{
    return hour * 60 + minute;
}

BOOL CMarketTimeManager::IsInTimeRange(int nowMin, int startMin, int endMin) const
{
    if (startMin < endMin)
    {
        return (startMin <= nowMin && nowMin < endMin);
    }

    // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ñ¾î°¡ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    return (nowMin >= startMin || nowMin < endMin);
}

int CMarketTimeManager::GetNthSunday(int year, int month, int nth) const
{
    int count = 0;

    for (int day = 1; day <= 31; ++day)
    {
        CTime t(year, month, day, 0, 0, 0);

        if (t.GetMonth() != month)
            break;

        if (t.GetDayOfWeek() == 1) // 1 = Sunday
        {
            ++count;

            if (count == nth)
                return day;
        }
    }

    return 0;
}

BOOL CMarketTimeManager::IsUSDST(const CTime& serverKst)
{
    CTime utcTime = serverKst + CTimeSpan(0, -9, 0, 0);  // KST(UTC+9) -> UTC

    int year = utcTime.GetYear();

    // DST start: 2nd Sunday of March, 02:00 EST(UTC-5) == 07:00 UTC
    CTime dstStartUtc(year, 3, GetNthSunday(year, 3, 2), 7, 0, 0);

    // DST end: 1st Sunday of November, 02:00 EDT(UTC-4) == 06:00 UTC
    CTime dstEndUtc(year, 11, GetNthSunday(year, 11, 1), 6, 0, 0);

    return (utcTime >= dstStartUtc && utcTime < dstEndUtc);
}

CMarketTimeManager::US_MARKET_SESSION
CMarketTimeManager::GetUSMarketSession(const CTime& serverKst)
{
    BOOL isDst = IsUSDST(serverKst);

    int nowMin = ToMinute(serverKst.GetHour(), serverKst.GetMinute());

    int dayStart;
    int dayEnd;
    int preStart;
    int preEnd;
    int regularStart;
    int regularEnd;
    int afterStart;
    int afterEnd;

    if (isDst)
    {
        dayStart = ToMinute(9, 0);
        dayEnd = ToMinute(17, 0);

        preStart = ToMinute(17, 0);
        preEnd = ToMinute(22, 30);

        regularStart = ToMinute(22, 30);
        regularEnd = ToMinute(5, 0);

        afterStart = ToMinute(5, 0);
        afterEnd = ToMinute(9, 0);
    }
    else
    {
        dayStart = ToMinute(10, 0);
        dayEnd = ToMinute(18, 0);

        preStart = ToMinute(18, 0);
        preEnd = ToMinute(23, 30);

        regularStart = ToMinute(23, 30);
        regularEnd = ToMinute(6, 0);

        afterStart = ToMinute(6, 0);
        afterEnd = ToMinute(10, 0);
    }

    if (IsInTimeRange(nowMin, dayStart, dayEnd))
        return US_SESSION_DAY;

    if (IsInTimeRange(nowMin, preStart, preEnd))
        return US_SESSION_PRE;

    if (IsInTimeRange(nowMin, regularStart, regularEnd))
        return US_SESSION_REGULAR;

    if (IsInTimeRange(nowMin, afterStart, afterEnd))
        return US_SESSION_AFTER;

    return US_SESSION_CLOSED;
}

BOOL CMarketTimeManager::IsUSDaySession(const CTime& serverKst)
{
    return GetUSMarketSession(serverKst) == US_SESSION_DAY;
}

CString CMarketTimeManager::GetUSMarketSessionName(US_MARKET_SESSION session)
{
    switch (session)
    {
    case US_SESSION_DAY:
        return _T("ÁÖ°£°Å·¡");

    case US_SESSION_PRE:
        return _T("ÇÁ¸®¸¶ÄÏ");

    case US_SESSION_REGULAR:
        return _T("Á¤±ÔÀå");

    case US_SESSION_AFTER:
        return _T("¾ÖÇÁÅÍÀå");

    default:
        return _T("Àå¿Ü");
    }
}

CTime CMarketTimeManager::GetServerKoreanTime()
{
    CTime localNow = CTime::GetCurrentTime();

    CString slog;
    CInternetSession session;
    CHttpFile* pHttpFile = nullptr;

    try
    {
        session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, VERIFIED_TIME_TIMEOUT_MS);
        session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, VERIFIED_TIME_TIMEOUT_MS);

        pHttpFile = (CHttpFile*)session.OpenURL(
            VERIFIED_TIME_URL,
            1,
            INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE);

        if (pHttpFile == nullptr)
        {
            slog.Format("[IMAXContainer][ERR][%s]<%d> pHttpFile is null  err[%d]", __FUNCTION__, __LINE__, GetLastError());
            OutputDebugString(slog);
            return localNow;
        }

        SYSTEMTIME st = {};
        DWORD dwSize = sizeof(st);
        BOOL bOk = pHttpFile->QueryInfo(HTTP_QUERY_DATE | HTTP_QUERY_FLAG_SYSTEMTIME, &st, &dwSize);

        pHttpFile->Close();
        delete pHttpFile;
        session.Close();

        if (!bOk)
        {
            slog.Format("[IMAXContainer][ERR][%s]<%d> QueryInfo is not bOk  err[%d]", __FUNCTION__, __LINE__, GetLastError());
            OutputDebugString(slog);
            return localNow;
        }

        CTime gmtTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        return gmtTime + CTimeSpan(0, 9, 0, 0);  // UTC -> KST
    }
    catch (CInternetException* pEx)
    {
        pEx->Delete();
        if (pHttpFile)
            delete pHttpFile;

        slog.Format("[IMAXContainer][ERR][%s]<%d> catch  err[%d]", __FUNCTION__, __LINE__, GetLastError());
        OutputDebugString(slog);

        return localNow;
    }
}