/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file priority_t.hpp                                                  *
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

#ifndef CME_PRIORITY_T_HPP
 #define CME_PRIORITY_T_HPP

namespace cme { namespace cme_detail {

enum priority_t
{
	 pt_opening_bracket
	, pt_plus_minus
	, pt_mul_div
	, pt_power
	, pt_unary_minus
	, pt_function_name
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_PRIORITY_T_HPP

/**
* 16.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
