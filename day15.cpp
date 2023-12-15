// Advent of Code 2023, Day 15 - Lens Library
// by Dan Higdon

#include "aoc.h"

using namespace std;
using namespace std::literals;
using value_t = int;

value_t HASH( string_view sv )
{
   return accumulate( begin( sv ), end( sv ), value_t{},
                      []( value_t r, char c ) { return ((r + c) * 17) & 0xff; } );
}

struct SEntry
{
   string_view mName{}; int mCount{};
};

// Compare on names only
inline bool operator== (SEntry const & left, SEntry const & right )
{
   return left.mName == right.mName;
}

int main( int argc, char * argv[] )
{
   if ( argc != 2 )
   {
      std::cerr << "Usage: day14 <filename>\n";
      return 1;
   }

   std::ifstream in{ argv[ 1 ] };
   string line{};

   getline( in, line );
   //line = "rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7";

   auto const codes = split( line, ',' );

   // Part 1 - hash the collection to make sure our HASH works correctly.
   value_t result = accumulate( begin( codes ), end( codes ), value_t{},
                                []( value_t x, string_view sv ) { return x + HASH( sv ); } );
   cout << "Part 1 - " << result << endl;

   // Part 2 - the real problem
   vector< SEntry > hashmap[ 256 ];
   for ( auto c : codes )
   {
      size_t pos = c.find_first_of( "=-" );
      string_view op = c.substr( 0, pos );
      int val = svtol( c.substr( pos + 1 ) );

      // Find the box
      vector< SEntry > & e = hashmap[ HASH( op ) ];
      // Check for this lens
      auto entry = find( begin( e ), end( e ), SEntry{ op } );
      
      // Perform the operation
      if ( c[ pos ] == '=' )
      {
         if ( entry != end( e ) ) entry->mCount = val;
         else e.push_back( SEntry{ op, val } );
      }
      else
      {
         if ( entry != end( e ) ) e.erase( entry );
      }
   }

   // Compute the power of the map
   value_t power{};
   for ( int nBox = 0; nBox < 256; ++nBox )
   {
      auto const & box = hashmap[ nBox ];
      for ( int i = 0; i < box.size(); ++i )
      {
         power += (nBox + 1) * (i + 1) * box[ i ].mCount;
      }
   }

   cout << "Part 2 - " << power << endl;

   return 0;
}
