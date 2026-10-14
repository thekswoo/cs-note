#include <iostream>
#include <vector>
#include <algorithm> 
#include <cstdint> 
#include <random>
#include "hpc_helper.hpp"
using namespace std;

// ========================================================
// struct 선언

template <typename value_t_, typename weight_t_>
struct generic_tuple_t {
    typedef value_t_  value_t;     // uint32_t 대신 템플릿 인자를 씀
    typedef weight_t_ weight_t;

    value_t_  value;
    weight_t_ weight;

    generic_tuple_t(value_t_ v, weight_t_ w) : value(v), weight(w) {}
};

template <typename bmask_t_, typename value_t_>
struct state_t {
    bmask_t_ bmask = 0;
    value_t_ value = 0;
};

// ========================================================
// type 지정

typedef uint64_t index_t;
typedef uint32_t value_t;
typedef uint32_t weight_t;
typedef uint32_t bmask_t;
typedef generic_tuple_t<value_t, weight_t> tuple_t;

// ========================================================
// 전역변수 선언

const index_t  num_items = 32; // 물건 개수 height수
const weight_t capacity  = 1500; // 가방 최대 용량
vector<tuple_t> tuples; // 모든 물건들 정보 저장하는 vector
state_t<bmask_t, value_t> global_state; // 현재상태에서 가장 최고 value와 그 시점의 bmask를 저장하는 구조체


// ========================================================

template <typename tuple_t, typename index_t>
void init_tuples(vector<tuple_t> &tuples, index_t num_entries)
{
    typedef typename tuple_t::value_t value_t;
    typedef typename tuple_t::weight_t weight_t;

    mt19937 engine(0);
    uniform_int_distribution<value_t> rho_v(80, 100);
    uniform_int_distribution<weight_t> rho_w(80, 100);

    for (index_t i = 0; i < num_entries; i++)
    {
        tuples.emplace_back(rho_v(engine), rho_w(engine));
    }

    auto predicate = [](const auto &lhs, const auto &rhs) -> bool
    {
        return lhs.value * rhs.weight > rhs.value * lhs.weight;
    };

    sort(tuples.begin(), tuples.end(), predicate);
}

template <typename tuple_t, typename bmask_t>
void sequential_update(tuple_t tuple, bmask_t bmask)
{
    if (global_state.value < tuple.value) {
        global_state.value = tuple.value;
        global_state.bmask = bmask;
    }
}

template<typename index_t, typename tuple_t>
typename tuple_t::value_t dantzig_bound(index_t height, tuple_t tuple) {
    auto predicate = [&](const index_t& i) {
        return i < num_items && tuple.weight < capacity;
    };

    for(index_t i = height; predicate(i); i++) {
        tuple.value += tuples[i].value;
        tuple.weight += tuples[i].weight;
    }
    return tuple.value;
}

template<typename index_t, typename tuple_t, typename bmask_t>
void traverse(index_t height, tuple_t tuple, bmask_t bmask) {
    const bool bit = (bmask >> height) % 2;
    tuple.weight += bit * tuples[height].weight;
    tuple.value += bit * tuples[height].value;

    if(tuple.weight > capacity) return;
    sequential_update(tuple, bmask);

    auto bsf = global_state.value;
    if(dantzig_bound(height+1, tuple) < bsf) {
        return;
    }

    if(height + 1 < num_items) {
        traverse(height + 1, tuple, bmask + (1 << (height + 1))); // left node 순회 (height+1 번째 물건 넣었을 경우)
        traverse(height + 1, tuple, bmask);
    }
}

int main()
{
    init_tuples(tuples, num_items); // tuples에 난수 생성후 density 기준으로 내림차순 정렬
    TIMERSTART(tra)
    traverse(0, tuple_t(0,0),0); // height 0번째 물건 안 넣었을 경우
    traverse(0, tuple_t(0,0),1); // height 0번째 물건 넣었을 경우 순회 시작
    TIMERSTOP(tra)

    cout << "value " << global_state.value << endl;

    auto bmask = global_state.bmask;
    for(index_t i = 0; i<num_items; i++) {
        cout << bmask % 2;
        bmask >>= 1;
    }
    cout << endl;
}