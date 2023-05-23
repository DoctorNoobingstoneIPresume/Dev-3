#ifndef __Dev_3__Common_hpp
#define __Dev_3__Common_hpp 1


#include "Checking.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <string>
#include <vector>
#include <deque>


#if defined (_WIN32)
	#if ! ADDER_TARGET_DDK
		#include <windows.h>
		#include <tchar.h>
	#else
		#include <windef.h>
	#endif
#endif


namespace Adder
{


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Integral types:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

using namespace Checking;


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Integral types:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

typedef char                 bchar;
typedef wchar_t              wchar;

// We define these to be compatible with Windows BYTE, WORD, DWORD...
// Later edit: We are now using the types in CStdInt.hpp,
// which are not always compatible with Windows BYTE, WORD, DWORD (so what ?)...

//typedef unsigned __int8       byte;
//typedef unsigned __int16      word;
//typedef unsigned __int32     dword;
//typedef unsigned __int64     qword;

//typedef unsigned char        byte;
//typedef unsigned short       word;
//typedef unsigned long        dword;
//typedef unsigned __int64     qword;

typedef uint8_t              byte;
typedef uint16_t             word;
typedef uint32_t             dword;
typedef uint64_t             qword;

typedef unsigned char        uchar;
//typedef unsigned char        ubchar;
//typedef unsigned wchar_t     uwchar;
typedef unsigned short       ushort;
typedef unsigned int         uint;
typedef unsigned long        ulong;


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// BTEXT, WTEXT (macros):
// 
// BTEXT ('c')       is the same as  'x'
// BTEXT ("string")  is the same as  "string"
// WTEXT ('c')       is the same as  L'x'
// WTEXT ("string")  is the same as  L"string"
// 
// These are provided for "consistency" with:
// 
// TEXT  ('c')       which means     'c' or L'c'
// TEXT  ("string")  which means     "string" or L"string"
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define BTEXT(x)             x
#define WTEXT(x)             L##x


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// XTEXT (macro):
// 
// XTEXT (type, 'c')       is  'c'      or L'c'
// XTEXT (type, "string")  is  "string" or L"string"
// 
// according to whether type is char or wchar_t.
// 
// Example of usage:
// 
//   template <class char_type>
//   void MyFunction (char_type *pc)
//   {
//     *pc = XTEXT (char_type, 'q');
//   }
// 
// 
// I hate macro's !
// 
// DMC needs a special version... because of a problem with casts and
// conditional expressions:
// 
//      int main ()
//      {
//        double *a = reinterpret_cast <double *>
//        (
//          1 == 2
//          ?
//          reinterpret_cast <float *> (0)
//          :
//          reinterpret_cast <float *> (0)
//        );
//        return 0;
//      }
// 
// The version I could come up with for DMC does unfortunately include both
// the ANSI version and the UNICODE version of the string in the program...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


#if ! defined (__DMC__)
	#define XTEXT(type, text) \
		( \
			static_cast <const type *> \
			( \
				sizeof (type) == 1 \
				? \
				static_cast <const void *> (BTEXT (text)) \
				: \
				static_cast <const void *> (WTEXT (text)) \
			) \
		)
#else
	template <class XChar>
	inline
	const XChar *__ReturnBOrWString (const Adder::bchar *pbsz, const Adder::wchar *pwsz);
	
	template <>
	inline
	const Adder::bchar *__ReturnBOrWString <Adder::bchar> (const Adder::bchar *pbsz, const Adder::wchar *pwsz)
	{
		return pbsz;
	}
	
	template <>
	inline
	const Adder::wchar *__ReturnBOrWString <Adder::wchar> (const Adder::bchar *pbsz, const Adder::wchar *pwsz)
	{
		return pwsz;
	}
	
	#define XTEXT(Char, Text) Adder::__ReturnBOrWString <Char> (BTEXT (Text), WTEXT (Text))
#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// bcout, bcin, bcerr:
// 
// These are synonims for cout, cin, cerr;
// they are provided to match the number of characters in wcout, wcin, wcerr.
// Ha-ha.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#define bcout cout
#define bcin  cin
#define bcerr cerr


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// xchar_tag, xcout:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <class xchar>
class xchar_tag
{
	typedef xchar char_type;
};

#if ! ADDER_TARGET_DDK
template <class xchar>
inline
std::basic_ostream <xchar> &xcout (xchar_tag <xchar>);

template <>
inline
std::basic_ostream <bchar> &xcout (xchar_tag <bchar>)
{
	return std::bcout;
}

template <>
inline
std::basic_ostream <wchar> &xcout (xchar_tag <wchar>)
{
	return std::wcout;
}
#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// TEXT, tcout, tcin, tcerr, tchar:
// 
// TEXT is defined as specified in the documentation for BTEXT/WTEXT above.
// 
// tchar is defined as bchar or wchar,
// tcout is defined as bcout or wcout,
// tcin  is defined as bcin  or wcin ,
// tcerr is defined as bcerr or wcerr
// 
// depending on ADDER_TARGET_TCHAR.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if ! ADDER_TARGET_UNICODE
	typedef bchar tchar;
	#ifndef __TEXT
		#define __TEXT(quote) quote
	#endif
	#define tcout bcout
	#define tcin  bcin
	#define tcerr bcerr
#else
	typedef wchar tchar;
	#ifndef __TEXT
		#define __TEXT(quote) L##quote
	#endif
	#define tcout wcout
	#define tcin  wcin
	#define tcerr wcerr
#endif

#ifndef TEXT
	#define TEXT(x) __TEXT(x)
#endif


} // Temporarly end namespace
#if ! defined (_TCHAR_DEFINED)
 typedef Adder::tchar TCHAR;
