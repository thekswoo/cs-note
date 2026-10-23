### 컴파일
`g++ -O2 -std=c++17 -pthread -latomic cas-counter.cpp -o cas-counter.out`

### 실행결과
```bash
thekswoo@DESKTOP-V7E87UB:~/cs-note/2026-2/parallel-programming/c11-threads/cas$ ./cas-counter.out 
# elapsed time (incorrect_max): 12.7868s
# elapsed time (correct_max): 13.8316s
999999998 999999999
```

### 분석
#### 1. incorrect_max: 최댓값이 더 작은 값으로 덮어써짐
기대값은 `num_iters - 1 = 999999999`인데 `false_max`의 결과는 `999999998`이 나왔다.
```cpp
if(i > *counter) {  // 1) load: 현재 값과 비교
    *counter = i;   // 2) store: 새 값 기록
}
```
`counter`가 `atomic`이라 1)의 load와 2)의 store는 **각각은** atomic하다. 하지만 **1)~2)를 합친 "비교 후 기록"(check-then-act)은 atomic하지 않다.** 그래서 두 동작 사이에 다른 thread가 끼어들 수 있다.

| 순서 | Thread A (`i = 999999998`) | Thread B (`i = 999999999`) | `counter` |
|---|---|---|---|
| ① | 1) `998 > counter` → 참 | | 999999990 |
| ② | | 1) `999 > counter` → 참 | 999999990 |
| ③ | | 2) `counter = 999` | **999999999** |
| ④ | 2) `counter = 998` (이미 오래된 비교 결과로 기록) | | **999999998** |

A는 ①에서 본 **오래된 값**을 기준으로 판단했기 때문에, B가 써 둔 더 큰 값을 작은 값으로 덮어썼다(lost update). 마지막 쓰기가 이기므로 최종 결과가 `999999998`로 남았다.

> 실행 타이밍에 따라 결과가 달라지는 비결정적 버그라서 실행할 때마다 값이 다를 수 있고, 운이 좋으면 정답이 나올 수도 있다.

#### 2. correct_max: CAS로 비교와 기록을 한 번에
```cpp
auto previous = counter->load();
while(previous < i && !counter->compare_exchange_weak(previous, i)) {}
```
`compare_exchange_weak(previous, i)`는 **"`counter`가 아직 `previous`와 같을 때만 `i`로 바꾼다"**를 하나의 atomic 연산으로 수행한다.
- 성공하면 그 사이에 아무도 값을 바꾸지 않았다는 뜻이므로 안전하게 갱신된다.
- 실패하면 다른 thread가 먼저 값을 바꾼 것이다. 이때 `previous`에 최신 값이 자동으로 들어가고, `previous < i`를 다시 검사한다. 다른 thread가 이미 더 큰 값을 썼다면 루프를 빠져나오고, 아니면 다시 시도한다.

따라서 위 표의 ④처럼 오래된 값을 기준으로 덮어쓰는 일이 생기지 않고, 결과는 항상 `999999999`이다.

#### 3. 수행 시간
correct_max(13.83s)가 incorrect_max(12.79s)보다 약 8% 느리다. CAS는 경합 상황에서 실패하면 재시도해야 하고, 10개 thread가 같은 cache line에 CAS를 시도하면서 비용이 늘어난다. 이 정도 오버헤드로 정확성을 보장할 수 있다.
