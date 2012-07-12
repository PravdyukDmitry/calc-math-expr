/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file make_cme_grammar.hpp                                            *
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

#ifndef CME_MAKE_CME_GRAMMAR_HPP
 #define CME_MAKE_CME_GRAMMAR_HPP

#include <cme/token_maker.hpp>
#include <cme/lexem_creator.hpp>
#include <cme/cme_exception.hpp>

#include <boost/spirit.hpp>
#include <boost/mpl/empty_base.hpp>

namespace cme { namespace cme_detail {

template < typename ScannerTy > 
class cme_grammar_definition
{
public:
		typedef boost::spirit::rule < ScannerTy > rule_type;

		template < typename MakeCMESelfGrammarTy >
		cme_grammar_definition (MakeCMESelfGrammarTy const& self)
			: um_function_name (self.get_umf_table ())
			, variable (self.get_var_table ())
			, constant (self.get_const_table ())
		{	
			fractional_constant =
				(
					*boost::spirit::digit_p 
					>> boost::spirit::ch_p ('.') 
					>> +boost::spirit::digit_p
				)|
				(
					+boost::spirit::digit_p 
					>> boost::spirit::ch_p ('.')
				);
			
			exponent_part =
				(
					boost::spirit::ch_p ('e') 
					| boost::spirit::ch_p ('E')
				)
				>> !(
					boost::spirit::ch_p ('+')
					| boost::spirit::ch_p ('-')
				) 
				>> +boost::spirit::digit_p;

			floating_literal =
				fractional_constant 
				>> !exponent_part
				|  +boost::spirit::digit_p 
				>> exponent_part;

			typedef typename MakeCMESelfGrammarTy::create_rpn_algorithm_type create_rpn_algorithm_type;

			create_rpn_algorithm_type& alg_ (
				const_cast < create_rpn_algorithm_type& > (self.get_create_rpn_algorithm ()));

#define CME_TOKEN_MAKER_MAKROS(ID) typename token_maker < ID, create_rpn_algorithm_type >::type (alg_)

			expression_is_in_brackets = 
				boost::spirit::ch_p ('(') [CME_TOKEN_MAKER_MAKROS (id_open_bracket)]
				>> expression 
				>> boost::spirit::ch_p (')') [CME_TOKEN_MAKER_MAKROS (id_closed_bracket)];

			unary_plus_or_minus =
				(
					boost::spirit::ch_p ('-') [CME_TOKEN_MAKER_MAKROS (id_unary_minus)] 
					| boost::spirit::ch_p ('+')
				) >> factor;

			factor =
				(floating_literal | +boost::spirit::digit_p) [CME_TOKEN_MAKER_MAKROS (id_number)]
				|   variable [CME_TOKEN_MAKER_MAKROS (id_variable)]
				|	constant [CME_TOKEN_MAKER_MAKROS (id_constant)]
				|	um_function 
				|	binary_function
				|   expression_is_in_brackets
				|	unary_plus_or_minus;

			power_rule = 
				factor 
				>> *(
					boost::spirit::ch_p ('^') [CME_TOKEN_MAKER_MAKROS (id_power)] 
					>> factor
				);

			term =
				power_rule
				>> *(
						(
							boost::spirit::ch_p ('*') [CME_TOKEN_MAKER_MAKROS (id_mul)] 
							>> power_rule
						)|
						(
							boost::spirit::ch_p ('/') [CME_TOKEN_MAKER_MAKROS (id_div)] 
							>> power_rule
						)
				);

			expression =
				term 
				>> *(
						(
							boost::spirit::ch_p ('+') [CME_TOKEN_MAKER_MAKROS (id_plus)] 
							>> term
						)|
						(
							boost::spirit::ch_p ('-') [CME_TOKEN_MAKER_MAKROS (id_minus)] 
							>> term
						)
				);

			um_function =
				um_function_name [CME_TOKEN_MAKER_MAKROS (id_function_name)]
				>> expression_is_in_brackets;

			typedef typename MakeCMESelfGrammarTy::char_type char_type;

			binary_function =
				(
					boost::spirit::str_p (lexem_creator < char_type >::get_power ()) [CME_TOKEN_MAKER_MAKROS (id_power_function)] 
					|	boost::spirit::str_p (lexem_creator < char_type >::get_root ()) [CME_TOKEN_MAKER_MAKROS (id_root_function)] 
				) 
				>> boost::spirit::ch_p ('(') [CME_TOKEN_MAKER_MAKROS (id_open_bracket)]
				>> expression
				>> boost::spirit::ch_p (',') [CME_TOKEN_MAKER_MAKROS (id_coma)]
				>> expression 
				>> boost::spirit::ch_p (')') [CME_TOKEN_MAKER_MAKROS (id_closed_bracket)];

#undef CME_TOKEN_MAKER_MAKROS
		};

		virtual ~cme_grammar_definition () = 0;
		
		rule_type const& get_start () const
		{
			return this->expression;
		};

	private: 
		rule_type	floating_literal, fractional_constant, exponent_part,
					variable, constant, um_function, um_function_name,
					unary_plus_or_minus, expression_is_in_brackets, 
					expression, term, factor, binary_function, power_rule; 
};

template < typename ScannerTy > 
cme_grammar_definition < ScannerTy >::~cme_grammar_definition ()
{
};

///////////////////////////////////////////////////////////////////////////////////////////

template < typename CharTy, typename CreateRPNAlgTy >
class make_cme_grammar_interface
{
public:

