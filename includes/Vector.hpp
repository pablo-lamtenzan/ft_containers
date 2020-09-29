/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 12:33:31 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/29 14:42:00 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ReverseIterator.hpp"

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

		/* Vector iterator */
		template<typename T, bool is_accesor>
		class vec_iterator
		{
			public:
		
			/* Use the std methods developped in aux.hpp for define if T is const or not const */
			typedef typename remove_cost<T>::obj_type							not_const_type;
			typedef typename add_const<T>::obj_type								const_type;
			typedef vec_iterator<not_const_type, false>							not_const_iterator;
			typedef vec_iterator<const_type, true>								const_iterator;
			typedef T															value_type;
			typedef typename auto_const<is_accesor, T*, const T*>::obj_type		pointer;
			typedef typename auto_const<is_accesor, T&, const T&>::obj_type		reference;
			typedef ::std::ptrdiff_t											difference_type;
			typedef std::bidirectional_iterator_tag								iterator_category;

			/* Current address in the vector */
			T																	*curr;
		
			/* Default class methods: constructors, copy constructor and destuctor */
			vec_iterator() : curr(NULL) {}
			vec_iterator(pointer ptr) : curr(ptr) {}
			vec_iterator(const not_const_iterator &target) {
				not_const_iterator copy = target;
				std::swap(curr, copy.curr) // for destroy the old curr (when it gonna be out of the exec scope it'll be destroyed)
				return (*this);
			}
			~vec_iterator() {}
		
			/* Implementation of vector iterator operators */
			vec_iterator		&operator=(const not_const_type &target) { curr = target.ptr; return (*this); }
			vec_iterator		operator+(difference_type nb) { return (vec_iterator(curr + nb);) }
			int64_t				operator+(const not_const_iterator &obj) { return (((int64_t)curr + (int64_t)obj.curr) / (uint64_t)sizeof(value_type)); }
			int64_t				operator+(const const_iterator &obj) { return (((int64_t)curr + (int64_t)obj.curr) / (int64_t)sizeof(value_type)); }
			vec_iterator		operator-(difference_type nb) { return (vec_iterator(curr - nb)); }
			int64_t				operator-(const not_const_iterator &obj) { return (((int64_t)curr - (int64_t)obj.curr) / (int64_t)sizeof(value_type)); }
			int64_t				operator-(const const_iterator &obj) { return (((int64_t)curr - (int64_t)obj.curr) / (int64_t)sizeof(value_type)); }
			vec_iterator		operator+=(difference_type nb) { curr += nb; return (*this); }
			vec_iterator		operator-=(difference_type nb) { curr -= nb; return (*this); }
			vec_iterator		operator++(int) { vec_iterator aux(curr); operator++(); return (aux); }
			vec_iterator		&operator++() { curr++; return (*this); }
			vec_iterator		operator--(int) { vec_iterator aux(curr); operator--(); return (aux); }
			vec_iterator		&operator--() { curr--; return (*this); }
			reference			operator*() {
				if (curr)
					return (curr->data);
				throw::std::out_of_range(std::string("Error: null ptr hasn't address to deference"));
			}
			reference			operator[](difference_type nb) { return (curr + nb); }
			template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
			bool				operator==(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr == it2.curr); }
			template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
			bool				operator!=(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr != it2.curr); }
			template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
			bool				operator<(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr < it2.curr); }
			template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
			bool				operator<=(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr <= it2.curr); }
			template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
			bool				operator>(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr > it2.curr); }
			template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
			bool				operator>=(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr >= it2.curr); }
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
			// using op=
			~vec_rev_it() {}
		};

		/* Iterators class members types */
		public :

		typedef lst_iterator<T, false>			iterator;
		typedef lst_iterator<T, true>			const_iterator;
		typedef lst_rev_it<iterator>			reverse_iterator;
		typedef lst_rev_it<const_iterator>		const_reverse_iterator;

		/* Core fill methods */

		protected:
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
			if (new_total_size >= capacity)
				array_reserve(capacity ? new_total_size * 2 : 1);
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
		
		public: 
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
	template<typename T, class Alloc>
	typename vector<T, Alloc>::iterator					vector<T, Alloc>::begin() { return (iterator(objs)); }
	template<typename T, class Alloc>
	typename vector<T, Alloc>::const_iterator			vector<T, Alloc>::begin() const { return (const_iterator(objs)); }
	
		/* end */
	template<typename T, class Alloc>
	typename vector<T, Alloc>::iterator					vector<T, Alloc>::end() { return (iterator(objs + total_size)); }
	template<typename T, class Alloc>
	typename vector<T, Alloc>::const_iterator			vector<T, Alloc>::begin() const { return (const_iterator(objs + total_size)); }

		/* rbegin */
	template<typename T, class Alloc>
	typename vector<T, Alloc>::reverse_iterator			vector<T, Alloc>::rbegin() { return (reverse_iterator(end())); }
	template<typename T, class Alloc>
	typename vector<T, Alloc>::const_reverse_iterator	vector<T, Alloc>::rbegin() const { return (const_reverse_iterator(end())); }
	
		/* rend */
	template<typename T, class Alloc>
	typename vector<T, Alloc>::reverse_iterator			vector<T, Alloc>::rend() { return (reverse_iterator(begin())); }
	template<typename T, class Alloc>
	typename vector<T, Alloc>::const_reverse_iterator	vector<T, Alloc>::rend() { return (const_reverse_iterator(begin())); }
	
	/* Capacity */
		/* empty */
	template<typename T, class Alloc>
	bool							vector<T, Alloc>::empty() const { return (!total_size); }
	
		/* size */
	template<typename T, class Alloc>
	typename vector<T, Alloc>::size_of_type		vector<T, Alloc>::size() const { return (total_size); }
	
		/* max_size */
	template<typename T, class Alloc>
	typename vector<T, Alloc>::size_of_type		vector<T, Alloc>::max_size() const
	{
		return (std::numeric_limits<size_of_type>::max() / sizeof(value_type));
	}
		/* reserve */
	template<typename T, class Alloc>
	void										vector<T, Alloc>::reserve(size_of_type new_cap)
	{
		if (new_cap > max_size())
			throw std::bad_alloc(std::string("Error: trying to allocate more than containers max_size"));
		array_reserve(new_cap);
	}
		/* capacity */
	template<typename T, class Alloc>
	typename vector<T, Alloc>::size_of_type		vector<T, Alloc>::capacity() const { return (capacity); }
	
	/* Modifiers */
		/* clear */
	template<typename T, class Alloc>
	void										vector<T, Alloc>::clear()
	{
		while (total_size--)
			memory.destroy(&at(objs + total_size));
	}
		/* insert */
	template<typename T, class Alloc>
	template<typename InputIt>
	void										vector<T, Alloc>::insert(vector<T, Alloc>::iterator pos, InputIt first, InputIt last)
	{
		// do
	}
	template<typename T, class Alloc>
	typename vector<T, Alloc>::iterator			vector<T, Alloc>::insert(iterator pos, const value_type &value)
	{
		// to do
	}

	template<typename T, class Alloc>
	void										vector<T, Alloc>::insert(iterator pos, size_of_type count, const value_type &value)
	{
		// to do
	}
	
		/* push_back */
	template<typename T, class Alloc>
	void										vector<T, Alloc>::push_back(const T &value)
	{
		array_resize(total_size);
		memory.construct(objs + total_size++, value;
	}
	
		/* pop_back */
	template<typename T, class Alloc>
	void										vector<T, Alloc>::pop_back() { total_size ? memory.destroy(objs + total_size-- - 1) : 0; }

		/* resize */
	template<typename T, class Alloc>
	void										vector<T, Alloc>::resize(size_of_type count, value_type value)
	{
		// have i to do this definition of resize ??
	}

		/* swap */
	template<typename T, class Alloc>
	void										vector<T, Alloc>::swap(vector<T, Alloc> &other)
	{
		std::swap(objs, other.objs);
		std::swap(capacity, other.capacity);
		std::swap(total_size, other.total_size);
	}
	
	/* Non-member functions */

	/*
	template<typename T, class Alloc>
	T					&operator=(const vector<T, Alloc> &target)
	{
		uint32_t		remember = total_size;
		clear();
		memory.deallocate(objs, remember);
		total_size = target.size;
		capacity = target.capacity;
		objs = other.objs;
		for (uint32_t i(0) : i < total_size)
			memory.construct(objs + i, target.at(i));
		return (*this);
	}
	*/

	template<typename T, class Alloc>
	bool				operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		for (uint32_t i(0) : i < lhs.size())
			if (lsh[i] == rhs[i])
				return (false);
		return (lhs.size() == rhs.size());
	}
	template<typename T, class Alloc>
	bool				operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) { return (!(lhs == rhs)); }
	template<typename T, class Alloc>
	bool				operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		// to do have to read about this
	}
	template<typename T, class Alloc>
	bool				operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) { return ((lhs == rhs) | (lhs < rhs)); }
	template<typename T, class Alloc>
	bool				operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) { return (!(lhs <= rhs)); }
	template<typename T, class Alloc>
	bool				operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) { return (!(lhs < rhs)); }
}