/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReverseIterators.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:40:13 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/27 14:09:54 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "aux.hpp"
#include <cstddef>

#define nullptr NULL

namespace ft {
	
	template<typename T, class Memory = ft::allocator<T>>
	class vector;
	template<typename T, class Memory = ft::allocator<T>>
	class list;

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
		virtual ~vec_iterator() {}
		
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
		// need to use friend function cause they need to acces curr while been called out of the class (cogical if i need 2 classes for cmp) 
		template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
		friend bool			operator==(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr == it2.curr); }
		template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
		friend bool			operator!=(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr != it2.curr); }
		template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
		friend bool			operator<(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr < it2.curr); }
		template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
		friend bool			operator<=(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr <= it2.curr); }
		template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
		friend bool			operator>(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr > it2.curr); }
		template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
		friend bool			operator>=(vec_iterator<T1, is_accesor1> it1, vec_iterator<T2, is_accesor2> it2) { return (it1.curr >= it2.curr); }
	};

	template <typename Object>
	struct Node
	{
		Object		data;
		Node		*prev;
		Node		*next;
		Node(const Object &d = Object(), Node *p = NULL, Node *n = NULL) : data(d), prev(p), next(n) {}	
	};

	template<typename T, bool is_accesor>
	class lst_iterator
	{
		public:

		/* Use the std methods developped in aux.hpp for define if T is const or not const */
		typedef typename remove_const<T>::obj_type										not_const_type;	
		typedef lst_iterator<not_const_type, false>										not_const_iterator;
		typedef typename auto_const<is_accesor, Node<T>*, const Node<T>*>::obj_type		node_pointer;
		typedef T																		value_type;
		typedef typename auto_const<is_accesor, T*, const T*>::obj_type					pointer;
		typedef typename auto_const<is_accesor, T&, const T&>::obj_type					reference;
		typedef ::std::ptrdiff_t														difference_type;
		typedef std::bidirectional_iterator_tag											iterator_category;

		/* Node ptr use for iterate in the list */
		Node<not_const_type>															*curr;
		
		/* Default class methods: constructors, copy constructor and destuctor */
		lst_iterator() : curr(NULL) {}
		lst_iterator(node_pointer ptr) : curr(ptr) {}
		lst_iterator(const not_const_iterator &target) {
			not_const_iterator copy = target;
			std::swap(curr, copy.curr); // for destroy the old curr (when it gonna be out of the exec scope it'll be destroyed)
			return (*this);
		}
		virtual ~lst_iterator() {}
		
		/* Implementation of list iterator operators */
		lst_iterator		&operator=(const not_const_iterator &target) { curr = target.curr; return (*this); }
		lst_iterator		operator++(int) { lst_iterator aux(curr); operator++(); return (aux); }
		lst_iterator		&operator++() { curr = curr ? curr->next : curr; return (*this); }
		lst_iterator		operator--(int) { lst_iterator aux(curr); operator--(); return (aux); }
		lst_iterator		&operator--() { curr = curr ? curr->prev : curr; return (*this); }
		reference			operator*() {
			if (curr)
				return (curr->data);
			throw::std::out_of_range(std::string("Error: null ptr hasn't address to deference"));
		}
		bool				operator==(node_pointer n1, node_pointer n2) { return (n1 == n2); }
		bool				operator!=(node_pointer n1, node_pointer n2) { return (n1 != n2); }
		// need to use friend function cause they need to acces curr while been called out of the class (cogical if i need 2 classes for cmp) 
		template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
		friend bool			operator==(const lst_iterator<T1, is_accesor1> &it1, const lst_iterator<T2, is_accesor2> &it2) {
			return (it1.curr == it2.curr);
		}
		template<typename T1, typename T2, bool is_accesor1, bool is_accesor2>
		friend bool			operator!=(const lst_iterator<T1, is_accesor1> &it1, const lst_iterator<T2, is_accesor2> &it2) {
			return (it1.curr != it2.curr);
		}
	};
	
	template<class U>
	class rev_iterator
	{
		public:

		typedef U											iterator_type;
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