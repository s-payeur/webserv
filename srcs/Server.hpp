/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 10:13:54 by spayeur          ###   ########.fr       */
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
	private :

	bool	_flag_listen;
	bool	_flag_server_name;
	bool	_flag_error_page;
	bool	_flag_client_max_body_size;
	bool	_flag_location;
	bool	_flag_limit_except;
	bool	_flag_return_;
	bool	_flag_root;
	bool	_flag_autoindex;
	bool	_flag_index;

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

	void	set_flag_listen(bool value);
	void	set_flag_server_name(bool value);
	void	set_flag_error_page(bool value);
	void	set_flag_client_max_body_size(bool value);
	void	set_flag_location(bool value);
	void	set_flag_limit_except(bool value);
	void	set_flag_return_(bool value);
	void	set_flag_root(bool value);
	void	set_flag_autoindex(bool value);
	void	set_flag_index(bool value);

	const bool	&get_flag_listen(void) const;
	const bool	&get_flag_server_name(void) const;
	const bool	&get_flag_error_page(void) const;
	const bool	&get_flag_client_max_body_size(void) const;
	const bool	&get_flag_location(void) const;
	const bool	&get_flag_limit_except(void) const;
	const bool	&get_flag_return_(void) const;
	const bool	&get_flag_root(void) const;
	const bool	&get_flag_autoindex(void) const;
	const bool	&get_flag_index(void) const;
};

#endif
