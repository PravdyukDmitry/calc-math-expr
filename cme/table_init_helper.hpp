/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   Библиотека cme                                                        *
 *   @file table_init_helper.hpp                                           *
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

#ifndef CME_TABLE_INIT_HELPER_HPP
 #define CME_TABLE_INIT_HELPER_HPP

#include <cme/unary_math_function.hpp>

namespace cme { namespace cme_detail {

template < typename ChTy >
struct init_helper;

template <>
struct init_helper < char >
{
	template < template < typename > class CTlbTy >
	static void aply (CTlbTy < char >* const mytlb_)
	{
		mytlb_->add ("PI",				"3.14159265358979323846");
		mytlb_->add ("ROOT_PI",			"1.772453850905516027");
		mytlb_->add ("ROOT_HALF_PI",	"1.253314137315500251");
		mytlb_->add ("ROOT_TWO_PI",		"2.506628274631000502");
		mytlb_->add ("ROOT_LN_FOUR",	"1.17741002251547469");
		mytlb_->add ("E",				"2.71828182845904523536");
		mytlb_->add ("HALF",			"0.5");
		mytlb_->add ("EULER",			"0.577215664901532860606");			
		mytlb_->add ("ROOT_TWO",		"1.4142135623730950488");
		mytlb_->add ("LN_TWO",			"0.6931471805599453094");
		mytlb_->add ("LN_LN_TWO",		"-0.3665129205816643");
		mytlb_->add ("THIRD",			"0.333333333333333333");			
		mytlb_->add ("TWOTHIRD",		"0.666666666666666666");
		mytlb_->add ("PI_MINUS_THREE",	"0.14159265358979323846");
		mytlb_->add ("FOUR_MINUS_PI",	"0.85840734641020676153735");
		mytlb_->add ("GOLDEN_RATIO",	"1.61803398874989484820458683436563811");
		mytlb_->add ("C",				"2.99792458e8"); /// Light Speed
		mytlb_->add ("PLANK_CONST",		"6.6260693e-34");
		/// и тд. ...
	};

	template < template < typename, typename > class UmfTlbTy, typename RealTy >
	static void aply (UmfTlbTy < char, RealTy >* const mytlb_)
	{
		mytlb_->add ("tg", &tg);
		mytlb_->add ("ctg", &ctg);
		mytlb_->add ("sin", &sin);
		mytlb_->add ("cos", &cos);

		mytlb_->add ("atg", &atg);
		mytlb_->add ("actg", &actg);
		mytlb_->add ("asinh", &asinh);
		mytlb_->add ("acosh", &acosh);
		mytlb_->add ("cosh", &cosh);
		mytlb_->add ("sinh", &sinh);

		mytlb_->add ("tgh", &tgh);
		mytlb_->add ("ctgh", &ctgh);
		mytlb_->add ("atgh", &atgh);
		mytlb_->add ("actgh", &actgh);

		mytlb_->add ("sec", &sec);
		mytlb_->add ("cosec", &cosec);
		mytlb_->add ("sech", &sech);
		mytlb_->add ("cosech", &cosech);
		mytlb_->add ("asech", &asech);
		mytlb_->add ("acosech", &acosech);

		mytlb_->add ("exp", &exp);
		mytlb_->add ("ln", &ln);
		mytlb_->add ("lg", &lg);
		mytlb_->add ("modul", &modul);
		mytlb_->add ("fact", &fact);
		mytlb_->add ("dfact", &dfact);
		mytlb_->add ("sqrt", &sqrt);
		/// и тд. ...
	};
};

template <>
struct init_helper < wchar_t >
{
	template < template < typename > class CTlbTy >
	static void aply (CTlbTy < wchar_t >* const mytlb_)	
	{
		mytlb_->add (L"PI",				L"3.14159265358979323846");
		mytlb_->add (L"ROOT_PI",		L"1.772453850905516027");
		mytlb_->add (L"ROOT_HALF_PI",	L"1.253314137315500251");
		mytlb_->add (L"ROOT_TWO_PI",	L"2.506628274631000502");
		mytlb_->add (L"ROOT_LN_FOUR",	L"1.17741002251547469");
		mytlb_->add (L"E",				L"2.71828182845904523536");
		mytlb_->add (L"HALF",			L"0.5");
		mytlb_->add (L"EULER",			L"0.577215664901532860606 ");			
		mytlb_->add (L"ROOT_TWO",		L"1.4142135623730950488");
		mytlb_->add (L"LN_TWO",			L"0.6931471805599453094");
		mytlb_->add (L"LN_LN_TWO",		L"-0.3665129205816643");
		mytlb_->add (L"THIRD",			L"0.333333333333333333");			
		mytlb_->add (L"TWOTHIRD",		L"0.666666666666666666");
		mytlb_->add (L"PI_MINUS_THREE",	L"0.14159265358979323846");
		mytlb_->add (L"FOUR_MINUS_PI",	L"0.85840734641020676153735");
		/// и тд. ...
	};

	template < template < typename, typename > class UmfTlbTy, typename RealTy >
	static void aply (UmfTlbTy < wchar_t, RealTy >* const mytlb_)
	{
		mytlb_->add (L"tg", &tg);
		mytlb_->add (L"ctg", &ctg);
		mytlb_->add (L"sin", &sin);
		mytlb_->add (L"cos", &cos);

		mytlb_->add (L"atg", &atg);
		mytlb_->add (L"actg", &actg);
		mytlb_->add (L"asinh", &asinh);
		mytlb_->add (L"acosh", &acosh);
		mytlb_->add (L"cosh", &cosh);
		mytlb_->add (L"sinh", &sinh);

		mytlb_->add (L"tgh", &tgh);
		mytlb_->add (L"ctgh", &ctgh);
		mytlb_->add (L"atgh", &atgh);
		mytlb_->add (L"actgh", &actgh);

		mytlb_->add (L"sec", &sec);
		mytlb_->add (L"cosec", &cosec);
		mytlb_->add (L"sech", &sech);
		mytlb_->add (L"cosech", &cosech);
		mytlb_->add (L"asech", &asech);
		mytlb_->add (L"acosech", &acosech);

		mytlb_->add (L"exp", &exp);
		mytlb_->add (L"ln", &ln);
		mytlb_->add (L"lg", &lg);
		mytlb_->add (L"modul", &modul);
		mytlb_->add (L"fact", &fact);
		mytlb_->add (L"dfact", &dfact);
		mytlb_->add (L"sqrt", &sqrt);
		/// и тд. ...
	};
};

}; /// end namespace cme::cme_detail
}; /// end namespace cme

#endif /// CME_TABLE_INIT_HELPER_HPP

/**
* 14.03.2009 - Потураев А.С. - подготовка релиза 1.3 :)
**/
