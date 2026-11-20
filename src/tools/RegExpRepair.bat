@echo off
setlocal EnableExtensions DisableDelayedExpansion
title VBScript RegExp 점검/복구

rem ---- self elevate --------------------------------------------------------
net session >nul 2>&1
if errorlevel 1 (
    echo 관리자 권한이 필요합니다. 권한 상승 창에서 [예]를 눌러주세요.
    powershell -NoProfile -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
    exit /b
)

set "VBS=%TEMP%\re_test.vbs"
set "LOG=%TEMP%\RegExpRepair.log"
set "CS32=%WINDIR%\SysWOW64\cscript.exe"
if not exist "%CS32%" set "CS32=%WINDIR%\System32\cscript.exe"
set "CLSID={3F4DACA4-160D-11D2-A8E9-00104B365C9F}"

rem ---- write test script (lines marked with ::: at the end of this file) ----
if exist "%VBS%" del /f /q "%VBS%"
for /f "tokens=* delims=:" %%L in ('findstr /b ":::" "%~f0"') do >>"%VBS%" echo %%L

echo ==== %DATE% %TIME% ==== >>"%LOG%"
echo.
echo ============================================================
echo  VBScript RegExp 점검/복구
echo  로그: %LOG%
echo ============================================================

echo.
echo [0] 현재 상태 점검
call :test
if not errorlevel 1 goto :ok

rem ---- step 1: per-user (HKCU) override -------------------------------------
echo.
echo [1] 사용자별(HKCU) 등록 키 확인/삭제
rem HTS is 32-bit: HKCU\Software\Classes\CLSID is redirected, so check the 32-bit view too
reg query "HKCU\Software\Classes\CLSID\%CLSID%" /reg:32 >nul 2>&1 && (
    echo     HKCU CLSID 키^(32비트^) 발견 - 삭제합니다.
    reg delete "HKCU\Software\Classes\CLSID\%CLSID%" /reg:32 /f >>"%LOG%" 2>&1
)
reg query "HKCU\Software\Classes\CLSID\%CLSID%" /reg:64 >nul 2>&1 && (
    echo     HKCU CLSID 키^(64비트^) 발견 - 삭제합니다.
    reg delete "HKCU\Software\Classes\CLSID\%CLSID%" /reg:64 /f >>"%LOG%" 2>&1
)
reg query "HKCU\Software\Classes\VBScript.RegExp" >nul 2>&1 && (
    echo     HKCU ProgID 키 발견 - 삭제합니다.
    reg delete "HKCU\Software\Classes\VBScript.RegExp" /f >>"%LOG%" 2>&1
)
call :test
if not errorlevel 1 goto :ok

rem ---- step 2: re-register vbscript.dll -------------------------------------
echo.
echo [2] vbscript.dll 재등록 (32/64비트)
if exist "%WINDIR%\SysWOW64\vbscript.dll" (
    "%WINDIR%\SysWOW64\regsvr32.exe" /s "%WINDIR%\SysWOW64\vbscript.dll"
    call echo     32비트 regsvr32 결과=%%ERRORLEVEL%%
)
if exist "%WINDIR%\System32\vbscript.dll" (
    "%WINDIR%\System32\regsvr32.exe" /s "%WINDIR%\System32\vbscript.dll"
    call echo     64비트 regsvr32 결과=%%ERRORLEVEL%%
)
echo     (결과가 0이 아니어도 보호된 정상 등록일 수 있으므로 다음 단계로 진행)
call :test
if not errorlevel 1 goto :ok

rem ---- step 3: DISM + SFC ---------------------------------------------------
echo.
echo [3] 시스템 파일 복구 (DISM RestoreHealth + sfc). 10~30분 걸릴 수 있습니다.
choice /c YN /m "    진행하시겠습니까"
if errorlevel 2 goto :step4
DISM /Online /Cleanup-Image /RestoreHealth
sfc /scannow
call :test
if not errorlevel 1 goto :ok_reboot

:step4
rem ---- step 4: reinstall VBScript optional feature (Win11 24H2+) ------------
echo.
echo [4] VBScript 선택적 기능 재설치 (Win11 24H2 이상)
set "CAP="
for /f "tokens=1 delims=| " %%C in ('DISM /Online /Get-Capabilities /Format:Table ^| findstr /i /b "VBSCRIPT"') do set "CAP=%%C"
if not defined CAP (
    echo     이 PC에는 VBScript 선택적 기능 항목이 없습니다. ^(Win10 등^) - 건너뜁니다.
    goto :fail
)
echo     기능 이름: %CAP%
choice /c YN /m "    제거 후 재설치하시겠습니까"
if errorlevel 2 goto :fail
DISM /Online /Remove-Capability /CapabilityName:%CAP%
DISM /Online /Add-Capability /CapabilityName:%CAP%
call :test
if not errorlevel 1 goto :ok_reboot
goto :fail

rem ---- results --------------------------------------------------------------
:ok
echo.
echo ============================================================
echo  [정상] RegExp 생성 OK.
echo ============================================================
goto :end

:ok_reboot
echo.
echo ============================================================
echo  [복구됨] RegExp 생성 OK. PC를 재부팅한 뒤 HTS를 실행해주세요.
echo ============================================================
goto :end

:fail
echo.
echo ============================================================
echo  [실패] RegExp 생성이 여전히 안 됩니다.
echo  백신/보안 프로그램의 스크립트 차단 여부를 확인하고,
echo  로그 파일을 전달해주세요: %LOG%
echo ============================================================
goto :end

:end
del /f /q "%VBS%" >nul 2>&1
echo.
pause
exit /b

rem ---- run test with 32-bit cscript, errorlevel 0 = OK ----------------------
:test
"%CS32%" //nologo "%VBS%" > "%TEMP%\re_test.out" 2>&1
set "RC=%ERRORLEVEL%"
type "%TEMP%\re_test.out"
type "%TEMP%\re_test.out" >>"%LOG%"
del /f /q "%TEMP%\re_test.out" >nul 2>&1
exit /b %RC%

rem ---- embedded VBScript test (do not edit the ::: prefix) ------------------
:::On Error Resume Next
:::Set r = New RegExp
:::e1 = Err.Number
:::d1 = Err.Description
:::Err.Clear
:::Set r2 = CreateObject("VBScript.RegExp")
:::e2 = Err.Number
:::d2 = Err.Description
:::Err.Clear
:::t = ""
:::If e1 = 0 Then
:::    r.Pattern = "[^0-9]"
:::    r.Global = True
:::    t = r.Replace("1,234won", "")
:::End If
:::WScript.Echo "    engine       = " & ScriptEngineMajorVersion & "." & ScriptEngineMinorVersion & "." & ScriptEngineBuildVersion
:::WScript.Echo "    New RegExp   = " & e1 & " " & d1
:::WScript.Echo "    CreateObject = " & e2 & " " & d2
:::WScript.Echo "    Replace test = [" & t & "] (expect 1234)"
:::If e1 = 0 And e2 = 0 And t = "1234" Then
:::    WScript.Echo "    => OK"
:::    WScript.Quit 0
:::End If
:::WScript.Echo "    => FAIL"
:::WScript.Quit 1
