/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 21:05:28 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/29 13:29:13 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// to do 
// change all children by child omg
// map iterator into class map

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
			key_compare					comp;
			uint32_t					total_size;
			Alloc						memory;

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

		template<typename T, typename Key>
		struct RBNode
		{
			enum class		Color
			{
				BLACK = 0,
				RED = 1
			};
			T				data;
			Key				key;
			Color			color;	
			RBNode			*child_left;
			RBNode			*child_right;
			RBNode			*parent_node;
		
		RBNode(Data d, Key k) : data(d), key(k), child_left(NULL); child_right(NULL), color(0) {}
		~RBNode() {}		
		};

		protected:

		typedef RBNode<value_type, key_type>			Node;
		typedef Alloc::template rebind<Node>::other		node_mem;

		/* Implementation of Map iterators */
		template<typename T, typename Key, bool is_accesor>
		class rbt_iterator
		{
			public:

			/* Use the std methods developped in aux.hpp for define if T is const or not const */
			typedef typename remove_const<T>::obj_type														not_const_type;	
			typedef rbt_iterator<not_const_type, false>														not_const_iterator;
			typedef typename auto_const<is_accesor, RBNode<T, Key>*, const RBNode<T, Key>*>::obj_type		node_pointer;
			typedef T																						value_type;
			typedef typename auto_const<is_accesor, T*, const T*>::obj_type									pointer;
			typedef typename auto_const<is_accesor, T&, const T&>::obj_type									reference;
			typedef ::std::ptrdiff_t																		difference_type;
			typedef std::bidirectional_iterator_tag															iterator_category;

			RBNode<not_const_type, Key>			*curr;
			RBNode<not_const_type, Key>* const	*root; // mmm

			/* Class defult methods implementation */
			rbt_iterator() : curr(NULL), root(NULL) {}
			rbt_iterator(RBNode<not_const_type, Key> &c, RBNode<not_const_type, Key>* const *r) : curr(c), root(r) {}
			rbt_iterator(const not_const_iterator &target) : curr(target.curr), root(target.root) {}
			~rbt_iterator() {}

			/* Class operator methods */
			reference				operator*() { curr ? return (curr->data) : throw std::out_of_range(std::string("Error: null ptr can't be deferenced")); }
			pointer					operator->() { curr ? return (&curr->data) : throw std::out_of_range(std::string("Error: null ptr can't be deferenced"))); }
			template<typename T, typename Key, typename is_accesor>
			bool					operator==(const rbt_iterator<T, Key, is_accesor> &lhs, const rbt_iterator<T, Key, is_accesor> &rhs) { return (lsh.curr == rhs.curr); }
			template<typename T, typename Key, typename is_accesor>
			bool					operator!=(const rbt_iterator<T, Key, is_accesor> &lhs, const rbt_iterator<T, Key, is_accesor> &rhs) { return (lsh.curr != rhs.curr); }
			rbt_iterator			&operator++() {
				// i have the function for do this in my lib
			}
			rbt_iterator			operator++(int) { rbt_iterator aux(curr, root); operator++(); return (aux); }
			rbt_iterator			&operator--() {
				// i have the function for do this in my lib
			}
			rbt_iterator			operator--(int) { rbt_iterator aux(curr, root); operator--(); return (aux); }
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
		typedef rbt_iterator<value_type, false>			iterator;
		typedef rbt_iterator<value_type, true>			const_iterator;
		typedef rbt_rev_it<iterator>					reverse_iterator;
		typedef rbt_rev_it<const_iterator>				const_reverse_iterator;			

		protected:

		Node								*root;
		Node								*nil;

		/* fill methods */

		/* Returns true if "target" is the right child */
		bool								rbt_is_right_child(Node *target) { return (target == target->parent_node->child_right); }
		/* Returns true if "target" is the left child */
		bool								rbt_is_left_child(Node *target) { return (target == target->parent_node->child_left); }
		/* Returns a pointed reference (void *&ptr ~= void **) to the left child of "l" */
		static Node							*&rbt_node_left_child(Node *l) { return (l->child_left); }
		/* Returns a pointed reference (void *&ptr ~= void **) to the right child of "r" */
		static Node							*&rbt_node_right_child(Node *r) { return (r->child_right); }
		/* Returns true if the rbt is empty */
		bool								rbt_empty() const { return (root == nil); }
		// ~RBTree()

		/* Puts src in dest and uptades the parent node pointers and itselfs pointer */
		void								rbt_move(Node *dest, Node *src) {
			if (dest->parent_node == nil)
				root = src;
			else if (rbt_is_left_child(dest))
				dest->parent_node->child_left = src;
			else
				dest->parent_node->child_right = src;
			dest->parent_node = src->parent_node;
		}
		
		/* destructs and deallocate recursivelly starting from node and
			iterating over the childrens */
		void								rbt_destroy(Node *node) {
			if (node != nil)
			{
				rbt_destroy(node->child_left);
				rbt_destroy(node->child_right);
				node_mem(memory).destroy(node);
				node_mem(memory).deallocate(node, 1);
			}
		}

		// RbTREE() do this in map constructor
		// RBTREE()

		/* Returns the current size of the rbt */
		uint32_t							rbt_size() const { return (size); }

		/* Generic rotation of rbt algorithm */
		template<typename Child1, typename Child2>
		Node								*rbt_generic_rotatation(Node *node, Child1 ch1, Child2 ch2) {
			Node *aux = ch2(node);
			ch2(node) = ch1(aux);
			
			if (ch1(aux) != nil)
				ch1(aux)->parent_node = node;
			if (node->parent_node == nil)
				root = aux;
			else if (node == ch1(node->parent_node))
				ch1(node->parent_node) = aux;
			else
				ch2(node->parent_node) = aux;
			aux->parent_node = node->parent_node;
			ch1(aux) = target_node;
			return (node->parent_node = aux);
		}
		
		/* Iterates node the most possible to the left of the rbt */
		Node								*rtb_minimum(Node *node) const {
			while (node->child_left != nil)
				node = node->child_left;
			return (node);
		}

		/* Iterates node the most possible to the right of the rbt */
		Node								*rbt_maximum(Node *node) const {
			while (node->child_right != nil)
				node = node->child_right;
			return (node);
		}

		/* Returns the pair in the minimum (top left) node */
		std::pair<key_type, mapped_type>	rbt_get_min() const {
			if (empty())
				throw std::underflow_error("Error: underflow detected");
			Node *min = rbt_minimum(root);
			return (std::pair<key_type, mapped_type>(min->key, min->data));
		}
		
		/* Returns the pair in the maximum (top rigt) node */
		std::pair<key_type, mapped_type>	rbt_get_max() const {
			if (empty())
				throw std::underflow_error("Error: underflow detected");
			Node *max = rbt_maximum(root);
			return (std::pair<key_type, mapped_type>(max->key, max->data));
		}
		
		/* Search in the rbt using the key given as parameter adn return the result */
		Node								*rbt_find_node(const key_type &key) const {
			Node *result = root;
			while (result != nil)
			{
				if (comp(key, result->key))
					result = result->child_left;
				else if (comp(result->key, key))
					result = result->child_right;
				else
					return (result);
			}
			return result;
		}
		
		
		/* Search in the rbt and returns true if the found result is equal to the given one as parameter */
		bool								rbt_get(const key_type &key, mapped_type &result) {
			Node *found = rbt_find_node(key);
			return (found ? (result = found->data ? true : true) : false);
		}

		/* Inits, constructs and allocates a new node, returns a pointer to the new node. */
		Node								*create_node(key_type &&key, mapped_type &&data) {
			Node *new_node(std::forward<Key> key, std::forward<T> key);
			node_mem(memory).construct(new_node);
			node_mem(memory).allocate(1);
			return (new_node);
		}

		/* Add a new node allocating it in the tail of one of the branches. It uses the key given as parameter for sellect the destination branch */
		Node								*bst_add_recursive(key_type &&key, mapped_type &&data, Node *parent, Node **new_node) {
			if (*new_node == nil)
			{
				*new_node = create_node(std::forward<Key>(key), std::forward<T>(data));
				(*new_node)->parent_node = parent;
				total_size++;
				return (*new_node);
			}
			else if (comp(key, (*new_node)->key))
				return (rbt_add_recursive(key, data, new_node, (*new_node)->child_left));
			else if (comp((*new_node)->key, key))
				return (rbt_add_recursive(key, data, new_node, (*new_node)->child_right));
			else
				return (NULL);
		}
		/* Add a new node allocating it in the tail of one of the branches. It uses the key given as parameter for sellect the destination branch */
		Node								*bst_add(key_type  &&key, mapped_type &&data) {
			Node *aux = root;
			Node *parent = nil;

			/* Searchs for the parent node */
			while (aux != nil)
			{
				parent = aux;
				if (comp(aux->key, key))
					aux = aux->child_right;
				else if (comp(key, aux->key))
					aux = aux->child_left;
				else
					return (NULL);
			}
			/* Allocates and search for destination branch  */
			Node *new_node = create_node(std::forward<key_type>(key), std::forward<mapped_type>(data));
			if (root == nil)
				root = new_node;
			else if (comp(parent->key, key))
				parent->child_right = new_node;
			else
				parent->child_left = new_node;
			new_node->parent_node = parent;
			total_size++;
			return (new_node);
		}
		
		/* Add new node using the red blac tree insertion algorithm (only used if adding a new node will broke the rbt rules) 
			Child1 Child2 are the typename of 2 function pointers. Each function returns a child but the functions ptrs can be
			called in reverse order depending of the parent node color in "rbt_fixed_add", */
		template<typename Child1, typename Child2>
		void								rbt_generic_fixed_add(Node **new_node, Child1 ch1, Child2 ch2) {
			Node *uncle = ch2((*new_node)->parent_node->parent_node);
			/* If the uncle node is red, "new_node" will be inserted as the parent of "uncle" node */
			if (uncle->color & RED)
				rbt_insert_move_up(new_node, uncle);
			else
			{
				/* if "new node" is the right branch of his  parent, rotate putting "new_node" in the parent address */
				if (*new_node == ch2((*new_node)->parent_node))
					rbt_generic_rotatation(*new_node = (*new_node)->parent_node, ch1, ch2);
				/* The grand parent must be red now */
				(*new_node)->parent_node->parent_node->color |= RED;
				/* Rotate current grand parent node */
				rbt_generic_rotatation((*new_node)->parent_node->parent_node, ch1, ch2)->color &= BLACK;
			}
		}

		/* Calls generic insertion algorith, swiching parameter depends of the parent node color */
		void								rbt_fixed_add(Node *new_node) { 
			while (new_node->parent_node->color & RED)
			{
				if(rbt_is_left_child(new_node->parent_node))
					rbt_generic_fixed_add(&new_node, rbt_node_left_child , tbt_node_right_child);
				else
					rbt_generic_fixed_add(&new_node, rbt_node_right_child, rbt_node_left_child);
			}
			root->color &= BLACK;
		}

		/* Adds a new element in rbt using the key, returns true on success */
		bool								rbt_generic_add(key_type &&key, mapped_type &&data) {
			Node *new_node = bst_add_recursive(std::forward<Key>(key), std::forward<T>::(data), nil, root);
			if (new_node)
				rbt_fixed_add(new_node);
			return (new_node);
		}

		/* Adds a new element in rbt using the key, returns true on success */
		bool								rbt_add(const key_type &key, const mapped_type &data) {
			return (rbt_generic_add(const_cast<key_type&>(key), const_cast<mapped_type&>(data)));
		}

		/* Adds a new element in rbt using the key, returns true on success */
		bool								rbt_add(const key_type &&key, const mapped_type &data) {
			return (rbt_generic_add(std::move(key), const_cast<mapped_type&>(data)));
		}

		/* Adds a new element in rbt using the key, returns true on success */
		bool								rbt_add(const key_type &key, const mapped_type &&data) {
			return (rbt_generic_add(const_cast<key_type&>(key), std::move(data)));
		}

		/* Adds a new element in rbt using the key, returns true on success */
		bool								rbt_add(const key_type &&key, const mapped_type &&data) {
			return (rbt_generic_add(std::move(key), std::move(data)));
		}

		/* Inserts a "insert_node" upstairs his uncle, uptading colors (of course) */
		void								rbt_insert_move_up(Node **insert_node, Node &uncle) {
			(*insert_node)->parent_node->parent_node->color |= RED;
			uncle->color &= BLACK;
			(*insert_node)->parent_node &= BLACK;
			*insert_node = (*insert_node)->parent_node->parent_node;
		}

		/* Iterates aplicating "action" into all the nodes of the path. 
			The iteration is donne left to right. */
		template<typename Action>
		void								rbt_iter_in_order_walk(Node *node, Action action) {
			/* Iterates the further possible to the left starting in node */
			Node *min = rtb_minimum(node);
			/* end of the iteration */
			while (min != node->parent_node)
			{
				action(min->key, min->data);
				/* if has a right child iterates the further possible to the right */
				if (min->child_right != nil)
					min = rtb_minimum(min->child_right);
				else
				{
					/* if "min" doesn't have a right child
					and isn't orphan or the left child iterates back to the parent */
					while (min->parent_node && rbt_is_right_chlid(min))
						min = min->parent_node;
					min = min->parent_node;
				}
			}
		}

		/* Apply "action" to all the nodes of brt in orther left to right */
		template<typename Action>
		void								rbt_in_order_walk(Action action) const {
			rbt_iter_in_orther_walk(root, action);
		}

		/* Apply "action" to btr starting in "node" and iterating over the children
			all the next generations recursivelly */
		template<typename Action>
		void								rbt_recursive_in_order_walk(Node *node, Action action) const{
			if (node != nil)
			{
				rbt_recursive_in_order_walk(node->child_left, action);
				action(node->key, node->data);
				rbt_recursive_in_order_walk(node->child_right, action);
			}
		}

		/* Apply "action" to all the elements of the btr using a stack in letf-rigt order */
		template<typename Action>
		void								rbt_stack_in_order_walk(Node *node, Action action) const
		{
			stack<Node *> stck;

			stck.push(NULL);
			stck.push(NULL);

			while (!stck.empty())
			{
				/* stack the nodes starting from the right (reversed order) */
				while (node != nil)
				{
					stck.push(node->child_right);
					stck.push(node);
					node = node->child_left;
				}
				/* action to the fisrt node in stack (the last node so the left one) */
				if (stck.top())
					action (stck.top()->key,stck.top()->data);
				
				/* removes the node who tooks an action */
				stck.pop();
				/* iterates and clear */
				node = stck.top();
				stck.pop();
			}
		}

		/* Deletes the target_node updating the colors and returns a pointer to the root */
		template<typename Child1, typename Child2>
		void							rbt_generic_fixed_delete(Node **target_node, Child1 ch1, Child2 ch2) {
			/* "aux" is the brother of "target_node" */
			Node *aux = ch2((*target_node)->parent_node);
			if (aux->color & RED)
			{
				/* if the brother is red it swap his color with his parent, rotate by the parent
					and "aux" (the rotated old brother) becomes his brother */
				std::swap(aux->color, (*target_node)->parent_node->color);
				rbt_generic_rotatation((*target_node)->parent_node, ch1, ch2);
				aux = ch2((*target_node)->parent_node);
			}
			
			/* if "aux" childrens are black, "aux" should be red and "target" become his parent */
			if (aux->child_left->color == BLACK && aux->child_right->color & BLACK)
			{
				aux->color |= RED;
				*target_node = (*target_node)->parent_node;
			}
			else /* if almost 1 child is red */
			{
				/* if "ch2" (function pointer that returns a child) return a black child
					the color will the swaped with his brother color, and then aux will be rotated */
				if (ch2(aux)->color & BLACK)
				{
					std::swap(aux->color, ch1(aux)->color)
					aux = rbt_generic_rotatation(aux, ch2, ch1);
				}
				/* nodes color are updated nad then rotated */
				aux->color = node->parent_node->color;
				(*target_node)->parent_node &= BLACK;
				ch2(aux)->color &= BLACK;
				rbt_generic_rotatation((*target_node)->parent_node, ch1, ch2);
				*target_node = root;
			}
		}

		/* Removes the target using the deltion algorithm */
		void							rbt_fixed_remove(Node *target_node) {
			while (target_node != root && target_node->color == BLACK)
			{
				if (rbt_is_left_child(target_node))
					rbt_generic_fixed_delete(&target_node, rbt_node_left_child, rbt_node_right_child);
				else
					rbt_generic_fixed_delete(&target_node, rbt_node_right_child, rbt_node_left_child);
			}
			target_node->color &= BLACK;
		}

		/* Deletes a node using the rbt deletion algorithm */
		void								rbt_delete_node(Node *target_node) {
			Node *aux = target_node->child_right;
			Node *xua = target_node;
			bool color_remember = target_node->color;

			if (target_node->child_left == nil)
				rbt_move(target_node, aux);
			else if (target_node->child_right == nil)
				rbt_move(target_node, aux = aux->child_left);
			else
			{
				xua = rtb_minimum(target_node->child_right);
				color_remember = xua->color;
				aux = xua->child_right;
				if (xua->parent_node == target_node)
					aux->parent_node = xua;
				else
				{
					rbt_move(xua, xua->child_right);
					xua->child_right = target_node->child_right;
					xua->child_right->parent_node = xua;
				}
				rbt_move(target_node, xua);
				xua->child_left = target_node->child_left;
				xua->child_left->parent_node = aux;
				xua->color = target_node->color;
			}
			if (color_remember == BLACK)
				rbt_fixed_remove(aux);
			size--;
			node_mem(memory).destroy(target_node);
			node_mem(memory).deallocate(target_node, 1);
		}

		/* Search using the key, if found something it ll be removed and "data" takes
			a reference to found->data. Returns true on succes.s */
		bool							rbt_remove(const key_type &key, mapped_type &data)
		{
			Node *node = rbt_find_node(key);
			if (bool tmp = (node != nil))
			{
				data = node->data;
				bst_delete_node_v2(node);
			}
			return (tmp);
		}

		/* Deletes a node using the rbt deletion algorithm */
		void							rbt_delete_node_v2(Node *target_node)
		{
			Node *aux = target_node->child_right;
			Node *xua = target_node;

			if (target_node->child_left == nil)
				rbt_move(target_node, aux);
			else if (target_node->child_right == nil)
				rbt_move(xua, aux);
			else
			{
				xua = rtb_minimum(target_node->child_right);
				aux = xua->child_right;
				target_node->data = std::move(xua->data);
				target_node->key = std::move(xua->key);
				rbt_move(xua, aux);
			}
			if (xau->color == BLACK)
				rbt_fixed_remove(aux);
			total_size--;
			node_mem(memory).destroy(xua);
			node_mem(memory).deallocate(xua, 1);
		}

		public:

		/* Class default methods */

		map(const key_compare cmp = key_compare()) : comp(cmp), total_size(0), memory(allocator_type()) {
			nil = create_node(key_compare(), mapped_type());
			root = nil;
		}
		template<typename InputIt>
		map(InputIt first, InputIt last, const key_compare &comp = key_compare()) : comp(cmp), total_size(0), memory(allocator_type()) {
			nil = create_node(key_compare(), mapped_type());
			root = nil;
			// have to insert nodes in wald order using the iterators
			// starts in first, ends in last
		}
		map(const map &other) : comp(cmp), total_size(0), memory(allocator_type()) {
			*this = other;
		}
		~map() {
			rbt_destroy(root);
		}

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
