"""
이동평균 교차(MA Crossover) 백테스트
- ticks.db에 저장된 실시간 틱을 이용해 단기/장기 이동평균 교차 시점에
  매수/매도했다면 손익이 어떻게 되었을지 시뮬레이션한다.
- 실제 주문은 하지 않는다 (검증용).

사용법: python backtest_ma_cross.py <종목코드>
"""
import sqlite3
import sys
import os

SHORT_WINDOW = 5
LONG_WINDOW = 20

DB_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ticks.db")


def load_prices(code):
    conn = sqlite3.connect(DB_PATH)
    cur = conn.execute(
        "SELECT trade_time, price FROM ticks WHERE code=? AND price IS NOT NULL ORDER BY id",
        (code,)
    )
    rows = cur.fetchall()
    conn.close()
    return rows


def sma_series(prices, window):
    # prices[i]까지의 window개 평균. 데이터 부족하면 None.
    result = []
    total = 0.0
    for i, p in enumerate(prices):
        total += p
        if i >= window:
            total -= prices[i - window]
        result.append(total / window if i >= window - 1 else None)
    return result


def find_trades(times, prices, short, long_):
    # 골든크로스(단기가 장기를 상향 돌파) 매수, 데드크로스(하향 돌파) 매도로 짝지어
    # 완결된 거래 목록과, 아직 청산 안 된 포지션(있다면)을 반환한다.
    position = None  # None=미보유, else 매수가격
    trades = []  # (buy_time, buy_price, sell_time, sell_price)
    entry = None

    for i in range(1, len(prices)):
        if short[i] is None or long_[i] is None or short[i - 1] is None or long_[i - 1] is None:
            continue

        golden_cross = short[i - 1] <= long_[i - 1] and short[i] > long_[i]
        dead_cross = short[i - 1] >= long_[i - 1] and short[i] < long_[i]

        if position is None and golden_cross:
            position = prices[i]
            entry = (times[i], prices[i])
        elif position is not None and dead_cross:
            trades.append((entry[0], entry[1], times[i], prices[i]))
            position = None
            entry = None

    return trades, entry


def backtest(code):
    rows = load_prices(code)
    if len(rows) < LONG_WINDOW + 1:
        print(f"데이터 부족: {code} 틱 {len(rows)}개 (최소 {LONG_WINDOW + 1}개 필요)")
        return

    times = [r[0] for r in rows]
    prices = [r[1] for r in rows]
    short = sma_series(prices, SHORT_WINDOW)
    long_ = sma_series(prices, LONG_WINDOW)
    trades, entry = find_trades(times, prices, short, long_)

    print(f"종목={code}  틱={len(rows)}개  단기={SHORT_WINDOW} 장기={LONG_WINDOW}")
    print(f"거래횟수={len(trades)}")
    total_pnl = 0
    wins = 0
    for buy_t, buy_p, sell_t, sell_p in trades:
        pnl = sell_p - buy_p
        total_pnl += pnl
        if pnl > 0:
            wins += 1
        print(f"  매수 {buy_t} @ {buy_p}  ->  매도 {sell_t} @ {sell_p}  손익={pnl:+.0f}")
    if trades:
        print(f"총손익={total_pnl:+.0f}  승률={wins * 100 / len(trades):.1f}%")
    if entry is not None:
        print(f"(미청산 보유중: {entry[0]} @ {entry[1]})")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("사용법: python backtest_ma_cross.py <종목코드> [단기윈도우] [장기윈도우]")
        sys.exit(1)
    if len(sys.argv) >= 4:
        SHORT_WINDOW = int(sys.argv[2])
        LONG_WINDOW = int(sys.argv[3])
    backtest(sys.argv[1])
