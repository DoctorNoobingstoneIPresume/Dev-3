#ifndef __Dev_3_String2_hpp
#define __Dev_3_String2_hpp 1


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// ... [xxx] TODO: Doc !
// 
// Unit testing is done in:
// 
//   - "Tests2\Config\02 - String2";
//   - "Tests2\Config\05 - CharConverter".
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <Adder/_Common.hpp>

#include <algorithm>
#include <functional>
#include <locale>
// Let's not put stuff in namespace std right now, okay ? It might be bad...
//namespace std
//{
//	class locale;
//};


#include <cstring>


#ifdef __BORLANDC__
 #pragma option push
 #pragma warn-inl
#endif


namespace Adder
{
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// xis... and xto... functions:
// 
//   xisalnum
//   xisalpha
//   xiscntrl
//   xisdigit
//   xisgraph
//   xislower
//   xisprint
//   xispunct
//   xisspace
//   xisupper
//   xisxdigit
//   
//   xtolower
//   xtoupper
//   _xtolower
//   _xtoupper
// 
// The intent was for these to be "aliases" for:
// 
//   is... and isw...
//   to... and tow...
// 
// - overloads with the same name (for both the narrow and wide versions)
// to support generic code.
// 
// But they weren't locase-sensitive (or rather, they always used
// the global C locale).
// 
// These functions are deprecated. Instead, one should use the
// is... and to... functions from <locale>.
// 
// (_xtolower and _xtoupper are deprecated, with no replacement recommended;
// the functions they actually called - _tolower, etc. - weren't standard
// and they were seldomly used.)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// <cstring> functions:
// 
// These functions have two purposes:
// 
// - They provide common names for the narrow and wide versions of widely used
//   functions, thus supporting generic code
//   (eg: instead of calling strlen or wcslen, one call xstrlen which
//   dispatches to the right version at compile-time).
// 
// - They extend the <cstring> functions with:
// 
//   - "safer" versions of widely used functions
//     (eg: xstrncpy0, which _always_ ends the target string with a 0);
//   
//   - "handy" functions, such as xstr(n)cmpi.
// 
// Note that some of these "handy" functions have changed (from version 2.1).
// For example, the xstr(n)cmpi functions now take a locale parameter.
// Also, dispatching functions are now inline.
// 
// We are no longer defining the "b", "w" and "t" versions
// (for narrow, wide and "t" characters).
// The rules of selecting overloaded functions work much better.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

 //---------------------------------------------------------------------------
 // xstrlen:
 // 
 // = generic version of strlen.
 //---------------------------------------------------------------------------

template <class xchar>
std::size_t xstrlen (const xchar *pc)
{
	const xchar *pcCrt = pc;
	for (; *pcCrt; ++pcCrt);
	return pcCrt - pc;
}


template <>
inline
std::size_t xstrlen (const bchar *pc)
{
	return std::strlen (pc);
}


template <>
inline
std::size_t xstrlen (const wchar *pc)
{
	return std::wcslen (pc);
}


 //---------------------------------------------------------------------------
 // xstrnlen:
 // 
 // = generic version of strnlen.
 //---------------------------------------------------------------------------

template <class xchar>
std::size_t xstrnlen (const xchar *pc, std::size_t ccMax)
{
	std::size_t cc = 0;
	for (; cc < ccMax && *pc; ++cc, ++pc);
	return cc;
}


#if defined (_MSC_VER) && _MSC_VER >= 1400
template <>
inline
std::size_t xstrnlen (const bchar *pc, std::size_t ccMax)
{
	using namespace std; // In msvc-8.0, the function called below is not a member of std.
	return strnlen (pc, ccMax);
}


template <>
inline
std::size_t xstrnlen (const wchar *pc, std::size_t ccMax)
{
	using namespace std; // In msvc-8.0, the function called below is not a member of std.
	return wcsnlen (pc, ccMax);
}
#endif


 //---------------------------------------------------------------------------
 // xstrcpy:
 // 
 // = generic version of strcpy.
 //---------------------------------------------------------------------------

template <class xchar>
xchar *xstrcpy (xchar *pszDst, const xchar *pszSrc)
{
	while (*pszSrc)
		* (pszDst++) = * (pszSrc++);
	return pszDst;
}


template <>
inline
bchar *xstrcpy <bchar> (bchar *pszDst, const bchar *pszSrc)
{
	return std::strcpy (pszDst, pszSrc);
}


template <>
inline
wchar *xstrcpy <wchar> (wchar *pszDst, const wchar *pszSrc)
{
	return std::wcscpy (pszDst, pszSrc);
}


