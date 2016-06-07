/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/05 17:50:53 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/05 19:39:56 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

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
	static const int	BUFFER = 1000;

private:
	void		createServer(int port);
	int 		ServerAcceptConnexion();
	std::string	ServerReceiveCmd(int cs);
	void		sendMessageToSocket(std::string str, int socket);
	void		cleanOldClient();

	int 			_socket;
	unsigned long	_nbClient;
	std::list<int>	child;
	char						_buffer[Server::BUFFER + 1];
	int							_iterBuffer;
};

std::ostream &operator<<(std::ostream &o, Server &c);
