#include <iostream>

//
// Created by Ethan on 2/8/2020.
//
#include<iostream>
#include<vector>

using std::vector;
using std::cout;
using std::endl;

class IsLargerThan {
public:
    bool operator()(int a, int b) { return a > b; };
};

template<class Iterator, class Condition>
int numOfPairs(Iterator begin, Iterator end, Condition condition) {
    if (begin == end)
        return 0;
    int cnt = 0;
    for (Iterator i(begin); !(i == end); ++i) {
        for (Iterator j(begin); !(j == end); ++j) {
            if (i == j)
                continue;
            if (i < j && condition(*i, *j))
                cnt++;
        }
    }
    return cnt;
}

bool isSorted(vector<int> v) {
    IsLargerThan isLargerThan;
    for (vector<int>::iterator i = (v.begin()); i != (v.end() - 1); ++i) {
        if (numOfPairs(i, i + 2, isLargerThan))
            return false;
    }
    return true;
}

int main() {
    //Test Sorted
    vector<int> v1;
    for (int i = 0; i <= 5; i++) {
        v1.push_back(i);
    }
    for (int i:v1)
        cout << i << "\t";
    cout << "Is Sorted? " << (isSorted(v1) ? "True" : "False") << endl;
    //Test unsorted
    vector<int> v2;
    for (int i = 0; i <= 5; i++) {
        v2.push_back(i);
    }
    v2[2] = 0;
    for (int i:v2)
        cout << i << "\t";
    cout << "Is Sorted? " << (isSorted(v2) ? "True" : "False") << endl;
    return 0;
}