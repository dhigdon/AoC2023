// Advent of Code 2023, Day 5 - If You Give A Seed A Fertilizer
// by Dan Higdon

#include "aoc.h"

using namespace std;
using value_t = unsigned long;

// Utility to read a sequence of space delimited numbers
void read_numbers( vector<value_t> & result, string_view sv )
{
   result.clear();
   while ( !sv.empty() )
   {
      size_t idx = 0;
      result.push_back( svtoul( sv, &idx ) );
      sv = sv.substr( idx );
   }
}

struct Entry { value_t dest, source, count; };
Entry parse_entry( string_view sv )
{
   size_t idx;
   return Entry
   {
      svtoul( sv, &idx ),
      svtoul( sv = sv.substr( idx ), &idx ),
      svtoul( sv = sv.substr( idx ) )
   };
}

// For sorting, we sort by source index
inline bool operator<(Entry const & left, Entry const & right)
{
   return left.source < right.source;
}

optional<value_t> convert( value_t value, Entry const & entry )
{
   if ( value >= entry.source && value < (entry.source + entry.count) )
      return value + (entry.dest - entry.source);

   return optional<value_t>{};
}


using Map = vector<Entry>;
Map read_map( istream & in, string_view name )
{
   Map result;
   string line;

   // Check the section name for sanity
   getline( in, line );
   if ( name != line ) { cerr << "unexpected line " << line << endl; exit( 1 ); }

   // Read the data.
   while ( getline( in, line ) && !line.empty() )
   {
      Entry const e = parse_entry( line );

      // Insert this in sorted position (todo - merge?)
      auto const pos = lower_bound( begin( result ), end( result ), e );
      result.insert( pos, e );
   }
   return result;
}

value_t convert( value_t value, Map const & map )
{
   for ( auto const & m : map )
   {
      // map is sorted, so if the source is greater than our value, we're done searching
      if ( m.source > value )
         break;
      
      // If this map can convert the value, return that conversion
      if ( auto const cvt = convert( value, m ); cvt.has_value() )
         return *cvt;
   }
   return value;
}

value_t run( value_t value, vector<Map> const & pipeline )
{
   return reduce( begin( pipeline ), end( pipeline ), value,
                  []( value_t v, Map const & m ) -> value_t { return convert( v, m ); } );
}

int main( int argc, char * argv[] )
{
   using namespace std::literals;

   if ( argc != 2 )
   {
      cerr << "Usage: day5 <filename>\n";
      exit( 1 );
   }

   ifstream in( argv[ 1 ], ifstream::in );
   string line;
   // The input is in a predicable series of blank-line delimited chunks

   // seeds: N ...
   getline( in, line );

   // Part 1 interprets each number as a seed
   vector<value_t> seeds;
   read_numbers( seeds, string_view{ line }.substr( 6 ) );

   // Skip blank line
   getline( in, line );

   vector<Map> const pipeline =
   {
      read_map( in, "seed-to-soil map:"sv ),
      read_map( in, "soil-to-fertilizer map:"sv ),
      read_map( in, "fertilizer-to-water map:"sv ),
      read_map( in, "water-to-light map:"sv ),
      read_map( in, "light-to-temperature map:"sv ),
      read_map( in, "temperature-to-humidity map:"sv ),
      read_map( in, "humidity-to-location map:"sv )
   };

   // Part 1 - plant the seeds
   value_t result = LONG_MAX;
   for ( auto seed : seeds ) result = min( result, run( seed, pipeline ) );
   cout << "Part 1: " << result << endl << flush;

   // Part 2 - the seeds are pairs of (start, count) values
   //          Brute force our way through this bad boy
   result = LONG_MAX;
   for ( size_t seed = 0; seed < seeds.size(); )
   {
      value_t const start = seeds[ seed++ ];
      value_t const count = seeds[ seed++ ];
      value_t const last = start + count;

      for ( value_t seed = start; seed < last; ++seed )
      {
         result = min( result, run( seed, pipeline ) );
      }
   }
   cout << "Part 2: " << result << endl << flush;
}
