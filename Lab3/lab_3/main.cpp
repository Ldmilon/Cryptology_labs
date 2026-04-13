#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <cmath>

using namespace std;

// English letter frequencies (in percentages)
const double ENGLISH_FREQ[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

// Function to encrypt text using Vigenere cipher
string encryptVigenere(string text, string key) {
    string result = "";
    int keyIndex = 0;

    for (int i = 0; i < text.length(); i++) {
        char c = text[i];

        // Process only letters
        if (isalpha(c)) {
            // Convert to uppercase for simplicity
            c = toupper(c);
            char keyChar = toupper(key[keyIndex % key.length()]);

            // Encrypt: (text + key) mod 26
            char encrypted = ((c - 'A') + (keyChar - 'A')) % 26 + 'A';
            result += encrypted;
            keyIndex++;
        }
    }

    return result;
}

// Function to decrypt text using Vigenere cipher
string decryptVigenere(string text, string key) {
    string result = "";
    int keyIndex = 0;

    for (int i = 0; i < text.length(); i++) {
        char c = text[i];

        if (isalpha(c)) {
            c = toupper(c);
            char keyChar = toupper(key[keyIndex % key.length()]);

            // Decrypt: (text - key + 26) mod 26
            char decrypted = ((c - 'A') - (keyChar - 'A') + 26) % 26 + 'A';
            result += decrypted;
            keyIndex++;
        }
    }

    return result;
}

// Function to find repeated substrings (Kasiski method)
map<string, vector<int>> findRepeatedSubstrings(string text, int minLength = 3) {
    map<string, vector<int>> repeats;

    // Search for all repeated substrings of length from minLength
    for (int len = minLength; len <= 6; len++) {
        for (int i = 0; i <= text.length() - len; i++) {
            string substr = text.substr(i, len);

            // Search for this substring further in the text
            for (int j = i + len; j <= text.length() - len; j++) {
                if (text.substr(j, len) == substr) {
                    repeats[substr].push_back(i);
                    repeats[substr].push_back(j);
                }
            }
        }
    }

    return repeats;
}

// Function to calculate GCD (Greatest Common Divisor)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to analyze distances between repetitions
vector<int> analyzeDistances(map<string, vector<int>> repeats) {
    map<int, int> distanceFrequency;

    for (auto& pair : repeats) {
        vector<int> positions = pair.second;

        // Remove duplicates
        sort(positions.begin(), positions.end());
        positions.erase(unique(positions.begin(), positions.end()), positions.end());

        // Calculate distances between positions
        for (int i = 0; i < positions.size() - 1; i++) {
            for (int j = i + 1; j < positions.size(); j++) {
                int distance = positions[j] - positions[i];
                distanceFrequency[distance]++;
            }
        }
    }

    // Output most frequent distances
    cout << "\n=== Distance Analysis Between Repetitions ===" << endl;
    vector<pair<int, int>> sortedDistances;
    for (auto& pair : distanceFrequency) {
        sortedDistances.push_back({pair.second, pair.first});
    }
    sort(sortedDistances.rbegin(), sortedDistances.rend());

    cout << "Most frequent distances:" << endl;
    for (int i = 0; i < min(10, (int)sortedDistances.size()); i++) {
        cout << "Distance: " << sortedDistances[i].second
             << " (count: " << sortedDistances[i].first << ")" << endl;
    }

    // Find possible key lengths (divisors of distances)
    map<int, int> keyLengthScore;
    for (auto& pair : distanceFrequency) {
        int distance = pair.first;
        int frequency = pair.second;

        // Check divisors from 2 to 20
        for (int keyLen = 2; keyLen <= 20; keyLen++) {
            if (distance % keyLen == 0) {
                keyLengthScore[keyLen] += frequency;
            }
        }
    }

    cout << "\n=== Possible Key Lengths ===" << endl;
    vector<pair<int, int>> sortedKeyLengths;
    for (auto& pair : keyLengthScore) {
        sortedKeyLengths.push_back({pair.second, pair.first});
    }
    sort(sortedKeyLengths.rbegin(), sortedKeyLengths.rend());

    vector<int> topKeyLengths;
    for (int i = 0; i < min(5, (int)sortedKeyLengths.size()); i++) {
        cout << "Length: " << sortedKeyLengths[i].second
             << " (score: " << sortedKeyLengths[i].first << ")" << endl;
        topKeyLengths.push_back(sortedKeyLengths[i].second);
    }

    return topKeyLengths;
}

// Function to calculate chi-squared statistic
double chiSquared(vector<int>& observed, int total) {
    double chiSq = 0.0;

    for (int i = 0; i < 26; i++) {
        double expected = (ENGLISH_FREQ[i] / 100.0) * total;
        if (expected > 0) {
            double diff = observed[i] - expected;
            chiSq += (diff * diff) / expected;
        }
    }

    return chiSq;
}

// Function to find the best shift for one position in the key
char findBestShift(string subsequence) {
    if (subsequence.empty()) return 'A';

    double bestChiSq = 1e9;
    char bestShift = 'A';

    // Try all possible shifts (0-25)
    for (int shift = 0; shift < 26; shift++) {
        vector<int> frequency(26, 0);

        // Decrypt with this shift and count frequencies
        for (char c : subsequence) {
            if (isalpha(c)) {
                c = toupper(c);
                int decrypted = ((c - 'A') - shift + 26) % 26;
                frequency[decrypted]++;
            }
        }

        // Calculate chi-squared
        double chiSq = chiSquared(frequency, subsequence.length());

        // Keep the shift with lowest chi-squared (best match to English)
        if (chiSq < bestChiSq) {
            bestChiSq = chiSq;
            bestShift = 'A' + shift;
        }
    }

    return bestShift;
}

// Function to recover the key using frequency analysis
string recoverKey(string ciphertext, int keyLength) {
    cout << "\n=== Attempting to Recover Key ===" << endl;
    cout << "Using frequency analysis for key length: " << keyLength << endl;

    string key = "";

    // For each position in the key
    for (int i = 0; i < keyLength; i++) {
        // Extract every keyLength-th character starting from position i
        string subsequence = "";
        for (int j = i; j < ciphertext.length(); j += keyLength) {
            if (isalpha(ciphertext[j])) {
                subsequence += ciphertext[j];
            }
        }

        // Find the best shift for this subsequence
        char keyChar = findBestShift(subsequence);
        key += keyChar;

        cout << "Position " << (i + 1) << ": '" << keyChar << "' (subsequence length: "
             << subsequence.length() << ")" << endl;
    }

    return key;
}

// Main function for Kasiski method
void kasiskiAnalysis(string ciphertext) {
    cout << "\n========================================" << endl;
    cout << "    KASISKI METHOD CRYPTANALYSIS" << endl;
    cout << "========================================" << endl;

    cout << "\nCiphertext length: " << ciphertext.length() << " characters" << endl;

    // Step 1: Find repeated substrings
    cout << "\n--- Step 1: Finding Repeated Fragments ---" << endl;
    map<string, vector<int>> repeats = findRepeatedSubstrings(ciphertext);

    cout << "Found repeated fragments: " << repeats.size() << endl;

    // Show most interesting repetitions
    int count = 0;
    for (auto& pair : repeats) {
        if (count++ >= 10) break; // Show only first 10

        vector<int> positions = pair.second;
        sort(positions.begin(), positions.end());
        positions.erase(unique(positions.begin(), positions.end()), positions.end());

        if (positions.size() >= 2) {
            cout << "Fragment '" << pair.first << "' found at positions: ";
            for (int pos : positions) {
                cout << pos << " ";
            }
            cout << endl;
        }
    }

    // Step 2: Distance analysis
    cout << "\n--- Step 2: Distance Analysis ---" << endl;
    vector<int> possibleKeyLengths = analyzeDistances(repeats);

    if (possibleKeyLengths.empty()) {
        cout << "\nFailed to determine key length!" << endl;
        return;
    }

    cout << "\nMost likely key length: " << possibleKeyLengths[0] << endl;

    // Step 3: Try to recover the key
    cout << "\n--- Step 3: Key Recovery ---" << endl;

    // Try the top 3 most likely key lengths
    vector<pair<string, double>> candidateKeys;

    for (int i = 0; i < min(3, (int)possibleKeyLengths.size()); i++) {
        int keyLen = possibleKeyLengths[i];
        cout << "\nTrying key length " << keyLen << "..." << endl;

        string recoveredKey = recoverKey(ciphertext, keyLen);

        // Test this key by decrypting and checking the result
        string decrypted = decryptVigenere(ciphertext, recoveredKey);

        // Calculate overall chi-squared for the decrypted text
        vector<int> frequency(26, 0);
        int letterCount = 0;
        for (char c : decrypted) {
            if (isalpha(c)) {
                frequency[toupper(c) - 'A']++;
                letterCount++;
            }
        }

        double chiSq = chiSquared(frequency, letterCount);
        candidateKeys.push_back({recoveredKey, chiSq});

        cout << "Recovered key: " << recoveredKey << " (chi-squared: " << chiSq << ")" << endl;
    }

    // Sort by chi-squared (lower is better)
    sort(candidateKeys.begin(), candidateKeys.end(),
         [](const pair<string, double>& a, const pair<string, double>& b) {
             return a.second < b.second;
         });

    cout << "\n=== BEST KEY CANDIDATE ===" << endl;
    cout << "Key: " << candidateKeys[0].first << endl;
    cout << "Chi-squared score: " << candidateKeys[0].second << endl;

    // Show preview of decrypted text with best key
    string bestDecrypted = decryptVigenere(ciphertext, candidateKeys[0].first);
    cout << "\nDecrypted text preview (first 300 characters):" << endl;
    cout << string(50, '-') << endl;
    cout << bestDecrypted.substr(0, min(300, (int)bestDecrypted.length())) << endl;
    cout << string(50, '-') << endl;
}

int main() {
    int choice;
    cout << "========================================" << endl;
    cout << "  VIGENERE CIPHER & KASISKI METHOD" << endl;
    cout << "========================================" << endl;
    cout << "\nChoose mode:" << endl;
    cout << "1. Encrypt text from file" << endl;
    cout << "2. Kasiski cryptanalysis with automatic key recovery" << endl;
    cout << "Your choice: ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        // Encryption mode
        string inputFile, outputFile, key;

        cout << "\nEnter input file name: ";
        getline(cin, inputFile);

        cout << "Enter encryption key: ";
        getline(cin, key);

        cout << "Enter output file name: ";
        getline(cin, outputFile);

        // Read text from file
        ifstream inFile(inputFile);
        if (!inFile) {
            cout << "Error: cannot open file " << inputFile << endl;
            return 1;
        }

        string text, line;
        while (getline(inFile, line)) {
            text += line + " ";
        }
        inFile.close();

        // Limit text to 1000 characters
        if (text.length() > 1000) {
            text = text.substr(0, 1000);
        }

        cout << "\nPlaintext length: " << text.length() << " characters" << endl;

        // Encrypt text
        string encrypted = encryptVigenere(text, key);

        // Write to file
        ofstream outFile(outputFile);
        if (!outFile) {
            cout << "Error: cannot create file " << outputFile << endl;
            return 1;
        }

        outFile << encrypted;
        outFile.close();

        cout << "Text encrypted successfully!" << endl;
        cout << "Encrypted text saved to file: " << outputFile << endl;
        cout << "\nFirst 100 characters of ciphertext:" << endl;
        cout << encrypted.substr(0, min(100, (int)encrypted.length())) << endl;

    } else if (choice == 2) {
        // Cryptanalysis mode
        string inputFile;

        cout << "\nEnter ciphertext file name: ";
        getline(cin, inputFile);

        // Read ciphertext
        ifstream inFile(inputFile);
        if (!inFile) {
            cout << "Error: cannot open file " << inputFile << endl;
            return 1;
        }

        string ciphertext, line;
        while (getline(inFile, line)) {
            ciphertext += line;
        }
        inFile.close();

        // Perform Kasiski analysis with automatic key recovery
        kasiskiAnalysis(ciphertext);

        // Offer to save decrypted text
        cout << "\n\nWould you like to save the decrypted text? (y/n): ";
        char answer;
        cin >> answer;
        cin.ignore();

        if (answer == 'y' || answer == 'Y') {
            string key, outputFile;
            cout << "Enter the key to use (or press Enter to use the recovered key): ";
            getline(cin, key);

            if (key.empty()) {
                cout << "Please enter the recovered key from above: ";
                getline(cin, key);
            }

            cout << "Enter output file name: ";
            getline(cin, outputFile);

            string decrypted = decryptVigenere(ciphertext, key);

            ofstream outFile(outputFile);
            outFile << decrypted;
            outFile.close();

            cout << "\nDecrypted text saved to file: " << outputFile << endl;
        }

    } else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}