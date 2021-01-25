import timeit
from heapq import nlargest
from random import randint, shuffle
from typing import List


def k_max_heap(arr: List[int], k: int):
    return nlargest(k, arr)


def k_max_queue(arr: List[int], k: int):
    q = [0 for _ in range(k)]
    for a in arr:
        i = k - 1
        while i > 0 and q[i - 1] < a:
            q[i] = q[i - 1]
            i -= 1
        if a > q[i]:
            q[i] = a
    return q[:3]


def k_max_sort(arr: List[int], k: int):
    return sorted(arr, reverse=True)[:k]


def k_max_choose(arr: List[int], k: int):
    c = arr.copy()

    def choose_max():
        m_idx = 0
        for i in range(1, len(c)):
            if c[m_idx] < c[i]:
                m_idx = i
        rv = c[m_idx]
        c.pop(m_idx)
        return rv

    return [choose_max() for _ in range(k)]


def test_k_max_times():
    # kMaxQueue([5, 7, 4, 2, 6, 8, 12], 3)
    max_value = 10_000
    arr_range = 100_000
    k = 3
    a = [randint(0, max_value) for i in range(arr_range)]
    for i in range(k):
        a.append(max_value + i + 10)
    shuffle(a)
    print(f"heap: ${k_max_heap(a, k)}")
    print(f"queue: ${k_max_queue(a, k)}")
    print(f"Sort: ${k_max_sort(a, k)}")
    print(f"Choice: ${k_max_choose(a, k)}")
    t = timeit.timeit(lambda: k_max_heap(a, k), number=1000)
    print(f"heap time: {t}")
    t = timeit.timeit(lambda: k_max_queue(a, k), number=1000)
    print(f"k max queue time: {t}")
    t = timeit.timeit(lambda: k_max_sort(a, k), number=1000)
    print(f"sort time: {t}")
    t = timeit.timeit(lambda: k_max_choose(a, k), number=1000)
    print(f"k max choice time: {t}")


test_k_max_times()
