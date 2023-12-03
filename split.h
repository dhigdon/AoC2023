// Utility - split a string_view into a vector of string_views
// by Dan Higdon

#pragma(once)
#ifndef SPLIT_H
#define SPLIT_H

#include <string_view>
#include <vector>

// Utility function to mimic the "split" feature of other languages
// It's not very efficient, but it's good enough for these toy applications.
// A better version would be a lazy iterator or something.
inline auto split( std::string_view src, char delim )
{
   std::vector< std::string_view > result;

   size_t start{ 0 }, end{ src.find( delim ) };
   while ( end != -1 )
   {
      result.push_back( src.substr( start, end - start ) );
      start = end + 1;
      end = src.find( delim, start );
   }
   result.push_back( src.substr( start ) );

   return result;
}

// TODO: reimplement this to return an iterator

#endif // SPLIT_H
