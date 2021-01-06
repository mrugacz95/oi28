from collections import defaultdict, deque

graph = defaultdict(list)
inv_graph = defaultdict(list)


def zero_one_bfs(n: int, x: int):
    dist = defaultdict(lambda: float('inf'))
    q = deque()
    for i in range(1, n + 1):
        node = (i, x)
        q.append(node)
        dist[node] = 0
    while q:
        current = q.pop()
        for (neighbour, weight) in inv_graph[current]:
            if dist[current] + weight < dist[neighbour]:
                dist[neighbour] = dist[current] + weight
                if weight == 0:
                    q.insert(0, neighbour)
                else:
                    q.append(neighbour)
    return dist


def read_ints():
    return list(map(int, input().split(' ')))


def main():
    n, x, z = read_ints()
    previous = []
    # build graph
    for lvl in range(1, n + 1):
        current = read_ints()[1:]  # read current holes
        last_node = (lvl, 1)
        ci = 0
        pi = 0
        # join holes from current and last platform with two pointers
        for _ in range(len(previous) + len(current)):
            if pi == len(previous) or (ci < len(current)
                                       and current[ci] < previous[pi]):  # choose hole from current platform
                next_hole = current[ci]
                ci += 1
                from_current = True
            else:  # choose hole from previous platform
                next_hole = previous[pi]
                pi += 1
                from_current = False
            if from_current:  # hole is on current platform
                after_hole = (lvl, next_hole + 1)
                below_hole = (lvl + 1, next_hole)
                graph[last_node].append((after_hole, 1))
                graph[last_node].append((below_hole, 0))
                last_node = after_hole
            else:  # hole is above current platform
                below_hole = (lvl, next_hole)
                after_hole = (lvl - 1, next_hole + 1)
                graph[last_node].append((below_hole, 0))
                graph[last_node].append((after_hole, 1))
                last_node = below_hole
        if last_node[1] != x:  # if last node is not utmost
            graph[last_node].append(((lvl, x), 0))
        previous = current  # move holes from current platform to previous platform
    # invert graph
    for node, neighbours in graph.items():
        for neighbour, weight in neighbours:
            inv_graph[neighbour].append((node, weight))
    # precalculate distances
    distances = zero_one_bfs(n, x)
    # answer questions
    for i in range(z):
        lvl = int(input())
        print(distances[(lvl, 1)])


if __name__ == '__main__':
    main()
