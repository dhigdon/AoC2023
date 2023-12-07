// Advent of Code 2023, Day 7 - Camel Cards
// by Dan Higdon

#include "aoc.h"

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

constexpr std::string_view lookup{ "0123456789TJQKA"sv };

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
   std::vector<int> cards;
   HandType type;
   int bid;
};

std::ostream & operator<<(std::ostream & out, Hand const & hand)
{
   out << "Hand ";
   for (auto c : hand.cards)
      out << lookup[c];
   out << " (" << type_names[hand.type]; 
   out << ") bid " << hand.bid;
   return out;
}

HandType evaluate(std::string_view sv)
{
   // Categorize
   switch (run_length(sv))
   {
   case 5: return FiveOfAKind;
   case 4: return FourOfAKind;
   case 3:
      if (sv[3] == sv[4]) return FullHouse;
      return ThreeOfAKind;

   case 2:
      if (sv[2] == sv[3] && sv[3] == sv[4]) return FullHouse;
      if (sv[2] == sv[3]) return TwoPair;
      if (sv[3] == sv[4]) return TwoPair;
      return OnePair;
 
   case 1:
      // First character is unique.
      // Try again, but with 4 card hand
      sv.remove_prefix(1);
      switch (run_length(sv))
      {
      case 4:
         return FourOfAKind;

      case 3:
         return ThreeOfAKind;

      case 2:
         if (sv[2] == sv[3]) return TwoPair;
         return OnePair;

      case 1:
         // Try again with 3 card hand
         sv.remove_prefix(1);
         switch (run_length(sv))
         {
         case 3: return ThreeOfAKind;
         case 2: return OnePair;
         case 1:
            // Try again with 2 card hand
            if (sv[1] == sv[2]) return OnePair;
            break;
         }
         return HighCard;
      }
   }

   std::cout << "oops\n";
   return HighCard;
}

Hand parse_hand( std::string_view sv )
{
   Hand result;

   auto const parts = split(sv, ' ');
   result.cards.reserve(parts[0].size());
   for (char c : parts[0])
      result.cards.push_back(lookup.find(c));
   result.bid = svtol(parts[1]);

   // Finaly, decide what hand this is
   // Note that evaluate expectes a sorted list,
   // which we generate locally here.
   std::string sorted{ parts[0] };
   std::sort(begin(sorted), end(sorted));
   result.type = evaluate(sorted);

   return result;
}

// Comparison for hands is more complex
bool operator < (Hand const &l, Hand const &r)
{
   if (l.type < r.type) return true;
   if (r.type < l.type) return false;

   for (int i = 0; i < l.cards.size(); ++i)
   {
      int const lc = l.cards[i];
      int const rc = r.cards[i];
      if (lc < rc) return true;
      if (rc < lc) return false;
   }
   return false;
}

int main( int argc, char *argv[])
{
   if (argc != 2)
   {
      std::cerr << "Usage: day7 <filename>\n";
      return 1;
   }

   std::vector<Hand> hands;
   {
      std::ifstream in{argv[1]};
      std::string line;

      while (std::getline(in, line) && !line.empty())
      {
         hands.push_back(parse_hand(line));
      }
   }

   std::sort(hands.begin(), hands.end(), std::less());

   // Now, the hands are in rank order
   long result = 0, rank = 1;
   for (auto const & h : hands)
   {
      result += h.bid * rank;
      ++rank;
   }
   std::cout << "Part 1: " << result << std::endl;

   return 0;
}
