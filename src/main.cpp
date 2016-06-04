/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/30 17:05:03 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/04 21:08:18 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <project.hpp>
#include "Socket.hpp"
#include "Tintin_reporter.hpp"

void signal(int sig)
{
	std::cout << "Signal:" << sig << std::endl;
	// exit(sig);
}

void catch_all_signals()
{
	int i;
	i = -1;
	while (++i < 50)
		std::signal(i, signal);
}

int main()
{
	Socket s;
	Tintin_reporter t;
	// catch_all_signals();

	std::string str;

	int cs;
	s.createServer(4242);
	std::cout << "Started " << s << std::endl;

	int pid = 1;
	while (pid > 0)
	{
		cs = s.ServerAcceptConnexion();
		if (cs != -1)
		{
			pid = fork();
			if (pid > 0)
				close(cs);
		}
		else
			exit(0);
	}
	while (1)
	{
		str = s.ServerReceiveCmd(cs);
		t.newPost(str);
	}
	close(cs);
	exit(0);

	std::cout << "END" << std::endl;
	return (0);
}
