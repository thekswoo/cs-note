
http://165.246.21.98/domjudge/

# PS 환경 (VSCode + C++ + DOMjudge) — WSL 기준

실습실 컴퓨터에서 빠르게 재세팅하기 위한 템플릿.
학교 PC에 **WSL이 이미 설치**돼 있으므로 리눅스 g++ 로 채점 환경과 동일하게 맞춘다.

## I-Class(학교 저장공간)에 올려둘 것

| 파일 | 설명 |
|---|---|
| `PS.code-profile` | VSCode 확장·설정 묶음 (아래 "프로필 만들기") |
| `ps-template/` (또는 zip) | `.vscode` 설정 + `main.cpp` |

> WSL을 쓰면 컴파일러 zip은 필요 없음. WSL이 막혀 있을 때만 Windows 대안(맨 아래) 참고.

---

## 프로필 만들기 (집에서 1회)

1. VSCode 왼쪽 아래 톱니바퀴 → **프로필** → **새 프로필 만들기** → 이름 `PS`
2. `PS` 프로필 상태에서 확장 설치:
   - `ms-vscode-remote.remote-wsl` (WSL)
   - `ms-vscode.cpptools` (C/C++)
   - `DivyanshuAgrawal.competitive-programming-helper` (cph)
3. 톱니바퀴 → 프로필 → `PS` 옆 `...` → **내보내기** → `PS.code-profile` 저장 → I-Class 업로드

---

## 실습실 재세팅 체크리스트 (5분)

### 1. WSL 안에 컴파일러 준비 (배포판 새로 깔렸으면 1회)
WSL 터미널(Ubuntu)에서:
```
sudo apt update && sudo apt install -y g++ gdb
g++ --version
```
> 학교 PC가 로그아웃 시 사용자 프로필을 초기화하면 WSL도 초기화될 수 있음 →
> 이 명령을 매번 실행 (인터넷 되면 1~2분).

### 2. I-Class에서 `PS.code-profile` + `ps-template` 다운로드

### 3. 템플릿을 WSL 안에 복사 (권장: 리눅스 홈, `/mnt/c` 보다 빠름)
WSL 터미널에서:
```
mkdir -p ~/ps && cp -r /mnt/c/Users/<Windows계정>/Downloads/ps-template ~/ps/
```

### 4. VSCode
1. 톱니바퀴 → 프로필 → **가져오기** → `PS.code-profile` (확장 자동 설치)
2. 명령 팔레트(`Ctrl+Shift+P`) → **WSL: Connect to WSL** (왼쪽 아래가 `WSL: Ubuntu` 로 바뀜)
3. `파일 → 폴더 열기` → `~/ps/ps-template`
4. 첫 연결 시 cpptools / cph 가 "WSL 쪽에" 자동 설치됨 (30초)
5. 확인: 통합 터미널이 bash 인지, `g++ --version` 되는지

### 5. (사용 후) 공용 PC 정리
프로필 삭제 or 기본 프로필로 전환, VSCode 계정 로그아웃

---

## 문제 풀이 흐름

1. `main.cpp` 편집 (문제마다 `1000.cpp` 처럼 복사 추천)
2. `Ctrl+Alt+B` → 오른쪽 cph 패널
   - `+ Test Case` 로 입력 / 기대 출력 붙여넣기 → `Run All` → 초록(AC) 확인
   - 또는 브라우저 **Competitive Companion** 확장으로 예제 자동 로드
3. 통과하면 DOMjudge 웹에 해당 `.cpp` 제출

### 단축키
| 키 | 동작 |
|---|---|
| `Ctrl+Alt+B` | cph: 컴파일 + 전체 테스트케이스 실행 |
| `Ctrl+Alt+D` | cph: 테스트케이스 패널 토글 |
| `Ctrl+Shift+B` | (cph 없이) g++ 빌드만 |
| `F5` | 디버깅 (중단점) |

---

## DOMjudge C++ 표준 맞추기

기본 `-std=c++17`. c++17 코드는 c++20 채점기에서도 그대로 동작하므로 보통 안 바꿔도 됨.
과목/대회가 c++20이고 신문법을 써야 할 때만 아래 3곳을 `c++20` 으로:
- `.vscode/settings.json` → `cph.language.cpp.Args`
- `.vscode/tasks.json` → `args` 의 `-std=c++17`
- `.vscode/settings.json` → `C_Cpp.default.cppStandard`

---

## Windows 대안 (WSL이 막혀 있을 때)

1. winlibs.com 에서 UCRT/GCC 최신 Win64 **zip** 다운로드 → I-Class 업로드
2. 실습실: `C:\Users\<계정>\mingw64` 에 압축 해제 →
   사용자 PATH 에 `...\mingw64\bin` 추가 → VSCode 재시작
3. `.vscode` 파일 3개를 Windows용으로 되돌리기:
   - `tasks.json` / `launch.json`: 출력 경로에 `.exe` 추가, `/` → `\\`,
     `miDebuggerPath` 를 `gdb.exe` 로
   - `settings.json`: `compilerPath` 를 `g++` 로
