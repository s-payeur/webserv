/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parse.cpp											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: spayeur <spayeur@student.42.fr>			+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2022/12/12 03:13:21 by spayeur		   #+#	  #+#			  */
/*	 Updated: 2022/12/16 13:12:59 by spayeur		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include <cstddef>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Webserv.hpp"

template <typename T1, typename T2 = const char[], typename T3 = const char[], typename T4 = const char[], typename T5 = const char[], typename T6 = const char[], typename T7 = const char[], typename T8 = const char[], typename T9 = const char[]>
int	parsing_error(T1 a, T2 b = "", T3 c = "", T4 d = "", T5 e = "", T6 f = "", T7 g = "", T8 h = "", T9 i = "")
{
	std::cerr << "webserv: parsing error: " << a << b << c << d << e << f << g << h << i << std::endl;
	return (-1);
}

int	invalid_number_of_arguments_error(const std::string directive)
{
	return (parsing_error("invalid number of arguments in '", directive, "' directive"));
}

int	invalid_value_error(const std::string value)
{
	return (parsing_error("invalid value '", value, "'"));
}

int	value_must_be_between_error(const std::string value, const long long min, const long long max, const std::string unit)
{
	return (parsing_error("value '", value, unit, "' must be between ", min, unit, " and ", max, unit));
}

std::string	get_first_word(const std::string str)
{
	const char	whitespaces_and_semicolon[] = " \t\n\v\f\r;";
	std::size_t	before_first_word;
	std::size_t	after_first_word;

	before_first_word = str.find_first_not_of(whitespaces_and_semicolon);
	after_first_word = str.find_first_of(whitespaces_and_semicolon, before_first_word);
	if (before_first_word == std::string::npos || after_first_word == std::string::npos)
		return (std::string());
	return (str.substr(before_first_word, after_first_word - before_first_word));
}

std::string	get_last_word(const std::string str)
{
	const char	whitespaces_and_semicolon[] = " \t\n\v\f\r;";
	std::size_t	rbefore_last_word;
	std::size_t	rafter_last_word;

	rbefore_last_word = str.find_last_not_of(whitespaces_and_semicolon);
	rafter_last_word = str.find_last_of(whitespaces_and_semicolon, rbefore_last_word);
	if (rbefore_last_word == std::string::npos || rafter_last_word == std::string::npos)
		return (std::string());
	return (str.substr(rafter_last_word + 1, rbefore_last_word - rafter_last_word));
}


int	check_number_of_arguments(const std::string directive, const std::string line, int unique)
{
	const char	whitespaces_and_semicolon[] = " \t\n\v\f\r;";
	std::string	first_word = get_first_word(line);
	std::string	last_word = get_last_word(line);

	if (first_word.empty() || last_word.empty())
		return (invalid_number_of_arguments_error(directive));
	
	if (unique)
	{
		if (first_word != last_word)		
			return (invalid_number_of_arguments_error(directive));
	}
	else
	{
		if (first_word == last_word && line.find(first_word) == line.rfind(last_word))
			return (invalid_number_of_arguments_error(directive));
	}
	return (0);
}

#include <map>
#include <stdlib.h>
static int	parse_error_page(Http *http, std::string line, std::size_t *before, std::size_t *after)
{
	const char	whitespaces[] = " \t\n\v\f\r";
	const char	digits[] = "0123456789";
	std::string	word;
	std::string	page;
	std::size_t	rbefore_page;
	std::size_t	rafter_page;
	int	code;

	rbefore_page = line.find_first_of(";", *after);
	rafter_page = line.find_last_of(whitespaces, rbefore_page);
	page = line.substr(rafter_page + 1, rbefore_page - rafter_page);

	*before = line.find_first_not_of(whitespaces, *after);
	*after = line.find_first_of(whitespaces, *before);
	word = line.substr(*before, *after - *before);

	if (word.compare(page) == 0)
	{
		std::cerr << "webserv: parsing error: invalid number of arguments in 'error_page' directive" << std::endl;
		return (-1);
	}

	while (word.compare(page) != 0)
	{
		if (word.find_first_not_of(digits) != std::string::npos)
		{
			std::cerr << "webserv: parsing error: invalid value '" << word << "'" << std::endl;
			return (-1);
		}
		code = atoi(word.c_str());
		if (code < 300 || 599 < code)
		{
			std::cerr << "webserv: parsing error: value'" << word << "' must be between 300 and 599" << std::endl;
			return (-1);
		}
		if (http->error_page.count(code) == 0);
			http->error_page.insert(std::pair<int, std::string>(code, page));

		*before = line.find_first_not_of(whitespaces, *after);
		*after = line.find_first_of(whitespaces, *before);
		word = line.substr(*before, *after - *before);
	}

//	for(std::map<int, std::string>::const_iterator it = http->error_page.begin(); it != http->error_page.end(); ++it)
//	{
//		std::cout << it->first << " " << it->second << "\n";
//	}
	return (0);
}

static int	parse_client_max_body_size(Http *http, const std::string directive, const std::string line)
{
	const char	digits[] = "0123456789";
	const char	units[] = "kKmMgG";
	std::string	word;
	std::string	value;
	std::size_t	before_value;
	std::size_t	after_value;
	std::string	unit;
	long long	size;
	long long	limit;

	if (check_number_of_arguments(directive, line, 1) < 0)
		return (-1);

	word = get_first_word(line);
	before_value = 0;
	after_value = word.find_first_not_of(digits, before_value);
	if (before_value == after_value)
		return (invalid_value_error(word));
	value = word.substr(before_value, after_value - before_value);

	if (after_value != std::string::npos)
	{
		unit = word.substr(after_value);
		if (unit == "k" || unit == "K")
			limit = 9007199254740991;
		else if (unit == "m" || unit == "M")
			limit = 8796093022207;
		else if (unit == "g" || unit == "G")
			limit = 8589934591;
		else
			return (invalid_value_error(word));
	}
	else
	{
		unit = "";
		limit = 9223372036854775807;
	}
	
	if (value.size() > 19 || (value.size() == 19 && value.compare("9223372036854775807") > 0))
		return (value_must_be_between_error(value, 0, limit, unit));
	size = atoll(word.c_str());
	if (size > limit)
		return (value_must_be_between_error(value, 0, limit, unit));
	http->client_max_body_size = size;

//	std::cout << http->client_max_body_size << std::endl;
	return (0);
}

static int	parse_http(Http *http, std::string str)
{
	const char	whitespaces[] = " \t\n\v\f\r";
//	const char	whitespaces_and_delimiters[] = " \t\n\v\f\r{};";
	std::string	word;
	std::size_t	before_word;
	std::size_t	after_word;

	before_word = str.find_first_not_of(whitespaces, 0);
	after_word = str.find_first_of(whitespaces, before_word);
	if (before_word == std::string::npos || after_word == std::string::npos)
	{
		std::cerr << "webserv: parsing error: unexpected end of file" << std::endl;
		return (-1);
	}
	word = str.substr(before_word, after_word - before_word);
	if (word.compare("http") != 0)
	{
		std::cerr << "webserv: parsing error: expected 'http' but found '" << word << "'" << std::endl;
		return (-1);
	}

	before_word = str.find_first_not_of(whitespaces, after_word);
	after_word = str.find_first_of(whitespaces, before_word);
	if (before_word == std::string::npos || after_word == std::string::npos || (str.substr(before_word, after_word - before_word)).compare("{") != 0)
	{
		std::cerr << "webserv: parsing error: expected '{' after_word 'http'" << std::endl;
		return (-1);
	}


	before_word = str.find_first_not_of(whitespaces, after_word);
	after_word = str.find_first_of(whitespaces, before_word);
	while (before_word != std::string::npos && after_word != std::string::npos)
	{
		word = str.substr(before_word, after_word - before_word);
		if (word.compare("}") == 0)
			break ;
		else if (word.compare("listen") == 0)
		{
			std::cerr << "webserv: parsing error: '" << word << "' directive is not allowed here" << std::endl;
			return (-1);
		}
		else if (word.compare("server_name") == 0)
		{
			std::cerr << "webserv: parsing error: '" << word << "' directive is not allowed here" << std::endl;
			return (-1);
		}
		else if (word.compare("error_page") == 0)
		{
			if (parse_error_page(http, str, &before_word, &after_word) < 0)
				return (-1);
		}
		else if (word.compare("client_max_body_size") == 0)
		{
			const char	delimiters[] = "{};";
			std::string	line;
			std::size_t	before_line;
			std::size_t	after_line;

	//		before_line = str.find_first_not_of(whitespaces, after_word);
			before_line = after_word;
			after_line = str.find_first_of(delimiters, before_line);
			if (before_line == std::string::npos || after_line == std::string::npos)
			{
				std::cerr << "webserv: parsing error: unexpected end of file" << std::endl;
				return (-1);
			}
			if (str[after_line] != ';')
			{
				std::cerr << "webserv: parsing error: directive '" << word << "' is not terminated by ';'" << std::endl;
				return (-1);
			}
			after_line += 1;
			line = str.substr(before_line, after_line - before_line);

			if (parse_client_max_body_size(http, word, line) < 0)
				return (-1);

			after_word = after_line;
		}
		else if (word.compare("server") == 0)
		{
		}
		else if (word.compare("limit_except") == 0)
		{
		}
		else if (word.compare("root") == 0)
		{
		}
		else if (word.compare("autoindex") == 0)
		{
		}
		else if (word.compare("index") == 0)
		{
		}
		else if (word.compare("cgi") == 0)
		{
			std::cerr << "webserv: parsing error: '" << word << "' directive is not allowed here" << std::endl;
			return (-1);
		}
		else
		{
			std::cerr << "webserv: parsing error: unknown directive '" << word << "'" << std::endl;
			return (-1);
		}
		before_word = str.find_first_not_of(whitespaces, after_word);
		after_word = str.find_first_of(whitespaces, before_word);
	}


	if (before_word == std::string::npos || after_word == std::string::npos)
	{
		std::cerr << "webserv: parsing error: unexpected end of file" << std::endl;
		return (-1);
	}
	if ((str.substr(before_word, after_word - before_word)).compare("}") != 0)
	{
		std::cerr << "webserv: parsing error: expected '}' at the end of 'http' section" << std::endl;
		return (-1);
	}
	return (0);
}

int	parse(Http *http, std::string configuration_file)
{
	std::ifstream		ifs;
	std::stringstream	sstr;

	ifs.open(configuration_file.c_str());
	if (!(ifs.is_open()))
	{
		std::cerr << "webserv: " << configuration_file << ": " << strerror(errno) << std::endl;
		return (-1);
	}
	sstr << ifs.rdbuf();
	if (parse_http(http, sstr.str()) < 0)
	{
		ifs.close();
		return (-1);
	}
	ifs.close();
	return (0);
}
