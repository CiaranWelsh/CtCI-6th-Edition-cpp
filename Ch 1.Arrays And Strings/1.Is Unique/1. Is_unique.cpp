#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <algorithm> // for sort() 

using namespace std;

bool isUniqueChars(const string &str) {
    // ascii uses 9 bits, but 1 is an error checking bit.
    // so ascii represents characters with 7 bits
    // 2^7 = 2 = 128 possible characters.
    //
    // obvs only works with ascii, not unicode.
    if (str.length() > 128) {
        return false;
    }
    // create an empty vector big enough to store true or false for each possible char
    vector<bool> char_set(128);
    for (auto val : str){
        // if we've seen char_set before, the index will be  true
        // and we are not unique.
        if (char_set[val]) {
            return false;
        }
        // otherwise set the value of the vector to true
        char_set[val] = true;
    }
    // if we get this far, all char in the string are unique
    return true;
}

bool isUniqueChars_bitvector(const string &str) {
    //Reduce space usage by a factor of 8 using bitvector.
    //Each boolean otherwise occupies a size of 8 bits.
    bitset<128> bits(0);
    for (auto val : str){
        if (bits.test(val) > 0) {
            return false;
        }
        bits.set(val);
    }
    return true;
}

bool isUniqueChars_noDS(string str) {
    // 
    sort(str.begin(), str.end()); // O(nlogn) sort from <algorithm>

    bool noRepeat = true;
    for (int i = 0; i < str.size() - 1; i++) {
        if (str[i] == str[i + 1]) {
            noRepeat = false;
            break;
        }
    }

    return noRepeat;
}

int main() {
    vector<string> words = {"abcde", "hello", "apple", "kite", "padle"};
    for (auto word : words) {
        cout << word << string(": ") << boolalpha << isUniqueChars(word) << endl;
    }
    cout << endl << "Using bit vector" << endl;
    for (auto word : words) {
        cout << word << string(": ") << boolalpha << isUniqueChars_bitvector(word) << endl;
    }
    cout << endl << "Using no Data Structures" << endl;
    for (auto word : words) {
        cout << word << string(": ") << boolalpha << isUniqueChars_noDS(word) << endl;
    }
    return 0;
}

