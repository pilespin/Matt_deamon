/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/05 17:50:53 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/24 14:47:53 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"
#include "Server.hpp"
#include <string.h>

#define ERR_TOO_MANY_CONNECTION_LOG	"Connexion rejeted, max number reached"
#define PROTO_ERROR					"Proto error"
#define BIND_ERROR					"Bind error"
#define SOCKET_ERROR				"Socket error"

static void	quit(int sig) {

	Tintin_reporter 	t;

	t.newPost("Exiting ...", "INFO", 1);
	t.newPost("Server OFF", "INFO", 1);
	unlink(FILE_LOCK);
	exit(sig);
}

Server::Server() {
	int				i;
	struct rlimit 	rlp;

	if (getrlimit(RLIMIT_NOFILE, &rlp) == -1)
	{
		quit(0);
	}
	this->_maxFd = rlp.rlim_cur;
	this->_fds = new t_fd[this->_maxFd];
	i = 0;
	while (i < this->_maxFd)
	{
		this->_fds[i].type = FD_FREE;
		i++;
	}
	this->_tintin.newPost("Server ON", "INFO", 1);
	this->_socket = -1;
	this->_nbClientMax = 0;
	this->_nbClientConnect = 0;
}

Server::~Server()					{
	this->_tintin.newPost("Server OFF", "INFO", 1);
}

Server::Server(Server const &src)	{	*this = src;	}

Server	&Server::operator=(Server const &rhs) {

	if (this != &rhs)
	{
		this->_socket = rhs._socket;
		this->_nbClientMax = rhs._nbClientMax;
		this->_nbClientConnect = rhs._nbClientConnect;
		this->_fds = rhs._fds;
		this->_port = rhs._port;
		this->_maxFd = rhs._maxFd;
		this->_fdRead = rhs._fdRead;
		this->_fdWrite = rhs._fdWrite;
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
		this->_tintin.newPost(PROTO_ERROR, "ERROR", 1);
		quit(0);
	}
	this->_socket = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->_socket, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		this->_tintin.newPost(BIND_ERROR, "ERROR", 1);
		this->_socket = -1;
		quit(0);
	}
	if (this->_socket == -1)
		return;
	listen(this->_socket, this->_nbClientMax);
	this->_fds[this->_socket].type = FD_SERV;
}

void	Server::check_fd(int r)
{
	int	i;

	i = 0;
	while ((i < this->_maxFd) && (r > 0))
	{
		if (FD_ISSET(i, &this->_fdRead))
		{
			if (this->_fds[i].type == FD_CLIENT)
				this->client_read(i);
			else if (this->_fds[i].type == FD_SERV)
				this->ServerAcceptConnexion();
		}
		if (FD_ISSET(i, &this->_fdRead) ||
			FD_ISSET(i, &this->_fdWrite))
			r--;
		i++;
	}
}

void	Server::init_fd()
{
	int	i;
	int	max;
	int	r;
	unsigned long tmp_nb_client		=	this->_nbClientConnect;

	i = 0;
	max = 0;
	FD_ZERO(&this->_fdRead);
	FD_ZERO(&this->_fdWrite);
	this->_nbClientConnect = 0;
	while (i < this->_maxFd)
	{
		if (this->_fds[i].type != FD_FREE)
		{
			FD_SET(i, &this->_fdRead);
			max = i;
		}
		if (this->_fds[i].type == FD_CLIENT)
			this->_nbClientConnect++;
		i++;
	}
	if (tmp_nb_client > this->_nbClientConnect)
	{
		this->_tintin.newPost("Client Disconected" , "INFO", 1);
		this->_tintin.newPost("Alive: " + std::to_string(this->_nbClientConnect), "INFO", 1);
	}
	FD_SET(this->_socket, &this->_fdRead);
	r = select(max + 1, &this->_fdRead, &this->_fdWrite, NULL, NULL);
	this->check_fd(r);
}

void		Server::ServerAcceptConnexion() {
	int					cs;
	socklen_t			cslen;
	struct sockaddr_in	csin;
	std::string			str;

	if (this->_socket <= 0)
	{
		this->_tintin.newPost(SOCKET_ERROR, "ERROR", 1);
		quit(0);
	}
	cslen = sizeof(csin);
	cs = accept(this->_socket, (struct sockaddr *)&csin, &cslen);
	if (this->_nbClientConnect < this->_nbClientMax)
	{
		char buffer[40];
		inet_ntop(AF_INET, &(csin.sin_addr), buffer, 40);
		std::string ip(buffer);

		this->_tintin.newPost("New client, ip: " + ip, "INFO", 1);
		this->_tintin.newPost("Alive: " + std::to_string(this->_nbClientConnect + 1), "INFO", 1);
		this->_fds[cs].type = FD_CLIENT;
	}
	else
	{
		this->_tintin.newPost(ERR_TOO_MANY_CONNECTION_LOG, "ERROR", 1);
		close(cs);
	}
}

void Server::client_read(int cs)
{
	int	len;

	len = recv(cs, &this->_buffer, Server::BUFFER, 0);
	if (len <= 0)
	{
		close(cs);
		this->_fds[cs].type = FD_FREE;
	}
	else
	{
		this->_buffer[len] = '\0';
		this->_fds[cs].str += this->_buffer;
		if (this->_fds[cs].str.find('\n') != std::string::npos)
		{
			if (this->_fds[cs].str == "quit\n")
			{
				this->CloseFd();
			}
			this->_tintin.newPost(this->_fds[cs].str, "LOG", 0);
			this->_fds[cs].str.clear();
		}
	}
}

void	Server::CloseFd()
{
	int	i;

	i = 0;
	while (i < this->_maxFd)
	{
		if (this->_fds[i].type == FD_CLIENT)
		{
			close(i);
		}
		i++;
	}
	quit(0);
}

static void	signal(int sig) {

	Tintin_reporter 	t;
	std::string			str;

	str += "Signal: " + std::to_string(sig);
	t.newPost(str, "INFO", 1);
	quit(sig);
}

void	Server::catchAllSignal() {

	int i = -1;

	while (++i < 50)
		std::signal(i, signal);
}

void	Server::launchServer(int port, unsigned long nbClientMax) {

	this->_nbClientMax = nbClientMax;
	this->createServer(port);
	this->catchAllSignal();
	while (1)
	{
		this->init_fd();
	}
}
