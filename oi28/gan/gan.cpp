#include <vector>
#include <iostream>
#include <tuple>
#include <deque>
#include <set>
using namespace std;

typedef pair<int, bool> pib;
typedef pair<int, int> pii;

struct Query
{
    int time, nodeId;
};

struct Change
{
    int edgeId, previusMascot, nextMascot;
};

struct DfsMove
{
    int nodeId, enter;
};

vector<int> state;
int stateSize = 0;
const int MAX_N = 100000,
          MAX_M = 150000,
          MAX_Z = 150000;
vector<int> tree[MAX_N];
int edgeContent[MAX_N * 2];
vector<Query> queries;
vector<Change> changes;
int preorder[100000];
int parent[100000];
int preorderCounter = 0;
vector<DfsMove> dfsOrder;
vector<pii> edges;
set<int> visitedEdges;
set<int> visitedNodes;
vector<int> childNodeIdToEdgeId;

void stateAdd(int x)
{
    cout << "state add " << x << "\n";
    if (state[x] == 0)
    {
        stateSize++;
    }
    state[x]++;
}

void stateDel(int x)
{
    cout << "state del " << x << "\n";
    state[x]--;
    if (state[x] == 0)
    {
        stateSize--;
    }
}

void dfs(int startNode)
{
    cout << "startNode in dfs" << startNode << "\n";
    dfsOrder.push_back({startNode, true});
    for (int edgeId : tree[startNode])
    {
        pii edge = edges[edgeId];
        int neighbour = (edge.first == startNode) ? edge.second : edge.first;
        if (preorder[neighbour] == 0 && neighbour != 0)
        {
            preorder[neighbour] = ++preorderCounter;
            parent[neighbour] = startNode;
            dfs(neighbour);
        }
    }
    preorderCounter++;
    dfsOrder.push_back({startNode, false});
}

void calulateChildNodeIdToEdgeId()
{
    for (int i = 0; i < edges.size(); i++)
    {
        auto edge = edges[i];
        int child;
        if (parent[edge.first] == edge.second)
        {
            child = edge.first;
        }
        else
        {
            child = edge.second;
        }
        childNodeIdToEdgeId[child] = i;
    }
}

int addEdge(int nodeId)
{
    cout << "addEgde(" << nodeId <<")\n";
    int edgeId = childNodeIdToEdgeId[nodeId];
    cout << "edge Id " << edgeId << "\n";
    int mascot = edgeContent[edgeId];
    stateAdd(mascot);
    cout << "add mascot:" << mascot << "\n";
    visitedNodes.insert(nodeId);
    visitedEdges.insert(edgeId);
    cout << "new current node" << nodeId << "\n";
    return nodeId;
}

int delEdge(int lastNodeId)
{
    cout << "delEgde(" << lastNodeId <<")\n";
    int edgeId = childNodeIdToEdgeId[lastNodeId];
    cout << "edge Id " << edgeId << "\n";
    int mascot = edgeContent[edgeId];
    stateDel(mascot);
    cout << "del mascot:" << mascot << "\n";
    visitedNodes.erase(lastNodeId);
    visitedEdges.erase(edgeId);
    cout << "new current node " << parent[lastNodeId] << "\n";
    return parent[lastNodeId];
}

