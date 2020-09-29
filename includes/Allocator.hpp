/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Allocator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:38:53 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/25 14:31:05 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* Implementation of allocator class from std */
/* https://en.cppreference.com/w/cpp/memory/allocator */

#include <stdio.h> // std::size_t
#include <limits> // std::numeric_limits


namespace ft {

	template <class T>
	class allocator {

		public :
		
		typedef T 					value_type;
		typedef T					*pointer;
		typedef const T				*const_pointer;
		typedef T					&reference;
		typedef const T				&const_reference;
		typedef std::size_t			size_type;
		typedef ::std::ptrdiff_t	difference_type;
		template <class U>
		struct rebind {
			typedef rebind { typedef allocator<U> other; };
		
		allocator() throw() {}
		~allocator() throw() {}
		allocator(const allocator &) throw() {}
		template <class U>
		allocator(const allocator<U> &) throw() {}
		
		pointer			address(reference value) const { return (&value); }
		const_pointer	address(const_reference value) const {return (&value); }
		pointer			allocate(size_type nb) {
			return ((pointer)(::operator new(nb * sizeof(T)))); }
		void			deallocate(pointer ptr, size_type nb) {
			(void)nb; ::operator delete((void *)p); }
		size_type		max_size() const throw() {
			return (std::numeric_limits<size_t>::max() / sizeof(T)); }
		void			construct(pointer p, const T &value) { new ((void *)p) T(value); }
		void			destroy(pointer p) { p->~T(); }
	};
}