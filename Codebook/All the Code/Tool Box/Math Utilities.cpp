/*
Base Conversion
- Given an integer in base a as a vector d of digits (where d[0] is the least
  significant digit), convert_base(d, a, b) returns a vector of the integer's
  digits when converted base b (again with index 0 storing the least significant
  digit). The actual value of the entire integer to be converted must be able to
  fit within an unsigned 64-bit integer for intermediate storage.
- convert_digits(x, b) returns the digits of the unsigned integer x in base b,
  where index 0 of the result stores the least significant digit.
- to_roman(x) returns the Roman numeral representation of the unsigned integer x
  as a C++ string.
*/

std::vector<int> convert_base(const std::vector<int> &d, int a, int b) {
  unsigned long long x = 0, power = 1;
  for (int i = 0; i < (int)d.size(); i++) {
    x += d[i]*power;
    power *= a;
  }
  int n = ceil(log(x + 1)/log(b));
  std::vector<int> res;
  for (int i = 0; i < n; i++) {
    res.push_back(x % b);
    x /= b;
  }
  return res;
}

std::vector<int> convert_base(unsigned int x, int b = 10) {
  std::vector<int> res;
  while (x != 0) {
    res.push_back(x % b);
    x /= b;
  }
  return res;
}

std::string to_roman(unsigned int x) {
  static const std::string h[] =
      {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
  static const std::string t[] =
      {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
  static const std::string o[] =
      {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
  std::string prefix(x / 1000, 'M');
  x %= 1000;
  return prefix + h[x/100] + t[x/10 % 10] + o[x % 10];
}
