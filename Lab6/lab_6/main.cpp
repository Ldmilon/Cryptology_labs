#include <iostream>
#include <string>

using namespace std;

// Прототипи функцій
string get_text();
int get_key(int key[]);
void display_array_K(int key[], int keySize);
void display_initial_S(int S[]);
void KSA(int S[], int key[], int keySize);
void display_permuted_S(int S[]);
void PRNG(int S[], int keystream[], int length);
void display_keystream(int keystream[], int length);
void encrypt_text(string text, int keystream[], int encrypted[]);
void display_encrypted(int encrypted[], int length);

int main() {
    cout << "========================================" << endl;
    cout << "    RC4 STREAM CIPHER IMPLEMENTATION    " << endl;
    cout << "========================================" << endl;

    // 1. Введення відкритого тексту
    string plaintext = get_text();

    // 2. Введення ключа (гами)
    int key[256];
    int keySize = get_key(key);

    // 3. Виведення масиву K
    display_array_K(key, keySize);

    // 4. Ініціалізація та виведення початкового масиву S
    int S[256];
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }
    display_initial_S(S);

    // 5. Перемішування масиву S (KSA)
    KSA(S, key, keySize);

    // 6. Виведення перетвореного масиву S
    display_permuted_S(S);

    // 7. Генерація псевдовипадкової послідовності (ключового потоку)
    int keystream[1000];
    PRNG(S, keystream, plaintext.length());

    // 8. Виведення згенерованої послідовності
    display_keystream(keystream, plaintext.length());

    // 9. Шифрування тексту
    int encrypted[1000];
    encrypt_text(plaintext, keystream, encrypted);

    // 10. Виведення зашифрованого тексту
    display_encrypted(encrypted, plaintext.length());

    cout << "\n========================================" << endl;
    cout << "         ENCRYPTION COMPLETED           " << endl;
    cout << "========================================" << endl;

    return 0;
}

// Функція введення тексту
string get_text() {
    string text;
    cout << "\n>>> Enter plaintext to encrypt: ";
    cin.ignore(); // Очищення буфера
    getline(cin, text);

    cout << "\nPlaintext: \"" << text << "\"" << endl;

    // Виведення ASCII кодів
    cout << "Plaintext (ASCII codes): [";
    for (int i = 0; i < text.length(); i++) {
        cout << (int)text[i];
        if (i < text.length() - 1) cout << ", ";
    }
    cout << "]" << endl;

    return text;
}

// Функція введення ключа
int get_key(int key[]) {
    int keySize;

    // Перевірка довжини ключа
    do {
        cout << "\n>>> Enter key length (minimum 3): ";
        cin >> keySize;

        if (keySize < 3) {
            cout << "ERROR: Key length must be at least 3!" << endl;
        }
    } while (keySize < 3);

    // Введення елементів ключа
    cout << "\n>>> Enter " << keySize << " key elements:" << endl;
    for (int i = 0; i < keySize; i++) {
        cout << "  Key[" << i << "]: ";
        cin >> key[i];
    }

    // Виведення введеного ключа
    cout << "\nKey (gamma): [";
    for (int i = 0; i < keySize; i++) {
        cout << key[i];
        if (i < keySize - 1) cout << ", ";
    }
    cout << "]" << endl;

    return keySize;
}

// Виведення масиву K (розширений ключ)
void display_array_K(int key[], int keySize) {
    cout << "\n========================================" << endl;
    cout << "         ARRAY K (Extended Key)         " << endl;
    cout << "========================================" << endl;

    int K[256];
    for (int i = 0; i < 256; i++) {
        K[i] = key[i % keySize];
    }

    cout << "K array (first 30 elements): [";
    for (int i = 0; i < 30; i++) {
        cout << K[i];
        if (i < 29) cout << ", ";
    }
    cout << ", ...]" << endl;

    cout << "K array (last 10 elements): [..., ";
    for (int i = 246; i < 256; i++) {
        cout << K[i];
        if (i < 255) cout << ", ";
    }
    cout << "]" << endl;
}

