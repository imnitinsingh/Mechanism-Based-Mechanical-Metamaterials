#ifndef DEFS_PSO_HPP
#define DEFS_PSO_HPP

// --------------------------------------------------------------------------------------------------------------------------------
// common includes (limit to those that are really handy)

#include <cassert>
#include <cmath>

// --------------------------------------------------------------------------------------------------------------------------------
// common types (prefer c++ using expression above old style typedefs)

using uint = unsigned int;
using idtype = unsigned long long;

// --------------------------------------------------------------------------------------------------------------------------------
// common macros (use macros definitions sparsely, its old style )

#ifndef THROW_UNLESS
#define THROW_UNLESS( CLASS, EXPRESSION ) if (!(EXPRESSION)) { DEBUGBREAK; std::string ex_msg = make_string() << "EXCEPTION: " << #CLASS << " in " << __FILE__ << ":" << __LINE__ <<  " - " << __FUNCTION__ << "() " << "Check [" << #EXPRESSION << "] failed."; throw CLASS (ex_msg); } 
#endif

#ifndef THROW_UNLESS_MSG
#define THROW_UNLESS_MSG( CLASS, EXPRESSION , MESSAGE) if (!(EXPRESSION)) { DEBUGBREAK; std::string ex_msg = make_string() << "EXCEPTION: " << #CLASS << " in " << __FILE__ << ":" << __LINE__ <<  " - " << __FUNCTION__ << "() " << MESSAGE; throw CLASS (ex_msg); }
#endif

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION( CLASS, MESSAGE ) { DEBUGBREAK; std::string ex_msg = make_string() << "EXCEPTION: " << #CLASS  << " in " << __FILE__ << ":" << __LINE__ <<  " - " << __FUNCTION__ << "() " << MESSAGE; throw CLASS (ex_msg); }
#endif

#ifndef ASSERT
#define ASSERT( EXPRESSION )     assert( EXPRESSION );
#endif

#ifndef VERIFY
#define VERIFY( EXPRESSION )     { bool check = EXPRESSION; assert( check ); UNUSED(check) }
#endif

#ifndef UNUSED
#define UNUSED( ... )     { unused(__VA_ARGS__); }
#endif

#ifndef DEBUGBREAK
#ifdef _DEBUG
#ifdef _MSC_VER
#define DEBUGBREAK         __debugbreak();
#else
#define DEBUGBREAK         std::raise(SIGTRAP);
#endif
#else
#define DEBUGBREAK
#endif
#endif

template<class... T> void unused(T&&...) { }

// --------------------------------------------------------------------------------------------------------------------------------
// common constants 

static struct GlobalPsoConfig
{
   const double PI = 4 * atan(1.0);

} PsoCfg;

// --------------------------------------------------------------------------------------------------------------------------------
// Library export/import definition

#if defined(_MSC_VER)
#if defined(PSO_EXPORTS)
#define PSO_EXPORT __declspec(dllexport)
#else
#define PSO_EXPORT __declspec(dllimport)
#endif
#else
#define PSO_EXPORT
#endif

// --------------------------------------------------------------------------------------------------------------------------------


#endif
