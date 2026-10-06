# cs-note

인하대학교 컴퓨터공학과 전공 수업에서 작성한 실습 코드, 과제, 학습 노트를 학기별·과목별로 정리한 저장소입니다.

## Structure

```
cs-note/
├── .gitignore              # 공통 무시 규칙 (OS, 에디터, 빌드 산출물, PDF)
└── 2026-2/
    ├── parallel-programming/
    ├── problem-solving/
    └── ...
```

- 학기 폴더(`YYYY-N`) 아래에 과목별 폴더를 둡니다.
- 과목별로 필요한 무시 규칙은 각 과목 폴더의 `.gitignore`에 따로 둡니다.

## 2026-2

| Course | Folder | Topics / Tools | Status |
|---|---|---|---|
| 병렬 프로그래밍 | [`parallel-programming`](2026-2/parallel-programming) | C11 threads, OpenMP, CUDA, MPI | 🟢 진행 중 |
| 문제해결기법 | [`problem-solving`](2026-2/problem-solving) | C/C++, 자료구조·알고리즘, DOMjudge | 🟢 진행 중 |
| 컴퓨터구조론 | `computer-architecture` | RISC-V ISA, Pipelining, Memory Hierarchy | ⚪ 예정 |
| 임베디드소프트웨어 | `embedded-software` | avr-gcc, Real-time Scheduling, Embedded Board | ⚪ 예정 |
| 모빌리티 임베디드시스템설계 | `mobility-embedded` | Raspberry Pi, Linux, Python/C, GPIO/PWM, OpenCV, SDV | ⚪ 예정 |

## Environment

- macOS / Windows 11 + WSL2 (Ubuntu)
- GCC/G++, CUDA Toolkit, Open MPI
- VS Code

## Notes

- 강의자료(PDF 등) 및 문제지는 저작권 문제로 포함하지 않습니다.
- 과제 및 실습 코드는 학습 기록 목적으로 보관하며, 수강생의 무단 복제·제출을 금합니다.
- 개념 정리는 별도의 Obsidian vault에서 관리하고, 이 저장소에는 코드와 간단한 요약만 둡니다.
