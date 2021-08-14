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
#include <boost/contract/public_function.hpp>
#include <complex>
#include <type_traits>
#include <concepts>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <numbers>

#define EPSILON 0.0001

template <std::floating_point T>
class point_t
{
public:
	T x;
	T y;
	point_t():x(0),y(0){}
	point_t(T _x, T _y) : x(_x), y(_y) {}
	point_t<T> operator+(point_t<T> p)
	{
		return point_t<T>(x+p.x,y+p.y);
	}
	point_t<T> operator-(point_t<T> p)
	{
		return point_t<T>(x-p.x,y-p.y);
	}
	point_t<T> operator*(T factor)
	{
		return point_t<T>(x*factor,y*factor);
	}
	void print(std::ostream& out) const
	{
		out << "(" << x << "," << y << ")";
	}
	point_t<T> rotate(T angle)
	{
		return point_t<T>((x*std::cos(angle))-(y*std::sin(angle)),
		                   (x*std::sin(angle)) + (y*std::cos(angle)));
	}
};


template <std::floating_point T>
constexpr T dist_squared(const point_t<T> p1, const point_t<T> p2)
{
	return ((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y) * (p1.y - p2.y));
}

template <std::floating_point T>
constexpr T distance(const point_t<T> p1, const point_t<T> p2)
{
	return std::sqrt(dist_squared(p1, p2));
}

template <std::floating_point T>
constexpr bool close_enough(T a, T b)
{
	return (std::fabs(a - b) < EPSILON);
}

template <std::floating_point T>
class triangle_t
{
protected:
	point_t<T> p1;
	point_t<T> p2;
	point_t<T> p3;

public:
	triangle_t(point_t<T> _p1, point_t<T> _p2,
		   point_t<T> _p3) : p1(_p1), p2(_p2), p3(_p3) {}
	triangle_t(T x1, T y1, T x2, T y2, T x3, T y3) : p1(x1,y1),
	p2(x2,y2), p3(x3,y3) {}
	void print(std::ostream& out) const
	{
		p1.print(out); out << ":";
		p2.print(out); out << ":";
		p3.print(out); out << std::endl;
	}
	void scale(T scale_factor)
	{
		p1=p1*scale_factor;
		p2=p2*scale_factor;
		p3=p3*scale_factor;
	}
	void translate(point_t<T> new_origin) 
	{
		p1=p1-new_origin;
		p2=p2-new_origin;
		p3=p3-new_origin;

	}

	void rotate(T angle)
	{
		p1=p1.rotate(angle);
		p2=p2.rotate(angle);
		p3=p3.rotate(angle);
		/*
		p1.x = (p1.x*std::cos(angle)) - (p1.y*std::sin(angle));
		p1.y = (p1.y*std::cos(angle)) + (p1.x*std::sin(angle));
		p2.x = (p2.x*std::cos(angle)) - (p2.y*std::sin(angle));
		p2.y = (p2.y*std::cos(angle)) + (p2.x*std::sin(angle));
		p3.x = (p3.x*std::cos(angle)) - (p3.y*std::sin(angle));
		p3.y = (p3.y*std::cos(angle)) + (p3.x*std::sin(angle));
		*/
	}
};


template <std::floating_point T>
class isosceles_triangle_t : public boost::contract::constructor_precondition<isosceles_triangle_t<T>>,
			     public triangle_t<T>
{
public:
	isosceles_triangle_t(point_t<T> _p1, point_t<T> _p2,
			     point_t<T> _p3) : boost::contract::constructor_precondition<isosceles_triangle_t<T>>([&]
				  {
				  T d1 = dist_squared(_p1, _p2);
				  T d2 = dist_squared(_p2, _p3);
				  T d3 = dist_squared(_p1, _p3);
				  BOOST_CONTRACT_ASSERT(close_enough(d1, d2) ||
					close_enough(d2, d3) || close_enough(d1, d3));
				  }),
					      triangle_t<T>::triangle_t(_p1, _p2, _p3) { }
	void invariant() const
	{
		point_t<T> pp1,pp2,pp3;
		pp1 = triangle_t<T>::p1;
		pp2 = triangle_t<T>::p2;
		pp3 = triangle_t<T>::p3;
		T d1 = distance(pp1,pp2);
		std::cout << d1 << std::endl;
		T d2 = distance(pp2,pp3);
		std::cout << d2 << std::endl;
		T d3 = distance(pp1, pp3);
		std::cout << d3 << std::endl;
		BOOST_CONTRACT_ASSERT(close_enough(d1, d2) ||
				      close_enough(d2, d3) || close_enough(d1, d3));
	}


	void translate(point_t<T> new_origin) 
	{
		boost::contract::check c = boost::contract::public_function(this);
		triangle_t<T>::translate(new_origin);

	}

	void scale(T scale_factor)
	{
		boost::contract::check c = boost::contract::public_function(this);
		triangle_t<T>::scale(scale_factor);
	}
	void rotate(T angle)
	{
		boost::contract::check c = boost::contract::public_function(this);
		triangle_t<T>::rotate(angle);
	}
};

int main()
{
	using ftype_t = long double;
	isosceles_triangle_t<ftype_t> t1(point_t<ftype_t>(0.0,0.0),
					 point_t<ftype_t>(1.0,0.0),
					 point_t<ftype_t>(0.0,1.0));
					 t1.print(std::cout);
	t1.translate(point_t<ftype_t>(2.0,2.0));
	t1.print(std::cout);
	t1.rotate(std::numbers::pi_v<ftype_t>/6.0);
	t1.print(std::cout);
	t1.scale(1.5);
	t1.print(std::cout);
	return EXIT_SUCCESS;
}