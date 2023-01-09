/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a.cpp                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*	 Created: 2022/12/17 10:43:15 by spayeur		   #+#	  #+#			  */
/*   Updated: 2023/01/09 16:11:08 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// [x] PARSER CORRECTEMENT LOCATION
// [x] METTRE DANS LES STRUCTURES
// [x] AJOUTER L'ERREUR SI LOCATION FAIT PAS PARTIE DE LA LOCATION PRECEDENT
// location "/b" is outside location "/a" in /etc/nginx/nginx.conf:xx
// [-] GERER LES DOUBLONS
// "client_max_body_size" directive is duplicate in /etc/nginx/nginx.conf:xx
// [x] CORRIGER LE PARSING DANS LE CAS OU UNE DIRECTIVE SE TROUVE APRES UN CONTEXT
// -> Pour ce faire, la structures des classes http, server et location du commit (b12d4a7)
// et à la fin d'un context, lancer un post-parsing qui définie pour toutes les valeurs
// définie manuellement du bloc en question (d'après le flag) les mêmes valeurs pour 
// tous les blocs enfant donc les valeurs en question ne sont pas définies (d'après le flag).


// [x] VERIFIER SI LOCATION ACCEPTE ./ ../
// => Oui, avec absolute:true et symbolic_link_resolution:true
// [x] VERIFIER SI LOCATION RENVOIE UNE ERREUR SI LE CHEMIN NE COMMENCE PAS PAR '/'
// => Non. Autorisé avec le comportement ci-dessus.
// [x] VERIFIER LE COMPORTEMENT AVEC PLUSIEURS LOCATION DESIGNANT LE MEME CHEMIN
// duplicate location "/" in /etc/nginx/nginx.conf:xx
// [-] VERIFIER SI LA VALEUR PAR DEFAUT DE LISTEN EN MODE NON-ROOT EST 8000 ou 8080

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stack>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <limits.h>	// PATH_MAX (directive, root. index, cgi)
#include <unistd.h>	// geteuid, readlink (root, index, cgi)
#include <netdb.h>		// getaddrinfo (listen)
#include <arpa/inet.h>	// inet_ntop (listen)
#include "Http.hpp"

template <typename T1, typename T2 = const char[], typename T3 = const char[], typename T4 = const char[], typename T5 = const char[], typename T6 = const char[], typename T7 = const char[], typename T8 = const char[], typename T9 = const char[], typename T10 = const char[], typename T11 = const char[], typename T12 = const char[], typename T13 = const char[], typename T14 = const char[]>
int	parsing_error(T1 a, T2 b = "", T3 c = "", T4 d = "", T5 e = "", T6 f = "", T7 g = "", T8 h = "", T9 i = "", T10 j = "", T11 k = "", T12 l = "", T13 m = "", T14 n = "")
{
	std::cerr << "webserv: parsing error: " << a << b << c << d << e << f << g << h << i << j << k << l << m << n << std::endl;
	return (-1);
}

int	unexpected_character_error(const char character, const size_t l)
{
	return (parsing_error("unexpected '", character, "' (line ", l, ")"));
}

int	unexpected_end_of_file_error(const size_t l)
{
	return (parsing_error("unexpected end of file, expecting ';' or '}' (line ", l, ")"));
}

int	unknown_directive_error(const std::string &directive, const size_t l)
{
	return (parsing_error("unknown directive '", directive, "' (line ", l, ")"));
}

int	directive_not_allowed_here_error(const std::string &directive, const size_t l)
{
	return (parsing_error("'", directive, "' directive is not allowed here (line ", l, ")"));
}

int	directive_has_no_opening_error(const std::string &directive, const size_t l)
{
	return (parsing_error("directive '", directive, "' has no opening '{' (line ", l, ")"));
}

int	invalid_number_of_arguments_error(const std::string &directive, const size_t l)
{
	return (parsing_error("invalid number of arguments in '", directive, "' directive (line ", l, ")"));
}

int	too_long_argument_error(const std::string &directive, const size_t l, const std::string &short_path)
{
	return (parsing_error("too long argument '", short_path, "' started in '", directive, "' directive (line ", l, ")"));
}

int	invalid_value_error(const std::string &directive, const size_t l, const std::string &value)
{
	return (parsing_error("invalid value '", value, "' in '", directive, "' directive (line ", l, ")"));
}

int	value_must_be_between_error(const std::string &directive, const size_t l, const std::string &value, const long long min, const long long max, const std::string &unit)
{
	return (parsing_error("value '", value, unit, "' must be between ", min, unit, " and ", max, unit, " in '", directive, "' directive (line ", l, ")"));
}

int	duplicate_location_error(const std::string &directive, const size_t l, const std::string &location, const std::string &aka)
{
	if (location == aka)
		return (parsing_error("duplicate location '", location, "' in '", directive, "' directive (line ", l, ")"));
	else
		return (parsing_error("duplicate location '", location, "' (aka '", aka, "') in '", directive, "' directive (line ", l, ")"));
}

int	location_is_outside_location_error(const std::string &directive, const size_t l, const std::string &location1, const std::string &aka, const std::string &location2)
{
	if (location1 == aka)
		return (parsing_error("location '", location1, "' is outside location '", location2, "' in '", directive, "' directive (line ", l, ")"));
	else
		return (parsing_error("location '", location1, "' (aka '", aka, "') is outside location '", location2, "' in '", directive, "' directive (line ", l, ")"));
}

int	invalid_host_error(const std::string &directive, const size_t l, const std::string &value)
{
	return (parsing_error("invalid host in '", value, "' of the '", directive, "' directive (line ", l, ")"));
}

int	host_must_be_between_error(const std::string &directive, const size_t l, const std::string &host, const long long min, const long long max)
{
	return (parsing_error("host '", host, "' bytes must be between ", min, " and ", max, " in '", directive, "' directive (line ", l, ")"));
}

std::string	host_not_found_error(const std::string &directive, const size_t l, const std::string &host_port)
{
	parsing_error("host not found in '", host_port, "' of the '", directive, "' directive (line ", l, ")");
	return (std::string());
}

int	invalid_port_error(const std::string &directive, const size_t l, const std::string &value)
{
	return (parsing_error("invalid port in '", value, "' of the '", directive, "' directive (line ", l, ")"));
}

int	port_must_be_between_error(const std::string &directive, const size_t l, const std::string &port, const long long min, const long long max)
{
	return (parsing_error("port '", port, "' must be between ", min, " and ", max, " in '", directive, "' directive (line ", l, ")"));
}

int	invalid_method_error(const std::string &directive, const size_t l, const std::string &method)
{
	return (parsing_error("invalid method '", method, "' in '", directive, "' directive (line ", l, ")"));
}

int	invalid_return_code_error(const std::string &directive, const size_t l, const std::string &code)
{
	return (parsing_error("invalid return code '", code, "' in '", directive, "' directive (line ", l, ")"));
}

int	return_code_must_be_between_error(const std::string &directive, const size_t l, const std::string &code, const long long min, const long long max)
{
	return (parsing_error("return code '", code, "' must be between ", min, " and ", max, " in '", directive, "' directive (line ", l, ")"));
}

std::string	symbolic_link_loop_error(const std::string &directive, const size_t l, const std::string &path)
{
	parsing_error("symbolic link loop detected in path '", path, "' of the '", directive, "' directive (line ", l, ")");
	return (std::string());
}

int too_long_path_after_resolution_error(const std::string &directive, const size_t l, const std::string &short_path)
{
	return (parsing_error("too long path '", short_path, "' after resolution started in '", directive, "' directive (line ", l, ")"));
}

////////////////////////////////////////////////////////////////////////////////
// LOCATION, ROOT, INDEX, CGI

static std::string	resolve_symbolic_link(std::string path)
{
	char		buffer[PATH_MAX];
	std::string	link_target;

	std::memset(buffer, 0, sizeof(char) * PATH_MAX);
	link_target = readlink(path.c_str(), buffer, PATH_MAX) < 0 ? std::string() : std::string(buffer);
	if (link_target.empty())
		return (path);
	if (link_target[0] == '/')
		return (link_target);
	else
		return (path.substr(0, path.rfind('/') + 1) + link_target);
}

static std::string	normalize_path(const std::string &directive, const size_t l, std::string path, bool absolute_path, bool symbolic_link_resolution, std::set<std::string> link_components = std::set<std::string>())
{
	std::vector<std::string>	components;
	std::vector<std::string>	normalized_components;
	std::string					resolved_component;
	std::string::size_type		start;
	std::string::size_type		end;

	// Split path into multiple components using '/' as delimiter
	start = 0;
	end = path.find('/');
	while (end != std::string::npos)
	{
		if (start != end)
			components.push_back(path.substr(start, end - start));
		start = end + 1;
		end = path.find('/', start);
	}
	if (start < path.size() && start != end)
		components.push_back(path.substr(start));

	for (std::vector<std::string>::iterator it = components.begin(); it != components.end(); it++)
	{
		if (*it == ".")
			continue ;
		else if (*it == "..")
		{
			if (absolute_path)
			{
				// Remove the previous component
				if (!(normalized_components.empty()))
					normalized_components.pop_back();
			}
			else
			{
				// Remove the previous component
				if (!(normalized_components.empty()) && normalized_components.back() != "..")
					normalized_components.pop_back();
				else
					normalized_components.push_back(*it);
			}
		}
		else
		{
			normalized_components.push_back(*it);
			if (symbolic_link_resolution)
			{
				// Construct the path to the file
				path.clear();
				for (std::vector<std::string>::const_iterator c = normalized_components.begin(); c != normalized_components.end(); c++)
					path += '/' + *c;
				resolved_component = resolve_symbolic_link(path);
				if (path != resolved_component)
				{
					// Check if the components has already been encountered
					if (link_components.count(path))
						return (symbolic_link_loop_error(directive, l, path));
					link_components.insert(path);
					return (normalize_path(directive, l, resolved_component, absolute_path, symbolic_link_resolution, link_components));
				}
			}
		}
	}

	// Construct the path
	path.clear();
	for (std::vector<std::string>::const_iterator it = normalized_components.begin(); it != normalized_components.end(); it++)
		path += '/' + *it;
	return (path.empty() ? "/" : path);
}

////////////////////////////////////////////////////////////////////////////////

void	parse_http(std::stack< std::pair<e_context, void *> > &contexts, Http &http)
{
	contexts.push(std::pair<e_context, void *>(HTTP, &http));
}

////////////////////////////////////////////////////////////////////////////////

void	parse_server(std::stack< std::pair<e_context, void *> > &contexts)
{
	Http	&http = get_context<Http>(contexts.top());

	http.server.push_back(Server(http));
	contexts.push(std::pair<e_context, void *>(SERVER, &(http.server.back())));
}

////////////////////////////////////////////////////////////////////////////////

int	parse_location(std::stack< std::pair<e_context, void *> > &contexts, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::string	uri;

	// Normalize path
	uri = normalize_path(directive, l, std::string(args[0]), true, true);
	if (uri.empty())
		return (-1);
	if (uri.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, uri.substr(0, 10) + "..."));

	if (contexts.top().first == SERVER)
	{
		Server	&server = get_context<Server>(contexts.top());

		for (std::vector<Location>::const_iterator it = server.location.begin(); it != server.location.end(); it++)
		{
			if (uri == (*it).uri)
				return (duplicate_location_error(directive, l, args[0], uri));
		}
		server.location.push_back(Location(server, uri));
		contexts.push(std::pair<e_context, void *>(LOCATION, &(server.location.back())));
	}
	else
	{
		Location	&location = get_context<Location>(contexts.top());

		if ((uri != location.uri) && (uri.find(location.uri) != 0 \
		|| uri.size() <= location.uri.size() || (location.uri != "/" && uri[location.uri.size()] != '/')))
			return (location_is_outside_location_error(directive, l, args[0], uri, location.uri));
		for (std::vector<Location>::const_iterator it = location.location.begin(); it != location.location.end(); it++)
		{
			if (uri == (*it).uri)
				return (duplicate_location_error(directive, l, args[0], uri));
		}
		location.location.push_back(Location(location, uri));
		contexts.push(std::pair<e_context, void *>(LOCATION, &(location.location.back())));
	}

//	std::cout << "-->" << uri << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

static std::string	resolve_hostname(const std::string &directive, const size_t l, const std::string &host_port, const std::string &hostname)
{
	char			ip[INET_ADDRSTRLEN];
	struct addrinfo	hints;
	struct addrinfo	*result;

	// Set up hints structure (IPV4)
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;

	// Call getaddrinfo
	if (getaddrinfo(hostname.c_str(), NULL, &hints, &result) != 0)
		return (host_not_found_error(directive, l, host_port));

	// Extract the IP address from the result
	if (!(inet_ntop(AF_INET, &(reinterpret_cast<struct sockaddr_in*>(result->ai_addr)->sin_addr), ip, INET_ADDRSTRLEN)))
	{
		freeaddrinfo(result);
		return (host_not_found_error(directive, l, host_port));
	}

	// Free the result of getaddrinfo
	freeaddrinfo(result);
	return (std::string(ip));
}

int	parse_listen(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::string					host;
	std::string					port;
	std::vector<std::string>	host_components;
	std::string::size_type		start;
	std::string::size_type		end;

	// Split listen into host and port
	start = args[0].find(':');
	end = args[0].rfind(':');
	if (start != end)
		return (invalid_value_error(directive, l, args[0]));
	if (start == std::string::npos)
	{
		if (args[0] == "*")
		{
			host = "0.0.0.0";
			port = !(geteuid()) ? "80" : "8000";
		}
		else if (args[0].find_first_not_of("0123456789.") != std::string::npos)
		{
			host = resolve_hostname(directive, l, args[0], args[0]);
			if (host.empty())
				return (-1);
			port = !(geteuid()) ? "80" : "8000";
		}
		else if (args[0].find('.') == std::string::npos)
		{
			host = "0.0.0.0";
			port = args[0];
		}
		else
		{
			host = args[0];
			port = !(geteuid()) ? "80" : "8000";
		}
	}
	else
	{
		host = args[0].substr(0, end);
		port = args[0].substr(start + 1);
		if (host == "*")
		{
			host = "0.0.0.0";
		}
		else if (host.find_first_not_of("0123456789.") != std::string::npos)
		{
			host = resolve_hostname(directive, l, args[0], host);
			if (host.empty())
				return (-1);
		}
	}

	// Split host into multiple host_components using '.' as delimiter
	start = 0;
	end = host.find('.');
	while (end != std::string::npos)
	{
		if (start == end)
			return (invalid_host_error(directive, l, args[0]));
		host_components.push_back(host.substr(start, end - start));
		start = end + 1;
		end = host.find('.', start);
	}
	if (start == end)
		return (invalid_host_error(directive, l, args[0]));
	if (start < host.size())
		host_components.push_back(host.substr(start));
	if (host_components.size() != 4)
		return (invalid_host_error(directive, l, args[0]));

	for (std::vector<std::string>::const_iterator it = host_components.begin(); it != host_components.end(); it++)
	{
		// Check host validity
		if ((*it).find_first_not_of("0123456789") != std::string::npos)
			return (invalid_host_error(directive, l, args[0]));
		// Check host limit
		if ((*it).size() > 3 || ((*it).size() == 3 && (*it).compare("255") > 0))
			return (host_must_be_between_error(directive, l, host, 0, 255));
	}

	// Check port validity
	if (port.find_first_not_of("0123456789") != std::string::npos)
		return (invalid_port_error(directive, l, args[0]));
	// Check port limit
	if (port.size() > 5 || (port.size() == 5 && port.compare("65535") > 0))
		return (port_must_be_between_error(directive, l, port, 0, 65535));

	get_context<Server>(context).listen = std::pair<std::string, std::string>(host, port);
	get_context<Server>(context).set_flag_listen(true);
//	std::cout << "-->" << host << ":" << port << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_server_name(std::pair<e_context, void*> &context, std::vector<std::string> &args)
{
	const std::string	server_name(args[0]);

	get_context<Server>(context).server_name = server_name;
	get_context<Server>(context).set_flag_server_name(true);
//	std::cout << "-->" << server_name << std::endl;;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_error_page(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	const std::set<std::string>	codes(args.begin(), args.end() - 1);
	const std::string			page = args[args.size() - 1];
	std::map<int, std::string>	error_page;

	for (std::set<std::string>::const_iterator it = codes.begin(); it != codes.end(); it++)
	{
		// Check error codes validity
		if ((*it).find_first_not_of("0123456789") != std::string::npos)
				return (invalid_value_error(directive, l, *it));
		// Check error codes limit
		if (std::atoi((*it).c_str()) < 300 || std::atoi((*it).c_str()) > 599)
			return (value_must_be_between_error(directive, l, *it, 300, 599, ""));
		error_page.insert(std::pair<int, std::string>(std::atoi((*it).c_str()), page));
	}

	if (context.first == HTTP)
	{
		get_context<Http>(context).error_page.insert(error_page.begin(), error_page.end());
		get_context<Http>(context).set_flag_error_page(true);
	}
	else if (context.first == SERVER)
	{
		get_context<Server>(context).error_page.insert(error_page.begin(), error_page.end());
		get_context<Server>(context).set_flag_error_page(true);
	}
	else
	{
		get_context<Location>(context).error_page.insert(error_page.begin(), error_page.end());
		get_context<Location>(context).set_flag_error_page(true);
	}
//	std::cout << "-->";
//	for (std::set<std::string>::const_iterator it = codes.begin(); it != codes.end(); it++)
//		std::cout << *it << " ";
//	std::cout << "<--> " << page << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_client_max_body_size(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::string				value;
	std::string				suffix;
	long long				size;
	long long				limit;
	std::string::size_type	i;

	// Split the argument in value/suffix
	i = 0;
	while (i < args[0].size() && std::isdigit(args[0][i]))
		i++;
	value = args[0].substr(0, i);
	suffix = args[0].substr(i, std::string::npos);

	// Check value validity
	if (value.empty() || suffix.size() > 1 || (suffix.size() == 1 \
	&&	suffix != "k" && suffix != "K" \
	&&	suffix != "m" && suffix != "M" \
	&&	suffix != "g" && suffix != "G"))
		return (invalid_value_error(directive, l, args[0]));

	// Set limit depending on suffix
	if (suffix.empty())
		limit = 9223372036854775807;
	else if (suffix == "k" || suffix == "K")
		limit = 9007199254740991;
	else if (suffix == "m" || suffix == "M")
		limit = 8796093022207;
	else if (suffix == "g" || suffix == "G")
		limit = 8589934591;

	// Check value limit
	if (value.size() > 19 || (value.size() == 19 && value.compare("9223372036854775807") > 0))
		return (value_must_be_between_error(directive, l, value, 0, limit, suffix));
	size = atoll(value.c_str());
	if (size > limit)
		return (value_must_be_between_error(directive, l, value, 0, limit, suffix));

	// Convert value to bytes based on suffix
	if (suffix == "k" || suffix == "K")
		size *= 1024;
	else if (suffix == "m" || suffix == "M")
		size *= 1024 * 1024;
	else if (suffix == "g" || suffix == "G")
		size *= 1024 * 1024 * 1024;

	if (context.first == HTTP)
	{
		get_context<Http>(context).client_max_body_size = size;
		get_context<Http>(context).set_flag_client_max_body_size(true);
	}
	else if (context.first == SERVER)
	{
		get_context<Server>(context).client_max_body_size = size;
		get_context<Server>(context).set_flag_client_max_body_size(true);
	}
	else
	{
		get_context<Location>(context).client_max_body_size = size;
		get_context<Location>(context).set_flag_client_max_body_size(true);
	}
//	std::cout << "-->" << size << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_limit_except(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::vector<std::string>	http_methods;

	// Get http_methods
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		// Check method validity
		if (*it != "GET" \
		&&	*it != "POST" \
		&&	*it != "DELETE")
			return (invalid_method_error(directive, l, *it));
		if (!((*it).empty()) && std::find(http_methods.begin(), http_methods.end(), *it) == http_methods.end())
			http_methods.push_back(*it);
	}

	if (context.first == HTTP)
	{
		Http	&current_http = get_context<Http>(context);

		for (std::vector<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
		{
			if (std::find(current_http.limit_except.begin(), current_http.limit_except.end(), *it) == current_http.limit_except.end())
				current_http.limit_except.push_back(*it);
		}
		get_context<Http>(context).set_flag_limit_except(true);
	}
	else if (context.first == SERVER)
	{
		Server	&current_server = get_context<Server>(context);

		for (std::vector<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
		{
			if (std::find(current_server.limit_except.begin(), current_server.limit_except.end(), *it) == current_server.limit_except.end())
				current_server.limit_except.push_back(*it);
		}
		get_context<Server>(context).set_flag_limit_except(true);
	}
	else
	{
		Location	&current_location = get_context<Location>(context);

		for (std::vector<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
		{
			if (std::find(current_location.limit_except.begin(), current_location.limit_except.end(), *it) == current_location.limit_except.end())
				current_location.limit_except.push_back(*it);
		}
		get_context<Location>(context).set_flag_limit_except(true);
	}
//	std::cout << "-->";
//	for (std::set<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
//		std::cout << *it << " ";
//	std::cout << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_return(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::string	code;
	std::string	text_url;

	// Get code and text_url
	if (args.size() == 1)
	{
		if (args[0].find_first_not_of("0123456789") == std::string::npos)
		{
			code = args[0];
			text_url = "";
		}
		else
		{
			code = "301";
			text_url = args[0];
		}
	}
	else
	{
		code = args[0];
		text_url = args[1];
	}

	// Check code validity
	if (code.find_first_not_of("0123456789") != std::string::npos)
		return (invalid_return_code_error(directive, l, code));
	code = code.substr(code.find_first_not_of("0"));
	// Check code limit
	if (code.size() != 3 || code.compare("599") > 0)
		return (return_code_must_be_between_error(directive, l, code, 100, 599));

	if (context.first == SERVER)
	{
		get_context<Server>(context).return_ = std::pair<int, std::string>(std::atoi(code.c_str()), text_url);
		get_context<Server>(context).set_flag_return_(true);
	}
	else
	{
		get_context<Location>(context).return_ = std::pair<int, std::string>(std::atoi(code.c_str()), text_url);
		get_context<Location>(context).set_flag_return_(true);
	}
//	std::cout << "-->" << code << " <--> " << text_url << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_root(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	// Construct path
	std::string			root;
	std::string			path;
	const char			*login = getlogin();
	const std::string	username = !(geteuid()) ? "root" : !(login) ? "root" : login;
	const std::string	working_directory = "/mnt/nfs/homes/" + username + "/goinfre/webserv/html";

	if (args[0][0] == '/')
		path = args[0];
	else
		path = working_directory + '/' + args[0];

	// Normalize path
	root = normalize_path(directive, l, path, true, true);
	if (root.empty())
		return (-1);
	if (root.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, root.substr(0, 10) + "..."));

	if (context.first == HTTP)
	{
		get_context<Http>(context).root = root;
		get_context<Http>(context).set_flag_root(true);
	}
	else if (context.first == SERVER)
	{
		get_context<Server>(context).root = root;
		get_context<Server>(context).set_flag_root(true);
	}
	else
	{
		get_context<Location>(context).root = root;
		get_context<Location>(context).set_flag_root(true);
	}
//	std::cout << "-->" << root << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_autoindex(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::string	value;
	bool		autoindex;

	value = args[0];
	for (std::string::iterator it = value.begin(); it != value.end(); it++)
		*it = std::tolower(*it);
	if (value == "on")
		autoindex = true;
	else if (value == "off")
		autoindex = false;
	else
		return (invalid_value_error(directive, l, value));

	if (context.first == HTTP)
	{
		get_context<Http>(context).autoindex = autoindex;
		get_context<Http>(context).set_flag_autoindex(true);
	}
	else if (context.first == SERVER)
	{
		get_context<Server>(context).autoindex = autoindex;
		get_context<Server>(context).set_flag_autoindex(true);
	}
	else
	{
		get_context<Location>(context).autoindex = autoindex;
		get_context<Location>(context).set_flag_autoindex(true);
	}
//	std::cout << std::boolalpha << "-->" << autoindex << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_index(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::vector<std::string>	pages;
	std::string					page;

	// Get pages
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		// Normalize page
		page = normalize_path(directive, l, *it, false, true);
		if (page.empty())
			return (-1);
		if (page.size() >= PATH_MAX)
			return (too_long_path_after_resolution_error(directive, l, page.substr(0, 10) + "..."));
		if (!(page.empty()) && std::find(pages.begin(), pages.end(), page) == pages.end())
			pages.push_back(page);
	}

	if (context.first == HTTP)
	{
		Http	&current_http = get_context<Http>(context);

		for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
		{
			if (std::find(current_http.index.begin(), current_http.index.end(), *it) == current_http.index.end())
				current_http.index.push_back(*it);
		}
		get_context<Http>(context).set_flag_index(true);
	}
	else if (context.first == SERVER)
	{
		Server	&current_server = get_context<Server>(context);

		for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
		{
			if (std::find(current_server.index.begin(), current_server.index.end(), *it) == current_server.index.end())
				current_server.index.push_back(*it);
		}
		get_context<Server>(context).set_flag_index(true);
	}
	else
	{
		Location	&current_location = get_context<Location>(context);

		for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
		{
			if (std::find(current_location.index.begin(), current_location.index.end(), *it) == current_location.index.end())
				current_location.index.push_back(*it);
		}
		get_context<Location>(context).set_flag_index(true);
	}
//	std::cout << "-->";
//	for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
//		std::cout << *it << " ";
//	std::cout << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_cgi(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	const std::set<std::string>						extensions(args.begin(), args.end() - 1);
	std::string										cgi_path;
	std::map<std::string ,std::string>				cgi;
	std::map<std::string, std::string>::iterator	pos;

	for (std::set<std::string>::const_iterator it = extensions.begin(); it != extensions.end(); it++)
	{
		// Check extensions validity
		if ((*it)[0] != '.' || (*it).find('.', 1) != std::string::npos)
				return (invalid_value_error(directive, l, *it));
	}

	// Normalize path
	cgi_path = normalize_path(directive, l, std::string(args[args.size() - 1]), false, true);
	if (cgi_path.empty())
		return (-1);
	if (cgi_path.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, cgi_path.substr(0, 10) + "..."));

	for (std::set<std::string>::const_iterator it = extensions.begin(); it != extensions.end(); it++)
		cgi.insert(std::pair<std::string, std::string>(*it, cgi_path));

	// Insert or assign
	Location	&current_location = get_context<Location>(context);
	for (std::map<std::string, std::string>::iterator it = cgi.begin(); it != cgi.end(); it++)
	{
		pos = current_location.cgi.find(it->first);
		if (pos == current_location.cgi.end())
			current_location.cgi.insert(*it);
		else
			pos->second = it->second;
	}

	get_context<Location>(context).set_flag_cgi(true);
//	std::cout << "-->";
//	for (std::set<std::string>::const_iterator it = extensions.begin(); it != extensions.end(); it++)
//		std::cout << *it << " ";
//	std::cout << "<--> " << cgi_path << std::endl;
	return (0);
}
