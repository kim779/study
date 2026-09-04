---
project: src
category: architecture
status: 작성됨
updated: 2026-07-16
---

# Architecture (Workspace Index)

- 생성일: 2026-07-07 (갱신: 2026-07-14)
- 목적: 이 저장소는 단일 프로젝트가 아니라 AXIS, AxisChaser, AxisAgent, ibks, builder, python, src_7_11 등
  다수의 독립 서브프로젝트로 구성된 워크스페이스다. 상세 아키텍처 문서는 각 서브프로젝트 폴더의
  자체 `docs/`에 두는 것을 컨벤션으로 확정한다(2026-07-14, AxisChaser/docs/ 선례 및 AXIS 분석 과정에서 확인).
  이 문서는 워크스페이스 전체를 아우르는 색인 역할만 한다.

## 개요

각 서브프로젝트는 자체 `<서브프로젝트>/docs/Architecture.md` 등을 보유한다. 루트 docs/에는
서브프로젝트 상세 내용을 복제하지 않는다(중복 시 오기재 위험 — 2026-07-14 AXIS 분석 때
루트 docs에 옮겨 적던 중 COM 초기화 함수명이 잘못 기재된 사례 있었음, 원본은 AXIS/docs/에서 정정).

## 서브프로젝트 문서 색인

| 서브프로젝트 | docs 경로 | 상태 |
|---|---|---|
| AXIS | [AXIS/docs/Architecture.md](../AXIS/docs/Architecture.md) | 분석 완료 (2026-07-14) — 크래시 원인분석용, endless 진행 중 |
| AxisChaser | [AxisChaser/docs/Architecture.md](../AxisChaser/docs/Architecture.md) | 분석 완료 (2026-07-10) |
| AxisAgent | - | 분석 전 |
| ibks | - | 분석 전 |
| builder | - | 분석 전 |
| python | - | 분석 전 |
| src_7_11 | - | 분석 전 (구버전 스냅샷 주의) |

## 목차

(서브프로젝트별 상세는 각 docs/ 참조)
