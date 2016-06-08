/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/30 17:05:03 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/08 19:07:09 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <project.hpp>
#include "Server.hpp"

#include <errno.h>  

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
	// int fd2;
	// int ret;

	// fd = open("matt_daemon.lock", O_CREAT);
	// ret = flock(fd, LOCK_EX);
	// fd2 = open("matt_daemon.lock", O_CREAT);

	// // FILE *fp = fopen("matt_daemon.lock", "r+");
	
	// // flock(fileno(fp), LOCK_EX);

	// // fd = open("matt_daemon.lock", O_RDWR | O_CREAT, 0777);
	// std::cout << "fd: " << fd << std::endl;

	// // ret = flock(fd, LOCK_EX);
	// std::cout << "flock: " << ret << std::endl;

	// // fd2 = open("matt_daemon.lock", O_RDWR | O_CREAT, 0777);
	// std::cout << "fd2: " << fd2 << std::endl;

	// if (ret == -1)
	// {
		// std::cout << "ERROR" << ret << std::endl;
	// 	perror("Error");
	// 	// std::cout << "ERROR" <<  errno() << ret << std::endl;
	// }
	// if ((ret = flock(fd, LOCK_EX)))
	// 	std::cout << "Locked" << ret << std::endl;
	// else
	// 	std::cout << "Not available" << ret << std::endl;
	s.launchServer(4243, 3);
}
