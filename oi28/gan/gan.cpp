#include <vector>
#include <iostream>
#include <deque>
#include <set>
#include <cmath>
#include <algorithm>
using namespace std;

typedef pair<int, bool> pib;
typedef pair<int, int> pii;

struct Query
{
    int time, nodeId;
};

struct Change
{
    int edgeId, previousMascot, nextMascot;
};

const int MAX_N = 100010,
          MAX_M = 150010,
          MAX_Z = 150010;

int n, m, z;
int stateSize = 0;
int state[MAX_M];
vector<int> tree[MAX_N];
int edgeContent[MAX_N];
Query queries[MAX_Z];
int queriesSize = 0;
Change changes[MAX_Z];
int changesSize = 0;
int preorder[MAX_N];
int parent[MAX_N];
int preorderCounter = 0;
int dfsCounter = 0;
int dfsOrder[MAX_N * 2];
pii edges[MAX_N];
int childNodeIdToEdgeId[MAX_N];
bool wasNodeVisited[MAX_N];
float sqrtN;

void stateAdd(int x)
{
    if (state[x] == 0)
    {
        stateSize++;
    }
    state[x]++;
}

void stateDel(int x)
{
    state[x]--;
    if (state[x] == 0)
    {
        stateSize--;
    }
}

void clearState()
{
    for (int i = 0; i < m; i++)
    {
        state[i] = 0;
    }
}

int getChild(pii &edge)
{
    return (parent[edge.first] == edge.second) ? edge.first : edge.second;
}

void dfs(int startNode)
{
    dfsOrder[dfsCounter++] = startNode;
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
    dfsOrder[dfsCounter++] = startNode;
}

void calulateChildNodeIdToEdgeId()
{
    for (int i = 0; i < n - 1; i++)
    {
        auto edge = edges[i];
        int child = getChild(edge);
        childNodeIdToEdgeId[child] = i;
    }
}

int addEdge(int nodeId)
{
    int edgeId = childNodeIdToEdgeId[nodeId];
    int mascot = edgeContent[edgeId];
    stateAdd(mascot);
    wasNodeVisited[nodeId] = true;
    return nodeId;
}

int delEdge(int lastNodeId)
{
    int edgeId = childNodeIdToEdgeId[lastNodeId];
    int mascot = edgeContent[edgeId];
    stateDel(mascot);
    wasNodeVisited[lastNodeId] = false;
    return parent[lastNodeId];
}

bool compareQueries(const int &lhs, const int &rhs)
{
    pii a = make_pair((int)floor((float)(queries[lhs].time + 1) / sqrtN), preorder[queries[lhs].nodeId]);
    pii b = make_pair((int)floor((float)(queries[rhs].time + 1) / sqrtN), preorder[queries[rhs].nodeId]);
    return a < b;
}

int main(int argc, char const *argv[])
{
    cin >> n >> m >> z;
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

    // read queries
    int time = 0;
    for (int i = 0; i < z; i++)
    {
        char type;
        cin >> type;
        switch (type)
        {
        case 'Z':
            int query;
            cin >> query;
            query--;
            queries[queriesSize++] = {time, query};
            break;
        default: // 'B'
            int edgeId, nextMascot;
            cin >> edgeId >> nextMascot;
            edgeId--;
            int currentMascot = edgeContent[edgeId];
            changes[changesSize++] = {edgeId, currentMascot, nextMascot};
            edgeContent[edgeId] = nextMascot;
            time++;
            break;
        }
    }
    // sort queries
    sqrtN = sqrt(n);
    vector<int> queriesOrder(queriesSize);
    for (int i = 0; i < queriesOrder.size(); i++)
    {
        queriesOrder[i] = i;
    }
    stable_sort(queriesOrder.begin(), queriesOrder.end(), compareQueries);

    int currentTime = time;
    int currentDfsStage = 0; // start when root already added
    int currentNode = 0;
    vector<int> answers(queriesSize);

    for (auto queryId : queriesOrder)
    {
        auto query = queries[queryId];
        while (currentDfsStage < preorder[query.nodeId])
        {
            int nextVisitedNode = dfsOrder[++currentDfsStage];
            if (!wasNodeVisited[nextVisitedNode])
            { // add edge and its content
                currentNode = addEdge(nextVisitedNode);
            }
            else
            {
                currentNode = delEdge(currentNode);
            }
        }
        while (currentDfsStage > preorder[query.nodeId])
        {
            int nextVisitedNode = dfsOrder[currentDfsStage--];
            if (wasNodeVisited[nextVisitedNode])
            { // remove edge and its content
                currentNode = delEdge(currentNode);
            }
            else
            {
                currentNode = addEdge(nextVisitedNode);
            }
        }
        while (currentTime < query.time)
        { // forward in time
            auto change = changes[currentTime++];
            int mascot = edgeContent[change.edgeId];
            pii edge = edges[change.edgeId];
            int edgeChild = getChild(edge);
            if (wasNodeVisited[edgeChild] == true)
            {
                stateDel(change.previousMascot);
                stateAdd(change.nextMascot);
            }
            edgeContent[change.edgeId] = change.nextMascot;
        }
        while (currentTime > query.time)
        { // back in time
            auto change = changes[--currentTime];
            int mascot = edgeContent[change.edgeId];
            pii edge = edges[change.edgeId];
            int edgeChild = getChild(edge);
            if (wasNodeVisited[edgeChild] == true)
            {
                stateDel(change.nextMascot);
                stateAdd(change.previousMascot);
            }
            edgeContent[change.edgeId] = change.previousMascot;
        }

        answers[queryId] = stateSize;
    }
    for (auto ans : answers)
    {
        cout << ans << "\n";
    }
    return 0;
}