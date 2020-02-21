#include "test_runner.h"

void Assert(bool b, const std::string& hint) {
    AssertEqual(b, true, hint);
}