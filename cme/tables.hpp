/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file tables.hpp                                                      *
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

#ifndef CME_TABLES_HPP
 #define CME_TABLES_HPP

#include <cme/table_base_impl.hpp>
#include <cme/table_init_helper.hpp>
#include <cme/tables_utility.hpp>

namespace cme {

template < typename CharTy = char >
class constant_table_t
	: public cme_detail::table_base_impl < CharTy, std::basic_string < CharTy > >
{
public:

	constant_table_t ()
		: super_type ()
	{
		init_table ();
	};

	constant_table_t (constant_table_t const& right_)
		: super_type (right_) 
	{
	};

private:
	virtual void init_table ()
	{
		cme_detail::init_helper < CharTy >::aply (this);
	};
};

template < typename CharTy = char, typename ValTy = double >
class unary_math_function_table_t
	: public cme_detail::table_base_impl < CharTy, 
		typename cme_detail::pointer_to_unary_math_function < ValTy >::type >
{
public:

	unary_math_function_table_t ()
		: super_type ()
	{
		init_table ();
	};

	unary_math_function_table_t (unary_math_function_table_t const& right_)
		: super_type (right_) 
	{
	};

private:

	virtual void init_table ()
	{
		cme_detail::init_helper < CharTy >::aply (this);
	};
};

template < typename CharTy = char, typename ValTy = double >
class variable_table_t
	: public cme_detail::table_base_impl < CharTy, ValTy >
{
public:

	variable_table_t ()
		: super_type () 
	{
	};

	variable_table_t (variable_table_t const& right_)
		: super_type (right_) 
	{
	};

private:
	virtual void init_table () 
	{
	};
};

typedef constant_table_t < char >		c_constant_table_t;
typedef constant_table_t < wchar_t >	wc_constant_table_t;

typedef unary_math_function_table_t < char, float >				cf_unary_math_function_table_t;
typedef unary_math_function_table_t < char, double >			cd_unary_math_function_table_t;
typedef unary_math_function_table_t < char, long double >		cld_unary_math_function_table_t;
typedef unary_math_function_table_t < wchar_t, float >			wcf_unary_math_function_table_t;
typedef unary_math_function_table_t < wchar_t, double >			wcd_unary_math_function_table_t;
typedef unary_math_function_table_t < wchar_t, long double >	wcld_unary_math_function_table_t;

typedef variable_table_t < char, float >			cf_variable_table_t;
typedef variable_table_t < char, double >			cd_variable_table_t;
typedef variable_table_t < char, long double >		cld_variable_table_t;
typedef variable_table_t < wchar_t, float >			wcf_variable_table_t;
typedef variable_table_t < wchar_t, double >		wcd_variable_table_t;
typedef variable_table_t < wchar_t, long double >	wcld_variable_table_t;

}; /// end namespace cme

#endif /// CME_TABLES_HPP

/**
* 17.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
