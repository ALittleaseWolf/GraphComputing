#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <utility>
#include <cassert>
#include <queue>
#include <algorithm>
#include <string>
typedef uint64_t u64;
typedef uint32_t u32;

#ifdef DEBUG
#define print(...) std::cout<<__VA_ARGS__<<std::endl
#define prints(...) std::cout<<__VA_ARGS__<< " "
#define line() std::cout << std::endl
#else
#define print(...)
#define prints(...)
#define line()
#endif

using Uint32Pair = std::pair<uint32_t, uint32_t>;

namespace utils{

uint64_t getFileSize(std::ifstream& openFile) {
  openFile.seekg(0, std::ios_base::end);
  return openFile.tellg();
}

template <typename type>
void print_vector(std::string s, std::vector<type> v)
{
    prints(s);
    for(auto v1 : v)
    {
        prints(v1);
    }
    line();
}

template <typename type1,typename type2>
void print_map(std::string s, std::map<type1,type2> m)
{
    print(s);
    for(auto m1:m)
    {
        prints(m1.first);
    }
    line();
    for(auto m1:m)
    {
        prints(m1.second);
    }
    line();
}
}
