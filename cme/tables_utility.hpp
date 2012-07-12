/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file tables_utility.hpp                                              *
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

#ifndef CME_TABLES_UTILITY_HPP
 #define CME_TABLES_UTILITY_HPP

#include <cme/real_parser_generator.hpp>

namespace cme { namespace cme_detail {

template < typename TlbTy >
class table_initer
{
public:
	typedef TlbTy table_type;
	typedef typename table_type::record_type record_type;
	typedef typename table_type::name_type name_type;
	typedef typename table_type::value_type value_type;

	table_initer (table_type& tlb)
		: tlb_ (tlb) 
	{
	};

	table_initer const& operator () 
		(name_type const& name_, value_type const& value_ = value_type ()) const
	{
		this->tlb_.add (name_, value_);
		return (*this);
	};

	template < typename IterTy >
	table_initer const& operator () 
		(IterTy begin_name_, IterTy const& end_name_, 
			value_type const& value = value_type ()) const
	{
		(*this) (name_type (begin_name_, end_name_), value);
		return (*this);
	};

	table_initer const& operator () (record_type const& rec_) const
	{
		this->tlb_.add (rec_);
		return (*this);
	};

private:
	table_type& tlb_;
};

}; /// end namespace cme::cme_detail

/**
* Удобный иницыализатор для таблиц...
* Пример:
* TlbTy my_var_table;
* init_table (my_var_table) ("x", 1.05) ("y") ("z", my_var_table ["x"]);
**/
template < typename TlbTy >
cme_detail::table_initer < TlbTy > const init_table (TlbTy& tlb_)
{
	return cme_detail::table_initer < TlbTy > (tlb_);
};

namespace cme_detail {

template < typename VarTlbTy >
class add_var_helper
{
public:
	typedef VarTlbTy variable_table_type;
	typedef typename variable_table_type::name_type key_type;
	typedef typename variable_table_type::value_type value_type;

	add_var_helper (variable_table_type const& vtlb, key_type& key)
		: vtlb_ (const_cast < variable_table_type& > (vtlb)), key_ (key) 
	{
	};

	template < typename IterTy >
	void operator () (IterTy const& begin, IterTy const& end) const
	{
		this->vtlb_.add 
			(this->key_.replace (this->key_.begin (), this->key_.end (), begin, end));
	};

	void operator () (value_type const& value_) const
	{
		this->vtlb_ [this->key_] = value_;
	};

private:
	variable_table_type& vtlb_;
	key_type& key_;
};

template < typename VarTlbTy >
add_var_helper < VarTlbTy > const 
	var_adder (VarTlbTy const& vtlb_, typename VarTlbTy::name_type& name_buff_)
{
	return add_var_helper < VarTlbTy > (vtlb_, name_buff_);
};

template < typename VarTlbTy >
class variable_list_grammar
	: public boost::spirit::grammar < variable_list_grammar < VarTlbTy > >
{
	typedef boost::spirit::grammar < variable_list_grammar < VarTlbTy > > base_type;
public:
	typedef VarTlbTy variable_table_type;

	variable_list_grammar (variable_table_type const& vtlb)
		: base_type (), vtlb_ (vtlb) 
	{
	};

	template < typename ScannerTy >
	class definition
	{
		friend class variable_list_grammar;
		typedef boost::spirit::rule < ScannerTy > rule_type;
		typedef typename cme_detail::real_parser_generator < 
			typename variable_table_type::value_type >::type real_parser_type;
	public:
		definition (variable_list_grammar const& self)
			: real_pars_ (), name_ ()
		{	
			var_name_ = boost::spirit::lexeme_d [+('_' | boost::spirit::alpha_p) 
				>> *(boost::spirit::alnum_p | '_')];

			var_decl_ = var_name_ [var_adder (self.vtlb_, name_)] 
				>> !('=' >> real_pars_ [var_adder (self.vtlb_, name_)]);

			var_list_ = var_decl_ % ',';
		};
		
		rule_type const& start () const 
		{
			return this->var_list_; 
		};

	private: 
		rule_type var_name_, var_decl_, var_list_; 
		typename variable_table_type::name_type name_;
		real_parser_type const real_pars_; 
	};

private:
	variable_table_type const& vtlb_;
};

template < typename ErrTy, typename PInfTy >
void pars_error_detect (PInfTy const& pi_)
{
	if(!pi_.full) 
		throw ErrTy (*pi_.stop);
};

template < template < typename > class VarListGrammarTy
	, typename VarTlbTy, typename IterTy >
void init_variable_table_helper (VarTlbTy& vtlb_, IterTy begin_var_list_, IterTy const& end_var_list_)
{
	cme_detail::pars_error_detect < cme_detail::parse_variable_list_error >
		(boost::spirit::parse (begin_var_list_, end_var_list_, 
		VarListGrammarTy < VarTlbTy > (vtlb_), boost::spirit::space_p));
};

}; /// end namespace cme::cme_detail

/**
* Иницыализаторы для таблиц переменных...
* На вход подаётся таблица которую нужно инициализировать и список переменных и их значений,
* заданный интервалом, Си-строкой или STL-подобным контейнером...
* Формат передаваемого списка: 
*	имя_переменной, имя_переменной = значение_переменной, и т.д. ...
* Пример: " x = 1.05, y , z = 9.67" 
*	- переменная 'y' будет иницыализированна конвтруктором по умолчанию для типа VarTlbTy::value_type
*
* !!! ВАЖНО !!!
*	Вызывающий код должен заботиться о том, чтобы список переменных НЕ заканчивались пробелами,
*	табами, и д.р. подобными символами,
*	т.е. ТАК НЕЛЬЗЯ:
*		Список переменных -> "x = 1.05, y, z = 9.67 " или "x = 1.05, y, z "
*	а ТАК МОЖНО:
*		Список переменных -> "x = 1.05, y, z = 9.67" или "x = 1.05, y, z"
* Вызвано это скорее всего моими кревыми руками, извиняюсь за неудобства ;)
**/

template < typename VarTlbTy, typename IterTy >
void init_variable_table (VarTlbTy& vtlb_, IterTy begin_var_list_, IterTy const& end_var_list_)
{
	cme_detail::init_variable_table_helper < cme_detail::variable_list_grammar > (
		vtlb_, begin_var_list_, end_var_list_);
};

template < typename VarTlbTy, typename StrTy >
void init_variable_table (VarTlbTy& vtlb_, StrTy const& var_list_)
{
	init_variable_table (vtlb_, var_list_.begin (), var_list_.end ());
};

template < typename VarTlbTy, typename CharTy >
void init_variable_table (VarTlbTy& vtlb_, CharTy const* const var_list_)
{
	cme_detail::pars_error_detect < cme_detail::parse_variable_list_error >
		(boost::spirit::parse (var_list_,
		cme_detail::variable_list_grammar < VarTlbTy > (vtlb_), boost::spirit::space_p));
};

}; /// end namespace cme

#endif /// CME_TABLES_UTILITY_HPP

/**
* 10.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
