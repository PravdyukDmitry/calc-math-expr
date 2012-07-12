/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file cme_exception.hpp                                               *
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

#ifndef CME_CME_EXCEPTION_HPP
 #define CME_CME_EXCEPTION_HPP

#include <stdexcept>
#include <typeinfo>

#include <boost/lexical_cast.hpp>

namespace cme { namespace cme_detail {

/// Базовый...
template < typename ExceptBaseTy >
struct cme_exception
	: public ExceptBaseTy
{
	typedef ExceptBaseTy base_exception_class;

	cme_exception (std::string const& msg_)
		: base_exception_class (msg_) 
	{
	};
};

/// Генерируется при неудачной попытке вставки нового элемента в таблицу
class table_add_record_error 
	: public cme_exception < std::runtime_error >
{
	class add_record_error_message_generator
	{
		template < typename Ty >
		struct key_name_to_adding_error_message;

		template <>
		struct key_name_to_adding_error_message < std::string >
		{
			static std::string const& convert (std::string const& key_name_)
			{
				return key_name_;
			};
		};

		template <>
		struct key_name_to_adding_error_message < std::wstring >
		{
			static std::string const convert (std::wstring const& key_name_)
			{
				return std::string (key_name_.begin (), key_name_.end ());
			};
		};

	public:
		template < typename StrTy >
		static std::string const generate (StrTy const& key_name_)
		{
			return "ERROR at adding of new element [ "
					+ key_name_to_adding_error_message < StrTy >::convert (key_name_)
					+ " ] to the table: element with such key (name) exists already!";
		};
	};

public:
	typedef cme_exception < std::runtime_error > base_exception_class;
	
	template < typename CharTy >
	table_add_record_error (std::basic_string < CharTy > const& key_name_)
		: base_exception_class (add_record_error_message_generator::generate (key_name_)) 
	{
	};
};

/// Генерируется при неудачной попытке вставки нового элемента в таблицу (в make_cme_grammar)
struct make_cme_grammar_init_error
	: public cme_exception < std::runtime_error >
{
	typedef cme_exception < std::runtime_error > base_exception_class;

	make_cme_grammar_init_error (std::string const& msg_)
		: base_exception_class (msg_) 
	{
	};
};

/**
* Генерируется при неудавшейся попытке конвертации строки (интервала) в число 
* с плавающей запятой в функции string_to_real_cast
**/
template < typename RealTy >
class bad_string_to_real_cast
	: public cme_exception < std::runtime_error >
{
	template < typename RealTy >
	class bad_string_to_real_cast_message_maker
	{
	public:

		bad_string_to_real_cast_message_maker ()
			: error_message_ ("ERROR: It was not succeeded to bring expression over of ") 
		{
		};

		template < typename IterTy >
		std::string const& operator () (IterTy const begin_expr_, IterTy const end_expr_)
		{
			this->error_message_.insert (this->error_message_.end (), begin_expr_, end_expr_);
			do_end ();
			return this->error_message_;
		};

	private:
		std::string error_message_;

		void do_end ()
		{
			this->error_message_.append (" to the type of ");
			this->error_message_.append (typeid (RealTy).name ());
		};
	};

public:
	typedef cme_exception < std::runtime_error > base_exception_class;

	template < typename IterTy >
	bad_string_to_real_cast (IterTy const begin_expr_, IterTy const end_expr_)
		: base_exception_class
			(bad_string_to_real_cast_message_maker < RealTy > () (begin_expr_, end_expr_))
	{
	};

	template < typename CharTy >
	bad_string_to_real_cast (std::basic_string < CharTy > const& expr_)
		: base_exception_class
			(bad_string_to_real_cast_message_maker < RealTy > () (expr_.begin (), expr_.end ()))
	{
	};
};

/**
* Генерируется в функции get_priority_t, если для id переданного ей токена неудаётся 
* вывести значение приоритета - priority_t 
* ... т.к. не всем id соответствуют приорететы !!!
**/
class get_priority_t_error
	: public cme_exception < std::invalid_argument >
{
public:
	typedef cme_exception < std::invalid_argument > base_exception_class;

	template < typename TokenTy >
	get_priority_t_error (TokenTy const& tok_)
		: base_exception_class (make_message (tok_))
	{
	};

private:

	template < typename TokenTy >
	static std::string const make_message (TokenTy const& tok_)
	{
		std::string msg_ ("The function of get_priority_t did not succeed to "
			"define priority of token [ token_name: ");
		msg_.append (tok_->get_name ().begin (), tok_->get_name ().end ());
		msg_.append (" ; token_id: ");
		msg_.append (boost::lexical_cast < std::string > (tok_->get_id ()));
		msg_.append (" ]");

		return msg_;
	};
};

/**
* Генерируется в конструкторе cme_creator_internal_stack_value_wrapper,
* если переданый ему токен == NULL
**/
struct null_token_error
	: public cme_exception < std::invalid_argument >
{
	typedef cme_exception < std::invalid_argument > base_exception_class;

	null_token_error ()
		: base_exception_class ("A vital error is in the designer of class of "
			 "cme_creator_internal_stack_value_wrapper. It is not token.")
	{
	};
};

struct message_creator
{
	template < typename CharTy >
	static std::string const create (char const* const msg_, CharTy const last_char_)
	{
		std::string except_msg_ (msg_);
		except_msg_.push_back (last_char_);

		return except_msg_;
	};
};

/// Генерируется в cme_creator_impl при неудачном разборе исходного выражения...
struct cme_create_error
	: public cme_exception < std::runtime_error >
{
	typedef cme_exception < std::runtime_error > base_exception_class;
	
	template < typename CharTy >
	cme_create_error (CharTy const last_char_)
		: base_exception_class (
		message_creator::create ("ERROR: cme pars stop at: ", last_char_))
	{
	};
};

/**
* Генерируется в cme_calculator_impl, если при подсчёте выражения встретился
* токен помеченный как переменная (id_variable), а таблица переменных пуста,
* т.е. cme_calculator_impl::p_vtlb_ == NULL
**/
struct null_variables_table_error
	: public cme_exception < std::runtime_error >
{
	typedef cme_exception < std::runtime_error > base_exception_class;

	null_variables_table_error ()
		: base_exception_class ("ERROR: The table of variables absents!") 
	{
	};
};

/**
* Генерируется в cme_calculator_impl, если при подсчёте выражения встретился
* токен помеченный как переменная (id_constant), а таблица констант пуста,
* т.е. cme_calculator_impl::p_ctlb_ == NULL
**/
struct null_constant_table_error
	: public cme_exception < std::runtime_error >
{
	typedef cme_exception < std::runtime_error > base_exception_class;

	null_constant_table_error ()
		: base_exception_class ("ERROR: The table of constant absents!") 
	{
	};
};

/**
* Генерируется в cme_calculator_impl, если при подсчёте выражения встретился
* токен помеченный как переменная (id_function_name), а таблица унарных 
* математических функцый пуста, т.е. cme_calculator_impl::p_umftlb_ == NULL
**/
struct null_unary_math_function_table_error
	: public cme_exception < std::runtime_error >
{
	typedef cme_exception < std::runtime_error > base_exception_class;

	null_unary_math_function_table_error ()
		: base_exception_class ("ERROR: The table of unary math function absents!") 
	{
	};
};

/**
* Генерируется, если в процессе вычисления значения по ОПЗ 
* встречен токен неизвестного, или не поддерживаемого типа ... 
**/
class unknown_token_type
	: public cme_exception < std::runtime_error >
{
public:
	typedef cme_exception < std::runtime_error > base_exception_class;

	template < typename TokDataTy >
	unknown_token_type (TokDataTy const& tok_)
		: base_exception_class (create_message (tok_)) 
	{
	};

private:
	template < typename TokDataTy >
	static std::string const create_message (TokDataTy const& tok_)
	{
		std::string msg_ ("In the process of calculation of RPN the lexeme ( ");
		msg_.append (tok_.get_name ().begin (), tok_.get_name ().end ());
		msg_.append (" ) of unknown type ( ");
		msg_.append (boost::lexical_cast < std::string > (tok_.get_id ()));
		msg_.append (" ) is met. :(");

		return msg_;
	};
};

/**
* Генерируется, если не выполняется последние условие вычисления значения по ОПЗ
* - cme_calculator_impl::stack_.size () != 1
**/
struct bad_end_calculate_condition_error
	: public cme_exception < std::runtime_error >
{
	typedef cme_exception < std::runtime_error > base_exception_class;

	bad_end_calculate_condition_error ()
		: base_exception_class ("ERROR: Upon completion of count of RPN "
		"a stack is contained by a zero / more than one values "
		"(cme_calculator_impl::stack_.size () != 1)!") 
	{
	};
};

/// Генерируется при неудачном разборе списка переменных...
struct parse_variable_list_error
	: public cme_exception < std::runtime_error >
{
	typedef cme_exception < std::runtime_error > base_exception_class;

	template < typename CharTy >
	parse_variable_list_error (CharTy const last_char_)
		: base_exception_class (message_creator::create 
		("ERROR at the analysis of list of variables. Last read character: ", last_char_)) 
	{
	};
};

/// Генерируется в функции save при неудачной попытке открыть файл
struct cme_expression_save_error
	: public cme_exception < std::runtime_error >
{
	typedef cme_exception < std::runtime_error > base_exception_class;

	template < typename CharTy >
	cme_expression_save_error (CharTy const* const save_file_name_)
		: base_exception_class (std::string ("ERROR at opening a file: ") + save_file_name_) 
	{
	};
};

/// Генерируется в функции load при неудачной попытке открыть файл
struct cme_expression_load_error
	: public cme_exception < std::runtime_error >
{
	typedef cme_exception < std::runtime_error > base_exception_class;

	template < typename CharTy >
	cme_expression_load_error (CharTy const* const save_file_name_)
		: base_exception_class (std::string ("ERROR at opening a file: ") + save_file_name_) 
	{
	};
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_CME_EXCEPTION_HPP

/**
* 18.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
