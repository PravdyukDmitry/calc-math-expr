/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file table_base_impl.hpp                                             *
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

#ifndef CME_TABLE_BASE_IMPL_HPP
 #define CME_TABLE_BASE_IMPL_HPP

#include <cme/cme_exception.hpp>

#include <string>

#include <boost/unordered_map.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/add_const.hpp>

namespace cme { namespace cme_detail {

template	<
	typename CharTy
	, typename ValTy 
	, typename InternalMapTy = boost::unordered_map < std::basic_string < CharTy >, ValTy >
			>
class table_base_impl
{
	typedef typename boost::add_const < 
		typename boost::call_traits < ValTy >::param_type >::type value_param_type;
	typedef std::basic_string < CharTy > string_type;
	typedef typename boost::add_const < 
		typename boost::call_traits < string_type >::param_type >::type name_param_type;

public:
	typedef InternalMapTy					data_type;
	typedef typename data_type::value_type	record_type;
	typedef string_type						name_type;
	typedef ValTy							value_type;
	typedef typename data_type::size_type	size_type;

	typedef typename 
		data_type::const_iterator			const_iterator;
	typedef typename data_type::iterator	iterator;

	typedef table_base_impl < CharTy, 
					ValTy, InternalMapTy >	super_type;

	virtual ~table_base_impl () = 0;

	iterator begin ()
	{
		return this->tlb_.begin ();
	};

	iterator end ()
	{
		return this->tlb_.end ();
	};

	const_iterator begin () const
	{
		return this->tlb_.begin ();
	};

	const_iterator end () const
	{
		return this->tlb_.end ();
	};

	bool const empty () const
	{
		return this->tlb_.empty();
	};

	bool const contained_here (name_param_type find_name_) const
	{
		return (this->tlb_.find(find_name_) != this->tlb_.end());
	};

	size_type const size () const
	{
		return this->tlb_.size ();
	};

	size_type const max_size () const
	{
		return this->tlb_.max_size ();
	};

	void clear ()
	{
		this->tlb_.clear();
	};

	void add (record_type const& new_rec_) 
	{
		if (this->tlb_.insert (new_rec_).second == false) 
			throw table_add_record_error (new_rec_.first);
	};

	void add (name_param_type name_, value_param_type val_ = value_type ())
	{
		this->add (std::make_pair(name_, val_));
	};

	value_type& operator [] (name_param_type name_)
	{
		return this->tlb_.at(name_);
	};

	value_type const& operator [] (name_param_type name_) const
	{
		return this->tlb_.at(name_);
	};

	value_type& at (name_param_type name_)
	{
		return this->tlb_.at(name_);
	};

	value_type const& at (name_param_type name_) const
	{
		return this->tlb_.at(name_);
	};

private:
	data_type tlb_;

	virtual void init_table () = 0;
};

template < typename CharTy, typename ValTy, typename InternalMapTy >
table_base_impl < CharTy, ValTy, InternalMapTy >::~table_base_impl ()
{
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_TABLE_BASE_IMPL_HPP

/**
* 18.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
