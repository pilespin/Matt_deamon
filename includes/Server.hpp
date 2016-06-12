/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/05 17:50:53 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/08 16:11:55 by pilespin         ###   ########.fr       */
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
#	include <sys/resource.h>

# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

typedef struct s_fd
{
	int	type;
	std::string	str;
}							t_fd;

class Server {

public:
	Server();
	~Server();
	Server(Server const &src);
	Server &operator=(Server const &rhs);

	void		launchServer(int port, unsigned long nbClient);
	int			getSocket() const;

	static const int	BUFFER = 4;

private:
	void		createServer(int port);
	void 		ServerAcceptConnexion();
	void		cleanOldClient();
	void		catchAllSignal();
	void		init_fd();
	void		check_fd(int r);
 	void		client_read(int cs);
	void		client_write(int cs);
	void		CloseFd();

	int 						_socket;
	unsigned long		_nbClientMax;
	unsigned long		_nbClientConnect;
	t_fd							*_fds;
	int							_port;
	int							_maxFd;
	char						_buffer[Server::BUFFER + 1];
	fd_set					_fdRead;
	fd_set					_fdWrite;

	Tintin_reporter	_tintin;

};

std::ostream &operator<<(std::ostream &o, Server &c);
