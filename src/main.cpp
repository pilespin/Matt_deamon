/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/30 17:05:03 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/05 18:40:26 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <project.hpp>
#include "Server.hpp"

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
	Server	s;

	// catch_all_signals();

	// int fd;

	// fd = open("matt_daemon.lock", O_RDWR | O_CREAT);
	// if ((flock(fd, LOCK_EX)) == 0)
	// 	std::cout << "Locked" << std::endl;
	// else
	// 	std::cout << "Not available" << std::endl;

	s.lunchServer(4242, 3);
}
