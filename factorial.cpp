/*    Copyright (C) 2021  Saurabh Joshi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <boost/contract.hpp>
#include <cstdlib>
#include <iostream>

int factorial(int x)
{
	int prev_result,result,i;
	boost::contract::check c = boost::contract::function()
	.precondition([&]{
		BOOST_CONTRACT_ASSERT(x>=0);
	})
	.postcondition(
		[&]{
			BOOST_CONTRACT_ASSERT(result>=x);
			BOOST_CONTRACT_ASSERT((x==0 && result ==1) || (result/prev_result==x));
		}
	);

	result=1;
	prev_result=1;
	i=1;
	for(i=1;i<=x;i++)
	{
		prev_result=result;
		result*=i;
	}
	return result;
}

int main()
{
	for(int i=0;i<100;i++)
	{
		std::cout << i << ":" << factorial(i) << std::endl;
	}
	return EXIT_SUCCESS;
}