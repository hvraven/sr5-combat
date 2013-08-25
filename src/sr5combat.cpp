#include "interface.hpp"
#include <iostream>

int main()
{
  interface i;

  try
    {
      i.run();
    }
  catch (const std::exception& e)
    {
      std::cerr << "What: " << e.what() << std::endl;
      return 1;
    }
}
