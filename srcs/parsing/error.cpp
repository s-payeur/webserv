/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:17:03 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/09 16:18:47 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

template <typename T1, typename T2 = const char[], typename T3 = const char[], typename T4 = const char[], typename T5 = const char[], typename T6 = const char[], typename T7 = const char[], typename T8 = const char[], typename T9 = const char[], typename T10 = const char[], typename T11 = const char[], typename T12 = const char[], typename T13 = const char[], typename T14 = const char[]>
int	parsing_error(T1 a, T2 b = "", T3 c = "", T4 d = "", T5 e = "", T6 f = "", T7 g = "", T8 h = "", T9 i = "", T10 j = "", T11 k = "", T12 l = "", T13 m = "", T14 n = "")
{
	std::cerr << "webserv: parsing error: " << a << b << c << d << e << f << g << h << i << j << k << l << m << n << std::endl;
	return (-1);
}

int	unexpected_character_error(const char character, const std::string::size_type l)
{
	return (parsing_error("unexpected '", character, "' (line ", l, ")"));
}

int	unexpected_end_of_file_error(const std::string::size_type l)
{
	return (parsing_error("unexpected end of file, expecting ';' or '}' (line ", l, ")"));
}

int	unknown_directive_error(const std::string &directive, const std::string::size_type l)
{
	return (parsing_error("unknown directive '", directive, "' (line ", l, ")"));
}

int	directive_not_allowed_here_error(const std::string &directive, const std::string::size_type l)
{
	return (parsing_error("'", directive, "' directive is not allowed here (line ", l, ")"));
}

int	directive_has_no_opening_error(const std::string &directive, const std::string::size_type l)
{
	return (parsing_error("directive '", directive, "' has no opening '{' (line ", l, ")"));
}

int	invalid_number_of_arguments_error(const std::string &directive, const std::string::size_type l)
{
	return (parsing_error("invalid number of arguments in '", directive, "' directive (line ", l, ")"));
}

int	too_long_argument_error(const std::string &directive, const std::string::size_type l, const std::string &short_path)
{
	return (parsing_error("too long argument '", short_path, "' started in '", directive, "' directive (line ", l, ")"));
}

int	invalid_value_error(const std::string &directive, const std::string::size_type l, const std::string &value)
{
	return (parsing_error("invalid value '", value, "' in '", directive, "' directive (line ", l, ")"));
}

int	value_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &value, const long long min, const long long max, const std::string &unit)
{
	return (parsing_error("value '", value, unit, "' must be between ", min, unit, " and ", max, unit, " in '", directive, "' directive (line ", l, ")"));
}

int	duplicate_location_error(const std::string &directive, const std::string::size_type l, const std::string &location, const std::string &aka)
{
	if (location == aka)
		return (parsing_error("duplicate location '", location, "' in '", directive, "' directive (line ", l, ")"));
	else
		return (parsing_error("duplicate location '", location, "' (aka '", aka, "') in '", directive, "' directive (line ", l, ")"));
}

int	location_is_outside_location_error(const std::string &directive, const std::string::size_type l, const std::string &location1, const std::string &aka, const std::string &location2)
{
	if (location1 == aka)
		return (parsing_error("location '", location1, "' is outside location '", location2, "' in '", directive, "' directive (line ", l, ")"));
	else
		return (parsing_error("location '", location1, "' (aka '", aka, "') is outside location '", location2, "' in '", directive, "' directive (line ", l, ")"));
}

int	invalid_host_error(const std::string &directive, const std::string::size_type l, const std::string &value)
{
	return (parsing_error("invalid host in '", value, "' of the '", directive, "' directive (line ", l, ")"));
}

int	host_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &host, const long long min, const long long max)
{
	return (parsing_error("host '", host, "' bytes must be between ", min, " and ", max, " in '", directive, "' directive (line ", l, ")"));
}

std::string	host_not_found_error(const std::string &directive, const std::string::size_type l, const std::string &host_port)
{
	parsing_error("host not found in '", host_port, "' of the '", directive, "' directive (line ", l, ")");
	return (std::string());
}

int	invalid_port_error(const std::string &directive, const std::string::size_type l, const std::string &value)
{
	return (parsing_error("invalid port in '", value, "' of the '", directive, "' directive (line ", l, ")"));
}

int	port_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &port, const long long min, const long long max)
{
	return (parsing_error("port '", port, "' must be between ", min, " and ", max, " in '", directive, "' directive (line ", l, ")"));
}

int	invalid_method_error(const std::string &directive, const std::string::size_type l, const std::string &method)
{
	return (parsing_error("invalid method '", method, "' in '", directive, "' directive (line ", l, ")"));
}

int	invalid_return_code_error(const std::string &directive, const std::string::size_type l, const std::string &code)
{
	return (parsing_error("invalid return code '", code, "' in '", directive, "' directive (line ", l, ")"));
}

int	return_code_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &code, const long long min, const long long max)
{
	return (parsing_error("return code '", code, "' must be between ", min, " and ", max, " in '", directive, "' directive (line ", l, ")"));
}

std::string	symbolic_link_loop_error(const std::string &directive, const std::string::size_type l, const std::string &path)
{
	parsing_error("symbolic link loop detected in path '", path, "' of the '", directive, "' directive (line ", l, ")");
	return (std::string());
}

int too_long_path_after_resolution_error(const std::string &directive, const std::string::size_type l, const std::string &short_path)
{
	return (parsing_error("too long path '", short_path, "' after resolution started in '", directive, "' directive (line ", l, ")"));
}
