/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 21:41:43 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/29 14:41:36 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ReverseIterator.hpp"

/* - The easiest way to implement std::list is using a doubly-linked list 
	between a head node and a tail node.
	- Must support constant time insertion and removal from anywhere in the
	list but fast random access (like array or vector) isn't supported.
	- Adding, removing or moving the elements within the list or across several
	list does not ivalidate the iterators or references. An iterator will be 
	invalidated only when the corresponding element is deleted.
*/

namespace {

	template<typename T, class Alloc>
	class list
	{
		
		public:
		
		typedef T						value_type;
		typedef const T					const_value_type;
		typedef T*						pointer;
		typedef const T*				const_pointer;
		typedef T&						reference;
		typedef const T&				const_reference;
		typedef	std::size_t				size_of_type;
		typedef Alloc					allocator_type;

		protected:

		typedef ft::Node<T>				Node;

		typedef typename Memory::template rebind<Node>::other node_mem;

		uint32_t						total_size;
		Node							*head;
		Node							*tail;
		Alloc							memory;

		/* Node list definition */
		template <typename Object>
		struct Node
		{
			Object		data;
			Node		*prev;
			Node		*next;
			Node(const Object &d = Object(), Node *p = NULL, Node *n = NULL) : data(d), prev(p), next(n) {}	
		};

		public :
		
		/* Implementation of list bidirectional iterator */
		template <typename I, bool is_accesor>
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
			~lst_iterator() {}
		
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

		/* Implementaion of list reverse iterator */
		template<typename Iterator>
		class lst_rev_it : public rev_iterator<Iterator>
		{
			protected:

			using	rev_iterator<Iterator>::it;

			public:

			typedef typename rev_iterator<Iterator>::not_const_iterator	not_const_iterator;
			
			lst_rev_it() : reverse_iterator<Iterator>() {}
			lst_rev_it(const rev_iterator<not_const_iterator> &it) : rev_iterator<Iterator>(it) {}
			lst_rev_it(const lst_rev_it<Iterator> &target) : rev_iterator<Iterator>(target.it) {}
			// op = ¿?
			~lst_rev_it() {}
		};

		typedef lst_iterator<T, false>		iterator;
		typedef lst_iterator<T, true>		const_iterator;
		typedef lst_rev_it<iterator>		reverse_iterator;
		typedef lst_rev_it<const_iterator>	const_reverse_iterator;

		/* Fill methods */

		/* Inserts new_node between node1 and node2 */
		void							node_insert(Node *new_node, Node *node1, Node *node2) {
			node1->next = new_node;
			new_node->prev = node1;
			node2->prev = new_node;
			new_node->next = node2;
		}

		/* Allocates a new node with and uses value to fill it */
		typename list<T, Alloc>::Node	*node_create(const T &value) {
			Node	*new_node = node_mem(memory).allocate(1);
			node_mem(memory).construct(new_node, Node(value));
			new_node->data = value;
			return (new_node);
		}

		/* Deletes target_node from the doubly-linked list */
		void							node_erase(Node *target_node) {
			target_node->prev->next = target_node->next;
			target_node->next->prev = target_node->prev;
			node_mem(memory).destroy(target_node);
			node_mem(memory).deallocate(target_node, 1);
			target_node = NULL;
		}

		/* In the doubly-linked list node1 takes the place of node2 and reciprocally
			node2 takes the place of node1 */
		void							node_swap(Node *node1, Node *node2) {
			if (node1->next == node2 || node2->prev == node1)
			{
				node1->prev->next = node1->next;
				node1->next->prev = node1->prev;
				node_insert(node1, node2, node2->next);
			}
			else if (node2->next == node1 || node1->prev == node2)
			{
				node2->prev->next = node2->next;
				node2->next->prev = node2->prev;
				node_insert(node2, node1, node1->next);
			}
			else
			{
				Node *aux;
				aux->prev = node1->prev;
				aux->next = node1->next;
				node1->prev->next = node1->next;
				node1->next->prev = node1->prev;
				node2->prev->next = node2->next;
				node2->next->prev = node2->prev;
				node_insert(node1, node2->prev, node2->next);
				node_insert(node2, aux->prev, aux->next);
			}
		}

		/* Deletes all nodes between head and tail */
		void							clear_all_nodes() {
			Node *tmp = head->next;
			Node *remember;
			while (tmp != tail)
			{
				remenber = tmp;
				node_mem(memory).destroy(tmp);
				node_mem(memory).deallocate(tmp, 1);
				tmp = remember;
			}
			head->next = tail;
			tail->prev = head;
			total_size = 0;
		}

