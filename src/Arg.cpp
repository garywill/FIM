/* $LastChangedDate: 2017-07-23 13:04:56 +0200 (Sun, 23 Jul 2017) $ */
/*
 Arg.cpp : argument lists

 (c) 2007-2017 Michele Martone

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "fim.h"
#include "Arg.h"

namespace fim
{

int fim_args_opt_count(const args_t& args, const char oc)
{
	int aoc = 0;
	fim::string s;

	for(size_t i=0;i<args.size();++i)
	{
		if ( args[i].length() && args[i].at(0) == oc )
			aoc++;
		else
			break;
	}
	return aoc;
}

bool fim_args_opt_have(const args_t& args, fim::string optname)
{
	fim::string s;

	for(size_t i=0;i<args.size();++i)
		if ( args[i] == optname )
			return true;
	return false;
}

	std::ostream& operator<<(std::ostream& os, const args_t& v)
	{
#if 0
		std::cout<<"{";
		for(size_t i=0;i<v.size();++i)
			os<<v[i]<<",";
		std::cout<<"}";
#else
		for(size_t i=0;i<v.size();++i)
		{
			if(i>0)
				os << " ";
			os << fim_shell_arg_escape(v[i]);
		}
#endif
		return os;
	}
} /* namespace fim */
