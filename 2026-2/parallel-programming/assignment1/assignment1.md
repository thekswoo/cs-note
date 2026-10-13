# Parallel Programming 과제 1: C++ Multithreading

## 목표
C++11로 Monte Carlo 시뮬레이션을 구현해서 $\pi$를 근사하고, 4가지 구현 방식의 **성능과 정확도**를 비교·분석한다.

## 원리
$[0,1] \times [0,1]$ 정사각형에 랜덤 점을 찍으면, 점이 반지름 1인 1/4 원 안에 들어갈 확률은 $\pi / 4$이다.
따라서 $N$개 중 원 안에 들어간 개수 $count$를 세면 다음과 같이 근사할 수 있다.

$$\pi \approx 4 \times \frac{count}{N}$$

## 파라미터
| 항목 | 값 |
|---|---|
| $N$ | $10^8$ |
| 기준 $\pi$ | 3.14159265358979323846 |
| 스레드 수 $T$ | 2, 4, 8, 16, 32 |

> [!note]
> Colab CPU 환경은 코어가 **2개**뿐이다.

## 구현할 4가지 버전
1. **Single Thread**
   - $N$개 점 $(x, y)$를 생성하고, $x^2 + y^2 \le 1$이면 `count`를 증가시킨다.
2. **Multithread (`local_count`)**
   - $N$을 $T$개 스레드에 균등하게 나눈다.
   - 각 스레드가 독립적으로 자기 `local_count`를 계산한다.
   - 모든 스레드가 끝나면 합쳐서 `global_count`를 만든다.
3. **Multithread (`global_count`, mutex 없음)**
   - `local_count` 없이 모든 스레드가 공유 변수 `global_count`를 직접 증가시킨다.
   - 결과가 어떻게 달라지는지 관찰하는 것이 목적이다.
4. **Multithread (`global_count`, mutex 사용)**
   - `global_count`를 갱신할 때 `mutex`로 상호배제를 건다.

## 측정 항목
- 실행 시간
- $\pi_{estimated} = 4 \times \dfrac{count}{N}$
- $Error = |\pi_{estimated} - \pi|$
- $Error\ Rate(\%) = 100 \times \dfrac{|\pi_{estimated} - \pi|}{\pi}$

## 제출물
1. **Colab 코드 파일**
2. **보고서 (A4 1장)**
   - 4가지 구현의 결과
   - 스레드 수가 늘어날 때 실행 시간 변화 분석
     - 시간이 안 줄거나 오히려 늘면 원인을 논의
     - **하드웨어 측면**: 코어 2개, 캐시 등
     - **소프트웨어 측면**: 스레드 생성 및 context switching 오버헤드, 동기화 비용 등
   - mutex 없이 `global_count`를 공유할 때 생기는 문제(race condition) 분석
   - mutex를 적용한 경우와 결과를 자세히 비교

## 제출 방법
- **마감**: 10/24 23:55 (지각 제출 불가)
- Colab 파일 + 보고서 PDF를 **zip 하나**로 묶어서 I-Class에 제출

> [!warning]
> 슬라이드에는 "2025/10/24 Friday"로 적혀 있지만 2026년 10/24는 토요일이다. 실제 마감일은 I-Class에서 확인할 것.


## 결과

### colab 환경
```bash
===========================================
[Result] single thread

Number of threads = 1
#time (counter): 2.7972584190s
#count        = 78537856
#estimated pi = 3.1415142400
#error        = 0.0000784136
#error rate   = 0.0024959821 %

===========================================
[Result] multi thread by local count

Number of threads = 2
#time (counter): 2.1037537120s
#count        = 78540214
#estimated pi = 3.1416085600
#error        = 0.0000159064
#error rate   = 0.0005063168 %

Number of threads = 4
#time (counter): 2.2060252320s
#count        = 78534440
#estimated pi = 3.1413776000
#error        = 0.0002150536
#error rate   = 0.0068453684 %

Number of threads = 8
#time (counter): 3.4550341010s
#count        = 78540953
#estimated pi = 3.1416381200
#error        = 0.0000454664
#error rate   = 0.0014472408 %

Number of threads = 16
#time (counter): 2.3015422030s
#count        = 78541897
#estimated pi = 3.1416758800
#error        = 0.0000832264
#error rate   = 0.0026491789 %

Number of threads = 32
#time (counter): 2.1339831740s
#count        = 78543106
#estimated pi = 3.1417242400
#error        = 0.0001315864
#error rate   = 0.0041885255 %

===========================================
[Result] multi thread by global count and no mutex

Number of threads = 2
#time (counter): 2.4578805880s
#count        = 75616750
#estimated pi = 3.0246700000
#error        = 0.1169226536
#error rate   = 3.7217636556 %

Number of threads = 4
#time (counter): 2.3908780300s
#count        = 75539537
#estimated pi = 3.0215814800
#error        = 0.1200111736
#error rate   = 3.8200743006 %

Number of threads = 8
#time (counter): 3.1018618830s
#count        = 76526613
#estimated pi = 3.0610645200
#error        = 0.0805281336
#error rate   = 2.5632901038 %

Number of threads = 16
#time (counter): 3.1592232420s
#count        = 76684026
#estimated pi = 3.0673610400
#error        = 0.0742316136
#error rate   = 2.3628656473 %

Number of threads = 32
#time (counter): 2.5338366690s
#count        = 75551721
#estimated pi = 3.0220688400
#error        = 0.1195238136
#error rate   = 3.8045611500 %

===========================================
[Result] multi thread by global count and yes mutex

Number of threads = 2
#time (counter): 4.7959720220s
#count        = 78540214
#estimated pi = 3.1416085600
#error        = 0.0000159064
#error rate   = 0.0005063168 %

Number of threads = 4
#time (counter): 5.6694773090s
#count        = 78534440
#estimated pi = 3.1413776000
#error        = 0.0002150536
#error rate   = 0.0068453684 %

Number of threads = 8
#time (counter): 4.7337433910s
#count        = 78540953
#estimated pi = 3.1416381200
#error        = 0.0000454664
#error rate   = 0.0014472408 %

Number of threads = 16
#time (counter): 4.9075321230s
#count        = 78541897
#estimated pi = 3.1416758800
#error        = 0.0000832264
#error rate   = 0.0026491789 %

Number of threads = 32
#time (counter): 5.6249267930s
#count        = 78543106
#estimated pi = 3.1417242400
#error        = 0.0001315864
#error rate   = 0.0041885255 %
```