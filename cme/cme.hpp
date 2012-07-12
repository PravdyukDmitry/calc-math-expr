/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file cme.hpp                                                         *
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

#ifndef CME_CME_HPP
 #define CME_CME_HPP

#include "rpn_creator.hpp"
#include "me_calculator.hpp"

/**
* @file cme.hpp - Основной заголовочный файл библиотеки cme, т.е. отсюда "видно" всю библиотеку...
* для использования преднозначены только обьекты и функции находящиеся в пространстве имён cme
**/

namespace cme {

/// Метафункция для быстрого выведения основных типов
template < typename RealTy, typename CharTy >
struct cme_generator
{
	typedef RealTy real_type;
	typedef CharTy char_type;

	typedef variable_table_t < char_type, real_type >	variable_table_type;
	typedef constant_table_t < char_type >	constant_table_type;
	typedef unary_math_function_table_t 
							< char_type, real_type >	unary_math_function_table_type;

	typedef cme_detail::cme_expression_generator < char_type >	exp_generator_type;
	typedef typename exp_generator_type::type					cme_expression_type;
	typedef typename exp_generator_type::token_data_type		token_data_type;
	typedef typename exp_generator_type::token_type				token_type;

	typedef rpn_creator	<
						char_type
						, real_type
						, variable_table_t
						, constant_table_t
						, unary_math_function_table_t
						>	rpn_creator_type;

	typedef me_calculator	<
							char_type
							, real_type
							, variable_table_t
							, constant_table_t
							, unary_math_function_table_t 
						>	cme_calculator_type;
};

/** 
* 3 версии функци calculate_math_expression принимают:
*	- математическое выражение заданное в параметре expr_ - Си-строка или 
*	STL-подобный контейнер или интервалом из итераторов [begin_expr_, end_expr_]; 
*	- таблицу переменных, заданную указателем на variable_table_t - p_vtlb_,
*	- таблицу констант, заданную указателем на constant_table_t - p_ctlb_;
*	- таблицу унарных математических функцый, заданную указателем на unary_math_function_table_t - p_umftlb_;
* и возвращает значение етого выражения...
*
* Если математическое выражение (expr_) несодержит переменных, то, в целях экономии ресурсов,
* установите указатель p_vtlb_ в значение NULL, также, при необходимости,
* поступите с указателями p_ctlb_ и p_umftlb_ ...
*
* !!! ВАЖНО !!!
*	Вызывающий код должен заботиться о том чтобы математическое выражение НЕ заканчивались пробелами,
*	табами, и д.р. подобными символами,
*	т.е. ТАК НЕЛЬЗЯ:
*		Математическое выражение -> " 3.0 * x + pow (4, fact(20) / sech (PI)) "
*                                                                            ^
*	а ТАК МОЖНО:
*		Математическое выражение -> " 3.0 * x + pow (4, fact(20) / sech (PI))"
* Вызвано это скорее всего моими кревыми руками, извиняюсь за неудобства ;)
**/

template	<
	typename RealTy
	, typename CharTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UmfTlbTy 
			>
RealTy const calculate_math_expression (CharTy const* const expr_, 
	VarTlbTy < CharTy, RealTy >* const p_vtlb_, 
	ConstTlbTy < CharTy > const* const p_ctlb_,
	UmfTlbTy < CharTy, RealTy > const* const p_umftlb_)
{
	return me_calculator < CharTy, RealTy, VarTlbTy, ConstTlbTy, UmfTlbTy > 
			(p_vtlb_, p_ctlb_, p_umftlb_)	(
				rpn_creator < CharTy, RealTy, VarTlbTy, ConstTlbTy, UmfTlbTy > 
						(p_vtlb_, p_ctlb_, p_umftlb_) (expr_)
											);
};

template	<
	typename RealTy
	, typename IterTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UmfTlbTy 
			>
RealTy const calculate_math_expression (IterTy begin_expr_, IterTy const& end_expr_, 
	VarTlbTy < typename std::iterator_traits < IterTy >::value_type, RealTy >* const p_vtlb_, 
	ConstTlbTy < typename std::iterator_traits < IterTy >::value_type > const* const p_ctlb_,
	UmfTlbTy < typename std::iterator_traits < IterTy >::value_type, RealTy > const* const p_umftlb_)
{
	return me_calculator	<
			 typename std::iterator_traits < IterTy >::value_type
			, RealTy, VarTlbTy, ConstTlbTy, UmfTlbTy 
			> (p_vtlb_, p_ctlb_, p_umftlb_)	(
				rpn_creator	<
					typename std::iterator_traits < IterTy >::value_type
					, RealTy, VarTlbTy, ConstTlbTy, UmfTlbTy 
				> (p_vtlb_, p_ctlb_, p_umftlb_) (begin_expr_, end_expr_)
											);
};

template	<
	typename RealTy
	, typename StrTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UmfTlbTy 
			>
RealTy const calculate_math_expression (StrTy const& expr_,
	VarTlbTy < typename StrTy::value_type, RealTy >* const p_vtlb_, 
	ConstTlbTy < typename StrTy::value_type > const* const p_ctlb_,
	UmfTlbTy < typename StrTy::value_type, RealTy > const* const p_umftlb_)
{
	return me_calculator < typename StrTy::value_type, RealTy,
			VarTlbTy, ConstTlbTy, UmfTlbTy > 
			(p_vtlb_, p_ctlb_, p_umftlb_)	(
				rpn_creator	< typename StrTy::value_type, RealTy,
					VarTlbTy, ConstTlbTy, UmfTlbTy > 
						(p_vtlb_, p_ctlb_, p_umftlb_) (expr_)
											);
};

}; /// end namespace cme

#endif /// CME_CME_HPP

/**
* 10.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
