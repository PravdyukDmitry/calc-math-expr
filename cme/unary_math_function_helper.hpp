/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file unary_math_function_helper.hpp                                  *
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

#ifndef CME_UNARY_MATH_FUNCTION_HELPER_HPP
 #define CME_UNARY_MATH_FUNCTION_HELPER_HPP

#include <cmath>
#include <boost/numeric/conversion/cast.hpp>

namespace cme { namespace cme_detail {

template < typename RealTy > 
RealTy const ln (RealTy const value_);

template < typename RealTy >
struct ld_umf_init_hlp_
{
	typedef RealTy real_type;

	static real_type const tg (real_type const value_)
	{
		return std::tan (value_);
	};

	static real_type const tgh (real_type const value_)
	{
		return std::tanh (value_);
	};

	static real_type const atg (real_type const value_)
	{
		return std::atan (value_);
	};

	static real_type const ln (real_type const value_)
	{
		return std::log (value_);
	};

	static real_type const exp (real_type const value_)
	{
		return std::exp (value_);
	};

	static real_type const lg (real_type const value_)
	{
		return std::log10 (value_);
	};
};

template <>
struct ld_umf_init_hlp_ < long double >
{
	typedef long double real_type;

	static real_type const tg (real_type const value_)
	{
		return ::tanl (value_);
	};

	static real_type const tgh (real_type const value_)
	{
		return ::tanhl (value_);
	};

	static real_type const atg (real_type const value_)
	{
		return ::atanl (value_);
	};

	static real_type const ln (real_type const value_)
	{
		return ::logl (value_);
	};

	static real_type const exp (real_type const value_)
	{
		return ::expl (value_);
	};

	static real_type const lg (real_type const value_)
	{
		return ::log10l (value_);
	};
};

template < typename RealTy >
unsigned const fact_hlp (RealTy const value_)
{
	return boost::numeric_cast < unsigned > (value_);
};

template < typename RealTy >
RealTy const acosech_helper (RealTy const value1_, RealTy const value2_)
{
	return ((1.0 + value1_) / value2_);
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_UNARY_MATH_FUNCTION_HELPER_HPP

/**
* 18.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
