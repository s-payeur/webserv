/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/06 17:25:17 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"

// Constructor
Server::Server(const Http &http)
{
	listen = std::pair<std::string, std::string>("0.0.0.0", (!(geteuid()) ? "80" : "8000"));
	server_name = std::string();
	error_page = http.error_page;
	client_max_body_size = http.client_max_body_size;
	location = std::vector<Location>();
	location_path = std::vector<std::string>();
	limit_except = http.limit_except;
	return_ = std::pair<int, std::string>(0, std::string());
	root = http.root;
	autoindex = http.autoindex;
	index = http.index;
}