		void							init() {
			head = node_mem(memory).allocate(1);
			node_mem(memory).construct((Node *)head, Node());
			head->next = tail = node_mem(memory).allocate(1);
			node_mem(memory).construct((Node *)tail, Node());
			tail->prev = head;
		}

		void							clear() {
			clear_all_nodes();
			node_mem(memory).destroy((Node *)head);
			node_mem(memory).deallocate(head, 1);
			head = NULL;
			node_mem(memory).destroy((Node *)tail);
			node_mem(memory).deallocate(tail, 1);
		}

		/* Class default methods */
		public :

		list(const allocator_type &mem) : memory(mem) total_size(0) { init(); }
		list(uint32_t count, const T &value, const allocator_type &mem) : memory(mem) total_size(0) { init(); for(uint32_t i(0); i < count; i++) push_back(value); }
		template<typename InputIt>
		list(InputIt first, InputIt last, const allocator_type &mem) : memory(mem) total_size(0) { init(); assign(first, last); }
		list(const list<value_type, Memory> &target) : total_size(0){ init(); *this = target; }
		~list() { clear(); }

		/* List methods */

			/* Member functions */
		template<typename InputIt>
		void						assign(InputIt first, InputIt last);
		void						assign(size_of_type nb_bytes, const value_type &value);
		// get allocator()
			/* Element access */
		reference					front();
		const_reference				front() const;
		reference					back();
		const_reference				back() const;
			/* Iterators */
		iterator					begin();
		const_iterator				begin() const;
		iterator					end();
		const_iterator				end() const;
		reverse_iterator			rbegin();
		const_reverse_iterator		rbegin() const;
		reverse_iterator			rend();
		const_reverse_iterator		rend() const;
			/* Capacity */
		bool						empty() const;
		uint32_t					size() const;
		uint32_t					max_size() const;
			/* Modifiers */
		void						clear();
		iterator					insert(iterator pos, const value_type &value = value_type());
		template<class Inputit>
		void						insert(iterator pos, Inputit first, Inputit last);
		// mmm is the definitions up and bellow in cpp 17 ???
		void						insert(iterator pos, size_of_type count, const value_type &value = value_type());
		iterator					erase(iterator pos);
		iterator					erase(iterator first, iterator last);
		void						push_back(const value_type &value);
		void						push_front(const value_type &value);
		void						pop_back();
		void						pop_front();
		void						resize(size_of_type count);
		void						resize(size_of_type count, const value_type &value = value_type());
		void						swap(list<value_type, Memory> &other);
			/* Opererations */
		void						merge(list<value_type, Memory> &other);
		template<class Compare>
		void						merge(list<value_type, Memory> &other, Compare cmp);
		void						splice(const iterator pos, const list<value_type, Memory> &other);
		void						splice(const iterator pos, const list<value_type, Memory> &other, const iterator i);
		void						splice(const iterator pos, const list<value_type, Memory> &other, const iterator first, const iterator last);
		void						remove(const value_type &value);
		template<class Predicate>
		void						remove_if(Predicate pred);
		void						reverse();
		void						unique();
		template<class BinaryPredicate>
		void						unique(BinaryPredicate bin_pred);
		void						sort();
			/* Non-member functions */
		template<class T, class Alloc>
		bool						operator==(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs);
				template<class T, class Alloc>
		bool						operator!=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs);
				template<class T, class Alloc>
		bool						operator<(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs);
				template<class T, class Alloc>
		bool						operator<=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs);
				template<class T, class Alloc>
		bool						operator>(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs);
				template<class T, class Alloc>
		bool						operator>=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs);
	};
}

/* Implementation of member functions */

	/* assign */
template<typename T, class Alloc>
template<typename InputIt>
void				list<T, Alloc>::assign(InputIt first, InputIt last)
{
	clear_all_nodes();
	for ((void)first; first != last; first++)
		push_back(*first);
}

template<typename T, class Alloc>
void				list<T, Alloc>::assign(size_of_type count, const value_type &value)
{
	clear_all_nodes();
	for (size_of_type i(0); i < count; i++)
		push_back(value);
}

/* Implementation of element access methods */

	/* front */
	/* calling front in a empty container causes undefined behaviour */
template<typename T, class Alloc>
T					&list<T, Alloc>::front() { return (head->next); }
template<typename T, class Alloc>
const T				&list<T, Alloc>::front() const { return (head->next); }

	/* back */
	/* calling back in a empty cointainer causes undefined behaviour */
template<typename T, class Alloc>
T					&list<T, Alloc>::back() { return (tail->prev); }
template<typename T, class Alloc>
const T				&list<T, Alloc>::back() const { return (tail->prev); }

/* Implementation of iterators methods */

	/* begin */
	/* if the list is empty it will return an iterator equal to end() */
