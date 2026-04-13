#include <iostream>
#include <string>
#include <fstream>      // Для роботи з файлами
#include <cctype>       // Для isalpha, toupper, tolower
#include <iomanip>      // Для setw, setprecision
#include <algorithm>    // Для sort
#include <vector>       // Для vector
#include <map>          // Для std::map

using namespace std;

// Оголошення функцій
void encrypt_text_file(char alphabet[], char randomAlphabet[], char alphabetUpper[], char randomAlphabetUpper[]);
void analyze_encrypted_file(const char* filename);
void decrypt_by_frequency(const char* encryptedFile, const char* decryptedFile);

int main() {
    // --- Завдання 1: Шифр заміни ---
    char alphabet[] = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w','x','y','z'
    };
    char randomAlphabet[] = {
        'q','w','e','z','t','y','a','p','r','u','o','i','s','d',
        'f','g','h','j','k','l','x','c','v','b','n','m'
    };
    char alphabetUpper[] = {
        'A','B','C','D','E','F','G','H','I','J','K','L','M',
        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
    };
    char randomAlphabetUpper[] = {
        'Q','W','E','Z','T','Y','A','P','R','U','O','I','S','D',
        'F','G','H','J','K','L','X','C','V','B','N','M'
    };

    // Крок 1: Шифруємо "text.txt" у "encrypted.txt"
    // Програма припускає, що "text.txt" вже існує.
    cout << "Reading from existing 'text.txt'..." << endl;
    encrypt_text_file(alphabet, randomAlphabet, alphabetUpper, randomAlphabetUpper);

    // --- Завдання 2: Частотний аналіз ---

    // Крок 2: Аналізуємо частоту літер у "encrypted.txt"
    analyze_encrypted_file("encrypted.txt");

    // Крок 3: Дешифруємо "encrypted.txt" у "decrypted.txt" за допомогою аналізу
    // Використовує СТАНДАРТНИЙ порядок ETAOIN...
    decrypt_by_frequency("encrypted.txt", "decrypted.txt");

    cout << "\nProcess Completed." << endl;
    return 0;
}

// --- Шифрування ---
// (Читає з "text.txt", який, як очікується, вже існує)
void encrypt_text_file(char alphabet[], char randomAlphabet[], char alphabetUpper[], char randomAlphabetUpper[]) {
    ifstream text_file("text.txt");
    ofstream encrypted("encrypted.txt");

    if (!text_file.is_open()) {
        cout << "Error: 'text.txt' not found!" << endl;
        cout << "Please make sure 'text.txt' exists in the same directory." << endl;
        return;
    }
     if (!encrypted.is_open()) {
        cout << "Error opening 'encrypted.txt' for writing!" << endl;
        return;
    }

    char ch;
    while (text_file.get(ch)) {
        bool replaced = false;
        for (int i = 0; i < 26; i++) {
            if (ch == alphabet[i]) {
                encrypted << randomAlphabet[i];
                replaced = true;
                break;
            }
        }
        if (!replaced) {
            for (int i = 0; i < 26; i++) {
                if (ch == alphabetUpper[i]) {
                    encrypted << randomAlphabetUpper[i];
                    replaced = true;
                    break;
                }
            }
        }
        if (!replaced) {
            encrypted << ch;
        }
    }

    text_file.close();
    encrypted.close();

    cout << "File 'text.txt' encrypted into 'encrypted.txt'" << endl;
}

// --- Аналіз ---
void analyze_encrypted_file(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open '" << filename << "' for analysis." << endl;
        return;
    }

    int counts[26] = {0};
    int totalLetters = 0;
    char ch;

    while (file.get(ch)) {
        if (isalpha(ch)) {
            ch = toupper(ch);
            counts[ch - 'A']++;
            totalLetters++;
        }
    }
    file.close();

    if (totalLetters == 0) {
        cout << "No letters in file '" << filename << "' to analyze!" << endl;
        return;
    }

    cout << "\n--- Frequency Analysis (" << totalLetters << " total letters) ---\n";
    cout << fixed << setprecision(2);

    vector<pair<double, char>> freqPairs;
    for (int i = 0; i < 26; i++) {
        double percentage = (counts[i] * 100.0) / totalLetters;
        freqPairs.push_back({percentage, (char)('A' + i)});
    }

    sort(freqPairs.rbegin(), freqPairs.rend());

    for(const auto& pair : freqPairs) {
         cout << pair.second << ": "
             << setw(6) << pair.first << "%" << endl;
    }
}

// --- Частотне дешифрування ---
// (Використовує стандартний порядок ETAOIN...)
void decrypt_by_frequency(const char* encryptedFile, const char* decryptedFile) {
    ifstream file(encryptedFile);
    if (!file.is_open()) {
        cout << "Error: Cannot open '" << encryptedFile << "' for decryption." << endl;
        return;
    }

    // 1. Підрахунок частоти
    int counts[26] = {0};
    string encryptedText;
    char ch;

    while (file.get(ch)) {
        encryptedText += ch;
        if (isalpha(ch)) {
            ch = toupper(ch);
            counts[ch - 'A']++;
        }
    }
    file.close();

    // 2. Створення пар (літера, частота)
    vector<pair<char, int>> freq;
    for (int i = 0; i < 26; i++) {
        freq.push_back({(char)('A' + i), counts[i]});
    }

    // 3. Сортування за спаданням частоти
    sort(freq.begin(), freq.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // 4. Типовий (стандартний) порядок частотності англійських літер:
    string englishFreq = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

    // 5. Створюємо таблицю відповідностей
    // (Найчастіша в шифрі -> 'E', друга за частотою -> 'T', і т.д.)
    map<char, char> decryptionMap;
    for (int i = 0; i < 26; i++) {
        // freq[i].first - це i-та найчастіша літера в шифротексті
        // englishFreq[i] - це i-та найчастіша літера в стандарті
        decryptionMap[freq[i].first] = englishFreq[i];
    }

    // (Друкуємо карту в консоль, щоб бачити, що програма "придумала")
    cout << "\n--- Guessed Decryption Map (Cipher -> Standard) ---" << endl;
    for(size_t i = 0; i < freq.size(); ++i) {
        cout << "  " << freq[i].first << " -> " << englishFreq[i] << endl;
    }


    // 6. Замінюємо символи і записуємо у файл
    ofstream decrypted(decryptedFile);
    if(!decrypted.is_open()){
        cout << "Error: Cannot create '" << decryptedFile << "'." << endl;
        return;
    }

    for (char c : encryptedText) {
        if (isalpha(c)) {
            if (isupper(c)) {
                decrypted << decryptionMap.at(c);
            } else {
                decrypted << (char)tolower(decryptionMap.at(toupper(c)));
            }
        } else {
            decrypted << c;
        }
    }

    decrypted.close();
    cout << "\nDecryption attempt saved to 'decrypted.txt'\n";
}
