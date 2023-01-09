/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:34:44 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 16:03:19 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utility>
#include "context.hpp"

template <class T>
T	&get_context(std::pair<e_context, void *> &context)
{
	return (*(reinterpret_cast<T *>(context.second)));
}