template<typename T, class Alloc>
typename list<T, Alloc>::iterator				list<T, Alloc>::begin() { return (iterator(head->next)); }
template<typename T, class Alloc>
typename list<T, Alloc>::const_iterator			list<T, Alloc>::begin() const { return (const_iterator(head->next)); }

	/* end */
	/* if the list is empty it will return an iterator equal to begin() */
template<typename T, class Alloc>
typename list<T, Alloc>::iterator				list<T, Alloc>::end() { return (iterator(tail->prev)); }
template<typename T, class Alloc>
typename list<T, Alloc>::const_iterator			list<T, Alloc>::end()  const { return (const_iterator(tail->prev)); }

	/* rbegin */
	/* if the list is empty it will return an iterator equal to rend() */
template<typename T, class Alloc>
typename list<T, Alloc>::reverse_iterator		list<T, Alloc>::rbegin() { return (reverse_iterator(tail->prev)); }
template<typename T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator	list<T, Alloc>::rbegin() const { return (const_reverse_iterator(tail->prev)); }

	/* rend */
	/* if the list is empty it will return an iterator equal to rbegin() */
template<typename T, class Alloc>
typename list<T, Alloc>::reverse_iterator		list<T, Alloc>::rend() { return (reverse_iterator(head->next)); }
template<typename T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator	list<T, Alloc>::rend() const { return (const_reverse_iterator(head->next)); }

/* Implementation of capacity methods */

	/* empty */
template<typename T, class Alloc>
bool					list<T, Alloc>::empty() const { return (head->next == tail || tail->prev == head); }

	/* size */
template<typename T, class Alloc>
uint32_t				list<T, Alloc>::size() const { return (total_size); }

	/* max_size */
template<typename T, class Alloc>
uint32_t				list<T, Alloc>::max_size() const { return (std::numeric_limits<uint32_t>::max() / sizeof(Node)); }

/* Implementation of modifiers methods */

	/* clear */
template<typename T, class Alloc>
void								list<T, Alloc>::clear() { clear_all_nodes(); }

	/* insert */
template<typename T, class Alloc>
typename list<T, Alloc>::iterator	list<T, Alloc>::insert(iterator pos, const value_type &value)
{
	Node	*i = ((lst_it<T, is_accesor> *)&pos).curr; // is wrong
	node_insert(node_create(value), i->prev, i);
	++total_size;
	return (pos);
}
template<typename T, class Alloc>
template<typename InputIt>
void								list<T, Alloc>::insert(list<T, Alloc>::iterator pos, InputIt first, InputIt last)
{
	for (Inputit i(first) : i != last)
		insert(pos, *i); // updates the size in insert()
}
template<typename T, class Alloc>
void								list<T, Alloc>::insert(iterator pos, size_of_type count, const value_type &value)
{
	for (uint32_t i(0) : i < count)
		insert(pos, value); // value or value[i] ?
}

	/* erase */
template<typename T, class Alloc>
typename list<T, Alloc>::iterator	list<T, Alloc>::erase(iterator pos)
{
	// to do
}
template<typename T, class Alloc>
typename list<T, Alloc>::iterator	list<T, Alloc>::erase(iterator first, iterator last)
{
	// to do
}
	
	/* push_back */
template<typename T, class Alloc>
void									list<T, Alloc>::push_back(const T &value)
{
	node_insert(node_create(value), head, head->next);
	++total_size;
}

	/* push_front */
template<typename T, class Alloc>
void									list<T, Alloc>::push_front(const T &value)
{
	node_insert(node_create(value), tail->prev, tail);
	--total_size;
}

	/* pop_back */
template<typename T, class Alloc>
void									list<T, Alloc>::pop_back()
{
	if (!empty())
	{
		node_erase(head->next);
		--total_size;
	}
}

	/* pop_front */
template<typename T, class Alloc>
void									list<T, Alloc>::pop_front()
{
	if (!empty())
	{
		node_erase(tail->prev);
		--total_size;
	}
}
	/* resize */
template<typename T, class Alloc>
void									list<T, Alloc>::resize(size_of_type count)
{
	if (count == total_size)
		return ;
	if (count < total_size)
		for (size_type i(0); i < total_size; i++)
			node_erase(tail->prev);
	else
	{
		for (size i(0); i < total_size; i++)
			insert(end(), count - total_size, NULL);
	}
	total_size = count;
}
template<typename T, class Alloc>
void									list<T, Alloc>::resize(size_of_type count, const value_type &value)
{
	if (count == total_size)
		return ;
	if (count < total_size)
		for (size_type i(0); i < total_size; i++)
			node_erase(tail->prev);
	else
	{
		for (size i(0); i < total_size; i++)
			insert(end(), count - total_size, value);
	}
	total_size = count;
}

	/* swap */
