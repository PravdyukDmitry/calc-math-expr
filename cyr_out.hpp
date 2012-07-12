/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   @file cyr_out.hpp                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
**/

#ifndef PCHOLA_CYR_OUT_HPP
 #define PCHOLA_CYR_OUT_HPP

#ifdef _WIN32

#include <windows.h>
#include <cstddef>
#include <ostream>
#include <stdexcept>

#include <boost/scoped_array.hpp>

namespace pchola { namespace pl_detail {

template < typename CharTy >
struct buff_len_policy;

template <>
struct buff_len_policy < char >
{
	typedef char char_type;

	static std::size_t const get (char_type const* const str_)
	{
		return ::strlen (str_) + 1;
	};
};

template <>
struct buff_len_policy < wchar_t >
{
	typedef wchar_t char_type;

	static std::size_t const get (char_type const* const str_)
	{
		std::size_t char_count_ = 0;
		char_type const* it_ = str_;

		while (*it_ != L'\0')
		{
			++char_count_;
			++it_;
		}

		return ++char_count_;
	};
};

template < typename CharTy >
struct char_to_oem_policy;

template <>
struct char_to_oem_policy < char >
{
	typedef char char_type;

	static int const convert (char_type const* const str_, char* buff_)
	{
		return CharToOemA (str_, buff_);
	};
};

template <>
struct char_to_oem_policy < wchar_t >
{
	typedef wchar_t char_type;

	static int const convert (char_type const* const str_, char* buff_)
	{
		return CharToOemW (str_, buff_);
	};
};

}; /// end namespace pchola::pl_detail

template <
	typename CharTy = char
	, typename BuffLenPolicyTy = pl_detail::buff_len_policy < CharTy >
	, typename CharToOemPolicyTy = pl_detail::char_to_oem_policy < CharTy >
		>
struct windows_console_printer
{
	typedef CharTy char_type;
	typedef std::basic_ostream < char_type > ostream_type;
	typedef boost::scoped_array < char > buffer_type;
	typedef BuffLenPolicyTy buff_len_policy_type;
	typedef CharToOemPolicyTy char_to_oem_policy_type;

	static void print (char_type const* const str_, ostream_type& os_ = std::cout)
	{
		if (str_)
		{
			buffer_type buff_ (new char_type [buff_len_policy_type::get (str_)]);
			char_to_oem_policy_type::convert (str_, buff_.get ());
			os_ << buff_.get ();
		}
		else
			throw std::invalid_argument ("ERROR in function windows_console_printer::print (): "
			"ptr to string == null !!!");
	};
};

template < typename CharTy >
struct cyr_out_t
{
	typedef CharTy char_type;
	typedef windows_console_printer < char_type > printer_type;

	cyr_out_t& operator << (char_type const* const str_)
	{
		printer_type::print (str_);
		return (*this);
	};
};

}; /// end namespace pchola

#endif /// _WIN32

#endif /// PCHOLA_CYR_OUT_HPP

/**
* 10.03.2009 - Потураев А.С.
**/
