// Advent of Code 2023, Day 7a - Camel Cards, Part 2
// by Dan Higdon

#include "aoc.h"

#include <cassert>

using namespace std::literals;

// categories in numeric order 
enum HandType
{
   HighCard,
   OnePair,
   TwoPair,
   ThreeOfAKind,
   FullHouse,
   FourOfAKind,
   FiveOfAKind
};

constexpr std::string_view type_names[]
{
   "HighCard"sv,
   "OnePair"sv,
   "TwoPair"sv,
   "ThreeOfAKind"sv,
   "FullHouse"sv,
   "FourOfAKind"sv,
   "FiveOfAKind"sv
};

std::string_view const lookup{ "0J23456789TQKA"sv };

inline int card_strength(char c)
{
   return (int)lookup.find( c );
}

inline char card_char( int strength )
{
   return lookup[ strength ];
}

bool compare_cards( char l, char r )
{
   return card_strength( l ) < card_strength( r );
}

// Returns the length of a run of 'ch'.
// For example, find_run("aaabc") == 3
int run_length(std::string_view sv)
{
   if (sv.empty()) return 0;

   char const ch = sv[0];
   int i = 1;
   while (i < sv.length() && sv[i] == ch)
      ++i;

   return i;
}

struct Hand
{
   std::vector<int> cards{};  // reserved_vector<int, 5>, ideally
   HandType type{ HighCard };
   int bid{ 0 };
};

std::ostream & operator<<( std::ostream & out, Hand const & hand )
{
   out << "Hand ";
   for ( auto c : hand.cards )
      out << card_char( c );
   out << " (" << type_names[ hand.type ];
   out << ") bid " << hand.bid;
   return out;
}

HandType evaluate( std::string_view input )
{
   // sv is already sorted. Split it into runs, ignoring length==1 runs
   // Note that with only 5 cards possible, and rejecting 1-card runs,
   // we can only have one or two runs, so we'll store them in a pair.

   std::string_view sv = input;
   std::pair<int, int> runs{ 0,0 };

   // Check for wildcards, and remove them
   int wild = 0;
   if ( sv[ 0 ] == 'J' )
   {
      wild = run_length( sv );
      sv.remove_prefix( wild );
   }

   // Find the first run > 1 (if any)
   do
   {
      runs.first = run_length( sv );
      sv.remove_prefix( runs.first );
   } while ( runs.first == 1 );

   // Find the second run > 1 (if any)
   do
   {
      runs.second = run_length( sv );
      sv.remove_prefix( runs.second );
   } while ( runs.second == 1 );

   // Make sure the runs are ordered
   if ( runs.second > runs.first )
   {
      std::swap( runs.first, runs.second );
   }

   // Accommodate the wildcard if we found some
   // Handle any special cases
   if ( wild > 3 )
      return FiveOfAKind;

   // Otherwise it needs to combine with the most numerous card
   // BUT if we have no first, pretend we had one
   if ( runs.first == 0 ) runs.first = 1;
   runs.first += wild;

   // Now, categorize the hands by their run lengths
   if ( runs == std::make_pair( 5, 0 ) ) return FiveOfAKind;
   if ( runs == std::make_pair( 4, 0 ) ) return FourOfAKind;
   if ( runs == std::make_pair( 3, 2 ) ) return FullHouse;
   if ( runs == std::make_pair( 3, 0 ) ) return ThreeOfAKind;
   if ( runs == std::make_pair( 2, 2 ) ) return TwoPair;
   if ( runs == std::make_pair( 2, 0 ) ) return OnePair;

   // Otherwise, it's just HighCard
   return HighCard;
}

Hand parse_hand( std::string_view sv )
{
   Hand result;

   size_t const space = sv.find( ' ' );
   std::string_view cardset = sv.substr( 0, space );
   result.bid = svtol( sv.substr( space + 1 ) );

   result.cards.reserve( space );
   for ( char c : cardset )
      result.cards.push_back( card_strength( c ) );

   // Finally, decide what hand this is
   // Note that evaluate() expects a sorted set of cards, which we generate locally here
   char sorted[ 5 ];
   assert( sizeof( sorted ) >= space * sizeof( sorted[ 0 ] ) );
   std::copy( std::begin( cardset ), std::end( cardset ), sorted );
   std::sort( sorted, sorted + space, compare_cards );
   result.type = evaluate( std::string_view( sorted, space ) );

   return result;
}

// Comparison for hands is more complex
bool operator < ( Hand const & l, Hand const & r )
{
   if ( l.type < r.type ) return true;
   if ( r.type < l.type ) return false;

   for ( int i = 0; i < l.cards.size(); ++i )
   {
      int const lc = l.cards[ i ];
      int const rc = r.cards[ i ];
      if ( lc < rc ) return true;
      if ( rc < lc ) return false;
   }
   return false;
}

int main( int argc, char * argv[] )
{
   if ( argc != 2 )
   {
      std::cerr << "Usage: day7 <filename>\n";
      return 1;
   }

   // Read the hands
   std::vector<Hand> hands;
   {
      std::ifstream in{ argv[ 1 ] };
      std::string line;

      while ( std::getline( in, line ) && !line.empty() )
      {
         hands.push_back( parse_hand( line ) );
      }
   }

   // Order them by strength 
   std::sort( hands.begin(), hands.end(), std::less() );

   long result = 0, rank = 1;
   for ( auto const & h : hands )
   {
      result += h.bid * rank;
      ++rank;
   }
   std::cout << "Part 2: " << result << std::endl;

   return 0;
}
