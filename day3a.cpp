// Advent of Code, Day 3 - Gear Ratios
// Reworked from original to make part 2 possible.
// by Dan Higdon

#include "aoc.h"

using namespace std;

// --------------------------------------------------------------------

constexpr char sDigits[] = "0123456789";

// --------------------------------------------------------------------
// Symbols live in a location
struct SSymbol
{
   SSymbol( int x, int y, char sym ) : mX(x), mY(y), mSymbol(sym){}

   bool overlaps( int x, int y ) const
   {
      return abs(x - mX) < 2 && abs(y - mY) < 2;
   }

   int mX, mY;
   char mSymbol;
};

ostream & operator<<( ostream & out, SSymbol const & sym )
{
   out << "SYM: '" << sym.mSymbol << "' (" << sym.mX << ", " << sym.mY << ")";
   return out;
}

// --------------------------------------------------------------------

struct SNumber
{
   SNumber( int row, int start, int end, int value )
      : mRow( row ), mStart( start ), mEnd( end ), mValue( value )
   {}

   bool overlaps( SSymbol const & sym ) const
   { 
      for (int i = mStart; i < mEnd; ++i)
         if (sym.overlaps(i, mRow))
            return true;
      return false;
   }

   int mRow, mStart, mEnd;
   int mValue{0};
};

ostream & operator<<( ostream & out, SNumber const & num )
{
   out << "NUM: " << num.mValue
      << " row=" << num.mRow
      << " span=" << num.mStart << '-' << num.mEnd;
   return out;
}

// --------------------------------------------------------------------

bool is_symbol( char ch )
{
   return ch != '.' && !isdigit( ch );
}

// --------------------------------------------------------------------

int main( int argc, char *argv[] )
{
   if (argc != 2)
   {
      cerr << "Usage: day3 <filename>" << endl;
      return 1;
   }

   vector< SSymbol > symbols;
   vector< SNumber > numbers;

   ifstream in( argv[1], ifstream::in );
   string line;
   int line_number{0};

   while ( getline( in, line ) )
   {
      // Mark the symbol map
      for ( int i = 0; i < line.size(); )
      {
         char const ch = line[i];
         if ( is_symbol( ch ) )
         {
            symbols.emplace_back(i, line_number, ch );
            ++i;
         }
         else if ( isdigit( ch ) )
         {
            int end = line.find_first_not_of( sDigits, i );
            if ( end == string::npos ) end = line.size();

            int value = stoi( line.substr( i, end - i ) ); 
            numbers.emplace_back( line_number, i, end, value );
            i = end;
         }
         else
         {
            ++i;
         }
      }

      ++line_number;
   }

   // Part 1 - scan over the sSmap and sum any numbers that have
   //          symbols in the map for their position
   long sum{ 0 };
   for (auto const & n : numbers)
   {
      for (auto const & s : symbols)
      {
         if (n.overlaps( s ))
         {
            sum += n.mValue;
            break;
         }
      }
   }
   cout << "Part 1: " << sum << endl;

   // Part 2 - find the gears
   // Gears need two '*' symbols near them
   sum = 0;
   for (auto const & s : symbols)
   {
      if (s.mSymbol != '*') continue;

      int count{0}, factor{1};
      for (auto const & n : numbers)
      {
         if (n.overlaps( s ))
         {
            ++count;
            factor *= n.mValue;
         }
      }
      if (count == 2) sum += factor;
   }
   cout << "Part 2: " << sum << endl;

   return 0;
}