 //---------------------------------------------------------------------------
 // xstrncpy:
 // 
 // = generic version of strcpy0.
 //---------------------------------------------------------------------------

template <class xchar>
xchar *xstrncpy (xchar *pszDst, const xchar *pszSrc, std::size_t nmax)
{
	std::size_t i;
	for (i = 0; *pszSrc; ++i)
	{
		if (i >= nmax)
			return pszDst;
		* (pszDst++) = * (pszSrc++);
	}
	if (i < nmax)
		memset (pszDst, 0, (nmax - i) * sizeof (xchar));
	return pszDst;
}


template <>
inline
bchar *xstrncpy <bchar> (bchar *pszDst, const bchar *pszSrc, std::size_t nmax)
{
	return std::strncpy (pszDst, pszSrc, nmax);
}


template <>
inline
wchar *xstrncpy <wchar> (wchar *pszDst, const wchar *pszSrc, std::size_t nmax)
{
	return std::wcsncpy (pszDst, pszSrc, nmax);
}


 //---------------------------------------------------------------------------
 // xstrncpy0:
 // 
 // This function behaves like xstrncpy, except that it
 // almost _always_ ends the target string with a 0 (at position nmax - 1).
 // 
 // The only exception is the "degenerate" case when nmax is 0.
 //---------------------------------------------------------------------------

template <class xchar>
xchar *xstrncpy0 (xchar *pszDst, const xchar *pszSrc, std::size_t nmax)
{
	xchar *const pszRet = xstrncpy (pszDst, pszSrc, nmax);
	if (nmax)
		pszDst [nmax - 1] = 0;
	return pszRet;
}


 //---------------------------------------------------------------------------
 // xstrcat:
 // 
 // = generic version of strcat.
 //---------------------------------------------------------------------------

template <class xchar>
xchar *xstrcat (xchar *pszDst, const xchar *pszToAdd)
{
	return xstrcpy (pszDst + xstrlen (pszDst), pszToAdd);
}


template <>
inline
bchar *xstrcat (bchar *pszDst, const bchar *pszToAdd)
{
	return std::strcat (pszDst, pszToAdd);
}


template <>
inline
wchar *xstrcat <wchar> (wchar *pszDst, const wchar *pszToAdd)
{
	return std::wcscat (pszDst, pszToAdd);
}


 //---------------------------------------------------------------------------
 // xstrncat:
 // 
 // = generic version of strncat.
 //---------------------------------------------------------------------------

template <class xchar>
xchar *xstrncat (xchar *pszDst, const xchar *pszToAdd, std::size_t nmaxToAdd)
{
	return xstrncpy0 (pszDst + xstrlen (pszDst), pszToAdd, nmaxToAdd + 1);
}


template <>
inline
bchar *xstrncat <bchar> (bchar *pszDst, const bchar *pszToAdd, std::size_t nmaxToAdd)
{
	return std::strncat (pszDst, pszToAdd, nmaxToAdd);
}


template <>
inline
wchar *xstrncat <wchar> (wchar *pszDst, const wchar *pszToAdd, std::size_t nmaxToAdd)
{
	return std::wcsncat (pszDst, pszToAdd, nmaxToAdd);
}


 //---------------------------------------------------------------------------
 // xstrncat0:
 // 
 // xstrncat0 (s1, s2, n) functions just like xstrncat (s1, s2, n - 1)
 // for n > 0.
 // 
 // For n == 0, this function does nothing.
 //---------------------------------------------------------------------------

template <class xchar>
xchar *xstrncat0 (xchar *pszDst, const xchar *pszToAdd, std::size_t nmaxToAdd)
{
	return xstrncpy0 (pszDst + xstrlen (pszDst), pszToAdd, nmaxToAdd);
}


 //---------------------------------------------------------------------------
 // xstrncattotal0:
 // 
 // xstrncattotal0 (s1, s2, n) concatenates s2 to s1,
 // making sure that the resulted string (s1), including the null terminator,
 // does not exceed n characters.
 //---------------------------------------------------------------------------

template <class xchar>
xchar *xstrncattotal0 (xchar *pszDst, const xchar *pszToAdd, std::size_t nmaxFinal)
{
	const std::size_t nCrt = xstrlen (pszDst);
	if (nCrt < nmaxFinal)
		xstrncat0 (pszDst + nCrt, pszToAdd, nmaxFinal - nCrt);
	return pszDst;
}


