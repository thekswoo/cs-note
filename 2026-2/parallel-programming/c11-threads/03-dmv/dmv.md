### 한 줄 요약

`Block`은 캐시에 좋고, `Cyclic`은 부하 분산에 좋지만 `false sharing` 위험이 있다.

`Block-Cyclic`은 `chunk_size = 64/sizeof(value_t)`로 cache line 단위로 나눠서 둘의 장점을 함께 챙긴다. 그리고 공유 배열에는 `accum`에 모았다가 한 번만 쓰자.