from collections import defaultdict, deque
from typing import Tuple, List

graph = []
inv_graph = []
holes_count = 0
holes_start = []
holes_end = []


def zero_one_bfs():
    dist = [float('inf')] * (holes_count + 1)  # init with infinity
    q = deque()
    for i in holes_end:
        q.append(i)
        dist[i] = 0
    while q:
        current = q.popleft()
        for (neighbour, weight) in inv_graph[current]:
            if dist[current] + weight < dist[neighbour]:
                dist[neighbour] = dist[current] + weight
                if weight == 0:
                    q.appendleft(neighbour)
                else:
                    q.append(neighbour)
    return dist


def read_ints():
    return list(map(int, input().split(' ')))


def main():
    global holes_count, holes_start, holes_end, inv_graph
    n, x, z = read_ints()
    previous = []
    # build graph
    holes_start = [0] * n
    holes_end = [0] * n
    for platform_id in range(n):
        last_node = holes_count
        holes_count += 1
        holes_start[platform_id] = last_node
        graph.append([])
        current = []  # read current holes
        for hole_id, hole in enumerate(read_ints()[1:]):
            current.append((hole_id + holes_count, hole))
        holes_count += len(current)
        ci = 0  # current holes iterator
        pi = 0  # previous holes iterator
        # join holes from current and last platform with two pointers
        for _ in range(len(previous) + len(current)):
            if pi == len(previous) or (ci < len(current)
                                       and current[ci][1] < previous[pi][1]):  # choose hole from current platform
                next_hole = current[ci]
                ci += 1
                from_current = True
            else:  # choose hole from previous platform
                next_hole = previous[pi]
                pi += 1
                from_current = False
            if from_current:  # hole is on current platform
                after_hole = next_hole[0]
                graph[last_node].append((after_hole, 1))
                last_node = after_hole
                graph.append([])
            else:  # hole is above current platform
                after_hole = next_hole[0]
                before_hole = after_hole - 1
                graph[before_hole].append((last_node, 0))
                graph[last_node].append((after_hole, 1))
        holes_end[platform_id] = last_node
        previous = current  # move holes from current platform to previous platform
    # invert graph
    inv_graph: List[List[Tuple[int, int]]] = [[] for _ in range(len(graph))]
    for node, neighbours in enumerate(graph):
        for neighbour, weight in neighbours:
            inv_graph[neighbour].append((node, weight))
        graph[node].clear()
    graph.clear()
    # precalculate distances
    distances = zero_one_bfs()
    # answer questions
    for i in range(z):
        platform_id = int(input()) - 1
        print(distances[holes_start[platform_id]])


if __name__ == '__main__':
    main()
