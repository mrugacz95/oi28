from collections import defaultdict, deque
from math import sqrt, floor
from typing import Tuple, List, Optional, Set


def read_ints():
    return list(map(int, input().split(" ")))


MAX_N = 100010
MAX_M = 150010
MAX_Z = 150010

state = defaultdict(int)

visited: List[bool] = [False] * MAX_N
child_node_id_to_edge_id = [-1] * MAX_N
tree = defaultdict(list)
dfs_order: List[int] = [-1] * (2 * MAX_N)
preorder: List[Optional[int]] = [-1] * MAX_N
parent: List[int] = [-1] * MAX_N
edges: List[Tuple[int, int]] = []
edges_content: List[int] = [-1] * MAX_N


def dfs(start_node: int):
    """
    We need iterative version as recursive version throws RecursionError :<
    """
    counter = 0
    q = deque([start_node])
    while len(q) != 0:
        node = q.popleft()
        if preorder[node] == -1:  # not visited
            q.appendleft(node)  # add again to calculate exit time
            dfs_order[counter] = node  # enter node
            preorder[node] = counter  # store preorder
            counter += 1
            for neighbour in tree[node]:
                if preorder[neighbour] == -1:  # not visited yet and not root
                    parent[neighbour] = node  # mark parent
                    q.appendleft(neighbour)
        else:
            dfs_order[counter] = node  # exit node
            counter += 1


def get_child(edge: Tuple[int, int]):
    return edge[0] if (parent[edge[0]] == edge[1]) else edge[1]


def calculate_child_node_id_to_end_id():
    for edge_id, edge in enumerate(edges):
        child = get_child(edge)
        child_node_id_to_edge_id[child] = edge_id


def edge_in_path(edge: Tuple[int, int], path: Set[int]) -> bool:
    return edge[0] in path and edge[1] in path


def main():
    n, m, z = read_ints()
    # read tree structure
    for i in range(n - 1):
        edge_from, edge_to, mascot = read_ints()
        edge_from, edge_to = edge_from - 1, edge_to - 1  # decrease
        edges.append((edge_from, edge_to))
        edges_content[i] = mascot - 1  # decrease for zero-based indexing
        tree[edge_from].append(edge_to)
        tree[edge_to].append(edge_from)

    # process tree
    dfs(0)
    calculate_child_node_id_to_end_id()

    # read queries and changes
    queries: List[Tuple[int, int]] = []
    changes: List[Tuple[int, int, int]] = []

    time = 0
    for i in range(z):
        query = input().split(" ")
        if query[0] == 'Z':  # query
            node = int(query[1]) - 1  # decrease for zero-based indexing
            queries.append((time, node))
        else:  # 'B' change mascot on edge
            edge_id, next_mascot = map(lambda x: int(x) - 1, query[1:])  # decrease for zero-based indexing
            current_mascot = edges_content[edge_id]
            changes.append((edge_id, current_mascot, next_mascot))
            edges_content[edge_id] = next_mascot
            time += 1

    def add_node_to_path(node_id: int):
        edge_id = child_node_id_to_edge_id[node_id]
        mascot = edges_content[edge_id]
        state[mascot] += 1
        visited[node_id] = True
        return node_id

    def remove_node_from_path(node_id: int):
        edge_id = child_node_id_to_edge_id[node_id]
        mascot = edges_content[edge_id]
        state[mascot] -= 1
        if state[mascot] == 0:
            state.pop(mascot)
        visited[node_id] = False
        return parent[node_id]

    # sort queries
    sqrt_n = sqrt(n)
    ordered_queries = sorted(range(len(queries)),
                             key=lambda q: (floor(queries[q][0] / sqrt_n), preorder[queries[q][1]]))

    # move through (t,v) space
    current_t = time  # current tree state is same as in finish time
    current_v = 0  # current dfs order, 1 root is already added
    current_node = 0  # current node, 0 because we start from root

    answers: List[int] = [0] * len(queries)  # init answers array as result will be calculated out of order
    for q_idx in ordered_queries:
        q_time, q_node = queries[q_idx]
        while current_v < preorder[q_node]:  # need to move dfs forward
            next_node = dfs_order[current_v + 1]  # node id, added/removed
            if not visited[next_node]:  # go down the tree
                current_node = add_node_to_path(next_node)
            else:  # return to node's parent
                current_node = remove_node_from_path(current_node)
            current_v += 1
        while current_v > preorder[q_node]:  # need to revert dfs
            last_node = dfs_order[current_v]  # node id, added/removed
            if visited[last_node]:  # last node was added, so to reverse it we have to remove it
                current_node = remove_node_from_path(current_node)
            else:
                current_node = add_node_to_path(last_node)
            current_v -= 1
        while current_t < q_time:  # shift state to future time
            edge_id, prev_mascot, next_mascot = changes[current_t]  # change mascot prev -> next on edge
            edge = edges[edge_id]
            child = get_child(edge)
            if visited[child]:  # update state if change is in current path
                state[prev_mascot] -= 1
                if state[prev_mascot] == 0:
                    state.pop(prev_mascot)
                state[next_mascot] += 1
            edges_content[edge_id] = next_mascot
            current_t += 1
        while current_t > q_time:  # shift state to past time
            edge_id, prev_mascot, next_mascot = changes[current_t - 1]  # change mascot next -> prev on edge
            edge = edges[edge_id]
            child = get_child(edge)
            if visited[child]:  # update state if change is in current path
                state[next_mascot] -= 1
                if state[next_mascot] == 0:
                    state.pop(next_mascot)
                state[prev_mascot] += 1
            edges_content[edge_id] = prev_mascot
            current_t -= 1
        answers[q_idx] = len(state)  # store number of distinct mascots in path
    for answer in answers:
        print(answer)


if __name__ == '__main__':
    main()
