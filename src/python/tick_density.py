"""
종목별/일자별 틱 밀도(분당 평균 틱수) 계산.
이동평균 윈도우를 "몇 틱"이 아니라 "몇 분"에 해당하는지 가늠할 때 참고용.

사용법: python tick_density.py [종목코드]
        (종목코드 생략 시 전체 종목)
"""
import sqlite3
import sys
import os
from datetime import datetime

DB_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ticks.db")


def parse_trade_time(s):
    if len(s) >= 14:
        try:
            return datetime.strptime(s[:14], "%Y%m%d%H%M%S")
        except ValueError:
            return None
    return None


def main(target_code=None):
    conn = sqlite3.connect(DB_PATH)
    if target_code:
        cur = conn.execute("SELECT DISTINCT code FROM ticks WHERE code=?", (target_code,))
    else:
        cur = conn.execute("SELECT DISTINCT code FROM ticks ORDER BY code")
    codes = [r[0] for r in cur.fetchall()]

    print(f"{'종목':10} {'날짜':>10} {'틱수':>8} {'시작':>8} {'끝':>8} {'경과(분)':>10} {'틱/분':>8}")
    for code in codes:
        cur = conn.execute(
            "SELECT trade_time FROM ticks WHERE code=? ORDER BY id", (code,)
        )
        times = [parse_trade_time(r[0]) for r in cur.fetchall()]
        times = [t for t in times if t]
        if not times:
            continue

        by_date = {}
        for t in times:
            by_date.setdefault(t.date(), []).append(t)

        for d, dts in sorted(by_date.items()):
            if len(dts) < 2:
                continue
            span_min = (dts[-1] - dts[0]).total_seconds() / 60
            if span_min <= 0:
                continue
            tpm = len(dts) / span_min
            print(f"{code:10} {d.isoformat():>10} {len(dts):>8} "
                  f"{dts[0].strftime('%H:%M'):>8} {dts[-1].strftime('%H:%M'):>8} "
                  f"{span_min:>10.1f} {tpm:>8.2f}")

    conn.close()


if __name__ == "__main__":
    code_arg = sys.argv[1] if len(sys.argv) > 1 else None
    main(code_arg)
