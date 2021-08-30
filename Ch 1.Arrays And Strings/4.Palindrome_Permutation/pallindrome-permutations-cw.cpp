

#include <string>
#include <exception>
#include <algorithm>
#include <iostream>

class EndOfIterationException : public std::exception {
public:
    EndOfIterationException(std::string what) : std::exception(what.c_str()){}
};

/**
 * Find all permutations.
 * Check if any are palindromes.
 *  if true return true
 *  else return false.
 */

class Generator {
public:
    Generator() = default;

    virtual bool next() = 0;

    virtual void reset() = 0;

    virtual void generate() = 0;

    bool isFinished() const {
        return isFinished_;
    }

protected:
    bool isFinished_ = false;
};


class PermutationGenerator : public Generator {
public:

    explicit PermutationGenerator(const std::string& s)
        :s_(s), tmp_(s){}

    bool isPermutation( std::string of){
        if (of.size() != s_.size()){
            return false;
        }
        std::string thisS = s_;
        std::sort(s_.begin(), s_.end());
        std::sort(of.begin(), of.end());
        for (int i=0; i<of.size(); i++){
            if (s_[i] != of.size()){
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Generate the next logical permutation
     *
     * @notes given a string s="House", to generate permutations you
     * iteratively swap char in the string, unless the two characters are
     * identical. We need two indexes, to keep track of the current pair
     */
    bool next() override{
//        if (i == 0 && j == 0){
//            j++;
//            return s_;
//        }
        if (j == tmp_.size()-1){
            if (i == tmp_.size()-1){
                return false;
            }
            i++;
        }
        j++;
        std::swap(tmp_[i], tmp_[j]);
        return true;
    }

    void reset() override{
        i = 0;
        j = 0;
    }

    void generate() override{

    }

private:
    std::string s_;
    std::string tmp_;
    int i = 0;
    int j = 0;
};

bool isPalindrome(std::string s){
    if (s.size() == 0 || s.size() == 1){
        return true;
    }
    char first = s[0];
    char last = s[s.size()-1];
    if (first != last){
        return false;
    } else {
        s.erase(s.begin());
        s.erase(s.end());
        return isPalindrome(s);
    }
}


int main(){
    PermutationGenerator permutationGenerator("words");
    while (!permutationGenerator.isFinished()){
        std::cout << permutationGenerator.next() << std::endl;
    }
}















