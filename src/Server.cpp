/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/05 17:50:53 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/05 20:33:26 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"
#include "Server.hpp"

#define ERR_TOO_MANY_CONNECTION 	"Too many connexion on server\n"
#define PROTO_ERROR					"proto error"
#define BIND_ERROR					"bind error"
#define SOCKET_ERROR				"socket error"

Server::Server() {
	this->_socket 	= -1;
	this->_iterBuffer = 0;
}

Server::~Server()					{}

Server::Server(Server const &src)	{	*this = src;	}

Server	&Server::operator=(Server const &rhs) {

	if (this != &rhs)
	{
		this->_socket = rhs._socket;
		this->_nbClient = rhs._nbClient;
		this->child = rhs.child;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &o, Server &c) {
	o << "Socket: " << c.getSocket() << " ";
	return (o);
}
///////////////////////////////////////////////////////////////////////////////
int		Server::getSocket() const	{	return (this->_socket);	}
///////////////////////////////////////////////////////////////////////////////
void	Server::createServer(int port) {

	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (!proto)
	{
		std::cout << PROTO_ERROR << std::endl;
		return;
	}
	this->_socket = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->_socket, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		std::cout << BIND_ERROR << std::endl;
		this->_socket = -1;
		return;
	}
	if (this->_socket == -1)
		return;
	listen(this->_socket, 1);
}

int		Server::ServerAcceptConnexion() {
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	std::string			str;

	if (this->_socket <= 0)
	{
		std::cout << SOCKET_ERROR << std::endl;
		return (-1);
	}
	cs = accept(this->_socket, (struct sockaddr *)&csin, &cslen);
	return (cs);
}

std::string	Server::ServerReceiveCmd(int cs) {

	std::string		str;
	int 					len	= 0;
	int						i = 0;
	while (1)
	{
		// len = recv(cs, &buf, 1, MSG_DONTWAIT);
		len = recv(cs, &this->_buffer, Server::BUFFER, 0);
		this->_buffer[len] = '\0';
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

void	Server::cleanOldClient() {

	std::list<int>::iterator 	i;
	int 						status;

	if (this->child.size() >= this->_nbClient)
	{
		i = this->child.begin();
		while (i != this->child.end())
		{
			if (waitpid(*i, &status, WNOHANG))
				this->child.erase(i);
			i++;
		}
	}
}

void	Server::sendMessageToSocket(std::string str, int socket) {

	send(socket, str.c_str(), str.length(), 0);
}

void	Server::lunchServer(int port, unsigned long nbClient) {

	Tintin_reporter 			t;
	int 						cs;
	int 						pid	= 1;

	this->_nbClient = nbClient;
	this->createServer(port);

	pid = 1;
	while (pid > 0)
	{
		cs = this->ServerAcceptConnexion();
		if (cs != -1)
		{
			this->cleanOldClient();
			if (this->child.size() < this->_nbClient)
			{
				pid = fork();
				if (pid > 0)
				{
					this->child.push_back(pid);
					close(cs);
				}
			}
			else
			{
				this->sendMessageToSocket(ERR_TOO_MANY_CONNECTION, cs);
				close(cs);
			}
		}
		else
			exit(0);
	}
	while (1)
	{
		t.newPost(this->ServerReceiveCmd(cs));
	}
	close(cs);
}
