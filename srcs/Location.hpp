/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/08 08:11:06 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <vector>
# include <map>
# include <utility>

class Server;
class Location
{
	public :

	std::string							uri;
	std::map<int, std::string>			error_page;
	long long							client_max_body_size;
	std::vector<Location>				location;
	std::vector<std::string>			limit_except;
	std::pair<int, std::string>			return_;
	std::string							root;
	bool								autoindex;
	std::vector<std::string>			index;
	std::map<std::string, std::string>	cgi;

	Location(const Server &server, const std::string &value);
	Location(const Location &inherited_location, const std::string &value);
};

#endif
