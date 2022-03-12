// --------------------------------------------------------------------------------------------------------------------------------
/*
Simple Unit Testing
Original: Cosmin Cremarenco.
*/
// --------------------------------------------------------------------------------------------------------------------------------

#ifndef TINYTEST_HPP
#define TINYTEST_HPP

// --------------------------------------------------------------------------------------------------------------------------------

#define NOMINMAX 
#include <inttypes.h>
#include <cmath>
#include <string>
#include <cstring>

// --------------------------------------------------------------------------------------------------------------------------------

#if defined(_MSC_VER)
#include <windows.h>

void TinyTestDebugString(const char * message)
{
   OutputDebugStringA(message);
}

int64_t TinyTestGetTime()
{
   LARGE_INTEGER li;
   QueryPerformanceCounter(&li);
   return li.QuadPart;
}

double TinyTestGetDuration(int64_t ts, int64_t te)
{
   LARGE_INTEGER f;
   QueryPerformanceFrequency(&f);
   return 1E6* static_cast<double>(te - ts) / f.QuadPart;
}

#else
#include <iostream>
#include <time.h>

void TinyTestDebugString(const char * message)
{
   std::cout << message;
}

int64_t TinyTestGetTime()
{
   struct timespec ts;
   clock_gettime(CLOCK_MONOTONIC, &ts);
   int64_t t = ts.tv_nsec + ts.tv_sec * 1E9;
   return t;
}

double TinyTestGetDuration(int64_t ts, int64_t te)
{
   struct timespec tr;
   clock_getres(CLOCK_MONOTONIC, &tr);
   int64_t r = tr.tv_nsec + tr.tv_sec * 1E9;
   return 1E-3* static_cast<double>(te - ts) / r;
}

#endif

// --------------------------------------------------------------------------------------------------------------------------------

typedef int(*TinyTestFunc)(void);

typedef struct TinyTestStruct
{
   TinyTestFunc m_func;
   const char* m_name;
   struct TinyTestStruct* m_next;
} TinyTest;

typedef struct TinyTestSuiteStruct
{
   struct TinyTestStruct* m_head;
   const char* m_name;
   struct TinyTestStruct* m_headTest;
   struct TinyTestSuiteStruct* m_next;
} TinyTestSuite;

typedef struct TinyTestRegistryStruct
{
   TinyTestSuite* m_headSuite;
} TinyTestRegistry;

// --------------------------------------------------------------------------------------------------------------------------------



#if defined(_DEBUG) && defined(_MSC_VER)
#define TINYSTINGLENGTH   256
#define TINYFAIL              __debugbreak(); return 0;
#define TINYPRINTA(fmt, ...)   if (fmt) { char szTmp[TINYSTINGLENGTH]; sprintf_s(szTmp, TINYSTINGLENGTH, fmt, ##__VA_ARGS__); TinyTestDebugString(szTmp); printf(szTmp); }
#define TINYERRORA(fmt, ...)   { char szTmp[TINYSTINGLENGTH]; sprintf_s(szTmp, TINYSTINGLENGTH, "\n%s:%d - ", __FILE__,__LINE__); TinyTestDebugString(szTmp); fprintf(stderr, szTmp); sprintf_s(szTmp, TINYSTINGLENGTH, fmt, ##__VA_ARGS__); TinyTestDebugString(szTmp); fprintf(stderr, szTmp); }
#define TINYPRINT(msg)   if (msg) { TinyTestDebugString(msg); printf(msg); }
#define TINYERROR(msg)   { TinyTestDebugString(msg); fprintf(stderr, msg); }
#else
#define TINYFAIL              return 0;
#define TINYPRINTA(fmt, ...)   if (fmt) printf(fmt, ##__VA_ARGS__);
#define TINYERRORA(fmt, ...)   if (fmt) fprintf(stderr, fmt, ##__VA_ARGS__);
#define TINYPRINT(msg)   if (msg) printf("%s", msg);
#define TINYERROR(msg)   if (msg) fprintf(stderr, "%s", msg);


#if defined(_MSC_VER)
#pragma warning(disable:4127)    // fmt is a string, so ignore 'conditional expression is constant' warning (on MSVC)
#endif

#endif


// --------------------------------------------------------------------------------------------------------------------------------