 //---------------------------------------------------------------------------
 // xstrcmp:
 // 
 // = generic version of strcmp.
 //---------------------------------------------------------------------------

template <class xchar>
inline
int xstrcmp (const xchar *psz1, const xchar *psz2)
{
	int iRet;
	
	for (;; ++psz1, ++psz2)
	{
		if (iRet = *psz1 - *psz2)
			return iRet;
		if (! *psz1)
			return 0;
	}
}


template <>
inline
int xstrcmp <bchar> (const bchar *psz1, const bchar *psz2)
{
	//using namespace std;
	// [xxx] Borland complains about __strcmp__ not being part of std::... should fix nicer !
	// Fixed !
	return std::strcmp (psz1, psz2);
}


template <>
inline
int xstrcmp <wchar> (const wchar *psz1, const wchar *psz2)
{
	return std::wcscmp (psz1, psz2);
}


 //---------------------------------------------------------------------------
 // xstrcmpi:
 // 
 // This function behaves like xstrcmp, except that the comparison of strings
 // is case-insensitive.
 // 
 // This function is locale-sensitive.
 // The header <locale> must be included in order to call this function.
 // 
 // Implementation note:
 // 
 //   Instead of defining a single version with a default value
 //   for the "loc" argument, we are defining two versions.
 //   The effect is the same, but we are no longer including <locale>
 //   (a forward declaration of std::locale will suffice).
 //   Many programs will not call this function, and including <locale>
 //   might unnecessarily slow down compilation.
 //   
 //   ... but... how can one provide a forward declaration of std::locale ?
 //   Item 37 of Herb Sutter's "More Exceptional C++" says that:
 //   
 //     "It is undefined for a C++ program to add declarations or definitions
 //     to namespace std or namespaces within namespace std
 //     unless otherwise specified."
 //   
 //   Does the Standard "otherwise specify" anything in this case ?
 //   
 //   Until this is clarified, we are including <locale>...
 //---------------------------------------------------------------------------

template <class xchar>
int xstrcmpi (const xchar *psz1, const xchar *psz2, const std::ctype <xchar> &ctype)
{
	for (;; ++psz1, ++psz2)
	{
		// [2016-08-29]
		//const int iRet = std::tolower (xchar (*psz1), loc) - std::tolower (xchar (*psz2), loc);
		const int iRet = ctype.tolower (xchar (*psz1))     - ctype.tolower (xchar (*psz2));
			// xchar (*psz...) is a work-around for Borland:
			// ctype is specialized for xchar, not for const xchar. :(
		if (iRet)
			return iRet;
		if (! *psz1)
			return 0;
	}
}

template <class xchar>
int xstrcmpi (const xchar *psz1, const xchar *psz2, const std::locale &loc)
{
	return xstrcmpi (psz1, psz2, std::use_facet <std::ctype <xchar> > (loc));
}


template <class xchar>
int xstrcmpi (const xchar *psz1, const xchar *psz2)
{
	return xstrcmpi (psz1, psz2, std::locale ());
}


 //---------------------------------------------------------------------------
 // xstrncmp:
 // 
 // = generic version of strncmp.
 //---------------------------------------------------------------------------

template <class xchar>
int xstrncmp (const xchar *psz1, const xchar *psz2, std::size_t nmax)
{
	for (std::size_t i = 0; i < nmax; ++i, ++psz1, ++psz2)
	{
		const int iRet = *psz1 - *psz2;
		if (iRet)
			return iRet;
		if (! *psz1)
			return 0;
	}
	return 0;
}


template <>
inline
int xstrncmp <bchar> (const bchar *psz1, const bchar *psz2, std::size_t nmax)
{
	return std::strncmp (psz1, psz2, nmax);
}


template <>
inline
int xstrncmp <wchar> (const wchar *psz1, const wchar *psz2, std::size_t nmax)
{
	return std::wcsncmp (psz1, psz2, nmax);
}


