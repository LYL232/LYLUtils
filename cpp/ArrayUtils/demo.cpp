#include<bits/stdc++.h>
#include "ArrayUtils.h"

const int MAX_N = (int) 1e5 + 25;
ArrayUtils<int> arrayUtils((size_t) MAX_N);

void demo_discretize() {
    int init[] = {0, 1, 12, 13, 4, 6, 7, 6, 6, 12, 2, 2, 3, 4, 5}, *res = new int[15];
    arrayUtils.init(init, 14);
    arrayUtils.discretize(init, res);
    for (int i = 1; i <= 14; ++i) {
        printf("%d ", res[i]);
    }
    putchar('\n');
    delete (res);
}

int main() {
    demo_discretize();
    return 0;
}

