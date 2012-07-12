/**
 ***************************************************************************
 *   Copyright (C) 2009 by Poturaev A.S. (Pchola) pchollin@mail.ru         *
 *                                                                         *
 *   ���������� cme                                                        *
 *   @file cme_test_program.hpp                                            *
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

/**
* @file cme_test_program.hpp - ���������� ������� cme_test_program () 
* ��� �������� ��������� (����������� win32 / linux) ���������� ��� ���������� cme ...
*
* ������:
*
* #include <cme_test_program.hpp>
*
* int main (int argc, _TCHAR* argv[]) 
* {
*	return cme_tst::cme_test_program < double, char > ();
* };
**/

#ifndef CME_TST_CME_TEST_PROGRAM_HPP
 #define CME_TST_CME_TEST_PROGRAM_HPP

/// CME - heder's
#include <cme/cme.hpp>

#ifdef _WIN32
	#include <cyr_out.hpp>
#endif /// _WIN32

/// Boost - heder's
#include <boost/cstdint.hpp>
#include <boost/algorithm/string/trim.hpp>

/// STL - heder's
#include <iostream>
#include <limits>
#include <cstdlib>

namespace cme_tst { namespace cme_tst_detail {

template < typename RealTy, typename CharTy >
class cme_test_program_helper
{
	typedef cme::cme_generator < RealTy, CharTy > cme_generator_type;
public:
	typedef CharTy char_type;
	typedef RealTy real_type;

	typedef typename cme_generator_type::variable_table_type variable_table_type;
	typedef typename cme_generator_type::constant_table_type constant_table_type;
	typedef typename 
		cme_generator_type::unary_math_function_table_type unary_math_function_table_type;
	typedef typename cme::cme_detail::cme_expression_generator < char_type >::type cme_expression_type;

#ifdef _WIN32
	typedef pchola::windows_console_printer < char > error_print;
	typedef pchola::cyr_out_t < char > cyr_out_type;
#else
	struct error_print
	{
		static void print (char const* const err_msg_, std::ostream& os_)
		{
			os_ << err_msg_;
		};
	};
#endif /// _WIN32

#ifdef _WIN32
	#define cyr_os this->cyr_out
#else
	#define cyr_os std::cout
#endif /// _WIN32

	cme_test_program_helper ()
		: inp_str_ (), p_vtlb_ (NULL) 
	{
	};

	~cme_test_program_helper ()
	{
		this->use_variable_release ();
	};

