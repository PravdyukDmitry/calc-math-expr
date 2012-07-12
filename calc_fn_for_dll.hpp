/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file calc_fn_for_dll.hpp                                             *
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

#ifndef FN_FOR_DLL_HPP
 #define FN_FOR_DLL_HPP

/**
* @file calc_fn_for_dll.hpp - Определяет функции для DLL
**/

/**
* Приведенный ниже блок ifdef - это стандартный метод создания макросов, упрощающий процедуру 
* экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа 
* MAKE_DLL_EXPORTS. Данный символ не должен быть определен ни в одном проекте,
* использующем данную DLL (если только вы не желаете пересобрать DLL). 
* Благодаря этому любой другой проект, чьи исходные файлы включают данный файл, видит 
* функции MAKE_DLL_API как импортированные из DLL, тогда как данная DLL видит символы,
* определяемые данным макросом, как экспортированные.
**/

#ifdef MAKE_DLL_EXPORTS
	#define MAKE_DLL_API extern "C" __declspec(dllexport)
#else
	#define MAKE_DLL_API extern "C" __declspec(dllimport)
#endif

#ifdef MAKE_CME_DLL_CALCULATE_FUNCTION_FOR_THE_EXTERNAL_USE

#include <cme/cme.hpp>

#include <boost/type_traits/add_const.hpp>

namespace cme { namespace cme_detail {

template < typename RealTy, typename CharTy >
RealTy const calculate_math_expression_for_dll_helper (
	CharTy const* const expr_, CharTy const* const var_list_,
	bool const use_constant_, bool const use_unary_math_func_)
{
	typedef CharTy char_type;
	typedef RealTy real_type;

	typedef cme_generator < real_type, char_type > generator_type;

	typedef typename generator_type::variable_table_type variable_table_type;
	typedef typename generator_type::constant_table_type constant_table_type;
	typedef typename generator_type::unary_math_function_table_type unary_math_function_table_type;

	if (var_list_)
	{
		variable_table_type var_tlb_;
		init_variable_table (var_tlb_, var_list_);

		if (use_constant_)
		{
			constant_table_type const_tlb_;

			if (use_unary_math_func_)
			{
				unary_math_function_table_type umf_tlb_;

				return calculate_math_expression (expr_, &var_tlb_, &const_tlb_, &umf_tlb_);
			}

#define cmefn calculate_math_expression < real_type, char_type, variable_table_t, constant_table_t, unary_math_function_table_t >
			
			return cmefn (expr_, &var_tlb_, &const_tlb_, NULL);
		}

		return cmefn (expr_, &var_tlb_, NULL, NULL);
	}

	if (use_constant_)
	{
		constant_table_type const_tlb_;

		if (use_unary_math_func_)
		{
			unary_math_function_table_type umf_tlb_;

			return cmefn (expr_, NULL, &const_tlb_, &umf_tlb_);
		}

		return cmefn (expr_, NULL, &const_tlb_, NULL);
	}

	if (use_unary_math_func_)
	{
		unary_math_function_table_type umf_tlb_;

		return cmefn (expr_, NULL, NULL, &umf_tlb_);
	}

	return cmefn (expr_, NULL, NULL, NULL);

#undef cmefn

};

}; /// end namespace cme::cme_detail

/// calculate function

/// char
MAKE_DLL_API
float const calculate_math_expression_cf (
	char const* const expr_, char const* const var_list_,
	bool const use_constant_, bool const use_unary_math_func_)
{
	return cme_detail::calculate_math_expression_for_dll_helper < float > 
		(expr_, var_list_, use_constant_, use_unary_math_func_);
};

MAKE_DLL_API
double const calculate_math_expression_cd (
	char const* const expr_, char const* const var_list_,
	bool const use_constant_, bool const use_unary_math_func_)
{
	return cme_detail::calculate_math_expression_for_dll_helper < double > 
		(expr_, var_list_, use_constant_, use_unary_math_func_);
};

MAKE_DLL_API
long double const calculate_math_expression_cld (
	char const* const expr_, char const* const var_list_,
	bool const use_constant_, bool const use_unary_math_func_)
{
	return cme_detail::calculate_math_expression_for_dll_helper < long double > 
		(expr_, var_list_, use_constant_, use_unary_math_func_);
};

/// wchar_t
MAKE_DLL_API
float const calculate_math_expression_wcf (
	wchar_t const* const expr_, wchar_t const* const var_list_,
	bool const use_constant_, bool const use_unary_math_func_)
{
	return cme_detail::calculate_math_expression_for_dll_helper < float > 
		(expr_, var_list_, use_constant_, use_unary_math_func_);
};

MAKE_DLL_API
double const calculate_math_expression_wcd (
	wchar_t const* const expr_, wchar_t const* const var_list_,
	bool const use_constant_, bool const use_unary_math_func_)
{
	return cme_detail::calculate_math_expression_for_dll_helper < double > 
		(expr_, var_list_, use_constant_, use_unary_math_func_);
};

MAKE_DLL_API
long double const calculate_math_expression_wcld (
	wchar_t const* const expr_, wchar_t const* const var_list_,
	bool const use_constant_, bool const use_unary_math_func_)
{
	return cme_detail::calculate_math_expression_for_dll_helper < long double > 
		(expr_, var_list_, use_constant_, use_unary_math_func_);
};

template < typename RealTy, typename CharTy >
struct pointer_to_calc_fn
{
	typedef 
		typename boost::add_const < RealTy >::type
		(__cdecl * type) (
		CharTy const* const
		, CharTy const* const
		, bool const
		, bool const
		);
};

}; /// end namespace cme

