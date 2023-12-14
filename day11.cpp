// Advent of Code 2023, Day 11 - Cosmic Expansion
// by Dan Higdon

#include "aoc.h"
using namespace std;
using value_t = int64_t;

struct SPos
{
   value_t x{0}, y{0};
};

struct SGalaxies
{
   value_t width{0}, height{0};
   vector<SPos> positions;
};

SGalaxies read_data(istream & in)
{
   SGalaxies galaxies;
   string line;
   while (getline( in, line ))
   {
      for (value_t x = 0; x < line.size(); ++x)
      {
         if (line[x] == '#')
            galaxies.positions.push_back( SPos{x, galaxies.height} );
      }
      galaxies.width = line.size();
      ++galaxies.height;
   }
   return galaxies;
}

void expand(SGalaxies & galaxies, value_t expand)
{
   // Vertical
   for (value_t i = 0; i < galaxies.height; ++i)
   {
      // Check this row for blanks
      bool blank = true;
      for ( auto const & g : galaxies.positions )
         if (g.y == i) { blank = false; break; }
      if (blank)
      {
         for (auto & g : galaxies.positions)
            if (g.y > i) g.y += expand-1;
         i += expand-1;
         galaxies.height += expand-1;
      }
   }
   // Horizontal
   for (value_t i = 0; i < galaxies.width; ++i)
   {
      bool blank = true;
      for ( auto const & g : galaxies.positions)
         if (g.x == i) { blank = false; break; }
      if (blank)
      {
         for (auto & g : galaxies.positions)
            if (g.x > i) g.x += expand-1;
         i += expand-1;
         galaxies.width += expand-1;
      }
   }
}

inline value_t dist(SPos a, SPos b)
{
   return abs(a.x - b.x) + abs(a.y - b.y);
}

value_t calculate( SGalaxies const & galaxies )
{
   value_t result{0};
   vector<SPos> const & pos = galaxies.positions;
   for (int i = 0; i < pos.size()-1; ++i)
   {
      for (int j = i+1; j < pos.size(); ++j)
      {
         result += dist(pos[i], pos[j]);
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
   auto galaxies = read_data( in );

   auto g = galaxies;
   expand(g, 2);
   cout << "Part 1: " << calculate(g) << endl;

   g = galaxies;
   expand(g, 1000000 );
   cout << "Part 2: " << calculate(g) << endl;
}

