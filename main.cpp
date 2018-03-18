#include <iostream>
#include "include/StringVariable.h"

using namespace std;

int main() {
    auto message = "Hello ${world}() ${world}|";
    auto result = strvar::transform(message,
                                    strvar::StringValue{"world", "YYY"}
                  );
    cout << "Result: '" << result << "'" << endl;
    return 0;
}