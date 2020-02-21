#pragma once

#include <assert.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

template < class T >
std::ostream &operator << (std::ostream &os, const std::vector < T > &data) {
    os << "[";
    bool first = true;

    for (const auto &item : data) {
        if (!first) {
            os << ",";
        } else {
            first = false;
        }
        os << item;
    }
    os << "]";

    return os;
}

template < class T >
std::ostream &operator << (std::ostream &os, const std::set < T > &data) {
    os << "(";
    bool first = true;

    for (const auto &item : data) {
        if (!first) {
            os << ",";
        } else {
            first = false;
        }
        os << item;
    }
    os << ")";

    return os;
}

template < class T , class V >
std::ostream &operator << (std::ostream &os, const std::map < T , V > &data) {
    os << "{";
    bool first = true;

    for (const auto &item : data) {
        if (!first) {
            os << ",";
        } else {
            first = false;
        }
        os << item.first << ":" << item.second;
    }
    os << "}";

    return os;
}

template < class T, class U >
void AssertEqual(const T& t, const U& u, const std::string& hint = {}) {
    if (t != u) {
        std::ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw std::runtime_error(os.str());
    }
}

void Assert(bool b, const std::string& hint);
//    {
//    AssertEqual(b, true, hint);
//}

class TestRunner {
public:
    template < class TestFunc >
    void RunTest(TestFunc func, const std::string& test_name) {
        try {
            func();
            std::cerr << test_name << " OK" << std::endl;
        } catch (std::exception& e) {
            ++fail_count;
            std::cerr << test_name << " fail: " << e.what() << std::endl;
        } catch (...) {
            ++fail_count;
            std::cerr << "Unknown exception caught" << std::endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};