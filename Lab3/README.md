Third lab realization Vigenere cipher encryption and cryptanalysis using Kasiski method and frequency analysis
There are functions like:
encryptVigenere - Encrypts text using the Vigenere cipher. Each letter of the plaintext is shifted by the corresponding letter of the key (modulo 26). Non-letter characters are ignored, and all letters are converted to uppercase.
decryptVigenere - Decrypts text encrypted with the Vigenere cipher. Performs reverse shifting of letters using the key: (text − key + 26) mod 26.
findRepeatedSubstrings - Finds repeated substrings in the ciphertext (used in Kasiski method). Searches for fragments of length 3 to 6 and stores their positions for further analysis.
gcd - Calculates the greatest common divisor of two numbers using the Euclidean algorithm. Used for mathematical analysis of distances (indirectly related to key length detection).
analyzeDistances - Analyzes distances between repeated substrings. Determines the most frequent distances and calculates possible key lengths by checking their divisors. Returns the most probable key lengths.
chiSquared - Calculates the chi-squared statistic. Compares observed letter frequencies in text with standard English letter frequencies. Used to evaluate how close text is to real English.
findBestShift - Finds the best Caesar shift for a substring. Tries all possible shifts (0–25) and selects the one with the lowest chi-squared value. Helps determine one character of the Vigenere key.
recoverKey - Recovers the Vigenere key using frequency analysis. Splits ciphertext into groups based on key length and determines each key character independently.
kasiskiAnalysis - Main function for cryptanalysis. Performs: detection of repeated substrings, distance analysis, estimation of key length, recovery of possible keys, decryption preview
