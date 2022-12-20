/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2022/12/12 04:20:30 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "Location"
#include "Server.hpp"

// Constructor
Server::Server(void)
{
	_host = "0.0.0.0";
	_port = !(geteuid()) ? 80 : 8000;
	_server_name = std::string();
	_error_page = std::map<int, std::string>();
	_client_max_body_size = 1048576; // equals 1 * 1024 * 1024 or 1MiB (1m)
	_location = std::vector<Location>();
	_limit_except = std::vector<std::string>();
	_return = std::pair<int, std::string> (0, std::string());
	_root = "../html";
	_autoindex = false;
	_index = std::vector<std::string>();
}

// Setters
void	Server::setHost(std::string host)
{
	this->_host = host;
}

// Getters
const std::string	Server::getHost(void) const
{
	return (this->_host);
}
