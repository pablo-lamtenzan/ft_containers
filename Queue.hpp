/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Queue.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 20:11:34 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/27 20:24:25 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "List.hpp"
#include "Vector.hpp"

/* std::queue is a container adapter, thas mean it gives the functionality of a stack
	of other containers */

namespace ft {
	
	template<typename T, class Container>
	class queue
	{
		public:

		typedef	Container		container_type;
		typedef T				value_type;
		typedef const T			const_value_type;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef std::size_t		size_of_type;

		protected:
		container_type			container;
		
		public:
		/* default class methods */
		queue(const container_type &c) { container = c; }
		~queue() {}

		/* Class Member methods */
		// op =
		
		/* Element access */
		reference			front() { return (container.front()); }
		const_reference		front() const { return (container.front()); }
		const_reference		back() const { return (container.back()); }
	
		/* Capacity */
		bool				empty() const { return (container.empty())); }
		size_of_type		size() const { return (container.size()); }

		/* Modifiers */
		void				push(const value_type &value) { container.push_back(value); }
		void				pop() { container.pop_front(); }
		void				swap(queue<T, Container> &other) {
			std::swap(container, other.container);
		}
		/* Non-member methods */
		bool				operator==(const queue<T, Container> &lhs, const queue<T, Container> &rhs) { return (lhs.container == rhs.container); }
		bool				operator!=(const queue<T, Container> &lhs, const queue<T, Container> &rhs) { return (lhs.container != rhs.container); }
		bool				operator<(const queue<T, Container> &lhs, const queue<T, Container> &rhs) { return (lhs.container < rhs.container); }
		bool				operator<=(const queue<T, Container> &lhs, const queue<T, Container> &rhs) { return (lhs.container <= rhs.container); }
		bool				operator>(const queue<T, Container> &lhs, const queue<T, Container> &rhs) { return (lhs.container > rhs.container); }
		bool				operator>=(const queue<T, Container> &lhs, const queuequeue<T, Container> &rhs) { return (lhs.container >= rhs.container); }
	};
}