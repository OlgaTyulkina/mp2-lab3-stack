#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdexcept>
#include <cstring>
#include "tformula.h"

TFormula::TFormula(const char* form)
{
    formulaLength = std::strlen(form);
    postfixFormulaLength = 0;

    if (formulaLength == 0) 
        throw std::range_error("Unexpected formula length");

    std::strcpy(Formula, form);
}

int TFormula::FormulaChecker(int* Brackets)
{
  TStack stack(formulaLength);
  int bracketsCount = 0;
  int bracketsArrIndex = 0;
  int errIndex = 0;

  for (auto i = 0; i < formulaLength; i++)                            // поиск пар скобок
  {
    if (Formula[i] == '(')
      stack.Put(++bracketsCount);
    else
      {
        if (Formula[i] == ')')
        {
          Brackets[++bracketsArrIndex] = ++bracketsCount;

          if (stack.IsEmpty())
          {
            Brackets[--bracketsArrIndex] = 0;
            bracketsArrIndex += 2;
            errIndex++;
          }
          else
          {
            Brackets[--bracketsArrIndex] = stack.Get();
            bracketsArrIndex += 2;
          }
        }
        else
          continue;
      }
  }

  if (!stack.IsEmpty())                                                // запись оставшихся в стеке открывающих скобок
  {
    while (!stack.IsEmpty())
    {
      Brackets[bracketsArrIndex++] = stack.Get();
      Brackets[bracketsArrIndex++] = 0;
      errIndex++;
    }
  }

  std::cout << "\t BRACKETS \n  Opening   |   Closing" << std::endl;  // вывод таблицы
  for (auto i = 0; i < bracketsArrIndex;)
  {
    if ((i + 1) % 2 != 0)
      if (Brackets[i] == 0)
      {
        std::cout << "     " << "-" << "      |      ";
        i++;
      }
      else
        std::cout << "     " << Brackets[i++] << "      |      ";
    else
      if (Brackets[i] == 0)
      {
        std::cout << "-" << std::endl;
        i++;
      }
      else
        std::cout << Brackets[i++] << std::endl;;
  }

  std::cout << "  Errors: " << errIndex << std::endl;

  return errIndex;
}

int TFormula::FormulaConverter()
{
  TStack stack(formulaLength);

  for (auto i = 0; i < formulaLength; i++)
  {
    if (isdigit(Formula[i]))
    {
      PostfixForm[postfixFormulaLength++] = Formula[i];
    }
    else  if (Formula[i] != ')')
          {
            if ((!stack.IsEmpty() && (Priority(Formula[i]) == 0 || 
            Priority(Formula[i]) > Priority(stack.TopElem()))) || 
            stack.IsEmpty())
              stack.Put(Formula[i]);
            
            else
            {
              while (!stack.IsEmpty() && Priority(stack.TopElem()) >= Priority(Formula[i]))
                PostfixForm[postfixFormulaLength++] = stack.Get();
              
              stack.Put(Formula[i]);
            }
          }
          else
          {
            while (!stack.IsEmpty() && Priority(stack.TopElem()) != Priority('('))
              PostfixForm[postfixFormulaLength++] = stack.Get();

            stack.Get();
          }
    }

  while (!stack.IsEmpty())
    PostfixForm[postfixFormulaLength++] = stack.Get();

  std::cout << "\t Postfix formula: ";

  for (auto i = 0; i < postfixFormulaLength; i++)
  {
    std::cout << PostfixForm[i];
  }
  std::cout << std::endl;
  return 0;
}

double TFormula::FormulaCalculator()
{
  int* Brackets = new int[formulaLength * 2]{};

  if (FormulaChecker(Brackets) != 0)
  {
    throw std::logic_error("Incorrect formula");
    return -1;
  }

  FormulaConverter();

  return 0;
}

int TFormula::Priority(char sign)
{
  switch (sign)
  {
    case '(' : return 0;
    case ')' : return 1;
    case '+' : return 2;
    case '-' : return 2;
    case '*' : return 3;
    case '/' : return 3;
    default : return -1;
  }
}