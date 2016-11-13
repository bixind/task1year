#include <iostream>
#include "deque.h"
#include <gtest/gtest.h>
#include <deque>
#include <iterator>

enum E_Operation_Type {
    OT_PUSH_FRONT,
    OT_PUSH_BACK,
    OT_POP_FRONT,
    OT_POP_BACK
};

E_Operation_Type randomOperation(int wPUSHFRONT, int wPUSHBACK, int wPOPFRONT, int wPOPBACK) {
    int r = rand() % (wPOPFRONT + wPOPBACK + wPUSHBACK + wPUSHFRONT);
    if (r < wPOPFRONT)
        return OT_POP_FRONT;
    if (r < wPOPFRONT + wPOPBACK)
        return OT_POP_BACK;
    if (r < wPOPBACK + wPOPFRONT + wPUSHBACK)
        return OT_PUSH_BACK;
    return OT_PUSH_FRONT;
}

template <class T>
using OperationList = std::vector<std::pair<T, E_Operation_Type> >;

const OperationList<int> generateIntTest(size_t len, int waddf, int waddb, int wpopf, int wpopb) {
    OperationList<int> l(len);
    int balance = 0;
    for (size_t i = 0; i < len; ++i) {
        E_Operation_Type ot;
        if (balance > 0)
            ot = randomOperation(waddf, waddb, wpopf, wpopb);
        else
            ot = randomOperation(waddf, waddb, 0, 0);
        if (ot == OT_POP_FRONT || ot == OT_POP_BACK)
            --balance;
        else
            ++balance;
        l[i] = std::make_pair(rand(), ot);
    }
    return l;
}

template <class MyIterator, class StlIterator>
void testIterators(MyIterator mBegin, MyIterator mEnd, StlIterator sBegin, StlIterator sEnd) {
    for (; sBegin != sEnd; ++sBegin, ++mBegin)
        ASSERT_EQ(*sBegin, *mBegin);
    ASSERT_TRUE((mBegin == mEnd));
};

template<class T>
void testForEquality(Deque<T> myDeque, std::deque<T> stlDeque) {
    ASSERT_EQ(myDeque.size(), stlDeque.size());
    for (size_t i = 0; i < myDeque.size(); ++i) {
        ASSERT_EQ(myDeque[i], stlDeque[i]);
    }
    testIterators(myDeque.begin(), myDeque.end(), stlDeque.begin(), stlDeque.end());
    testIterators(myDeque.cbegin(), myDeque.cend(), stlDeque.cbegin(), stlDeque.cend());
    testIterators(myDeque.rbegin(), myDeque.rend(), stlDeque.rbegin(), stlDeque.rend());
    testIterators(myDeque.crbegin(), myDeque.crend(), stlDeque.crbegin(), stlDeque.crend());
}

template<class T>
const std::pair<Deque<T>, std::deque<T> > executeAndCheck(OperationList<T> operations, bool detailed) {
    std::deque<T> stlDeque;
    Deque<T> myDeque;
    for (size_t i = 0; i < operations.size(); ++i) {
        switch (operations[i].second) {
            case OT_POP_BACK:
                stlDeque.pop_back();
                myDeque.pop_back();
                break;
            case OT_POP_FRONT:
                stlDeque.pop_front();
                myDeque.pop_front();
                break;
            case OT_PUSH_BACK:
                stlDeque.push_back(operations[i].first);
                myDeque.push_back(operations[i].first);
                break;
            case OT_PUSH_FRONT:
                stlDeque.push_front(operations[i].first);
                myDeque.push_front(operations[i].first);
                break;
        }
        if (detailed)
            testForEquality(myDeque, stlDeque);
    }
    testForEquality(myDeque, stlDeque);
    return std::make_pair(myDeque, stlDeque);
}

TEST(basic_tests_case, push_test1) {
    OperationList<int> list;
    for (size_t i = 0; i < 100; ++i)
        list.push_back(std::make_pair(i, OT_PUSH_BACK));
    executeAndCheck(list, true);
}

TEST(basic_tests_case, push_test2) {
    OperationList<int> list;
    for (size_t i = 0; i < 100; ++i)
        list.push_back(std::make_pair(i, OT_PUSH_FRONT));
    executeAndCheck(list, true);
}

TEST(basic_tests_case, push_test3) {
    OperationList<int> list;
    for (size_t i = 0; i < 100; ++i)
        list.push_back(std::make_pair(i, (i % 2 ? OT_PUSH_FRONT : OT_PUSH_BACK)));
    executeAndCheck(list, true);
}

TEST(basic_tests_case, full_test) {
    OperationList<int> list;
    for (size_t i = 0; i < 100; ++i)
        list.push_back(std::make_pair(i, OT_PUSH_FRONT));
    for (size_t i = 0; i < 2000; ++i)
        list.push_back(std::make_pair(i, randomOperation(1, 1, 1, 1)));
    executeAndCheck(list, true);
}

