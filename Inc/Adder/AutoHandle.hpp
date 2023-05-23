#ifndef __Dev_3_AutoHandle_hpp
#define __Dev_3_AutoHandle_hpp 1


#include "Swap.hpp"

#include <boost/core/exchange.hpp>
#include <boost/config.hpp>

#include <algorithm>


namespace Adder
{


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template <class Traits>
class AutoHandle
{
 public:
	typedef          Traits             TraitsType;
	typedef typename Traits::HandleType HandleType;

 private:
	      HandleType                        _h;

 public:
	~AutoHandle
	()
	{
		if (Traits::IsValid (_h))
			Traits::Close (_h);
	}
	
	void
	Swap
	(AutoHandle &rhs)
	BOOST_NOEXCEPT
	{
		using std::swap;
		swap (_h, rhs._h);
	}
	
	#ifndef BOOST_NO_RVALUE_REFERENCES
	AutoHandle
	(AutoHandle &&rhs)
	BOOST_NOEXCEPT
		: _h (boost::exchange (rhs._h, Traits::GetAnInvalidValue ()))
	{}
	
	AutoHandle &
	operator=
	(AutoHandle &&rhs)
	BOOST_NOEXCEPT
	{
		AutoHandle tmp (std::move (rhs));
		Swap (tmp);
		return *this;
	}
	#endif
	
 private:
	AutoHandle
	(const AutoHandle &rhs);
	
	AutoHandle &
	operator=
	(const AutoHandle &rhs);

 public:
	#ifndef BOOST_NO_RVALUE_REFERENCES
	explicit
	AutoHandle
	(HandleType &&h)
	BOOST_NOEXCEPT
		: _h (std::move (h))
	{}
	#endif
	
	explicit
	AutoHandle
	(const HandleType &h)
		: _h (h)
	{}
	
	AutoHandle
	()
	BOOST_NOEXCEPT
		: _h (Traits::GetAnInvalidValue ())
	{}

 public:
	BOOST_CONSTEXPR
	bool
	IsValid
	()
	const
	BOOST_NOEXCEPT
	{
		return Traits::IsValid (_h);
	}
	
	BOOST_CONSTEXPR
	HandleType
	Get
	()
	const
	BOOST_NOEXCEPT
	{
		return _h;
	}
	
	BOOST_CONSTEXPR
	operator HandleType
	()
	const
	BOOST_NOEXCEPT
	{
		return Get ();
	}
	
	void
	Release
	(HandleType *ph)
	BOOST_NOEXCEPT
	{
		*ph = boost::exchange (_h, Traits::GetAnInvalidValue ());
	}
	
	HandleType
	Release
	()
	BOOST_NOEXCEPT
	{
		return boost::exchange (_h, Traits::GetAnInvalidValue ());
	}
	
	void
	Reset
	()
	BOOST_NOEXCEPT
	{
		AutoHandle tmp;
		Swap (tmp);
	}
	
	#ifndef BOOST_NO_RVALUE_REFERENCES
	void
	Reset
	(HandleType &&h)
	BOOST_NOEXCEPT
	{
		AutoHandle tmp (std::move (h));
		Swap (tmp);
	}
	#endif
	
	void
	Reset
	(const HandleType &h)
	{
		AutoHandle tmp (h);
		Swap (tmp);
	}
};

template <class Traits>
void
swap
(AutoHandle <Traits> &lhs, AutoHandle <Traits> &rhs)
BOOST_NOEXCEPT
{
	return lhs.Swap (rhs);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


}


#endif
