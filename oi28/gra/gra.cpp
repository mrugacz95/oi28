#include <iostream>
#include <map>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int, int> pii;

int n, m, z;

struct Edge
{
  int target;
  char weight;
};

const int MAX_K = 2100000; // number of holes +  starting points
vector<Edge> graph[MAX_K],
             invGraph[MAX_K];
vector<int> dist(MAX_K, -1);
vector<int> platformStart,
            platformEnd;

int holesCount = 0;

void mergeHoles(int platformId, vector<pii> &currentHoles, vector<pii> &lastHoles)
{
  int lastNode = platformStart[platformId]; // start from the beginning 
  int lastIt = 0,                           // last platform holes iterator
      currIt = 0;                            // current platform holes iterator
  pii nextVal;
  bool fromCurrent;
  for (int i = 0; i < lastHoles.size() + currentHoles.size(); i++)
  {
    // choose next smaller value from two arrays using second value
    if ((lastIt == lastHoles.size()) || (currIt < currentHoles.size() && currentHoles[currIt].second < lastHoles[lastIt].second))
    {
      nextVal = currentHoles[currIt];
      fromCurrent = true;
      currIt++;
    }
    else
    {
      nextVal = lastHoles[lastIt];
      fromCurrent = false;
      lastIt++;
    }
    // Add next node
    if (fromCurrent) // hole on current platform
    {
      int afterHole = nextVal.first;
      graph[lastNode].push_back({afterHole, 1}); // jump over with cost 1
      lastNode = afterHole;
    }
    else // hole is in celling of current platform
    {
      int afterHole = nextVal.first;
      int beforeHole = afterHole - 1;
      graph[lastNode].push_back({afterHole, 1}); // jump on with cost 1
      graph[beforeHole].push_back({lastNode, 0}); // jump off with cost 0
    }
  }
  platformEnd[platformId] = lastNode;
}

void zero_one_bfs()
{
  deque<int> q;
  for (int i = 0; i < n; i++)
  {
    auto node = platformEnd[i];
    q.push_back(node);
    dist[node] = 0;
  }
  while (!q.empty())
  {
    auto current = q.front();
    q.pop_front();
    for (auto edge : invGraph[current])
    {
      if (dist[edge.target] == -1 || dist[current] + edge.weight < dist[edge.target])
      {
        dist[edge.target] = dist[current] + edge.weight;
        if (edge.weight == 0)
        {
          q.push_front(edge.target);
        }
        else
        {
          q.push_back(edge.target);
        }
      }
    }
  }
}

int main(int argc, char const *argv[])
{
  cin >> n >> m >> z;
  platformStart.resize(n);
  platformEnd.resize(n);
  vector<pii> holes1;
  vector<pii> holes2;
  bool loadToHoles1 = true;
  for (int platform = 0; platform < n; platform++)
  {
    int k, hole;
    cin >> k;
    platformStart[platform] = holesCount++;
    for (int j = 0; j < k; j++)
    {
      cin >> hole;
      if (loadToHoles1)
      {
        holes1.push_back({holesCount++, hole}); // assign id with holesCount
      }
      else
      {
        holes2.push_back({holesCount++, hole}); // assign id with holesCount
      }
    }
    if (loadToHoles1)
    {
      mergeHoles(platform, holes1, holes2);
      holes2.clear();
    }
    else
    {
      mergeHoles(platform, holes2, holes1);
      holes1.clear();
    }
    platformEnd[platform] = holesCount - 1;
    loadToHoles1 = !loadToHoles1;
  }
  for (int i = 0; i < holesCount; i++)
  {
    for (auto el : graph[i])
    {
      invGraph[el.target].push_back({i, el.weight});
    }
  }
  zero_one_bfs();
  for (int i = 0; i < z; i++)
  {
    int platform;
    cin >> platform;
    platform--;
    cout << dist[platformStart[platform]] << "\n";
  }
  return 0;
}