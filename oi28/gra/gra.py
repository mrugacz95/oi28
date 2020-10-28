from collections import defaultdict

graph = defaultdict(list)

n, x, z = 0, 0, 0


def find_next(from_node, lvl):
    closest = x
    # connect all on one lvl
    for (s_lvl, s_pos), neighbours in graph.items():
        if lvl == s_lvl and from_node < s_pos < closest:
            closest = s_pos
    return closest


def dfs(start_node):
    q = [n for n in graph[start_node]]
    visited = set()
    min_c = float('inf')
    while q:
        c_y, c_x, c_c = q.pop(0)
        if c_x == x:
            min_c = min(c_c, min_c)
        if (c_y, c_x) in visited:
            continue
        visited.add((c_y, c_x))
        for n_y, n_x, n_c in graph[(c_y, c_x)]:
            q.append((n_y, n_x, n_c + c_c))
    return min_c


def main():
    global n, x, z
    n, x, z = map(int, input().split(' '))
    for lvl in range(n):
        holes = list(map(lambda  x: int(x)-1, input().split(' ')))
        holes.pop(0)
        for h in holes:
            graph[(lvl, h - 1)].append((lvl + 1, h, 0))  # fall
            graph[(lvl + 1, h)].append((lvl, h + 1, 1))  # jump up
            graph[(lvl, h - 1)].append((lvl, h + 1, 1))  # jump over
            if (lvl, h + 1) not in graph and h + 1 != x:
                graph[(lvl, h + 1)] = []
            if (lvl + 1, h) not in graph:
                graph[(lvl + 1, h)] = []
    for lvl in range(n):
        next_node = 0
        from_node = 0
        while from_node != x:
            next_node = find_next(next_node, lvl)
            if (lvl, next_node) not in graph[(lvl, from_node)]:
                graph[(lvl, from_node)].append((lvl, next_node, 0))
            from_node = next_node
    for _ in range(z):
        start_pos = int(input())
        print(dfs((start_pos - 1, 0)))


if __name__ == '__main__':
    main()