#endif /// MAKE_CME_DLL_CALCULATE_FUNCTION_FOR_THE_EXTERNAL_USE

#ifdef MAKE_CME_DLL_STRING_TO_REAL_CAST
/// string to real cast function

#include <cme/cme.hpp>

#include <boost/type_traits/add_const.hpp>

namespace cme {

/// char

MAKE_DLL_API
float const str_to_f (const char* const num_str_)
{
	return string_to_real_cast < float > (num_str_);
};

MAKE_DLL_API
double const str_to_d (const char* const num_str_)
{
	return string_to_real_cast < double > (num_str_);
};

MAKE_DLL_API
long double const str_to_ld (const char* const num_str_)
{
	return string_to_real_cast < long double > (num_str_);
};

/// wchar_t

MAKE_DLL_API
float const wstr_to_f (const wchar_t* const num_str_)
{
	return string_to_real_cast < float > (num_str_);
};

MAKE_DLL_API
double const wstr_to_d (const wchar_t* const num_str_)
{
	return string_to_real_cast < double > (num_str_);
};

MAKE_DLL_API
long double const wstr_to_ld (const wchar_t* const num_str_)
{
	return string_to_real_cast < long double > (num_str_);
};

template < typename RealTy, typename CharTy >
struct pointer_to_cast_fn
{
	typedef 
		typename boost::add_const < RealTy >::type
		(__cdecl * type) (CharTy const* const);
};

}; /// end namespace cme

#endif /// MAKE_CME_DLL_STRING_TO_REAL_CAST

#ifdef MAKE_TST_PLUGIN_DLL

#include "runge_do_execute.hpp"

/// Функция в dll запускающая "метод"

typedef char const* const string_type;

typedef bool const (__cdecl *execute_function_ptr_type) (string_type, char*&, char*&);

MAKE_DLL_API
bool const ///is ok
	execute (string_type condition_file_patch_
		, char*& answer_file_patch_, char*& graph_file_patch_)
{
	//"tst_answer_file.ans", "tst_gfl.gfl"
	answer_file_patch_ = "runge.ans";
	graph_file_patch_ = "runge.gfl";

	return do_execute (condition_file_patch_, answer_file_patch_, graph_file_patch_);
};

/// Информация о загруженой dll

typedef string_type (__cdecl *text_plugin_info_function_ptr_type) (void);

/// Путь к файлу с решением...
MAKE_DLL_API
string_type answer_file_patch () 
{
	return "runge.ans";
};

/// Путь к файлу с графиком...
MAKE_DLL_API
string_type graph_file_patch () 
{
	return "runge.gfl";
};

/// Авторские права и лицензия ...
MAKE_DLL_API
string_type copyright () 
{
	return "Copyright (с) 2009 Потураев А.С.";
};

/// Краткая справка по методу ...
MAKE_DLL_API
string_type short_metod_help ()
{
	return "Здесь будет краткая информация о методе ...";
};

/// краткая справка по использованию данной dll ...
MAKE_DLL_API
string_type short_use_help ()
{
	return "Здесь будет краткая информация о использовании кода ...";
};

/// Имеет ли метод графическое отображение ...

typedef bool const (__cdecl *is_plot_info_function_ptr_type) (void);

MAKE_DLL_API
bool const is_plot ()
{
	return true;
};

/// Версия реализации ...

typedef float const (__cdecl *version_info_function_ptr_type) (void);

MAKE_DLL_API
float const version ()
{
	return 1.0;
};

#endif /// MAKE_TST_PLUGIN_DLL

#endif /// FN_FOR_DLL_HPP

/**
*	18.06.2009 - Потураев А.С. - подготовка релиза... :)
**/

