#include <iostream>
#include <set>
using namespace std;
const int MAX_N = 100000;
int n;
int num[MAX_N][3];
int sum[MAX_N];
set<int> notNines;

int main(int argc, char const *argv[])
{
    int z;
    cin >> n >> z;
    string numberA, numberB;
    cin >> numberA >> numberB;
    for (int i = 1; i <= numberA.size(); i++)
    {
        num[n - i][0] = numberA[numberA.size() - i] - '0';
    }
    for (int i = 1; i <= numberA.size(); i++)
    {
        num[n - i][1] = numberB[numberB.size() - i] - '0';
    }
    for (int i = 0; i < n; i++)
    {
        num[i][2] = num[i][1] + num[i][0];
        if (num[i][2] != 9)
        {
            notNines.insert(i + 1);
        }
    }
    for (int i = 0; i < z; i++)
    {
        char command;
        cin >> command;
        switch (command)
        {
        case 'S':
        {
            int i;
            cin >> i;
            i = n - i + 1;
            auto it = notNines.upper_bound(i);
            int lb = 0;
            if (it != notNines.end())
            {
                lb = num[*it - 1][2];
            }
            int ans;
            i--;
            if (lb <= 8)
            {
                ans = num[i][2] % 10;
            }
            else
            {
                ans = (num[i][2] + 1) % 10;
            }
            cout << ans << "\n";
            break;
        }
        default:
        {
            int i, c;
            cin >> i >> c;
            i = n - i;
            int numIdx = (command == 'W') ? 0 : 1;
            num[i][numIdx] = c;
            num[i][2] = num[i][0] + num[i][1];
            if (num[i][2] == 9)
            {
                notNines.erase(i + 1);
            }
            else
            {
                notNines.insert(i + 1);
            }
            break;
        }
        }
    }
    return 0;
}
