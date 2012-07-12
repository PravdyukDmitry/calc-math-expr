/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file rpn_creator_impl.hpp                                            *
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

#ifndef CME_RPN_CREATOR_IMPL_HPP
 #define CME_RPN_CREATOR_IMPL_HPP

#include <cme/create_rpn_algorithm_t.hpp>
#include <cme/make_cme_grammar.hpp>
#include <cme/cme_expression_generator.hpp>
#include <cme/cme_exception.hpp>
#include <cme/tables_utility.hpp>

namespace cme { namespace cme_detail {

template	<
	typename CharTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy 
			>
class rpn_creator_impl
{
	rpn_creator_impl const& operator = (rpn_creator_impl const&);
public:
	typedef CharTy	char_type;
	typedef RealTy	real_type;

	typedef VarTlbTy < CharTy, RealTy >		variable_table_type;
	typedef ConstTlbTy < CharTy >			constant_table_type;
	typedef UMFuncTlbTy < CharTy, RealTy >	unary_math_function_table_type;

	typedef typename cme_expression_generator < char_type >::type				cme_expression_type;
	typedef typename cme_expression_type::const_iterator						iterator_type;
	typedef typename cme_expression_generator < char_type >::token_type			token_type;
	typedef typename cme_expression_generator < char_type >::token_data_type	token_data_type;

	typedef create_rpn_algorithm_t < cme_expression_type >				create_rpn_algorithm_type;
	typedef make_cme_grammar < char_type, create_rpn_algorithm_type >	make_cme_grammar_type;

	rpn_creator_impl (variable_table_type const* const p_var_tlb, 
		constant_table_type const* const p_const_tlb, 
		unary_math_function_table_type const* const p_umf_tlb)
		: cme_grammar_ (p_var_tlb, p_const_tlb, p_umf_tlb) 
	{
	};

	/// Создание ОПЗ ...
	template < typename IterTy >
	cme_expression_type const& build (IterTy const begin_expr, IterTy const& end_expr) const
	{
		this->do_build (begin_expr, end_expr);
		return this->cme_grammar_.get_cme_expr ();
	};

	template < typename IterTy >
	cme_expression_type& build (IterTy const begin_expr, IterTy const& end_expr)
	{
		this->do_build (begin_expr, end_expr);
		return this->cme_grammar_.get_cme_expr ();
	};

	template < typename ChTy >
	cme_expression_type const& build (ChTy const* const expr) const
	{
		this->do_build (expr);
		return this->cme_grammar_.get_cme_expr ();
	};

	template < typename ChTy >
	cme_expression_type& build (ChTy const* const expr)
	{
		this->do_build (expr);
		return this->cme_grammar_.get_cme_expr ();
	};

private:
	make_cme_grammar_type cme_grammar_;

	template < typename IterTy >
	void do_build (IterTy const begin_expr, IterTy const& end_expr)
	{
		pars_error_detect < cme_create_error >
			(boost::spirit::parse (begin_expr, end_expr, this->cme_grammar_, boost::spirit::space_p));
	};

	template < typename ChTy >
	void do_build (ChTy const* const expr)
	{
		pars_error_detect < cme_create_error >
			(boost::spirit::parse (expr, this->cme_grammar_, boost::spirit::space_p));
	};
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_RPN_CREATOR_IMPL_HPP

/**
* 18.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
