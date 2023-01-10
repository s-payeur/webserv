/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   root.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:52:14 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/10 10:48:19 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <string>
#include <limits.h>
#include <unistd.h>
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "parsing.hpp"

int	parse_root(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	// Construct path
	std::string			root;
	std::string			path;
	const char			*login = getlogin();
	const std::string	username = !(geteuid()) ? "root" : !(login) ? "root" : login;
	const std::string	working_directory = "/mnt/nfs/homes/" + username + "/goinfre/webserv/html";

	if (args[0][0] == '/')
		path = args[0];
	else
		path = working_directory + '/' + args[0];

	// Normalize path
	root = normalize_path(directive, l, path, true, true);
	if (root.empty())
		return (-1);
	if (root.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, root.substr(0, 10) + "..."));

	if (context.first == HTTP)
	{
		get_context<Http>(context).root = root;
		get_context<Http>(context).set_flag_root(true);
	}
	else if (context.first == SERVER)
	{
		get_context<Server>(context).root = root;
		get_context<Server>(context).set_flag_root(true);
	}
	else
	{
		get_context<Location>(context).root = root;
		get_context<Location>(context).set_flag_root(true);
	}
	return (0);
}
