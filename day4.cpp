// Advent of Code 2023, Day 4 - Scratchcards
// by Dan Higdon

#include "aoc.h"
using namespace std;

// Utility to read a sequence of space delimited numbers
vector<int> read_numbers( string_view sv )
{
   vector<int> result;

   while (!sv.empty())
   {
      size_t idx = 0;
      result.push_back(svtol(sv, &idx));
      sv = sv.substr(idx);
   }

   return result;
}

int main(int argc, char *argv[])
{
   if (argc != 2)
   {
      cerr << "Usage: day4 <filename>" << endl;
      return 1;
   }

   // Vectors we'll need.
   // NB: allocate here so they won't be reallocated every time,
   //     only when necessary.
   vector<int> winning_numbers, winners, numbers;
   vector<long> cards;
   long score = 0;

   // Read the card data
   ifstream in{ argv[1], ifstream::in };
   string line;
   while (getline(in, line))
   {
      string_view const view( line );
      int const colon = view.find(':');
      int const sep   = view.find('|');

      // Split the view and find the numbers within
      winners = read_numbers(view.substr(colon + 1, sep - colon - 2));
      numbers = read_numbers(view.substr(sep + 1));

      // Tally winning numbers
      int tally = 0;
      for (int n : numbers)
         tally += count(begin(winners), end(winners), n);

      // Score the card (part 1)
      if (tally > 0)
         score += 1 << (tally - 1);

      // Collect card win numbers (part 2)
      cards.push_back(1);
      winning_numbers.push_back(tally);
   }

   cout << "Part1: " << score << endl;

   // Part 2's score can be computed now
   score = 0;
   for (int i = 0; i < cards.size(); ++i)
   {
      int const c = cards[i];
      score += c;
      for (int j = 0; j < winning_numbers[i]; ++j)
         cards[i + j + 1] += c;
   }

   cout << "Part2: " << score << endl;

   return 0;
}

