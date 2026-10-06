# Embedded Software (임베디드소프트웨어)

2026-2 임베디드소프트웨어 실습 코드 (uC/OS-II, DOSBox + Borland C++ 4.5)

## Directory

```
embedded-software/
├── README.md
├── sync.sh              # C:\SOFTWARE → cs-note 복사 + commit/push
└── ucos-ii/
    └── EX1/             # C:\SOFTWARE\uCOS-II\EX1_x86L\BC45\SOURCE 사본
```

> ⚠️ 원본은 항상 `C:\SOFTWARE\...`. cs-note 안의 사본은 직접 수정하지 않는다.

## 실습 순서

### 1. DOSBox 실행 & 이동

```
mount c c:\                         ← 수업에서 쓰는 mount 명령으로 수정
c:
cd \SOFTWARE\uCOS-II\EX1_x86L\BC45
```

### 2. 소스 작성

- 작업 폴더: `SOURCE\`
- 주차별 파일은 복사해서 새로 만든다 (기존 파일 보존)

```
cd SOURCE
copy EX1.C EX1_W6.C
```

### 3. 빌드 & 실행

```
cd ..\TEST
MAKETEST                            ← 수업에서 쓰는 빌드 명령으로 수정
TEST                                ← 실행 파일 이름으로 수정
```

### 4. 실습 끝나면 (WSL)

```bash
~/cs-note/2026-2/embedded-software/sync.sh "embedded-software: 6week"
```

## DOS 주의사항

- **파일명 8.3 규칙**: 이름 8자 + 확장자 3자까지.
  `EX1_2week.C`(9자)는 DOS에서 `EX1_2W~1.C`처럼 잘려 보임 → `EX1_W2.C` 형태 권장
- 대소문자 구분 없음 (`ex1.c` = `EX1.C`)
- 줄바꿈은 CRLF

## 실습 기록

| 주차 | 파일 | 내용 | 메모 |
|---|---|---|---|
| 2 | `EX1_2week.C` | | |
| 3 | `EX1_3week.C` | | |
| 4 | `EX1_4week.C` | | |
| 5 | `EX1_5week.C` | | |

## 핵심 API 메모

| 함수 | 역할 |
|---|---|
| `OSInit()` | 커널 초기화 (가장 먼저 호출) |
| `OSTaskCreate()` | 태스크 생성 (우선순위 지정) |
| `OSStart()` | 멀티태스킹 시작 (리턴하지 않음) |
| `OSTimeDly()` | 지정한 tick만큼 태스크 대기 |
| `OSSemCreate()` / `OSSemPend()` / `OSSemPost()` | 세마포어 생성 / 획득 / 해제 |
| `OSMboxPost()` / `OSMboxPend()` | 메일박스 송신 / 수신 |