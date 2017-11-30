#include "onmt/unicode/Unicode.h"

namespace onmt
{
  namespace unicode
  {

    std::string cp_to_utf8(code_point_t uc)
    {
      unsigned char u1, u2, u3, u4;
      char ret[5];

      if (uc < 0x80)
      {
        ret[0] = uc; ret[1] = 0;
        return ret;
      }
      else if (uc < 0x800)
      {
        u2 = 0xC0 | (uc >> 6);
        u1 = 0x80 | (uc & 0x3F);
        ret[0] = u2;
        ret[1] = u1;
        ret[2] = 0;
        return ret;
      }
      else if (uc < 0x10000)
      {
        u3 = 0xE0 | (uc >> 12);
        u2 = 0x80 | ((uc >> 6) & 0x3F);
        u1 = 0x80 | (uc & 0x3F);
        ret[0] = u3;
        ret[1] = u2;
        ret[2] = u1;
        ret[3] = 0;
        return ret;
      }
      else if (uc < 0x200000)
      {
        u4 = 0xF0 | (uc >> 18);
        u3 = 0x80 | ((uc >> 12) & 0x3F);
        u2 = 0x80 | ((uc >> 6) & 0x3F);
        u1 = 0x80 | (uc & 0x3F);
        ret[0] = u4;
        ret[1] = u3;
        ret[2] = u2;
        ret[3] = u1;
        ret[4] = 0;
        return ret;
      }

      return "";
    }

    code_point_t utf8_to_cp(const unsigned char* s, unsigned int &l)
    {
      if (*s == 0 || *s >= 0xfe)
        return 0;
      if (*s <= 0x7f)
      {
        l = 1;
        return *s;
      }
      if (!s[1])
        return 0;
      if (*s < 0xe0)
      {
        l = 2;
        return ((s[0] & 0x1f) << 6) + ((s[1] & 0x3f));
      }
      if (!s[2])
        return 0;
      if (*s < 0xf0)
      {
        l = 3;
        return ((s[0] & 0x0f) << 12) + ((s[1] & 0x3f) << 6) + ((s[2] & 0x3f));
      }
      if (!s[3])
        return 0;
      if (*s < 0xf8)
      {
        l = 4;
        return (((s[0] & 0x07) << 18) + ((s[1] & 0x3f) << 12) + ((s[2] & 0x3f) << 6)
                + ((s[3] & 0x3f)));
      }

      return 0; // Incorrect unicode
    }

    std::vector<std::string> split_utf8(const std::string& str, const std::string& sep)
    {
      std::vector<std::string> chars;
      std::vector<code_point_t> code_points;

      explode_utf8(str, chars, code_points);

      std::vector<std::string> fragments;
      std::string fragment;

      for (size_t i = 0; i < chars.size(); ++i)
      {
        if (chars[i] == sep)
        {
          fragments.push_back(fragment);
          fragment.clear();
        }
        else
        {
          fragment += chars[i];
        }
      }

      if (!fragment.empty() || chars.back() == sep)
        fragments.push_back(fragment);

      return fragments;
    }

    void explode_utf8(const std::string &str,
                      std::vector<std::string>& chars,
                      std::vector<code_point_t>& code_points)
    {
      size_t offset = 0;

      while (offset < str.length())
      {
        unsigned int char_size = 0;
        code_point_t code_point = utf8_to_cp(reinterpret_cast<const unsigned char*>(str.c_str()+offset),
                                                     char_size);

        code_points.push_back(code_point);
        chars.emplace_back(str.substr(offset, char_size));
        offset += char_size;
      }
    }

    bool _find_codepoint(code_point_t u, const map_of_list_t &map)
    {
      if (!u) return false;
      map_of_list_t::const_iterator it = map.begin();

      while (it != map.end())
      {
        if (u >= it->first)
        {
          unsigned int idx = ((u - it->first) >> 4);
          if (idx < it->second.size())
          {
            unsigned int p = (u - it->first) & 0xf;
            return (((it->second[idx] << p)) & 0x8000);
          }
        }

        it++;
      }

      return 0;
    }

    // convert unicode character to lowercase form if defined in unicodedata
    code_point_t get_lower(code_point_t u)
    {
      map_unicode::const_iterator it = map_lower.find(u);
      if (it == map_lower.end())
        return 0;
      return it->second;
    }

    // convert unicode character to uppercase form if defined in unicodedata
    // dynamically reverse maplower if necessary
    static map_unicode map_upper;

    code_point_t get_upper(code_point_t u)
    {
      if (map_upper.size() == 0)
      {
        map_unicode::const_iterator it;
        for (it = map_lower.begin(); it != map_lower.end(); it++)
        {
          map_unicode::const_iterator jt = map_upper.find(it->second);
          // reversing, we find smallest codepoint
          if (jt == map_upper.end() || jt->second>it->first)
            map_upper[it->second] = it->first;
        }
      }

      map_unicode::const_iterator it = map_upper.find(u);
      if (it == map_upper.end())
        return 0;
      return it->second;
    }

  }
}