#endif
namespace Adder
{


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Some ANSI/UNICODE defines:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

typedef std::basic_string        <bchar>             bstring;
typedef std::basic_string        <wchar>             wstring;
typedef std::basic_string        <tchar>             tstring;

typedef std::basic_istream       <bchar>            bistream;
typedef std::basic_istream       <wchar>            wistream;
typedef std::basic_istream       <tchar>            tistream;

typedef std::basic_ostream       <bchar>            bostream;
typedef std::basic_ostream       <wchar>            wostream;
typedef std::basic_ostream       <tchar>            tostream;

typedef std::basic_iostream      <bchar>           biostream;
typedef std::basic_iostream      <wchar>           wiostream;	
typedef std::basic_iostream      <tchar>           tiostream;

typedef std::basic_istringstream <bchar>      bistringstream;
typedef std::basic_istringstream <wchar>      wistringstream;
typedef std::basic_istringstream <tchar>      tistringstream;

typedef std::basic_ostringstream <bchar>      bostringstream;
typedef std::basic_ostringstream <wchar>      wostringstream;
typedef std::basic_ostringstream <tchar>      tostringstream;

typedef std::basic_stringstream  <bchar>       bstringstream;
typedef std::basic_stringstream  <wchar>       wstringstream;	
typedef std::basic_stringstream  <tchar>       tstringstream;

typedef std::basic_ifstream      <bchar>           bifstream;
typedef std::basic_ifstream      <wchar>           wifstream;
typedef std::basic_ifstream      <tchar>           tifstream;

typedef std::basic_ofstream      <bchar>           bofstream;
typedef std::basic_ofstream      <wchar>           wofstream;
typedef std::basic_ofstream      <tchar>           tofstream;

typedef std::basic_fstream       <bchar>            bfstream;
typedef std::basic_fstream       <wchar>            wfstream;	
typedef std::basic_fstream       <tchar>            tfstream;


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// DummyOutIter:
// 
// [xxx] Note(s) (2012-02-29 - Happy Birthday, Christina !):
//   This fresh breath of code was added today !
// 
// This type is a model of the "Output Iterator" concept.
// An object of this type
// supports pre-increment, post-increment and dereference assignment.
// All these are no-op's, though ! ^^
// 
// Usage:
// 
//   In many situations, an algorithm produces output
//   that, traditionally, was written via a (user-supplied) output iterator.
//   
//   However, to support efficient (exact) memory pre-allocation,
//   the algorithm might allow the user to obtain
//   the exact number of output elements via an optional reference parameter
//   (e.g. a pointer to std::size_t).
//   
//   In such cases, the user will probably want to call the algorithm twice:
//   
//     - once with a dummy output iterator
//       and a reference (e.g. a pointer) to a variable
//       that receives the number of elements;
//     
//     - again with the actual output iterator (e.g. a back_insert_iterator).
//   
//   In between the calls, the user calls reserve for the output sequence.
//   
//   Even better, the user might check out the bHasReserve value
//   of the SeqReserver <...> class template
//   and call the algorithm once or twice, accordingly.
//   
//   As far as I my humble experience currently goes,
//   this is the price to pay for genericity and efficiency.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class DummyOutIterProxy
{
 public:
	template <typename T>
	DummyOutIterProxy &operator= (const T &t)
	{
		static_cast <void> (t);
		return *this;
	}
};


class DummyOutIter
{
 public:
	DummyOutIterProxy operator* () const
	{
		return DummyOutIterProxy ();
	}
	
	DummyOutIter &operator++ ()
	{
		return *this;
	}
	
