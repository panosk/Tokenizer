#pragma once

#include <string>

#include "onmt/unicode/Data.h"

namespace onmt
{
  namespace unicode
  {

    bool _find_codepoint(code_point_t u, const map_of_list_t &map);

    std::string cp_to_utf8(code_point_t u);
    code_point_t utf8_to_cp(const unsigned char* s, unsigned int &l);

    std::vector<std::string> split_utf8(const std::string& str, const std::string& sep);
    void explode_utf8(const std::string &str,
                      std::vector<std::string>& chars,
                      std::vector<code_point_t>& code_points);

    enum _type_letter
    {
      _letter_other,
      _letter_lower,
      _letter_upper
    };

    inline bool is_separator(code_point_t u)
    {
      return (u >= 9 && u <= 13) || u==' ' || _find_codepoint(u, unidata_Separator);
    }

    inline bool is_letter(code_point_t u, _type_letter &tl)
    {
      // most frequent request goes fastest
      if (u<='z') {
        if (u>='a') {
          tl = _letter_lower;
          return 1;
        }
        if (u>='A' && u<='Z') {
          tl = _letter_upper;
          return 1;
        }
      }
      // unicode letter or CJK Unified Ideograph
      if ((u>=0x4E00 && u<=0x9FD5) // CJK Unified Ideograph
          || (u>=0x2F00 && u<=0x2FD5) // Kangxi Radicals
          || (u>=0x2E80 && u<=0x2EFF) // CJK Radicals Supplement
          || (u>=0x3040 && u<=0x319F) // Hiragana, Katakana, Bopomofo, Hangul Compatibility Jamo, Kanbun
          || (u>=0x1100 && u<=0x11FF) // Hangul Jamo
          || (u>=0xAC00 && u<=0xD7AF) // Hangul Syllables
          || _find_codepoint(u, unidata_LetterOther))
      {
        tl = _letter_other;
        return 1;
      }
      if (_find_codepoint(u, unidata_LetterLower))
      {
        tl = _letter_lower;
        return 1;
      }
      if (_find_codepoint(u, unidata_LetterUpper))
      {
        tl = _letter_upper;
        return 1;
      }

      return 0;
    }

    inline bool is_number(code_point_t u)
    {
      return (u>='0' && u<='9') || _find_codepoint(u, unidata_Number);
    }

    inline bool is_mark(code_point_t u)
    {
      return _find_codepoint(u, unidata_Mark);
    }

    code_point_t get_upper(code_point_t u);
    code_point_t get_lower(code_point_t u);

  }
}
