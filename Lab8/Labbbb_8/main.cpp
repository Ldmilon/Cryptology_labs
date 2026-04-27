#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct Point {
    int x, y;
};

class SteganoVariant4 {
private:
    // ZigZag path generation (Scan method 2) [cite: 126]
    vector<Point> generateZigZagPath(int width, int height, int maxPoints) {
        vector<Point> path;
        for (int s = 0; s <= width + height - 2; ++s) {
            if (s % 2 == 0) {
                for (int x = s; x >= 0; --x) {
                    int y = s - x;
                    if (x < width && y < height) {
                        path.push_back({x, y});
                        if (path.size() >= maxPoints) return path;
                    }
                }
            } else {
                for (int x = 0; x <= s; ++x) {
                    int y = s - x;
                    if (x < width && y < height) {
                        path.push_back({x, y});
                        if (path.size() >= maxPoints) return path;
                    }
                }
            }
        }
        return path;
    }

public:
    // Additive Method (Function 2: f' = f + L*w) [cite: 187]
    void additiveEmbed(vector<unsigned char>& img, const string& text, int L, int w, int h, int offset) {
        vector<Point> path = generateZigZagPath(w, h, text.length());
        for (size_t i = 0; i < text.length(); ++i) {
            int pixelPos = offset + (path[i].y * w + path[i].x) * 3;
            if (pixelPos < img.size()) {
                int newVal = img[pixelPos] + (L * (unsigned char)text[i]);
                img[pixelPos] = (unsigned char)(newVal % 256); // Overflow handling [cite: 337-339]
            }
        }
    }

    // Extraction for Additive Method [cite: 164]
    string additiveExtract(const vector<unsigned char>& original, const vector<unsigned char>& stego, int L, int w, int h, int offset, int length) {
        string res = "";
        vector<Point> path = generateZigZagPath(w, h, length);
        for (int i = 0; i < length; ++i) {
            int pos = offset + (path[i].y * w + path[i].x) * 3;
            int diff = (int)stego[pos] - (int)original[pos];
            if (diff < 0) diff += 256;
            res += (char)(diff / L);
        }
        return res;
    }

    // LSB Method (2 bits) [cite: 174, 183]
    void lsb2BitEmbed(vector<unsigned char>& img, const string& text, int offset) {
        size_t byteIdx = offset;
        for (unsigned char c : text) {
            for (int shift = 6; shift >= 0; shift -= 2) {
                if (byteIdx >= img.size()) return;
                unsigned char bitsToHide = (c >> shift) & 0x03;
                img[byteIdx] = (img[byteIdx] & 0xFC) | bitsToHide;
                byteIdx++;
            }
        }
    }

    string lsb2BitExtract(const vector<unsigned char>& img, int length, int offset) {
        string res = "";
        size_t byteIdx = offset;
        for (int i = 0; i < length; ++i) {
            unsigned char c = 0;
            for (int shift = 6; shift >= 0; shift -= 2) {
                c |= (img[byteIdx] & 0x03) << shift;
                byteIdx++;
            }
            res += c;
        }
        return res;
    }
};

int main() {
    const int L_CONST = 5; // Constant L (Rule 4) [cite: 199]
    string secret = "TEST123";

    ifstream file("input.bmp", ios::binary);
    vector<unsigned char> original((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    if (original.empty()) {
        cout << "Error: 'input.bmp' not found!" << endl;
        return 1;
    }

    int offset = *(int*)&original[10];
    int width = *(int*)&original[18];
    int height = *(int*)&original[22];

    cout << "--- Container Info ---" << endl;
    cout << "Offset: " << offset << endl;
    cout << "Resolution: " << width << "x" << height << endl;

    SteganoVariant4 tool;

    // Additive test
    cout << "\n--- Additive Method (ZigZag) ---" << endl;
    vector<unsigned char> stegoAdd = original;
    tool.additiveEmbed(stegoAdd, secret, L_CONST, width, height, offset);
    string resAdd = tool.additiveExtract(original, stegoAdd, L_CONST, width, height, offset, secret.length());
    cout << "Extracted: " << resAdd << endl;

    // LSB test
    cout << "\n--- LSB Method (2-bit) ---" << endl;
    vector<unsigned char> stegoLsb = original;
    tool.lsb2BitEmbed(stegoLsb, secret, offset);
    string resLsb = tool.lsb2BitExtract(stegoLsb, secret.length(), offset);
    cout << "Extracted: " << resLsb << endl;

    // Save files
    ofstream f1("out_add.bmp", ios::binary);
    f1.write((char*)stegoAdd.data(), stegoAdd.size());
    ofstream f2("out_lsb.bmp", ios::binary);
    f2.write((char*)stegoLsb.data(), stegoLsb.size());

    cout << "\nFiles saved: 'out_add.bmp' and 'out_lsb.bmp'" << endl;

    return 0;
}