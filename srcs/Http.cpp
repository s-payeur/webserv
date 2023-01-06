/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/06 13:03:45 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "Http.hpp"
#include "Server.hpp"

// Constructor
Http::Http(void)
{
	const char			*login = getlogin();
	const std::string	username = !(geteuid()) ? "root" : !(login) ? "root" : login;

	_flag_error_page = false;
	_flag_client_max_body_size = false;
	_flag_server = false;
	_flag_limit_except = false;
	_flag_root = false;
	_flag_autoindex = false;
	_flag_index = false;

	error_page = std::map<int, std::string>();
	client_max_body_size = 1048576; // equals 1 * 1024 * 1024 or 1MiB (1m)
	server = std::vector<Server>();
	limit_except = std::vector<std::string>();
	root = "/mnt/nfs/homes/" + username + "/goinfre/webserv/html";
	autoindex = false;
	index = std::vector<std::string>();
}

// Setters
void	Http::set_flag_error_page(bool value)
{
	this->_flag_error_page = value;
}

void	Http::set_flag_client_max_body_size(bool value)
{
	this->_flag_client_max_body_size = value;
}

void	Http::set_flag_server(bool value)
{
	this->_flag_server = value;
}

void	Http::set_flag_limit_except(bool value)
{
	this->_flag_limit_except = value;
}

void	Http::set_flag_root(bool value)
{
	this->_flag_root = value;
}

void	Http::set_flag_autoindex(bool value)
{
	this->_flag_autoindex = value;
}

void	Http::set_flag_index(bool value)
{
	this->_flag_index = value;
}

// Getters
const bool	&Http::get_flag_error_page(void) const
{
	return (this->_flag_error_page);
}

const bool	&Http::get_flag_client_max_body_size(void) const
{
	return (this->_flag_client_max_body_size);
}

const bool	&Http::get_flag_server(void) const
{
	return (this->_flag_server);
}

const bool	&Http::get_flag_limit_except(void) const
{
	return (this->_flag_limit_except);
}

const bool	&Http::get_flag_root(void) const
{
	return (this->_flag_root);
}

const bool	&Http::get_flag_autoindex(void) const
{
	return (this->_flag_autoindex);
}

const bool	&Http::get_flag_index(void) const
{
	return (this->_flag_index);
}
