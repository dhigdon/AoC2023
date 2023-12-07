// Advent of Code 2023, Day 6 - Wait For It
// by Dan Higdon

#include "AoC.h"

using data_t = std::vector<int>;
using value_t = __int64;

// test data
data_t const time_test{ 7, 15, 30 };
data_t const dist_test{ 9, 40, 200 };
value_t const time_test_part2{ 71530 };
value_t const dist_test_part2{ 940200 };

// Problem data
data_t const time_data{ 34, 90, 89, 86 };
data_t const dist_data{ 204, 1713, 1210, 1780 };
value_t const time_data_part2{ 34908986 };
value_t const dist_data_part2{ 204171312101780 };


// Returns count of solutions that beat the distance
value_t solve_x( value_t time, value_t distance )
{
   value_t count = 0;
   for ( value_t i = 1; i < (time - 1); ++i )
   {
      count += (time - i) * i > distance;
   }
   return count;
}

value_t solve_part1( data_t const & times, data_t const & distances )
{
   value_t result = 1;
   for ( int race = 0; race < times.size(); ++race )
   {
      result *= solve_x( times[ race ], distances[ race ] );
   }
   return result;
}


int main()
{
   std::cout << "Part 1(test): " << std::endl << solve_part1( time_test, dist_test ) << std::endl;
   std::cout << "Part 1: " << solve_part1( time_data, dist_data ) << std::endl;

   std::cout << "Part 2(test): " << solve_x( time_test_part2, dist_test_part2 ) << std::endl;
   std::cout << "Part 2: " << solve_x( time_data_part2, dist_data_part2 ) << std::endl;

   return 0;
}
