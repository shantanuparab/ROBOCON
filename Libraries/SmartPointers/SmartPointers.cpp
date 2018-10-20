// SmartPointers.cpp : Defines the entry point for the application.
//

#include "SmartPointers.h"

#include <iostream>

void FuncPtr(int* ptr)
{
   *ptr = 3;
}

void convert(SmartUniquePtr<int>& p_val)
{
   *p_val = 4;
}

struct Calculator
{
   int a;
   int b;

   int Add()
   {
      return a + b;
   }
   int Sub()
   {
      return a - b;
   }
};

int main()
{
   // Allocates an Array of 3 Integers
   SmartUniquePtr<int[]> x{3};
   // Allocates an Array of 3 Integers
   SmartUniquePtr<int[]> y{3};

   x[0] = 24;
   x[1] = 25;

   // SHow 24, 25
   std::cout << "Hello X is now." << x[0] << x[1] << '\n';

   y[0] = 34;
   y[1] = 45;

   x = y;

   // SHow 34, 45
   std::cout << "Hello X is now." << x[0] << x[1] << '\n';
	int i; std::cin>>i;

   // Allocates an Integer Ptr with value 45
   SmartUniquePtr<int> a(i);

   std::cout << "Value of a is " << *a << '\n';
   // This will convert a value from 45 to 4
   convert(a);
   // Will Display 4
   std::cout << "Value of a is " << *a << '\n';
   // Note that FuncPtr takes Parameter as Pointer
   // Not a reference
   // Hence pass Raw Pointer via get method
   FuncPtr(a.get());
   // Will Display 3
   std::cout << "Value of a is " << *a << '\n';

	SmartUniquePtr<int> b(3);

	//We can compare the underlying values too
	if (a == b)
      std::cout << "a is same as b" << '\n';

   SmartUniquePtr<Calculator> calc{};
   calc->a = 45;
   calc->b = 41;

   std::cout << "Applying Calculator we Get Add : " << calc->Add() << " : Sub"
             << calc->Sub();

   // Note that all these values will be deallocated
   // at the end of their Scope
   return 0;
}