// Виведення початкового масиву S
void display_initial_S(int S[]) {
    cout << "\n========================================" << endl;
    cout << "         INITIAL ARRAY S                " << endl;
    cout << "========================================" << endl;

    cout << "S (first 30 elements): [";
    for (int i = 0; i < 30; i++) {
        cout << S[i];
        if (i < 29) cout << ", ";
    }
    cout << ", ...]" << endl;

    cout << "S (last 10 elements): [..., ";
    for (int i = 246; i < 256; i++) {
        cout << S[i];
        if (i < 255) cout << ", ";
    }
    cout << "]" << endl;
}

// KSA - Key Scheduling Algorithm (перемішування масиву S)
void KSA(int S[], int key[], int keySize) {
    cout << "\n========================================" << endl;
    cout << "     KSA - Permuting Array S            " << endl;
    cout << "========================================" << endl;

    // Створюємо розширений масив K
    int K[256];
    for (int i = 0; i < 256; i++) {
        K[i] = key[i % keySize];
    }

    // Перемішування
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + K[i]) % 256;

        // Обмін місцями S[i] та S[j]
        int temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }

    cout << "Array S has been permuted using the key!" << endl;
}

// Виведення перетвореного масиву S
void display_permuted_S(int S[]) {
    cout << "\n========================================" << endl;
    cout << "         PERMUTED ARRAY S               " << endl;
    cout << "========================================" << endl;

    cout << "Permuted S (first 30 elements): [";
    for (int i = 0; i < 30; i++) {
        cout << S[i];
        if (i < 29) cout << ", ";
    }
    cout << ", ...]" << endl;

    cout << "Permuted S (last 10 elements): [..., ";
    for (int i = 246; i < 256; i++) {
        cout << S[i];
        if (i < 255) cout << ", ";
    }
    cout << "]" << endl;
}

// PRNG - Pseudo-Random Number Generator (генерація ключового потоку)
void PRNG(int S[], int keystream[], int length) {
    cout << "\n========================================" << endl;
    cout << "     PRNG - Generating Keystream        " << endl;
    cout << "========================================" << endl;

    int i = 0, j = 0;

    for (int n = 0; n < length; n++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;

        // Обмін місцями S[i] та S[j]
        int temp = S[i];
        S[i] = S[j];
        S[j] = temp;

        // Генерація байта ключового потоку
        int t = (S[i] + S[j]) % 256;
        keystream[n] = S[t];
    }

    cout << "Keystream generated successfully!" << endl;
}

// Виведення ключового потоку
void display_keystream(int keystream[], int length) {
    cout << "\n========================================" << endl;
    cout << "         GENERATED KEYSTREAM            " << endl;
    cout << "========================================" << endl;

    cout << "Keystream (" << length << " bytes): [";
    for (int i = 0; i < length; i++) {
        cout << keystream[i];
        if (i < length - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Шифрування тексту
void encrypt_text(string text, int keystream[], int encrypted[]) {
    cout << "\n========================================" << endl;
    cout << "         ENCRYPTION PROCESS             " << endl;
    cout << "========================================" << endl;

    cout << "\nXOR operations:" << endl;
    for (int i = 0; i < text.length(); i++) {
        int plainByte = (int)text[i];
        encrypted[i] = plainByte ^ keystream[i];

        cout << "  '" << text[i] << "' (" << plainByte << ") XOR "
             << keystream[i] << " = " << encrypted[i] << endl;
    }
}

// Виведення зашифрованого тексту
void display_encrypted(int encrypted[], int length) {
    cout << "\n========================================" << endl;
    cout << "         ENCRYPTED TEXT                 " << endl;
    cout << "========================================" << endl;

    // Числовий формат
    cout << "\nEncrypted (decimal): [";
    for (int i = 0; i < length; i++) {
        cout << encrypted[i];
        if (i < length - 1) cout << ", ";
    }
    cout << "]" << endl;

    // Hex формат
    cout << "\nEncrypted (hexadecimal): ";
    for (int i = 0; i < length; i++) {
        printf("%02X ", encrypted[i]);
    }
    cout << endl;

    // Символьний вигляд
    cout << "\nEncrypted (characters): \"";
    for (int i = 0; i < length; i++) {
        if (encrypted[i] >= 32 && encrypted[i] <= 126) {
            cout << (char)encrypted[i];
        } else {
            cout << ".";  // Непридатні для друку символи
        }
    }
    cout << "\"" << endl;
}