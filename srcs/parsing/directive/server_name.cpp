/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_name.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:42:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/10 10:48:19 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <string>
#include "Server.hpp"
#include "parsing.hpp"

int	parse_server_name(std::pair<e_context, void*> &context, std::vector<std::string> &args)
{
	const std::string	server_name(args[0]);

	get_context<Server>(context).server_name = server_name;
	get_context<Server>(context).set_flag_server_name(true);
	return (0);
}
