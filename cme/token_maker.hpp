/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file token_maker.hpp                                                 *
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

#ifndef CME_TOKEN_MAKER_HPP
 #define CME_TOKEN_MAKER_HPP

#include <cme/token_id.hpp>

#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/integral_c.hpp>

namespace cme { namespace cme_detail {

template < rpn_token_id_t id_, bool, typename CreateRpnAlgTy >
class token_maker_helper
{
	typedef typename CreateRpnAlgTy::expr_value_type expr_value_type;
	typedef typename expr_value_type::element_type element_type;
public:
	typedef CreateRpnAlgTy create_rpn_algorithm_type;

	token_maker_helper (create_rpn_algorithm_type& crpnalg)
		: crpnalg_ (crpnalg) 
	{
	};

	template < typename IterTy >
	void operator () (IterTy const begin_, IterTy const end_) const
	{
		this->crpnalg_ (expr_value_type (new element_type (begin_, end_, id_)));
	};

private:
	create_rpn_algorithm_type& crpnalg_;
};

template < rpn_token_id_t id_, typename CreateRpnAlgTy >
class token_maker_helper < id_, false, CreateRpnAlgTy >
{
	typedef typename CreateRpnAlgTy::expr_value_type expr_value_type;
	typedef typename expr_value_type::element_type element_type;
public:
	typedef CreateRpnAlgTy create_rpn_algorithm_type;
	
	token_maker_helper (create_rpn_algorithm_type& crpnalg)
		: crpnalg_ (crpnalg) 
	{
	};

	template < typename CharTy >
	void operator () (CharTy const op_) const
	{
		this->crpnalg_ (expr_value_type (new element_type (op_, id_)));
	};

private:
	create_rpn_algorithm_type& crpnalg_;
};

template < rpn_token_id_t id_ >
struct two_iter_inp_t
{
	typedef boost::mpl::contains <
		boost::mpl::vector_c 
		< rpn_token_id_t 
			, id_number 
			, id_variable
			, id_constant
			, id_function_name
			, id_power_function
			, id_root_function
		>
		, boost::mpl::integral_c < rpn_token_id_t, id_ > > type;
};

template < rpn_token_id_t id_, typename CreateRpnAlgTy >
struct token_maker
{
	typedef token_maker_helper < id_, 
		two_iter_inp_t < id_ >::type::value, CreateRpnAlgTy > type;
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_TOKEN_MAKER_HPP

/**
* 18.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
