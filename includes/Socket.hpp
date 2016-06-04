/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/30 17:15:22 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/04 13:13:41 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

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

class Socket {

public:
	Socket();
	~Socket();
	Socket(Socket const &src);
	Socket &operator=(Socket const &rhs);

	void		createServer(int port);
	int 		ServerAcceptConnexion();
	std::string	ServerReceiveCmd(int cs);

	int			getSocket() const ;

private:
	int _socket;
};

std::ostream &operator<<(std::ostream &o, Socket &c);

#endif
