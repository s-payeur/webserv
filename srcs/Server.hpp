/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/08 08:11:01 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include <map>
# include <utility>
# include "Location.hpp"

class Http;
class Server
{
	public :

	std::pair<std::string, std::string>	listen;
	std::string							server_name;
	std::map<int, std::string>			error_page;
	long long							client_max_body_size;
	std::vector<Location>				location;
	std::vector<std::string>			limit_except;
	std::pair<int, std::string>			return_;
	std::string							root;
	bool								autoindex;
	std::vector<std::string>			index;

	Server(const Http &http);
};

#endif
