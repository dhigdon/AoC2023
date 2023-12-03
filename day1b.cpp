// Advent of Code 2023 - Day 1 part 1: Trebuchet?!
// by Dan Higdon

#include "aoc.h"

std::string_view const names[]
{
   "zero",
   "one", "two", "three", "four", "five",
   "six", "seven", "eight", "nine",
};

// Returns index of name in names
int to_num( char const * const name )
{
   // There is certainly a better way to do this.... 
   for ( int i = 0; i < 10; ++i )
   {
      std::string const n( name, names[i].size() );

      // See if n is the prefix of name
      if ( names[i] == n )
         return i;
   }
   return -1;
}

int eval( std::vector<int> const & str )
{
   if ( str.empty() ) return 0;
   return str.front() * 10 + str.back();
}

int main( int argc, char **argv )
{
   std::string line;
   int sum = 0;
   std::ifstream file( argv[1] );
   while ( file >> line )
   {
      std::vector<int> result;
      for ( char const * p = line.c_str(); *p; ++p )
      {
         if ( isdigit( *p ) )
         {
            result.push_back( *p - '0' );
         }
         else if ( int n = to_num( p ); n >= 0 )
         {
            result.push_back( n );
         }
      }

      int val = eval( result );

      sum += val;
   }

   std::cout << sum << std::endl;

   return 0;
}

