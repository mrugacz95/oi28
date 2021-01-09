from collections import defaultdict, deque
from math import sqrt, floor
from typing import Tuple, List, Optional, Set


def read_ints():
    return list(map(int, input().split(" ")))


class State:
    def __init__(self, n: int):
        self.size = 0
        self._state = [0] * n

    def add(self, x: int):
        if self._state[x] == 0:
            self.size += 1
        self._state[x] += 1

    def delete(self, x: int):
        self._state[x] -= 1
        if self._state[x] == 0:
            self.size -= 1


counter = 0
tree = defaultdict(list)
dfs_order: List[Tuple[int, bool]] = []
preorder: List[Optional[int]]
parent: List[int]


def get_edge(current_node, next_node) -> Tuple[int, int]:
    return (next_node, current_node) if next_node < current_node else (current_node, next_node)


def dfs(start_node: int):  # we need iterative version as recursive version throws RecursionError :<
    global counter, dfs_order
    q = deque([start_node])
    while len(q) != 0:
        node = q.popleft()
        if preorder[node] == -1:  # not visited
            q.appendleft(node)  # add again to calculate exit time
            dfs_order[counter] = (node, True)  # enter node
            preorder[node] = counter  # store preorder
            counter += 1
            for neighbour in tree[node]:
                if preorder[neighbour] == -1:  # not visited yet
                    parent[neighbour] = node  # mark parent
                    q.appendleft(neighbour)
        else:
            dfs_order[counter] = (node, False)  # exit node
            counter += 1


def edge_in_path(edge: Tuple[int, int], path: Set[int]) -> bool:
    return edge[0] in path and edge[1] in path


def main():
    n, m, z = read_ints()
    state = State(m)
    edges = {}
    edges_content = {}
    # read tree structure
    for i in range(n - 1):
        edge_from, edge_to, mascot = read_ints()
        edge_from, edge_to = edge_from - 1, edge_to - 1  # decrease
        edge = get_edge(edge_from, edge_to)
        edges[i] = edge
        edges_content[edge] = mascot - 1  # decrease for zero-based indexing
        tree[edge_from].append(edge_to)
        tree[edge_to].append(edge_from)
    queries: List[Tuple[int, int]] = []
    changes: List[Tuple[int, int, int]] = []
    # read queries
    time = 0
    for i in range(z):
        query = input().split(" ")
        if query[0] == 'Z':  # query
            node = int(query[1]) - 1  # decrease for zero-based indexing
            queries.append((time, node))
        else:  # q[0] == 'B' change mascot on edge
            time += 1
            edge_id, next_mascot = map(lambda x: int(x) - 1, query[1:])  # decrease for zero-based indexing
            edge = edges[edge_id]
            current_mascot = edges_content[edge]
            changes.append((edge_id, current_mascot, next_mascot))
            edges_content[edge] = next_mascot

    # init arrays
    global preorder, parent, dfs_order
    preorder = [-1] * n
    parent = [0] * n
    parent[0] = 0
    dfs_order = [(-1, False)] * (2 * n)  # 0(2n) memory, we can afford it, but we need to preallocate it
    # assign node ids, determine parent
    dfs(0)

    # move through (t,v) space
    current_t = time  # current tree state is same as in finish time
    current_v = 0  # current dfs order, 1 root is already added
    current_node = 0  # current node, 0 because we start from root
    current_path = {0}  # current path, only 0 because we start from root

    def add_node_to_path(node_to_add: int):
        nonlocal current_node
        edge = get_edge(node_to_add, current_node)
        mascot_to_add = edges_content[edge]
        state.add(mascot_to_add)
        current_path.add(node_to_add)
        current_node = node_to_add

    def remove_last_node_from_path():
        nonlocal current_node
        edge = get_edge(current_node, parent[current_node])
        mascot_to_del = edges_content[edge]
        state.delete(mascot_to_del)
        current_path.remove(current_node)
        current_node = parent[current_node]

    # sort queries
    sqrt_n = sqrt(n)
    # as current tree state is in last time stamp, we will sort by -t/sqrt_n
    ordered_queries = map(lambda x: x[2], sorted((-floor(q_t / sqrt_n), q_v, q_idx)
                                                 for q_idx, (q_t, q_v) in enumerate(queries)))

    answers: List[int] = [0] * len(queries)  # init answers array as result will be calculated out of order
    for q_idx in ordered_queries:
        qt, qv = queries[q_idx]
        while current_t < qt:  # shift state to future time
            edge_id, prev_mascot, next_mascot = changes[current_t]  # change mascot prev -> next on edge
            edge = edges[edge_id]
            if edge_in_path(edge, current_path):  # update state if change is in current path
                state.delete(prev_mascot)
                state.add(next_mascot)
            edges_content[edge] = next_mascot
            current_t += 1
        while current_t > qt:  # shift state to past time
            edge_id, prev_mascot, next_mascot = changes[current_t - 1]  # change mascot next -> prev on edge
            edge = edges[edge_id]
            if edge_in_path(edge, current_path):  # update state if change is in current path
                state.delete(next_mascot)
                state.add(prev_mascot)
            edges_content[edge] = prev_mascot
            current_t -= 1
        while current_v < preorder[qv]:  # need to move dfs forward
            next_node, next_operation = dfs_order[current_v + 1]  # node id, added/removed
            if next_operation:  # go down the tree
                add_node_to_path(next_node)
            else:  # return to node's parent
                remove_last_node_from_path()
            current_v += 1
        while current_v > preorder[qv]:  # need to revert dfs
            last_node, last_operation = dfs_order[current_v]  # node id, added/removed
            if last_operation:  # last node was added, so to reverse it we have to remove it
                remove_last_node_from_path()
            else:
                add_node_to_path(last_node)
            current_v -= 1
        answers[q_idx] = state.size  # store number of distinct mascots in path
    for answer in answers:
        print(answer)


if __name__ == '__main__':
    main()
