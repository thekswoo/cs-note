### 컴파일
`g++ -O2 -std=c11 -pthread -latomic atomic-counter.cpp -o atomic-counter.out`

### 실행결과
```bash
thekswoo@DESKTOP-V7E87UB:~/cs-note/2026-2/parallel-programming/c11-threads/atomic$ ./atomic-counter.out 
# elapsed time (mutex_multithreaded): 8.25296s
# elapsed time (atomic_multithreaded): 1.39928s
100000000 100000000
```