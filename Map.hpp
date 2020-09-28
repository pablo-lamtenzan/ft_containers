/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 21:05:28 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/28 18:39:58 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ReverseIterators.hpp"
#include "Stack.hpp"

# define BLACK		0
# define RED		1

//https://en.wikipedia.org/wiki/Red%E2%80%93black_tree

namespace ft {
	
	template<class Key, class T, class Compare, class Alloc>
	class map
	{
		public:

		/* Class member types */
		typedef	Key										key_type;
		typedef T										mapped_type;
		typedef std::pair<const key_type, mapped_type>	value_type;
		typedef	std::size_t								size_of_type;
		typedef std::ptrdiff_t							difference_type;
		typedef Compare									key_compare;
		typedef	Alloc									allocator_type;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef value_type*								pointer;
		typedef const value_type*						const_pointer;
		
		/* Value compare class implementation (cmp objs of value_type) */
		class value_compare
		{
			protected:

			/* Protected member objects */
			key_compare		comp;
			uint32_t		size;

			public:

			/* Member types */
			typedef bool						result_type;
			typedef std::pair<const Key, T> 	first_argument_type;
			typedef std::pair<const Key, T>		second_argument_type;

			/* Member functions */
			protected: 
			value_compare() {}

			public :
			value_compare(const value_compare &other) : comp(other.comp) {}
			value_compare(const key_value &c) : comp(c) {}
			~value_compare() {}

			bool operator()(const value_type &lhs, const value_type &rhs) const {
				return (comp(lhs.first, rhs.last));
			}
		};

		protected:

		typedef ft::RBNode<value_type, key_type>		Node;
		typedef Alloc::template rebind<Node>::other		node_mem;

		/* Implementation of Map iterators */
		template<typename I, bool is_accesor>
		class rbt_it : public rbt_iterator<I, is_accesor>
		{
			protected:
			
			// to do

			public:
			
			typedef I															value_type;
			typedef typename auto_const<is_accesor, I*, const I*>::obj_type		pointer;
			typedef typename auto_const<is_accesor, I&, const I&>::obj_type		reference;
			typedef typename remove_const<I>::obj_type							not_const_type;
			typedef lst_it<not_const_type, false>								not_const_iterator;
			typedef std::ptrdiff_t												difference_type;
			typedef std::random_access_iterator_tag								iterator_category;

			/* Defualt methods */
			rbt_it() : rbt_iterator<I, is_accesor>() {}
			// to do
			~rbt_it() {}
		};

		/* Implementaion of Map reverse iterators */
		template<typename Iterator>
		class rbt_rev_it : public rev_iterator
		{
			protected:

			typedef typename rev_iterator<Iterator>::not_const_iterator not_const_iterator;

			public:

			rbt_rev_it() : rev_it<Iterator>() {}
			rbt_rev_it(const rbt_rev_it<not_const_iterator> &it) : rev_it<Iterator>(it) {}
			rbt_rev_it(const rev_it<Iterator> &rev_it) : rbt_rev_it<Iterator>(rev_it.curr) {}
			// op=
			~rbt_rev_it() {}
		};

		public:

		/* Class member iterators types */
		typedef rbt_it<value_type, false>				iterator;
		typedef rbt_it<value_type, true>				const_iterator;
		typedef rbt_rev_it<iterator>					reverse_iterator;
		typedef rbt_rev_it<const_iterator>				const_reverse_iterator;			

		protected:

		Node								*root;
		Node								*nil;

		/* fill methods */

		bool								rbt_empty() const { return (root == nil); }
		// ~RBTree()

		void								rbt_transplant(Node *lhs, Node *rhs) {
			if (lhs->parent_node == nil)
				root = rhs;
			else if (rbt_is_left_child(lhs))
				lhs->parent_node->children_left = rhs;
			else
				lhs->parent_node->children_right = rhs;
			rhs->parent_node = lhs->parent_node;
		}
		
		void								rbt_delete_node(Node *node) {
			// to do
		}
		
		void								rbt_destroy(Node *node) {
			if (node != nil)
			{
				rbt_destroy(node->children_left);
				rbt_destroy(node->children_right);
				// mem deallocation using the allocator
			}
		}
		// RbTREE()
		// RBTREE()
		uint32_t							rbt_size() const { return (size); }
		template<typename Children1, typename Children2>
		Node								*rbt_generic_rotatation(Node * x, Children1 ch1, Children2 ch2) {
			// to do
		}
		bool								rbt_generic_add(key_type &key, mapped_type &data) {
			// to do
		}
		bool								rbt_add(const key_type &key, const mapped_type &data) {
			return (rbt_generic_add(const_cast<key_type&>(key), const_cast<mapped_type&>(data)));
		}
		
