#ifndef __Dev_3__ForceInclude__Config_hpp
#define __Dev_3__ForceInclude__Config_hpp 1


#include <boost/config.hpp>


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if ADDER_TARGET_TCHAR < 2
 #define ADDER_TARGET_UNICODE 0
#else
 #define ADDER_TARGET_UNICODE 1
#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// ADDER_CONCAT:
// 
// Joins together its two arguments, similarly to LOKI_CONCAT and BOOST_JOIN.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define ADDER_CONCAT(X, Y)   ADDER_CONCAT_1 (X, Y)
#define ADDER_CONCAT_1(X, Y) ADDER_CONCAT_2 (X, Y)
#define ADDER_CONCAT_2(X, Y) X##Y


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// ADDER_STATIC_CHECK, ADDER_STATIC_ASSERT:
// 
// Stolen from Loki and Boost.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace Adder
{
	template <int I>
	struct CompileTimeError;
	
	
	template <>
	struct CompileTimeError <true>
	{};
}


#define ADDER_STATIC_CHECK(expr, msg) \
	enum \
	{ \
		ADDER_CONCAT (ERROR_##msg, __LINE__) = sizeof (::Adder::CompileTimeError <(expr) != 0>) \
	}


#define ADDER_STATIC_ASSERT(expr) ADDER_STATIC_CHECK (expr, YouAreTooLazyToWriteAMessage)


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES
 #define ADDER_CXX11_RVALUE_REFERENCES_V 0
#else
 #define ADDER_CXX11_RVALUE_REFERENCES_V 1
#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// ADDER_TARGET_* Macros (2):
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

 //---------------------------------------------------------------------------
 // ADDER_TARGET_DEBUG:
 // (might be useful, for convenience)
 // 
 // == 0 for release builds;
 // != 0 for debug   builds (more precisely: == 1).
 // 
 // The standard way (according to the C and C++ ANSI/ISO standards) to detect
 // a debug/release build is to use the NDEBUG macro, which is:
 //   - not defined (for debug   builds - and non-conforming compilers :D)
 //   - defined     (for release builds)
 // 
 // At least the following compilers conform to the standard in this respect:
 //   borland, dmc, msvc, darwin.
 // 
 // However, at many times, it is more convenient to have a macro
 // that is always defined and the (boolean-testable) value of which
 // distinguishes between debug/release builds. Example:
 // 
 //   SaveFmtFlags <tchar> fmt (&tcout, ADDER_TARGET_DEBUG);
 // 
 // The compiler-specific _DEBUG and DEBUG macros are also checked/defined
 // here (for consistency).
 // 
 // The commented code here shows how some compilers signal a debug build
 // (eg: by defining DEBUG or _DEBUG).
 //---------------------------------------------------------------------------

#if ! defined (ADDER_TARGET_DEBUG)
	#if defined (NDEBUG)
		#define ADDER_TARGET_DEBUG 0
	#else
		#define ADDER_TARGET_DEBUG 1
	#endif
#endif


//#if ! defined (ADDER_TARGET_DEBUG)
//	#if defined (_MSC_VER) || defined (__BORLANDC__)
//		#if defined (_DEBUG)
//			#define ADDER_TARGET_DEBUG 1
//		#else
//			#define ADDER_TARGET_DEBUG 0
//		#endif
//	#elif defined (__DMC__) || defined(__GNUC__)
//		#if defined (DEBUG)
//			#define ADDER_TARGET_DEBUG 1
//		#else
//			#define ADDER_TARGET_DEBUG 0
//		#endif
//	#endif
//#endif


#if ! defined (ADDER_TARGET_DEBUG)
	#error ADDER_TARGET_DEBUG is not defined !
#endif


#if defined (NDEBUG) && (defined (DEBUG) || defined (_DEBUG))
	#error Macro definition inconsistency: NDEBUG vs DEBUG/_DEBUG !
#endif


#if defined (NDEBUG) && ADDER_TARGET_DEBUG
	#error Macro definition inconsistency: NDEBUG vs ADDER_TARGET_DEBUG !
#endif


 //---------------------------------------------------------------------------
 // ADDER_TARGET_DEBUGSTL:
 // (might be useful, for convenience)
 // 
 // != 0 for builds with debug versions of STL (more precisely: == 1);
 // == 0 for other builds.
 // 
 // Several popular implementations of STL provide a debug mode,
 // which are enabled as follows:
 // 
 //   - RogueWave/Apache stdcxx: _RWSTDDEBUG defined;
 //   - STLport:                 _STLP_DEBUG defined;
 //   - Dinkumware:              _HAS_ITERATOR_DEBUGGING defined as non-zero;
 //   - LibStdC++:               _GLIBCXX_DEBUG defined.
 //---------------------------------------------------------------------------

#if ! defined (ADDER_TARGET_DEBUGSTL)
	#if defined (_RWSTDDEBUG) || defined (_STLP_DEBUG) || _HAS_ITERATOR_DEBUGGING || defined (_GLIBCXX_DEBUG)
		#define ADDER_TARGET_DEBUGSTL 1
	#else
		#define ADDER_TARGET_DEBUGSTL 0
	#endif
#endif


#if ! defined (ADDER_TARGET_DEBUGSTL)
	#error ADDER_TARGET_DEBUGSTL is not defined !
#endif


#if (defined (_RWSTDDEBUG) || defined (_STLP_DEBUG) || _HAS_ITERATOR_DEBUGGING || defined (_GLIBCXX_DEBUG)) && ! ADDER_TARGET_DEBUGSTL
	#error Macro definition inconsistency: ADDER_TARGET_DEBUGSTL !
#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// ADDER_NOTHROW:
// 
// This macro is to be used instead of the exception specification "throw ()".
// For some compilers/systems/configurations, it could be defined to expand
// to nothing.
// 
// It is analogous to:
//
//   - _RWSTD_THROW_SPEC_NULL  (borland-5.5 RogueWave  STL);
//   - _THROW0                 (msvc-7.1    Dinkumware STL).
// 
// (Note: STLport 4.5.xx, which is delivered with DigitalMars among others,
// does not even use a macro, it simply avoids "throw ()" --
// and, of course, any other exception specification).
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define ADDER_NOTHROW throw ()


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// conmv:
// 
// This macro expands to nothing (but we might decide to expand it to "const").
// It has the same number of characters as "const".
// It is meant as a hint for the human reader:
// the variable defined here is not modified, except for being moved from/returned.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define conmv


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// ADDER_OVERRIDE:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifdef BOOST_NO_CXX11_OVERRIDE
 #define ADDER_OVERRIDE
#else
 #define ADDER_OVERRIDE override
#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


#endif
