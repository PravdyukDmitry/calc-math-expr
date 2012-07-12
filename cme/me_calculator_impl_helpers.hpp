/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file me_calculator_impl_helpers.hpp                                  *
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

#ifndef CME_ME_CALCULATOR_IMPL_HELPERS_HPP
 #define CME_ME_CALCULATOR_IMPL_HELPERS_HPP

#include <cmath>
#include <functional>

#include <boost/numeric/conversion/cast.hpp>

namespace cme { namespace cme_detail {

template < typename RealTy >
struct me_calculator_impl_helpers_base
	: public std::binary_function < RealTy, RealTy, RealTy >
{
};

template < typename RealTy >
struct power_helper
	: public me_calculator_impl_helpers_base < RealTy >
{
	typedef RealTy real_type;

	real_type const operator () 
		(real_type const left_value_, real_type const right_value_) const
	{
		return std::pow (left_value_, right_value_);
	};
};

template < typename RealTy >
struct root_helper
	: public me_calculator_impl_helpers_base < RealTy >
{
	typedef RealTy real_type;

	real_type const operator () 
		(real_type const left_value_, real_type const right_value_) const
	{
		return std::pow (left_value_, 
			boost::numeric_cast < real_type > (1.0 / right_value_));
	};
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_ME_CALCULATOR_IMPL_HELPERS_HPP

/**
* 10.04.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
