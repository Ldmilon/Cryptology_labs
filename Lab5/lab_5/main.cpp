#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

using Bits = vector<int>;

// Permutation using 1-based index table
Bits permute(const Bits &in, const vector<int> &table) {
    Bits out;
    out.reserve(table.size());
    for (int idx1based : table) {
        int idx = idx1based - 1;
        if (idx < 0 || idx >= (int)in.size()) throw runtime_error("Permutation index out of range");
        out.push_back(in[idx]);
    }
    return out;
}

Bits leftShift(const Bits &in, int shift) {
    int n = (int)in.size();
    Bits out(n);
    for (int i = 0; i < n; ++i) out[i] = in[(i + shift) % n];
    return out;
}

Bits xorBits(const Bits &a, const Bits &b) {
    if (a.size() != b.size()) throw runtime_error("xorBits: sizes differ");
    Bits out(a.size());
    for (size_t i = 0; i < a.size(); ++i) out[i] = a[i] ^ b[i];
    return out;
}

Bits sbox_lookup(const Bits &in4, const int box[4][4]) {
    // row = first and fourth bits, col = second and third bits
    int row = in4[0] * 2 + in4[3];
    int col = in4[1] * 2 + in4[2];
    int val = box[row][col];
    // return two bits MSB..LSB
    return Bits{ (val >> 1) & 1, val & 1 };
}

Bits fFunction(const Bits &R4, const Bits &K8) {
    // E/P and P4 tables (1-based)
    static const vector<int> EP  = {4,1,2,3,2,3,4,1};
    static const vector<int> P4  = {2,4,3,1};
    static const int S0[4][4] = {
        {1,0,3,2},
        {3,2,1,0},
        {0,2,1,3},
        {3,1,3,2}
    };
    static const int S1[4][4] = {
        {0,1,2,3},
        {2,0,1,3},
        {3,0,1,0},
        {2,1,0,3}
    };

    // 1) expand/permutation 4->8
    Bits expanded = permute(R4, EP);
    // 2) xor with subkey
    Bits x = xorBits(expanded, K8);
    // 3) split to two 4-bit halves
    Bits left4(x.begin(), x.begin()+4);
    Bits right4(x.begin()+4, x.end());
    // 4) S-box lookups
    Bits s0 = sbox_lookup(left4, S0);
    Bits s1 = sbox_lookup(right4, S1);
    // 5) combine and P4
    Bits combined = { s0[0], s0[1], s1[0], s1[1] };
    Bits out = permute(combined, P4);
    return out; // 4 bits
}

void generateKeys(const Bits &key10, Bits &K1, Bits &K2) {
    if (key10.size() != 10) throw runtime_error("Key must be 10 bits");
    static const vector<int> P10 = {3,5,2,7,4,10,1,9,8,6};
    static const vector<int> P8  = {6,3,7,4,8,5,10,9};

    Bits perm = permute(key10, P10); // 10 bits after P10

    // split correctly using iterator constructor
    Bits left5 (perm.begin(), perm.begin() + 5);
    Bits right5(perm.begin() + 5, perm.end());

    // round 1 shifts by 1
    left5  = leftShift(left5, 1);
    right5 = leftShift(right5, 1);

    Bits combined1;
    combined1.insert(combined1.end(), left5.begin(), left5.end());
    combined1.insert(combined1.end(), right5.begin(), right5.end());
    K1 = permute(combined1, P8);

    // round 2: shift the already-shifted halves by 2 more (total effect equals spec)
    left5  = leftShift(left5, 2);
    right5 = leftShift(right5, 2);

    Bits combined2;
    combined2.insert(combined2.end(), left5.begin(), left5.end());
    combined2.insert(combined2.end(), right5.begin(), right5.end());
    K2 = permute(combined2, P8);
}

Bits feistelRound(const Bits &block8, const Bits &subkey, bool doSwap) {
    // block8 = L(4) || R(4)
    Bits L(block8.begin(), block8.begin() + 4);
    Bits R(block8.begin() + 4, block8.end());

    Bits f = fFunction(R, subkey); // 4 bits
    Bits Lnew = xorBits(L, f);

    if (doSwap) {
        // swap: output = R || Lnew
        Bits out;
        out.insert(out.end(), R.begin(), R.end());
        out.insert(out.end(), Lnew.begin(), Lnew.end());
        return out;
    } else {
        // no swap: output = Lnew || R
        Bits out;
        out.insert(out.end(), Lnew.begin(), Lnew.end());
        out.insert(out.end(), R.begin(), R.end());
        return out;
    }
}

Bits encryptBlock(const Bits &plain8, const Bits &K1, const Bits &K2) {
    static const vector<int> IP  = {2,6,3,1,4,8,5,7};
    static const vector<int> IP1 = {4,1,3,5,7,2,8,6};

    Bits block = permute(plain8, IP);
    block = feistelRound(block, K1, true);  // first round + swap
    block = feistelRound(block, K2, false); // second round, no swap after
    Bits cipher = permute(block, IP1);
    return cipher;
}

Bits decryptBlock(const Bits &cipher8, const Bits &K1, const Bits &K2) {
    // same as encrypt but keys in reverse order
    static const vector<int> IP  = {2,6,3,1,4,8,5,7};
    static const vector<int> IP1 = {4,1,3,5,7,2,8,6};

    Bits block = permute(cipher8, IP);
    block = feistelRound(block, K2, true);  // first round of decrypt uses K2
    block = feistelRound(block, K1, false); // second round uses K1
    Bits plain = permute(block, IP1);
    return plain;
}

// Helpers to convert from string "0101..." and print Bits
Bits strToBits(const string &s) {
    Bits b;
    for (char c : s) {
        if (c == '0') b.push_back(0);
        else if (c == '1') b.push_back(1);
        else throw runtime_error("Input string must contain only '0' and '1'");
    }
    return b;
}

string bitsToStr(const Bits &b) {
    string s;
    s.reserve(b.size());
    for (int x : b) s.push_back(x ? '1' : '0');
    return s;
}

int main() {
    try {
        cout << "S-DES (fixed) - Enter 10-bit key (e.g. 1011100110): ";
        string keyS; cin >> keyS;
        Bits key10 = strToBits(keyS);
        if (key10.size() != 10) { cerr << "Key must be 10 bits\n"; return 1; }

        cout << "Enter 8-bit plaintext (e.g. 11110010): ";
        string ptS; cin >> ptS;
        Bits plain8 = strToBits(ptS);
        if (plain8.size() != 8) { cerr << "Plaintext must be 8 bits\n"; return 1; }

        Bits K1, K2;
        generateKeys(key10, K1, K2);

        cout << "K1 = " << bitsToStr(K1) << "\n";
        cout << "K2 = " << bitsToStr(K2) << "\n";

        Bits cipher = encryptBlock(plain8, K1, K2);
        cout << "Encrypted: " << bitsToStr(cipher) << "\n";

        Bits decrypted = decryptBlock(cipher, K1, K2);
        cout << "Decrypted: " << bitsToStr(decrypted) << "\n";

        return 0;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << "\n";
        return 2;
    }
}
