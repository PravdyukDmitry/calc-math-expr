/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file token_t.hpp                                                     *
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

#ifndef CME_TOKEN_T_HPP
 #define CME_TOKEN_T_HPP

#include <string>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

namespace cme { namespace cme_detail {

template < typename CharTy = char, typename IDTy = int >
class token_t
{
public: 
	typedef CharTy	char_type;
	typedef IDTy	id_type;
	typedef std::basic_string < char_type > string_type;

	token_t ()
		: name_ (), id_ () 
	{
	};

	token_t (string_type const& tok_name, id_type const id)
		: name_ (tok_name), id_ (id)  
	{
	};

	template < typename IterTy >
	token_t (IterTy const begin_, IterTy const end_, id_type const id)
		: name_ (begin_, end_), id_ (id) 
	{
	};

	token_t (char_type const tok_name, id_type const id)
		: name_ (1, tok_name), id_ (id) 
	{
	};

	token_t (token_t const& right)
		: name_ (right.name_), id_ (right.id_) 
	{
	};

	template < typename IterTy >
	void set_name (IterTy const beg, IterTy const end)
	{
		this->name_.replace 
			(this->name_.begin (), this->name_.end (), beg, end);
	};

	void set_name (char_type const op)
	{
		this->name_.errase ();
		this->name_.push_back (op);
	};

	string_type const& get_name () const
	{
		return this->name_;
	};

	void set_id (id_type const id)
	{
		this->id_ = id;
	};

	id_type const get_id () const
	{
		return this->id_;
	};

private:
	string_type	name_;
	id_type		id_;

	friend class boost::serialization::access;
 
	template < typename XMLArchiveTy >
	void serialize (XMLArchiveTy& archive_, unsigned int const version_)
	{
		using namespace boost::serialization;

		archive_ & make_nvp ("token_name", this->name_);
		archive_ & make_nvp ("token_id", this->id_);
	};
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_TOKEN_T_HPP

/**
* 13.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
