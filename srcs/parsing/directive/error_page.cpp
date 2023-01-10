/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_page.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:43:42 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/10 10:48:18 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <set>
#include <utility>
#include <string>
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "parsing.hpp"

int	parse_error_page(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const std::string::size_type l)
{
	const std::set<std::string>	codes(args.begin(), args.end() - 1);
	const std::string			page = args[args.size() - 1];
	std::map<int, std::string>	error_page;

	for (std::set<std::string>::const_iterator it = codes.begin(); it != codes.end(); it++)
	{
		// Check error codes validity
		if ((*it).find_first_not_of("0123456789") != std::string::npos)
				return (invalid_value_error(directive, l, *it));
		// Check error codes limit
		if (std::atoi((*it).c_str()) < 300 || std::atoi((*it).c_str()) > 599)
			return (value_must_be_between_error(directive, l, *it, 300, 599, ""));
		error_page.insert(std::pair<int, std::string>(std::atoi((*it).c_str()), page));
	}

	if (context.first == HTTP)
	{
		get_context<Http>(context).error_page.insert(error_page.begin(), error_page.end());
		get_context<Http>(context).set_flag_error_page(true);
	}
	else if (context.first == SERVER)
	{
		get_context<Server>(context).error_page.insert(error_page.begin(), error_page.end());
		get_context<Server>(context).set_flag_error_page(true);
	}
	else
	{
		get_context<Location>(context).error_page.insert(error_page.begin(), error_page.end());
		get_context<Location>(context).set_flag_error_page(true);
	}
	return (0);
}
