/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 12:33:31 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/27 13:50:27 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ReverseIterators.hpp"

/* - For std::vector implementation i ll to use and array and reallocate blocks of
		memory when the total_size > capacity
*/

namespace ft {
	
	template<typename T, class Alloc>
	class vector
	{
		public:

		/* Member types */
		typedef T						value_type;
		typedef const T					const_value_type;
		typedef T*						pointer;
		typedef const T*				const_pointer;
		typedef T&						reference;
		typedef const T&				const_reference;
		typedef	std::size_t				size_of_type;
		typedef Alloc					allocator_type;

		protected:

		/* Core */
		size_of_type					total_size;
		size_of_type					capacity;
		pointer							objs;
		allocator_type					memory;
		// resize ?

		/* Vector iterator */
		template<typename I, bool is_accesor>
		class vec_it : public vec_iterator<I, is_accesor>
		{
			protected:

			using vec_iterator<I, is_accesor>::curr;

			public:

			/* Iterator member types */
			typedef I															value_type;
			typedef typename auto_const<is_accesor, I*, const I*>::obj_type		pointer;
			typedef typename auto_const<is_accesor, I&, const I&>::obj_type		reference;
			typedef typename remove_const<I>::obj_type							not_const_type;
			typedef lst_it<not_const_type, false>								not_const_iterator;
			typedef std::ptrdiff_t												difference_type;
			typedef std::random_access_iterator_tag								iterator_category;

			/* Iterator default methods */
			vec_it() : vec_iterator<I, false> () {}
			vec_it(const vec_iterator<I, false> &it) : vec_iterator<I, is_accesor>(it.curr) {}
			vec_it(const vec_it<I, is_accesor> &target) : vec_iterator<I, is_accesor>(target) {}
			// using operator= <-WHY???
			~vec_iterator() {}
		};

		/* Vector reverse iterator */
		template<typename Iterator>
		class vec_rev_it : public reverse_iterator
		{
			protected:
			
			using reverse_iterator<Iterator>::it;
			typedef typename reverse_iterator<Iterator>::not_const_iterator not_const_iterator;

			public:
			
			/* Reverse iterator default methods */
			vec_rev_it() : reverse_iterator<Iterator>() {}
			vec_rev_it(const reverse_iterator<not_const_iterator> &it) : reverse_iterator<Iterator>(it) {}
			vec_rev_it(const vec_rev_it<Iterator> &target) : reverse_iterator<Iterator>(target.it) {}
			// using op= WHY
			~vec_rev_it() {}
		};

		/* Iterators class members types */
		typedef lst_it<T, false>				iterator;
		typedef lst_it<T, true>					const_iterator;
		typedef lst_rev_it<iterator>			reverse_iterator;
		typedef lst_rev_it<const_iterator>		const_reverse_iterator;

		/* Core fill methods */
		
		/* Class default methods */
		
		/* List of methods */

			/* Member functions */
			// assign
			// get_allocator
			/* Element access */
			// at
			// operator[]
			// front
			// back
			/* Iterators */
			// begin
			// end
			// rbegin
			// rend
			/* Capacity */
			// empty
			// size
			// max_size
			// reserve
			// capacity
			/* Modifiers */
			// clear
			// insert
			// erase
			// push_back
			// pop_back
			// resize
			// swap
			/* Non-member functions */
	};

	
}