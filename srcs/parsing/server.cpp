/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:31:50 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 16:32:56 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>
#include <utility>
#include "Http.hpp"
#include "context.hpp"

void	parse_server(std::stack< std::pair<e_context, void *> > &contexts)
{
	Http	&http = get_context<Http>(contexts.top());

	http.server.push_back(Server(http));
	contexts.push(std::pair<e_context, void *>(SERVER, &(http.server.back())));
}
