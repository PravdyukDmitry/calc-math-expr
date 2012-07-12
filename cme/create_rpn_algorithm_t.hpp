/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file create_rpn_algorithm_t.hpp                                      *
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

#ifndef CME_CREATE_RPN_ALGORITHM_T_HPP
 #define CME_CREATE_RPN_ALGORITHM_T_HPP

#include <cme/token_id.hpp>
#include <cme/priority_t.hpp>
#include <cme/cme_exception.hpp>

#include <stack>
#include <functional>

namespace cme { namespace cme_detail {

template < typename TokenTy >
priority_t const get_priority_t (TokenTy const& tok)
{
	switch (tok->get_id ())
	{
		case id_open_bracket:			return pt_opening_bracket;
		case id_plus: case id_minus:	return pt_plus_minus;
		case id_mul: case id_div:		return pt_mul_div;
		case id_unary_minus:			return pt_unary_minus;
		case id_function_name:
		case id_power_function:
		case id_root_function:			return pt_function_name;
		case id_power:					return pt_power;
		default:						
			throw get_priority_t_error (tok);
	}	
};

template < typename TokTy >
class rpn_creator_internal_stack_value_wrapper
	: virtual public TokTy
{
	typedef TokTy base_type;
public:
	rpn_creator_internal_stack_value_wrapper (base_type const& tok)
		: base_type (tok)
	{
	};

	operator priority_t const () const
	{
		return get_priority_t (*this);
	};
};

template < typename OutCMEExprTy >
class create_rpn_algorithm_t
{
public:
	typedef OutCMEExprTy cme_expression_type;
	typedef typename cme_expression_type::value_type expr_value_type;
	typedef rpn_creator_internal_stack_value_wrapper < expr_value_type > steck_value_type;
	typedef std::stack < steck_value_type > token_stack_type;

	create_rpn_algorithm_t ()
		: token_stack_ (), out_cme_expr_ () 
	{
	};

	/// Запускает алгоритм ...
	void operator () (expr_value_type const& tok_)
	{
		switch (tok_->get_id ())
		{
		case id_coma: 
			this->unload_expr ();
			break;
		case id_open_bracket:
			this->token_stack_.push (tok_);
			break;
		case id_closed_bracket:
			{
				this->unload_expr ();
				this->token_stack_.pop ();
				break;
			}
		case id_number: case id_variable: case id_constant:
			this->out_cme_expr_.push_back (tok_);
			break;
		case id_power:
			{
				while (this->cond (!this->token_stack_.empty () && this->token_stack_.top () > pt_power))
					this->add_tok();
				this->token_stack_.push (tok_);
				break;
			}
		default:
				this->alg_execute (tok_);
				break;
		}
	};

	/// Возвращает готовую ОПЗ
	cme_expression_type const& get_cme_expr () const
	{
		this->do_release ();
		return this->out_cme_expr_;
	};

	cme_expression_type& get_cme_expr ()
	{
		this->do_release ();
		return this->out_cme_expr_;
	};

	void release ()
	{
		this->do_release ();
		this->out_cme_expr_.clear ();
	};

	bool const is_released () const
	{
		return (this->token_stack_.empty () && this->out_cme_expr_.empty ());
	};

private:
	token_stack_type	token_stack_;
	cme_expression_type	out_cme_expr_;

	void do_release ()
	{
		while (!this->token_stack_.empty ())
			this->add_tok ();
	};

	void add_tok ()
	{
		this->out_cme_expr_.push_back (this->token_stack_.top ());
		this->token_stack_.pop ();
	};

	void unload_expr ()
	{
		while (this->token_stack_.top () != pt_opening_bracket)
			this->add_tok();
	};

	bool const cond (bool const state_) const
	{
		return (!this->token_stack_.empty () && state_);
	};

	bool const cond (priority_t const state_) const
	{
		return this->cond (this->token_stack_.top () >= state_);
	};

	void alg_execute (steck_value_type const& tok_and_prior_)
	{
		priority_t const tmp_prior_ = tok_and_prior_;

M1:		if (this->token_stack_.empty () || this->token_stack_.top () < tmp_prior_)
		{
			this->token_stack_.push (tok_and_prior_);
			return;
		}

		if (this->cond (tmp_prior_))
		{
			this->add_tok();
			
			//while (this->cond (tmp_prior_))
				//this->add_tok();

			goto M1;
		}
	};
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_CREATE_RPN_ALGORITHM_T_HPP

/**
* 30.05.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
