---
project: ibks
category: build
status: 완료
updated: 2026-08-21
---

# ibks 프로젝트 빌드 가이드


## 목차

- [문서 목적](#문서-목적)
- [1. 빌드 환경 요구사항](#1-빌드-환경-요구사항)
  - [필수 소프트웨어](#필수-소프트웨어)
  - [선택 소프트웨어](#선택-소프트웨어)
  - [환경 변수](#환경-변수)
- [2. 프로젝트 구조](#2-프로젝트-구조)
  - [주요 vcxproj 파일](#주요-vcxproj-파일)
- [3. 빌드 순서](#3-빌드-순서)
  - [1단계: axisvbs.dll (스크립트 엔진)](#1단계-axisvbsdll-스크립트-엔진)
  - [2단계: axwizard.dll (화면 런타임)](#2단계-axwizarddll-화면-런타임)
  - [3단계: axisbuilder.exe (화면 편집기)](#3단계-axisbuilderexe-화면-편집기)
- [4. vcxproj 설정 상세](#4-vcxproj-설정-상세)
  - [axisvbs.vcxproj 설정](#axisvbsvcxproj-설정)
    - [Include Directories](#include-directories)
    - [Library Directories](#library-directories)
    - [Additional Dependencies](#additional-dependencies)
    - [Runtime Library](#runtime-library)
  - [Wizard.vcxproj 설정](#wizardvcxproj-설정)
    - [Include Directories](#include-directories-1)
    - [Library Directories](#library-directories-1)
    - [Additional Dependencies](#additional-dependencies-1)
- [5. 빌드 트러블슈팅](#5-빌드-트러블슈팅)
  - [오류 1: LNK1104 - 파일을 열 수 없음](#오류-1-lnk1104---파일을-열-수-없음)
  - [오류 2: C1083 - Include 파일을 열 수 없음](#오류-2-c1083---include-파일을-열-수-없음)
  - [오류 3: LNK2019 - 외부 기호 확인 불가](#오류-3-lnk2019---외부-기호-확인-불가)
  - [오류 4: MSB3191 - OutDir 드라이브 문제 (Release)](#오류-4-msb3191---outdir-드라이브-문제-release)
  - [오류 5: LNK2019 - 노트북/PC 간 드라이브 문자(D:\ vs F:\) 불일치 (2026-07-20)](#오류-5-lnk2019---노트북pc-간-드라이브-문자d-vs-f-불일치-2026-07-20)
- [6. 배포 단계](#6-배포-단계)
  - [배포 폴더 구성](#배포-폴더-구성)
  - [배포 체크리스트](#배포-체크리스트)
  - [배포 후 검증](#배포-후-검증)
- [7. 개발 빌드 vs 배포 빌드](#7-개발-빌드-vs-배포-빌드)
  - [Debug 빌드 (개발)](#debug-빌드-개발)
  - [Release 빌드 (배포)](#release-빌드-배포)
- [8. 빌드 자동화 (옵션)](#8-빌드-자동화-옵션)
  - [MSBuild 명령어](#msbuild-명령어)
  - [배치 파일 예시](#배치-파일-예시)
- [9. 빌드 검사 (Pre-Build)](#9-빌드-검사-pre-build)
  - [필수 사항 확인](#필수-사항-확인)
- [10. 관련 문서](#10-관련-문서)

---

## 문서 목적

ibks 프로젝트의 빌드 환경, 프로세스, 문제 해결 방법을 정의합니다.

---

## 1. 빌드 환경 요구사항

### 필수 소프트웨어

| 항목 | 버전 | 용도 | 설치 경로 |
|------|------|------|----------|
| **Visual Studio** | 2015+ | C++ 컴파일러 | C:\Program Files\Microsoft Visual Studio 14.0 |
| **Python** | 3.11.6 (32비트) | Python C API | C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32 |
| **Windows SDK** | 8.1+ | Win32 API, COM | (VS에 포함) |

### 선택 소프트웨어

| 항목 | 용도 |
|------|------|
| Git | 버전 관리 |
| Visual Studio Code / 편집기 | 코드 편집 |

### 환경 변수

```
PATH: C:\Program Files\Microsoft Visual Studio 14.0\VC\bin
PATH: C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32
PYTHON_HOME: C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32
```

---

## 2. 프로젝트 구조

### 주요 vcxproj 파일

```
ibks/
  ├── dll/vbs/
  │   └── axisvbs.vcxproj          (스크립트 엔진 DLL)
  ├── Wizard/
  │   └── Wizard.sln               (axwizard 솔루션)
  │       └── Wizard.vcxproj
  └── builder/
      └── builder.sln              (axisbuilder 솔루션)
          └── AxisBuilder.vcxproj
```

---

## 3. 빌드 순서

### 1단계: axisvbs.dll (스크립트 엔진)

**파일:** `ibks/dll/vbs/axisvbs.vcxproj`

**구성:**
- Debug|Win32
- Release|Win32

**빌드 단계:**

1. Visual Studio 2015 이상 열기
2. `axisvbs.vcxproj` 열기
3. 메뉴: Build → Build Solution
4. 또는 Ctrl+Shift+B

**결과:**
```
Output: ibks/dll/vbs/bin/[Debug|Release]/axisvbs.dll
         ibks/dll/vbs/bin/[Debug|Release]/axisvbs.lib
```

**의존성:**
- python311.lib
- ole32.lib, oleaut32.lib
- kernel32.lib, user32.lib

**확인 사항:**
```
✓ 컴파일 오류 없음
✓ python311.lib 찾음 (IncludeDirectories/AdditionalLibraryDirectories)
✓ C4996 경고만 발생 (python 3.11 API)
```

### 2단계: axwizard.dll (화면 런타임)

**파일:** `ibks/Wizard/Wizard.vcxproj`

**구성:**
- Debug|Win32
- Release|Win32

**빌드 단계:**

1. Visual Studio 2015 이상 열기
2. `Wizard.sln` 열기
3. axisvbs.dll이 먼저 빌드되었는지 확인
4. 메뉴: Build → Build Solution
5. 또는 Ctrl+Shift+B

**결과:**
```
Output: ibks/Wizard/bin/[Debug|Release]/axwizard.dll
         ibks/Wizard/bin/[Debug|Release]/axwizard.lib
```

**의존성:**
- axisvbs.lib (링크)
- mfc*.lib (MFC)
- ole32.lib, oleaut32.lib (COM)

**확인 사항:**
```
✓ axisvbs.lib 찾음
✓ 링크 오류 없음
✓ axwizard.dll 생성됨
```

### 3단계: axisbuilder.exe (화면 편집기)

**파일:** `builder/builder.sln` 또는 `builder/builder.vcxproj`

**구성:**
- Debug|Win32
- Release|Win32

**빌드 단계:**

1. Visual Studio 2015 이상 열기
2. `builder.sln` 열기
3. axisvbs.dll, axwizard.dll이 먼저 빌드되었는지 확인
4. 메뉴: Build → Build Solution
5. 또는 Ctrl+Shift+B

**결과:**
```
Output: builder/bin/[Debug|Release]/AxisBuilder.exe
         builder/bin/[Debug|Release]/aw*.dll (여러 DLL)
```

**의존성:**
- awWcc.lib (빌드 컴파일러 DLL)
- awBuild.lib (맵 로더 DLL)
- mfc*.lib (MFC)

**확인 사항:**
```
✓ 모든 DLL 링크됨
✓ AxisBuilder.exe 생성됨
```

---

## 4. vcxproj 설정 상세

### axisvbs.vcxproj 설정

#### Include Directories

```xml
<PropertyGroup>
  <IncludeDirectories>
    C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\include;
    D:\src\IBKS\src\ibks\dll\vbs;
    %(IncludeDirectories)
  </IncludeDirectories>
</PropertyGroup>
```

#### Library Directories

```xml
<ItemDefinitionGroup>
  <Link>
    <AdditionalLibraryDirectories>
      C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\libs;
      D:\src\IBKS\src\ibks\dll\vbs\bin\$(Configuration);
      %(AdditionalLibraryDirectories)
    </AdditionalLibraryDirectories>
  </Link>
</ItemDefinitionGroup>
```

#### Additional Dependencies

```xml
<ItemDefinitionGroup>
  <Link>
    <AdditionalDependencies>
      python311.lib;
      ole32.lib;
      oleaut32.lib;
      kernel32.lib;
      user32.lib;
      %(AdditionalDependencies)
    </AdditionalDependencies>
  </Link>
</ItemDefinitionGroup>
```

#### Runtime Library

```xml
<ItemDefinitionGroup>
  <ClCompile>
    <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>  <!-- /MD -->
  </ClCompile>
</ItemDefinitionGroup>
```

### Wizard.vcxproj 설정

#### Include Directories

```xml
<IncludeDirectories>
  D:\src\IBKS\src\ibks\Wizard;
  D:\src\IBKS\src\ibks\dll\vbs;
  %(IncludeDirectories)
</IncludeDirectories>
```

#### Library Directories

```xml
<AdditionalLibraryDirectories>
  D:\src\IBKS\src\ibks\dll\vbs\bin\$(Configuration);
  %(AdditionalLibraryDirectories)
</AdditionalLibraryDirectories>
```

#### Additional Dependencies

```xml
<AdditionalDependencies>
  axisvbs.lib;
  ole32.lib;
  oleaut32.lib;
  %(AdditionalDependencies)
</AdditionalDependencies>
```

---

## 5. 빌드 트러블슈팅

### 오류 1: LNK1104 - 파일을 열 수 없음

```
error LNK1104: cannot open file 'python311.lib'
```

**원인:**
- Python 라이브러리 경로 잘못됨
- Python 3.11.6 32비트 미설치

**해결:**
1. Python 설치 경로 확인
   ```cmd
   dir "C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\libs\python311.lib"
   ```

2. vcxproj 수정
   ```xml
   <AdditionalLibraryDirectories>
     C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\libs;
   </AdditionalLibraryDirectories>
   ```

3. 재빌드

### 오류 2: C1083 - Include 파일을 열 수 없음

```
fatal error C1083: Cannot open include file: 'Python.h'
```

**원인:**
- Python include 경로 잘못됨

**해결:**
1. Include 경로 확인
   ```cmd
   dir "C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\include\Python.h"
   ```

2. vcxproj 수정
   ```xml
   <IncludeDirectories>
     C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\include;
   </IncludeDirectories>
   ```

3. 재빌드

### 오류 3: LNK2019 - 외부 기호 확인 불가

```
error LNK2019: unresolved external symbol "int __cdecl Py_Initialize(void)"
```

**원인:**
- python311.lib 링크되지 않음
- Platform Toolset 불일치 (vs2015 vs vs2017 등)

**해결:**
1. Platform Toolset 확인
   ```xml
   <PropertyGroup>
     <PlatformToolset>v140</PlatformToolset>  <!-- VS2015 -->
   </PropertyGroup>
   ```

2. AdditionalDependencies 확인
   ```xml
   <AdditionalDependencies>
     python311.lib;
   </AdditionalDependencies>
   ```

3. 빌드 정리 후 재빌드
   ```cmd
   Clean Solution → Rebuild Solution
   ```

### 오류 4: MSB3191 - OutDir 드라이브 문제 (Release)

```
error MSB3191: Unable to create directory "...\..\bin\Release\"
```

**원인:**
- OutDir이 다른 PC 드라이브로 설정됨 (예: E:\, F:\)
- 현재 PC에는 해당 드라이브 없음

**해결:**
1. 프로젝트 속성 열기 (우클릭 → Properties)
2. Configuration: Release|Win32
3. General → Output Directory
4. `$(ProjectDir)\bin\$(Configuration)\` 로 수정

### 오류 5: LNK2019 - 노트북/PC 간 드라이브 문자(D:\ vs F:\) 불일치 (2026-07-20)

```
error LNK2019: unresolved external symbol "public: bool __thiscall CEngineWrapper::LoadScript(...)"
error LNK1120: 1개의 확인할 수 없는 외부 참조입니다.
```

**원인:**
- `Wizard.vcxproj`의 `AdditionalDependencies`에 axisvbs.lib 경로가 절대경로(`D:\...` 등)로 박혀있으면, 다른 PC(노트북 등, 리포지토리 드라이브 문자가 `F:\` 등으로 다름)에서 열었을 때 해당 경로를 못 찾음
- 이 상태에서 링크가 오래된/엉뚱한 axisvbs.lib을 잡거나 아예 못 찾으면서 `LoadScript` 심볼 unresolved 발생

**해결:**
- `AdditionalDependencies`를 절대경로 대신 상대경로로 수정 (드라이브 문자와 무관하게 어느 PC에서든 동작)
  ```xml
  <!-- Before -->
  D:\src\IBKS\src\src_7_11\platform\dll\vbs\Release\axisvbs.lib

  <!-- After -->
  ..\..\src_7_11\platform\dll\vbs\Release\axisvbs.lib
  ```

**중요 — 트리 구조 확인 (2026-07-20 확인):**
- `Wizard`(axwizard, 화면 런타임)는 **`ibks` 트리** 기준으로 작업 중
- 반면 **`axisvbs`(Python/VBS 스크립트 엔진, dll/vbs)는 `src_7_11/platform/dll/vbs` 트리 기준으로 작업 중** — `ibks/dll/vbs`가 아님
- 즉 `Wizard.vcxproj`가 링크하는 `axisvbs.lib`은 의도적으로 **다른 트리(`src_7_11/platform`)의 빌드 결과물**을 가리킴 (실수 아님, 확인됨)
- `ibks/dll/vbs/pythonEngine.cpp`와 `src_7_11/platform/dll/vbs/pythonEngine.cpp`는 내용이 서로 다름(약 290줄 차이, 2026-07-20 diff 확인) — **axisvbs 관련 실제 코드 수정은 반드시 `src_7_11/platform/dll/vbs`에서 할 것.** `ibks/dll/vbs`를 고치면 Wizard.ocx에는 반영되지 않음.
- `engineWrapper.cpp/h`는 두 트리가 현재 동일함 (2026-07-20 기준, 앞으로 divergence 가능성 있으니 수정 시 트리 재확인 권장)

---

## 6. 배포 단계

### 배포 폴더 구성

```
HTS/
  ├── bin/
  │   ├── axisvbs.dll                 ← 신규
  │   ├── axwizard.dll                ← 수정
  │   ├── python311.dll               ← 신규 (필수)
  │   ├── vcruntime140.dll            ← 신규 (필수)
  │   ├── msvcp140.dll                ← 선택
  │   └── [기타 기존 DLL]
  └── [기타 구성]
```

### 배포 체크리스트

- [ ] axisvbs.dll 복사 (Release 빌드)
- [ ] axwizard.dll 복사 (Release 빌드)
- [ ] python311.dll 복사 (C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\)
- [ ] vcruntime140.dll 복사 (VC++ 런타임)
- [ ] 빌드 구성 확인 (Release|Win32)
- [ ] 테스트: HTS 실행 후 Python 화면 로드 확인

### 배포 후 검증

**테스트 항목:**

1. HTS 실행
   ```cmd
   cd HTS\bin
   HTS.exe
   ```

2. 기존 VBS 화면 로드
   ```
   - 화면 로드 성공 확인
   - 스크립트 실행 정상 확인
   - 이벤트 처리 정상 확인
   ```

3. Python 화면 로드 (axisbuilder로 생성한 Python 화면)
   ```
   - 화면 로드 성공 확인
   - Python 스크립트 실행 정상 확인
   - COM 객체(Screen, System 등) 접근 정상 확인
   ```

---

## 7. 개발 빌드 vs 배포 빌드

### Debug 빌드 (개발)

```
Configuration: Debug|Win32
설정:
  - Runtime Library: /MDd (MultiThreadedDebugDLL)
  - Optimization: Disabled (/Od)
  - Debug Information: Full (/Zi)
  - Define: _DEBUG
```

**사용처:**
- 로컬 개발
- 디버거 연결
- 정보 손실 없음

**출력:**
```
bin/Debug/axisvbs.dll (릴리즈보다 큼, 디버그 정보 포함)
```

### Release 빌드 (배포)

```
Configuration: Release|Win32
설정:
  - Runtime Library: /MD (MultiThreadedDLL)
  - Optimization: /O2 (최적화)
  - Debug Information: Embedded (/Zi) 또는 없음 (/Zl)
  - Define: NDEBUG
```

**사용처:**
- 최종 배포
- 성능 중요
- 파일 크기 최소화

**출력:**
```
bin/Release/axisvbs.dll (최적화, 디버그 정보 제거)
```

---

## 8. 빌드 자동화 (옵션)

### MSBuild 명령어

**Debug 빌드:**
```cmd
msbuild axisvbs.vcxproj /p:Configuration=Debug /p:Platform=Win32
msbuild Wizard.vcxproj /p:Configuration=Debug /p:Platform=Win32
msbuild AxisBuilder.vcxproj /p:Configuration=Debug /p:Platform=Win32
```

**Release 빌드:**
```cmd
msbuild axisvbs.vcxproj /p:Configuration=Release /p:Platform=Win32
msbuild Wizard.vcxproj /p:Configuration=Release /p:Platform=Win32
msbuild AxisBuilder.vcxproj /p:Configuration=Release /p:Platform=Win32
```

### 배치 파일 예시

**build_all.bat:**
```batch
@echo off
setlocal

echo Building axisvbs.dll (Release)...
msbuild ibks\dll\vbs\axisvbs.vcxproj /p:Configuration=Release /p:Platform=Win32
if errorlevel 1 goto error

echo Building axwizard.dll (Release)...
msbuild ibks\Wizard\Wizard.vcxproj /p:Configuration=Release /p:Platform=Win32
if errorlevel 1 goto error

echo Building axisbuilder.exe (Release)...
msbuild builder\builder.vcxproj /p:Configuration=Release /p:Platform=Win32
if errorlevel 1 goto error

echo.
echo All builds completed successfully!
goto end

:error
echo Build failed with error code %errorlevel%
exit /b 1

:end
```

---

## 9. 빌드 검사 (Pre-Build)

### 필수 사항 확인

1. **Python 설치 확인**
   ```cmd
   dir "C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\libs\python311.lib"
   ```

2. **vcxproj 경로 확인**
   ```
   axisvbs.vcxproj:
     - IncludeDirectories: Python311-32\include
     - AdditionalLibraryDirectories: Python311-32\libs
   ```

3. **이전 빌드 파일 삭제**
   ```cmd
   Clean Solution (Visual Studio)
   ```

---

## 10. 관련 문서

- `@docs/Architecture.md` - 모듈 구조
- `@docs/Dependency.md` - 라이브러리 의존성
- `@docs/python_engine_260608.md` - Python 엔진 통합 상세 기록
