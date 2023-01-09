/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_parsing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:32:46 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 16:06:46 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"

static void	undiff(const Http &http, Server &server)
{
	if (http.get_flag_error_page() && !(server.get_flag_error_page()))
	{
		server.error_page = http.error_page;
		server.set_flag_error_page(true);
	}
	if (http.get_flag_client_max_body_size() && !(server.get_flag_client_max_body_size()))
	{
		server.client_max_body_size = http.client_max_body_size;
		server.set_flag_client_max_body_size(true);
	}
	if (http.get_flag_limit_except() && !(server.get_flag_limit_except()))
	{
		server.limit_except = http.limit_except;
		server.set_flag_limit_except(true);
	}
	if (http.get_flag_root() && !(server.get_flag_root()))
	{
		server.root = http.root;
		server.set_flag_root(true);
	}
	if (http.get_flag_autoindex() && !(server.get_flag_autoindex()))
	{
		server.autoindex = http.autoindex;
		server.set_flag_autoindex(true);
	}
	if (http.get_flag_index() && !(server.get_flag_index()))
	{
		server.index = http.index;
		server.set_flag_index(true);
	}
}

static void	undiff(const Server &server, Location &location)
{
	if (server.get_flag_error_page() && !(location.get_flag_error_page()))
	{
		location.error_page = server.error_page;
		location.set_flag_error_page(true);
	}
	if (server.get_flag_client_max_body_size() && !(location.get_flag_client_max_body_size()))
	{
		location.client_max_body_size = server.client_max_body_size;
		location.set_flag_client_max_body_size(true);
	}
	if (server.get_flag_limit_except() && !(location.get_flag_limit_except()))
	{
		location.limit_except = server.limit_except;
		location.set_flag_limit_except(true);
	}
	if (server.get_flag_return_() && !(location.get_flag_return_()))
	{
		location.return_ = server.return_;
		location.set_flag_return_(true);
	}
	if (server.get_flag_root() && !(location.get_flag_root()))
	{
		location.root = server.root;
		location.set_flag_root(true);
	}
	if (server.get_flag_autoindex() && !(location.get_flag_autoindex()))
	{
		location.autoindex = server.autoindex;
		location.set_flag_autoindex(true);
	}
	if (server.get_flag_index() && !(location.get_flag_index()))
	{
		location.index = server.index;
		location.set_flag_index(true);
	}
}

static void	undiff(const Location &location, Location &sublocation)
{
	if (location.get_flag_error_page() && !(sublocation.get_flag_error_page()))
	{
		sublocation.error_page = location.error_page;
		sublocation.set_flag_error_page(true);
	}
	if (location.get_flag_client_max_body_size() && !(sublocation.get_flag_client_max_body_size()))
	{
		sublocation.client_max_body_size = location.client_max_body_size;
		sublocation.set_flag_client_max_body_size(true);
	}
	if (location.get_flag_limit_except() && !(sublocation.get_flag_limit_except()))
	{
		sublocation.limit_except = location.limit_except;
		sublocation.set_flag_limit_except(true);
	}
	if (location.get_flag_return_() && !(sublocation.get_flag_return_()))
	{
		sublocation.return_ = location.return_;
		sublocation.set_flag_return_(true);
	}
	if (location.get_flag_root() && !(sublocation.get_flag_root()))
	{
		sublocation.root = location.root;
		sublocation.set_flag_root(true);
	}
	if (location.get_flag_autoindex() && !(sublocation.get_flag_autoindex()))
	{
		sublocation.autoindex = location.autoindex;
		sublocation.set_flag_autoindex(true);
	}
	if (location.get_flag_index() && !(sublocation.get_flag_index()))
	{
		sublocation.index = location.index;
		sublocation.set_flag_index(true);
	}
	if (location.get_flag_cgi() && !(sublocation.get_flag_cgi()))
	{
		sublocation.cgi = location.cgi;
		sublocation.set_flag_cgi(true);
	}
}

static void	fix_parsing_location(Location &location)
{
	for (std::vector<Location>::iterator sublocation_it = location.location.begin(); sublocation_it != location.location.end(); sublocation_it++)
	{
		undiff(location, *sublocation_it);
		fix_parsing_location(*sublocation_it);
	}
}

void	fix_parsing(Http &http)
{
	for (std::vector<Server>::iterator server_it = http.server.begin(); server_it != http.server.end(); server_it++)
	{
		undiff(http, *server_it);
		for (std::vector<Location>::iterator location_it = (*server_it).location.begin(); location_it != (*server_it).location.end(); location_it++)
		{
			undiff(*server_it, *location_it);
			fix_parsing_location(*location_it);
		}
	}
}
