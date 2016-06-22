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

#define ERROR_NEED_ROOT		"You need to be root to perform this action"
#define ERROR_NEED_ROOT_LOG	"Attempt to launch whitout root acces"
#define ERROR_LOCKED_USER	"Matt is not available"
#define ERROR_LOCKED_LOG	"Attempt to launch a new instance"

void deamonize()
{
	int pid;
	if ((pid = fork()) < 0)
	{
		std::cerr << "Fork fail" << std::endl;
		exit(0);
	}
	else if (pid > 0)
		exit(0);
	chdir("/");
	setsid();
	umask(0);

	// close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	Tintin_reporter 	t;
	t.newPost("Demonized", "INFO", 1);
}

void check_no_instance_running()
{
	int fd;

	fd = open(FILE_LOCK, O_CREAT);
	if (flock(fd, LOCK_EX | LOCK_NB))
	{
		std::cout << ERROR_LOCKED_USER << std::endl;
		Tintin_reporter 	t;
		t.newPost(ERROR_LOCKED_LOG, "ERROR", 1);
		close(fd);
		exit(0);
	}
}

int check_root_acces()
{
	if (getuid())
	{
		std::cerr << ERROR_NEED_ROOT << std::endl;
		Tintin_reporter 	t;
		t.newPost(ERROR_NEED_ROOT_LOG, "ERROR", 1);
		exit(0);
	}
	return (1);
}

int main()
{
	check_root_acces();
	check_no_instance_running();
	deamonize();

	Server	s;
	s.launchServer(4242, 3);

	return(0);
}
