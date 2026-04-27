#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <numeric>

using namespace std;

// --- Helper Functions ---

// Check if a number is prime
bool isPrime(long long n) {
    if (n < 2) return false;
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Greatest Common Divisor
long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// Modular Exponentiation: (base^exp) % mod
long long modExp(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// Extended Euclidean Algorithm to find modular inverse (d)
// d * e = 1 mod phi(n)
long long calculateD(long long e, long long phi) {
    long long m0 = phi, t, q;
    long long x0 = 0, x1 = 1;

    if (phi == 1) return 0;

    while (e > 1) {
        // q is quotient
        q = e / phi;
        t = phi;

        // phi is remainder
        phi = e % phi, e = t;
        t = x0;

        x0 = x1 - q * x0;
        x1 = t;
    }

    // Make x1 positive
    if (x1 < 0) x1 += m0;

    return x1;
}

int main() {
    long long p, q;

    cout << "=== RSA Crypto System (Lab 7) ===" << endl;

    // 1. Input prime numbers
    while (true) {
        cout << "Enter prime number p: ";
        cin >> p;
        cout << "Enter prime number q: ";
        cin >> q;

        if (isPrime(p) && isPrime(q) && p != q) {
            break;
        } else {
            cout << "Error! Numbers must be prime and different.\n";
        }
    }

    // 2. Calculate modulus n
    long long n = p * q;

    // 3. Calculate Euler's totient function phi
    long long phi = (p - 1) * (q - 1);

    cout << "\n[Key Parameters]" << endl;
    cout << "Modulus n = " << n << endl;
    cout << "Euler's totient function phi(n) = " << phi << endl;

    // 4. Choose public exponent e
    // Automatically find smallest coprime e
    long long e_exp = 2;
    while (e_exp < phi) {
        if (gcd(e_exp, phi) == 1) break;
        e_exp++;
    }

    // Option to manually input e
    cout << "Automatically selected e = " << e_exp << ". Do you want to enter custom e? (1 - yes, 0 - no): ";
    int choice;
    cin >> choice;
    if (choice == 1) {
        cout << "Enter e (must be coprime with " << phi << "): ";
        cin >> e_exp;
        if (gcd(e_exp, phi) != 1) {
            cout << "Warning: entered number is not coprime! RSA will not work correctly.\n";
        }
    }

    // 5. Calculate private exponent d
    long long d = calculateD(e_exp, phi);

    cout << "\n[Generated Keys]" << endl;
    cout << "Public Key (e, n): (" << e_exp << ", " << n << ")" << endl;
    cout << "Private Key (d, n): (" << d << ", " << n << ")" << endl;

    // --- Action Menu ---
    while (true) {
        cout << "\nChoose action:\n1. Encrypt text\n2. Decrypt codes\n3. Exit\n> ";
        int action;
        cin >> action;

        if (action == 3) break;

        if (action == 1) {
            // Encryption
            string msg;
            cout << "Enter text (no spaces): ";
            cin >> msg;

            cout << "Encryption result (char-by-char): ";
            for (unsigned char c : msg) {
                long long m = (long long)c;
                // C = M^e mod n
                long long encrypted = modExp(m, e_exp, n);
                cout << encrypted << " ";
            }
            cout << endl;
        }
        else if (action == 2) {
            // Decryption
            int count;
            cout << "How many numbers to enter? ";
            cin >> count;

            cout << "Enter numbers separated by space: ";
            vector<long long> cipherText(count);
            for (int i = 0; i < count; i++) {
                cin >> cipherText[i];
            }

            cout << "Decrypted text: ";
            for (long long c : cipherText) {
                // M = C^d mod n
                long long decrypted = modExp(c, d, n);
                cout << (unsigned char)decrypted;
            }
            cout << endl;
        }
    }

    return 0;
}