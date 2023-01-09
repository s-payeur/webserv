/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:55:24 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 16:56:37 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <limits.h>
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "context.hpp"

int	parse_index(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::vector<std::string>	pages;
	std::string					page;

	// Get pages
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		// Normalize page
		page = normalize_path(directive, l, *it, false, true);
		if (page.empty())
			return (-1);
		if (page.size() >= PATH_MAX)
			return (too_long_path_after_resolution_error(directive, l, page.substr(0, 10) + "..."));
		if (!(page.empty()) && std::find(pages.begin(), pages.end(), page) == pages.end())
			pages.push_back(page);
	}

	if (context.first == HTTP)
	{
		Http	&current_http = get_context<Http>(context);

		for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
		{
			if (std::find(current_http.index.begin(), current_http.index.end(), *it) == current_http.index.end())
				current_http.index.push_back(*it);
		}
		get_context<Http>(context).set_flag_index(true);
	}
	else if (context.first == SERVER)
	{
		Server	&current_server = get_context<Server>(context);

		for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
		{
			if (std::find(current_server.index.begin(), current_server.index.end(), *it) == current_server.index.end())
				current_server.index.push_back(*it);
		}
		get_context<Server>(context).set_flag_index(true);
	}
	else
	{
		Location	&current_location = get_context<Location>(context);

		for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
		{
			if (std::find(current_location.index.begin(), current_location.index.end(), *it) == current_location.index.end())
				current_location.index.push_back(*it);
		}
		get_context<Location>(context).set_flag_index(true);
	}
	return (0);
}
