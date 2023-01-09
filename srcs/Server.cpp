/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 10:14:44 by spayeur          ###   ########.fr       */
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
	_flag_listen = false;
	_flag_server_name = false;
	_flag_error_page = false;
	_flag_client_max_body_size = false;
	_flag_location = false;
	_flag_limit_except = false;
	_flag_return_ = false;
	_flag_root = false;
	_flag_autoindex = false;
	_flag_index = false;

	listen = std::pair<std::string, std::string>("0.0.0.0", (!(geteuid()) ? "80" : "8000"));
	server_name = std::string();
	error_page = http.error_page;
	client_max_body_size = http.client_max_body_size;
	location = std::vector<Location>();
	limit_except = http.limit_except;
	return_ = std::pair<int, std::string>(0, std::string());
	root = http.root;
	autoindex = http.autoindex;
	index = http.index;
}

// Setters
void	Server::set_flag_listen(bool value)
{
	this->_flag_listen = value;
}

void	Server::set_flag_server_name(bool value)
{
	this->_flag_server_name = value;
}

void	Server::set_flag_error_page(bool value)
{
	this->_flag_error_page = value;
}

void	Server::set_flag_client_max_body_size(bool value)
{
	this->_flag_client_max_body_size = value;
}

void	Server::set_flag_location(bool value)
{
	this->_flag_location = value;
}

void	Server::set_flag_limit_except(bool value)
{
	this->_flag_limit_except = value;
}

void	Server::set_flag_return_(bool value)
{
	this->_flag_return_ = value;
}

void	Server::set_flag_root(bool value)
{
	this->_flag_root = value;
}

void	Server::set_flag_autoindex(bool value)
{
	this->_flag_autoindex = value;
}

void	Server::set_flag_index(bool value)
{
	this->_flag_index = value;
}

// Getters
const bool	&Server::get_flag_listen(void) const
{
	return (this->_flag_listen);
}

const bool	&Server::get_flag_server_name(void) const
{
	return (this->_flag_server_name);
}

const bool	&Server::get_flag_error_page(void) const
{
	return (this->_flag_error_page);
}

const bool	&Server::get_flag_client_max_body_size(void) const
{
	return (this->_flag_client_max_body_size);
}

const bool	&Server::get_flag_location(void) const
{
	return (this->_flag_location);
}

const bool	&Server::get_flag_limit_except(void) const
{
	return (this->_flag_limit_except);
}

const bool	&Server::get_flag_return_(void) const
{
	return (this->_flag_return_);
}

const bool	&Server::get_flag_root(void) const
{
	return (this->_flag_root);
}

const bool	&Server::get_flag_autoindex(void) const
{
	return (this->_flag_autoindex);
}

const bool	&Server::get_flag_index(void) const
{
	return (this->_flag_index);
}
