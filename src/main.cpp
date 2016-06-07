/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/30 17:05:03 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/07 18:48:11 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <project.hpp>
#include "Server.hpp"

#define ERROR_NEED_ROOT	"You need to be root to perform this action"

void deamonize()
{
	// if (getuid())
	// {
	// 	std::cerr << ERROR_NEED_ROOT << std::endl;
	// 	exit(0);
	// }
	int pid;

	if ((pid = fork()) < 0)
	{
		std::cerr << "Fork fail" << std::endl;
		exit(0);
	}
	else if (pid > 0)
		exit(0);
	// umask(0);
	// int id = setsid();
	// std::cerr << "id :" << id << std::endl;

	// close(STDIN_FILENO);
	// close(STDOUT_FILENO);
	// close(STDERR_FILENO);

	// while (1) {}
}

int main()
{
	//deamonize();
	Server	s;

	// int fd;

	// fd = open("matt_daemon.lock", O_RDWR | O_CREAT);
	// if ((flock(fd, LOCK_EX)) == 0)
	// 	std::cout << "Locked" << std::endl;
	// else
	// 	std::cout << "Not available" << std::endl;
	s.lunchServer(4242, 3);
}