	DummyOutIter operator++ (int)
	{
		DummyOutIter tmp (*this);
		++ (*this);
		return tmp;
	}
};


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// "new" & "delete" using specified allocators:
// 
// [2020-08-29] TODO: Forwarding references...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <class A, typename Value>
#if ! ADDER_CXX11_RVALUE_REFERENCES_V
 typename A::pointer NewViaAllocator (A   a, const Value  &t = Value ())
#else
 typename A::pointer NewViaAllocator (A &&a,       Value &&t = Value ())
#endif
{
	const typename A::pointer p = a.allocate (1);
	
	// [2020-08-29] TODO: RAII object (with "release" feature) instead of try-catch block, to preserve throw point if there is no handler.
	try
	{
		a.construct (p, t);
	}
	catch (...)
	{
		a.deallocate (p, 1);
		throw;
	}
	
	return p;
}

template <class A, typename Pointer>
#if ! ADDER_CXX11_RVALUE_REFERENCES_V
 void DeleteViaAllocator (A   a, const Pointer   p)
#else
 void DeleteViaAllocator (A &&a,       Pointer &&p)
#endif
{
	a.destroy    (p);
	a.deallocate (p, 1);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// "auto_ptr" using specified allocator:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <class A, typename Value>
class AutoPtrViaAllocator
{
 private:
	template <class A2, typename Value2>
	class AutoPtrViaAllocatorRef
	{
	 public:
		AutoPtrViaAllocator <A2, Value2> &_ap;
		
		AutoPtrViaAllocatorRef (AutoPtrViaAllocator <A2, Value2> &ap)
			: _ap (ap)
		{}
	};


 private:
	typedef
		typename A::pointer
		pointer;
	
	      pointer                      _p;
	      A                            _a;


 public:
	typedef Value                           element_type;
	typedef AutoPtrViaAllocator <A, Value>  MyAutoPtrViaAllocator;
	
	
	explicit
	AutoPtrViaAllocator
	#if ! ADDER_CXX11_RVALUE_REFERENCES_V
	 (Value *p = 0, const A  &a = A ())
	#else
	 (Value *p = 0,       A &&a = A ())
	#endif
	ADDER_NOTHROW
		: _p (p)
		, _a (a)
	{}
	
	
	AutoPtrViaAllocator
	(AutoPtrViaAllocator <A, Value> &ap)
	ADDER_NOTHROW
		: _p (ap.release ())
		, _a (ap._a)
	{};
	
	
	AutoPtrViaAllocator &
	operator=
	(AutoPtrViaAllocator &ap)
	ADDER_NOTHROW
	{
		reset (ap.release ());
		return *this;
	}
	
	
	#if ! defined (_MSC_VER) || _MSC_VER >= 1310
	template <class A2, typename Value2>
	AutoPtrViaAllocator
	(AutoPtrViaAllocator <A2, Value2> &ap)
	ADDER_NOTHROW
		: _p (ap.release ())
		, _a (A ())
	{}
	
	
	template <class A2, typename Value2>
	AutoPtrViaAllocator &
	operator=
	(AutoPtrViaAllocator <A2, Value2> &ap)
	ADDER_NOTHROW
	{
		reset (ap.release ());
		return *this;
	}
	#endif
	
	
	~AutoPtrViaAllocator
	()
	ADDER_NOTHROW
	{
		if (_p)
			DeleteViaAllocator (_a, _p);
	}
	
	
	Value &
	operator*
	()
	const
	ADDER_NOTHROW
	{
		return *_p;
	}
	
	
	Value *
	operator->
	()
	const
	ADDER_NOTHROW
	{
		return _p;
	}
	
	
	Value *
	get
	()
	const
	ADDER_NOTHROW
	{
		return _p;
	}
	
	
	Value *
	release
	()
	ADDER_NOTHROW
	{
		Value *const pTmp = _p;
		_p = 0;
		return pTmp;
	}
	
	
	#if ! defined (_MSC_VER) || _MSC_VER >= 1310
	void
	reset
	(Value &p = 0)
	ADDER_NOTHROW
	{
		if (_p != p)
		{
			DeleteViaAllocator (_a, _p);
			_p = p;
		}
	}
	#endif
	
	
	AutoPtrViaAllocator
	(AutoPtrViaAllocatorRef <A, Value> &rap)
	ADDER_NOTHROW
	{
		reset (rap._ap.release ());
	}
	
	
	template <class A2, typename Value2>
	operator AutoPtrViaAllocatorRef <A2, Value2>
	()
	ADDER_NOTHROW
	{
		return AutoPtrViaAllocatorRef <A2, Value2> (*this);
	}
	
	
	template <class A2, typename Value2>
	operator AutoPtrViaAllocator <A2, Value2>
	()
	ADDER_NOTHROW
	{
		AutoPtrViaAllocator <A2, Value2> apTmp;
		apTmp.reset (release ());
		return apTmp;
	}	
};


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// SaveFmtFlags:
// 
// The constructor saves the formatting flags for a stream; the destructor
// restores them; eg:
// 
//     {
//         const SaveFmtFlags <char> savefmt (&std::cout);
//         std::cout << std::hex << 1024;     // Displays "400" (1024 in hex)
//     }
//     std::cout << 1024;                     // Displays "1024"
// 
// Sometimes you only want to do this in debug builds; you can specify an
// additional argument with the value "false" to completely disable
// SaveFmtFlags' functionality:
// 
//         const SaveFmtFlags <char> savefmt (&std::cout, bDebugBuild);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if ! ADDER_TARGET_DDK
template <typename Char, class Traits = std::char_traits <Char> >
class SaveFmtFlags
{
 private:
	//      std::ios_base                    *_pfile;
	      std::basic_ios <Char, Traits>    *_pfile;
	      std::ios_base::fmtflags           _flags;
	      Char                              _cFill;

 private:
	SaveFmtFlags
	(const SaveFmtFlags &);
	
	SaveFmtFlags &
	operator=
	(const SaveFmtFlags &);

 public:
	explicit
	SaveFmtFlags
	(std::basic_ios <Char, Traits> *pfileA, bool bReallyDoIt = true)
	{
		if (bReallyDoIt)
		{
			_pfile = pfileA;
			_flags = _pfile->flags ();
			_cFill = _pfile->fill  ();
		}
		else
			_pfile = 0;
	}
	
	~SaveFmtFlags
	()
	{
		if (_pfile)
		{
			_pfile->flags (_flags);
			_pfile->fill  (_cFill);
		}
	}
};
#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// SyncWithStdio:
// 
// This is useful when combining <iostream> console output
// with <cstdio> console output.
// 
// Usage:
// 
// void MyFunction ()
// {
//   const SyncWithStdio syncio;
//   std::cout << "Hello, world !";
// }
// 
// void MyOldFunction ()
// {
//   const SyncWithStdio syncio;
//   std::printf ("Hello, old world !");
// }
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if ! ADDER_TARGET_DDK
class SyncWithStdio
{
 private:
	SyncWithStdio
	(const SyncWithStdio &);
	
	SyncWithStdio &
	operator=
	(const SyncWithStdio &);

 public:
	explicit
	SyncWithStdio
	(bool bReallyDoIt = true)
		: _bReallyDoIt (bReallyDoIt)
	{
		FlushEm ();
	}
	
	~SyncWithStdio
	()
	{
		FlushEm ();
	}
	
	void
	FlushEm
	()
	{
		if (_bReallyDoIt)
		{
			using namespace std;
				// Borland_551: stdout is #define'd as &_streams [0] or something.
				// Visual_60  : fflush is not a member of std.
				//              (later edit: we made it be a member of std, using _Config.hpp.)
			std::fflush (stdout);
			std::bcout.flush ();
			std::wcout.flush ();
		}
	}
	
 private:
 	bool _bReallyDoIt;
};
#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Quick'n'Dirty Hex Dump !
// 
// Usage: tcout << CHexDump (pb, pb + 100);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

 //---------------------------------------------------------------------------
 // MyDirtyHexDump:
 // 
 // Displays a hex dump !
 // 
 // Template argument(s):
 // 
 //   Char:
 //     The character type of the output stream
 //     (e.g. char, wchar_t).
 //   
 //   Traits:
 //     The traits type of the output stream
 //     (e.g. std::char_traits <Char>).
 //   
 //   FwdIter:
 //     A model of the Forward Iterator concept
 //     that is used to iterate over the bytes to be dumped
 //     (e.g. const byte *).
 // 
 // Argument(s):
 // 
 //   file:
 //     (A reference to) the output stream.
 //   
 //   iterBeg:
 //     An iterator pointing to the beginning of the range to be dumped.
 //   
 //   iterEnd:
 //     An iterator pointing one-past-the-end of the range to be dumped.
 //   
 //   iAddress:
 //     The initial address to display on the left of each row.
 //   
 //   cbitsAddress:
 //     The number of bits to consider in the address.
 //     This is typically 32, but it might be 64 in some cases
 //     (e.g. when dumping a part of the contents of a huge file).
 //     The -1 value holds the special meaning of "use the default".
 //   
 //   pszRowHeader:
 //     The header of each row.
 //     A null pointer has the special meaning of "use the default"
 //     (two whitespace characters).
 //     This is a work-around for the implementation of XTEXT
 //     on the Digital Mars C++ compiler (DMC).
 //   
 //   cbRow:
 //     The number of bytes to display on each row.
 // 
 // Return value:
 //   (A reference to) the output stream.
 //---------------------------------------------------------------------------

template <typename Char, class Traits, typename FwdIter>
std::basic_ostream <Char, Traits> &
MyDirtyHexDump
(
	      std::basic_ostream <Char, Traits> &file,
	const FwdIter                           &iterBeg,             // [xxx] Note(s) (2012-02-22): This used to be: const byte *pbFirst.
	const FwdIter                           &iterEnd,             // [xxx] Note(s) (2012-02-22): This used to be: const byte *pbLast.
	      uint64_t                           iAddress     =   0,
	      uint                               cbitsAddress = - 1,
	const Char                              *pszRowHeader =   0,
	      uint                               cbRow        =  16
)
{
	//using namespace std;
	
	if (cbitsAddress == uint (-1)) cbitsAddress = 32;
	if (! pszRowHeader)            pszRowHeader = XTEXT (Char, "  ");
	
	SaveFmtFlags <Char> fmt (&file);
	file << std::uppercase << std::setfill (Char ('0')) << std::right << std::hex;
	
	const std::locale loc               = file.getloc ();
	
	const uint        ccAddress         = (cbitsAddress + 3) / 4;
	const uint64_t    iAddressMask      = (uint64_t (1) << cbitsAddress) - 1;
	
	      FwdIter     iterRow           = iterBeg;
	      uint        iWithinRow        = 0;
	      
	      FwdIter     iter              = iterBeg;
	      std::size_t i                 = 0;
	
	while (iter != iterEnd)
	{
		if (! (i % cbRow))
			file << pszRowHeader << std::setw (ccAddress) << ((iAddress + i) & iAddressMask) << XTEXT (Char, ":  ");
		
		file << std::setw (2) << (uint (*iter) & 0xFF) << XTEXT (Char, " ");
		++iWithinRow;
		
		++iter;
		++i;
		
		if (! (i % cbRow) || iter == iterEnd)
		{
			for (uint jWithinRow = iWithinRow; jWithinRow < cbRow; ++jWithinRow)
				file << XTEXT (Char, "   ");
			
			file << XTEXT (Char, "  ");
			
			for (FwdIter iterChr = iterRow; iterChr != iter; ++iterChr)
			{
				bchar c = bchar (byte (*iterChr));
				{
					if (! std::isgraph (c, loc) && c != ' ')
						c = '.';
				}
				
				file << c;
			}
			
			file << Char ('\n');
			
			iterRow    = iter;
			iWithinRow = 0;
		}
	}
	
	return file;
}


class CHexDump
{
 public:
	CHexDump
	(
		const void     *pbBeg,
		const void     *pbEnd,
		      uint64_t  iAddress = 0
	)
		: _pbBeg    (static_cast <const byte *> (pbBeg))
		, _pbEnd    (static_cast <const byte *> (pbEnd))
		, _iAddress (iAddress)
	{}
	
	const byte *
	GetFirst
	()
	const
	{
		return _pbBeg;
	}
	
	const byte *
	begin
	()
	const
	{
		return _pbBeg;
	}
	
	const byte *
	GetLast
	()
	const
	{
		return _pbEnd;
	}
	
	const byte *
	end
	()
	const
	{
		return _pbEnd;
	}
	
	uint64_t
	GetAddress
	()
	const
	{
		return _iAddress;
	}

 private:
	const byte     *_pbBeg;
	const byte     *_pbEnd;
	      uint64_t  _iAddress;
};


template <typename Char, class Traits>
std::basic_ostream <Char, Traits> &
operator<<
(std::basic_ostream <Char, Traits> &file, const CHexDump &x)
{
	MyDirtyHexDump (file, x.GetFirst (), x.GetLast (), x.GetAddress ());
	return file;
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// HexDump:
// 
// [xxx] TODO: Doc !
// 
// [2012-02-22]:
//   This is a generic, more flexible version of the CHexDump class above.
//   It relies on the improved version of the MyDirtyHexDump function.
// 
// Usage:
// 
//   void ShowMeWhatYouHaveGot (const BYTE *pb, std::size_t cb)
//   {
//     std::tcout << Adder::HexDump <const BYTE *, TCHAR> (pb, pb + cb) << TEXT ("\n");
//   }
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <typename FwdIter, typename Char, class Traits = std::char_traits <Char>, class Alloc = std::allocator <Char> >
class HexDump
{
 //---------------------------------------------------------------------------
 // Type(s):
 //---------------------------------------------------------------------------
 public:
	typedef FwdIter                                   iterator;
	typedef Char                                      char_type;
	typedef Traits                                    traits_type;
	typedef Alloc                                     allocator_type;
	typedef std::basic_string  <Char, Traits, Alloc>  string_type;
	typedef std::basic_ostream <Char, Traits>         ostream_type;
	
	
 //---------------------------------------------------------------------------
 // Class mechanics:
 //---------------------------------------------------------------------------
 public:
	HexDump
	(
		const FwdIter           &iterBeg,
		const FwdIter           &iterEnd,
		      uint64_t           iAddress      =   0,
		      uint               cbitsAddress  = - 1,
		const string_type       &sRowHeader    = string_type (2, Char (' ')),
		      uint               cbRow         =  16
	)
		: _iterBeg              (iterBeg)
		, _iterEnd              (iterEnd)
		, _iAddress             (iAddress)
		, _cbitsAddress         (cbitsAddress)
		, _sRowHeader           (sRowHeader)
		, _cbRow                (cbRow)
	{
		Assert <FalsePrecondition> ((cbitsAddress && cbitsAddress <= 64) || cbitsAddress == uint (-1));
		Assert <FalsePrecondition> (cbRow && cbRow <= 256);
	}
	
	
 //---------------------------------------------------------------------------
 // Access to data:
 //---------------------------------------------------------------------------
 public:
	//--------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------
	
	FwdIter GetBeg ()
	const
	{
		return _iterBeg;
	}
	
	void SetBeg (const FwdIter &x)
	{
		_iterBeg = x;
	}
	
	
	//--------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------
	
	FwdIter GetEnd ()
	const
	{
		return _iterEnd;
	}
	
	void SetEnd (const FwdIter &x)
	{
		_iterEnd = x;
	}
	
	
	//--------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------
	
	uint64_t GetAddress ()
	const
	{
		return _iAddress;
	}
	
	void SetAddress (uint64_t x)
	{
		_iAddress = x;
	}
	
	
	//--------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------
	
	uint GetAddressWidth ()
	const
	{
		return _cbitsAddress;
	}
	
	void SetAddressWidth (uint x)
	{
		Assert <FalsePrecondition> ((x && x <= 64) || x == uint (-1));
		_cbitsAddress = x;
	}
	
	
	//--------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------
	
	void GetRowHeader (string_type *psOut)
	const
	{
		// [xxx] Note(s) (2012-02-22):
		//   The commented code represents our current style.
		//   This is the first attempt we make to entirely optimize away processing if the supplied pointer is null.  (-:
		//string_type sOut (_sRowHeader);
		//if (psOut) psOut->swap (sOut);
		if (psOut)
		{
			string_type sOut (_sRowHeader);
			psOut->swap (sOut);
		}
	}
	
	string_type GetRowHeader ()
	const
	{
		return _sRowHeader;
	}
	
	typename string_type::size_type GetRowHeaderLen ()
	const
	{
		return _sRowHeader.size ();
	}
	
	const Char *GetRowHeaderQ ()
	const
	{
		return _sRowHeader.c_str ();
	}
	
	void SetRowHeader (const string_type &x)
	{
		_sRowHeader = x;
	}
	
	
	//--------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------
	
	uint GetRowSize ()
	const
	{
		return _cbRow;
	}
	
	void SetRowSize (uint x)
	{
		Assert <FalsePrecondition> (x && x <= 256);
		_cbRow = x;
	}
	
	
 //---------------------------------------------------------------------------
 // Output:
 //---------------------------------------------------------------------------
 public:
	ostream_type &Put (ostream_type &file)
	const
	{
		return MyDirtyHexDump (file, _iterBeg, _iterEnd, _iAddress, _cbitsAddress, _sRowHeader.c_str (), _cbRow);
	}
	
	
 //---------------------------------------------------------------------------
 // 
 //---------------------------------------------------------------------------
 private:
	FwdIter      _iterBeg;
	FwdIter      _iterEnd;
	uint64_t     _iAddress;
	uint         _cbitsAddress;
	string_type  _sRowHeader;
	uint         _cbRow;
};


template <typename FwdIter, typename Char, class Traits, class Alloc>
std::basic_ostream <Char, Traits> &operator<<
(
	      std::basic_ostream <         Char, Traits>        &file,
	const HexDump            <FwdIter, Char, Traits, Alloc> &q
)
{
	return q.Put (file);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// IfFwd_Distance_Else_0_Helper (helper function):
// 
// Given two input iterators (which are not forward iterators), it returns 0;
// given two forward iterators, it returns the distance between them.
// 
// The function is meant to be used when reserving space (eg: in a vector)
// for a number of elements.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <class InpIter>
typename std::iterator_traits <InpIter>::difference_type
IfFwd_Distance_Else_0_Helper
(
	      InpIter                           iterBeg,
	      InpIter                           iterEnd,
	      std::input_iterator_tag
)
{
	return 0;
}

template <class FwdIter>
typename std::iterator_traits <FwdIter>::difference_type
IfFwd_Distance_Else_0_Helper
(
	      FwdIter                           iterBeg,
	      FwdIter                           iterEnd,
	      std::forward_iterator_tag
)
{
	return std::distance (iterBeg, iterEnd);
}

template <class InpIter>
typename std::iterator_traits <InpIter>::difference_type
IfFwd_Distance_Else_0
(
	      InpIter                           iterBeg,
	      InpIter                           iterEnd
)
{
	return IfFwd_Distance_Else_0_Helper
	(
		iterBeg,
		iterEnd,
		typename std::iterator_traits <InpIter>::iterator_category ()
	);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Output: Hex, Dec, Txt...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

class Hex      {};
class HexBlank {};
class Dec      {};
class DecBlank {};
class Txt      {};


template <class Char, class Traits>
std::basic_ostream <Char, Traits> &
operator<<
(
	std::basic_ostream <Char, Traits> &file,
	Hex
)
{
	return file << std::setfill (Char ('0')) << std::uppercase << std::right << std::hex;
}


template <class Char, class Traits>
std::basic_ostream <Char, Traits> &
operator<<
(
	std::basic_ostream <Char, Traits> &file,
	HexBlank
)
{
	return file << std::setfill (Char (' ')) << std::uppercase << std::right << std::hex;
}


template <class Char, class Traits>
std::basic_ostream <Char, Traits> &
operator<<
(
	std::basic_ostream <Char, Traits> &file,
	Dec
)
{
	return file << std::setfill (Char ('0')) << std::right << std::dec;
}


template <class Char, class Traits>
std::basic_ostream <Char, Traits> &
operator<<
(
	std::basic_ostream <Char, Traits> &file,
	DecBlank
)
{
	return file << std::setfill (Char (' ')) << std::right << std::dec;
}


template <class Char, class Traits>
std::basic_ostream <Char, Traits> &
operator<<
(
	std::basic_ostream <Char, Traits> &file,
	Txt
)
{
	return file << std::setfill (Char (' ')) << std::left;
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Manipulators (QHex, QDec, QTxt...):
// 
// [xxx] Note(s) (2012-07-11):
//   A careful reading of Bjarne Stroustrup's chapter on IOStreams
//   (from "The C++ Programming Language", 3rd special edition)
//   has shown me a better way to do things (without temporary objects)...
//   Enter our new manipulators !
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

std::ios_base &QHexBase (std::ios_base &base);
std::ios_base &QDecBase (std::ios_base &base);
std::ios_base &QTxtBase (std::ios_base &base);

#if 1
inline std::ios_base &QHexBase (std::ios_base &base)
{
	base.setf
	(
		std::ios_base::right       | std::ios_base::hex       | std::ios_base::uppercase,
		std::ios_base::adjustfield | std::ios_base::basefield | std::ios_base::uppercase
	);
	
	return base;
}

inline std::ios_base &QDecBase (std::ios_base &base)
{
	base.setf
	(
		std::ios_base::right       | std::ios_base::dec,
		std::ios_base::adjustfield | std::ios_base::basefield
	);
	
	return base;
}

inline std::ios_base &QTxtBase (std::ios_base &base)
{
	base.setf
	(
		std::ios_base::left,
		std::ios_base::adjustfield
	);
	
	return base;
}
#endif

template <typename Char, class Traits>
std::basic_ios <Char, Traits> &QHex      (std::basic_ios <Char, Traits> &ios)
{
	QHexBase (ios);
	ios.fill (Char ('0'));
	return ios;
}

template <typename Char, class Traits>
std::basic_ios <Char, Traits> &QHexBlank (std::basic_ios <Char, Traits> &ios)
{
	QHexBase (ios);
	ios.fill (Char (' '));
	return ios;
}

template <typename Char, class Traits>
std::basic_ios <Char, Traits> &QDec      (std::basic_ios <Char, Traits> &ios)
{
	QDecBase (ios);
	ios.fill (Char ('0'));
	return ios;
}

template <typename Char, class Traits>
std::basic_ios <Char, Traits> &QDecBlank (std::basic_ios <Char, Traits> &ios)
{
	QDecBase (ios);
	ios.fill (Char (' '));
	return ios;
}

template <typename Char, class Traits>
std::basic_ios <Char, Traits> &QTxt      (std::basic_ios <Char, Traits> &ios)
{
	QTxtBase (ios);
	ios.fill (Char (' '));
	return ios;
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// DirtyPut_Type, DirtyPut:
// 
//   return std::tcout << DirtyPut (myobj);
// means:
//   return myobj.Put (std::tcout);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <class T>
class DirtyPut_Type
{
	const T &_t;
	
 public:
	DirtyPut_Type (const T &tA)
		: _t (tA)
	{}
	
	T &Value () const
	{
		return _t;
	}
};

template <class T>
DirtyPut_Type <T> DirtyPut (const T &tA)
{
	return DirtyPut_Type <T> (tA);
}

template <typename Char, class Traits, class T>
std::basic_ostream <Char, Traits> &
operator<<
(
	      std::basic_ostream <Char, Traits>        &file,
	      DirtyPut_Type <T>                         dirtyt
)
{
	return dirtyt.Value ().Put (file);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// DirtyGet_Type, DirtyGet
// 
//   return std::tcin >> DirtyGet (myobj);
// means:
//   return myobj.Get (std::tcout);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <class T>
class DirtyGet_Type
{
 private:
	T &_t;
	
 public:
	DirtyGet_Type (T &tA)
		: _t (tA)
	{}
	
	T &Value () const
	{
		return _t;
	}
};


template <class T>
DirtyGet_Type <T> DirtyGet (T &tA)
{
	return DirtyGet_Type <T> (tA);
}


template <typename Char, class Traits, class T>
std::basic_istream <Char, Traits> &
operator>>
(
	      std::basic_istream <Char, Traits>        &file,
	      DirtyGet_Type <T>                         dirtyt
)
{
	return dirtyt.Value ().Get (file);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Indent function - for debugging purposes:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if ! ADDER_TARGET_DDK

#ifdef __BORLANDC__
 #pragma option push
 #pragma warn-inl
#endif

template <typename Char, class Traits = std::char_traits <Char>, class Alloc = std::allocator <Char> >
class Indent
{
 //---------------------------------------------------------------------------
 // Type(s):
 //---------------------------------------------------------------------------
 public:
	typedef Char                                                       char_type;
	typedef Traits                                                     traits_type;
	typedef Alloc                                                      allocator_type;
	typedef std::basic_string <char_type, traits_type, allocator_type>  string_type;
	
	
 //---------------------------------------------------------------------------
 // Data:
 //---------------------------------------------------------------------------
 private:
	      unsigned                  _iLevel;
	      bool                      _bNewLine;
	      unsigned                  _ccTab;
	      string_type               _sTab;
	
	
 //---------------------------------------------------------------------------
 // Constructor(s), assignment, destructor:
 //---------------------------------------------------------------------------
 public:
	
	//--------------------------------------------------------------------
	// Constructor:
	//--------------------------------------------------------------------
	
	Indent
	(
		      unsigned   iLevelA,
		      bool       bNewLineA = true,
		      unsigned   ccTabA    = 2,
		const char_type *pszTab    = 0
	)
		: _iLevel       (iLevelA)
		, _bNewLine     (bNewLineA)
		, _ccTab        (ccTabA)
	{
		if (pszTab)
			_sTab.assign (pszTab);
		else
			_sTab.assign (XTEXT (Char, " "));
	}
	
	
 //---------------------------------------------------------------------------
 // Method(s):
 //---------------------------------------------------------------------------
 public:
	//--------------------------------------------------------------------
	// Put:
	// 
	// (Note: This method used to be called "Write" !)
	//--------------------------------------------------------------------
	
	//template <typename Char, class Traits>
	////Adder::tostream &Write (Adder::tostream &file) const
	std::basic_ostream <Char, Traits> &
	Put
	(std::basic_ostream <Char, Traits> &file)
	const
	{
		if (_bNewLine)
			file << XTEXT (Char, "\n");
		
		for (unsigned i = 0; i < _iLevel; i++)
			for (unsigned j = 0; j < _ccTab; j++)
				file << _sTab;
		
		return file;
	}
};


 //---------------------------------------------------------------------------
 // Non-member(s):
 //---------------------------------------------------------------------------

//inline
//Adder::tostream &operator<< (Adder::tostream &file, const Indent &indent)
template <typename Char, class Traits, class Alloc>
std::basic_ostream <Char, Traits> &
operator<<
(
	      std::basic_ostream <Char, Traits> &file,
	const Indent <Char, Traits, Alloc>      &indent
)
{
	return indent.Put (file);
}


#ifdef __BORLANDC__
 #pragma option pop
#endif

#endif // ADDER_TARGET_DDK


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// EatWhiteSpace:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if ! ADDER_TARGET_DDK

template <typename Char, class Traits>
std::basic_istream <Char, Traits> &
EatWhiteSpace
(std::basic_istream <Char, Traits> &is)
{
	const std::locale loc = is.getloc ();
	
	while (is)
	{
		Char c;
		is.get (c);
		
		if (is.eof ())
		{
			// For character-level input, failbit is set alongside eofbit, so we clear it and break.
			// See Section 1.3.1 ("The Stream State Flags")
			// of "Standard C++ IOStreams and Locales" (by Angelika Langer and Klaus Kreft).
			is.clear (std::ios_base::eofbit);
			break;
		}
		else if (! is) // The "else" before this "if" would be vital if both of the lines in the brace above were missing.
			break;
		else if (! std::isspace (c, loc))
		{
			is.putback (c);
			break;
		}
		//else
		//	continue;
	}
	
	return is;
}

#endif // ADDER_TARGET_DDK


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Class template SeqReserver:
// 
// In a perfect world, this should have been a function template,
// but we can only partially specialize class templates
// (and not function templates).
// 
// Basically, this class allows one to call the member function "reserve"
// on a sequence, if that sequence type supports that member function.
// 
// [2020-08-29] TODO: SFINAE (for boost::container::vector etc.).
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <class T_OutSeq>
class SeqReserver
{
 public:
	typedef T_OutSeq OutSeq; // [2015-03-24 :X] We are adding this typedef.
	typedef typename OutSeq::size_type size_type;
	
	enum { bHasReserve = false };
	
	static
	void
	Reserve
	(OutSeq *pseqOut, size_type n)
	{}
};

template <class T, class Alloc>
class SeqReserver <std::vector <T, Alloc> >
{
 public:
	typedef std::vector <T, Alloc> OutSeq;
	typedef typename OutSeq::size_type size_type;
	
	enum { bHasReserve = true };
	
	static
	void
	Reserve
	(OutSeq *pseqOut, size_type n)
	{
		pseqOut->reserve (n);
	}
};

template <typename Char, class Traits, class Alloc>
class SeqReserver <std::basic_string <Char, Traits, Alloc> >
{
 public:
	typedef std::basic_string <Char, Traits, Alloc> OutSeq;
	typedef typename OutSeq::size_type size_type;
	
	enum { bHasReserve = true };
	
	static
	void
	Reserve
	(OutSeq *pseqOut, size_type n)
	{
		pseqOut->reserve (n);
	}
};


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// IndirectPerformer:
// 
// [xxx] TODO: Doc !
// 
// This is inspired by Loki...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//template <class Fn, class ReturnType>
//void Perform (const Fn &fn, ReturnType *pxRet)
//{
//	if (pxRet)
//		*pxRet = fn ();
//	else
//		         fn ();
//}
//
//
//template <class Fn>
//void Perform (const Fn &fn, void       *pxRet)
//{
//	                 fn ();
//}


template <typename ReturnType, typename PFn>
class IndirectPerformer
{
 public:
	explicit
	IndirectPerformer (const PFn &pfn)
		: _pfn (pfn)
	{}
	
	
	ReturnType operator() () const
		{ return (*_pfn) (); }
	
	
	template              <typename P1 >
	ReturnType operator() (const P1 &p1) const
		{ return (*_pfn) (p1); }
	
	
	template              <typename P1 , typename P2 >
	ReturnType operator() (const P1 &p1, const P2 &p2) const
		{ return (*_pfn) (p1, p2); }
	
	
	template              <typename P1 , typename P2 , typename P3 >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3) const
		{ return (*_pfn) (p1, p2, p3); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4) const
		{ return (*_pfn) (p1, p2, p3, p4); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5) const
		{ return (*_pfn) (p1, p2, p3, p4, p5); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6, const P7 &p7) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6, p7); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8 >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6, const P7 &p7, const P8 &p8) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6, p7, p8); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8 , typename P9 >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6, const P7 &p7, const P8 &p8, const P9 &p9) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8 , typename P9 , typename P10  >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6, const P7 &p7, const P8 &p8, const P9 &p9, const P10 &p10) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8 , typename P9 , typename P10  , typename P11  >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6, const P7 &p7, const P8 &p8, const P9 &p9, const P10 &p10, const P11 &p11) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8 , typename P9 , typename P10  , typename P11  , typename P12  >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6, const P7 &p7, const P8 &p8, const P9 &p9, const P10 &p10, const P11 &p11, const P12 &p12) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8 , typename P9 , typename P10  , typename P11  , typename P12  , typename P13  >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6, const P7 &p7, const P8 &p8, const P9 &p9, const P10 &p10, const P11 &p11, const P12 &p12, const P13 &p13) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8 , typename P9 , typename P10  , typename P11  , typename P12  , typename P13  , typename P14  >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6, const P7 &p7, const P8 &p8, const P9 &p9, const P10 &p10, const P11 &p11, const P12 &p12, const P13 &p13, const P14 &p14) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14); }
	
	
	template              <typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8 , typename P9 , typename P10 , typename P11  , typename P12  , typename P13   , typename P14  , typename P15  >
	ReturnType operator() (const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6, const P7 &p7, const P8 &p8, const P9 &p9, const P10 &p10, const P11 &p11, const P12 &p12, const P13 &p13, const P14 &p14, const P15 &p15) const
		{ return (*_pfn) (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15); }
	
	
 private:
	      PFn               _pfn;
};


