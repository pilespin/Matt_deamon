/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/04 13:30:26 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/04 14:44:07 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket() 					{	this->_socket = -1;	}

Socket::~Socket()					{}

Socket::Socket(Socket const &src)	{	*this = src;	}

Socket	&Socket::operator=(Socket const &rhs) {

	if (this != &rhs)
	{
		this->_socket = rhs._socket;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &o, Socket &c) {
	o << "Socket: " << c.getSocket() << " ";
	return (o);
}
///////////////////////////////////////////////////////////////////////////////
int		Socket::getSocket() const	{	return (this->_socket);	}
///////////////////////////////////////////////////////////////////////////////
void	Socket::createServer(int port) {

	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (!proto)
	{
		std::cout << "proto error" << std::endl;
		return;
	}
	this->_socket = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->_socket, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		std::cout << "bind error" << std::endl;
		this->_socket = -1;
		return;
	}
	if (this->_socket == -1)
		return;
	listen(this->_socket, 1);
}

int		Socket::ServerAcceptConnexion() {
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	std::string			str;

	if (this->_socket <= 0)
	{
		std::cout << "socket error" << std::endl;
		return (-1);
	}
	cs = accept(this->_socket, (struct sockaddr *)&csin, &cslen);
	return (cs);
}

std::string	Socket::ServerReceiveCmd(int cs) {

	std::string		str;
	char 			buf[2];
	int 			len	= 0;
	while (1)
	{
		// len = recv(cs, &buf, 1, MSG_DONTWAIT);
		len = recv(cs, &buf, 1, 0);
		if (len == 1)
		{
			buf[1] = '\0';
			if (buf[0] == '\n' && str.length() == 0)
			{
			}
			else if (buf[0] == '\n' && str.length() > 0)
			{
				return (str);
			}
			else
				str += buf;
			if (!str.compare("quit"))
			{
				close(cs);
				exit(0);
			}
		}
		else
		{
			close(cs);
			exit(0);
		}
	}
	return (str);
}
