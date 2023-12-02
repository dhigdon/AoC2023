// Advent of Code 2023, Day 2 - Cube Conundrum
// by Dan Higdon

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// Just makes things so much nicer....
#include "split.h"

// The game is played by taking a handful of colored cubes from a bag
struct Hand
{
   int red{}, green{}, blue{};

   // Part one - is this hand possible, given the limits?
   bool possible( Hand const & limit ) const
   {
      return red <= limit.red
         && green <= limit.green
         && blue <= limit.blue;
   }

   // Part two - increase this hand to allow for other's marbles
   void maxWith( Hand const & other )
   {
      if (other.red > red) red = other.red;
      if (other.green > green) green = other.green;
      if (other.blue > blue) blue = other.blue;
   }

   // Part two - power evaluation of the marbles in this hand
   int power() const { return red * green * blue; }
};

// Makes debugging easier
std::ostream & operator<< (std::ostream & out, Hand const & hand )
{
   return out << hand.red   << " red, "
              << hand.green << " green, "
              << hand.blue  << " blue; ";
}

// Parsing

// Update a hand with the color data in the given view
void parse_color( std::string_view view, Hand & hand )
{
   // Color format is "### NAME"
   char * next{};
   int const value = (int)::strtol( view.data(), &next, 10 );

   // Note that the number and name are separated by exactly 1 space
   char const color = view[ next - view.data() + 1 ];

   // And since the colors differ in their first letter...
   if ( color == 'r' ) hand.red   = value;
   if ( color == 'g' ) hand.green = value;
   if ( color == 'b' ) hand.blue  = value;
}

// Fill a hand with the specified marbles
Hand parse_hand( std::string_view src )
{
   Hand result;
   // each clause adds a new color to result
   for ( auto rec : split( src, ',' ) )
      parse_color( rec, result );
   return result;
}

// Collect all hands in the given game's specification
auto parse_game( std::string_view src )
{
   auto const hands = split( src, ';' );

   std::vector<Hand> result( hands.size() );
   std::transform( hands.begin(), hands.end(), result.begin(), parse_hand );
   return result;
}

// Soluions

int main( int argc, char *argv[])
{
   if ( argc != 2 )
   {
      std::cerr << "Missing filename" << std::endl;
      return 1;
   }

   std::ifstream in( argv[1], std::ifstream::in );
   std::string game, buf;
   int count, sum{0}, powers{0};
   char sep;

   while ( getline( in, buf ) )
   {
      std::istringstream ss( buf ); 

      // Skip over the "Game #:" part, but remember the count
      ss >> game >> count >> sep;

      // Parse this line's data part
      auto const hands = parse_game( std::string_view{ buf }.substr( ss.tellg() ) );

      // Part 1 - count all possible games
      constexpr Hand limit{ 12, 13, 14 };
      bool all = true;
      for ( Hand const & h : hands )
      {
         if ( !h.possible( limit ) )
            all = false;
      }
      if (all)
         sum += count;

      // Part 2 - compute minimum cubes that would have
      // made the game possible.
      // This is just the MAX of each field, so
      Hand hypothetical;
      for ( Hand const & h : hands )
         hypothetical.maxWith( h );
      powers += hypothetical.power();
   }

   std::cout << "Part 1: " << sum << "\nPart 2: " << powers << "\n";
   return 0;
}

