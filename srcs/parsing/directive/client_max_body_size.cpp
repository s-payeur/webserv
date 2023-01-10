/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_max_body_size.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:46:25 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/10 10:48:18 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <string>
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "parsing.hpp"

int	parse_client_max_body_size(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const std::string::size_type l)
{
	std::string				value;
	std::string				suffix;
	long long				size;
	long long				limit;
	std::string::size_type	i;

	// Split the argument in value/suffix
	i = 0;
	while (i < args[0].size() && std::isdigit(args[0][i]))
		i++;
	value = args[0].substr(0, i);
	suffix = args[0].substr(i, std::string::npos);

	// Check value validity
	if (value.empty() || suffix.size() > 1 || (suffix.size() == 1 \
	&&	suffix != "k" && suffix != "K" \
	&&	suffix != "m" && suffix != "M" \
	&&	suffix != "g" && suffix != "G"))
		return (invalid_value_error(directive, l, args[0]));

	// Set limit depending on suffix
	if (suffix.empty())
		limit = 9223372036854775807;
	else if (suffix == "k" || suffix == "K")
		limit = 9007199254740991;
	else if (suffix == "m" || suffix == "M")
		limit = 8796093022207;
	else if (suffix == "g" || suffix == "G")
		limit = 8589934591;

	// Check value limit
	if (value.size() > 19 || (value.size() == 19 && value.compare("9223372036854775807") > 0))
		return (value_must_be_between_error(directive, l, value, 0, limit, suffix));
	size = atoll(value.c_str());
	if (size > limit)
		return (value_must_be_between_error(directive, l, value, 0, limit, suffix));

	// Convert value to bytes based on suffix
	if (suffix == "k" || suffix == "K")
		size *= 1024;
	else if (suffix == "m" || suffix == "M")
		size *= 1024 * 1024;
	else if (suffix == "g" || suffix == "G")
		size *= 1024 * 1024 * 1024;

	if (context.first == HTTP)
	{
		get_context<Http>(context).client_max_body_size = size;
		get_context<Http>(context).set_flag_client_max_body_size(true);
	}
	else if (context.first == SERVER)
	{
		get_context<Server>(context).client_max_body_size = size;
		get_context<Server>(context).set_flag_client_max_body_size(true);
	}
	else
	{
		get_context<Location>(context).client_max_body_size = size;
		get_context<Location>(context).set_flag_client_max_body_size(true);
	}
	return (0);
}
