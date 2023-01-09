/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limit_except.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:48:37 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 16:49:51 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "context.hpp"

int	parse_limit_except(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::vector<std::string>	http_methods;

	// Get http_methods
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		// Check method validity
		if (*it != "GET" \
		&&	*it != "POST" \
		&&	*it != "DELETE")
			return (invalid_method_error(directive, l, *it));
		if (!((*it).empty()) && std::find(http_methods.begin(), http_methods.end(), *it) == http_methods.end())
			http_methods.push_back(*it);
	}

	if (context.first == HTTP)
	{
		Http	&current_http = get_context<Http>(context);

		for (std::vector<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
		{
			if (std::find(current_http.limit_except.begin(), current_http.limit_except.end(), *it) == current_http.limit_except.end())
				current_http.limit_except.push_back(*it);
		}
		get_context<Http>(context).set_flag_limit_except(true);
	}
	else if (context.first == SERVER)
	{
		Server	&current_server = get_context<Server>(context);

		for (std::vector<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
		{
			if (std::find(current_server.limit_except.begin(), current_server.limit_except.end(), *it) == current_server.limit_except.end())
				current_server.limit_except.push_back(*it);
		}
		get_context<Server>(context).set_flag_limit_except(true);
	}
	else
	{
		Location	&current_location = get_context<Location>(context);

		for (std::vector<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
		{
			if (std::find(current_location.limit_except.begin(), current_location.limit_except.end(), *it) == current_location.limit_except.end())
				current_location.limit_except.push_back(*it);
		}
		get_context<Location>(context).set_flag_limit_except(true);
	}
	return (0);
}