 //---------------------------------------------------------------------------
 // xstrncmpi:
 // 
 // This function behaves like xstrncmp, except that the comparison of strings
 // is case-insensitive.
 // 
 // This function is locale-sensitive.
 // The header <locale> must be included in order to call this function.
 // 
 // Please see the notes for xstrcmpi above.
 //---------------------------------------------------------------------------

template <class xchar>
int xstrncmpi (const xchar *psz1, const xchar *psz2, std::size_t nmax, const std::ctype <xchar> &ctype)
{
	for (std::size_t i = 0; i < nmax; ++i, ++psz1, ++psz2)
	{
		// [2016-08-29]
		//const int iRet = std::tolower (xchar (*psz1), loc) - std::tolower (xchar (*psz2), loc);
		const int iRet = ctype.tolower (xchar (*psz1))     - ctype.tolower (xchar (*psz2));
		if (iRet)
			return iRet;
		if (! *psz1)
			return 0;
	}
	return 0;
}

template <class xchar>
int xstrncmpi (const xchar *psz1, const xchar *psz2, std::size_t nmax, const std::locale &loc)
{
	return xstrncmpi (psz1, psz2, nmax, std::use_facet <std::ctype <xchar> > (loc));
}


template <class xchar>
int xstrncmpi (const xchar *psz1, const xchar *psz2, std::size_t nmax)
{
	return xstrncmpi (psz1, psz2, nmax, std::locale ());
}


