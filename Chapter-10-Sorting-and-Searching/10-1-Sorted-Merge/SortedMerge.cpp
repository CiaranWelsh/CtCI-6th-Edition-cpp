
#include <vector>
#include <iostream>


std::vector<int> sortedMerge(std::vector<int> first, std::vector<int> second) {
    std::vector<int> smaller;
    std::vector<int> larger;
    if (first.size() < second.size()) {
        smaller = first;
        larger = second;
    } else {
        larger = first;
        smaller = second;
    }
    std::vector<int> merged(smaller.size() + larger.size());
    // when smaller and larger have comparable indices
    for (auto i = 0; i < smaller.size() - 1; i++) {
        int mergeIdx = i*2;
        if (smaller[i] <= larger[i]) {
            merged[mergeIdx] = smaller[i];
            merged[mergeIdx+1] = larger[i];
        } else {
            merged[i] = larger[i];
            merged[i + 1] = smaller[i];
        }
    }

    // when handling the larger
    auto mergeIdx = 2*smaller.size();
    for (auto i=smaller.size()-1; i<larger.size()-1; i++){
        std::cout << i << std::endl;
        merged[i] = larger[i];
        mergeIdx++;
    }
    return merged;
}


int main() {

    {
        std::vector<int> smaller({1, 2, 3, 4,5});
        std::vector<int> larger({1, 2, 3, 4, 5, 6, 7, 8});
        std::vector<int> merged = sortedMerge(smaller, larger);

        std::cout << "merged: ";
        for (auto i: merged){
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }


    return 0;
}






















