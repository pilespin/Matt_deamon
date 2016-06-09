/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/05 17:50:53 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/08 16:31:58 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"
#include "Server.hpp"

#define ERR_TOO_MANY_CONNECTION 	"Too many connexion on server\n"
#define ERR_TOO_MANY_CONNECTION_LOG	"Connexion rejeted, max number reached"
#define PROTO_ERROR					"Proto error"
#define BIND_ERROR					"Bind error"
#define SOCKET_ERROR				"Socket error"

Server::Server() {
	this->_tintin.newPost("Server ON");
	this->_socket 	= -1;
	this->_iterBuffer = 0;
}

Server::~Server()					{
	this->_tintin.newPost("Server OFF");
}

Server::Server(Server const &src)	{	*this = src;	}

Server	&Server::operator=(Server const &rhs) {

	if (this != &rhs)
	{
		this->_socket = rhs._socket;
		this->_nbClient = rhs._nbClient;
		this->_child = rhs._child;
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
		std::cerr << PROTO_ERROR << std::endl;
		exit(0);
	}
	this->_socket = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->_socket, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		std::cerr << BIND_ERROR << std::endl;
		this->_tintin.newPost(BIND_ERROR);
		this->_socket = -1;
		exit(0);
	}
	if (this->_socket == -1)
		return;
	listen(this->_socket, this->_nbClient);
}

int		Server::ServerAcceptConnexion() {
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	std::string			str;

	if (this->_socket <= 0)
	{
		std::cerr << SOCKET_ERROR << std::endl;
		this->_tintin.newPost(SOCKET_ERROR);
		return (-1);
	}
	cs = accept(this->_socket, (struct sockaddr *)&csin, &cslen);
	return (cs);
}

std::string	Server::ServerReceiveCmd(int cs) {

	std::string		str;
	int 					len	= 0;
	int						i;
	while (1)
	{
		// len = recv(cs, &buf, 1, MSG_DONTWAIT);
		if (this->_iterBuffer)
			i = this->_iterBuffer;
		else
		{
			if (0 >= (len = recv(cs, &this->_buffer, Server::BUFFER, 0)))
			{
				close(cs);
				exit(0);
			}
			this->_buffer[len] = '\0';
			i = 0;
		}
		while (this->_buffer[i])
		{
			if (this->_buffer[i] == '\n')
			{
				this->_iterBuffer = i + 1;
				if (!str.compare("quit"))
				{
					/////////////// killer le pere
					this->_tintin.newPost("Server OFF");
					close(cs);
					exit(0);
				}
				if (str.length() > 0)
					return (str);
				else
					return ("Empty line from user");
			}
			else
				str += this->_buffer[i];
			i++;
		}
		this->_iterBuffer = 0;
	}
	return ("STRANGE ERROR");
}

void	Server::cleanOldClient() {

	std::list<int>::iterator 	i;
	int 						status;

	if (this->_child.size() >= this->_nbClient)
	{
		i = this->_child.begin();
		while (i != this->_child.end())
		{
			if (waitpid(*i, &status, WNOHANG))
				this->_child.erase(i);
			i++;
		}
	}
}

void	Server::sendMessageToSocket(std::string str, int socket) {

	send(socket, str.c_str(), str.length(), 0);
}

static void	signal(int sig) {

	Tintin_reporter 	t;
	std::string			str;

	str += "Signal: " + std::to_string(sig);
	t.newPost(str);
	// exit(sig);
}

void	Server::catchAllSignal() {

	int i = -1;

	while (++i < 50)
		std::signal(i, signal);
}

void	Server::launchServer(int port, unsigned long nbClient) {

	int 	cs;
	int 	pid	= 1;

	this->_nbClient = nbClient;
	this->createServer(port);
	// this->catchAllSignal();

	pid = 1;
	while (pid > 0)
	{
		cs = this->ServerAcceptConnexion();
		if (cs != -1)
		{
			this->cleanOldClient();
			if (this->_child.size() < this->_nbClient)
			{
				pid = fork();
				if (pid > 0)
				{
					this->_child.push_back(pid);
					this->_tintin.newPost("New User Added");
					this->_tintin.newPost("Nbr client: " + std::to_string(this->_child.size()));
					close(cs);
				}
			}
			else
			{
				this->sendMessageToSocket(ERR_TOO_MANY_CONNECTION, cs);
				this->_tintin.newPost(ERR_TOO_MANY_CONNECTION_LOG);
				sleep(1);
				close(cs);
			}
		}
		else
			exit(0);
	}
	while (1)
	{
		this->_tintin.newPost(this->ServerReceiveCmd(cs));
	}
	close(cs);
}