	bool const execute ()
	{
		try {

			cyr_os << "������ !:)\n"
					"��� ��������� ��������� �������� �������������� ���������.\n";

BEGIN_MARK:

			cyr_os << "� ��� ���� ��������� �����������:\n"
					"\t1 - ���������� ��������� ���������;\n"
					"\t2 - ���������� ��������� �� ��� �������� �������� ������ ����������� �� �����;\n"
					"\t3 - ���������� �������� �������� ������ ��������� ���������;\n"
					"\t4 - ����� �������;\n"
					"\t5 - �����.\n"
					"����������, �������� ���� �����: ";

			this->read_str ();

			try {

				switch (boost::lexical_cast < boost::uint16_t > (this->inp_str_))
				{
				case 1:
					{
						this->exec ();
						goto BEGIN_MARK;
					}
				case 2:
					{
INPUT_LOAD_FILE_NAME:
						cyr_os << "������� ���������� ���� � ����� (���������� ����������� cme) "
								"����������� ��� ��������������� ���������: ";

						this->read_str ();

						cme_expression_type rpn_me_;

						try {
							cme::load < char_type > (this->inp_str_.c_str (), rpn_me_);
						} 
						catch (cme::cme_detail::cme_expression_load_error const& er_)
						{
							this->inp_patch_err_report (er_);

RENEWAL_AFTER_MISENTRY_THE_FILE_NAME_FOR_LOAD:

							this->ret_to_inp_patch ();
							
							try {
								if (boost::lexical_cast < bool > (this->inp_str_))
									goto INPUT_LOAD_FILE_NAME;
								else
									goto BEGIN_MARK;
							}
							catch (boost::bad_lexical_cast const&)
							{
								this->inp_err_report ();
								goto RENEWAL_AFTER_MISENTRY_THE_FILE_NAME_FOR_LOAD;
							}
						}
						
						this->p_vtlb_ = new variable_table_type;
						{
							typedef typename cme_expression_type::const_iterator iter_t;
							
							iter_t it_ (rpn_me_.begin ());
							iter_t const end_ (rpn_me_.end ());

							while (it_ != end_)
							{
								if ((*it_)->get_id () == cme::cme_detail::id_variable)
								{
INIT_VAR:
									cyr_os << "���������� ��������������� ���������� ";
									std::cout << (*it_)->get_name () << " = ";

									this->read_str ();

									try {
										this->p_vtlb_->add ((*it_)->get_name (), boost::lexical_cast < real_type > (this->inp_str_));
									} 
									catch (boost::bad_lexical_cast const&)
									{
										cyr_os << "\n\n������: �������� ���� ����� ������� ������������� �������� ����������! ��������� ����! \n\n";
										goto INIT_VAR;
									}
								}
								++it_;
							}

							if (this->p_vtlb_->empty ())
								this->use_variable_release ();
						}
						cyr_os << "\n�����: ";
						std::cout << cme::me_calc (rpn_me_, this->p_vtlb_, 
							&constant_table_type (), &unary_math_function_table_type ()) << "\n\n";

						this->use_variable_release ();
						goto BEGIN_MARK;
					}
				case 3:
					{
						this->exec (false);
						goto BEGIN_MARK;
					}
				case 4:
					{
						cyr_os << "\n������� ������� �� ������������ cme\n"
								"- �++ ���������� ��� ������� �������������� ���������.\n\n"
								"�������:\n";

						std::cout << "sin (x), cos (x), sinh (x), cosh (x), tg (x), ctg, tgh (x), ctgh (x), atg (x),\n"
							<< "actg (x), asinh (x), acosh (x), atgh (x), actgh (x), sec (x), cosec (x),\n"
							<< "sech (x), cosech (x), asech (x), acosech (x), ln (x), lg (x), exp (x),\n"
							<< "modul (x), fact (x), dfact (x), sqrt (x)\n"
							<< "pow (x, y), root (x, y)\n\n";

						cyr_os << "���������:\n";

						std::cout << "PI, ROOT_PI, ROOT_HALF_PI, ROOT_TWO_PI, ROOT_LN_FOUR, E, HALF, EULER,\n"
							 << "ROOT_TWO, LN_TWO, LN_LN_TWO, THIRD, TWOTHIRD, PI_MINUS_THREE, FOUR_MINUS_PI,"
							 << "GOLDEN_RATIO, C, PLANK_CONST\n\n";

						cyr_os << "��������:\t+ - * / ^\n\n";

						cyr_os << "������ ����� ����������:\n"
							"���_����������_1 = ��������_1, ���_����������_2, ���_����������_3=��������_3 � �.�.\n"
							"!!! ���_����������_2 ������������� �������� 0.0 !!!\n\n\n";

						goto BEGIN_MARK;
					}
				case 5: 
					goto QUIT_MARK;
				default:
					{
						error_print::print 
							("������: ����������� ��������� !!!\n", std::cerr);
						goto BEGIN_MARK;
					}
				}
			}
			catch (boost::bad_lexical_cast const&)
			{
				this->inp_err_report ();
				goto BEGIN_MARK;
			}
		}
		catch (std::exception const& er_) 
		{
			this->use_variable_release ();

			error_print::print (er_.what (), std::cerr);
			std::cerr << '\n';

			getchar ();
			return EXIT_FAILURE; 
		}
		catch (...) 
		{
			this->use_variable_release ();

			error_print::print 
				("\n������: ��������� ����������� ������. ��������� ����� �������� �������! :(\n", std::cerr);

			::getchar ();
			return EXIT_FAILURE; 
		}

QUIT_MARK:

		cyr_os << "\n���� !!! ;)\n\nCopyright (C) 2009 by �������� �.�. (Pchola) pchollin@mail.ru";

		std::cout << std::endl;
		::getchar ();
		return EXIT_SUCCESS;
	};

private:
	std::string inp_str_; 
	variable_table_type* p_vtlb_;

#ifdef _WIN32
	cyr_out_type cyr_out;
#endif /// _WIN32

