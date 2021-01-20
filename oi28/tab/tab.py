def read_ints():
    return list(map(int, input().split(" ")))


def main():
    n, m, q = read_ints()
    arr = [[False] * m for _ in range(n)]
    size = 0
    for _ in range(q):
        y1, x1, y2, x2 = list(map(lambda x: x - 1, read_ints()))

        def flip(y: int, x: int):
            nonlocal size
            if arr[y][x]:
                size -= 1
            else:
                size += 1
            arr[y][x] ^= True

        flip(y2, x2)
        if x1 != 0:
            flip(y2, x1 - 1)
        if y1 != 0:
            flip(y1 - 1, x2)
        if x1 != 0 and y1 != 0:
            flip(y1 - 1, x1 - 1)

        print(size)


if __name__ == '__main__':
    main()
