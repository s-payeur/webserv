/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:57:15 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 16:58:06 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <set>
#include <utility>
#include <string>
#include <limits.h>
#include "Location.hpp"
#include "context.hpp"

int	parse_cgi(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	const std::set<std::string>						extensions(args.begin(), args.end() - 1);
	std::string										cgi_path;
	std::map<std::string ,std::string>				cgi;
	std::map<std::string, std::string>::iterator	pos;

	for (std::set<std::string>::const_iterator it = extensions.begin(); it != extensions.end(); it++)
	{
		// Check extensions validity
		if ((*it)[0] != '.' || (*it).find('.', 1) != std::string::npos)
				return (invalid_value_error(directive, l, *it));
	}

	// Normalize path
	cgi_path = normalize_path(directive, l, std::string(args[args.size() - 1]), false, true);
	if (cgi_path.empty())
		return (-1);
	if (cgi_path.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, cgi_path.substr(0, 10) + "..."));

	for (std::set<std::string>::const_iterator it = extensions.begin(); it != extensions.end(); it++)
		cgi.insert(std::pair<std::string, std::string>(*it, cgi_path));

	// Insert or assign
	Location	&current_location = get_context<Location>(context);
	for (std::map<std::string, std::string>::iterator it = cgi.begin(); it != cgi.end(); it++)
	{
		pos = current_location.cgi.find(it->first);
		if (pos == current_location.cgi.end())
			current_location.cgi.insert(*it);
		else
			pos->second = it->second;
	}

	get_context<Location>(context).set_flag_cgi(true);
	return (0);
}
