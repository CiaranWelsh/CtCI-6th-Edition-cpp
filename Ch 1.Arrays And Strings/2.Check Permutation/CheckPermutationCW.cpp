//
// Created by Ciaran on 14/08/2021.
//

#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <cassert>

/**
 * Given two strings, write a method to decide if one is a permutation of the
 * other.
 *
 * notes
 * ------
 * What does permutation mean? If a string has the same characters as another but in a different
 * order, it as a permutation. Also called an anagram, when applied to letters and words.
 * Strings must necessarily be the same length.
 *
 * Sort and compare like for like
 */

/**
 * @brief Algorithm for comparing strings  @param s1 and
 * @param s2. Returns true if these strings are permutations
 * of eachother.
 * @details uses std::sort, which has complexity n*log(n)
 */
bool isPermutationNLogN(std::string &s1, std::string &s2) {
    // if these strings are not the same size, they are not
    // permutations
    if (s1.size() != s2.size())
        return false;

    // now we sort the strings and compare character for character
    std::sort(s1.begin(), s1.end());
    std::sort(s2.begin(), s2.end());
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i]) {
            return false;
        }
    }
    return true;
}

std::unordered_map<char, int> countCharAsMap(std::string &s) {
    std::unordered_map<char, int> m;
    for (char s1IthChar : s) {
        if (m.find(s1IthChar) != m.end()) {
            // we've seen this char before
            m[s1IthChar]++;
        } else {
            m.insert({s1IthChar, 1});
        }
    }
    return m;
}

/**
 * @brief This is an alternative solution that uses O(N) time, but it requires
 * more space than isPermutationNLogN.
 * @details I'll use a hash map for fast lookup and try to make sure that
 *
 * @notes on my implementation. The complexity is O(N), since each loop (3 in total)
 * is a maximum of size n. However, this could be reduced to 2 loops if done in the way the
 * example does it.
 *      They keep a "count" vector, one for each char in ascii characters. Init to 0. When iter
 *      through the first string you add 1 for each character you observe. For the second string, you
 *      minus 1. If any final counts are > 0 then they are not permutations.
 */
bool isPermutationON(std::string &s1, std::string &s2) {
    if (s1.size() != s2.size()) {
        return false;
    }
    std::unordered_map<char, int> m1 = countCharAsMap(s1);
    std::unordered_map<char, int> m2 = countCharAsMap(s2);

    for (auto c : s2){
        if (m1.find(c) == m1.end()){
            // if character from s2 not in the s1 map
            // they are not permutations
            return false;
        } else {
            // if it is, then we must check how many
            if ( m1[c] != m2[c]){
                return false;
            }
        }
    }
    // if we get this far, we have permutations.
    return true;


}

int main() {
    {
        // true case
        std::string s1 = "permutation";
        std::string s2 = "neutmatriop";
        assert(isPermutationNLogN(s1, s2));
    }
    {
        // false case
        std::string s1 = "permutation";
        std::string s2 = "NotAPerm";
        assert(!isPermutationNLogN(s1, s2));
    }
    {
        // true case
        std::string s1 = "permutation";
        std::string s2 = "neutmatriop";
        assert(isPermutationON(s1, s2));
    }
    {
        // false case
        std::string s1 = "permutation";
        std::string s2 = "NotAPerm";
        assert(!isPermutationON(s1, s2));
    }
}