		Node								*rtb_minimum(Node *node) const {
			while (node->children_left != nil)
				node = node->children_left;
			return (node);
		}

		Node								*rbt_maximum(Node *node) const {
			while (node->children_right != nil)
				node = node->children_right;
			return (node);
		}
		std::pair<key_type, mapped_type>	rbt_get_min() const {
			if (empty())
				throw std::underflow_error("Error: underflow detected");
			Node *min = rbt_minimum(root);
			return (std::pair<key_type, mapped_type>(min->key, min->data));
		}
		
		std::pair<key_type, mapped_type>	rbt_get_max() const {
			if (empty())
				throw std::underflow_error("Error: underflow detected");
			Node *max = rbt_maximum(root);
			return (std::pair<key_type, mapped_type>(max->key, max->data));
		}
		
		Node								*rbt_find_node(const key_type &key) const {
			Node *node = root;
			while (node != nil)
			{
				if (comp(key, node->key))
					node = node->children_left;
				else if (comp(node->key, key))
					node = node->children_right;
				else
					return (node);
			}
			return node;
		}
		
		bool								rbt_get(const key_type &key, mapped_type &result) {
			Node *found = rbt_find(key); // mmm
			return (found ? (result = found->data ? true : true) : false);
		}
		Node								*create_node(key_type key, mapped_type data) {
			// to do
		}
		
		Node								*bst_add_recursive(key_type &key, mapped_type &data, Node *parent, Node *node) {
			if (node == nil)
			{
				// create node
				node->parent_node = parent;
				size++;
				return (node);
			}
			else if (comp(key, node->key))
				return (rbt_add_recursive(key, data, node, node->children_left));
			else if (comp(node->key, key))
				return (rbt_add_recursive(key, data, node, node->children_right));
			else
				return (NULL);
		}
		
		Node								*bst_add(key_type &key, mapped_type &data) {
			Node *node = root;
			Node *parent = nil;

			while (node != nil)
			{
				parent = node;
				if (comp(node->key, key))
					node = node->children_right;
				else if (comp(key, node->key))
					node = node->children_left;
				else
					return (NULL);
			}
			
			Node *new_node = create_node(std::forward<key_type>(key), std::forward<mapped_type>(data));
			if (root == nil)
				root = new_node;
			else if (comp(parent->key, key))
				parent->children_right = new_node;
			else
				parent->children_left = new_node;
			new_node->parent_node = parent;
			size++;
			return (new_node);
		}
		
		template<typename Children1, typename Children2>
		void								rbt_generic_fixed_add(Node &node, Children1 ch1, Children2 ch2) {
			Node *uncle = ch2(node->parent_node->parent_node);
			if (uncle->color & RED)
				rbt_insert_move_up(node, uncle);
			else
			{
				if (node == ch2(node->parent_node))
					rbt_generic_rotatation(node = node->parent_node, ch1, ch2);
				node->parent_node->parent_node->color |= RED;
				rbt_generic_rotatation(node->parent_node->parent_node, ch1, ch2)->color &= BLACK;
			}
		}
		// is_left_child
		void								rbt_fixed_add(Node *node) { 
			while (node->parent_node->color & RED)
			{
				if(rbt_is_left_child(node->parent_node))
					rbt_generic_fixed_add(node, node->children_left, node->children_right);
					// check the previous line
				else
					rbt_generic_fixed_add(node, node->children_right, node->children_left);
					// check the previous line
			}
			root->color &= BLACK;
		}

		void								rbt_insert_move_up(Node &node, Node &uncle) {
			node->parent_node->parent_node->color |= RED;
			uncle->color &= BLACK;
			node->parent_node &= BLACK;
			node = node->parent_node->parent_node;
		}

		template<typename Action>
		void								rbt_iter_in_order_walk(Node *node, Action action) {
			Node *min = rtb_minimum(node);
			while (min != node->parent_node)
			{
				action(min->key, min->data);
				if (min->children_right != nil)
					min = rtb_minimum(min->children_right);
				else
				{
					while (min->parent_node && rbt_is_right_chlid(min))
						min = min->parent_node;
					min = min->parent_node;
				}
			}
		}

		template<typename Action>
		void								rbt_in_order_walk(Action action) const {
			rbt_iter_in_orther_walk(root, action);
		}

		template<typename Action>
		void								rbt_recursive_in_order_walk(Node *node, Action action) const{
			if (node != nil)
			{
				rbt_recursive_in_order_walk(node->children_left, action);
				action(node->key, node->data);
				rbt_recursive_in_order_walk(node->children_right, action);
			}
		}

