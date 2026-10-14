# Knapsack 시험 요약

## 1. Sequential → Parallel 바뀌는 것 (이것만)

| | Sequential | Parallel |
|---|---|---|
| `global_state` | `state_t<...>` | `atomic<state_t<...>>` |
| 갱신 | `sequential_update` | `atomic_update` (CAS) |
| bsf 읽기 | `global_state.value` | `global_state.load().value` |
| 시작 | `traverse` 2번 호출 | `TP.spawn` 2번 + `TP.wait_and_stop()` |

나머지(`init_tuples`, `dantzig_bound`, 탐색 로직)는 동일.

---

## 2. 핵심 코드

### traverse — 반영 → 초과 → 갱신 → bound → 분기
```cpp
const bool bit = (bmask >> height) % 2;               // height번 비트 꺼내기
tuple.weight += bit * tuples[height].weight;
tuple.value  += bit * tuples[height].value;

if (tuple.weight > capacity) return;                  // 용량 초과
atomic_update(tuple, bmask);                          // 최선 갱신

auto bsf = global_state.load().value;
if (dantzig_bound(height + 1, tuple) < bsf) return;   // pruning

if (height + 1 < num_items) {
    traverse(height + 1, tuple, bmask + (1 << (height + 1)));   // 넣음 (먼저)
    traverse(height + 1, tuple, bmask);                         // 안 넣음
}
```

### atomic_update — CAS 루프
```cpp
auto g_state = global_state.load();
auto l_value = tuple.value;
state_t<bmask_t, value_t> target;
do {
    if (g_state.value > l_value) return;
    target.value = l_value;
    target.bmask = bmask;
} while (!global_state.compare_exchange_weak(g_state, target));
```

### dantzig_bound — upper bound
```cpp
auto predicate = [&](const index_t& i) {
    return i < num_items && tuple.weight < capacity;
};
for (index_t i = height; predicate(i); i++) {
    tuple.value  += tuples[i].value;
    tuple.weight += tuples[i].weight;
}
return tuple.value;
```

### density 정렬 (내림차순, 교차곱)
```cpp
return lhs.value * rhs.weight > rhs.value * lhs.weight;
```

### parallel main
```cpp
ThreadPool TP(2);
TP.spawn(traverse<index_t, tuple_t, bmask_t>, 0, tuple_t(0, 0), 0);
TP.spawn(traverse<index_t, tuple_t, bmask_t>, 0, tuple_t(0, 0), 1);
TP.wait_and_stop();
auto g_state = global_state.load();   // 출력은 g_state.value / g_state.bmask
```

---

## 3. 서술형 한 줄 답

| 질문 | 답 |
|---|---|
| 병렬에서 `sequential_update` 문제 | race condition(나쁜 값이 덮어씀) + torn state(value·bmask 짝 깨짐) |
| value·bmask를 한 구조체로 묶는 이유 | CAS 한 번에 둘을 동시 교체 → torn state 방지 (64bit lock-free) |
| CAS 실패 시 | 교체 안 하고 expected를 최신값으로 갱신 → 루프에서 재검사 |
| `weak` 쓰는 이유 | spurious failure 있어도 루프라 OK, strong보다 빠를 수 있음 |
| `load()` | atomic 값을 원자적으로 통째로 복사 (멤버 직접 접근 불가) |
| 교차곱 이유 | 정수 나눗셈은 소수점 손실 + 나눗셈이 느림 |
| `dantzig_bound`가 upper bound인 이유 | density 순으로 용량까지 채움(마지막은 통째로) ≥ fractional 최적 ≥ 0/1 최적 |
| bound 제거 시 | pruning 약해져 2.4초 → 22초 이상 |
| `[&]` 캡처 이유 | 루프에서 바뀌는 `tuple.weight`를 predicate가 매번 봐야 함 |
| `tuple` 값 전달 이유 | 노드마다 자기 누적합 복사본, 호출 측 상태 보존 |
| 넣는 가지 먼저 탐색 | density 정렬과 합쳐 좋은 해를 일찍 찾음 → bsf↑ → pruning↑ |
| 정렬이 greedy와 다른 점 | greedy는 그 순서로 확정, B&B는 bound·탐색순서 가이드로만 쓰고 양쪽 다 탐색 → 최적 보장 |
| 스레드 2개면 충분한 이유 | spawn하는 작업이 루트 2개뿐 |
| `wait_and_stop()` 없으면 | spawn은 바로 리턴 → 탐색 끝나기 전 측정·출력 |

---

## 4. Super-linear Speedup

- 결과: 2스레드로 **4배** (강의 2.4s → 0.6s / 내 PC 1.16s → 0.21s, 5.5배)
- 원인: 최적해가 "물건 0 넣음" 쪽. sequential은 "안 넣음" 쪽을 먼저 다 뒤짐 → parallel은 동시 탐색으로 좋은 bsf를 일찍 공유 → pruning↑ → **총 작업량 감소**
- **Spurious(가짜)**: 자원 한계 극복이 아니라 탐색 순서 변화. sequential 호출 순서만 바꿔도 빨라짐
- 진짜 super-linear: 데이터 분할 → 코어당 데이터가 캐시에 들어감 → hit ratio↑

