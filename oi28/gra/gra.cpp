#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
using namespace std;

int holes1Count, holes2Count;
int holes[2][1000000];
int lastHoles = 1;
int n, m, z;

class Edge
{
public:
  pair<int, int> target;
  short weight;

  Edge(pair<int, int> t, short w) : target(t), weight(w) {}
};

map<pair<int, int>, vector<Edge>> g;

void mergeHoles(int platformId)
{
  pair<int, int> lastNode = make_pair(platformId, 0);
  int i1 = 0, i2 = 0, nextVal = 0, source = -1;

  for (int i = 0; i < holes1Count + holes2Count; i++)
  {
    if ((i1 >= holes1Count) || (holes[0][i1] < holes[1][i2]))
    {
      nextVal = holes[0][i2];
      source = 0;
      i1++;
    }
    else
    {
      nextVal = holes[1][i2];
      source = 1;
      i2++;
    }
    // Add next node
    pair<int, int> nextNode;
    if (lastHoles - 1 == source) // hole on current platform
    {
      nextNode = make_pair(platformId, nextVal - 1);
      g[lastNode].push_back(Edge(nextNode, 1));
      lastNode = nextNode;
      nextNode = make_pair(platformId, nextVal + 1);
    }
    else // hole is in celling of current platform
    {
      pair<int, int> upperBeforeHole = make_pair(platformId - 1, nextVal - 1);
      pair<int, int> upperAfterHole = make_pair(platformId - 1, nextVal + 1);
      nextNode = make_pair(platformId, nextVal);
      g[upperBeforeHole].push_back(Edge(nextNode, 0));
      g[nextNode].push_back(Edge(upperAfterHole, 1));
      lastNode = nextNode;
    }
    lastNode = nextNode;
  }
}
class CompareDist
{
public:
  const bool operator()(pair<int, pair<int, int>> lhs, pair<int, pair<int, int>> rhs)
  {
    return lhs.first < rhs.first;
  }
};

int dijkstra(int startPlatform)
{
  priority_queue<pair<int, pair<int, int>>> q;
  map<int, pair<int, int>> dist;
  q.push(make_pair(0, make_pair(startPlatform, 0)));
  while (!q.empty())
  {
    pair<int, pair<int, int>> current = q.top();
    q.pop();
    pair<int, int> node = current.second;
    int dist = current.first;
    for (auto edge : g[node])
    {
      if (edge.target.second == m - 1)
      {
        return dist;
      }
      q.push(make_pair(dist + edge.weight, edge.target));
    }
  }
  return -1;
}

int main(int argc, char const *argv[])
{
  cin >> n >> m >> z;
  for (int i = 0; i < n; i++)
  {
    int k, h;
    if (lastHoles == 1)
    {
      cin >> holes1Count;
    }
    else
    {
      cin >> holes2Count;
    }

    for (int j = 0; j < k; j++)
    {
      cin >> holes[lastHoles - 1][j];
    }
    lastHoles = (lastHoles == 1) ? 2 : 1;
    mergeHoles(i);
  }
  for (int i = 0; i < z; i++)
  {
    int p;
    cin >> p;
    cout << dijkstra(p - 1) << "\n";
  }
  return 0;
}
