// Catch2 recommends placing these lines by themselves in a translation unit
// which will help reduce unnecessary recompilations of the expensive Catch main
#define CATCH_CONFIG_MAIN
//#include "catch.hpp"
//#include <catch2/catch_all.hpp>
#include <catch2/catch.hpp>

#if 0
int main(int argc, char* argv[])
{
  Catch::Session session;

  int returnCode = session.applyCommandLine(argc, argv);
  if(returnCode != 0)
    return returnCode;

  int numFailed = session.run();
  return numFailed;
}
#endif
