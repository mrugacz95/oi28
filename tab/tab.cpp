#include <iostream>
using namespace std;

int opsNeeded = 0;
const int MAX_N = 1000;
const int MAX_M = 1000;
bool table[MAX_N][MAX_M];

void flip(int y, int x)
{
    if (table[y][x])
    {
        opsNeeded--;
    }
    else
    {
        opsNeeded++;
    }
    table[y][x] ^= true;
}

int main(int argc, char const *argv[])
{
    int n, m, q;
    cin >> n >> m >> q;
    for (int i = 0; i < q; i++)
    {
        int y1, x1, y2, x2;
        cin >> y1 >> x1 >> y2 >> x2;
        y1--;
        x1--;
        y2--;
        x2--;
        flip(y2, x2);
        if (x1 != 0)
        {
            flip(y2, x1 - 1);
        }
        if (y1 != 0)
        {
            flip(y1 - 1, x2);
        }
        if (x1 != 0 && y1 != 0)
        {
            flip(y1 - 1, x1 - 1);
        }
        cout << opsNeeded << "\n";
    }

    return 0;
}
