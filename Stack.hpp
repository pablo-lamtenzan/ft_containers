/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 19:37:10 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/27 20:08:38 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "List.hpp"
#include "Vector.hpp"

/* std::stack is a container adapter, thas mean it gives the functionality of a stack
	of other containers */

namespace ft {
	
	template<typename T, class Container>
	class stack
	{
		public:

		typedef	Container		container_type;
		typedef T				value_type;
		typedef const T			const_value_type;
		typedef std::size_t		size_of_type;

		protected:
		container_type			container;
		
		public:
		/* default class methods */
		stack(const container_type &c) { container = c; }
		~stack() {}

		/* Class Member methods */
		// op =
		
		/* Element access */
		value_type			top() { return (container.back()); }
		const_value_type	top() const { return (container.back()); }
	
		/* Capacity */
		bool				empty() const { return (container.empty())); }
		size_of_type		size() const { return (container.size()); }

		/* Modifiers */
		void				push(const value_type &value) { container.push_back(value); }
		void				pop() { container.pop_back(); }
		void				swap(stack<T, Container> &other) {
			std::swap(container, other.container);
		}
		/* Non-member methods */
		bool				operator==(const stack<T, Container> &lhs, const stack<T, Container> &rhs) { return (lhs.container == rhs.container); }
		bool				operator!=(const stack<T, Container> &lhs, const stack<T, Container> &rhs) { return (lhs.container != rhs.container); }
		bool				operator<(const stack<T, Container> &lhs, const stack<T, Container> &rhs) { return (lhs.container < rhs.container); }
		bool				operator<=(const stack<T, Container> &lhs, const stack<T, Container> &rhs) { return (lhs.container <= rhs.container); }
		bool				operator>(const stack<T, Container> &lhs, const stack<T, Container> &rhs) { return (lhs.container > rhs.container); }
		bool				operator>=(const stack<T, Container> &lhs, const stack<T, Container> &rhs) { return (lhs.container >= rhs.container); }
	};
}