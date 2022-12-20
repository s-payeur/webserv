/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2022/12/16 00:03:45 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Webserv_HPP
# define Webserv_HPP

# include <string>
# include "Http.hpp"

class Webserv
{
	private :

	Http	_http;

	public :

	Webserv(std::string configuration_file);
};

#endif
