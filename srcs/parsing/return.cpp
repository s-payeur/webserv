/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:50:51 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 16:51:30 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <string>
#include "Server.hpp"
#include "Location.hpp"
#include "context.hpp"

int	parse_return(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::string	code;
	std::string	text_url;

	// Get code and text_url
	if (args.size() == 1)
	{
		if (args[0].find_first_not_of("0123456789") == std::string::npos)
		{
			code = args[0];
			text_url = "";
		}
		else
		{
			code = "301";
			text_url = args[0];
		}
	}
	else
	{
		code = args[0];
		text_url = args[1];
	}

	// Check code validity
	if (code.find_first_not_of("0123456789") != std::string::npos)
		return (invalid_return_code_error(directive, l, code));
	code = code.substr(code.find_first_not_of("0"));
	// Check code limit
	if (code.size() != 3 || code.compare("599") > 0)
		return (return_code_must_be_between_error(directive, l, code, 100, 599));

	if (context.first == SERVER)
	{
		get_context<Server>(context).return_ = std::pair<int, std::string>(std::atoi(code.c_str()), text_url);
		get_context<Server>(context).set_flag_return_(true);
	}
	else
	{
		get_context<Location>(context).return_ = std::pair<int, std::string>(std::atoi(code.c_str()), text_url);
		get_context<Location>(context).set_flag_return_(true);
	}
	return (0);
}
