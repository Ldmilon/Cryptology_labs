uppercase = "АБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ"
lowercase = "абвгґдеєжзиіїйклмнопрстуфхцчшщьюя"
print(f"Великий {uppercase}, малий {lowercase}")
print("\nШифрування Цезаря")
text = input("Введіть текст: ")
while True:
    try:
        key = int(input("Введіть додатнє число: "))
        if key > 0:
            break
        else:
            print("Число повинно бути більше 0")
    except ValueError:
        print("Потрібно вводити тільки число")
cipher = []

for char in text:
    if char in lowercase:
        current_index = lowercase.index(char)
        new_index = (current_index + key) % len(lowercase)
        cipher.append(lowercase[new_index])
    elif char in uppercase:
        current_index = uppercase.index(char)
        new_index = (current_index + key) % len(uppercase)
        cipher.append(uppercase[new_index])
    else:
        cipher.append(char)
result = "".join(cipher)
with open("сypher1.txt", "w", encoding="utf-8") as f:
    f.write(result)
print(result)
print("\nДешифрування Цезаря")
with open("сypher1.txt", "r", encoding="utf-8") as f:
    cipherfromfile = f.read()
cipher = []
with open("open1.txt", "w", encoding="utf-8") as f:
    pass
for i in range(len(lowercase)):
    for char in cipherfromfile:
        if char in lowercase:
            current_index = lowercase.index(char)
            new_index = (current_index - (i+1)) % len(lowercase)
            cipher.append(lowercase[new_index])
        elif char in uppercase:
            current_index = uppercase.index(char)
            new_index = (current_index - (i+1)) % len(uppercase)
            cipher.append(uppercase[new_index])
        else:
            cipher.append(char)
    decrypted = "".join(cipher)
    print(decrypted)
    with open("open1.txt", "a", encoding="utf-8") as f:
        f.write(f"{decrypted}, key {i + 1}\n")
    decrypted = []
    cipher = []
print("\n--- XOR шифрування ---")
xor_key = int(input("Введіть ключ XOR: "))
xor_cipher = []
for char in text:
    xor_char = chr(ord(char) ^ xor_key)
    xor_cipher.append(xor_char)
xor_result = "".join(xor_cipher)
print("XOR шифротекст:", xor_result)
with open("cypher2.txt", "w", encoding="utf-8") as f:
    f.write(xor_result)
print("\n--- XOR дешифрування ---")
xor_key = int(input("Введіть ключ XOR для дешифрування: "))
with open("cypher2.txt", "r", encoding="utf-8") as f:
    xor_from_file = f.read()
xor_decrypted = []
for char in xor_from_file:
    open_char = chr(ord(char) ^ xor_key)
    xor_decrypted.append(open_char)

xor_open_text = "".join(xor_decrypted)
print("Розшифрований текст:", xor_open_text)
with open("open2.txt", "w", encoding="utf-8") as f:
    f.write(xor_open_text)