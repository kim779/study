#pragma once
class CMarketTimeManager
{
    public:
        enum US_MARKET_SESSION
        {
            US_SESSION_CLOSED = 0,
            US_SESSION_DAY,       // 주간거래
            US_SESSION_PRE,       // 프리마켓
            US_SESSION_REGULAR,   // 정규장
            US_SESSION_AFTER      // 애프터장
        };

    public:
        CMarketTimeManager();
        virtual ~CMarketTimeManager();

    public:
        US_MARKET_SESSION GetUSMarketSession(const CTime& serverKst);
        BOOL IsUSDST(const CTime& serverKst);
        BOOL IsUSDaySession(const CTime& serverKst);

        CString GetUSMarketSessionName(US_MARKET_SESSION session);

        CTime GetServerKoreanTime();   // verified time via HTTP Date header, falls back to local PC time

    private:
        int ToMinute(int hour, int minute) const;
        BOOL IsInTimeRange(int nowMin, int startMin, int endMin) const;

        int GetNthSunday(int year, int month, int nth) const;
};

