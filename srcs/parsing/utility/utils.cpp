/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:34:44 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/10 11:27:44 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <set>
#include <string>
#include <cstring>
#include <limits.h>
#include <unistd.h>
#include "parsing.hpp"

static std::string	resolve_symbolic_link(std::string path)
{
	char		buffer[PATH_MAX];
	std::string	link_target;

	std::memset(buffer, 0, sizeof(char) * PATH_MAX);
	link_target = readlink(path.c_str(), buffer, PATH_MAX) < 0 ? std::string() : std::string(buffer);
	if (link_target.empty())
		return (path);
	if (link_target[0] == '/')
		return (link_target);
	else
		return (path.substr(0, path.rfind('/') + 1) + link_target);
}

std::string	normalize_path(const std::string &directive, const size_t l, std::string path, bool absolute_path, bool symbolic_link_resolution, std::set<std::string> link_components/* = std::set<std::string>()*/)
{
	std::vector<std::string>	components;
	std::vector<std::string>	normalized_components;
	std::string					resolved_component;
	std::string::size_type		start;
	std::string::size_type		end;

	// Split path into multiple components using '/' as delimiter
	start = 0;
	end = path.find('/');
	while (end != std::string::npos)
	{
		if (start != end)
			components.push_back(path.substr(start, end - start));
		start = end + 1;
		end = path.find('/', start);
	}
	if (start < path.size() && start != end)
		components.push_back(path.substr(start));

	for (std::vector<std::string>::iterator it = components.begin(); it != components.end(); it++)
	{
		if (*it == ".")
			continue ;
		else if (*it == "..")
		{
			if (absolute_path)
			{
				// Remove the previous component
				if (!(normalized_components.empty()))
					normalized_components.pop_back();
			}
			else
			{
				// Remove the previous component
				if (!(normalized_components.empty()) && normalized_components.back() != "..")
					normalized_components.pop_back();
				else
					normalized_components.push_back(*it);
			}
		}
		else
		{
			normalized_components.push_back(*it);
			if (symbolic_link_resolution)
			{
				// Construct the path to the file
				path.clear();
				for (std::vector<std::string>::const_iterator c = normalized_components.begin(); c != normalized_components.end(); c++)
					path += '/' + *c;
				resolved_component = resolve_symbolic_link(path);
				if (path != resolved_component)
				{
					// Check if the components has already been encountered
					if (link_components.count(path))
						return (symbolic_link_loop_error(directive, l, path));
					link_components.insert(path);
					return (normalize_path(directive, l, resolved_component, absolute_path, symbolic_link_resolution, link_components));
				}
			}
		}
	}

	// Construct the path
	path.clear();
	for (std::vector<std::string>::const_iterator it = normalized_components.begin(); it != normalized_components.end(); it++)
		path += '/' + *it;
	return (path.empty() ? "/" : path);
}
