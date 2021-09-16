#include "defer.h"


#if defined __clang__  // requires -fblocks (lambdas)
void cleanup_deferred (void (^*d) (void))
{
  (*d)();
}
#elif defined __GNUC__ // nested-function-in-stmt-expressionstatic
void cleanup_deferred (void (**d) (void))
{ 
  (*d)();
}
#endif

