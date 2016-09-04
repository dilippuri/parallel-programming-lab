#include <iostream>
#include <cmath>

using namespace std;

bool isPrime(int x)
{
  int limit = std::sqrt(x);
  for (int i = 2; i <= limit; ++i)
    {
      if (x % i == 0)
	{
	  return false;
	}
    }
  return true;
}

int main()
{
  int primeCount = 0;
  for (int i = 0; i < 1000000; ++i)
    {
      if (isPrime(i))
	{
	  ++primeCount;
	}
    }
  cout << primeCount << endl;
}
