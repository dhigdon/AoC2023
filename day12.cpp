// Advent of Code 2023, Day 12 - Hot Springs
// by Dan Higdon

#include "../aoc.h"
#include <cassert>
#include <ranges>

using namespace std;
using namespace std::literals;
using value_t = int;

ostream & operator<<(ostream & out, vector<value_t> const & vs )
{
   out << "[ ";
   for ( auto v : vs ) cout << v << ' ';
   out << ']';
   return out;
}

bool is_spring( char ch ) { return ch == '#' || ch == '?'; }
bool is_space( char ch ) { return ch == '.' || ch == '?'; }

bool match( string_view sv, size_t run )
{
   // Can't match if there aren't enough characters
   if ( run > sv.size() ) return false;

   // See if the first 'run' characters of 'sv' match as springs, and that the next character matches as a space
   for ( size_t i = 0; i < run; ++i)
   {
      if ( !is_spring( sv[ i ] ) ) return false;
   }

   // Must end in a space
   return run == sv.size() || is_space( sv[ run ] );
}

optional<value_t> process( string_view sv, vector<int> const & runs, int start )
{
   // We're done - succeed with no more variations
   if ( start >= runs.size() ) return 1;

   size_t const run = runs[ start ];
   optional<value_t> count{};

   // We still have runs, but are out of data - fail
   if ( sv.empty() || sv.size() < run ) return count;

   while ( run <= sv.length() )
   {
      if ( match( sv, run ) )
      {
         // See if we can match the rest
         auto const rest = sv.substr( min( sv.length(), run + 1 ) );
         auto const rest_variants = process( rest, runs, start + 1 );

         if ( rest_variants.has_value() )
         {
            count = count.value_or( 0 ) + *rest_variants;
         }
      }

      // If the prefix is "hard", we can't skip it, so no more searching
      if (sv.front() == '#' )
      {
         break;
      }

      sv.remove_prefix( 1 );
   }

   return count;
}


int main( int argc, char * argv[] )
{
   if ( argc != 2 )
   {
      std::cerr << "Usage: day12 <filename>\n";
      return 1;
   }

   value_t total{};
   vector<value_t> counts{};

   std::ifstream in{ argv[ 1 ] };
   string line{};
   while ( getline( in, line ) )
   {
      string_view parse{ line };
      auto caret = parse.find( ' ' );
      auto source = parse.substr( 0, caret );
      parse.remove_prefix( caret );
      counts.clear();

      caret = 0;
      while ( auto v = svtol( parse, &caret ) )
      {
         counts.push_back( v );
         if ( caret == parse.size() )
            break;
         parse.remove_prefix( caret + 1 );
      }

      auto result = process( source, counts, 0 );
      cout << source << ' ' << counts << ' ' << result.value_or( -1 ) << endl;

      total += result.value_or( 0 );
   }

   cout << "Part 1 " << total << endl;

   //cout << "Part 1 test: " << process( "???.###", { 1, 1, 3 }, 0 ).value_or( -1 ) << endl;
   //cout << "Part 1 test: " << process( ".??..??...?##.", { 1, 1, 3 }, 0 ).value_or( -1 ) << endl;
   //cout << "Part 1 test: " << process( "?#?#?#?#?#?#?#?", { 1, 3, 1, 6 }, 0 ).value_or( -1 ) << endl;
   //cout << "Part 1 test: " << process( "????.#...#...", { 4, 1, 1 }, 0 ).value_or( -1 ) << endl;
   //cout << "Part 1 test: " << process( "????.######..#####.", { 1, 6, 5 }, 0 ).value_or( -1 ) << endl;
   //cout << "Part 1 test: " << process( "?###????????", { 3, 2, 1 }, 0 ).value_or( -1 ) << endl;

   return 0;
}
