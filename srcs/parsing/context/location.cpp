/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:34:26 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/10 10:49:01 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>
#include <utility>
#include <limits.h>
#include "Http.hpp"
#include "Server.hpp"
#include "parsing.hpp"

int	parse_location(std::stack< std::pair<e_context, void *> > &contexts, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::string	uri;

	// Normalize path
	uri = normalize_path(directive, l, std::string(args[0]), true, true);
	if (uri.empty())
		return (-1);
	if (uri.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, uri.substr(0, 10) + "..."));

	if (contexts.top().first == SERVER)
	{
		Server	&server = get_context<Server>(contexts.top());

		for (std::vector<Location>::const_iterator it = server.location.begin(); it != server.location.end(); it++)
		{
			if (uri == (*it).uri)
				return (duplicate_location_error(directive, l, args[0], uri));
		}
		server.location.push_back(Location(server, uri));
		contexts.push(std::pair<e_context, void *>(LOCATION, &(server.location.back())));
	}
	else
	{
		Location	&location = get_context<Location>(contexts.top());

		if ((uri != location.uri) && (uri.find(location.uri) != 0 \
		|| uri.size() <= location.uri.size() || (location.uri != "/" && uri[location.uri.size()] != '/')))
			return (location_is_outside_location_error(directive, l, args[0], uri, location.uri));
		for (std::vector<Location>::const_iterator it = location.location.begin(); it != location.location.end(); it++)
		{
			if (uri == (*it).uri)
				return (duplicate_location_error(directive, l, args[0], uri));
		}
		location.location.push_back(Location(location, uri));
		contexts.push(std::pair<e_context, void *>(LOCATION, &(location.location.back())));
	}
	return (0);
}
