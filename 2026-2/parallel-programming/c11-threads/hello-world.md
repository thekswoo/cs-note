### 실행 과정
`g++ -O2 -std=c++11 -pthread hello-world.cpp -o hello-world.out`
- `-O2` : 일반적인 최적화.
- `-std=c++11` : c++11 문법과 라이브러리를 사용
- `-pthread` : `std::thread`가 내부적으로 posix thread를 사용해서 해당 라이브러리랑 연결
- `-o 이름.out`:출력 파일 이름 지정. output의 약자임.



### 실행 결과
```bash
thekswoo@DESKTOP-V7E87UB:~/parallel-programming/c11-threads$ ./hello-world.out 
Hello from thread: Hello from thread: 3
Hello from thread: Hello from thread: 0
2
1
thekswoo@DESKTOP-V7E87UB:~/parallel-programming/c11-threads$ ./hello-world.out 
Hello from thread: Hello from thread: 03
Hello from thread: 1
Hello from thread: 2

thekswoo@DESKTOP-V7E87UB:~/parallel-programming/c11-threads$ ./hello-world.out 
Hello from thread: Hello from thread: 2
Hello from thread: 3
0
Hello from thread: 1
thekswoo@DESKTOP-V7E87UB:~/parallel-programming/c11-threads$ ./hello-world.out 
Hello from thread: 2
Hello from thread: 0
Hello from thread: 1
Hello from thread: 3
thekswoo@DESKTOP-V7E87UB:~/parallel-programming/c11-threads$ ./hello-world.out 
Hello from thread: Hello from thread: 1
Hello from thread: 3
0
Hello from thread: 2
thekswoo@DESKTOP-V7E87UB:~/parallel-programming/c11-threads$ ./hello-world.out 
Hello from thread: 0
Hello from thread: 3
Hello from thread: 1
Hello from thread: 2
thekswoo@DESKTOP-V7E87UB:~/parallel-programming/c11-threads$ ./hello-world.out 
Hello from thread: Hello from thread: 01

Hello from thread: 2
Hello from thread: 3
```

### 분석1
출력 결과를 확인해보면 thread간의 함수 호출이 섞여서 출력됐다.

- `cout << "Hello from thread: " << id << endl;`은 실제로 **독립된 출력 호출 3번**이다.
- `cout`은 모든 스레드가 **공유하는 자원**이며, 세 호출을 하나로 묶어 보호하는 장치가 없다.
- 스레드 실행 순서는 OS 스케줄러가 정하며 **매번 달라질 수 있어서**, 호출 사이에 다른 스레드가 끼어들 수 있다.

이처럼 **실행 타이밍에 따라 결과가 달라지는 것이 race condition**이다.
출력 구간이 critical section이며, `mutex`로 상호 배제하면 해결할 수 있다.

### 분석2
thread 출력 순서가 실행할때마다 항상 달라진다.

- `condition_variable`으로 순서를 강제하는 방법
- 순서와 무관하게 각 쓰레드 별로 결과를 자신의 id에 맞는 칸에 저장하고 `join`후에 메인 쓰레드가 0번부터 차례대로 출력하는 방법


