"""
daily_candles(ticks.db)에 쌓인 일봉으로 이동평균(5/20일) 크로스오버 전략을 검증한다.
pandas 없이 표준 라이브러리(sqlite3 + list)만 사용.

사용법:
    python backtest.py [종목코드]   (기본값 005930)
"""
import sqlite3
import os
import sys

DB_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ticks.db")


def load_daily(conn, code, unit=1, dindex=1):
    cur = conn.execute(
        "SELECT date, open, high, low, close, volume FROM daily_candles "
        "WHERE code = ? AND unit = ? AND dindex = ? ORDER BY date",
        (code, unit, dindex)
    )
    return [
        {"date": r[0], "open": r[1], "high": r[2], "low": r[3], "close": r[4], "volume": r[5]}
        for r in cur.fetchall()
    ]


def sma(values, window):
    # values와 같은 길이의 리스트를 반환. 아직 window만큼 데이터가 안 쌓인 구간은 None.
    out = [None] * len(values)
    total = 0
    for i, v in enumerate(values):
        total += v
        if i >= window:
            total -= values[i - window]
        if i >= window - 1:
            out[i] = total / window
    return out


def cross_signals(short_ma, long_ma):
    # 골드크로스(단기선이 장기선을 상향돌파) -> BUY, 데드크로스(하향돌파) -> SELL
    signals = [None] * len(short_ma)
    for i in range(1, len(short_ma)):
        s0, l0, s1, l1 = short_ma[i - 1], long_ma[i - 1], short_ma[i], long_ma[i]
        if None in (s0, l0, s1, l1):
            continue
        if s0 <= l0 and s1 > l1:
            signals[i] = "BUY"
        elif s0 >= l0 and s1 < l1:
            signals[i] = "SELL"
    return signals


def simulate(rows, signals, regime_ok=None):
    # 신호가 뜬 날의 '다음날 시가'에 체결된다고 가정한다.
    # regime_ok가 주어지면 BUY만 그날 조건이 True일 때 채택하고(SELL/청산은 항상 채택),
    # 이미 보유 중이면 새 BUY 신호는 무시한다(중복 진입 방지).
    # 마지막 날(오늘)의 신호는 다음날 시가가 아직 없어 체결 시뮬레이션에서 제외되므로,
    # 진행 중인 포지션(있다면)을 함께 반환해서 today_signal()이 "내일 뭘 할지" 판단할 수 있게 한다.
    trades = []
    position = None
    for i in range(len(rows) - 1):  # 마지막 날은 다음날 시가가 없어 체결 불가
        sig = signals[i]
        if sig is None:
            continue
        if sig == "BUY" and regime_ok is not None and not regime_ok[i]:
            continue
        next_open = rows[i + 1]["open"]
        if sig == "BUY" and position is None:
            position = {"entry_date": rows[i + 1]["date"], "entry_price": next_open}
        elif sig == "SELL" and position is not None:
            ret = (next_open - position["entry_price"]) / position["entry_price"]
            trades.append({**position, "exit_date": rows[i + 1]["date"],
                            "exit_price": next_open, "return": ret})
            position = None
    return trades, position


def today_signal(rows, signals, position, regime_ok=None):
    # 마지막 날(오늘) 종가 기준 신호를 현재 보유 상태와 맞춰봐서 "내일 뭘 할지"를 판단한다.
    # 보유중이 아닌데 BUY 신호(+레짐 필터 통과)면 매수 후보, 보유중인데 SELL 신호면 매도 후보.
    i = len(rows) - 1
    sig = signals[i]
    if position is None:
        if sig == "BUY" and (regime_ok is None or regime_ok[i]):
            return "BUY"
    else:
        if sig == "SELL":
            return "SELL"
    return None


def summarize(trades, label):
    if not trades:
        print(f"[{label}] 거래 없음")
        return
    n = len(trades)
    wins = [t for t in trades if t["return"] > 0]
    cum = 1.0
    for t in trades:
        cum *= (1 + t["return"])
    avg_return = sum(t["return"] for t in trades) / n
    print(f"[{label}] 거래횟수={n} 승률={len(wins) / n * 100:.1f}% "
          f"평균수익률={avg_return * 100:.2f}% 누적수익률={(cum - 1) * 100:.2f}%")
    for t in trades:
        print(f"    {t['entry_date']} 매수 {t['entry_price']} -> "
              f"{t['exit_date']} 매도 {t['exit_price']} ({t['return'] * 100:+.2f}%)")


def main():
    code = sys.argv[1] if len(sys.argv) > 1 else "005930"
    conn = sqlite3.connect(DB_PATH)

    stock = load_daily(conn, code, unit=1, dindex=1)
    if len(stock) < 25:
        print(f"{code} 데이터가 부족합니다(레코드 {len(stock)}건). "
              f"차트조회(대상=종목)로 일봉을 먼저 모아주세요.")
        return

    closes = [r["close"] for r in stock]
    signals = cross_signals(sma(closes, 5), sma(closes, 20))

    trades, position = simulate(stock, signals)
    summarize(trades, f"{code} 필터없음")
    sig = today_signal(stock, signals, position)
    print(f"    오늘({stock[-1]['date']}) 신호: {sig or '없음'}"
          f" (현재 {'보유중' if position else '미보유'})")

    kospi = load_daily(conn, "001", unit=2, dindex=1)
    if len(kospi) < 25:
        print("코스피(001) 업종 데이터가 없어 레짐 필터 비교는 건너뜁니다. "
              "차트조회에서 대상=업종, 코드=001로 먼저 모아주세요.")
        conn.close()
        return

    kospi_closes = [r["close"] for r in kospi]
    kospi_ma = sma(kospi_closes, 20)
    kospi_regime_by_date = {
        r["date"]: (kospi_ma[i] is not None and kospi_closes[i] > kospi_ma[i])
        for i, r in enumerate(kospi)
    }
    regime_ok = [kospi_regime_by_date.get(r["date"], False) for r in stock]

    trades, position = simulate(stock, signals, regime_ok=regime_ok)
    summarize(trades, f"{code} 코스피20일선 필터")
    sig = today_signal(stock, signals, position, regime_ok=regime_ok)
    print(f"    오늘({stock[-1]['date']}) 신호: {sig or '없음'}"
          f" (현재 {'보유중' if position else '미보유'})")

    conn.close()


if __name__ == "__main__":
    main()
