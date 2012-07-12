/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file rpn_creator.hpp                                                 *
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

#ifndef CME_RPN_CREATOR_HPP
 #define CME_RPN_CREATOR_HPP

#include <cme/tables.hpp>
#include <cme/rpn_creator_impl.hpp>

#include <boost/shared_ptr.hpp>

namespace cme {

/**
* rpn_creator - функтор создающий ОПЗ из входного выражения 
*	представленного параметрами rpn_creator::operator ()
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
class rpn_creator
{
	rpn_creator const& operator = (rpn_creator const&);
public:
	typedef cme_detail::rpn_creator_impl 
		< CharTy, RealTy, VarTlbTy, ConstTlbTy, UMFuncTlbTy > impl_type;
	typedef boost::shared_ptr < impl_type > data_type;

	typedef CharTy	char_type;
	typedef RealTy	real_type;

	typedef typename impl_type::variable_table_type				variable_table_type;
	typedef typename impl_type::constant_table_type				constant_table_type;
	typedef typename impl_type::unary_math_function_table_type	unary_math_function_table_type;

	typedef typename impl_type::cme_expression_type cme_expression_type;

	rpn_creator (variable_table_type const* const p_var_tlb, 
		constant_table_type const* const p_const_tlb, 
		unary_math_function_table_type const* const p_umf_tlb)
		: p_my_creator_ (new impl_type (p_var_tlb, p_const_tlb, p_umf_tlb)) 
	{
	};

	/// Операторы создающие ОПЗ...

	template < typename IterTy >
	cme_expression_type const& operator () (IterTy const begin_expr, IterTy const& end_expr) const
	{
		return this->p_my_creator_->build (begin_expr, end_expr);
	};

	template < typename IterTy >
	cme_expression_type& operator () (IterTy const begin_expr, IterTy const& end_expr)
	{
		return this->p_my_creator_->build (begin_expr, end_expr);
	};

	template < typename StrTy >
	cme_expression_type const& operator () (StrTy const& expr) const
	{
		return (*this) (expr.begin (), expr.end ());
	};

	template < typename StrTy >
	cme_expression_type& operator () (StrTy const& expr)
	{
		return (*this) (expr.begin (), expr.end ());
	};

	cme_expression_type const& operator () (char_type const* const expr) const
	{
		return this->p_my_creator_->build (expr);
	};

	cme_expression_type& operator () (char_type const* const expr)
	{
		return this->p_my_creator_->build (expr);
	};

private:
	data_type const p_my_creator_;
};

/// Вспомогательные функции ...

template	<
	typename CharTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy
			>
rpn_creator < CharTy, RealTy, VarTlbTy, ConstTlbTy, UMFuncTlbTy > 
make_rpn_creator (VarTlbTy < CharTy, RealTy >* const p_vtlb_,
	ConstTlbTy < CharTy > const* const p_ctlb_, UMFuncTlbTy < CharTy, RealTy > const* const p_umftlb_)
{
	return rpn_creator < CharTy, RealTy, VarTlbTy, ConstTlbTy, UMFuncTlbTy > (p_vtlb_, p_ctlb_, p_umftlb_);
};

template	<
	typename CharTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy
			>
typename cme_detail::cme_expression_generator < CharTy >::type
create_rpn (CharTy const* const expr_, VarTlbTy < CharTy, RealTy >* const p_vtlb_,
	ConstTlbTy < CharTy > const* const p_ctlb_, UMFuncTlbTy < CharTy, RealTy > const* const p_umftlb_)
{
	return make_rpn_creator (p_vtlb_, p_ctlb_, p_umftlb_) (expr_);
};

template	<
	typename StrTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy
			>
typename cme_detail::cme_expression_generator < typename StrTy::value_type >::type
create_rpn (StrTy const& expr_, VarTlbTy < typename StrTy::value_type, RealTy >* const p_vtlb_,
	ConstTlbTy < typename StrTy::value_type > const* const p_ctlb_,
	UMFuncTlbTy < typename StrTy::value_type, RealTy > const* const p_umftlb_)
{
	return make_rpn_creator (p_vtlb_, p_ctlb_, p_umftlb_) (expr_);
};

template	<
	typename IterTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy
			>
typename cme_detail::cme_expression_generator < typename std::iterator_traits < IterTy >::value_type >::type
create_rpn (IterTy begin_expr_, IterTy const& end_expr_, 
	VarTlbTy < typename std::iterator_traits < IterTy >::value_type, RealTy >* const p_vtlb_,
	ConstTlbTy < typename std::iterator_traits < IterTy >::value_type > const* const p_ctlb_, 
	UMFuncTlbTy < typename std::iterator_traits < IterTy >::value_type, RealTy > const* const p_umftlb_)
{
	return make_rpn_creator (p_vtlb_, p_ctlb_, p_umftlb_) (begin_expr_, end_expr_);
};

}; /// end namespace cme

#endif /// CME_RPN_CREATOR_HPP

/**
* 24.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