 //---------------------------------------------------------------------------
 // xstrstr:
 // 
 // = generic version of strcmp.
 // 
 // [xxx] Note(s):
 // 
 //   No generic version written, just specializations for bchar and wchar.
 //---------------------------------------------------------------------------

template <class xchar>
inline
const xchar *xstrstr (const xchar *psz1, const xchar *psz2);


template <>
inline
const bchar *xstrstr <bchar> (const bchar *psz1, const bchar *psz2)
{
	//using namespace std;
	// [xxx] Borland complains about __strcmp__ not being part of std::... should fix nicer !
	// Fixed !
	return std::strstr (psz1, psz2);
}


template <>
inline
const wchar *xstrstr <wchar> (const wchar *psz1, const wchar *psz2)
{
	return std::wcsstr (psz1, psz2);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// CharConverter:
// 
// This is a unary_function that converts a character
// (eg: char to char, char to wchar_t, wchar_t to char, wchar_t to char)
// - according to a ctype facet (eg: from a locale).
// 
// One can use std::transform with an argument of this type
// to convert one string to another, eg:
// 
//   wstring s1 (L"Bla bla bla");
//   string s2;
//   transform (s1.begin (), s2.begin (), back_inserter (s2),
//              CharConverter <wchar_t, char> (locale ()));
//   
//   ... // Now s2 is s1 converted to narrow characters,
//       // using the widen/narrow functions of the ctype <wchar_t> facet
//       // of the global locale.
// 
// Template argument(s):
// 
//   CharSrc = source character type;
//   CharDst = target character type.
// 
//   These types must conform to the requirements of "characters",
//   as stated in Paragraph 21-1 of the C++ Standard ('98 or '03):
//   "[...] characters may be of any POD (3.9) type".
// 
// Implementation note:
// 
//   For the <char, wchar_t> and <wchar_t, char> specializations,
//   we actually DO use the supplied locale object
//   and we "cache" a pointer to the contained ctype <wchar_t> facet.
//   
//   We shouldn't really worry about copying/assigning these specializations,
//   because the (references to) the facets (obtained with use_locale)
//   remain "valid at least as long as any copy of loc exists"
//   (C++ '98/'03 Standard, Section 22.1.2).
//   
//   However, I find that too fragile to trust (on some implementations),
//   and since the effort of assuring correct semantics of copy and assignment
//   is small, I take extra care here...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

 //---------------------------------------------------------------------------
 // Generic version
 // (used for <char, char> and <wchar_t, wchar_t>, among other combinations):
 //---------------------------------------------------------------------------

template <typename CharSrc, typename CharDst>
class CharConverter
	: public std::unary_function <CharSrc, CharDst>
{
 public:
	typedef CharSrc argument_type;
	typedef CharDst result_type;
	
	
	CharConverter
	()
	{}
	
	
	explicit
	CharConverter
	(const std::locale &, CharDst = CharDst ('?'))
	{}
	
	
	CharDst
	operator
	()
	(CharSrc c)
	const
	{
		return CharDst (c);
	}
	
	
};


 //---------------------------------------------------------------------------
 // Full specialization (char to wchar_t):
 //---------------------------------------------------------------------------

template <>
class CharConverter <bchar, wchar>
	: public std::unary_function <bchar, wchar>
{
 private:
	      std::locale         _loc;
	const std::ctype <wchar> *_pFacet;
	
 public:
	typedef bchar argument_type;
	typedef wchar result_type;
	
	explicit
	CharConverter
	(const std::locale &locA = std::locale (), wchar = WTEXT ('?'))
		: _loc    (locA)
		, _pFacet (&std::use_facet <std::ctype <wchar> > (_loc))
	{}
	
	CharConverter
	(const CharConverter &rhs)
		: _loc    (rhs._loc)
		, _pFacet (&std::use_facet <std::ctype <wchar> > (_loc))
	{}
	
	CharConverter &
	operator=
	(const CharConverter &rhs)
	{
		_loc    = rhs._loc;
		_pFacet = &std::use_facet <std::ctype <wchar> > (_loc);
		return *this;
	}
	
	wchar
	operator
	()
	(bchar c)
	const
	{
		return _pFacet->widen (c);
	}
};


 //---------------------------------------------------------------------------
 // Full specialization (wchar_t to char):
 //---------------------------------------------------------------------------

template <>
class CharConverter <wchar, bchar>
	: public std::unary_function <wchar, bchar>
{
 private:
	      std::locale         _loc;
	const std::ctype <wchar> *_pFacet;
	      bchar               _cDef;
	
 public:
	typedef wchar argument_type;
	typedef bchar result_type;
	
	explicit
	CharConverter
	(const std::locale &locA = std::locale (), bchar cDefaultA = BTEXT ('?'))
		: _loc    (locA)
		, _pFacet (&std::use_facet <std::ctype <wchar> > (_loc))
		, _cDef   (cDefaultA)
	{}
	
	CharConverter
	(const CharConverter &rhs)
		: _loc    (rhs._loc)
		, _pFacet (&std::use_facet <std::ctype <wchar> > (_loc))
		, _cDef   (rhs._cDef)
	{}
	
	CharConverter &
	operator=
	(const CharConverter &rhs)
	{
		_loc    = rhs._loc;
		_pFacet = &std::use_facet <std::ctype <wchar> > (_loc);
		_cDef   = rhs._cDef;
		return *this;
	}
	
	bchar
	operator
	()
	(wchar c)
	const
	{
		return _pFacet->narrow (c, _cDef);
	}
};


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// StringConverter:
// 
// This is a unary function that converts a string type to another string type
// using the CharConverter defined above.
// 
// It also provides some handy "Convert" functions, which can be used instead
// of operator () for greater efficiency.
// 
// Template argument(s):
// 
//   StringSrc: the source string type;
//   StringDst: the target string type.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <class StringSrc, class StringDst>
class StringConverter
	: public std::unary_function <StringSrc, StringDst>
{
 public:
	typedef StringSrc argument_type;
	typedef StringDst result_type;
	
	// In order to allow msvc-6.0 to parse this template,
	// we avoid writing "typename T::x_type", where T is a template argument;
	// instead, we use "typename T::x_type", where T is a here-defined type.
	typedef typename argument_type::value_type CharSrc;
	typedef typename result_type  ::value_type CharDst;

 private:
	const std::locale &_loc;
	      CharDst      _cDef;
	
 public:
	explicit
	StringConverter
	(const std::locale &locA = std::locale (), CharDst cDefaultA = CharDst ('?'))
		: _loc  (locA)
		, _cDef (cDefaultA)
	{}
	
	void
	Convert
	(const StringSrc &sSrc, StringDst *psDst)
	const
	{
		StringDst sDstTemp;
		sDstTemp.resize (sSrc.size ());
		std::transform
		(
			sSrc.begin (),
			sSrc.end (),
			sDstTemp.begin (),
			CharConverter <CharSrc, CharDst> (_loc, _cDef)
		);
		psDst->swap (sDstTemp);
	}
	
	void
	Convert
	(const CharSrc *pszSrc, StringDst *psDst)
	const
	{
		const std::size_t n = xstrlen (pszSrc);
		StringDst sDstTemp (n, CharDst ('#'));
		sDstTemp.resize (n);
		std::transform
		(
			pszSrc,
			pszSrc + n,
			sDstTemp.begin (),
			CharConverter <CharSrc, CharDst> (_loc, _cDef)
		);
		psDst->swap (sDstTemp);
	}
	
	template <class FwdIterSrc>
	void
	Convert
	(FwdIterSrc iterSrcBeg, FwdIterSrc iterSrcEnd, StringDst *psDst)
	const
	{
		const std::size_t n = std::distance (iterSrcBeg, iterSrcEnd);
		StringDst sDstTemp (n, CharDst ('#'));
		std::transform
		(
			iterSrcBeg,
			iterSrcEnd,
			sDstTemp.begin (),
			CharConverter <CharSrc, CharDst> (_loc, _cDef)
		);
		psDst->swap (sDstTemp);
	}
	
	StringDst
	operator()
	(const StringSrc &sSrc)
	const
	{
		StringDst sDstTemp (sSrc.size (), CharDst ('#')); // [xxx] TODO: Consider this line when I'm less sleepy !
		Convert (sSrc, &sDstTemp);
		return sDstTemp;
	}
};


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// MakeBString, MakeWString, MakeTString:
// 
// Given a string (either a basic_string or a null-terminated string)
// of char/wchar_t elements,
// 
// - MakeBString creates a basic_string <char>;
// - MakeWString creates a basic_string <wchar_t>.
// 
// MakeTString stands for either MakeBString or MakeWString,
// depending on ADDER_TARGET_UNICODE.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

 //---------------------------------------------------------------------------
 // MakeBString:
 //---------------------------------------------------------------------------

template <class CharSrc, class TraitsSrc, class AllocSrc>
bstring
MakeBString
(
	const std::basic_string <CharSrc, TraitsSrc, AllocSrc> &sSrc,
	const std::locale                                      &loc,      //= std::locale (),
	      bchar                                             cDefaultA = BTEXT ('?')
)
{
	typedef std::basic_string <CharSrc, TraitsSrc, AllocSrc> StringSrc;
	
	bstring sDstTemp;
	StringConverter <StringSrc, bstring> (loc, cDefaultA).Convert (sSrc, &sDstTemp);
	return sDstTemp;
}

template <class CharSrc>
bstring
MakeBString
(
	const CharSrc     *pszSrc,
	const std::locale &loc       = std::locale (),
	      bchar        cDefaultA = BTEXT ('?')
)
{
	typedef std::basic_string <CharSrc> StringSrc;
	
	bstring sDstTemp;
	StringConverter <StringSrc, bstring> (loc, cDefaultA).Convert (pszSrc, &sDstTemp);
	return sDstTemp;
}

template <class CharSrc, class TraitsSrc, class AllocSrc>
bstring
MakeBString
(
	const std::basic_string <CharSrc, TraitsSrc, AllocSrc> &sSrc
)
{
	return MakeBString (sSrc, std::locale ());
}

inline
bstring
MakeBString
(
	const bstring &sSrc
)
{
	return sSrc;
}


 //---------------------------------------------------------------------------
 // MakeWString:
 //---------------------------------------------------------------------------

template <class CharSrc, class TraitsSrc, class AllocSrc>
wstring
MakeWString
(
	const std::basic_string <CharSrc, TraitsSrc, AllocSrc> &sSrc,
	const std::locale                                      &loc,      //= std::locale (),
	      wchar                                             cDefaultA = WTEXT ('?')
)
{
	typedef std::basic_string <CharSrc, TraitsSrc, AllocSrc> StringSrc;
	
	wstring sDstTemp;
	StringConverter <StringSrc, wstring> (loc, cDefaultA).Convert (sSrc, &sDstTemp);
	return sDstTemp;
}

template <class CharSrc>
wstring
MakeWString
(
	const CharSrc     *pszSrc,
	const std::locale &loc       = std::locale (),
	      wchar        cDefaultA = WTEXT ('?')
)
{
	typedef std::basic_string <CharSrc> StringSrc;
	
	wstring sDstTemp;
	StringConverter <StringSrc, wstring> (loc, cDefaultA).Convert (pszSrc, &sDstTemp);
	return sDstTemp;
}


template <class CharSrc, class TraitsSrc, class AllocSrc>
wstring
MakeWString
(
	const std::basic_string <CharSrc, TraitsSrc, AllocSrc> &sSrc
)
{
	return MakeWString (sSrc, std::locale ());
}


inline
wstring MakeWString
(
	const wstring &sSrc
)
{
	return sSrc;
}


 //---------------------------------------------------------------------------
 // MakeTString:
 //---------------------------------------------------------------------------

#if ! ADDER_TARGET_UNICODE

template <class CharSrc, class TraitsSrc, class AllocSrc>
bstring
MakeTString
(
	const std::basic_string <CharSrc, TraitsSrc, AllocSrc> &sSrc,
	const std::locale                                      &loc,      //= std::locale (),
	      bchar                                             cDefaultA = BTEXT ('?')
)
{
	return MakeBString (sSrc, loc, cDefaultA);
}

template <class CharSrc>
bstring
MakeTString
(
	const CharSrc     *pszSrc,
	const std::locale &loc       = std::locale (),
	      bchar        cDefaultA = BTEXT ('?')
)
{
	return MakeBString (pszSrc, loc, cDefaultA);
}

template <class CharSrc, class TraitsSrc, class AllocSrc>
bstring
MakeTString
(
	const std::basic_string <CharSrc, TraitsSrc, AllocSrc> &sSrc
)
{
	return MakeBString (sSrc);
}

inline
bstring
MakeTString
(
	const bstring &sSrc
)
{
	return sSrc;
}


#else

template <class CharSrc, class TraitsSrc, class AllocSrc>
wstring
MakeTString
(
	const std::basic_string <CharSrc, TraitsSrc, AllocSrc> &sSrc,
	const std::locale                                      &loc,      //= std::locale (),
	      wchar                                             cDefaultA = WTEXT ('?')
)
{
	return MakeWString (sSrc, loc, cDefaultA);
}

template <class CharSrc>
wstring
MakeTString
(
	const CharSrc     *pszSrc,
	const std::locale &loc       = std::locale (),
	      wchar        cDefaultA = WTEXT ('?')
)
{
	return MakeWString (pszSrc, loc, cDefaultA);
}

template <class CharSrc, class TraitsSrc, class AllocSrc>
wstring
MakeTString
(
	const std::basic_string <CharSrc, TraitsSrc, AllocSrc> &sSrc
)
{
	return MakeWString (sSrc);
}

inline
wstring
MakeTString
(
	const wstring &sSrc
)
{
	return sSrc;
}


#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// [2013-12-29]
//   We change the return type from byte to unsigned.
//   We guarantee that the result fits into a byte.
//   It will be in the range [0..36) or [0..16) for valid input
//   or outside that range for invalid input.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

unsigned
Base36Digit_To_Value
(unsigned c);


unsigned
HexDigit_To_Value
(unsigned c);


inline
unsigned
HexDigitToValue
(unsigned c)
{
	return HexDigit_To_Value (c);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// [2013-12-28]
//   The time has come for yet another implementation of std::atoi ! (-:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/*
template <typename FwdIter>
bool
StringRange_To_uint64_t
(
	const FwdIter  &iterBeg,
	const FwdIter  &iterEnd,
	      uint64_t *py,
	      unsigned  iBase   = 0
)
{
	Assert <FPrecond> (iBase != 1 && iBase <= 36);
	
	uint64_t y = 0;
	{
		std::locale loc;
		
		unsigned iAnything = 0;
		
		for (FwdIter iter = iterBeg; iter != iterEnd; ++iter)
		{
			const unsigned c = *iter;
			if (c >= 0x80) return false;
			
			for (bool bProcessed = false; ! bProcessed; )
			{
				bProcessed = true;
				
				if (! iAnything)
				{
					if (std::isspace (char (c), loc) || c == '+')
						;
					else
					if (c == '0')
					{
						if (! iBase)
							iAnything = 1;
						else
							iAnything = 2;
					}
					else
						{ iAnything = 2; bProcessed = false; }
				}
				else
				if (iAnything == 1)
				{
					if (std::isspace (char (c)))
						break;
					else
					if (c == 'x' || c == 'X')
					{
						if (! iBase)
							{ iBase = 16; iAnything = 2; }
						else
							{             iAnything = 2; bProcessed = false; }
					}
					else
					{
						if (! iBase)
							iBase = 10;
						
						if (true)
							{             iAnything = 2; bProcessed = false; }
					}
				}
				else
				//if (iAnything == 2)
				{
					unsigned bValue = Base36Digit_To_Value (c);
					
					if (bValue >= iBase)
				}
				//else
				//	throw InternalError ();
			}
			
		
		}
	}
	
	if (py) *py = y;
	return true;
}
*/


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


} // end namespace


#ifdef __BORLANDC__
 #pragma option pop
#endif


#endif