		template<typename Action>
		void								rbt_stack_in_order_walk(Node *node, Action action) const
		{
			stack<Node *> stck;

			stck.push(NULL);
			stck.push(NULL);

			while (!stck.empty())
			{
				while (node != nil)
				{
					stck.push(node->children_right);
					stck.push(node);
					node = node->children_left;
				}
				if (stck.top())
					action (stck.top()->key,stck.top()->data);
				
				stck.pop();
				node = stck.top();
				stck.pop();
			}
		}

		template<typename Children1, typename Children2>
		void							rbt_generic_fixed_delete(Node *node, Children1 ch1, Children2 ch2) {
			Node *aux = ch2(node->parent_node);
			if (aux->color & RED)
			{
				std::swap(aux->color, node->parent_node->color);
				rbt_generic_rotatation(node->parent_node, ch1, ch2);
				aux = ch2(node->parent_node);
			}
			if (aux->children_left->color & RED && aux->children_right->color & RED)
			{
				aux->color |= RED;
				node = node->parent_node;
			}
			else
			{
				if (ch2(aux) & RED)
				{
					std::swap(aux->color, ch1(aux)->color)
					aux = rbt_generic_rotatation(aux, ch1, ch2);
				}
				aux->color = node->parent_node->color;
				node->parent_node &= BLACK;
				ch2(aux)->color &= BLACK;
				rbt_generic_rotatation(node->parent_node, ch1, ch2);
				node = root;
			}
		}

		template<typename Children1, typename Children2>
		void							rbt_fixed_remove(Node *node) {
			while (node != root && node->color & BLACK)
			{
				if (rbt_is_left_child(node))
					rbt_generic_fixed_delete(node, node->children_left, node->children_right);
					// check the previosu declaration
				else
					rbt_generic_fixed_delete(node, node->children_right, node->children_left);
					// check the previous declaration
			}
			node->color &= BLACK;
		}
		
		void							rbt_remove(const key_type &key, mapped_type &data)
		{
			Node *node = rbt_find_node(key);
			if (node != nil)
			{
				data = node->data;
				// delete v2
			}
			return (node != nil);
		}

		public:

		/* Class methods definition*/

