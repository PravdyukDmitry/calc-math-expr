/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file lexem_creator.hpp                                               *
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

#ifndef CME_LEXEM_CREATOR_HPP
 #define CME_LEXEM_CREATOR_HPP

namespace cme { namespace cme_detail {

template < typename CharTy >
struct lexem_creator;

template <>
struct lexem_creator < char >
{
	static char const* const get_power () 
	{
		return "pow";
	};

	static char const* const get_root () 
	{
		return "root";
	};
};

template <>
struct lexem_creator < wchar_t >
{
	static wchar_t const* const get_power () 
	{
		return L"pow";
	};

	static wchar_t const* const get_root () 
	{
		return L"root";
	};
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_LEXEM_CREATOR_HPP

/**
* 14.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
