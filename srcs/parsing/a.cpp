/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a.cpp                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*	 Created: 2022/12/17 10:43:15 by spayeur		   #+#	  #+#			  */
/*   Updated: 2023/01/09 16:58:52 by spayeur          ###   ########.fr       */
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
