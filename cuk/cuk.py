import itertools
import timeit
from heapq import nlargest, nsmallest
from random import randint, shuffle
from typing import List, Tuple


def main():
    n = int(input())
    arr: List[List[int]] = []
    m: List[int] = [0] * n  # max value for every row
    has_values = [False] * 3  # stores info if there is any non-zero value in each column
    # read input
    for col_id in range(n):
        row = list(map(int, input().split(" ")))  # read row
        m[col_id] = max(row)  # store max value
        arr.append(row)  # add row to array
        for j, value in enumerate(row):  # check if any value is 0 for each column
            has_values[j] |= value != 0
    initial_cost = 0
    # calculate initial cost if only max values would stay at each row
    for col_id in range(n):
        for j in range(3):
            initial_cost += arr[col_id][j]
        initial_cost -= m[col_id]
    min_cost = float("inf")
    # find indices of max values in each column
    max_values: List[List[int]] = []
    for col_id in range(3):
        indices = nsmallest(3, range(n), key=lambda r_id: m[r_id] - arr[r_id][col_id])  # select row ids with less cost
        max_values.append(indices)
    # check every permutation of column indices to choose best rows
    for rows_ids in itertools.product(*max_values):
        if len(set(rows_ids)) != 3:  # if rows indices are distinct
            continue
        # calculate new cost if rows indices would be chosen
        new_cost = initial_cost
        for col_id, row_id in enumerate(rows_ids):
            if has_values[col_id]:
                new_cost += m[row_id] - arr[row_id][col_id]  # plus max values, minus chosen column
        if min_cost > new_cost:  # save cost if minimal
            min_cost = new_cost
    print(min_cost)


if __name__ == '__main__':
    main()