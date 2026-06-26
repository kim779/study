# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

IBK증권 OCX 커넥터(`ibkconnector.ocx`, GUID `{CDADD338-C7AB-4977-B65D-8E988B5958E3}`)를 PyQt5 `QAxWidget`으로 제어하는 테스트/개발 프로그램. Windows 전용이며, COM 컨트롤이 레지스트리에 등록되어 있어야 실행 가능.

## Commands

```bash
# 실행
python test_ocx.py

# 단독 실행 파일 빌드 (PyInstaller)
pyinstaller test_ocx.spec

# 또는 spec 없이 빌드
pyinstaller --onefile --noconsole test_ocx.py
```

빌드 결과물: `dist/test_ocx.exe`

## Architecture

### 핵심 구조 (`test_ocx.py`)

- **`TestWindow(QMainWindow)`** — 단일 파일, 단일 클래스 구성. UI 빌더 메서드(`_build_*_group`)와 이벤트 핸들러(`_on_*`, `_evt_*`)로 분리됨.
- **OCX 연동** — `QAxWidget.dynamicCall()`로 OCX 메서드 호출, `ocx.OnXxx.connect()`로 이벤트 수신.
- **이진 데이터 파싱** — `OnRecvData` 콜백의 `dptr`(포인터)를 `ctypes.string_at(addr & 0xFFFFFFFF, size)`로 즉시 복사 후 `ctypes.Structure`(`_pack_=1`)로 해석. 문자열 인코딩은 CP949.

### TR 키 상수 매핑

| 상수 | 값 | 용도 |
|------|----|------|
| TK_TR1001 | 1 | 주식 시세 |
| TK_TR1201 | 4 | 주식 주문 |
| TK_TR1211 | 7 | 주식 체결조회 |
| TK_TR1221 | 8 | 주식 잔고조회 |
| TK_TR3001 | 14 | 선물옵션 시세 |
| TK_TR3201 | 17 | 선물옵션 주문 |
| TK_TR3211 | 19 | 선물옵션 체결조회 |
| TK_TR3221 | 20 | 선물옵션 잔고조회 |

TR 키는 `Common.h`와 정확히 일치해야 함.

### 바이너리 레이아웃 규칙

- 응답 패킷 헤더: `acno[11] + nrec[4]` = 15바이트 고정
- 그리드 레코드는 TR별 고정 크기(GRID): TR3001=149, TR1221=194, TR3211=179, TR1211=222
- 다음 페이지가 있으면 `b_next=True` + `nkey` 값으로 동일 TR 재호출

### 실시간 데이터 (`OnRealData`)

탭 구분 포맷: `종목코드\t필드번호\t값\t필드번호\t값\t...`
- 주식 실시간 코드에 `A` prefix 붙어 옴 → `lstrip('A')` 후 비교
- 필드 034(체결시간) 있으면 체결 데이터, 040(호가시간) 있으면 호가 데이터

## 주의사항

- `ibkconnector.ocx`가 레지스트리에 등록된 PC에서만 실행됨
- 서버 IP 기본값: `211.255.204.75:15201`
- `GetAccounts()` 반환 포맷: `계좌번호\t계좌명\n...` (계좌번호는 11자리로 슬라이싱)
- 한글 인코딩 처리 시 Python 대신 `iconv` 사용 (GNU libiconv 1.18 설치됨)
