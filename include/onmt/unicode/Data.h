#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace onmt
{
  namespace unicode
  {

    typedef uint32_t code_point_t;
    typedef std::map<code_point_t, std::vector<code_point_t> > map_of_list_t;
    typedef std::unordered_map<code_point_t, code_point_t> map_unicode;

    extern const map_of_list_t unidata_Number;
    extern const map_of_list_t unidata_LetterUpper;
    extern const map_of_list_t unidata_LetterLower;
    extern const map_of_list_t unidata_LetterOther;
    extern const map_of_list_t unidata_Separator;
    extern const map_of_list_t unidata_Mark;
    extern const map_unicode map_lower;

  }
}
