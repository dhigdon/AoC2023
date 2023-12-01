// Advent of Code 2023 - Day 1 part 1: Trebuchet?!
// by Dan Higdon

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

string const names[]
{
   "zero", "one", "two", "three", "four",
   "five", "six", "seven", "eight", "nine",
};

// Returns index of name in names
int to_num( char const * const name )
{
   // There is certainly a better way to do this.... 
   for ( int i = 0; i < 10; ++i )
   {
      string const n( name, names[i].size() );

      // See if n is the prefix of name
      if ( names[i] == n )
         return i;
   }
   return -1;
}

int eval( string const & str )
{
   if ( str.empty() ) return 0;
   return (str.front() - '0') * 10 + (str.back() - '0');
}

int main( int argc, char **argv )
{
   int sum = 0;
   auto file = ifstream( argv[1] );
   while ( file )
   {
      string line{}, result{};
      file >> line;

      for ( char const * p = line.c_str(); *p; ++p )
      {
         if ( isdigit( *p ) )
         {
            result.push_back( *p );
         }
         else if ( int n = to_num( p ); n >= 0 )
         {
            result.push_back( n + '0' );
         }
      }

      sum += eval( result );
   }

   cout << sum << endl;

   return 0;
}

