/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 04:22:46 by spayeur           #+#    #+#             */
/*   Updated: 2022/12/16 00:37:58 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include "Http.hpp"

int	parse(Http *http, std::string configuration_file);

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

	if (parse(&http, configuration_file) < 0)
		return (1);

	return (0);
}
