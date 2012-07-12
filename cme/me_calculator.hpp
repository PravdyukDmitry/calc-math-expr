/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file me_calculator.hpp                                               *
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

#ifndef CME_ME_CALCULATOR_HPP
 #define CME_ME_CALCULATOR_HPP

#include <cme/tables.hpp>
#include <cme/me_calculator_impl.hpp>

#include <boost/shared_ptr.hpp>

namespace cme {

/**
 * me_calculator - функтор расчитывающий значение по ОПЗ
 *	представленной параметрами me_calculator::operator ()
 * Параметры шаблона:
 * CharTy - тип символов
 * RealTy - тип с плавоющей точкой
 * VarTlbTy - тип таблицы переменных
 * ConstTlbTy - тип таблицы констант
 * UMFuncTlbTy - тип таблицы унарных математических функций
 **/

template	<
	typename CharTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy = variable_table_t
	, template < typename > class ConstTlbTy = constant_table_t
	, template < typename, typename > class UMFuncTlbTy = unary_math_function_table_t 
			>
class me_calculator
{
	me_calculator const& operator = (me_calculator const&);
public:
	typedef cme_detail::me_calculator_impl 
			< CharTy, RealTy, VarTlbTy, ConstTlbTy, UMFuncTlbTy > impl_type;
	typedef boost::shared_ptr < impl_type > data_type;
	
	typedef CharTy	char_type;
	typedef RealTy	real_type;

	typedef typename impl_type::variable_table_type				variable_table_type;
	typedef typename impl_type::constant_table_type				constant_table_type;
	typedef typename impl_type::unary_math_function_table_type	unary_math_function_table_type;

	typedef typename impl_type::token_data_type		token_data_type;
	typedef typename impl_type::token_type			token_type;
	typedef typename impl_type::cme_expression_type	cme_expression_type;

	me_calculator	(variable_table_type *const p_vtlb, constant_table_type const* const p_ctlb, 
		unary_math_function_table_type const* const p_umftlb)
		: my_cme_calc_ (new impl_type (p_vtlb, p_ctlb, p_umftlb)) 
	{
	};

	/// Операторы расчитывающие значение по ОПЗ

	template < typename CMEExprIterTy >
	real_type const operator () (CMEExprIterTy begin_rpn_expr_, CMEExprIterTy const& end_rpn_expr_) const
	{
		return this->my_cme_calc_->calculate (begin_rpn_expr_, end_rpn_expr_);
	};

	template < typename CMEExprTy >
	real_type const operator () (CMEExprTy const& rpn_expr_) const
	{
		return (*this) (rpn_expr_.begin (), rpn_expr_.end ());
	};

private:
	data_type const my_cme_calc_;
};

/// Вспомогательные функции ...

template	<
	typename CharTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy
			>
me_calculator < CharTy, RealTy, VarTlbTy, ConstTlbTy, UMFuncTlbTy >
make_me_calculator (VarTlbTy < CharTy, RealTy >* const p_vtlb_,
	ConstTlbTy < CharTy > const* const p_ctlb_, UMFuncTlbTy < CharTy, RealTy > const* const p_umftlb_)
{
	return me_calculator < CharTy, RealTy, VarTlbTy, ConstTlbTy, UMFuncTlbTy > (p_vtlb_, p_ctlb_, p_umftlb_);
};

template	<
	typename CharTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy
			>
RealTy const me_calc (typename cme_detail::cme_expression_generator < CharTy >::type const& rpn_mexpr_,
	VarTlbTy < CharTy, RealTy >* const p_vtlb_, ConstTlbTy < CharTy > const* const p_ctlb_, 
	UMFuncTlbTy < CharTy, RealTy > const* const p_umftlb_)
{
	return make_me_calculator (p_vtlb_, p_ctlb_, p_umftlb_) (rpn_mexpr_);
};

template	<
	typename CMEExprTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy
			>
RealTy const me_calc (CMEExprTy const& rpn_mexpr_,
	VarTlbTy < typename CMEExprTy::char_type, RealTy >* const p_vtlb_,
	ConstTlbTy < typename CMEExprTy::char_type > const* const p_ctlb_, 
	UMFuncTlbTy < typename CMEExprTy::char_type, RealTy > const* const p_umftlb_)
{
	return make_me_calculator (p_vtlb_, p_ctlb_, p_umftlb_) (rpn_mexpr_);
};

template	<
	typename CMEExprIterTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy
			>
RealTy const me_calc (CMEExprIterTy begin_rpn_expr_, CMEExprIterTy const& end_rpn_expr_,
	VarTlbTy < typename CMEExprIterTy::value_type::char_type, RealTy >* const p_vtlb_,
	ConstTlbTy < typename CMEExprIterTy::value_type::char_type > const* const p_ctlb_, 
	UMFuncTlbTy < typename CMEExprIterTy::value_type::char_type, RealTy > const* const p_umftlb_)
{
	return make_me_calculator (p_vtlb_, p_ctlb_, p_umftlb_) (begin_rpn_expr_, end_rpn_expr_);
};

}; /// end namespace cme

#endif /// CME_ME_CALCULATOR_HPP

/**
* 24.03.2009 - Потураев А.С. - подготовка релиза... :)
**/
