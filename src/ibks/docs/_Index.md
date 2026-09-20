---
project: ibks
category: index
status: 작성됨
updated: 2026-09-13
---

# ibks 문서 홈

이 vault(`ibks/docs`)에 쌓인 문서 전체를 주제별로 모아둔 시작점입니다. 개별 문서 끝의 "관련 문서" 절도 `[[...]]` 링크로 서로 연결돼 있으니, 여기서 시작해서 그래프뷰/백링크로 따라가면 됩니다.

## 목차

- [1. 연말 플랫폼 마이그레이션 명세 (최우선)](#1-연말-플랫폼-마이그레이션-명세-최우선)
- [2. axWizard 코어 아키텍처](#2-axwizard-코어-아키텍처)
- [3. chart_dll 조사 (C_Total / C_UserGrp / axisG*)](#3-chart_dll-조사-c_total--c_usergrp--axisg)
- [4. ibks 프로젝트 기초 문서 (Python 엔진 전환기)](#4-ibks-프로젝트-기초-문서-python-엔진-전환기)
- [5. 작업일지 / 기타](#5-작업일지-기타)

---

## 1. 연말 플랫폼 마이그레이션 명세 (최우선)

새 플랫폼으로 이전하기 위한 실측 스펙 문서들. `MigrationSpec_SocketToDrawing.md`가 핵심 축.

- [[MigrationSpec_SocketToDrawing.md]] — 소켓 프로토콜(_axisH), 패킷재조립, 화면식별, 필드파싱, 암호화, msgK 전체 목록. 실측+패킷캡처 3중검증
- [[WizardFireProtocol.md]] — AxisSock↔axWizard↔Main(호스트 EXE) 이벤트/명령 프로토콜, OnFireEvent/OnFire/OnFireRec 배선, FEV_AXIS 이중의미
- [[LoginSequence.md]] — 로그인 시퀀스 송수신 전체 카탈로그, OnFireEvent 분기, 계좌/CA 레지스트리 블록, 공동인증서 로그인 변형 흐름
- [[FilePatchProtocol.md]] — 로그인시 파일 패치 프로토콜, MakeUpdateList() 판정로직, infoAXIS/infoRSC 카탈로그
- [[CertifyArchitecture.md]] — 공동인증서(CertifyCtrl) 전체 흐름, Wizard↔CertifyCtrl 디스패치, 헤드리스봇 이식 불가 결론
- [[CX_WebViewBidirectional.md]] — CX_WebView(WebView2) 양방향 통신 현황, Gap 분석
- [[WizardDependency.md]] — axWizard.ocx 의존성 4계층(빌드타임 링크/COM컨트롤/고정LoadLibrary/가변 플러그인)
- [[DebugLogGuide.md]] — axlog 중앙로깅 인프라, 전체 로그 태그 카탈로그 (DebugView 검색용, 다른 문서 작업 시에도 계속 참고)

## 2. axWizard 코어 아키텍처

axwizard(Wizard.dll) 자체의 클래스 계층과 내부 동작 분석.

- [[WizardArchitecture.md]] — axWizard 클래스 계층, 이벤트→프로시저 매핑, 전체 클래스 레퍼런스
- [[AxisformArchitecture.md]] — axisform.dll(컨트롤 렌더링 레이어) 클래스 계층, DLL 경계 vtable 위험성
- [[RealtimeCodeIndex_Investigation.md]] — RTM 종목코드 역인덱스 조사, FA_FLASH 필드 식별
- [[KnowledgeBase.md]] — 설계 의도, 트러블슈팅, 버그 해결 이력 누적 기록

## 3. chart_dll 조사 (C_Total / C_UserGrp / axisG*)

`CONTROL/ibk_chart_dll_20220831` 아래 차트 엔진 서브시스템 — Wizard와는 별도의 GOOPHOOP 클라이언트 구현.

- [[C_TotalArchitecture.md]] — C_Total.dll(메인 OLE/COM 캔들차트 컨트롤) 아키텍처
- [[C_TotalDataFlow.md]] — C_Total 데이터 흐름
- [[C_TotalDependency.md]] — C_Total 의존성 분석
- [[C_TotalSourceIndex.md]] — C_Total 소스 인덱스
- [[C_TotalTRProtocol.md]] — C_Total TR 요청/응답 구조체(송수신 프로토콜)
- [[C_UserGrpArchitecture.md]] — C_UserGrp.dll(단순 라인/바 차트 컨트롤) 아키텍처
- [[C_UserGrpDataFlow.md]] — C_UserGrp 데이터 흐름
- [[C_UserGrpDependency.md]] — C_UserGrp 의존성 분석
- [[C_UserGrpSourceIndex.md]] — C_UserGrp 소스 인덱스
- [[AxisGData_CandleConstruction.md]] — axisGData 캔들(OHLC) 구성 방식, 실시간 틱→분봉 합성 로직
- [[AxisGMain_GDlgDependency.md]] — axisGMain.dll/axisGDlg.dll 의존성 분석

## 4. ibks 프로젝트 기초 문서 (Python 엔진 전환기)

VBScript→Python 스크립트엔진 전환 프로젝트 당시 작성된 초기 구조 문서들.

- [[Architecture.md]] — ibks 모듈 구조, 계층, DLL 관계, 인터페이스
- [[Dependency.md]] — Header/DLL/Library 의존성 분석, 배포 체크리스트
- [[SourceIndex.md]] — 소스 파일 색인, 클래스/함수, 호출 관계
- [[CallGraph.md]] — 함수 호출 흐름도(Mermaid), 초기화/이벤트 흐름
- [[Build.md]] — 빌드 환경, 프로세스, vcxproj 설정, 트러블슈팅
- [[ProjectReport.md]] — 프로젝트 전체 요약, 기술 분석, 위험 평가
- [[Todo.md]] — 진행 과제, 테스트 계획, 마일스톤, 리스크 관리
- [[python_engine_260608.md]] — VBScript→Python Engine 전환 프로젝트 상세 기록

## 5. 작업일지 / 기타

- [[2026-08-17.md]] — 작업일지(platform 분석 내용 정리 등)
