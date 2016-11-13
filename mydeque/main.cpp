#include <iostream>
#include "deque.h"
#include <typeinfo>


using namespace std;

int main() {
    Deque<int> d;
    for (int i = 0; i < 10; ++i)
        d.push_back(i);
    Deque<int>::const_reverse_iterator it;
    for (it = d.rbegin(); it != d.rend(); ++it) {
//        cout << *it << ' ';
    }
//    cout << endl;
    const Deque<int> d1 = d;
    cout << d1[0] << endl;
//    (*d1.cbegin()) = 12;
    cout << d1[0] << endl;
    return 0;
}