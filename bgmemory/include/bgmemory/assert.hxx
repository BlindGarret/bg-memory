#if ASSERTIONS_ENABLED

#include <iostream>
#include <signal.h>

// define some inline assembly that causes a break
// into the debugger -- this will be different on each
// target CPU
#define debugBreak() { raise(SIGTRAP); }

// check the expression and fail if it is false
#define ASSERT(expr) \
if (expr) { } \
else \
{ \
std::cout << "ASSERTION FAILURE: " << #expr << " : " << __FILE__ << " : " << __LINE__ << std::endl; \
debugBreak(); \
}
#else
#define ASSERT(expr)
// evaluates to nothing
#endif