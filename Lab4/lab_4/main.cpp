#include <bitset>
#include <iostream>
#include <fstream>
#include <cstdint>
using namespace std;

void genenarator_Lemera();
void test();

int main() {
    genenarator_Lemera();
    test();
}

void genenarator_Lemera() {
    long long m, a, c, x0;
    int n = 7;
    for(int i = 0; i < n; i++) {
        cout << "Enter the namber for module m = " ;
        cin >> m;
        if (m <= 0) {
            cout << "Invalid Input, type number that bigger than 0" << endl;
            n++;
        } else {
            break;
        }
    }
    // m = 2147483647 a = 16807 c = 1 x0 = 1
    cout << "Type number for a, c, x0, they must be bigger than 0 and lower than " << m << endl;
    for(int i = 0; i < n; i++) {
        cout << "a=";
        cin >> a;
        cout << "c=";
        cin >> c;
        cout << "x0=";
        cin >> x0;
        if (a > 0 && c > 0 && x0 > 0 && x0 < m && a < m && c < m) {
            break;
        } else {
            cout << "Invalid Input, choose another number" << endl;
            n++;
        }
    }
    ofstream SaveToFile("Lemera.txt");
    long long Xn;
    Xn = (a*x0 + c) % m;
    for (int i = 0; i < 625; i++) {
        SaveToFile << Xn << endl;
        Xn = (a*Xn + c) % m;
    }
}

void test() {
    ifstream ReadFile("Lemera.txt");
    ofstream BitsFile ("Bits.txt");
    int buffer;
    while (ReadFile >> buffer) {
        uint32_t num = buffer;
        bitset<32> bits(num);
        BitsFile << bits << endl;
    }
    ifstream ReadBitsFile ("Bits.txt");
    string line;
    int countOnes = 0;
    while (ReadBitsFile >> line) {
        for (char ch : line) {
            if (ch == '1') countOnes++;
        }
    }
    if(countOnes > 9654 && countOnes < 10346) {
        cout << "You have passed the first test!" << endl;
    } else {
        cout << "You have failed the test!" << endl;
    }
    cout << "Number of 1 " << countOnes << endl;
    ReadBitsFile.close();
    ifstream RreadBitsFile ("Bits.txt");
    ofstream SecondBitsFile ("SecondBits.txt");
    int numer = 1;
    for (int i = 0; i < 5000; i++) {
        while (RreadBitsFile >> line) {
            for (char ch : line) {
                if (numer < 4) {
                    numer++;
                    SecondBitsFile << ch;
                } else {
                    numer = 1;
                    SecondBitsFile << ch << endl;
                }
            }
        }
    }
    ifstream ReadSecondBitsFile ("SecondBits.txt");
    ofstream Count ("Count.txt");
    string arr[16] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
    for(int j = 0; j < 16; j++) {
        int count = 0;
        for (int i = 0; i < 5000; i++) {
            while (ReadSecondBitsFile >> line) {
                if (arr[j] == line) {
                    ++count;
                }
            }
        }
        Count << count << endl;
        ReadSecondBitsFile.clear();
        ReadSecondBitsFile.seekg(0, ios::beg);
    }
    ifstream ReadCount ("Count.txt");
    int n;
    int m = 0;
    for (int i = 0; i < 16; i++) {
        while (ReadCount >> n) {
            m += n*n;
        }
    }
    double result;
    result = 16.0 / 5000.0 * m - 5000.0;
    cout << result << endl;
    if (result < 1.03 || result > 157.4) {
        cout << "You have failed the second test!" << endl;
    } else {
        cout << "You have passed the second test!" << endl;
    }
}