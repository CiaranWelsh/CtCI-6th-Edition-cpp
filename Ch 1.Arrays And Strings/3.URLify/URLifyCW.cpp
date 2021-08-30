//
// Created by Ciaran on 14/08/2021.
//

#include <string>
#include <cassert>
#include <iostream>

/**
 * URLify: Write a method to replace all spaces in a string with '%20'. You may assume that the string
 * has sufficient space at the end to hold the additional characters, and that you are given the "true"
 * length of the string. (Note: If implementing in Java, please use a character array so that you can
 * perform this operation in place.)
 * EXAMPLE
 * Input: "Mr John Smith ", 13
 * Output: "Mr%20John%20Smith"
 * Hints: #53, # 118
 */


std::string urlify(std::string& s){
    std::string sNew;
    for (int i=0; i<s.size(); i++){
        char c = s[i];
        if (c == ' '){
            sNew += "%20";
        } else {
            sNew += c;
        }
    }
    return sNew;
}

int main(){
    std::string url = "Mr John Smith ";
    std::string expected = "Mr%20John%20Smith%20";
    std::string out = urlify(url);
    std::cout << "url: " << url << std::endl;
    std::cout << "out: " << out << std::endl;
    assert(expected == out);

}