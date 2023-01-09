/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:54:14 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 16:54:48 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <string>
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "context.hpp"

int	parse_autoindex(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::string	value;
	bool		autoindex;

	value = args[0];
	for (std::string::iterator it = value.begin(); it != value.end(); it++)
		*it = std::tolower(*it);
	if (value == "on")
		autoindex = true;
	else if (value == "off")
		autoindex = false;
	else
		return (invalid_value_error(directive, l, value));

	if (context.first == HTTP)
	{
		get_context<Http>(context).autoindex = autoindex;
		get_context<Http>(context).set_flag_autoindex(true);
	}
	else if (context.first == SERVER)
	{
		get_context<Server>(context).autoindex = autoindex;
		get_context<Server>(context).set_flag_autoindex(true);
	}
	else
	{
		get_context<Location>(context).autoindex = autoindex;
		get_context<Location>(context).set_flag_autoindex(true);
	}
	return (0);
}
