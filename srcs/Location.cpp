/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/06 17:25:26 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "Server.hpp"
#include "Location.hpp"

// Constructor
Location::Location(const Server &server)
{
	error_page = server.error_page;
	client_max_body_size = server.client_max_body_size;
	location = std::vector<Location>();
	location_path = std::vector<std::string>();
	limit_except = server.limit_except;
	return_ = server.return_;
	root = server.root;
	autoindex = server.autoindex;
	index = server.index;
	cgi = std::map<std::string, std::string>();
}

Location::Location(const Location &inherited_location)
{
	error_page = inherited_location.error_page;
	client_max_body_size = inherited_location.client_max_body_size;
	location = std::vector<Location>();
	location_path = std::vector<std::string>();
	limit_except = inherited_location.limit_except;
	return_ = inherited_location.return_;
	root = inherited_location.root;
	autoindex = inherited_location.autoindex;
	index = inherited_location.index;
	cgi = inherited_location.cgi;
}
