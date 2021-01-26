#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>
typedef unsigned long long ull;
using namespace std;

int n, z;
string a[2];
ull num[3][15000];
int numberOfZeros = 18;
ull powTab[18] = {
    100000000000000000, 1, 10, 100, 1000, 10000,
    100000, 1000000, 10000000, 100000000, 1000000000,
    10000000000, 100000000000, 1000000000000, 10000000000000,
    100000000000000, 1000000000000000, 10000000000000000};

ull zerosPow = 1000000000000000000;

void calcSum(int toIndex)
{
    ull carry = 0;
    for (int i = 0; i <= toIndex; i++)
    {
        num[2][i] = num[0][i] + num[1][i] + carry;
        carry = num[2][i] / zerosPow;
        num[2][i] %= zerosPow;
    }
}

int main(int argc, char const *argv[])
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n >> z;
    cin >> a[0] >> a[1];
    int seg = ceil(n / (float)numberOfZeros);
    for (int i = 0; i < 2; i++)
    {
        int count = 0;
        int segCount = 0;
        while (count + 1 < n)
        {
            ull currentSegment = 0;
            ull pow = 1;
            for (int j = 0; j < numberOfZeros && count < a[i].length(); j++)
            {
                char c = a[i][a[i].length() - 1 - count];
                currentSegment += (c - '0') * pow;
                pow *= 10;
                count++;
            }
            num[i][segCount] = currentSegment;
            segCount++;
            if (count >= a[i].length())
            {
                break;
            }
        }
    }
    int calculatedIndex = -1;
    for (int i = 0; i < z; i++)
    {
        char command;
        int it, c;
        cin >> command;
        switch (command)
        {
        case 'S':
        {
            cin >> it;
            int index = (it - 1) / numberOfZeros;
            if (index > calculatedIndex)
            {
                calculatedIndex = index;
                calcSum(index);
            }
            int pos = it % numberOfZeros;
            ull pow = powTab[pos];
            cout << num[2][index] / pow % 10 << "\n";
            break;
        }
        default:
        {
            cin >> it >> c;
            int index = (it - 1) / numberOfZeros;
            int pos = it % numberOfZeros;
            ull pow = powTab[pos];

            int numbIndex = (command == 'W') ? 0 : 1;
            ull right = 0;
            if (pow > 1)
            {
                right = num[numbIndex][index] % pow;
            }
            ull left = num[numbIndex][index] / (pow * 10) * (pow * 10);
            num[numbIndex][index] = left + c * pow + right;
            calculatedIndex = -1;
            break;
        }
        }
    }
    return 0;
}