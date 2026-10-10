## Ping-Pong with Condition Variable

**공유 상태와 차례 표시.** 두 스레드는 같은 `mutex`, `condition_variable`, 공유 변수 `is_ping`을 함께 사용한다. `is_ping`이 `true`면 ping 차례, `false`면 pong 차례다. 각 스레드는 `cv.wait(lock, predicate)`로 자기 차례가 될 때까지 잠든다. 이 호출은 `while (!predicate()) cv.wait(lock);`과 같아서 spurious wakeup에도 안전하다. ping은 `is_ping`, pong은 `!is_ping`을 조건으로 쓰기 때문에 항상 한 스레드만 진행한다.

**lock의 흐름.** `wait()`는 잠들 때 lock을 자동으로 풀고, 깨어나면 다시 잡은 뒤 조건을 재확인한다. 중간에 lock을 풀었다 다시 잡아야 하므로 `lock_guard`가 아닌 `unique_lock`을 쓴다. 조건이 참이면 lock을 쥔 채로 출력하고, `is_ping`을 뒤집고, `notify_one()`으로 상대를 깨운다. 루프 한 바퀴가 끝나 scope를 벗어날 때 lock이 해제된다.

**핵심 원리.** 상대를 진행시키려면 "조건 변경"과 "notify"가 둘 다 필요하다. notify는 잠든 스레드를 깨우는 신호일 뿐이고, 깨어난 스레드는 공유 상태를 보고 진행할지 다시 잘지 판단한다. 이 방식은 차례가 아닌 스레드를 재워 두므로 busy waiting 없이 두 스레드를 정확히 번갈아 실행시킨다. 단, 기다리는 스레드가 여럿이고 조건이 서로 다르면 엉뚱한 스레드만 깨어날 수 있으므로 `notify_all`을 써야 한다.

### 실행결과
```bash
thekswoo@DESKTOP-V7E87UB:~/cs-note/2026-2/parallel-programming/c11-threads$ ./cv-pingpong.out 
ping
pong
ping
pong
ping
pong
ping
pong
ping
pong
ping
pong
ping
^C

```