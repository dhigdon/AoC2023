// Advent of Code 2023, Day 14 - Parabolic Reflector Dish
// by Dan Higdon

#include "aoc.h"
#include <cassert>

using namespace std;
using namespace std::literals;
using value_t = int;

// A 2D-grid of element_t elements
template< typename element_t >
class CGrid
{
public:
   CGrid() = default;
   CGrid( int width, int height ) : mWidth( width ), mHeight( height )
   {
      mData.resize( width * height, ' ' );
   }

   element_t & at( int x, int y )         { return mData[ y * mWidth + x ]; }
   element_t   at( int x, int y ) const   { return mData[ y * mWidth + x ]; }
   bool        valid( int x, int y )      { return mData.size() > 0 && x >= 0 && x < mWidth && y >= 0 && y < mHeight; }

   int width() const { return mWidth; }
   int height() const { return mHeight; }

   bool operator==( CGrid<element_t> const & other )
   {
      return mData == other.mData;
   }

private:
   int mWidth{ 0 }, mHeight{ 0 };
   vector<element_t> mData;
};

using TCharGrid = CGrid<char>;

template<typename T>
ostream & operator<<( ostream & out, CGrid<T> const & grid )
{
   for ( int y = 0; y < grid.height(); ++y )
   {
      for ( int x = 0; x < grid.width(); ++x )
         out << grid.at( x, y );
      out << endl;
   }
   return out;
}


void slide_vertical( TCharGrid & grid, char chunk, char space )
{
   for ( int col = 0; col < grid.width(); ++col )
   {
      for ( int row = 1; row < grid.height(); /* inline */ )
      {
         if ( row > 0 && grid.at( col, row ) == chunk && grid.at( col, row - 1 ) == space )
         {
            swap( grid.at( col, row ), grid.at( col, row - 1 ) );
            --row;
         }
         else
         {
            ++row;
         }
      }
   }
}

void slide_horizontal( TCharGrid & grid, char chunk, char space )
{
   for ( int row = 0; row < grid.height(); ++row )
   {
      for ( int col = 1; col < grid.width(); )
      {
         if ( col > 0 && grid.at( col, row ) == chunk && grid.at( col - 1, row ) == space )
         {
            swap( grid.at( col, row ), grid.at( col - 1, row ) );
            --col;
         }
         else
         {
            ++col;
         }
      }
   }
}

void cycle(TCharGrid & grid)
{
   slide_vertical( grid, 'O', '.' );      // NORTH
   slide_horizontal( grid, 'O', '.' );    // WEST
   slide_vertical( grid, '.', 'O' );      // SOUTH
   slide_horizontal( grid, '.', 'O' );    // EAST
}


int64_t evaluate( TCharGrid const & grid )
{
   int64_t result{ 0 };
   for ( int row = 0; row < grid.height(); ++row )
   {
      for ( int col = 0; col < grid.width(); ++col )
      {
         if ( grid.at( col, row ) == 'O' )
         {
            result += grid.height() - row;
         }
      }
   }
   return result;
}


int main( int argc, char * argv[] )
{
   if ( argc != 2 )
   {
      std::cerr << "Usage: day14 <filename>\n";
      return 1;
   }

   TCharGrid map;

   std::ifstream in{ argv[ 1 ] };
   string line{};

   // Read the data
   for ( int row = 0; getline( in, line ); ++row )
   {
      int const width = (int)line.length();

      if ( !map.valid( 0, 0 ) ) map = TCharGrid( width, width );

      for ( int x = 0; x < width; ++x ) map.at( x, row ) = line[ x ];
   }

   TCharGrid part1{ map };
   slide_vertical( part1, 'O', '.' );
   cout << "Part 1: " << evaluate( part1 ) << endl;

   vector<TCharGrid> history;
   history.reserve( 100 );
   TCharGrid part2{ map };
   int64_t count{ 1000000000 };
   int64_t c;
   for ( c = 0; c < count; ++c )
   {
      cycle( part2 );

      // Hopefully the data loops. If so, figure out how wide the loop is 
      // and advance c by that amount;
      auto found = find( begin( history ), end( history ), part2 );
      if ( found != end( history ) )
      {
         size_t const offset = found - history.begin();
         size_t const period = history.end() - found;
         c += period * ((count - offset) / period) - period + 1;
         break;
      }

      history.push_back( part2 );
   }

   // Run out the remaining cycles
   for ( ; c < count; ++c )
      cycle( part2 );

   cout << "Part 2: " << evaluate( part2 ) << endl;;

   return 0;
}
