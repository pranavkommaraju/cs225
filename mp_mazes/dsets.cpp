/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; ++i) {
        sets.push_back(-1);
    }
    
}

int DisjointSets::find(int elem) {
    
    if (sets.at(elem) < 0) {
        return elem;
    } else {
        return sets.at(elem) = find(sets.at(elem));
    }
}

void DisjointSets::setunion(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) {
        return;
    }
    int newsize = size(a) + size(b);
    if (size(a) < size(b)) {
        sets.at(a) = b;
        sets.at(b) = -newsize;
    } else {
        sets.at(b) = a;
        sets.at(a) = -newsize;
    }
}

int DisjointSets::size(int elem) {
    if (sets.at(elem) < 0) {
        return std::abs(sets.at(elem));
    } else {
        return size(sets.at(elem));
    }
}