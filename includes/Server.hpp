/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/05 17:50:53 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/07 17:32:51 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Tintin_reporter.hpp"

#include <iostream>
#include <csignal>

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <list>

class Server {

public:
	Server();
	~Server();
	Server(Server const &src);
	Server &operator=(Server const &rhs);

	void		lunchServer(int port, unsigned long nbClient);
	int			getSocket() const;

private:
	void		createServer(int port);
	int 		ServerAcceptConnexion();
	std::string	ServerReceiveCmd(int cs);
	void		sendMessageToSocket(std::string str, int socket);
	void		cleanOldClient();
	void		catchAllSignal();

	int 			_socket;
	unsigned long	_nbClient;
	std::list<int>	_child;
	Tintin_reporter	_tintin;
};

std::ostream &operator<<(std::ostream &o, Server &c);
