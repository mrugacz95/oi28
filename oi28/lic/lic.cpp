#include <iostream>
#include <fstream>

using namespace std;

int n,z, seg;
string a[2];
long long num [3][15000];
int numberOfZeros = 17;
long long powTab[19] = {
    1, 10, 100, 1000, 10000,
    100000, 1000000, 10000000, 100000000, 1000000000,
    10000000000, 100000000000, 1000000000000, 10000000000000,
    100000000000000, 1000000000000000, 10000000000000000, 100000000000000000,
    1000000000000000000
};


long long zerosPow = powTab[numberOfZeros];
long long zerosPowOneLess = powTab[numberOfZeros - 1];

void calcSum(){
    long long carry = 0;
    for(int i =0;i<seg;i++){
        num[2][i] = num[0][i] + num[1][i] + carry;
        carry = num[2][i] / zerosPow;
        num[2][i] %= zerosPow;
    }
}

void printNum(int id){
    for(int i=seg;i>= 0 ;i--){
        cout << std::setfill('0') << std::setw(numberOfZeros) <<  num[id][i];
    }
}
void printStatus(){
    cout << "status\nA: ";
    printNum(0);
    cout << "\nB: ";
    printNum(1);
    cout << "\nS: ";
    printNum(2);
    cout <<"\n";
}

long long calcPow(int pos){
//    switch (pos) {
//        case 0:
//            return 1000;
//        case 1:
//            return 1;
//        case 2:
//            return 10;
//        case 3:
//            return 100;
//        default:
//            return -1;
//    }
    if(pos == 0){
        return zerosPowOneLess;
    }
    return powTab[pos - 1];
}

int main(int argc, char const *argv[])
{
//    fstream input;
//    input.open(argv[1]);
//    if (!input.is_open())
//    {
//        cout << "File not found";
//        return 0;
//    }
    cin >> n >> z;
    cin >> a[0] >> a[1];
    reverse(a[0].begin(), a[0].end());
    reverse(a[1].begin(), a[1].end());
    seg = ceil(n / (float)numberOfZeros);
    for(int i = 0;i < 2 ;i ++){
        int count = 0;
        int segCount = 0;
        while(count  + 1 < n){
            long long currentSegment = 0;
            long long pow = 1;
            for(int j = 0 ;j < numberOfZeros && count < a[i].length();j++){
                char c = a[i][count];
                currentSegment += (c - '0') * pow;
                pow *= 10;
                count ++;
            }
            num[i][segCount] = currentSegment;
            segCount ++;
            if(count >= a[i].length()){
                break;
            }
        }
        
    }
    calcSum();
//    printStatus();
    for(int i=0;i<z;i++){
        char command;
        int it, c;
        cin >> command;
        switch (command) {
            case 'S': {
                cin >> it;
//                cout <<"S "<<it<<"\n";
                int index = (it - 1) / numberOfZeros;
                int pos = it % numberOfZeros;
                long long pow = calcPow(pos);
                cout << num[2][index] / pow % 10 << "\n";
                break;
            }
            case 'Z':
            case 'W': {
                cin >> it >> c;
//                cout << command <<" "<<it<<" "<<c<<"\n";
                int index = (it - 1) / numberOfZeros;
                int pos = it % numberOfZeros;
                long long pow =calcPow(pos);
                
                int numbIndex = (command == 'W')? 0 : 1;
                long long right = 0;
                if(pow > 1){
                    right = num[numbIndex][index] % pow;
                }
                long long left =num[numbIndex][index] / (pow *  10 );
                num[numbIndex][index] = left * pow * 10 + c * pow + right;
                calcSum();
                break;
            }
            default:
                cout << "Wrong command";
                break;
        }
//        printStatus();
    }
    return 0;
}
