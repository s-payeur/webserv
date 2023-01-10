/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:21:22 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/10 13:01:47 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include "Http.hpp"
#include "parsing.hpp"

// TMP /////////////////////////////////////////////////////////////////////////
#include <vector>
#include <map>

static void	print_http(const Http &http)
{
	std::cout << "http {" << std::endl;
	if (http.get_flag_error_page())
	{
		for (std::map<int, std::string>::const_iterator it = http.error_page.begin(); it != http.error_page.end(); it++)
		{
			std::cout << "\terror_page ";
			std::cout << it->first << " " << it->second << ";" << std::endl;
		}
	}
	if (http.get_flag_client_max_body_size())
	{
		std::cout << "\tclient_max_body_size " << http.client_max_body_size << ";" << std::endl;
	}
	if (http.get_flag_limit_except())
	{
		std::cout << "\tlimit_except";
		for (std::vector<std::string>::const_iterator it = http.limit_except.begin(); it != http.limit_except.end(); it++)
		{
			std::cout << " " << *it;
		}
		std::cout << ";" << std::endl;
	}
	if (http.get_flag_root())
	{
		std::cout << "\troot " << http.root << ";" << std::endl;
	}
	if (http.get_flag_autoindex())
	{
		std::cout << "\tautoindex " << (http.autoindex ? "on" : "off") << ";" << std::endl;
	}
	if (http.get_flag_index())
	{
		std::cout << "\tindex";
		for (std::vector<std::string>::const_iterator it = http.index.begin(); it != http.index.end(); it++)
		{
			std::cout << " " << *it;
		}
		std::cout << ";" << std::endl;
	}
}

static void	print_server(const Server &server)
{
	std::cout << "\tserver {" << std::endl;
	if (server.get_flag_listen())
	{
		// A CHANGER
		std::cout << "\t\tlisten " << server.listen.first << ":" << server.listen.second << ";" << std::endl;
	}
	if (server.get_flag_server_name())
	{
		// A CHANGER
		std::cout << "\t\tserver_name " << server.server_name << ";" << std::endl;
	}
	if (server.get_flag_error_page())
	{
		for (std::map<int, std::string>::const_iterator it = server.error_page.begin(); it != server.error_page.end(); it++)
		{
			std::cout << "\t\terror_page ";
			std::cout << it->first << " " << it->second << ";" << std::endl;
		}
	}
	if (server.get_flag_client_max_body_size())
	{
		std::cout << "\t\tclient_max_body_size " << server.client_max_body_size << ";" << std::endl;
	}
	if (server.get_flag_limit_except())
	{
		std::cout << "\t\tlimit_except";
		for (std::vector<std::string>::const_iterator it = server.limit_except.begin(); it != server.limit_except.end(); it++)
		{
			std::cout << " " << *it;
		}
		std::cout << ";" << std::endl;
	}
	if (server.get_flag_return_())
	{
		std::cout << "\t\treturn " << server.return_.first << " " << server.return_.second << ";" << std::endl;
	}
	if (server.get_flag_root())
	{
		std::cout << "\t\troot " << server.root << ";" << std::endl;
	}
	if (server.get_flag_autoindex())
	{
		std::cout << "\t\tautoindex " << (server.autoindex ? "on" : "off") << ";" << std::endl;
	}
	if (server.get_flag_index())
	{
		std::cout << "\t\tindex";
		for (std::vector<std::string>::const_iterator it = server.index.begin(); it != server.index.end(); it++)
		{
			std::cout << " " << *it;
		}
		std::cout << ";" << std::endl;
	}
}

static void tabs_location(const unsigned int t)
{
	for (unsigned int i = 0; i < t; i++)
		std::cout << "\t";
}

static void	print_location(const Location &location, unsigned int t)
{
	tabs_location(t - 1);
	std::cout << "location " << location.uri << " {" << std::endl;
	if (location.get_flag_error_page())
	{
		for (std::map<int, std::string>::const_iterator it = location.error_page.begin(); it != location.error_page.end(); it++)
		{
			tabs_location(t);
			std::cout << "error_page ";
			std::cout << it->first << " " << it->second << ";" << std::endl;
		}
	}
	if (location.get_flag_client_max_body_size())
	{
		tabs_location(t);
		std::cout << "client_max_body_size " << location.client_max_body_size << ";" << std::endl;
	}
	if (location.get_flag_limit_except())
	{
		tabs_location(t);
		std::cout << "limit_except";
		for (std::vector<std::string>::const_iterator it = location.limit_except.begin(); it != location.limit_except.end(); it++)
		{
			std::cout << " " << *it;
		}
		std::cout << ";" << std::endl;
	}
	if (location.get_flag_return_())
	{
		tabs_location(t);
		std::cout << "return " << location.return_.first << " " << location.return_.second << ";" << std::endl;
	}
	if (location.get_flag_root())
	{
		tabs_location(t);
		std::cout << "root " << location.root << ";" << std::endl;
	}
	if (location.get_flag_autoindex())
	{
		tabs_location(t);
		std::cout << "autoindex " << (location.autoindex ? "on" : "off") << ";" << std::endl;
	}
	if (location.get_flag_index())
	{
		tabs_location(t);
		std::cout << "index";
		for (std::vector<std::string>::const_iterator it = location.index.begin(); it != location.index.end(); it++)
		{
			std::cout << " " << *it;
		}
		std::cout << ";" << std::endl;
	}
	if (location.get_flag_cgi())
	{
		for (std::map<std::string, std::string>::const_iterator it = location.cgi.begin(); it != location.cgi.end(); it++)
		{
			tabs_location(t);
			std::cout << "cgi ";
			std::cout << it->first << " " << it->second << ";" << std::endl;
		}
	}
}

static void	print_location_recursively(Location &location)
{
	static unsigned int	t = 3;

	print_location(location, t);
	for (std::vector<Location>::iterator sublocation_it = location.location.begin(); sublocation_it != location.location.end(); sublocation_it++)
	{
		t++;
		print_location_recursively(*sublocation_it);
	}
	tabs_location(t - 1);
	std::cout << "}" << std::endl;
	if (t > 3)
		t--;
}

void	print(Http &http)
{
	print_http(http);
	for (std::vector<Server>::iterator server_it = http.server.begin(); server_it != http.server.end(); server_it++)
	{
		print_server(*server_it);
		for (std::vector<Location>::iterator location_it = (*server_it).location.begin(); location_it != (*server_it).location.end(); location_it++)
		{
			print_location_recursively(*location_it);
		}
		std::cout << "\t}" << std::endl;
	}
	std::cout << "}" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////

int	main(int argc, char **argv)
{
	Http		http;
	std::string	configuration_file;

	if (argc > 2)
	{
		std::cerr << "webserv: too many arguments" << std::endl;
		return (1);
	}
	if (argc == 1)
		configuration_file = "conf/default.conf";
	else
		configuration_file = argv[1];
	if (parse(http, configuration_file) < 0)
		return (1);
	print(http);
	return (0);
}
