#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "HashMap.hpp"
#include <string>

class InvalidKey : public std::invalid_argument
{
  public:
    InvalidKey () : std::invalid_argument ("")
    {
    }

    explicit InvalidKey (const std::string &msg) : std::invalid_argument (msg)
    {
    }
};

class Dictionary : public HashMap<std::string, std::string>
{
  public:
    using HashMap<std::string, std::string>::HashMap;

    bool erase (const std::string &key) override
    {
      if (!HashMap::erase (key))
      {
        throw InvalidKey (VALUE_WITH_THAT_KEY_DOESNT_EXIST);
      }

      return true;
    }

    // TODO: ask if that's the correct way of receiving an iterator

    template <class Iterator>
    void update (Iterator begin, const Iterator &end)
    {
      for (; begin != end; ++begin)
      {
        updateValueT value = *begin;
        HashMap::insertion (value.first, value.second,
                            true); // true for override
      }
    }

  private:
    typedef std::pair<std::string, std::string> updateValueT;
};

#endif