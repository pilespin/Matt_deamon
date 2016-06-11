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
#include <sys/file.h>

#define ERROR_NEED_ROOT	"You need to be root to perform this action"
#define FILE_LOCK		"matt_daemon.lock"

void deamonize()
{
	if (getuid())
	{
		std::cerr << ERROR_NEED_ROOT << std::endl;
		exit(0);
	}

	setsid();
	int pid;
	if ((pid = fork()) < 0)
	{
		std::cerr << "Fork fail" << std::endl;
		exit(0);
	}
	else if (pid > 0)
		exit(0);
	// chdir("/");
	// umask(0);
	// int id = setsid();
	// std::cerr << "id :" << id << std::endl;

	// close(STDIN_FILENO);
	// close(STDOUT_FILENO);
	// close(STDERR_FILENO);

	// while (1) {}
}

void check_no_instance_running()
{
	int fd;

	fd = open(FILE_LOCK, O_CREAT);
	if (flock(fd, LOCK_EX | LOCK_NB))
	{
		std::cout << "Matt is not available" << std::endl;
		close(fd);
		unlink(FILE_LOCK);
		exit(0);
	}
}

int main()
{
	// int pid;
	// if ((pid = fork()) == 0)
	// {
	//check_no_instance_running();
	// deamonize();

	Server	s;
	std::cout << "Avant launchServer" << std::endl;
	s.launchServer(4240, 2);
	// }
	// while(1) {}
	return(0);
}