template<typename T, class Alloc>
void									list<T, Alloc>::swap(list<T, Alloc> &other)
{
	std::swap(head, other.head);
	std::swap(tail), other.tail);
	std::swap(total_size, other.total_size);
}
	
/* Implementation of operation methods */

	/* merge */
template<typename T, class Alloc>
void									list<T, Alloc>::merge(list<T, Alloc> &other)
{
	tail->prev->next = other.head->next;
	other.head->next->prev = tail->prev;
	total_size += other.total_size;
	other->head->next = other->tail;
	other->tail->prev = other->head;
	other.total_size = 0;
	sort();
}

template<typename T, class Alloc>
template<class Compare>
void									list<T, Alloc>::merge(list<T, Alloc> &other, Compare comp)
{
	// to do the same with a comp function pointer 
}

	/* splice */
template<typename T, class Alloc>
void									list<T, Alloc>::splice(const iterator pos, const list<T, Alloc> &other)
{
	splice(pos, other, other.begin(), other.end());
}
template<typename T, class Alloc>
void									list<T, Alloc>::splice(const iterator pos, const list<T, Alloc> &other, const iterator first, const iterator last)
{
	// need to insert the nodes from other to *this starting in first and ending in last
	// have to manage the sizes
	// have to do it using the iterators
}
template<typename T, class Alloc>
void									list<T, Alloc>::splice(const iterator pos, const list<T, Alloc> &other, const iterator i)
{
	// to do
}

	/* remove */
template<typename T, class Alloc>
void									list<T, Alloc>::remove(const T &value)
{
	for (iterator i(begin()); i != end(); i++)
		*i == value ? i = erase(i) : NULL;

}

	/* remove_if */
template<typename T, class Alloc>
template<class Predicate>
void									list<T, Alloc>::remove_if(Predicate pred)
{
	for (iterator i(begin()); i != end(); i++)
		pred(*i) ? i = erase(i) : NULL;
}

	/* reverse */
template<typename T, class Alloc>
void									list<T, Alloc>::reverse()
{
	Node *aux = head->next;
	while (aux != tail)
	{
		std::swap(aux->next, aux->prev)
		aux = aux->prev;
	}
	std::swap(head->next, tail->prev);
}

	/* unique */
template<typename T, class Alloc>
void									list<T, Alloc>::unique()
{
	for (iterator i(begin()); i != end(); i++)
		for iterator y(i + 1); y != end(); y++)
			*y == *i ? erase(y) : NULL;
}

template<typename T, class Alloc>
template<class BinaryPredicate>
void									list<T, Alloc>::unique(BinaryPredicate bin_pred)
{
		for (iterator i(begin()); i != end(); i++)
		for iterator y(i + 1); y != end(); y++)
			bin_pred(*i, *y) ? erase(y) : NULL;
}

	/* sort */
template<typename T, class Alloc>
void									list<T, Alloc>::sort()
{
	// have to search the best algorithm for sort
}

/* Implementaion of non-member functions */

// Undefined stuff have to be declared here or in iterator ????

//template<typename T, class Alloc>
//T				&list<T, Alloc>::operator[](size_of_tyoe count) { return (*(begin() + count)); }
//template<typename T, class Alloc>
//const T		&list<T, Alloc>::operator[](size_of_type count) const { return (*(begin() + count)); }

/*
template<typename T, class Alloc>
list<T, Alloc>	&list<T, Alloc>::operator=(const list<T, Alloc> &other)
{
	memory = other.memory;
	for (list<T, Alloc>::const_iterator i(other.begin()); i != other.end(); i++)
		push_back(*i);
	return (*this);
}
*/

template<typename T, class Alloc>
bool			operator==(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
{
	if (lhs.total_size != rhs.total_size)
		return (false);
	for (list<T, Alloc>::const_iterator i(lhs.begin()); i++)
		for (list<T, Alloc>::const_iterator y(rhs.begin()); y++)
			if (*i != *y) return (false);
	return (true);
}

template<typename T, class Alloc>
bool			operator!=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs) { return (!(lhs == rhs)); }
template<typename T, class Alloc>
bool			operator<(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
{
	// to do need to read about it
}
template<typename T, class Alloc>
bool			operator<=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs) { return ((lhs == rhs) | (lhs < rhs)); }
template<typename T, class Alloc>
bool			operator>(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs) { return (!(lhs <= rhs)); }
template<typename T, class Alloc>
bool			operator>=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs) { return ((lhs == rhs) | (lhs < rhs); }

// end! (have to complete the code but not write further)