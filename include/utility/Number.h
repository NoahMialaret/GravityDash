#ifndef SCORE_H
#define SCORE_H

#include "Utility.h"
#include "ProgramSettings.h"

#include <list>

// A templated class which represents a number as a list of digits 
template<typename Digit>
class Number
{
public:
  // Constructs a number with a given starting value
  Number(unsigned int startingValue = 0);

  // Adds `value` to Number
  void AddValue(int value);
  // Adds `value` to `digits`
  static void AddValue(std::list<Digit>& digits, int value);
  // Zeros out Number
  void Zero(); 
  // Zeros out `digits`
  static void Zero(std::list<Digit>& digits);
  // Returns the string representation of the number
  std::string AsString() const;

private:
  // Adds a positve value to the number
  static void Add(std::list<Digit>& digits, unsigned int value);
  // Substracts a value from the number, Number can't be negative
  static void Subtract(std::list<Digit>& digits, unsigned int value);

private:
  std::list<Digit> digits; // The digits of the number, ordered from most to least significant
};

template <typename Digit>
inline Number<Digit>::Number(unsigned int startingValue)
{
  if (startingValue == 0)
  {
    digits.push_back(Digit(0));
    return;
  }

  Add(digits, startingValue);
}

template <typename Digit>
void Number<Digit>::AddValue(int value)
{
  AddValue(digits, value);
}

template <typename Digit>
inline void Number<Digit>::AddValue(std::list<Digit>& digits, int value)
{
  if (value > 0)
    Add(digits, value);
  else if (value < 0)
    Subtract(digits, -value);
}

template <typename Digit>
void Number<Digit>::Zero()
{
  Zero(digits);
}

template <typename Digit>
inline void Number<Digit>::Zero(std::list<Digit>& digits)
{
  auto it = digits.begin();
  (*it) = 0;
  
  it++;
  while (it != digits.end())
    it = digits.erase(it);
}

template <typename Digit>
inline std::string Number<Digit>::AsString() const
{
  std::string num;

  for (auto it = digits.begin(); it != digits.end(); it++)
  {
    num.append(std::to_string(int(*it)));
  }

  return num;
}

template <typename Digit>
void Number<Digit>::Add(std::list<Digit>& digits, unsigned int value)
{
  bool carry = false;
  for (auto it = digits.rbegin(); it != digits.rend() && (value != 0 || carry); it++, value /= 10)
  {
    int digit = value % 10;

    *it += digit + carry;

    carry = false;

    if ((int)(*it) > 9)
    {
      *it -= 10;
      carry = true;
    }
  }

  while (value != 0 || carry)
  {
    int digit = value % 10;

    int newDigit = digit + carry;

    carry = false;
    if (newDigit > 9)
    {
      newDigit -= 10;
      carry = true;
    }

    digits.push_front(Digit(newDigit));

    value /= 10;
  }
}

template <typename Digit>
void Number<Digit>::Subtract(std::list<Digit>& digits, unsigned int value)
{
  bool carry = false;
  
  for (auto it = digits.rbegin(); it != digits.rend() && (value != 0 || carry); it++, value /= 10)
  {
    int digit = value % 10;

    *it -= digit + carry;

    carry = false;

    if ((int)(*it) < 0)
    {
      if (digits.size() == 1)
      {
        Zero(digits);
        return;
      }
      *it += 10;
      carry = true;
    }
  }

  if (value != 0 || carry)
  {
    Zero(digits);
    return;
  }

  for (auto it = digits.begin(); it != digits.end() && *it == 0;)
    it = digits.erase(it);

  if (digits.empty())
    Zero(digits);
}


#endif