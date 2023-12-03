// Advent of Code 2023 - Day 1 part 1: Trebuchet?!
// by Dan Higdon

#include "aoc.h"

int main( int argc, char **argv )
{
   int sum = 0;
   std::string str;
   std::ifstream file( argv[1] );
   while (file >> str)
   {
      if ( !str.empty() )
      {
         std::string result;
         std::copy_if( str.begin(), str.end(), back_inserter( result ), std::isdigit );

         int value = (result.front() - '0') * 10 + (result.back() - '0');
         std::cout << "value " << value << std::endl;
         sum += value;
      }
   }
   std::cout << sum << std::endl;

   return 0;
}

