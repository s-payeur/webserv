/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2022/12/12 02:56:46 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include <map>
# include <utility>
# include "Location.hpp"

class Server
{
	private :

	std::string					_host;
	unsigned short				_port;
	std::string					_server_name;
	std::map<int, std::string>	_error_page;
	long						_client_max_body_size;
	std::vector<Location>		_location;
	std::vector<std::string>	_limit_except; // NORMALEMENT AUTORISÉ UNIQUEMENT DANS LOCATION
	std::pair<int, std::string>	_return;
	std::string					_root;
	bool						_autoindex;
	std::vector<std::string>	_index;

	public :

};

#endif
