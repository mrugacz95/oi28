def main():
    n = int(input())
    g = []
    for i in range(n):
        g.append(list(map(int, input().split(' '))))
    cost_tab = []
    for row in g:
        cost_tab.append([row[1] + row[2], row[0] + row[2], row[0] + row[1]])
    global_g = [(0, -1) for _ in range(3)]
    for i in range(n):
        for g_id, (current, (glob_max, glob_i)) in enumerate(zip(g[i], global_g)):
            if current > glob_max:
                global_g[g_id] = (current, i)
    forbidden_indices = list(map(lambda x: x[1], global_g))
    ans = 0
    for i in range(n):
        if i in forbidden_indices:
            pos = forbidden_indices.index(i)
            ans += cost_tab[i][pos]
        else:
            ans += min(cost_tab[i])
    print(ans)


if __name__ == '__main__':
    main()
