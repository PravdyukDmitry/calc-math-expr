/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file string_to_real_cast.hpp                                         *
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

#ifndef CME_STRING_TO_REAL_CAST_HPP
 #define CME_STRING_TO_REAL_CAST_HPP

#include <cme/cme_exception.hpp>
#include <cme/real_parser_generator.hpp>

namespace cme {

template < typename RealTy, typename CharTy >
RealTy const string_to_real_cast (CharTy const* const expr_)
{
	typedef RealTy real_type;
	typedef typename cme_detail::real_parser_generator < real_type >::type real_parser_type;
	real_type value_ = real_type ();
	real_parser_type const real_pars_ = real_parser_type ();

	if (!boost::spirit::parse (expr_, real_pars_ [boost::spirit::assign_a (value_)]).full)
		throw cme_detail::bad_string_to_real_cast < RealTy > (std::basic_string < CharTy > (expr_));

	return value_;
};

template < typename RealTy, typename IterTy >
RealTy const string_to_real_cast (IterTy const begin_expr_, IterTy const end_expr_)
{
	typedef RealTy real_type;
	typedef typename cme_detail::real_parser_generator < real_type >::type real_parser_type;
	real_type value_ = real_type ();
	real_parser_type const real_pars_ = real_parser_type ();

	if (!boost::spirit::parse (begin_expr_, end_expr_, real_pars_ [boost::spirit::assign_a (value_)]).full)
		throw cme_detail::bad_string_to_real_cast < RealTy > (begin_expr_, end_expr_);

	return value_;
};

template < typename RealTy, typename StrTy >
RealTy const string_to_real_cast (StrTy const& expr_)
{
	return string_to_real_cast < RealTy > (expr_.begin (), expr_.end ());
};

}; /// end namespace cme

#endif /// CME_STRING_TO_REAL_CAST_HPP

/**
* 10.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
