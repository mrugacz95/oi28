def main():
    n,x,z = map(int, input().split())
    for i in range(n):
        holes = list(map(int, input().split(' ')[1:]))
        print(f'{str.rjust(str(i), 2)}: |', end='')
        for p in range(x):
            if p+1 in holes:
                print(' ', end='')
            else:
                print('_', end='')
        print()

if __name__ == '__main__':
    main()
