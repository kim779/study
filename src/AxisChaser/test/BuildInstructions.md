# AxisChaser 테스트 빌드 지침


## 목차

- [빌드 전 확인사항](#빌드-전-확인사항)
  - [1. 의존성 확인](#1-의존성-확인)
  - [2. 빌드 순서](#2-빌드-순서)
  - [3. Visual Studio 설정](#3-visual-studio-설정)
  - [4. 코드 인코딩 주의](#4-코드-인코딩-주의)
- [빌드 명령어](#빌드-명령어)
  - [Command Line 빌드 (MSBuild)](#command-line-빌드-msbuild)
  - [Visual Studio IDE 빌드](#visual-studio-ide-빌드)
- [빌드 오류 해결](#빌드-오류-해결)
  - [오류 1: "RichLib.lib not found"](#오류-1-richliblib-not-found)
  - [오류 2: "axisfire.h not found" 또는 "axis.h not found"](#오류-2-axisfireh-not-found-또는-axish-not-found)
  - [오류 3: "Cannot open include file: 'afxwin.h'"](#오류-3-cannot-open-include-file-afxwinh)
  - [오류 4: "LNK1104: cannot open file 'RichLib.lib'"](#오류-4-lnk1104-cannot-open-file-richliblib)
- [테스트 실행 전 체크리스트](#테스트-실행-전-체크리스트)
  - [빌드 완료 후](#빌드-완료-후)
  - [실행 전 준비](#실행-전-준비)
  - [실행 명령어](#실행-명령어)
- [성능 프로파일링 설정](#성능-프로파일링-설정)
  - [1. Debug 출력 캡처](#1-debug-출력-캡처)
  - [2. Performance Monitoring (Windows Task Manager)](#2-performance-monitoring-windows-task-manager)
  - [3. Event Viewer (크래시 로그)](#3-event-viewer-크래시-로그)
- [테스트 데이터 생성기](#테스트-데이터-생성기)
  - [C++ 테스트 프로그램 (TestSender.exe)](#c-테스트-프로그램-testsenderexe)
- [빌드 성공 확인](#빌드-성공-확인)
- [주의사항](#주의사항)
- [관련 문서](#관련-문서)

---

**작성일**: 2026-07-30  
**목적**: 성능 개선 테스트를 위한 빌드 및 준비 사항

---

## 빌드 전 확인사항

### 1. 의존성 확인

AxisChaser는 다음 두 개의 프로젝트 파일을 포함합니다:

| 프로젝트 | 경로 | 역할 | 필수여부 |
|---------|------|------|---------|
| RichLib | `RichLib/RichLib.vcxproj` | RTF 렌더링 라이브러리 | **필수** (먼저 빌드) |
| AxisChaser | `AxisChaser.vcxproj` | 메인 실행 파일 | **필수** |

### 2. 빌드 순서

**중요**: RichLib을 먼저 빌드해야 합니다. AxisChaser는 `RichLib.lib`에 링크합니다.

```
Step 1: RichLib.vcxproj 빌드 (Release x64)
        └─> 산출물: RichLib/Release/RichLib.lib

Step 2: AxisChaser.vcxproj 빌드 (Release x64)
        └─> 링크 입력: ../RichLib/Release/RichLib.lib
        └─> 산출물: Release/AxisChaser.exe
```

### 3. Visual Studio 설정

**필수 워크로드**:
- Desktop development with C++
- MFC support (ATL/MFC)

**권장 버전**:
- Visual Studio 2019 이상 (프로젝트가 v142 또는 v143 toolset 사용)

### 4. 코드 인코딩 주의

**중요**: ChildView.cpp 및 기타 소스 파일은 **CP949 (Windows 한글 코드페이지)**로 인코딩되어 있습니다.

테스트를 위해 코드를 수정할 필요가 있으면:

```bash
# Step 1: CP949 to UTF-8 변환
iconv -f CP949 -t UTF-8 "ChildView.cpp" > "ChildView.cpp.utf8.tmp"

# Step 2: UTF-8 파일 수정 (Edit 도구 사용)
# ... modify ChildView.cpp.utf8.tmp ...

# Step 3: UTF-8 to CP949 역변환
iconv -f UTF-8 -t CP949 "ChildView.cpp.utf8.tmp" > "ChildView.cpp"

# Step 4: 임시 파일 삭제
rm "ChildView.cpp.utf8.tmp"

# Step 5: 변경 줄 수 검증 (몇 줄 고치려던 건데 수백 줄이 나오면 실패)
git diff --stat -- "ChildView.cpp"
```

---

## 빌드 명령어

### Command Line 빌드 (MSBuild)

```batch
:: Step 1: RichLib 빌드
cd d:\src\IBKS\src\AxisChaser\RichLib
msbuild RichLib.vcxproj /p:Configuration=Release /p:Platform=x64

:: Step 2: AxisChaser 빌드
cd d:\src\IBKS\src\AxisChaser
msbuild AxisChaser.vcxproj /p:Configuration=Release /p:Platform=x64

:: Step 3: 빌드 결과 확인
dir Release\AxisChaser.exe
```

### Visual Studio IDE 빌드

1. Visual Studio 2019 열기
2. `AxisChaser.sln` 열기 (또는 개별 `.vcxproj`)
3. **솔루션 구성**: Release | x64 선택
4. **프로젝트 빌드 순서**:
   - RichLib 먼저 빌드 (우클릭 > Build)
   - AxisChaser 빌드 (우클릭 > Build)
5. **산출물 확인**:
   - `Release\AxisChaser.exe` 생성 확인

---

## 빌드 오류 해결

### 오류 1: "RichLib.lib not found"

**원인**: RichLib 빌드 안 됨

**해결**:
```bash
cd d:\src\IBKS\src\AxisChaser\RichLib
msbuild RichLib.vcxproj /p:Configuration=Release /p:Platform=x64 /verbosity:detailed
```

### 오류 2: "axisfire.h not found" 또는 "axis.h not found"

**원인**: `d:\src\IBKS\src\H\` 헤더 경로 문제

**해결**: AxisChaser.vcxproj 확인
```xml
<ItemGroup>
  <ClInclude Include="..\H\axisfire.h" />
  <ClInclude Include="..\H\axis.h" />
</ItemGroup>
```

경로가 올바른지 확인하고, 없으면 프로젝트 속성 > C/C++ > General > Additional Include Directories에 추가:
```
..\H
```

### 오류 3: "Cannot open include file: 'afxwin.h'"

**원인**: MFC 헤더 누락

**해결**: Visual Studio에서 MFC 지원 설정 확인
- 프로젝트 속성 > General > Use of MFC: **Use MFC in a Static Library** 또는 **Use MFC in a Shared DLL**

### 오류 4: "LNK1104: cannot open file 'RichLib.lib'"

**원인**: RichLib.lib 링크 경로 오류

**해결**: AxisChaser.vcxproj에서 확인
```xml
<ItemGroup>
  <ProjectReference Include="..\RichLib\RichLib.vcxproj">
    <Project>{...GUID...}</Project>
  </ProjectReference>
</ItemGroup>
```

또는 직접 lib 경로 지정:
```xml
<AdditionalLibraryDirectories>
  ..\RichLib\Release;%(AdditionalLibraryDirectories)
</AdditionalLibraryDirectories>
<AdditionalDependencies>
  RichLib.lib;%(AdditionalDependencies)
</AdditionalDependencies>
```

---

## 테스트 실행 전 체크리스트

### 빌드 완료 후

- [ ] `Release\AxisChaser.exe` 생성됨
- [ ] `RichLib\Release\RichLib.lib` 생성됨
- [ ] 빌드 경고 없음 (또는 무시해도 되는 경고만)
- [ ] 링크 오류 없음

### 실행 전 준비

- [ ] Visual Studio/DebugView 실행 (Debug 출력 캡처용)
- [ ] 작업 관리자 열기 (Performance 탭)
- [ ] AXIS 프로세스 준비 또는 테스트 데이터 송신 프로그램 준비
- [ ] 테스트 로그 저장 폴더 준비: `d:\src\IBKS\err\test\`

### 실행 명령어

```bash
# 명령행 실행
cd d:\src\IBKS\src\AxisChaser\Release
AxisChaser.exe -c "[WINDOW_CLASS_NAME]" -r "AXIS"

# 또는 간단히
d:\src\IBKS\src\AxisChaser\Release\AxisChaser.exe
```

---

## 성능 프로파일링 설정

### 1. Debug 출력 캡처

**Tool**: Sysinternals DebugView 또는 Visual Studio Output Window

```bash
# DebugView 다운로드
https://docs.microsoft.com/en-us/sysinternals/downloads/debugview

# 실행
DebugView.exe
```

### 2. Performance Monitoring (Windows Task Manager)

1. 작업 관리자 > Performance 탭
2. CPU, Memory, Disk 모니터링
3. 테스트 중 CSV 내보내기 (오른쪽 클릭 > Create dump file 또는 수동 기록)

### 3. Event Viewer (크래시 로그)

```
Event Viewer > Windows Logs > System
오류 필터링: Source = "Application Error" or "Windows Error Reporting"
```

---

## 테스트 데이터 생성기

테스트용 AXIS 프로세스가 없으면, 다음과 같이 테스트 데이터를 AxisChaser로 송신할 수 있습니다:

### C++ 테스트 프로그램 (TestSender.exe)

[test/PerfTestHelper.cpp 참고 — 아래 코드를 별도 프로젝트로 컴파일]

```cpp
// 간단 버전: 콘솔 프로그램으로 컴파일
#include <windows.h>
#include <stdio.h>
#include <thread>
#include <chrono>

struct _exeCDSS {
    DWORD flag;
    DWORD len;
};

void SendRTMData(HWND hChaser) {
    for (int i = 0; i < 100; i++) {
        char buffer[2048];
        _exeCDSS* cdss = (_exeCDSS*)buffer;
        cdss->flag = 1;  // RTM
        
        char* data = buffer + sizeof(_exeCDSS);
        sprintf_s(data, 2000, "TEST_%d\tPRICE\t12345.50\tVOLUME\t1000000\n", i);
        cdss->len = strlen(data) + 1;
        
        COPYDATASTRUCT cds;
        cds.dwData = 0;
        cds.cbData = sizeof(_exeCDSS) + cdss->len;
        cds.lpData = buffer;
        
        SendMessage(hChaser, WM_COPYDATA, 0, (LPARAM)&cds);
        Sleep(10);  // 10ms delay
    }
}

int main() {
    HWND hChaser = FindWindow(NULL, "AxisChaser");
    if (!hChaser) {
        printf("AxisChaser not found\n");
        return 1;
    }
    
    printf("Sending test data...\n");
    SendRTMData(hChaser);
    printf("Done\n");
    return 0;
}
```

---

## 빌드 성공 확인

```bash
# Step 1: 실행 파일 생성 확인
file "d:\src\IBKS\src\AxisChaser\Release\AxisChaser.exe"
# 출력: Windows PE x64 executable

# Step 2: 파일 크기 확인 (일반적으로 2-5MB)
ls -lh "d:\src\IBKS\src\AxisChaser\Release\AxisChaser.exe"

# Step 3: 실행 파일 의존성 확인 (Dependency Walker 사용)
# https://www.dependencywalker.com/
# depends.exe "d:\src\IBKS\src\AxisChaser\Release\AxisChaser.exe"
```

---

## 주의사항

1. **x64 플랫폼 필수**: 프로젝트가 x64로 설정되어 있는지 확인
2. **Release 빌드**: 성능 테스트는 Release 빌드로 수행 (Debug는 오버헤드 높음)
3. **인코딩**: CP949 파일 수정 시 iconv 사용 (직접 Edit 금지)
4. **대기 후 재빌드**: 빌드 오류 발생 시, Clean 후 다시 빌드:
   ```bash
   msbuild AxisChaser.vcxproj /t:Clean /p:Configuration=Release /p:Platform=x64
   msbuild AxisChaser.vcxproj /p:Configuration=Release /p:Platform=x64
   ```

---

## 관련 문서

- [PerfTestScenario.md](PerfTestScenario.md) — 테스트 시나리오
- [TestResultTemplate.md](TestResultTemplate.md) — 테스트 결과 기록 템플릿
- [PerfTestHelper.cpp](PerfTestHelper.cpp) — 성능 측정 헬퍼 코드
- [../docs/Dependency.md](../docs/Dependency.md) — 프로젝트 의존성
- [../CLAUDE.md](../CLAUDE.md) — 프로젝트 가이드
