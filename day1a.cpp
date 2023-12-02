// Advent of Code 2023 - Day 1 part 1: Trebuchet?!
// by Dan Higdon

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

int main( int argc, char **argv )
{
   int sum = 0;
   string str;
   auto file = ifstream( argv[1] );
   while (file)
   {
      file >> str;
      if ( !str.empty() )
      {
         string result;
         copy_if( str.begin(), str.end(), back_inserter( result ), isdigit );

         int value = (result.front() - '0') * 10 + (result.back() - '0');
         sum += value;
      }
   }
   cout << sum << endl;

   return 0;
}

