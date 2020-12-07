#include <iostream>
#include "Graph.h"
#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;

struct fuck {
    int bb;
};

struct huge {
    huge() {}

    long long ind;
    double *tst;

    bool operator==(const huge &to_comp) const {
        return this->ind == to_comp.ind;
    }

    huge(long long a) {
//        cout << 3;
        ind = a;
        tst = new double[1000000];
    }

    huge &operator=(const huge &rhs) {
//        cout << 2;
        if (this == &rhs)
            return *this;
        ind = rhs.ind;
        tst = new double[1000000];
    }

    huge(const huge &a) {
//        cout << 1;
        this->ind = a.ind;
        this->tst = new double[1000000];
    }

    ~huge() {
//        cout << 4;
        delete[] tst;
    }
};

template<class T>
void cout_pair(std::pair<T, T> to_cout) {
    cout << to_cout.first << ' ' << to_cout.second << endl;
}

int main() {
    int b;

    //freopen("output.txt","w",stdout);
    Graph<huge > a;
    for(int i = 9 ; i < 20000 ; i++)
        a.add_node(huge(i));
    cout << 4;
    cin >> b;
    a.clear();
    cout << 4;
    cin >> b ;
}
