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

std::vector<Graph<int>::Node_iterator>  test(Graph<int> &to_work,const int &to_search){
    std::vector<Graph<int>::Node_iterator> to_return;
    for(auto i = to_work.begin_edge() ; i!= to_work.end_edge() ; i++){
        if((*i).first == to_search)to_return.push_back(to_work.find((*i).second)); else
        if((*i).second == to_search)to_return.push_back(to_work.find((*i).first));
    }
    return to_return;
}

int main() {
    int b;

    //freopen("output.txt","w",stdout);
    Graph<int> a;
    a.add_node(1);
    a.add_node(3);
    a.add_node(2);
    a.add_node(4);
    a.add_node(6);
    a.add_edge(1, 1);
    a.add_edge(1, 6);
    a.add_edge(3, 2);
    a.add_edge(4, 1);
    a.add_edge(6, 3);
    //for_each(a.begin_edge(),a.end_edge(),[](auto tmp){cout_pair(tmp);});
    for(auto &i:test(a,1))
        cout << *i;
}
