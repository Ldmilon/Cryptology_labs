This section describes the methods used to implement the symmetric block cipher S-DES.

strToBits(string s): Converts a binary string into a custom Bits container (vector of integers).

bitsToStr(Bits b): Converts a bit sequence back into a string format for console output.

permute(Bits b, vector<int> p): A general-purpose function that rearranges bits according to a specified permutation table.

shift(Bits b, int n): Performs a cyclic left shift on a bit sequence by a given number of positions.

generateKeys(Bits key10, Bits &K1, Bits &K2): Implements the key schedule algorithm. It applies P10 permutation, performs shifts, and uses P8 permutation to generate two 8-bit round keys.

sbox(Bits b, int matrix[4][4]): Processes a 4-bit input through an S-Box (Substitution Box) by using specific bits for row and column indexing.

fK(Bits b, Bits k): The core Feistel function. It includes expansion (EP), XORing with the round key, S-Box substitution, and P4 permutation.

encryptBlock(Bits plain8, Bits K1, Bits K2): Orchestrates the encryption process: Initial Permutation (IP) -> Round 1 -> Switch (SW) -> Round 2 -> Inverse IP.

decryptBlock(Bits cipher8, Bits K1, Bits K2): Performs decryption by using the same logic as encryption but with the round keys applied in reverse order (K2 then K1).
