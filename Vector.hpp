/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 12:33:31 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/27 17:34:18 by plamtenz         ###   ########.fr       */
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
		class vec_rev_it : public rev_iterator
		{
			protected:
			
			using rev_iterator<Iterator>::it;
			/* Reverse iterator member type */
			typedef typename rev_iterator<Iterator>::not_const_iterator not_const_iterator;

			public:
			
			/* Reverse iterator default methods */
			vec_rev_it() : reverse_iterator<Iterator>() {}
			vec_rev_it(const rev_iterator<not_const_iterator> &it) : rev_iterator<Iterator>(it) {}
			vec_rev_it(const vec_rev_it<Iterator> &target) : rev_iterator<Iterator>(target.it) {}
			// using op= WHY
			~vec_rev_it() {}
		};

		/* Iterators class members types */
		public :

		typedef lst_it<T, false>				iterator;
		typedef lst_it<T, true>					const_iterator;
		typedef lst_rev_it<iterator>			reverse_iterator;
		typedef lst_rev_it<const_iterator>		const_reverse_iterator;

		/* Core fill methods */

		/* Allocates a new_array with the new capacity, move all the objs from the new_array,
			swaps the new_array with the current array and deletes the (old) current array */
		void				array_reserve(int32_t new_capacity) {
			if (new_capacity < total_size)
				return ;
			value_type		*new_array = memory.allocate(new_capacity);
			for (uint32_t i(0) : i < total_size)
				new_array = std::move(objs + i);
			capacity = new_capacity;
			std::swap(objs, new_array);
			for (uint32_t i(0) : i < total_size)
				memory.destroy(new_array + i);
			memory.deallocate(new_array, total_size);
			// have i to update here total_size i guess
		}
	
		/* Resizes the array if there isn't enought space */
		void				array_resize(int32_t new_total_size) {
			if (new_total_size > capacity)
				array_reserve(new_total_size);
			total_size = new_total_size;
		}
		
		/* Class default methods */
		vector(const allocator_type &mem) : total_size(0), capacity(0), objs(NULL), memory(mem) {}
		vector(uint32_t count, const value_type &value, const allocator_type &mem) : total_size(count), capacity(count), memory(mem) {
			objs = memory.allocate(total_size);
			for (uint32_t i(0) : i < total_size)
				memory.construct(objs + i, value);
		}
		template<typename InputIt>
		vector(InputIt first, InputIt last, const allocator_type &mem) : total_size(0), capacity(0), objs(NULL), memory(mem) {
			assing(first, last);
		}
		vector(const vector<T, Alloc> &target) : objs(NULL) { *this = target; }
		~vector() { for (size_of_type i(0) : i < total_size) memory.destroy(objs + i); memory.deallocate(objs, total_size); }
		
		/* List of methods */

			/* Member functions */
		template<class InputIt>
		void								assign(InputIt first, InputIt last);
		void								assign(size_of_type count, const value_type &value);
		// get_allocator
			/* Element access */
		reference							at(size_of_type pos);
		const_reference						at(size_of_type pos) const;
		reference							operator[](size_of_type pos);
		const_reference						operator[](size_of_type pos) const;
		reference							front();
		const_reference						front() const;
		reference							back();
		const_reference						back() const;
			/* Iterators */
		iterator							begin();
		const_iterator						begin() const;
		iterator							end();
		const_iterator						end() const;
		reverse_iterator					rbegin();
		const_reverse_iterator				rbegin() const;
		reverse_iterator					rend();
		const_reverse_iterator				rend() const;
			/* Capacity */
		bool								empty() const;
		size_of_type						size() const;
		size_of_type						max_size() const;
		void								reserve(size_of_type new_cap);
		size_of_type						capacity() const;
			/* Modifiers */
		void								clear();
		template<typename InputIt>
		void								insert(iterator pos, InputIt first, InputIt last);
		void								insert(iterator pos, size_of_type count, const value_type &value = value_type());
		iterator							insert(iterator pos, const value_type &value = value_type());
		iterator							erase(iterator pos);
		iterator							erase(iterator first, iterator last);
		void								push_back(const value_type &value);
		void								pop_back();
		void								resize(size_of_type count, value_type value = value_type());
		void								swap(vector<T, Alloc> &other);
			/* Non-member functions */
		template<class T, class Alloc>
		bool								operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);
		template<class T, class Alloc>
		bool								operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);
		template<class T, class Alloc>
		bool								operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);
		template<class T, class Alloc>
		bool								operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);
		template<class T, class Alloc>
		bool								operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);
		template<class T, class Alloc>
		bool								operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs); 
	};
	/* Implementation fo class methods */
	
	/* Member functions */
		/* assign */
	template<typename T, class Alloc>
	template<class InputIt>
	void			vector<T, Alloc>::assign(InputIt first, InputIt last)
	{
		int32_t count(0);
		for (InputIt i(first) : i != last)
			count++;
		if (count > capacity)
			array_reserve(count);
		for (size_of_type i(0) : i < total_size)
			memory.destroy(&at(i + 1));
		total_size = 0;
		for (InputIt i(first) : I != last) // or insert could be better than loop
			push_back(*it);
	}
	template<typename T, class Alloc>
	void			vector<T, Alloc>::assign(size_of_type count, const value_type &value)
	{
		if (size < 0)
			return ; // throw an exeption
		if (count > capacity)
			array_reserve(count);
		for (size_of_type i(0) : i < total_size)
			memory.destroy(&at(i + 1));
		total_size = 0;
		for (InputIt i(first) : I < count) // or insert could be better than loop
			push_back(*it);
	}
		/* get_allocator */

	/* Element access methods */
		/* at */
	template<typename T, class Alloc>
	T						&vector<T, Alloc>::at(size_of_type pos)
	{
		if (!(pos < size()))
			throw std::out_of_range(std::string("Error: out of bounds acess"));
		return (objs[pos]);
	}
	template<typename T, class Alloc>
	const T					&vector<T, Alloc>::at(size_of_type pos) const
	{
		if (!(pos < size()))
			throw std::out_of_range(std::string("Error: out of bounds"));
		return (objs[pos]);
	}
		/* operator[]*/
	template<typename T, class Alloc>
	T						&vector<T, Alloc>::operator[](size_of_type pos) { return (objs[pos]); }
	template<typename T, class Alloc>
	const T					&vector<T, Alloc>::operator[](size_of_type pos) const { return (objs[pos]); }

		/* front */
	template<typename T, class Alloc>
	T						&vector<T, Alloc>::front() { return (objs[0]); }
	template<typename T, class Alloc>
	const T					&vector<T, Alloc>::front() const { return (objs[0]); }

		/* back */
	template<typename T, class Alloc>
	T						&vector<T, Alloc>::back() { return (objs[total_size -1]); }
	template<typename T, class Alloc>
	const T					&vector<T, Alloc>::back() const { return (objs[total_size - 1]); }

	/* Iterators */
		/* begin */
	
		/* end */
		/* rbegin */
		/* rend */
	/* Capacity */
		/* empty */
		/* size */
		/* max_size */
		/* reserve */
		/* capacity */
	/* Modifiers */
		/* clear */
		/* insert */
		/* push_back */
		/* pop_back */
		/* resize */
		/* swap */
	/* Non-member functions */
}