import sys

import numpy as np


def read_number():
    line = input()
    line = sys.stdin.read()
    yield from np.char.split(line)
    # last_pos = 0
    # pos = 0
    # for pos, c in enumerate(line):
    #     if c == ' ':
    #         yield int(line[last_pos: pos])
    #         last_pos = pos + 1
    # yield int(line[last_pos: pos + 1])


def main():
    n = int(input())
    mab = np.empty((4, n), dtype=int)
    number_input = read_number()
    mab[0] = np.fromiter(read_number(), dtype=int)  # m
    mab[1] = np.fromiter(read_number(), dtype=int)  # a
    mab[2] = np.fromiter(read_number(), dtype=int)  # b
    mab[1:3] -= 1
    min_all = min(mab[0])
    for i in range(n):
        mab[3, mab[2, i]] = mab[1, i]
    vis = np.empty(n, dtype=bool)
    w = 0
    for i in range(n):
        if not vis[i]:
            vis[i] = True
            x = i
            mass_sum = mab[0, x]
            min_c = mab[0, x]
            clen = 1
            x = mab[3, x]
            while vis[x] is not np.True_:
                vis[x] = True
                mass_sum += mab[0, x]
                min_c = min(min_c, mab[0, x])
                clen += 1
                x = mab[3, x]
            w += min(mass_sum + (clen - 2) * min_c,
                     mass_sum + min_c + (clen + 1) * min_all)
    print(w)


if __name__ == '__main__':
    main()
