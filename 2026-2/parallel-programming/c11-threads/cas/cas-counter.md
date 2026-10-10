### 컴파일
`g++ -O2 -std=c++17 -pthread -latomic cas-counter.cpp -o cas-counter.out`

### 실행결과
```bash
thekswoo@DESKTOP-V7E87UB:~/cs-note/2026-2/parallel-programming/c11-threads/cas$ ./cas-counter.out 
# elapsed time (incorrect_max): 12.7868s
# elapsed time (correct_max): 13.8316s
999999998 999999999
```