//template <class PFn>
//class IndirectPerformer <void, PFn>
//{
// public:
//	explicit
//	IndirectPerformer (const PFn &pfn)
//		: _pfn (pfn)
//	{}
//	
//	
//	void operator() () const
//	{
//		(*_pfn) ();
//	}
//	
//	
// private:
//	PFn _pfn;
//};


template <typename ReturnType, typename PFn>
IndirectPerformer <ReturnType, PFn> MakeIndirectPerformer (const PFn &pfn)
{
	return IndirectPerformer <ReturnType, PFn> (pfn);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Pretty_Ptr:
// 
// Note(s) (2011-05-13):
//   This has been moved here from CStdInt.hpp,
//   after some complaints from gcc.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if ! ADDER_TARGET_DDK
class Pretty_Ptr
{
 public:
	explicit
	Pretty_Ptr
	(const volatile void *p, bool bPad = true, wchar_t cPad = wchar_t (' '))
		: _p    (p)
		, _bPad (bPad)
		, _cPad (cPad)
	{}
	
	template <class Char, class Traits>
	std::basic_ostream <Char, Traits> &
	Put
	(std::basic_ostream <Char, Traits> &os)
	const
	{
		SaveFmtFlags <Char> fmt (&os);
		
		os << std::uppercase << std::right << std::hex;
		
		if (_bPad)
			//os << std::setfill (Char ('0')) << std::setw (2 * sizeof (_p));
			os << std::setfill (Char (_cPad)) << std::setw (2 * sizeof (_p));
		
		os << reinterpret_cast <uintptr_t> (_p) << TEXT ("h");
		
		return os;
	}

 private:
	const volatile void    *_p;
	               bool     _bPad;
	               wchar_t  _cPad;
};


template <class Char, class Traits>
std::basic_ostream <Char, Traits> &operator<< (std::basic_ostream <Char, Traits> &os, const Pretty_Ptr &x)
{
	return x.Put (os);
}

#endif


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// CNumCmp, numcmp:
// 
// [2014-01-14]
// 
//   We would like to use this just as (x)strcmp.
//   It may allow us to write some beautifully aligned code
//   in operator== for a class
//   that has both string member vairables and numeric member variables.
//   (-:
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <typename T>
class CNumCmp
{
 public:
	int
	operator()
	(const T &x, const T &y)
	const
	{
		if (x < y)
			return -1;
		else
		if (y < x)
			return  1;
		else
			return  0;
	}
};


template <typename T>
int
numcmp
(const T &x, const T &y)
{
	const CNumCmp <T> cmp;
	return cmp (x, y);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=




//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


}


#endif
