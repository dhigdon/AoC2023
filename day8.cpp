// Advent of Code 2023, Day 8 - Haunted Wasteland
// by Dan Higdon

#include "aoc.h"
#include <map>

using namespace std;

struct Node { string left, right; };
using MapSet = map<string, Node>;

// Parse the line into "KKK = {LLL, RRR)" and add it to the map
void add_node( string_view sv, MapSet &map )
{
   string_view key = sv.substr(0, sv.find(' '));
   string_view vals = sv.substr(sv.find('(') + 1);

   Node val
   {
      string{vals.substr(0, 3)},
      string{vals.substr(5, 3)}
   };

   map.insert(make_pair(string{key}, val));
}

bool terminate_part1( string const &pos )
{
   return pos == "ZZZ";
}

int64_t run1( string const & start,
              string_view input,
              MapSet const & maps,
              bool (*term)(string const &) )
{
   int64_t count = 0;

   string pos = start;
   while (!term(pos))
   {
      auto next = maps.find(pos);
      switch (input[count++ % input.size()])
      {
      case 'L': pos = next->second.left; break;
      case 'R': pos = next->second.right; break;
      }
   }

   return count;
}

bool is_start( string_view sv )
{
   return sv[sv.size()-1] == 'A';
}

vector<string> get_starts( MapSet const &map )
{
   vector<string> result;
   for (auto const & m : map)
   {
      if (is_start(m.first)) result.push_back(m.first);
   }
   return result;
}

bool terminate_part2(string const &pos)
{
   return pos[pos.size()-1] == 'Z';
}

int64_t run2( string_view input, MapSet const & maps )
{
   auto run = [&](int64_t total, string const & start) -> int64_t 
   {
      return lcm(total, run1(start, input, maps, terminate_part2));
   };

   auto const starts = get_starts(maps);
   return accumulate(begin(starts), end(starts), int64_t(1), run);
}

int main( int argc, char *argv[])
{
   if ( argc != 2 )
   {
      cout << "Usage: day8 <filename>\n";
      return 0;
   }

   ifstream in( argv[1], ifstream::in );
   string line;

   string instructions;
   getline(in, instructions);

   getline(in, line); // Skip blank line

   MapSet maps;
   while (getline( in, line )) add_node(line, maps);

   // Part 1
   cout
      << "Part 1: "
      << run1("AAA", instructions, maps, terminate_part1 )
      << endl
      << "Part 2: "
      << run2( instructions, maps )
      << endl;

   return 0;
}
