def sumab(a, b):
    ia = int(a)
    ib = int(b)
    ic = ia + ib
    return list(str(ic))


def main():
    n, z = map(int, input().split(' '))
    a = input()
    b = input()
    for i in range(z):
        comm = input()
        # print(comm)
        if comm[0] == 'S':
            c, it = comm.split(' ')
            it = int(it)
            c = sumab(a, b)
            print(c[-it])
        else:
            c, it, new_num = comm.split(' ')
            it = int(it)
            if c == 'W':
                a = a[:- it] + new_num + a[-it:][1:]
            else:
                b = b[:- it] + new_num + b[-it:][1:]
        # print(a)
        # print(b)
        # print(''.join(sumab(a, b)))
        # print()


if __name__ == '__main__':
    main()
