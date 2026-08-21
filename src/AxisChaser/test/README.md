# AxisChaser 성능 개선 테스트 실행 가이드


## 목차

- [빠른 시작 (Quick Start)](#빠른-시작-quick-start)
  - [1. 빌드](#1-빌드)
  - [2. 테스트 실행](#2-테스트-실행)
  - [3. 결과 기록](#3-결과-기록)
- [전체 문서 구조](#전체-문서-구조)
- [테스트 개요](#테스트-개요)
  - [목표](#목표)
  - [시간 예상](#시간-예상)
- [필수 조건](#필수-조건)
  - [소프트웨어](#소프트웨어)
  - [하드웨어](#하드웨어)
  - [환경](#환경)
- [단계별 실행 절차](#단계별-실행-절차)
  - [Step 1: 준비 단계 (5분)](#step-1-준비-단계-5분)
  - [Step 2: 빌드 (15분)](#step-2-빌드-15분)
  - [Step 3: 회귀 테스트 (Phase 1, 30분)](#step-3-회귀-테스트-phase-1-30분)
  - [Step 4: 성능 테스트 (Phase 2-5, 130분)](#step-4-성능-테스트-phase-2-5-130분)
  - [Step 5: 분석 및 보고 (30분)](#step-5-분석-및-보고-30분)
- [체크리스트](#체크리스트)
  - [빌드 전](#빌드-전)
  - [빌드 단계](#빌드-단계)
  - [테스트 준비](#테스트-준비)
  - [Phase 1 (회귀)](#phase-1-회귀)
  - [Phase 2-5 (성능)](#phase-2-5-성능)
  - [최종](#최종)
- [주요 참고사항](#주요-참고사항)
  - [1. 인코딩 주의](#1-인코딩-주의)
  - [2. AXIS 프로세스 없는 경우](#2-axis-프로세스-없는-경우)
  - [3. 메모리 누수 확인](#3-메모리-누수-확인)
  - [4. Release 빌드 필수](#4-release-빌드-필수)
  - [5. 크래시 분석](#5-크래시-분석)
- [주의사항](#주의사항)
- [문제 해결](#문제-해결)
  - [Q: RichLib 빌드 오류](#q-richlib-빌드-오류)
  - [Q: AxisChaser 링크 오류](#q-axischaser-링크-오류)
  - [Q: axisfire.h not found](#q-axisfireh-not-found)
  - [Q: 데이터 송신 안 됨](#q-데이터-송신-안-됨)
- [다음 단계](#다음-단계)
- [참고 문서](#참고-문서)
- [예상 결과](#예상-결과)
  - [Pass 시나리오 (모든 Phase PASS)](#pass-시나리오-모든-phase-pass)
  - [Fail 시나리오](#fail-시나리오)
- [최종 확인](#최종-확인)

---

**작성일**: 2026-07-30  
**최종 검증**: 모든 5가지 성능 개선사항 구현 확인됨  
**상태**: 테스트 준비 완료

---

## 빠른 시작 (Quick Start)

### 1. 빌드

```bash
# RichLib 먼저 빌드 (필수)
cd d:\src\IBKS\src\AxisChaser\RichLib
msbuild RichLib.vcxproj /p:Configuration=Release /p:Platform=x64

# AxisChaser 빌드
cd d:\src\IBKS\src\AxisChaser
msbuild AxisChaser.vcxproj /p:Configuration=Release /p:Platform=x64

# 빌드 결과 확인
dir Release\AxisChaser.exe
```

### 2. 테스트 실행

```bash
# 1단계: 회귀 테스트 (기존 기능 확인) - 30분
# 수동으로 AxisChaser.exe 실행 후 기본 기능 확인
# 참고: test/PerfTestScenario.md Phase 1

# 2단계: 성능 테스트 (Phase 2-5) - 120분
# AXIS 프로세스 또는 TestSender.exe로 데이터 송신
# 작업 관리자에서 CPU/메모리 모니터링
```

### 3. 결과 기록

```bash
# TestResultTemplate.md를 사용하여 결과 문서화
copy test\TestResultTemplate.md test\TestResult_[DATE].md
# [DATE]는 YYYYMMDD 형식 (예: TestResult_20260730.md)
```

---

## 전체 문서 구조

```
test/
├── README.md                          (이 파일)
├── BuildInstructions.md               ← 빌드 지침 (먼저 읽기)
├── PerfTestScenario.md                ← 테스트 시나리오 (5개 Phase, 12개 TC)
├── ImplementationVerification.md      ← 구현 검증 (5가지 개선사항 확인)
├── PerfTestHelper.cpp                 ← 성능 측정 헬퍼 코드
├── TestResultTemplate.md              ← 테스트 결과 기록 템플릿
├── README.md                          (이 파일)
└── TestResult_[DATE].md               ← 테스트 결과 (작성 예정)
```

---

## 테스트 개요

### 목표

AxisChaser 성능 개선 5가지 사항의 기능/성능 검증:

1. **WM_VSCROLL 배치 처리** (line 1085)
   - 렌더링 오버헤드 80-90% 감소
   - CPU 60-90% → 20-40%

2. **OutputDebugString 조건부** (line 788-791)
   - Release 빌드에서 Debug 메시지 제거

3. **RichEdit Truncate (50MB)** (line 803-807)
   - 메모리 사용량 100-200MB 안정화

4. **Send/Receive 큐 상한** (line 1042-1047)
   - RTM: 200건, SND/RCV: 500건
   - 메모리 누수 방지

5. **메모리 누수 방지**
   - 큐 상한 + Truncate + CQue 소멸자

### 시간 예상

| 단계 | 내용 | 예상 시간 |
|------|------|----------|
| Phase 1 | 회귀 테스트 (기존 기능) | 30분 |
| Phase 2 | WM_VSCROLL 배치 | 20분 |
| Phase 3 | Truncate 검증 | 30분 |
| Phase 4 | 큐 상한 검증 | 20분 |
| Phase 5 | Stress 테스트 | 60분 |
| **총합** | | **160분 (2.7시간)** |

---

## 필수 조건

### 소프트웨어

- [x] Visual Studio 2019+ (v142 toolset)
- [x] MFC (Static/Shared DLL)
- [x] Windows SDK 10.0+
- [x] Git (원격 저장소 동기화용, 옵션)

### 하드웨어

- [x] 프로세서: Intel Core i5 이상 (성능 측정 정확도 향상)
- [x] 메모리: 4GB 이상
- [x] 디스크: 500MB 이상 (빌드 산출물)

### 환경

- [x] Windows 10 LTSC 2019 이상
- [x] AXIS 프로세스 또는 테스트 데이터 송신 프로그램
- [x] 관리자 권한 (Event Viewer 접근)

---

## 단계별 실행 절차

### Step 1: 준비 단계 (5분)

```bash
# 1. 문서 읽기
cat test/BuildInstructions.md
cat test/PerfTestScenario.md

# 2. 빌드 환경 확인
msbuild /version

# 3. 테스트 폴더 생성
mkdir d:\src\IBKS\err\test
```

### Step 2: 빌드 (15분)

```bash
# 자세한 내용은 test/BuildInstructions.md 참고
cd d:\src\IBKS\src\AxisChaser\RichLib
msbuild RichLib.vcxproj /p:Configuration=Release /p:Platform=x64

cd ..\
msbuild AxisChaser.vcxproj /p:Configuration=Release /p:Platform=x64
```

### Step 3: 회귀 테스트 (Phase 1, 30분)

**참고**: test/PerfTestScenario.md Phase 1 참조

1. AxisChaser.exe 실행
2. AXIS 프로세스에서 기본 데이터 송신
3. UI 확인: 텍스트/색상/폰트 정상 표시
4. 기능 확인: 필터, 옵션, 로그 저장
5. TestResultTemplate.md에 결과 기록

### Step 4: 성능 테스트 (Phase 2-5, 130분)

**참고**: test/PerfTestScenario.md Phase 2-5 참조

1. DebugView.exe 실행 (Debug 출력 캡처)
2. 작업 관리자 Performance 탭 열기
3. AXIS/TestSender.exe에서 대량 데이터 송신
   - TC-2.1: RTM 50건/초, 5초
   - TC-2.2: RTM 200건/초, 10초
   - TC-3.1: Truncate 50MB 도달
   - TC-4.1/4.2: 큐 상한 검증
   - TC-5.1: Stress 10분 이상
4. CPU/메모리 수치 기록
5. TestResultTemplate.md에 결과 기록

### Step 5: 분석 및 보고 (30분)

1. 수집한 데이터 분석
2. Pass/Fail 판정
3. 리스크 재평가
4. 권장사항 정리
5. 최종 보고서 작성

---

## 체크리스트

### 빌드 전

- [ ] Visual Studio 2019+ 설치
- [ ] MFC 설치
- [ ] Windows SDK 10.0+ 설치
- [ ] git 설치 (옵션)

### 빌드 단계

- [ ] RichLib.vcxproj Release x64 빌드 성공
- [ ] RichLib/Release/RichLib.lib 생성 확인
- [ ] AxisChaser.vcxproj Release x64 빌드 성공
- [ ] Release/AxisChaser.exe 생성 확인
- [ ] 빌드 경고 검토 (무시 가능한지 확인)

### 테스트 준비

- [ ] DebugView.exe 다운로드/설치
- [ ] 작업 관리자 준비
- [ ] AXIS 프로세스 또는 TestSender.exe 준비
- [ ] 로그 폴더 생성 (d:\src\IBKS\err\test\)
- [ ] TestResultTemplate.md 복사

### Phase 1 (회귀)

- [ ] TC-1.1: 기본 데이터 표시 PASS
- [ ] TC-1.2: 옵션 다이얼로그 PASS
- [ ] TC-1.3: 로그 파일 저장 PASS

### Phase 2-5 (성능)

- [ ] TC-2.1: CPU 개선율 기록
- [ ] TC-2.2: 대량 배치 성능 기록
- [ ] TC-3.1: Truncate 지연 시간 < 500ms
- [ ] TC-3.2: 스크롤 위치 손실 확인
- [ ] TC-4.1/4.2: 큐 드롭 로그 확인
- [ ] TC-5.1: 10분 Stress 완료

### 최종

- [ ] TestResult_[DATE].md 작성 완료
- [ ] 모든 Phase PASS 또는 FAIL 판정
- [ ] 개선 효과 정량화
- [ ] 리스크 재평가 완료

---

## 주요 참고사항

### 1. 인코딩 주의

**중요**: ChildView.cpp 수정 필요 시 CP949 → UTF-8 → CP949 변환 필수

```bash
# 수정 시 절차
iconv -f CP949 -t UTF-8 "ChildView.cpp" > "ChildView.cpp.utf8.tmp"
# [Edit 도구로 수정]
iconv -f UTF-8 -t CP949 "ChildView.cpp.utf8.tmp" > "ChildView.cpp"
rm "ChildView.cpp.utf8.tmp"
```

### 2. AXIS 프로세스 없는 경우

TestSender.exe를 별도로 작성하여 데이터 송신 가능. 참고: test/PerfTestHelper.cpp

### 3. 메모리 누수 확인

CQue 소멸자에서 delete[] m_pBytes 호출 확인됨 (ChildView.h:26)
```cpp
virtual ~CQue()	{ if (m_pBytes)	delete [] m_pBytes; }
```

### 4. Release 빌드 필수

성능 테스트는 Release 빌드에서만 의미 있음 (Debug는 오버헤드 높음)

### 5. 크래시 분석

Event Viewer에서 크래시 로그 확인:
```
Event Viewer > Windows Logs > System > [Error 필터]
```

---

## 주의사항

1. **테스트 중 문제 발생 시**
   - Phase 1 실패 → 기본 기능 손상, 개선사항 롤백 검토
   - Phase 2-5 실패 → 해당 Phase의 구현 재검토

2. **데이터 손실은 예상됨**
   - 큐 상한 도달 시 데이터 드롭
   - 로그용 도구이므로 일부 손실 허용

3. **UI 프리징은 불가피**
   - Truncate 중 100-500ms 프리징 가능
   - SetRedraw() 사용 시 더 심할 수 있음

4. **메모리 누수 위험**
   - 큐 상한 미달 상태에서는 누수 가능성 있음
   - 테스트 후 메모리 추이 관찰 필수

---

## 문제 해결

### Q: RichLib 빌드 오류

**A**: 다음 순서로 확인
1. Visual Studio 버전 확인 (2019+)
2. RichLib/RichLib.vcxproj 열기
3. x64 Platform 선택
4. Clean → Build 순서로 다시 빌드

### Q: AxisChaser 링크 오류

**A**: RichLib.lib 경로 확인
```bash
dir d:\src\IBKS\src\AxisChaser\RichLib\Release\RichLib.lib
```
없으면 RichLib 재빌드

### Q: axisfire.h not found

**A**: 헤더 경로 확인
```bash
dir d:\src\IBKS\src\H\axisfire.h
```
없으면 프로젝트 속성 > C/C++ > Additional Include Directories에 `..\H` 추가

### Q: 데이터 송신 안 됨

**A**: AXIS 프로세스 상태 확인 또는 TestSender.exe 사용

---

## 다음 단계

테스트 완료 후:

1. **버그 수정** (발견 시)
2. **문서 갱신** (Architecture.md, KnowledgeBase.md)
3. **리팩터링** (구조 개선 - Optional)
4. **배포** (Release 빌드)

---

## 참고 문서

| 문서 | 내용 |
|------|------|
| [BuildInstructions.md](BuildInstructions.md) | 빌드 지침 (필수 읽기) |
| [PerfTestScenario.md](PerfTestScenario.md) | 테스트 시나리오 상세 |
| [ImplementationVerification.md](ImplementationVerification.md) | 구현 검증 결과 |
| [PerfTestHelper.cpp](PerfTestHelper.cpp) | 성능 측정 코드 |
| [TestResultTemplate.md](TestResultTemplate.md) | 결과 기록 템플릿 |
| [../docs/KnowledgeBase.md](../docs/KnowledgeBase.md) | 설계 의도 |
| [../docs/Architecture.md](../docs/Architecture.md) | 프로젝트 구조 |

---

## 예상 결과

### Pass 시나리오 (모든 Phase PASS)

✓ 모든 기능 정상 작동
✓ CPU 사용률 50% 이상 개선
✓ UI 응답 시간 < 100ms
✓ 메모리 안정화 (100-200MB)
✓ 10분 Stress 테스트 완료

### Fail 시나리오

✗ Phase 1 실패 → 기본 기능 손상, 긴급 조치
✗ Phase 2-5 실패 → 해당 개선사항 검토 필요

---

## 최종 확인

**구현 상태**: ✓ 5/5 항목 구현 확인됨  
**빌드 상태**: 준비 완료 (실행 대기)  
**테스트 상태**: 준비 완료 (시나리오 문서 작성 완료)  
**문서 상태**: 완성 (KnowledgeBase.md, Todo.md, test/ 갱신)

---

**테스트 실행 권한**: Ready  
**최종 검증자**: Testing Agent (2026-07-30)  
**다음 단계**: Phase 1 회귀 테스트 실행 (수동 AXIS 프로세스 필요)
