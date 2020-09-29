/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReverseIterator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:40:13 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/29 14:41:27 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "aux.hpp"
#include "Allocator.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>

#define nullptr NULL

namespace ft {
	
	template<class InputIt>
	class rev_iterator
	{
		public:

		typedef InputIt										iterator_type;
		typedef typename iterator_type::not_const_iterator	not_const_iterator;
		typedef typename iterator_type::not_const_type		not_const_type;
		typedef typename iterator_type::value_type			value_type;
		typedef typename iterator_type::pointer				pointer;
		typedef typename iterator_type::reference			reference;
		typedef typename iterator_type::difference_type		difference_type;
		typedef typename iterator_type::iterator_category	iterator_category;

		iterator_type										it;

		/* Default class methods: constructors, copy constructor and destructor */
		rev_iterator() {}
		rev_iterator(const iterator_type &i) : it(i) {}
		rev_iterator(const rev_iterator<not_const_iterator> &rev_it) {
			rev_iterator<not_const_iterator> copy = rev_it;
			std::swap(it, copy.it);
			return (*this);
		}
		virtual ~rev_iterator() {}
		
		/* Implementation of reverse_iterator operators */
		rev_iterator				&operator=(const rev_iterator<not_const_iterator> &target) {
			it = target.it; return (*this);
		}
		rev_iterator				operator+(difference_type nb) { rev_iterator aux(*this); while (nb--) ++aux; return (aux); }
		rev_iterator				operator-(difference_type nb) { rev_iterator aux(*this); while (nb--) --aux; return (aux); }
		rev_iterator				operator++(int) { rev_iterator aux(*this); --it; return (aux); }
		rev_iterator				&operator++() { --it; return (*this); }
		rev_iterator				operator--(int) { rev_iterator aux(*this); ++it; return (aux); }
		reve_iterator				&operator--() { ++it; return (*this); }
		reference					operator*() { return (*--it); }
		reference					operator[](difference_type nb) { return (it + (-nb - 1)); }
		pointer						operator->() { return ((--it).operator->()); }
		template<typename U1, typename U2>
		friend bool					operator==(const rev_iterator<U1> &it1, const rev_iterator<U2> &it2) { return (it1.it == it2.it); }
		template<typename U1, typename U2>
		friend bool					operator!=(const rev_iterator<U1> &it1, const rev_iterator<U2> &it2) { return (it1.it != it2.it); }
		template<typename U1, typename U2>
		friend bool					operator<(const rev_iterator<U1> &it1, const rev_iterator<U2> &it2) { return (it1.it < it2.it); }
		template<typename U1, typename U2>
		friend bool					operator<=(const rev_iterator<U1> &it1, const rev_iterator<U2> &it2) { return (it1.it <= it2.it); }
		template<typename U1, typename U2>
		friend bool					operator>(const rev_iterator<U1> &it1, const rev_iterator<U2> &it2) { return (it1.it > it2.it); }
		template<typename U1, typename U2>
		friend bool					operator>=(const rev_iterator<U1> &it1, const rev_iterator<U2> &it2) { return (it1.it >= it2.it); }
		
		iterator_type				get_iterator() { return (it); }
	};
}