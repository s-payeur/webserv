/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 10:29:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/10 11:26:59 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

/* ************************************************************************** */
/*                                 Libraries                                  */
/* ************************************************************************** */

# include <fstream>
# include <vector>
# include <set>
# include <stack>
# include <utility>
# include <string>
# include "Http.hpp"

/* ************************************************************************** */
/*                                 Structures                                 */
/* ************************************************************************** */

enum e_context {
	MAIN = 1,
	HTTP = 2,
	SERVER = 4,
	LOCATION = 8
};

/* ************************************************************************** */
/*                           Function declarations                            */
/* ************************************************************************** */

/* ******************************** Parsing ********************************* */

int	parse(Http &http, const std::string &configuration_file);
int	core_parsing(Http &http, std::ifstream &ifs);
int	extract_tokens(std::ifstream &ifs, std::vector<std::string> &tokens, std::vector<std::string>::const_iterator &token_it, std::string::size_type &l);
void	fix_parsing(Http &http);

/* ******************************** Context ********************************* */

int	parse_context(std::ifstream &ifs, std::string &directive, std::vector<std::string> &args, e_context context, size_t allowed_contexts, std::vector<std::string> &tokens, std::vector<std::string>::const_iterator &token, std::string::size_type l);
void	parse_http(std::stack< std::pair<e_context, void *> > &contexts, Http &http);
void	parse_server(std::stack< std::pair<e_context, void *> > &contexts);
int	parse_location(std::stack< std::pair<e_context, void *> > &contexts, const std::string &directive, const std::vector<std::string> &args, const size_t l);

/* ******************************* Directive ******************************** */

int	parse_directive(std::ifstream &ifs, std::string &directive, std::vector<std::string> &args, e_context context, size_t allowed_contexts, std::vector<std::string> &tokens, std::vector<std::string>::const_iterator &token, std::string::size_type l);
int	parse_listen(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const std::string::size_type l);
int	parse_server_name(std::pair<e_context, void*> &context, std::vector<std::string> &args);
int	parse_error_page(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const std::string::size_type l);
int	parse_client_max_body_size(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const std::string::size_type l);
int	parse_limit_except(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l);
int	parse_return(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l);
int	parse_root(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l);
int	parse_autoindex(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l);
int	parse_index(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l);
int	parse_cgi(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l);

/* ********************************* Error ********************************** */

int	unexpected_character_error(const char character, const std::string::size_type l);
int	unexpected_end_of_file_error(const std::string::size_type l);
int	unknown_directive_error(const std::string &directive, const std::string::size_type l);
int	directive_not_allowed_here_error(const std::string &directive, const std::string::size_type l);
int	directive_has_no_opening_error(const std::string &directive, const std::string::size_type l);
int	invalid_number_of_arguments_error(const std::string &directive, const std::string::size_type l);
int	too_long_argument_error(const std::string &directive, const std::string::size_type l, const std::string &short_path);
int	invalid_value_error(const std::string &directive, const std::string::size_type l, const std::string &value);
int	value_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &value, const long long min, const long long max, const std::string &unit);
int	duplicate_location_error(const std::string &directive, const std::string::size_type l, const std::string &location, const std::string &aka);
int	location_is_outside_location_error(const std::string &directive, const std::string::size_type l, const std::string &location1, const std::string &aka, const std::string &location2);
int	invalid_host_error(const std::string &directive, const std::string::size_type l, const std::string &value);
int	host_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &host, const long long min, const long long max);
std::string	host_not_found_error(const std::string &directive, const std::string::size_type l, const std::string &host_port);
int	invalid_port_error(const std::string &directive, const std::string::size_type l, const std::string &value);
int	port_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &port, const long long min, const long long max);
int	invalid_method_error(const std::string &directive, const std::string::size_type l, const std::string &method);
int	invalid_return_code_error(const std::string &directive, const std::string::size_type l, const std::string &code);
int	return_code_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &code, const long long min, const long long max);
std::string	symbolic_link_loop_error(const std::string &directive, const std::string::size_type l, const std::string &path);
int too_long_path_after_resolution_error(const std::string &directive, const std::string::size_type l, const std::string &short_path);

/* ********************************* Utils ********************************** */

std::string	normalize_path(const std::string &directive, const size_t l, std::string path, bool absolute_path, bool symbolic_link_resolution, std::set<std::string> link_components = std::set<std::string>());

/* ************************************************************************** */
/*                                 Templates                                  */
/* ************************************************************************** */

template <class T>
T	&get_context(std::pair<e_context, void *> &context)
{
	return (*(reinterpret_cast<T *>(context.second)));
}

#endif
