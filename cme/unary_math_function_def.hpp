/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file unary_math_function_def.hpp                                     *
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

#ifndef CME_UNARY_MATH_FUNCTION_DEF_HPP
 #define CME_UNARY_MATH_FUNCTION_DEF_HPP

#include <cme/unary_math_function_helper.hpp>

#include <boost/math/special_functions/factorials.hpp>
#include <boost/math/special_functions/asinh.hpp>
#include <boost/math/special_functions/acosh.hpp>
#include <boost/math/special_functions/atanh.hpp>

namespace cme { namespace cme_detail {

/// @short Синус
template < typename RealTy >
RealTy const sin (RealTy const value_)
{
	return std::sin (value_);
};

/// @short Косинус
template < typename RealTy >
RealTy const cos (RealTy const value_)
{
	return std::cos (value_);
};

/// @short Синус гипербалический
template < typename RealTy >
RealTy const sinh (RealTy const value_)
{
	return std::sinh (value_);
};

/// @short Косинус гипербалический
template < typename RealTy >
RealTy const cosh (RealTy const value_)
{
	return std::cosh (value_);
};

/// @short Тангенс
template < typename RealTy >
RealTy const tg (RealTy const value_)
{
	return ld_umf_init_hlp_ < RealTy >::tg (value_);
};

/// @short Катангенс
template < typename RealTy >
RealTy const ctg (RealTy const value_)
{
	return (1.0 / tg (value_)); 
};

/// @short Гиперболический тангенс
template < typename RealTy >
RealTy const tgh (RealTy const value_)
{
	return ld_umf_init_hlp_ < RealTy >::tgh (value_);
};

/// @short Гиперболический катангенс
template < typename RealTy >
RealTy const ctgh (RealTy const value_) 
{ 
	return (1.0 / tgh (value_)); 
};

/// @short Арктангенс
template < typename RealTy >
RealTy const atg (RealTy const value_)
{
	return ld_umf_init_hlp_ < RealTy >::atg (value_);
};

/// @short Арккатангенс
template < typename RealTy >
RealTy const actg (RealTy const value_)
{
	return (atg (1.0 / value_));
};

/// @short Гиперболический арксинус
template < typename RealTy >
RealTy const asinh (RealTy const value_)
{ 
	return boost::math::asinh < RealTy > (value_); 
};

/// @short Гиперболический арккосинус
template < typename RealTy >
RealTy const acosh (RealTy const value_)
{
	return boost::math::acosh < RealTy > (value_);
};

/// @short Гиперболический арктангенс
template < typename RealTy >
RealTy const atgh (RealTy const value_)
{ 
	return boost::math::atanh < RealTy > (value_); 
};

/// @short Гиперболический арккатангенс
template < typename RealTy >
RealTy const actgh (RealTy const value_)
{ 
	return (0.5 * ln < RealTy > ((value_ + 1.0) / (value_ - 1.0)));
};

/// @short Секанс
template < typename RealTy >
RealTy const sec (RealTy const value_)
{ 
	return (1.0 / cos < RealTy > (value_));
};

/// @short Косеканс
template < typename RealTy >
RealTy const cosec (RealTy const value_)
{ 
	return (1.0 / sinh < RealTy > (value_));
};

/// @short Гиперболический секанс
template < typename RealTy >
RealTy const sech (RealTy const value_)
{ 
	return (1.0 / cosh < RealTy > (value_));
};

/// @short Гиперболический косеканс
template < typename RealTy >
RealTy const cosech (RealTy const value_)
{ 
	return (1.0 / sinh < RealTy > (value_));
};

/// @short Обратный гиперболический секанс
template < typename RealTy >
RealTy const asech (RealTy const value_)
{ 
	return ln < RealTy > 
		((1.0 + (std::sqrt (1.0 - value_ * value_))) / value_);
};

/// @short Обратный гиперболический косеканс
template < typename RealTy >
RealTy const acosech (RealTy const value_)
{
	RealTy const internal_value_ = std::sqrt (1.0 + value_ * value_);
	return (value_ < 0.0) ? acosech_helper (-internal_value_, value_)
		: acosech_helper (internal_value_, value_);
};

/// @short Натуральный логарифм
template < typename RealTy >
RealTy const ln (RealTy const value_)
{ 
	return ld_umf_init_hlp_ < RealTy >::ln (value_); 
};

/// @short Експанента
template < typename RealTy >
RealTy const exp (RealTy const value_)
{ 
	return ld_umf_init_hlp_ < RealTy >::exp (value_); 
};

/// @short Десятичный логарифм
template < typename RealTy >
RealTy const lg (RealTy const value_)
{ 
	return ld_umf_init_hlp_ < RealTy >::lg (value_); 
};

/// @short Модуль
template < typename RealTy >
RealTy const modul (RealTy const value_)
{
	return (value_ > 0 ? value_ : - value_);
};

/// @short Факториал
template < typename RealTy >
RealTy const fact (RealTy const value_)
{
	return boost::math::factorial < RealTy > (fact_hlp (value_));
};

/// @short двойной факториал
template < typename RealTy >
RealTy const dfact (RealTy const value_)
{
	return boost::math::double_factorial < RealTy > (fact_hlp (value_));
};

/// @short квадратный корень
template < typename RealTy >
RealTy const sqrt (RealTy const value_)
{
	return std::sqrt (value_);
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_UNARY_MATH_FUNCTION_DEF_HPP

/**
* 18.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
