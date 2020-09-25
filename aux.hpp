/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:38:28 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/25 17:36:13 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Allocator.hpp"
#include <exception>
#include <stdint.h>

namespace ft {
	
	/* Implementation of std::add_const */
	template<class T> struct add_const { typedef const T obj_type; };
	template<class T> struct add_const<const T> { typedef const T obj_type; };
	
	/* Implementation of std::remove_const */
	template<class T> struct remove_const {typedef T obj_type; };
	template<class T> struct remove_const<const T> {typedef T obj_type; };

	/* Implementation of std::is_same */
	template<class T, class U> struct is_same : std::false_type{};
	template<class T> struct is_same : std::true_type{};

	/* Implementation of std::enable_if */
	template<bool B, class T = void> struct enable_if {};
	template<class T> struct enable_if<true, T> {typedef T obj_type; };
	
		/* Implementation of auto_const a template that type automatically with const or not const */
	template<bool B, class TrueType, class FalseType> struct auto_const;
	template<class TrueType, class FalseType> struct auto_const<true, TrueType, FalseType> {typedef TrueType obj_type; };
	template<class TrueType, class FalseType> struct auto_const<false, TrueType, FalseType> {typedef FalseType obj_type; };

	/* Implementation of iterator operations:
		- advance: advance iterator (function template)
		- distance: return distance between iterators (function template)
		- begin: iterator to beginnig (function template)
		- end: iterator to end (function template)
		- prev: get iterator to previous element (function template)
		- next: get iterator to next element (function template)	
	 */
	template<typename Iterator>
	void			advance(Iterator &head, uint32_t nb) { while (nb--) {head++;} }

	template<typename Iterator>
	int64_t			distance(Iterator head, Iterator tail) {
		Iterator	positive(head);
		Iterator	negative(head);
		int64_t		dist = -1;

		while (positive != tail)
		{
			if (negative == tail)
				return (-dist);
			++dist; // this could be dab if return when dist == -1
			++positive;
			--negative;
			if (head == positive == negative) // can i do this ?
				throw std::exception();
		}
		return (dist);
	}

	template <typename Iterator>
	Iterator		next(Iterator head, uint32_t nb) { while (nb--) {head++;} return (head); }
	// Have i to do the rest or i have to implement it in the operators in ReverseIterators.hpp ?
	
	/* Implementation of std::is_const */
	template<typename T>
	bool			is_const(T &addr) { static_cast<void>(addr); return (false); }
	template<typename T>
	bool			is_const(const T &addr) { static_cast<void>(addr); return (true); }
	
	/* Implementation of std::less */
	template<typename T = void>
	bool			less(T lhs, T rhs) { return (lhs < rhs); }

}