/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file save_load_cme_expression.hpp                                    *
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

#ifndef CME_SAVE_LOAD_CME_EXPRESSION_HPP
 #define CME_SAVE_LOAD_CME_EXPRESSION_HPP

#include <cme/cme_exception.hpp>

#include <fstream>

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

namespace cme {

template < typename CharTy, typename CMEExprTy >
void save (CharTy const* const save_file_name_, CMEExprTy const& expr_)
{
	std::ofstream ofs_ (save_file_name_, std::ios::out);

	if (!ofs_)
	{
		ofs_.clear ();
		ofs_.close ();
		throw cme_detail::cme_expression_save_error (save_file_name_);
	}

	boost::archive::xml_oarchive oa_ (ofs_);

	oa_ << boost::serialization::make_nvp ("cmelib_cme_expression_file", 
		static_cast < std::vector < typename CMEExprTy::value_type > const& > (expr_));
};

template < typename CharTy, typename CMEExprTy >
void load (CharTy const* const load_file_name_, CMEExprTy& expr_)
{
	std::ifstream ifs_ (load_file_name_, std::ios::in);

	if (!ifs_)
	{
		ifs_.clear ();
		ifs_.close ();
		throw cme_detail::cme_expression_load_error (load_file_name_);
	}

	boost::archive::xml_iarchive ia_ (ifs_);

	if (!expr_.empty ())
		expr_.clear ();

	ia_ >> boost::serialization::make_nvp ("cmelib_cme_expression_file", 
		static_cast < std::vector < typename CMEExprTy::value_type >& > (expr_));
};

}; /// end namespace cme

#endif /// CME_SAVE_LOAD_CME_EXPRESSION_HPP

/**
* 17.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
