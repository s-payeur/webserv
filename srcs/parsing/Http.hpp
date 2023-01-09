/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 10:12:00 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include <string>
# include <vector>
# include <map>
# include "Server.hpp"

class Http
{
	private :

	bool	_flag_error_page;
	bool	_flag_client_max_body_size;
	bool	_flag_server;
	bool	_flag_limit_except;
	bool	_flag_root;
	bool	_flag_autoindex;
	bool	_flag_index;

	public :

	std::map<int, std::string>			error_page;
	long long							client_max_body_size;
	std::vector<Server>					server;
	std::vector<std::string>			limit_except;
	std::string							root;
	bool								autoindex;
	std::vector<std::string>			index;

	Http(void);

	void	set_flag_error_page(bool value);
	void	set_flag_client_max_body_size(bool value);
	void	set_flag_server(bool value);
	void	set_flag_limit_except(bool value);
	void	set_flag_root(bool value);
	void	set_flag_autoindex(bool value);
	void	set_flag_index(bool value);

	const bool	&get_flag_error_page(void) const;
	const bool	&get_flag_client_max_body_size(void) const;
	const bool	&get_flag_server(void) const;
	const bool	&get_flag_limit_except(void) const;
	const bool	&get_flag_root(void) const;
	const bool	&get_flag_autoindex(void) const;
	const bool	&get_flag_index(void) const;
};

#endif
