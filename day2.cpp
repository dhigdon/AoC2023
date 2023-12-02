// Advent of Code 2023, Day 2 - Cube Conundrum
// by Dan Higdon

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Hand
{
   int red{0}, green{0}, blue{0};

   void maxWith(Hand const & other)
   {
      if (other.red > red) red = other.red;
      if (other.green > green) green = other.green;
      if (other.blue > blue) blue = other.blue;
   }

   int power() const { return red * green * blue; }
};

// Makes debugging easier
ostream & operator<< (ostream & out, Hand const & hand )
{
   out << hand.red   << " red, "
       << hand.green << " green, "
       << hand.blue  << " blue; ";
   return out;
}

bool possible( Hand const & hand )
{
   constexpr Hand limit{ 12, 13, 14 };

   return hand.red <= limit.red
      && hand.green <= limit.green
      && hand.blue <= limit.blue;
}

char parse_color( istream & in, Hand & hand )
{
   int value;
   string color;

   in >> value >> color;
   if ( color[0] == 'r' ) hand.red = value;
   if ( color[0] == 'g' ) hand.green = value;
   if ( color[0] == 'b' ) hand.blue = value;

   return color[color.size()-1];
}

istream & parse_hand( istream & in, Hand & result )
{
   while ( parse_color( in, result ) == ',' )
      ;

   return in;
}

vector<Hand> parse( istream & in )
{
   vector<Hand> result;
   Hand h;

   while ( parse_hand( in, h ) )
      result.push_back( h );

   return result;
}

int main( int argc, char *argv[])
{
   if ( argc < 2 )
   {
      cerr << "Missing filename" << endl;
      return 1;
   }

   ifstream in( argv[1], ifstream::in );

   string game, buf;
   int count, sum=0, powers=0;
   char sep;
   while ( getline( in, buf ) )
   {
      istringstream ss( buf ); 

      // Skip over the "Game #:" part, but remember the count
      ss >> game >> count >> sep;

      // Parse this line's data
      vector<Hand> hands = parse( ss );

      // Part 1 - count all possible games
      bool all = true;
      for ( Hand const & h : hands )
      {
         if ( !possible(h) )
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

   cout << sum << ' ' << powers << endl;
   return 0;
}
