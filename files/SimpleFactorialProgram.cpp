#include <iostream>
// Tested here https://godbolt.org/z/aPGrdznbn
int main() {
  int num = 6;
  int res = 1, i = 2;
  while (i != num + 1) {
    res *= i;
    ++i;
  }
  std::cout << "Factorial of " << num << " is " << res << "\n";
  return 0;
}