		/* Member functions */
		// get allocator
		// op=
		/* Element access */
		mapped_type									&operator[](const key_type &key);
		/* Iterators */
		iterator									begin();
		const_iterator								begin() const;
		iterator									end();
		const_iterator								end() const;
		reverse_iterator							rbegin();
		const_reverse_iterator						rbegin() const;
		reverse_iterator							rend();
		const_reverse_iterator						rend() const;
		/* Capacity */
		bool										empty() const;
		size_of_type								size() const;
		size_of_type								max_size() const;
		/* Modifiers */
		void										clear();
		iterator									insert(iterator pos, const value_type &value);
		template <class InputIt>
		void										insert(InputIt first, InputIt last);
		std::pair<iterator, bool>					insert(const value_type &value);
		void										erase(iterator first, iterator last);
		void										erase(iterator pos);
		size_of_type								erase(const key_type &key);
		void										swap(map<Key, T, Compare, Alloc> &other);
		/* Lookup */
		size_of_type								count(const key_type &key);
		iterator									find(const key_type &key);
		const_iterator								find(const key_type &key) const;
		std::pair<iterator, iterator>				equal_range(const key_type &key);
		std::pair<const_iterator, const_iterator>	equal_range(const key_type &key) const;
		iterator									lower_bound(const key_type &key);
		const_iterator								lower_bound(const key_type &key) const;
		iterator									upper_bound(const key_type &key);
		const_iterator								upper_bound(const key_type &key) const;
		/* Observes */
		key_compare									key_comp() const;
		value_compare								value_comp() const;
		/* non members methods definition */
		template<class Key, class T, class Compare, class Alloc>
		bool										operator==(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs);
		template<class Key, class T, class Compare, class Alloc>
		bool										operator!=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs);
		template<class Key, class T, class Compare, class Alloc>
		bool										operator<(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs);
		template<class Key, class T, class Compare, class Alloc>
		bool										operator<=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs);
		template<class Key, class T, class Compare, class Alloc>
		bool										operator>(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs);
		template<class Key, class T, class Compare, class Alloc>
		bool										operator>=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs);
	};

	/* Implementation of class methods */

	/* Member functions */
		/* get_allocator */
	
	/* Element access methods */
		/* operator[] */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::mapped_type					&map<Key, T, Compare, Alloc>::operator[](const key_type &key)
	{
		
	}
	
	/* Iterators */
		/* begin */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::iterator						map<Key, T, Compare, Alloc>::begin()
	{
		
	}
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::const_iterator					map<Key, T, Compare, Alloc>::begin() const
	{
		
	}
		/*end */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::iterator						map<Key, T, Compare, Alloc>::end()
	{
		
	}
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::const_iterator					map<Key, T, Compare, Alloc>::end() const
	{
		
	}

		/* rbegin */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::reverse_iterator				map<Key, T, Compare, Alloc>::rbegin()
	{
		
	}
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::const_reverse_iterator			map<Key, T, Compare, Alloc>::rbegin() const
	{
		
	}
	
		/* rend */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::reverse_iterator				map<Key, T, Compare, Alloc>::rend()
	{
		
	}
	
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::const_reverse_iterator			map<Key, T, Compare, Alloc>::rend() const
	{
		
	}

	/* Capacity */
		/* empty */
	template<class Key, class T, class Compare, class Alloc>
	bool										map<Key, T, Compare, Alloc>::empty() const
	{
		
	}
		/* size */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::size_of_type	map<Key, T, Compare, Alloc>::size() const
	{
		
	}
		/* max_size */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::size_of_type	map<Key, T, Compare, Alloc>::max_size() const
	{
		
	}

	/* Modofiers */
		/* clear */
	template<class Key, class T, class Compare, class Alloc>
	void										map<Key, T, Compare, Alloc>::clear()
	{
		
	}
	
		/* insert */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::iterator		map<Key, T, Compare, Alloc>::insert(iterator pos, const value_type &value)
	{
		
	}
	template<class Key, class T, class Compare, class Alloc>
	template <class InputIt>
	void										map<Key, T, Compare, Alloc>::insert(InputIt first, InputIt last)
	{
		
	}
	//template<class Key, class T, class Compare, class Alloc>
	//std::pair<iterator, bool>					map<Key, T, Compare, Alloc>::insert(const value_type &value);
		/* erase */
	template<class Key, class T, class Compare, class Alloc>
	void										map<Key, T, Compare, Alloc>::erase(iterator first, iterator last)
	{

	}
	
	template<class Key, class T, class Compare, class Alloc>
	void										map<Key, T, Compare, Alloc>::erase(iterator pos)
	{

	}
	
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::size_of_type	map<Key, T, Compare, Alloc>::erase(const key_type &key)
	{
		
	}
		/* swap */
	template<class Key, class T, class Compare, class Alloc>
	void										map<Key, T, Compare, Alloc>::swap(map<Key, T, Compare, Alloc> &other)
	{
		
	}

	/* Lookup */
		/* count */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::size_of_type	map<Key, T, Compare, Alloc>::count(const key_type &key)
	{
		
	}
		/* find */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::iterator		map<Key, T, Compare, Alloc>::find(const key_type &key)
	{
		
	}

	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::const_iterator	map<Key, T, Compare, Alloc>::find(const key_type &key) const
	{
		
	}
		/* equal_range */
	//template<class Key, class T, class Compare, class Alloc>
	//std::pair<iterator, iterator>				map<Key, T, Compare, Alloc>::equal_range(const key_type &key);
	//template<class Key, class T, class Compare, class Alloc>
	//std::pair<const_iterator, const_iterator>	map<Key, T, Compare, Alloc>::equal_range(const key_type &key) const;
	//	/* lower_bound */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::iterator		map<Key, T, Compare, Alloc>::lower_bound(const Key &key)
	{
		
	}

	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::const_iterator	map<Key, T, Compare, Alloc>::lower_bound(const Key &key) const
	{
		
	}

		/* upper_bound */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::iterator		map<Key, T, Compare, Alloc>::upper_bound(const key_type &key)
	{
		
	}

	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::const_iterator	map<Key, T, Compare, Alloc>::upper_bound(const key_type &key) const
	{
		
	}

	/* Observes */
		/* key_comp */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::key_compare	map<Key, T, Compare, Alloc>::key_comp() const
	{
		
	}

		/* value_comp */
	template<class Key, class T, class Compare, class Alloc>
	map<Key, T, Compare, Alloc>::value_compare	map<Key, T, Compare, Alloc>::value_comp() const
	{
		
	}

	/* Non-member function */
	template<class Key, class T, class Compare, class Alloc>
	bool										map<Key, T, Compare, Alloc>::operator==(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		
	}
	template<class Key, class T, class Compare, class Alloc>
	bool										map<Key, T, Compare, Alloc>::operator!=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		
	}
	template<class Key, class T, class Compare, class Alloc>
	bool										map<Key, T, Compare, Alloc>::operator<(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		
	}
	template<class Key, class T, class Compare, class Alloc>
	bool										map<Key, T, Compare, Alloc>::operator<=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		
	}
	template<class Key, class T, class Compare, class Alloc>
	bool										map<Key, T, Compare, Alloc>::operator>(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		
	}
	template<class Key, class T, class Compare, class Alloc>
	bool										map<Key, T, Compare, Alloc>::operator>=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs)
	{
		
	}
}
