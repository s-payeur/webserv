/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:21:22 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 15:46:48 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include "Http.hpp"

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
	return (0);
}
