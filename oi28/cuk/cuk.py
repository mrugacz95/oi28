import itertools
import timeit
from heapq import nlargest
from random import randint, shuffle
from typing import List, Tuple


def k_max(arr: List[List[int]], col: int, k: int):
    heap = [(x[col], i) for i, x in enumerate(arr)]
    return list(map(lambda x: x[1], nlargest(k, heap)))


def main():
    n = int(input())
    arr: List[List[int]] = []
    m: List[int] = []
    has_values = [False for _ in range(3)]  # stores info if there is any non-zero value in each column
    # read input
    for i in range(n):
        row = list(map(int, input().split(" ")))  # read row
        m.append(max(row))  # add max value
        arr.append(row)  # add row to array
        for j, value in enumerate(row):  # check if any value is 0 for each column
            has_values[j] |= value != 0
    initial_cost = 0
    # calculate initial cost if only max values would stay at each row
    for i in range(n):
        for j in range(3):
            initial_cost += arr[i][j]
        initial_cost -= m[i]
    min_cost = float("inf")
    # find indices of max values in each column
    max_values = [k_max(arr, i, 3) for i in range(3)]
    # check every permutation of column indices to choose best rows
    for rows_ids in itertools.product(*max_values):
        if len(set(rows_ids)) != 3:  # if rows indices are distinct
            continue
        # calculate new cost if rows indices would be chosen
        new_cost = initial_cost
        for i, row_id in enumerate(rows_ids):
            if has_values[i]:
                new_cost += m[row_id] - arr[row_id][i] # minus max values, plus chosen column
        if min_cost > new_cost:  # save cost if minimal
            min_cost = new_cost
    print(min_cost)


if __name__ == '__main__':
    main()
