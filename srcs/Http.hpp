/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/06 13:38:07 by spayeur          ###   ########.fr       */
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
	public :

	std::map<int, std::string>			error_page;
	long long							client_max_body_size;
	std::vector<Server>					server;
	std::vector<std::string>			limit_except;
	std::string							root;
	bool								autoindex;
	std::vector<std::string>			index;

	Http(void);
};

#endif
