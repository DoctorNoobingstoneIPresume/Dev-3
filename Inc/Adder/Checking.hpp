#ifndef __Dev_3_Checking_hpp
#define __Dev_3_Checking_hpp 1


#include "_Fwd.hpp"


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This file contains
// the Assert function template and some exception classes,
// all wrapped up in an internal namespace
// to ease importing in a project-specific namespace.
// 
// Thus, users should be able to write:
// 
//   Assert <FPrecond> (iArg >= 10);
// 
// instead of:
// 
//   Adder::Assert <Adder::FalsePrecondition> (iArg >= 10);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//#include <Adder/_Common.hpp>

#if ! ADDER_TARGET_DDK
 #include <stdexcept>
#endif


namespace Adder
{
namespace Checking
{


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Assertions:
// 
// See paragraph 24.3.7 in Bjarne Stroustrup's book
// ("The C++ Programming Language", 3rd Special Edition, 2000) !
// 
// Eg. of usage:
//   class MyException {};
//   Assert <MyException> (! ADDER_TARGET_DEBUG || x != 0);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if ! ADDER_TARGET_DDK

#if defined (__BORLANDC__)
 #pragma option push
 #pragma warn-ccc
 #pragma warn-rch
#endif
template <class Exception>
void
Assert
(bool bAssertion)
{
	//const unsigned iDebugLevel = 1;
	if (! bAssertion)
	{
		////if (iDebugLevel) std::tcout << TEXT (" [Assertion failed !]");
		//std::tcout.flush ();
		throw Exception ();
	}
}
#if defined (__BORLANDC__)
 #pragma option pop
#endif

#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Exception(s):
// 
// [xxx] TODO: Doc !
// 
// Note(s) (2010-12-18):
// 
//   FalseInvariant is meant to be used inside an internal, invariant-checking
//   member function of a class.
//   Somehow, neither FalsePrecondition, nor FalsePostcondition
//   seemed appropriate.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if ! ADDER_TARGET_DDK

#define __Dev_3_DefineExceptionClass(C, CParent) \
	class C \
		: public virtual CParent \
	{ \
		const char *what () const ADDER_NOTHROW ADDER_OVERRIDE \
		{ \
			return "Adder::" # C; \
		} \
	};


__Dev_3_DefineExceptionClass (Error             , std::exception)
__Dev_3_DefineExceptionClass (InternalError     , Error         )
__Dev_3_DefineExceptionClass (FalsePrecondition , Error         )
__Dev_3_DefineExceptionClass (FalsePostcondition, Error         )
__Dev_3_DefineExceptionClass (FalseInvariant    , Error         )
__Dev_3_DefineExceptionClass (NotImplemented    , Error         )
__Dev_3_DefineExceptionClass (Surprise          , Error         )

typedef InternalError      Internal;
typedef FalsePrecondition  FPrecond;
typedef FalsePostcondition FPostcond;
typedef FalseInvariant     FInvariant;
typedef NotImplemented     NotImpl;

#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


} // end namespace
} // end namespace


#endif