	void use_variable ()
	{
		cyr_os << "������������ ����������?\n"
				"\t 0 - ���;\n"
				"\t 1 - ��.\n"
				"����������, �������� ���� �����: ";

		this->read_str ();

		if (boost::lexical_cast < bool > (this->inp_str_))
		{
			cyr_os << "������� ������ ����������: ";
	
			this->get_str ();

			this->p_vtlb_ = new variable_table_type;
			cme::init_variable_table (*(this->p_vtlb_), this->inp_str_);
		}
	};

	void use_variable_release ()
	{
		if (this->p_vtlb_)
		{
			delete this->p_vtlb_;
			this->p_vtlb_ = NULL;
		}
	};

	void exec (bool const state_ = true)
	{
		this->use_variable ();
		state_ ? this->execute_calc () : this->make_rpn ();
		this->use_variable_release ();
	};

	void inp_err_report () const
	{
		error_print::print ("\n�� ����� ����� �� ����! ��������� ����!\n\n", std::cerr);
	};

	void inp_me ()
	{
		cyr_os << "������� �������������� ���������: ";
		
		this->get_str ();
	};

	void read_str ()
	{
		std::cin >> this->inp_str_;
		std::cin.get ();
	};

	void get_str ()
	{
		std::getline (std::cin, this->inp_str_);
		boost::algorithm::trim_right (this->inp_str_);
	};

	void execute_calc ()
	{
		this->inp_me ();		

		cyr_os << "\n�����:   ";

		std::streamsize const old_precision_ = std::cout.precision 
			(static_cast < std::streamsize > (std::numeric_limits < real_type >::digits10));

		std::cout << cme::calculate_math_expression (this->inp_str_, p_vtlb_, &constant_table_type (),
			&unary_math_function_table_type ()) << "\n\n";

		std::cout.precision (old_precision_);
	};

	template < typename ErrTy >
	void inp_patch_err_report (ErrTy const& er_) 
	{
		error_print::print (er_.what (), std::cerr);
		std::cerr << "\n\n";
	};

	void ret_to_inp_patch ()
	{
		cyr_os << "\t0 - ��������� � �������� ����;\n"
				"\t1 - ��������� ���� ���� � �����.\n"
				"����������, �������� ���� �����: "; 
		
		this->read_str ();
	};

	void make_rpn ()
	{
		this->inp_me ();

		cyr_os << "\n��� ��� ��������� ���� ���������: ";

		cme_expression_type const rpn_me_ (cme::create_rpn (this->inp_str_.c_str (), this->p_vtlb_, 
			&constant_table_type (), &unary_math_function_table_type ()));

		std::cout << rpn_me_ << "\n\n";

SAVE_RPN_ME_TO_FILE_CHOICE:
		cyr_os << "��������� � ����?\n"
			"\t 0 - ���;\n"
			"\t 1 - ��.\n"
			"����������, �������� ���� �����: ";

		this->read_str ();

		try {
			if (boost::lexical_cast < bool > (this->inp_str_))
			{
INPUT_SAVE_RPN_ME_TO_FILE:		
				cyr_os << "���������� ������� ���� � ����� ��� ���������� ���:\n";
				
				this->read_str ();

				try {
					cme::save (this->inp_str_.c_str (), rpn_me_);
				}
				catch (cme::cme_detail::cme_expression_save_error const& er_)
				{
					this->inp_patch_err_report (er_);
					this->ret_to_inp_patch ();
					
					try {
						if (boost::lexical_cast < bool > (this->inp_str_))
							goto INPUT_SAVE_RPN_ME_TO_FILE;
						else
							return;
					}
					catch (boost::bad_lexical_cast const&)
					{
						this->inp_err_report ();
						goto INPUT_SAVE_RPN_ME_TO_FILE;
					}
				}
			}
		}
		catch (boost::bad_lexical_cast const&)
		{
			this->inp_err_report ();
			goto SAVE_RPN_ME_TO_FILE_CHOICE;
		}
	};

#undef cyr_os

};

}; /// end namespace cme_tst::cme_tst_detail 

template < typename RealTy, typename CharTy >
bool const cme_test_program ()
{
	return cme_tst_detail::cme_test_program_helper < RealTy, CharTy > ().execute ();
};

}; /// end namespace cme_tst

#endif /// CME_TST_CME_TEST_PROGRAM_HPP

/**
* 14.03.2009 - �������� �.�.
**/
