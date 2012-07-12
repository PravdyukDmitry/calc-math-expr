/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file me_calculator_impl.hpp                                          *
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

#ifndef CME_ME_CALCULATOR_IMPL_HPP
 #define CME_ME_CALCULATOR_IMPL_HPP

#include <cme/me_calculator_impl_helpers.hpp>
#include <cme/cme_expression_generator.hpp>
#include <cme/string_to_real_cast.hpp>
#include <cme/cme_exception.hpp>

#include <stack>

namespace cme { namespace cme_detail {

template	<
	typename CharTy
	, typename RealTy
	, template < typename, typename > class VarTlbTy
	, template < typename > class ConstTlbTy
	, template < typename, typename > class UMFuncTlbTy 
			>
class me_calculator_impl
{
	typedef std::stack < RealTy > internal_stack_type;

	me_calculator_impl const& operator = (me_calculator_impl const&);
public:
	typedef CharTy	char_type;
	typedef RealTy	real_type;

	typedef VarTlbTy < CharTy, RealTy >		variable_table_type;
	typedef ConstTlbTy < CharTy >			constant_table_type;
	typedef UMFuncTlbTy < CharTy, RealTy >	unary_math_function_table_type;

	typedef cme_expression_generator < char_type >			generator_type;
	typedef typename generator_type::token_type				token_type;
	typedef typename generator_type::token_data_type		token_data_type;
	typedef typename generator_type::type					cme_expression_type;
	typedef typename cme_expression_type::const_iterator	iterator_type;

	me_calculator_impl	(variable_table_type *const p_vtlb, constant_table_type const* const p_ctlb, 
		unary_math_function_table_type const* const p_umftlb)
		: stack_ (), tmp_value_ (), p_vtlb_ (p_vtlb), p_ctlb_ (p_ctlb), p_umftlb_ (p_umftlb) 
	{
	};

	template < typename IterTy >
	real_type const calculate (IterTy it_, IterTy const& end_)
	{
		while (it_ != end_)
			this->calculate_helper(*it_++);

		if (this->stack_.size () == 1)
		{
			real_type result = this->stack_.top ();
			this->stack_.pop();
			return result;
		}
		else 
			throw bad_end_calculate_condition_error ();
	};

private:
	internal_stack_type							stack_;
	real_type									tmp_value_;
	variable_table_type *const					p_vtlb_;
	constant_table_type const* const			p_ctlb_;
	unary_math_function_table_type const* const	p_umftlb_;

	template < typename OpTy >
	void build_op (OpTy const& op_)
	{
		this->tmp_value_ = this->stack_.top ();
		this->stack_.pop ();
		this->stack_.top () = op_ (this->stack_.top (), this->tmp_value_);
	};

	template < typename TokTy >
	void calculate_helper (TokTy const& tok_)
	{
		switch (tok_->get_id ())
		{
		case id_number: 
			this->stack_.push (string_to_real_cast < real_type > (tok_->get_name ())); 
			break;
		case id_variable: 
			{
			if (this->p_vtlb_ == NULL)
				throw null_variables_table_error ();
			this->stack_.push (this->p_vtlb_->at (tok_->get_name ()));
			break;
			}
		case id_constant: 
			{
			if (this->p_ctlb_ == NULL)
				throw null_constant_table_error ();
			this->stack_.push (string_to_real_cast < real_type > (this->p_ctlb_->at(tok_->get_name ())));
			break;
			}
		case id_plus: 
			this->build_op (std::plus < real_type > ());
			break;
		case id_minus: 
			this->build_op (std::minus < real_type > ());
			break;
		case id_mul: 
			this->build_op (std::multiplies < real_type > ());
			break;
		case id_div: 
			this->build_op (std::divides < real_type > ());
			break;
		case id_unary_minus:
			this->stack_.top () = -this->stack_.top ();
			break;
		case id_power: case id_power_function:
			this->build_op (power_helper < real_type > ());
			break;
		case id_root_function: 
			this->build_op (root_helper < real_type > ());
			break;
		case id_function_name:
			{
			if (this->p_umftlb_ == NULL)
				throw null_unary_math_function_table_error ();
			this->stack_.top () = this->p_umftlb_->at(tok_->get_name ()) (this->stack_.top ());
			break;
			}
		default:	
			throw unknown_token_type (*tok_);
		}
	};
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_ME_CALCULATOR_IMPL_HPP

/**
* 18.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
