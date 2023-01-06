/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/06 13:22:52 by spayeur          ###   ########.fr       */
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

	error_page = std::map<int, std::string>();
	client_max_body_size = 1048576; // equals 1 * 1024 * 1024 or 1MiB (1m)
	server = std::vector<Server>();
	limit_except = std::vector<std::string>();
	root = "/mnt/nfs/homes/" + username + "/goinfre/webserv/html";
	autoindex = false;
	index = std::vector<std::string>();
}
