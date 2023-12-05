// string_view to integer
// "Fixes" the missing stoi and friends

#pragma once
#ifndef SVTOI_H
#define SVTOI_H

#include <string_view>

long svtol( std::string_view sv, size_t * idx = nullptr, int base = 10 )
{
   char * next{};
   auto result = strtol( sv.data(), &next, base );

   if (idx) *idx = next - sv.data();
   return result;
}

unsigned long svtoul( std::string_view sv, size_t * idx = nullptr, int base = 10 )
{
   char * next{};
   auto result = strtoul( sv.data(), &next, base );

   if (idx) *idx = next - sv.data();
   return result;
}

#endif // SVTOI_H
