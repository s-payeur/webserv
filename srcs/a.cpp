/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 a.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: spayeur <spayeur@student.42.fr>			+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2022/12/17 10:43:15 by spayeur		   #+#	  #+#			  */
/*	 Updated: 2022/12/17 11:03:40 by spayeur		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <stack>
#include <vector>
#include <set>
#include <functional>
#include <limits.h>	// PATH_MAX (root)
#include <unistd.h>	// readlink (root)
#include <netdb.h>		// getaddrinfo (listen)
#include <arpa/inet.h>	// inet_ntop (listen)

enum e_section {
	NONE = 0,
	HTTP = 1,
	SERVER = 2,
	LOCATION = 4
};

////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2 = const char[], typename T3 = const char[], typename T4 = const char[], typename T5 = const char[], typename T6 = const char[], typename T7 = const char[], typename T8 = const char[], typename T9 = const char[], typename T10 = const char[], typename T11 = const char[], typename T12 = const char[], typename T13 = const char[], typename T14 = const char[]>
int	parsing_error(T1 a, T2 b = "", T3 c = "", T4 d = "", T5 e = "", T6 f = "", T7 g = "", T8 h = "", T9 i = "", T10 j = "", T11 k = "", T12 l = "", T13 m = "", T14 n = "")
{
	std::cerr << "webserv: parsing error: " << a << b << c << d << e << f << g << h << i << j << k << l << m << n << std::endl;
	return (-1);
}

int	directive_not_allowed_here_error(const std::string &directive, const size_t l)
{
	return (parsing_error("'", directive, "' directive is not allowed here (line ", l, ")"));
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

template <class BinaryPredicate>
int	parse_directive(std::string &directive, std::vector<std::string> &args, enum e_section section, size_t allowed_sections, const std::vector<std::string> &tokens, std::vector<std::string>::const_iterator token, BinaryPredicate p, std::string::size_type args_size_limit, const size_t l)
{
	directive = *token++;
	// Check section validity
	if (!(section & allowed_sections))
		return (directive_not_allowed_here_error(directive, l));
	// Get directive arguments
	while (token != tokens.end() && *token != ";" && *token != "{" && *token != "}")
	{
		// Check argument size
		if ((*token).size() >= PATH_MAX)
			return (too_long_argument_error(directive, l, (*token).substr(0, 10) + "..."));
		args.push_back(*token);
		token++;
	}
	// Check arguments number
	if (p(args.size(), args_size_limit))
		return (invalid_number_of_arguments_error(directive, l));
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_error_page(const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	const std::set<std::string>	error_codes(args.begin(), args.end() - 1);
	std::string					error_page;

	for (std::set<std::string>::const_iterator it = error_codes.begin(); it != error_codes.end(); it++)
	{
		// Check error codes validity
		for (size_t j = 0; j < (*it).size(); j++)
		{
			if (!(std::isdigit((*it)[j])))
				return (invalid_value_error(directive, l, *it));
		}

		// Check error codes limit
		if (std::stoi(*it) < 300 || std::stoi(*it) > 599)
			return (value_must_be_between_error(directive, l, *it, 300, 599, ""));
	}

	// Get error page
	error_page = args[args.size() - 1];

//	std::cout << "-->";
//	for (std::set<std::string>::const_iterator it = error_codes.begin(); it != error_codes.end(); it++)
//		std::cout << *it << " ";
//	std::cout << "<--> " << error_page << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_client_max_body_size(const std::string &directive, const std::vector<std::string> &args, const size_t l)
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
	&&  suffix != "k" && suffix != "K" \
	&&  suffix != "m" && suffix != "M" \
	&&  suffix != "g" && suffix != "G"))
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

//	std::cout << "-->" << size << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_limit_except(const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	const std::set<std::string>	http_methods(args.begin(), args.end());

	for (std::set<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
	{
		// Check methods validity
		if (*it != "GET" \
		&&  *it != "POST" \
		&&  *it != "DELETE")
			return (invalid_method_error(directive, l, *it));
	}

//	std::cout << "-->";
//	for (std::set<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
//		std::cout << *it << " ";
//	std::cout << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

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

static std::string	normalize_path(const std::string &directive, const size_t l, std::string path, std::set<std::string> link_components = std::set<std::string>())
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
			// Remove the previous component
			if (!(normalized_components.empty()))
				normalized_components.pop_back();
		}
		else
		{
			normalized_components.push_back(*it);
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
				return (normalize_path(directive, l, resolved_component, link_components));
			}
		}
	}

	// Construct the path
	path.clear();
	for (std::vector<std::string>::const_iterator it = normalized_components.begin(); it != normalized_components.end(); it++)
		path += '/' + *it;
	return (path.empty() ? "/" : path);
}

int	parse_root(const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	// Construct path
	std::string			path;
	const char			*login = getlogin();
	const std::string	username = !(geteuid()) ? "root" : !(login) ? "root" : login;
	const std::string	working_directory = "/home/" + username + "/goinfre/webserv/html";

	if (args[0][0] == '/')
		path = args[0];
	else
		path = working_directory + '/' + args[0];

	// Normalize path
	std::string	root = normalize_path(directive, l, path);
	if (root.empty())
		return (-1);
	if (root.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, root.substr(0, 10) + "..."));

//	std::cout << "-->" << root << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_autoindex(const std::string &directive, const std::vector<std::string> &args, const size_t l)
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

//	std::cout << std::boolalpha << "-->" << autoindex << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_index(const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::vector<std::string>	pages;
	std::string					page;

	// Get pages
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		// Normalize page
		page = normalize_path(directive, l, *it);
		if (page.empty())
			return (-1);
		// Remove the starting '/' added if not needed
		if ((*it)[0] != '/' && page[0] == '/')
			page = page.substr(1);
		if (page.size() >= PATH_MAX)
			return (too_long_path_after_resolution_error(directive, l, page.substr(0, 10) + "..."));
		if (!(page.empty()) && std::find(pages.begin(), pages.end(), page) == pages.end())
			pages.push_back(page);
	}

//	std::cout << "-->";
//	for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
//		std::cout << *it << " ";
//	std::cout << std::endl;
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
		return (host_not_found_error(directive, l, host_port));

	// Free the result of getaddrinfo
	freeaddrinfo(result);
	return (std::string(ip));
}

int	parse_listen(const std::string &directive, const std::vector<std::string> &args, const size_t l)
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

//	std::cout << "-->" << host << ":" << port << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parse_server_name(const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	const std::set<std::string>	server_names(args.begin(), args.end());

//	std::cout << "-->";
//	for (std::set<std::string>::const_iterator it = server_names.begin(); it != server_names.end(); it++)
//		std::cout << *it << " ";
//	std::cout << std::endl;
	return (0);
}

////////////////////////////////////////////////////////////////////////////////

int	parseConfigFile(const std::string &filename) {
	std::ifstream file(filename);
	std::string line;
	std::stack<e_section> sections; // stack to keep track of the context hierarchy

	size_t	l = 0;
	sections.push(NONE); // default context

	while (std::getline(file, line))
	{
		l++;
		// Split the line into tokens
		std::vector<std::string> tokens;
		std::istringstream iss(line);
		std::string token;
		while (iss >> token) {
			// Split the token into multiple tokens if it contains ';{}' character
			size_t pos = 0;
			while ((pos = token.find_first_of(";{}")) != std::string::npos) {
				std::string token1 = token.substr(0, pos);
				std::string token2 = token.substr(pos, 1);
				if (!token1.empty())
					tokens.push_back(token1);
				if (!token2.empty())
					tokens.push_back(token2);
				// Update the token for the next iteration
				token = token.substr(pos + 1);
			}
			// Add the remaining token to the vector
			if (!token.empty()) {
				tokens.push_back(token);
			}
		}

		// Skip empty line and commented lined
		if (tokens.empty() || tokens[0] == "#")
			continue ;

		// Check the first token to determine the current context
		if (tokens.size() > 0)
		{
			// Enter a new section by encountering 'http', 'server' or 'location'
			// Leave the current section by encountering '}'
			if (tokens[0] == "http")
				sections.push(HTTP);
			else if (tokens[0] == "server")
				sections.push(SERVER);
			else if (tokens[0] == "location")
				sections.push(LOCATION);
			else if (tokens[0] == "}")
				sections.pop();
		}

		for (std::vector<std::string>::const_iterator token = tokens.begin(); token != tokens.end(); token++)
		{
			if (*token == "error_page")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(directive, args, sections.top(), HTTP | SERVER | LOCATION, tokens, token, std::less<std::string::size_type>(), 2, l) < 0)
					return (-1);
				if (parse_error_page(directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "client_max_body_size")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(directive, args, sections.top(), HTTP | SERVER | LOCATION, tokens, token, std::not_equal_to<std::string::size_type>(), 1, l) < 0)
					return (-1);
				if (parse_client_max_body_size(directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "limit_except")
			{
				std::string					directive;
				std::vector<std::string>	args;

				// Normally allowed only in LOCATION but allowed in HTTP and SERVER to respect the subject's requirements
				if (parse_directive(directive, args, sections.top(), HTTP | SERVER | LOCATION, tokens, token, std::less<std::string::size_type>(), 1, l) < 0)
					return (-1);
				if (parse_limit_except(directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "root")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(directive, args, sections.top(), HTTP | SERVER | LOCATION, tokens, token, std::not_equal_to<std::string::size_type>(), 1, l) < 0)
					return (-1);
				if (parse_root(directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "autoindex")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(directive, args, sections.top(), HTTP | SERVER | LOCATION, tokens, token, std::not_equal_to<std::string::size_type>(), 1, l) < 0)
					return (-1);
				if (parse_autoindex(directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "index")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(directive, args, sections.top(), HTTP | SERVER | LOCATION, tokens, token, std::less<std::string::size_type>(), 1, l) < 0)
					return (-1);
				if (parse_index(directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "listen")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(directive, args, sections.top(), SERVER, tokens, token, std::not_equal_to<std::string::size_type>(), 1, l) < 0)
					return (-1);
				if (parse_listen(directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "server_name")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(directive, args, sections.top(), SERVER, tokens, token, std::less<std::string::size_type>(), 1, l) < 0)
					return (-1);
				if (parse_server_name(directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "return") {
				// Handle return directive
			} else if (*token == "fastcgi_pass") {
				// Handle fastcgi_pass directive
			}
/*
			std::string context;
			if (sections.top() == NONE)
				context = "NONE";
			if (sections.top() == HTTP)
				context = "HTTP";
			if (sections.top() == SERVER)
				context = "SERVER";
			if (sections.top() == LOCATION)
				context = "LOCATION";
			std::cout << context << " -> " << *token << std::endl;
*/
		}
	}
	return (0);
}

int main(int argc, char **argv) {
	if (parseConfigFile(argv[1]) < 0)
		return (1);
	return (0);
}