#define TINYTEST_TIME_START() { int64_t te,ts; ts = TinyTestGetTime();
#define TINYTEST_TIME_STOP(msg) te = TinyTestGetTime(); TINYPRINTA("Time '%s': %0.3lf us\r\n", msg, TinyTestGetDuration(ts,te)); }

#define TINYTEST_TIME_ELAPSE(msg, code_block ) \
TINYTEST_TIME_START()                          \
/code_block/                                   \
TINYTEST_TIME_STOP(msg)

// --------------------------------------------------------------------------------------------------------------------------------

#define TINYTEST_CHECK_THROW(s, E)                                                        \
   try { s;                                                                               \
      TINYERRORA("'%s' was not thrown", #E)                                               \
      TINYFAIL                                                                            \
   }                                                                                      \
   catch (E)                                                                              \
   { }                                                                                    \
   catch (...)                                                                            \
   { TINYERRORA("'%s' was not thrown, but some other exception was!", #E)                 \
     TINYFAIL  }

#define TINYTEST_CHECK_NOTHROW(s)                                                         \
   try { s;  }                                                                            \
   catch (...)                                                                            \
   { TINYERROR("Exception was unexpected!") TINYFAIL }                                    \

// --------------------------------------------------------------------------------------------------------------------------------

#define TINYTEST_ALMOST_EQUAL_MSG(expected, actual, precision, msg)                                                          \
if ( std::fabs(expected - actual) > precision ) {                                                                            \
    TINYERRORA("expected %.16g, actual: %.16g, delta: %.16g\n", expected, actual, std::fabs(expected - actual))              \
    TINYPRINT(msg)                                                                                                           \
    TINYFAIL                                                                                                                 \
}

#define TINYTEST_ALMOST_REL_EQUAL_MSG(expected, actual, relprecision, msg)                                                   \
if ( std::fabs((expected - actual) / expected) > relprecision ) {                                                            \
    TINYERRORA("expected %.16g, actual: %.16g, rel: %.16g\n", expected, actual, std::fabs((expected - actual) / expected))   \
    TINYPRINT(msg)                                                                                                           \
    TINYFAIL                                                                                                                 \
  }

// --------------------------------------------------------------------------------------------------------------------------------

#define TINYTEST_ALMOST_EQUALS(expected, actual)                        \
  TINYTEST_ALMOST_EQUAL_MSG(expected, actual, 1E-15, #actual)


#define TINYTEST_ALMOST_EQUAL(expected, actual, precision)              \
  TINYTEST_ALMOST_EQUAL_MSG(expected, actual, precision, #actual)

#define TINYTEST_ALMOST_REL_EQUAL(expected, actual, precision)          \
  TINYTEST_ALMOST_REL_EQUAL_MSG(expected, actual, precision, #actual)

// --------------------------------------------------------------------------------------------------------------------------------

#define TINYTEST_EQUAL_MSG(expected, actual, msg)                       \
  if ( (expected) != (actual) ) {                                       \
    TINYERRORA("expected %s, actual: %s\n", #expected, #actual)         \
    TINYPRINT(msg)                                                      \
    TINYFAIL                                                            \
  }

// --------------------------------------------------------------------------------------------------------------------------------

#define TINYTEST_FAIL(expression)                                       \
  TINYTEST_ASSERT(!(expression))

#define TINYTEST_FAIL_MSG(expression, msg)                              \
  TINYTEST_ASSERT_MSG(!(expression), msg)


#define TINYTEST_EQUAL(expected, actual)                                \
  TINYTEST_EQUAL_MSG(expected, actual, #actual)

// --------------------------------------------------------------------------------------------------------------------------------

#define TINYTEST_STR_EQUAL_MSG(expected, actual, msg)                   \
  if ( std::strcmp((expected), (actual)) ) {                            \
    TINYERRORA("expected \"%s\", actual: \"%s\"\n", expected, actual)   \
    TINYPRINT(msg);                                                     \
    TINYFAIL                                                            \
  }

#define TINYTEST_STR_EQUAL(expected, actual)                            \
  TINYTEST_STR_EQUAL_MSG(expected, actual, #actual)

// --------------------------------------------------------------------------------------------------------------------------------

#define TINYTEST_ASSERT_MSG(assertion, msg)                             \
  if ( !(assertion) ) {                                                 \
    TINYERRORA("assertion failed: \"%s\"\n", #assertion)                \
    TINYPRINT(msg)                                                      \
    TINYFAIL                                                            \
  }

#define TINYTEST_ASSERT(assertion)                                      \
  TINYTEST_ASSERT_MSG(assertion, #assertion)

// --------------------------------------------------------------------------------------------------------------------------------

#define TINYTEST_DECLARE_SUITE(suiteName)                               \
  void Suite##suiteName(TinyTestRegistry* registry)

#define TINYTEST_START_SUITE(suiteName)                                 \
void Suite##suiteName(TinyTestRegistry* registry)                       \
{                                                                       \
  TinyTestSuite* suite = (TinyTestSuite*)malloc(sizeof(TinyTestSuite)); \
  suite->m_name = #suiteName;                                           \
  suite->m_headTest = nullptr;                                          \
  suite->m_next = nullptr;                                              \
  TinyTest* lasttest = nullptr;

#define TINYTEST_ADD_TEST(test)                                                                        \
  TinyTest* test##decl = (TinyTest*)malloc(sizeof(TinyTest));                                          \
  test##decl->m_func = test;                                                                           \
  test##decl->m_name = #test;                                                                          \
  test##decl->m_next = nullptr;                                                                        \
  if (lasttest) lasttest->m_next = test##decl; else suite->m_headTest = test##decl;                    \
  lasttest = test##decl;

#define TINYTEST_END_SUITE()                                            \
  suite->m_next = nullptr;                                              \
  if (registry->m_headSuite)                                            \
  { TinyTestSuite* lastsuite = registry->m_headSuite;                   \
    while (lastsuite->m_next) lastsuite = lastsuite->m_next;            \
    lastsuite->m_next = suite;                                          \
  } else registry->m_headSuite = suite;                                 \
}

#define TINYTEST_START_MAIN()                                           \
  int main(/*int argc, char* argv[]*/)                                  \
  {                                                                     \
    TinyTestRegistry registry;                                          \
    registry.m_headSuite = nullptr;

#define TINYTEST_RUN_SUITE(suiteName)                                   \
  Suite##suiteName(&registry) 

#define TINYTEST_INTERNAL_FREE_TESTS()                                  \
  {                                                                     \
    TinyTestSuite* s = registry.m_headSuite;                            \
    TinyTestSuite* ss = nullptr;                                        \
    for ( ; s; s = ss )                                                 \
    {                                                                   \
      ss = s->m_next;                                                   \
      TinyTest* t = s->m_headTest;                                      \
      TinyTest* tt = nullptr;                                           \
      for ( ; t; t = tt )                                               \
      {                                                                 \
        tt = t->m_next;                                                 \
        free(t);                                                        \
      }                                                                 \
      free(s);                                                          \
    }                                                                   \
  }

#define TINYTEST_INTERNAL_RUN_TESTS()                                   \
  int okTests = 0;                                                      \
  int failedTests = 0;                                                  \
  {                                                                     \
    TinyTestSuite* s = registry.m_headSuite;                            \
    for ( ; s; s = s->m_next )                                          \
    {                                                                   \
      TINYPRINTA("%s:",s->m_name);                                      \
      TinyTest* t = s->m_headTest;                                      \
      for ( ; t; t = t->m_next )                                        \
      {                                                                 \
        if ( (*t->m_func)() )                                           \
        {                                                               \
          TINYPRINT(".");                                               \
          ++okTests;                                                    \
        }                                                               \
        else                                                            \
        {                                                               \
          TINYPRINT("x");                                               \
          ++failedTests;                                                \
        }                                                               \
      }                                                                 \
      TINYPRINT("\n");                                                  \
    }                                                                   \
    TINYPRINTA("\nOK: %d", okTests);                                    \
    if ( failedTests )                                                  \
    {                                                                   \
      TINYPRINTA(" FAILED: %d", failedTests);                           \
    }                                                                   \
    TINYPRINT("\n");                                                    \
  }

#define TINYTEST_END_MAIN()                                             \
    TINYTEST_INTERNAL_RUN_TESTS();                                      \
    TINYPRINT("\n");                                                    \
    TINYTEST_INTERNAL_FREE_TESTS()                                      \
    return failedTests;                                                 \
  }

#define TINYTEST_MAIN_SINGLE_SUITE(suiteName)                           \
  TINYTEST_START_MAIN();                                                \
  TINYTEST_RUN_SUITE(suiteName);                                        \
  TINYTEST_END_MAIN();

// --------------------------------------------------------------------------------------------------------------------------------

#endif
