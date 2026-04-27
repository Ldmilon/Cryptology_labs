This section details the functions used to implement the RC4 (Rivest Cipher 4) stream encryption algorithm.

swap(int *a, int *b): A utility function to swap two integer values in the state array.

ksa(int key[], int key_length, int S[]): The Key Scheduling Algorithm. It initializes the state array S (from 0 to 255) and performs a permutation based on the provided secret key.

prga(int S[], int data_length, int keystream[]): The Pseudo-Random Generation Algorithm. It generates a keystream of a specific length by continuously shuffling the state array.

rc4_encrypt(int keystream[], char plaintext[], int encrypted[], int length): Encrypts the plaintext by performing a bitwise XOR operation between each character and the corresponding byte from the RC4 keystream.

display_encrypted(int encrypted[], int length): Formats and displays the encryption results in decimal, hexadecimal, and character formats for analysis.
