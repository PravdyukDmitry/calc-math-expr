/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file cme_expression.hpp                                              *
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

#ifndef CME_CME_EXPRESSION_HPP
 #define CME_CME_EXPRESSION_HPP

#include <vector>

#include <cme/save_load_cme_expression.hpp>

namespace cme {

template < typename Ty, /** Под параметром Ty подразумевается что то 
	вроде этого = boost::shared_ptr < token_t < CharTy, rpn_token_id_t > > **/
	typename AllocTy = std::allocator < Ty > >
class cme_expression
{
	typedef std::vector < Ty, AllocTy > super_type;
public:
	
	typedef typename super_type::value_type				value_type;
	typedef typename value_type::element_type			internal_element_type;
	typedef typename super_type::allocator_type			allocator_type;
	typedef typename super_type::size_type				size_type;
	typedef typename super_type::iterator				iterator;
	typedef typename super_type::const_iterator			const_iterator;
	typedef typename super_type::reverse_iterator		reverse_iterator;
	typedef typename super_type::const_reverse_iterator	const_reverse_iterator;
	typedef typename internal_element_type::string_type	string_type;
	typedef typename internal_element_type::char_type	char_type;
	
	cme_expression ()
		: buff_ () 
	{
	};

	template < typename IterTy >
	cme_expression (IterTy first_, IterTy last_)
		: buff_ (first_, last_) 
	{
	};

	cme_expression (cme_expression const& right_)
		: buff_ (right_.buff_) 
	{
	};

	cme_expression& operator = (cme_expression const& right_)
	{
		this->buff_ = right_.buff_;
		return (*this);
	};

	operator super_type& ()
	{
		return this->buff_;
	};

	operator super_type const& () const
	{
		return this->buff_;
	};

	iterator begin ()
	{
		return this->buff_.begin ();
	};

	const_iterator begin () const
	{
		return this->buff_.begin ();
	};

	iterator end ()
	{
		return this->buff_.end ();
	};

	const_iterator end () const
	{
		return this->buff_.end ();
	};

	reverse_iterator rbegin ()
	{
		return this->buff_.rbegin ();
	};

	const_reverse_iterator rbegin () const
	{
		return this->buff_.rbegin ();
	};

	reverse_iterator rend ()
	{
		return this->buff_.rend ();
	};

	const_reverse_iterator rend () const
	{
		return this->buff_.rend ();
	};

	size_type const size () const
	{
		return this->buff_.size ();
	};

	bool const empty () const
	{
		return this->buff_.empty ();
	};

	void clear ()
	{
		this->buff_.clear ();
	};

	void push_back (value_type const& value_)
	{
		this->buff_.push_back (value_);
	};

	void swap (cme_expression const& right_)
	{
		this->buff_.swap (right_.buff_); 
	};

	allocator_type const& get_allocator () const
	{
		return this->buff_.get_allocator ();
	};

private:
	super_type buff_;
};

namespace cme_detail {

template < typename CMEExprTy >
class cme_expression_to_string_helper
{
public:
	typedef CMEExprTy expression_type;
	typedef typename expression_type::string_type string_type;
	typedef typename expression_type::char_type char_type;
	typedef typename expression_type::const_iterator iterator_type;

	cme_expression_to_string_helper (char_type const tok_delim = '|',
		char_type const unary_minus_substitute = '!')
		: it_ (), tok_delim_ (tok_delim), 
		unary_minus_substitute_ (unary_minus_substitute) 
	{
	};

	void operator () (expression_type const& cme_expr_, string_type& out_str_)
	{
		this->it_ = cme_expr_.begin ();
		iterator_type const end_ (--cme_expr_.end ());

		while (this->it_ != end_)
		{
			this->add (out_str_);
			out_str_.push_back (this->tok_delim_);
			++(this->it_);
		}
		this->add (out_str_);
	};

private:
	iterator_type it_;
	char_type const tok_delim_, unary_minus_substitute_;

	void do_add (string_type& out_str_) const
	{
		out_str_.append ((*(this->it_))->get_name ());
	};

	void do_um_add (string_type& out_str_) const
	{
		out_str_.push_back (this->unary_minus_substitute_);
	};

	void add (string_type& out_str_) const
	{
		((*(this->it_))->get_id () == id_unary_minus) ?
			this->do_um_add (out_str_) : this->do_add (out_str_);
	};
};

}; /// end namespace cme::cme_detail

template < typename CMEExprTy >
typename CMEExprTy::string_type 
cme_expression_to_string (CMEExprTy const& cme_expr_, 
	typename CMEExprTy::char_type const tok_delim_ = '|', 
	typename CMEExprTy::char_type const unary_minus_substitute_ = '!')
{
	typename CMEExprTy::string_type buff_;
	cme_detail::cme_expression_to_string_helper < CMEExprTy > 
		(tok_delim_, unary_minus_substitute_) (cme_expr_, buff_);
	return buff_;
};

template < typename OsTy, typename Ty, typename AllocTy >
OsTy& operator << (OsTy& os_, cme_expression < Ty, AllocTy > const& expr_)
{
	return os_ << cme_expression_to_string (expr_);
};

}; /// end namespace cme

#endif /// CME_CME_EXPRESSION_HPP

/**
* 03.06.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
