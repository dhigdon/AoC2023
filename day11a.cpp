// Advent of Code 2023, Day 11 - 
// by Dan Higdon

#include "aoc.h"
using namespace std;

struct SPos { int x, y; };
using TField = vector< string >;
using TGalaxyList = vector< SPos >;

int count_galaxies( string const & line )
{
   int result = 0;
   for (char c : line)
      if (c == '#') ++result;
   return result;
}

int dist(SPos a, SPos b)
{
   // cout << "dist " << a.x << ',' << a.y <<
   //   " and " << b.x << ',' << b.y << " = ";

   int result = abs(a.x - b.x) + abs(a.y - b.y);
   // cout << result << endl;

   return result;
}

TGalaxyList read_data(istream & in, TField & field)
{
   int galaxies{0}, width{0};
   string line;
   while (getline( in, line ))
   {
      galaxies += count_galaxies(line);
      field.push_back( line );

      // Will always be the same.
      width = line.size();

      // Duplicate blank lines
      if (line.find_first_not_of('.') == string::npos)
      {
         field.push_back( line );
      }
   }

   // Now, duplicate blank columns
   for (int i = 0; i < width; ++i)
   {
      bool blank=true;
      for (int j = 0; j < field.size(); ++j)
      {
         if (field[j][i] == '#')
         {
            blank = false;
            break;
         }
      }
      if (blank)
      {
         cout << "col " << i << endl;
         for (int j = 0; j < field.size(); ++j)
         {
            field[j].insert( i, 1, '.' );
         }
         ++i;  // Skip the newly added row
         ++width;
      }
   }

   TGalaxyList roster;
   roster.reserve(galaxies);
   for (int y = 0; y < field.size(); ++y)
   {
      auto const & row = field[y];
      for (int x = 0; x < width; ++x)
      {
         if (row[x] == '#')
            roster.push_back(SPos{x, y});
      }
   }

   return roster;
}

int part1( TGalaxyList const & galaxies )
{
   int result{0};
   for (int i = 0; i < galaxies.size()-1; ++i)
   {
      for (int j = i+1; j < galaxies.size(); ++j)
      {
         result += dist(galaxies[i], galaxies[j]);
      }
   }
   return result;
}

int main( int argc, char *argv[] )
{
   if (argc != 2)
   {
      cerr << "Usage: day3 <filename>" << endl;
      return 1;
   }

   ifstream in( argv[1], ifstream::in );
   TField space;
   TGalaxyList galaxies = read_data( in, space );

   cout << "Galaxies=" << galaxies.size() << endl;
   for (auto const & row : space)
      cout << row << endl;

   cout << "Part 1: " << part1(galaxies) << endl;
}