	typedef CharTy						char_type;
	typedef boost::spirit::symbols < 
		boost::mpl::empty_base, char_type >
										base_tlb_type;
	typedef base_tlb_type				const_table_type;
	typedef base_tlb_type				var_table_type;
	typedef base_tlb_type				umf_table_type;
	typedef CreateRPNAlgTy				create_rpn_algorithm_type;
	typedef typename 
		create_rpn_algorithm_type::cme_expression_type cme_expression_type;

	virtual ~make_cme_grammar_interface () = 0;

/// Интерфейс ...

///		Инициализация таблиц...

	template < typename VarTlbTy >
	void init_var_table (VarTlbTy const* const p_var_tlb)
	{
		if (p_var_tlb != NULL) 
			this->init_tlb_helper (
				get_var_table (), p_var_tlb
				, make_cme_grammar_init_error (
					"Error at addition the variable name in grammar !!! :(")
				);
	};

	template < typename ConstTlbTy >
	void init_const_table (ConstTlbTy const* const p_const_tlb)
	{
		if (p_const_tlb != NULL)
			this->init_tlb_helper (
				get_const_table (), p_const_tlb, 
				make_cme_grammar_init_error (
					"Error at addition the name of constant in grammar !!! :(")
				);
	};

	template < typename UMFuncTlbTy >
	void init_umf_table (UMFuncTlbTy const* const p_umf_tlb)
	{
		if (p_umf_tlb != NULL)
			this->init_tlb_helper (
				get_umf_table (), p_umf_tlb, 
				make_cme_grammar_init_error ("Error at addition the name of "
					"unary mathematical function in grammar !!! :(")
				);
	};

///		Доступ ...

	/// Доступ к таблице переменных
	var_table_type const& get_var_table () const
	{
		return this->vtlb_;
	};

	var_table_type& get_var_table ()
	{
		return this->vtlb_;
	};

	/// Доступ к таблице констант
	const_table_type const& get_const_table () const
	{
		return this->ctlb_;
	};

	const_table_type& get_const_table ()
	{
		return this->ctlb_;
	};

	/// Доступ к таблице унарных математических функций
	umf_table_type const& get_umf_table () const
	{
		return this->umftlb_;
	};

	umf_table_type& get_umf_table ()
	{
		return this->umftlb_;
	};

	/// Доступ к алгритму создающему ОПЗ
	create_rpn_algorithm_type const& get_create_rpn_algorithm () const
	{
		return this->alg_;
	};

	create_rpn_algorithm_type& get_create_rpn_algorithm ()
	{
		return this->alg_;
	};

	/// Доступ к готовому ОПЗ выражению
	cme_expression_type const& get_cme_expr () const
	{
		return this->alg_.get_cme_expr ();
	};

	cme_expression_type& get_cme_expr ()
	{
		return this->alg_.get_cme_expr ();
	};

protected:
	template < typename OutTlb, typename InpTlb, typename ErrTy >
	void init_tlb_helper (OutTlb& otlb_, InpTlb const* const itlb_, ErrTy const& err_)
	{
		typedef typename InpTlb::const_iterator iterator_type;
		iterator_type it_ = itlb_->begin();
		iterator_type const end_ = itlb_->end();
		
		while (it_ != end_)
			(boost::spirit::add (otlb_, it_->first.c_str ()) == NULL)
				? throw err_ : ++it_;
	};

private:
	var_table_type				vtlb_;
	const_table_type			ctlb_;
	umf_table_type				umftlb_;
	create_rpn_algorithm_type	alg_;
};

template < typename CharTy, typename CreateRPNAlgTy >
make_cme_grammar_interface < CharTy, CreateRPNAlgTy >::~make_cme_grammar_interface ()
{
};

///////////////////////////////////////////////////////////////////////////////////////////

template < typename CharTy, typename CreateRPNAlgTy >
class make_cme_grammar
	: public make_cme_grammar_interface < CharTy, CreateRPNAlgTy >
	, public boost::spirit::grammar < make_cme_grammar < CharTy, CreateRPNAlgTy > >
{
	typedef make_cme_grammar						this_type;
	typedef boost::spirit::grammar < this_type >	super_type;
	typedef make_cme_grammar_interface < CharTy, CreateRPNAlgTy > interface_type;

public:

	make_cme_grammar ()
		: interface_type (), super_type ()
	{
	};

	template < typename VarTlbTy, typename ConstTlbTy, typename UMFuncTlbTy >
	make_cme_grammar (VarTlbTy const* const p_var_tlb, 
		ConstTlbTy const* const p_const_tlb, UMFuncTlbTy const* const p_umf_tlb)
		: interface_type (), super_type ()
	{
		init_var_table (p_var_tlb);	
		init_const_table (p_const_tlb);
		init_umf_table (p_umf_tlb);
	};

	virtual ~make_cme_grammar ()
	{
	};

	template < typename ScannerTy > 
	class definition
		: public cme_grammar_definition < ScannerTy >
	{
		friend class make_cme_grammar;

	public:
		typedef cme_grammar_definition < ScannerTy > base_grammar_definition_type;
		typedef typename base_grammar_definition_type::rule_type rule_type;

		definition (make_cme_grammar const& self)
			: base_grammar_definition_type (self)
		{	
		};

		virtual ~definition ()
		{
		};
		
		rule_type const& start () const
		{
			return this->get_start ();
		};
	}; /// end class definition
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_MAKE_CME_GRAMMAR_HPP

/**
* 13.06.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
