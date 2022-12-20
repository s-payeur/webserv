/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2022/12/12 02:52:01 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <vector>
# include <map>
# include <utility>

class Location
{
	private :

	std::map<int, std::string>			_error_page;
	long								_client_max_body_size;
	std::vector<Location>				_location;
	std::vector<std::string>			_limit_except;
	std::pair<int, std::string>			_return;
	std::string							_root;
	bool								_autoindex;
	std::vector<std::string>			_index;
	std::map<std::string, std::string>	_cgi;

	public :

};

#endif