int main(int argc, char const *argv[])
{
    int n, m, z;
    cin >> n >> m >> z;
    state.reserve(m);
    queries.reserve(z);
    changes.reserve(z);
    edges.resize(n - 1);
    childNodeIdToEdgeId.resize(n - 1);
    for (int i = 0; i < n - 1; i++)
    {
        int nodeFrom, nodeTo, mascotId;
        cin >> nodeFrom >> nodeTo >> mascotId;
        nodeFrom--;
        nodeTo--;
        edgeContent[i] = mascotId;
        edges[i] = {nodeFrom, nodeTo};
        tree[nodeFrom].push_back(i); // store edge id instead of node id
        tree[nodeTo].push_back(i);
    }
    // process tree
    parent[0] = -1;
    dfs(0);
    childNodeIdToEdgeId[0] = -1;
    calulateChildNodeIdToEdgeId();
    cout << "parent ";
    for (int i = 0; i < n; i++)
    {
        cout << parent[i] << " ";
    }
    cout << "\n";
    cout << "preorder ";
    for (int i = 0; i < n; i++)
    {
        cout << preorder[i] << " ";
    }
    cout << "\n";

    cout << "dfs order ";
    for (int i = 0; i < dfsOrder.size(); i++)
    {
        cout << " (" << ((dfsOrder[i].enter) ? 't' : 'f') << " " << dfsOrder[i].nodeId << ")";
    }
    cout << "\n";

    // read queries
    int time = 0;
    for (int i = 0; i < z; i++)
    {
        char type;
        cin >> type;
        cout << "type " << type << "\n";
        switch (type)
        {
        case 'Z':
            int query;
            cin >> query;
            query--;
            queries.push_back({time, query});
            break;
        default: // 'B'
            int edgeId, nextMascot;
            cin >> edgeId >> nextMascot;
            edgeId--;
            int currentMascot = edgeContent[edgeId];
            changes.push_back({edgeId, currentMascot, nextMascot});
            edgeContent[edgeId] = nextMascot;
            time++;
            break;
        }
    }
    int currentTime = time;
    int currentDfsStage = 0; // start when root already added
    int currentNode = 0;
    vector<int> answers(queries.size());
    for (int i = 0; i < queries.size(); i++)
    {
        auto query = queries[i];
        cout << "q.time " << query.time << " q.nodeId " << query.nodeId << " currentTime:" << currentTime << " currentNode:" << currentNode << "\n";
        cout << "edgesContent ";
        for (int i = 0; i < n - 1; i++)
        {
            cout << edgeContent[i] << " ";
        }
        cout << "\n";

        cout << "child to edge id ";
        for (int i = 0; i < n; i++)
        {
            cout << childNodeIdToEdgeId[i] << " ";
        }
        cout << "\n";
        while (currentTime < query.time)
        { // forward in time
            auto change = changes[currentTime++];
            cout << "forward in time edge:" << change.edgeId << " " << change.previusMascot << " -> " << change.nextMascot << "\n";
            int mascot = edgeContent[change.edgeId];
            if (visitedEdges.find(change.edgeId) != visitedEdges.end())
            {
                stateDel(change.previusMascot);
                stateAdd(change.nextMascot);
            }
            edgeContent[change.edgeId] = change.nextMascot;
        }
        while (currentTime > query.time)
        { // back in time
            auto change = changes[--currentTime];
            cout << "back in time edge:" << change.edgeId << " " << change.previusMascot << " -> " << change.nextMascot << "\n";
            int mascot = edgeContent[change.edgeId];
            if (visitedEdges.find(change.edgeId) != visitedEdges.end())
            {
                stateDel(change.nextMascot);
                stateAdd(change.previusMascot);
            }
            edgeContent[change.edgeId] = change.previusMascot;
        }
        while (currentDfsStage < preorder[query.nodeId])
        {
            DfsMove nextDfsMove = dfsOrder[++currentDfsStage];
            cout << "dfs move " << ((nextDfsMove.enter) ? "true" : "false") << " " << nextDfsMove.nodeId << "\n";
            if (nextDfsMove.enter)
            { // add edge and its content
                currentNode = addEdge(nextDfsMove.nodeId);
            }
            else
            {
                currentNode = delEdge(currentNode);
            }
        }
        while (currentDfsStage > preorder[query.nodeId])
        {
            DfsMove nextDfsMove = dfsOrder[currentDfsStage--];
            cout << "reverse dfs move currentDfsStage:" << currentDfsStage << " " << ((nextDfsMove.enter) ? "true" : "false") << " " << nextDfsMove.nodeId << "\n";
            if (nextDfsMove.enter)
            { // remove edge and its content
                currentNode = delEdge(currentNode);
            }
            else
            {
                currentNode = addEdge(nextDfsMove.nodeId);
            }
        }
        cout << "stateSize" << stateSize << "\n";
        answers[i] = stateSize;
    }
    for (auto ans : answers)
    {
        cout << ans << "\n";
    }
    return 0;
}
