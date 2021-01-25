#include <iostream>
#include <limits>
#include <algorithm>
using namespace std;

typedef unsigned long long ull;

const int MAX_N = 300000;
int arr[MAX_N][3];
int m[MAX_N];
bool hasValues[3];
int maxValues[3][3];
int tmp[MAX_N];

int main(int argc, char const *argv[])
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int d, p, r;
        cin >> d >> p >> r;
        arr[i][0] = d;
        arr[i][1] = p;
        arr[i][2] = r;
        m[i] = max(d, p);
        m[i] = max(m[i], r);
        hasValues[0] |= d != 0;
        hasValues[1] |= p != 0;
        hasValues[2] |= r != 0;
    }
    ull initialCost = 0;
    for (int rowIdx = 0; rowIdx < n; rowIdx++)
    {
        for (int colIdx = 0; colIdx < 3; colIdx++)
        {
            initialCost += arr[rowIdx][colIdx];
        }
        initialCost -= m[rowIdx];
    }
    for (int i = 0; i < n; i++)
    {
        tmp[i] = i;
    }
    for (int colIdx = 0; colIdx < 3; colIdx++)
    {
        make_heap(tmp, tmp + n, [&](int &a, int &b) { return m[a] - arr[a][colIdx] > m[b] - arr[b][colIdx]; });
        for (int i = 0; i < 3; i++)
        {
            maxValues[colIdx][i] = tmp[i];
        }
    }
    ull minCost = numeric_limits<ull>::max();
    for (int dMaxIdx = 0; dMaxIdx < 3; dMaxIdx++)
    {
        for (int pMaxIdx = 0; pMaxIdx < 3; pMaxIdx++)
        {
            for (int rMaxIdx = 0; rMaxIdx < 3; rMaxIdx++)
            {
                int dRowId = maxValues[0][dMaxIdx];
                int pRowId = maxValues[1][pMaxIdx];
                int rRowId = maxValues[2][rMaxIdx];
                if (dRowId == pRowId || pRowId == rRowId || rRowId == dRowId)
                {
                    continue;
                }
                ull newCost = initialCost;
                if (hasValues[0])
                {
                    newCost += m[dRowId] - arr[dRowId][0];
                }
                if (hasValues[1])
                {
                    newCost += m[pRowId] - arr[pRowId][1];
                }
                if (hasValues[2])
                {
                    newCost += m[rRowId] - arr[rRowId][2];
                }
                if (minCost > newCost)
                {
                    minCost = newCost;
                }
            }
        }
    }
    cout << minCost;

    return 0;
}
