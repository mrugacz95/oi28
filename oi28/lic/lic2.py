from bisect import bisect_right
from math import log
from typing import List, Callable, Optional


def read_ints():
    return list(map(int, input().split(" ")))


def left_child(idx: int):
    return idx * 2


def right_child(idx: int):
    return idx * 2 + 1


class SegmentTree(object):

    def __init__(self, leaves: List[int]):
        self.initial = -1
        self.n = len(leaves)
        self.tree = [self.initial] * self.n + leaves
        # fill rest of nodes with max of children
        for i in reversed(range(self.n)):
            self.tree[i] = max(self.tree[i* 2], self.tree[(i * 2) + 1])

    def update(self, idx, value):
        current = self.n + idx
        self.tree[current] = value
        while current > 1:
            self.tree[current // 2] = max(self.tree[current], self.tree[current ^ 1])
            current //= 2

    def query(self, left, right):
        left += self.n
        right += self.n
        m = self.initial
        while left < right:
            if left & 1:
                m = max(m, self.tree[left])
                left += 1
            if right & 1:
                right -= 1
                m = max(m, self.tree[right])
            left //= 2
            right //= 2
        return m


def main():
    n, z = map(int, input().split(" "))
    a = list(map(int, reversed(input())))
    a += [0] * (n - len(a))
    b = list(map(int, reversed(input())))
    b += [0] * (n - len(b))
    sum = []
    for i in range(n):
        sum.append(a[i] + b[i])
    tree = SegmentTree([idx if x != 9 else -1 for idx, x in enumerate(sum)])
    # print(a, b, sum)
    for _ in range(z):
        command = input().split(" ")
        # print(command)
        numbers = list(map(int, command[1:]))
        command = command[0]
        if command == "W" or command == "Z":
            arr = a if command == "W" else b
            i, c = numbers
            i -= 1
            arr[i] = c
            sum[i] = a[i] + b[i]
            if sum[i] == 9:
                tree.update(i, -1)
            else:
                tree.update(i, i)
        elif command[0] == "S":
            # real_a = "".join(map(str,reversed(a)))
            # real_b = "".join(map(str,reversed(b)))
            # real_sum = int(real_a) + int(real_b)
            # print(f'a: {real_a} {a}')
            # print(f'b: {real_b} {b}')
            # print(f's: {real_sum} {sum}')
            # print(f't: {tree.tree[n:]}')
            i = numbers[0] - 1
            # print(f"query {0}-{i}")
            # print(tree.tree)
            lb = tree.query(0, i)
            # print(f"lb idx: {lb}")
            lb = sum[lb] if lb > -1 else 0
            # print(f"lb: {lb}")
            # print(f"should be {str(real_sum)[-i-1]}")
            if lb <= 8:
                print(sum[i] % 10)
            else:
                print((sum[i] + 1) % 10)


def test_tree():
    # tree = SegmentTree(8)
    # tree.insert(3)
    # tree.insert(5)
    # print(tree.lower_bound(7))
    # tree.erase(5)
    # print(tree.lower_bound(7))

    tree = SegmentTree([0, 1, 2, 0, 4, 5, 6, 7, 8, 9])
    print(tree.tree)
    print(tree.query(0, 10))
    print(tree.query(0, 4))


if __name__ == '__main__':
    main()
