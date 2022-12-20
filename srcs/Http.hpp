/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2022/12/16 01:20:28 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include <string>
# include <vector>
# include <map>
# include <utility>
# include "Server.hpp"

class Http
{
	public :

	std::map<int, std::string>	error_page;
	long long					client_max_body_size;
	std::vector<Server>			server;
	std::vector<std::string>	limit_except; // NORMALEMENT AUTORISÉ UNIQUEMENT DANS LOCATION
	std::string					root;
	bool						autoindex;
	std::vector<std::string>	index;
};

#endif
