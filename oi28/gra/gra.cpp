#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;

int holes1Count, holes2Count;
int lastHoles = 1;
int n, m, z;

// class Edge
// {
// public:
//   pair<int, int> target;
//   short weight;

//   Edge(pair<int, int> t, short w) : target(t), weight(w) {}
// };

// struct hash_pair
// {
//   template <class T1, class T2>
//   size_t operator()(const pair<T1, T2> &p) const
//   {
//     auto hash1 = hash<T1>{}(p.first);
//     auto hash2 = hash<T2>{}(p.second);
//     return hash1 ^ hash2;
//   }
// };

typedef pair<int, int> pii;

unordered_map<int, vector<pair<int, int>>> g;
unordered_map<int, vector<pair<int,int>>> inv_g;
unordered_map<int, int> dist;

int node_to_id(int i, int x)
{
  return i * (m+ 1) + x;
}

pii id_to_node(int id)
{
  return {id / (m+1), id % (m+1)};
}

void mergeHoles(int platformId, vector<int> *lastHoles, vector<int> *currentHoles)
{
  int lastNode = node_to_id(platformId, 1); // begin with leftmost node
  int lastIt = 0,                           // last platform holes iterator
      currIt = 0,                           // current platform holes iterator
      nextVal;
  bool fromCurrent;
  // cout << "platform " << platformId << "\n";
  for (int i = 0; i < lastHoles->size() + currentHoles->size(); i++)
  {
    if ((lastIt == lastHoles->size()) || (currIt < currentHoles->size() && currentHoles->at(currIt) < lastHoles->at(lastIt)))
    {
      nextVal = currentHoles->at(currIt);
      fromCurrent = true;
      currIt++;
    }
    else
    {
      nextVal = lastHoles->at(lastIt);
      fromCurrent = false;
      lastIt++;
    }
    // cout << "next val:" << nextVal << "\n";
    // Add next node
    if (fromCurrent) // hole on current platform
    {
      int afterHole = node_to_id(platformId, nextVal + 1); // node after hole
      int belowHole = node_to_id(platformId + 1, nextVal); // node after hole
      g[lastNode].push_back({afterHole, 1});
      g[lastNode].push_back({belowHole, 0});
      lastNode = afterHole;
    }
    else // hole is in celling of current platform
    {
      int upperAfterHole = node_to_id(platformId - 1, nextVal + 1);
      int underHole = node_to_id(platformId, nextVal);
      g[lastNode].push_back({upperAfterHole, 1});
      if (lastNode != underHole)
      {
        g[lastNode].push_back({underHole, 0});
      }
      lastNode = underHole;
    }
  }
  if (id_to_node(lastNode).second != m)
  {
    int finalNode = node_to_id(platformId, m);
    g[lastNode].push_back({finalNode, 0});
  }
}

void zero_one_bfs()
{
  deque<int> q;
  for (int i = 1; i <= n; i++)
  {
    auto node = node_to_id(i, m);
    q.push_back(node);
    dist[node] = 0;
  }
  // cout << "q size " << q.size() << "\n";
  // unordered_set<pair<int, int>, hash_pair> visited;
  while (!q.empty())
  {
    int current = q.front();
    q.pop_front();

    // visited.insert(current);
    // cout <<  "current " << current.first << " " << current.second << "\n";
    // cout << "curr dist: " << dist[current] << "\n";
    for (pii edge : inv_g[current])
    {
      // if(visited.find(edge.target) != visited.end()){
      //   continue;
      // }
      if (dist.find(edge.first) == dist.end() || dist[current] + edge.second < dist[edge.first])
      {

        dist[edge.first] = dist[current] + edge.second;
        if (edge.second == 0)
        {
          q.push_front(edge.first);
        }
        else
        {
          q.push_back(edge.first);
        }
      }
    }
  }
}

int main(int argc, char const *argv[])
{
  cin >> n >> m >> z;
  vector<int> *lastHoles = new vector<int>;
  vector<int> *currentHoles;
  for (int platform = 1; platform <= n; platform++)
  {
    int k, hole;
    cin >> k;
    currentHoles = new vector<int>(k);
    for (int j = 0; j < k; j++)
    {
      cin >> hole;
      (*currentHoles)[j] = hole;
    }
    mergeHoles(platform, lastHoles, currentHoles);
    lastHoles->clear();
    delete lastHoles;
    lastHoles = currentHoles;
  }
  delete currentHoles;
  for (auto el : g)
  {
    for (pii node : el.second)
    {
      // cout << "from " << el.first.first << "," << el.first.second << " to " << node.target.first << ", " << node.target.second << " w: " << node.weight << "\n";
      inv_g[node.first].push_back({el.first, node.second});
    }
  }
  g.clear();

  zero_one_bfs();

  for (int i = 0; i < z; i++)
  {
    int platform;
    cin >> platform;
    cout << dist[node_to_id(platform, 1)] << "\n";
  }

  return 0;
}
