from typing import List


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
            self.tree[i] = max(self.tree[i * 2], self.tree[(i * 2) + 1])

    def update(self, idx: int, value: int):
        current = self.n + idx
        self.tree[current] = value
        while current > 1:
            self.tree[current // 2] = max(self.tree[current], self.tree[current ^ 1])
            current //= 2

    def query(self, left: int, right: int):
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
    s = []
    for i in range(n):
        s.append(a[i] + b[i])
    tree = SegmentTree([idx if x != 9 else -1 for idx, x in enumerate(s)])
    for _ in range(z):
        command = input().split(" ")
        numbers = list(map(int, command[1:]))
        command = command[0]
        if command == "W" or command == "Z":
            arr = a if command == "W" else b
            i, c = numbers
            i -= 1
            arr[i] = c
            s[i] = a[i] + b[i]
            if s[i] == 9:
                tree.update(i, -1)
            else:
                tree.update(i, i)
        elif command[0] == "S":
            i = numbers[0] - 1
            lb = tree.query(0, i)
            lb = s[lb] if lb > -1 else 0
            if lb <= 8:
                print(s[i] % 10)
            else:
                print((s[i] + 1) % 10)


if __name__ == '__main__':
    main()
