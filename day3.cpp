// Advent of Code, Day 3 - Gear Ratios
//

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <cctype>

using namespace std;

// Keep a 2D map 
vector< string > sMap;

// The data is 140 characters square, so for simplicity,
// we will allocate the symbol zone map as a static 2D buffer
// and let the compiler do the math for us.
// NB: sSymbolZone[ y ][ x ]
constexpr int sMapSize = 140;
bool sSymbolZone[sMapSize][sMapSize]{};
char const sDigits[] = "0123456789";

bool is_symbol( char ch )
{
   return ch != '.' && !isdigit( ch );
}

bool is_partnum( int y, int start, int end )
{
   bool const * const row = sSymbolZone[ y ];
   for ( int i = start; i < end; ++i )
      if ( row[i] ) return true;
   return false;
}

void dump_symbol_zone( int size )
{
   for ( int y = 0; y < size; ++y )
   {
      for ( int x = 0; x < size; ++x )
      {
         if ( sSymbolZone[y][x] )
            cout << '*';
         else
            cout << '.';
      }
      cout << endl;
   }
}

bool check_symbol( int x, int y )
{
   if ( x >= 0 && x < sMapSize && y >= 0 && y < sMapSize )
      return sSymbolZone[y][x];
   return false;
}

void fill_symbol( int x, int y )
{
   if ( x >= 0 && x < sMapSize && y >= 0 && y < sMapSize )
      sSymbolZone[y][x] = true;
}

void fill_symbol_row( int x, int y )
{
   fill_symbol( x - 1, y );
   fill_symbol( x, y );
   fill_symbol( x + 1, y );
}

void fill_symbol_zone( int x, int y )
{
   fill_symbol_row( x, y - 1 );
   fill_symbol_row( x, y );
   fill_symbol_row( x, y + 1 );
}


int main( int argc, char *argv[] )
{
   if (argc != 2)
   {
      cerr << "Usage: day3 <filename>" << endl;
      return 1;
   }

   ifstream in( argv[1], ifstream::in );
   string line;
   int line_number{0};

   while ( getline( in, line ) )
   {
      sMap.reserve( line.size() );
      sMap.push_back( line );

      // Mark the symbol map
      for ( int i = 0; i < line.size(); ++i )
      {
         if (is_symbol(line[i])) fill_symbol_zone( i, line_number );
      }

      ++line_number;
   }

   // Part 1 - scan over the sSmap and sum any numbers that have
   //          symbols in the map for their position
   long sum{ 0 };
   for ( int y = 0; y < sMap.size(); ++y )
   {
      string_view const line = sMap[ y ];

      // Scan for digits
      string::size_type start = 0;
      while ( start < line.size() )
      {
         start = line.find_first_of( sDigits, start );
         if ( start != string::npos )
         {
            auto rest = line.substr( start );
            auto end = rest.find_first_not_of( sDigits );
            auto num = rest.substr( 0, end );

            // Check this span's validity
            end = start + num.size();
            if ( is_partnum( y, start, end ) )
            {
               int value = stoi( string{ num } );
               sum += value;
            }

            start = end;
         }
      }
   }
   cout << "Part 1: " << sum << endl;

   // Part 2 - find the gears

   return 0;
}
