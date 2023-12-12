// Advent of Code 2023, Day 9 - Mirage Maintenance
// by Dan Higdon

#include "aoc.h"

using namespace std;
using value_t = int64_t;

inline bool every( vector<value_t> const & values, value_t target )
{
   for ( value_t const v : values )
      if (v != target) return false;
   return true;
}

value_t extend1( vector<value_t> const & values )
{
   // Recursion base-case
   if (every( values, 0 )) return 0;

   // Recursive case:
   vector<value_t> diffs;
   diffs.reserve(values.size() - 1);
   for (int i = 0; i < values.size() - 1; ++i)
      diffs.push_back(values[i+1] - values[i]);
   
   return values.back() + extend1(diffs);
}

value_t extend2( vector<value_t> const & values )
{
   // Recursion base-case
   if (every( values, 0 )) return 0;

   // Recursive case:
   vector<value_t> diffs;
   diffs.reserve(values.size() - 1);
   for (int i = 0; i < values.size() - 1; ++i)
      diffs.push_back(values[i+1] - values[i]);
   
   return values.front() - extend2(diffs);
}

int main(int argc, char *argv[])
{
   if (argc != 2)
   {
      cerr << "Usage: day9 <filename>\n";
      return 1;
   }

   ifstream in(argv[1], ifstream::in);
   string line;
   vector<value_t> values;
   values.reserve(32);
   value_t result_1{0}, result_2{0};
   while (getline(in, line))
   {
      values.clear();
      for ( auto const & num : split(line, ' '))
         values.push_back(svtol(num));
      result_1 += extend1(values);
      result_2 += extend2(values);
   }
   cout << "Part 1: " << result_1 << endl;
   cout << "Part 2: " << result_2 << endl;
}

