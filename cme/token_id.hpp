/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file token_id.hpp                                                    *
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

#ifndef CME_TOKEN_ID_HPP
 #define CME_TOKEN_ID_HPP

namespace cme { namespace cme_detail {

enum rpn_token_id_t
{
	id_coma
	, id_open_bracket
	, id_closed_bracket
	, id_number
	, id_variable
	, id_constant
	, id_plus
	, id_minus
	, id_mul
	, id_div
	, id_power
	, id_unary_minus
	, id_function_name
	, id_power_function
	, id_root_function
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_TOKEN_ID_HPP

/**
* 16.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
