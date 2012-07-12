/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file cme_expression_generator.hpp                                    *
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

#ifndef CME_CME_EXPRESSION_GENERATOR_HPP
 #define CME_CME_EXPRESSION_GENERATOR_HPP

#include <cme/token_t.hpp>
#include <cme/token_id.hpp>
#include <cme/cme_expression.hpp>

#include <boost/shared_ptr.hpp>

namespace cme { namespace cme_detail {

template < typename CharTy > 
struct cme_expression_generator
{
	typedef token_t < CharTy, rpn_token_id_t >		token_data_type; 
	typedef boost::shared_ptr < token_data_type >	token_type;
	typedef cme_expression < token_type >			type;
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_CME_EXPRESSION_GENERATOR_HPP

/**
* 10.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