TEST(random_tests_case, random_test) {
    for (int i = 0; i < 10; ++i) {
        OperationList<int> list;
        for (size_t i = 0; i < 1000; ++i)
            list.push_back(std::make_pair(i, OT_PUSH_FRONT));
        for (size_t i = 0; i < 10000; ++i)
            list.push_back(std::make_pair(i, randomOperation(1, 1, 1, 1)));
        executeAndCheck(list, false);
    }
}

TEST(random_tests_case, growth_test) {
    for (int i = 0; i < 10; ++i) {
        OperationList<int> list;
        for (size_t i = 0; i < 1000; ++i)
            list.push_back(std::make_pair(i, OT_PUSH_FRONT));
        for (size_t i = 0; i < 10000; ++i)
            list.push_back(std::make_pair(i, randomOperation(3, 3, 1, 1)));
        executeAndCheck(list, false);
    }
}

TEST(random_tests_case, shrink_test) {
    for (int i = 0; i < 10; ++i) {
        OperationList<int> list;
        for (size_t i = 0; i < 10000; ++i)
            list.push_back(std::make_pair(i, OT_PUSH_FRONT));
        for (size_t i = 0; i < 10000; ++i)
            list.push_back(std::make_pair(i, randomOperation(1, 1, 10, 10)));
        executeAndCheck(list, false);
    }
}

TEST(iterators_tests_case, random_access_test) {
    OperationList<int> list;
    for (size_t i = 0; i < 10000; ++i)
        list.push_back(std::make_pair(i, OT_PUSH_FRONT));
    std::pair<Deque<int>, std::deque<int> > p = executeAndCheck(list, false);
    std::sort(p.first.begin(), p.first.end());
    std::sort(p.second.begin(), p.second.end());
    testForEquality(p.first, p.second);
}


TEST(iterators_tests_case, traits_test) {
    struct S {};
    typedef std::iterator_traits<Deque<S>::iterator> traits;
    ::testing::StaticAssertTypeEq<traits::difference_type, std::ptrdiff_t>();
    ::testing::StaticAssertTypeEq<traits::iterator_category, std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<traits::value_type, S>();
    ::testing::StaticAssertTypeEq<traits::reference, S&>();
    ::testing::StaticAssertTypeEq<traits::pointer, S*>();

    typedef std::iterator_traits<Deque<S>::const_iterator> const_traits;
    ::testing::StaticAssertTypeEq<const_traits::difference_type, std::ptrdiff_t>();
    ::testing::StaticAssertTypeEq<const_traits::iterator_category, std::random_access_iterator_tag>();
    ::testing::StaticAssertTypeEq<const_traits::value_type, S>();
    ::testing::StaticAssertTypeEq<const_traits::reference, const S&>();
    ::testing::StaticAssertTypeEq<const_traits::pointer, const S*>();
}

TEST(iterators_tests_case, operation_test) {
    OperationList<int> list;
    for (size_t i = 0; i < 100; ++i)
        list.push_back(std::make_pair(i, OT_PUSH_FRONT));
    Deque<int> d = executeAndCheck(list, false).first;
    ASSERT_EQ(d.begin() + d.size(), d.end());
    ASSERT_EQ(d.end() - d.size(), d.begin());
    ASSERT_EQ(d.size() + d.begin(), d.end());
    ASSERT_EQ(d.size(), d.end() - d.begin());
    ASSERT_EQ(d.begin()[0], d.front());
    ASSERT_EQ(d.begin()[d.size() - 1], d.back());
}

TEST (method_tests_case, method_test) {
    OperationList<int> list;
    ASSERT_TRUE(Deque<int>().empty());
    for (size_t i = 0; i < 100; ++i)
        list.push_back(std::make_pair(i, OT_PUSH_FRONT));
    std::pair<Deque<int>, std::deque<int> > p = executeAndCheck(list, false);
    const Deque<int> td = p.first;
    ASSERT_FALSE(td.empty());
    ASSERT_EQ(td.front(), p.first.front());
    ASSERT_EQ(td.front(), p.second.front());
    ASSERT_EQ(td.back(), p.first.back());
    ASSERT_EQ(td.back(), p.second.back());
    for (size_t i = 0; i < td.size(); ++i) {
        ASSERT_EQ(td[i], p.second[i]);
    }
}

long long elapsed() {
    static long long t;
    long long buf = t;
    t = clock();
    return buf - t;
}

TEST(time_tests, time_test) {
    std::vector<long long> times;
    size_t size = 100;
    for (size_t i = 0; i < 5; ++i, size *= 10) {
        OperationList<int> l = generateIntTest(size, 5, 5, 1, 1);
        elapsed();
        executeAndCheck(l, false);
        times.push_back(elapsed());
    }
    for (size_t i = 1; i < times.size(); ++i) {

        ASSERT_NEAR(10, static_cast<double>(times[i]) / times[i - 1], 1);
    }
}

TEST(swap_tests, swap_test) {
    Deque<int> d(100), f;
    for (int i = 0; i < 1000; ++i) {
        f = d;
        d = d;
    }
}

int main(int argc, char *argv[]) {
    srand(123);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}