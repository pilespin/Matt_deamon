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
	int	i;
	struct rlimit rlp;
	if (getrlimit(RLIMIT_NOFILE, &rlp) == -1)
	{
		exit(0);
	}
	this->_maxFd = rlp.rlim_cur;
	this->_fds = new t_fd[this->_maxFd];
	i = 0;
	while (i < this->_maxFd)
	{
		this->_fds[i].type = FD_FREE;
		i++;
	}
	this->_tintin.newPost("Server ON", "INFO");
	this->_socket 	= -1;
	this->_nbClientMax = 0;
	this->_nbClientConnect = 0;
}

Server::~Server()					{
	this->_tintin.newPost("Server OFF", "INFO");
}

Server::Server(Server const &src)	{	*this = src;	}

Server	&Server::operator=(Server const &rhs) {

	if (this != &rhs)
	{
		this->_socket = rhs._socket;
		this->_nbClientMax = rhs._nbClientMax;
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
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->_socket, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		std::cerr << BIND_ERROR << std::endl;
		this->_tintin.newPost(BIND_ERROR, "ERROR");
		this->_socket = -1;
		exit(0);
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
      if (FD_ISSET(i, &this->_fdWrite))
				this->client_write(i);
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
		FD_SET(this->_socket, &this->_fdRead);
		r = select(max + 1, &this->_fdRead, &this->_fdWrite, NULL,NULL);
		this->check_fd(r);
}

void		Server::ServerAcceptConnexion() {
	int					cs;
	socklen_t		cslen;
	struct sockaddr_in	csin;
	std::string			str;

	if (this->_socket <= 0)
	{
		std::cerr << SOCKET_ERROR << std::endl;
		this->_tintin.newPost(SOCKET_ERROR, "ERROR");
		exit(0);
	}
	cslen = sizeof(csin);
	cs = accept(this->_socket, (struct sockaddr *)&csin, &cslen);
	if (this->_nbClientConnect < this->_nbClientMax)
	{
		this->_fds[cs].type = FD_CLIENT;
	}
	else
	{
		//nbre de clients max
		close(cs);
	}
}

void	Server::client_write(int cs)
{
	(void)cs;
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
			//Kill matt_daemon
			if (this->_fds[cs].str == "quit\n")
			{
				this->CloseFd();
			}
			this->_tintin.newPost(this->_fds[cs].str, "LOG");
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
		this->_tintin.newPost("Quitting.", "INFO");
		exit(0);
	}
}
static void	signal(int sig) {

	Tintin_reporter 	t;
	std::string			str;

	str += "Signal: " + std::to_string(sig);
	t.newPost(str, "INFO");
	// exit(sig);
}

void	Server::catchAllSignal() {

	int i = -1;

	while (++i < 50)
		std::signal(i, signal);
}

void	Server::launchServer(int port, unsigned long nbClientMax) {

	this->_nbClientMax = nbClientMax;
	this->createServer(port);
	// this->catchAllSignal();
	while (1)
	{
		this->init_fd();
	}
	sleep(1);
}
