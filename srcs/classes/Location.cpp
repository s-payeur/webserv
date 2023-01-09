/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 10:16:22 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "Server.hpp"
#include "Location.hpp"

// Constructor
Location::Location(const Server &server, const std::string &value)
{
	_flag_error_page = false;
	_flag_client_max_body_size = false;
	_flag_location = false;
	_flag_limit_except = false;
	_flag_return_ = false;
	_flag_root = false;
	_flag_autoindex = false;
	_flag_index = false;
	_flag_cgi = false;

	uri = value;
	error_page = server.error_page;
	client_max_body_size = server.client_max_body_size;
	location = std::vector<Location>();
	limit_except = server.limit_except;
	return_ = server.return_;
	root = server.root;
	autoindex = server.autoindex;
	index = server.index;
	cgi = std::map<std::string, std::string>();
}

Location::Location(const Location &inherited_location, const std::string &value)
{
	_flag_error_page = false;
	_flag_client_max_body_size = false;
	_flag_location = false;
	_flag_limit_except = false;
	_flag_return_ = false;
	_flag_root = false;
	_flag_autoindex = false;
	_flag_index = false;
	_flag_cgi = false;

	uri = value;
	error_page = inherited_location.error_page;
	client_max_body_size = inherited_location.client_max_body_size;
	location = std::vector<Location>();
	limit_except = inherited_location.limit_except;
	return_ = inherited_location.return_;
	root = inherited_location.root;
	autoindex = inherited_location.autoindex;
	index = inherited_location.index;
	cgi = inherited_location.cgi;
}

// Setters
void	Location::set_flag_error_page(bool value)
{
	this->_flag_error_page = value;
}

void	Location::set_flag_client_max_body_size(bool value)
{
	this->_flag_client_max_body_size = value;
}

void	Location::set_flag_location(bool value)
{
	this->_flag_location = value;
}

void	Location::set_flag_limit_except(bool value)
{
	this->_flag_limit_except = value;
}

void	Location::set_flag_return_(bool value)
{
	this->_flag_return_ = value;
}

void	Location::set_flag_root(bool value)
{
	this->_flag_root = value;
}

void	Location::set_flag_autoindex(bool value)
{
	this->_flag_autoindex = value;
}

void	Location::set_flag_index(bool value)
{
	this->_flag_index = value;
}

void	Location::set_flag_cgi(bool value)
{
	this->_flag_cgi = value;
}

// Getters
const bool	&Location::get_flag_error_page(void) const
{
	return (this->_flag_error_page);
}

const bool	&Location::get_flag_client_max_body_size(void) const
{
	return (this->_flag_client_max_body_size);
}

const bool	&Location::get_flag_location(void) const
{
	return (this->_flag_location);
}

const bool	&Location::get_flag_limit_except(void) const
{
	return (this->_flag_limit_except);
}

const bool	&Location::get_flag_return_(void) const
{
	return (this->_flag_return_);
}

const bool	&Location::get_flag_root(void) const
{
	return (this->_flag_root);
}

const bool	&Location::get_flag_autoindex(void) const
{
	return (this->_flag_autoindex);
}

const bool	&Location::get_flag_index(void) const
{
	return (this->_flag_index);
}

const bool	&Location::get_flag_cgi(void) const
{
	return (this->_flag_cgi);
}