---

## 5. 실수 주의

- 람다 끝 `};`
- atomic은 `.load()` 후 멤버 접근
- `&&` 아니라 `&`, `cout << (bmask & 1)` 괄호
- `typename tuple_t::value_t`
- 템플릿 함수 spawn 시 `<index_t, tuple_t, bmask_t>` 명시
- 문제 정의 제약조건은 Σ**W** ≤ c (슬라이드 Σ V는 오타)



# Knapsack 손코딩 빈칸 예상 문제

> 먼저 정답 보지 말고 채워보기. 정답은 맨 아래.

---

## 1. traverse ⭐⭐⭐

```cpp
template<typename index_t, typename tuple_t, typename bmask_t>
void traverse(index_t height, tuple_t tuple, bmask_t bmask) {
    const bool bit = ____(1)____;
    tuple.weight += bit * tuples[height].weight;
    tuple.value  += bit * tuples[height].value;

    if (____(2)____) return;
    atomic_update(tuple, bmask);

    auto bsf = ____(3)____;
    if (____(4)____ < bsf) return;

    if (____(5)____) {
        traverse(height + 1, tuple, ____(6)____);
        traverse(height + 1, tuple, ____(7)____);
    }
}
```

## 2. atomic_update ⭐⭐⭐

```cpp
auto g_state = ____(8)____;
auto l_value = tuple.value;
state_t<bmask_t, value_t> target;

do {
    if (____(9)____) return;
    target.value = l_value;
    target.bmask = bmask;
} while (____(10)____);
```

## 3. dantzig_bound ⭐⭐

```cpp
auto predicate = ____(11)____ (const index_t& i) {
    return ____(12)____;
};
for (index_t i = ____(13)____; predicate(i); i++) {
    tuple.value  += tuples[i].value;
    tuple.weight += tuples[i].weight;
}
return tuple.value;
```

## 4. density 정렬 ⭐⭐

```cpp
auto predicate = [](const auto& lhs, const auto& rhs) -> bool {
    return ____(14)____;
};
```

## 5. parallel main ⭐⭐

```cpp
ThreadPool TP(____(15)____);
TP.spawn(traverse<index_t, tuple_t, bmask_t>, 0, tuple_t(0, 0), ____(16)____);
TP.spawn(traverse<index_t, tuple_t, bmask_t>, 0, tuple_t(0, 0), ____(17)____);
____(18)____;
auto g_state = global_state.load();
```

---
---

## 정답

| # | 정답 | 함정 포인트 |
|---|---|---|
| 1 | `(bmask >> height) % 2` | `height + 1` 아님, 자기 비트 |
| 2 | `tuple.weight > capacity` | `>=` 아님 (딱 맞는 건 허용) |
| 3 | `global_state.load().value` | `.value` 직접 접근 ✗ |
| 4 | `dantzig_bound(height + 1, tuple)` | `height` 아님, **남은 물건**부터 |
| 5 | `height + 1 < num_items` | 마지막 물건이면 분기 X |
| 6 | `bmask + (1 << (height + 1))` | 넣는 가지가 **먼저** |
| 7 | `bmask` | 안 넣음 = 그대로 |
| 8 | `global_state.load()` | 루프 **밖**에서 한 번 |
| 9 | `g_state.value > l_value` | 이미 더 좋으면 포기 |
| 10 | `!global_state.compare_exchange_weak(g_state, target)` | `!` 빠뜨리기 쉬움, 인자 순서 (expected, desired) |
| 11 | `[&]` | `[=]`이면 무게 변화를 못 봄 |
| 12 | `i < num_items && tuple.weight < capacity` | 조건 두 개 다 |
| 13 | `height` | 함수 인자 그대로 (호출 쪽에서 이미 +1) |
| 14 | `lhs.value * rhs.weight > rhs.value * lhs.weight` | `>` = 내림차순, 교차 위치 |
| 15 | `2` | 루트 작업이 2개 |
| 16 | `0` | 물건 0 안 넣음 |
| 17 | `1` | 물건 0 넣음 |
| 18 | `TP.wait_and_stop()` | 없으면 측정·출력이 탐색보다 먼저 |

---

## 많이 틀릴 곳 TOP 5

1. **(10)** `!` 빠뜨리기 — CAS가 **실패하는 동안** 반복하니까 `while (!CAS)`
2. **(4) vs (13)** — 호출할 때 `height + 1`을 넘기고, 함수 안에서는 받은 `height`부터 시작. 둘 다 +1 하면 물건 하나를 건너뜀
3. **(3), (8)** — atomic은 무조건 `load()` 먼저
4. **(6) 순서** — 넣는 가지가 위, 안 넣는 가지가 아래
5. **(11)** `[&]` — 캡처 방식 자체가 빈칸으로 나오기 좋은 자리

---

## 오답 체크

| 회차 | 틀린 번호 |
|---|---|
| 1회 | |
| 2회 | |
| 3회 | |