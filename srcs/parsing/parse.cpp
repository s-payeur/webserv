/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:27:42 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 15:51:48 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <cstring>
#include "Http.hpp"

int	parse(Http &http, const std::string &configuration_file)
{
	std::ifstream	ifs;

	ifs.open(configuration_file.c_str());
	if (!(ifs.is_open()))
	{
		std::cerr << "webserv: " << configuration_file << ": " << strerror(errno) << std::endl;
		return (-1);
	}
	if (core_parsing(http, ifs) < 0)
	{
		ifs.close();
		return (-1);
	}
	ifs.close();
	return (0);
}
