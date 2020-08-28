#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y) out << (x) << endl << (y) << endl \

int main() {
    TestRunner tr;
    tr.RunTest([] {
        ostringstream output;
        PRINT_VALUES(output, 5, "red belt");
        ASSERT_EQUAL(output.str(), "5\nred belt\n");
    }, "PRINT_VALUES usage example");
    tr.RunTest([] {
        ostringstream output;
        int x = 1;
        PRINT_VALUES(output, ++x, "red belt");
        ASSERT_EQUAL(output.str(), "2\nred belt\n");
    }, "x++");
    tr.RunTest([] {
        ostringstream output;
        int x = 1;
        PRINT_VALUES(output, ++x, "red belt");
        ASSERT_EQUAL(output.str(), "2\nred belt\n");
    }, "x++");
    tr.RunTest([] {
        ostringstream output;
        int x = 1;
        if (true)
            PRINT_VALUES(output, ++x, "red belt");
        else
            PRINT_VALUES(output, ++x, "red belt");

        ASSERT_EQUAL(output.str(), "\nred belt\n");
    }, "x++");

}
