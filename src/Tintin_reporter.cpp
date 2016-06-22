/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/04 15:29:53 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/07 17:22:42 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"

#define LOG_PATH					 "/var/log/"
#define LOG_FOLDER					 "matt_daemon/"
#define LOG_FILE					 "matt_daemon.log"

Tintin_reporter::Tintin_reporter() 						{	this->_val = 0;	}

Tintin_reporter::~Tintin_reporter()						{}

Tintin_reporter::Tintin_reporter(Tintin_reporter const &src)	{	*this = src;	}

Tintin_reporter	&Tintin_reporter::operator=(Tintin_reporter const &rhs) {

	if (this != &rhs)
	{
		this->_val = rhs._val;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &o, Tintin_reporter &c) {
	o << "Tintin_reporter: " << c.getValue() << " ";
	return (o);
}

///////////////////////////////////////////////////////////////////////////////
int		Tintin_reporter::getValue() const	{	return (this->_val);	}
///////////////////////////////////////////////////////////////////////////////

void	Tintin_reporter::newPost(std::string post, std::string cat, bool eol) {

	std::string path = LOG_PATH;
	path += LOG_FOLDER; 
	path += LOG_FILE;

	std::string folder = LOG_PATH;
	folder += LOG_FOLDER;

	struct stat status;
	stat(folder.c_str(), &status);
	if (status.st_mode & S_IFDIR)
	{}
	else
		mkdir(folder.c_str(), 0755);

	std::ofstream Log(path, std::ios::app);
	Log << this->getTime() << " [ " << cat << " ] - Matt_daemon: " << post;
	if (eol)
		Log << std::endl;
}

std::string	Tintin_reporter::getTime() {

	std::ostringstream	stream;
	time_t 				now 	= std::time(0);
	tm 					*time 	= std::localtime(&now);

	stream << "[";
	if (time->tm_mday < 10)
		stream << "0";
	stream << time->tm_mday << "/";
	if (1 + time->tm_mon < 10)
		stream << "0";
	stream << 1 + time->tm_mon << "/";
	stream << 1900 + time->tm_year << " - ";
	if (time->tm_hour < 10)
		stream << "0";
	stream << time->tm_hour << ":";
	if (time->tm_min < 10)
		stream << "0";
	stream << time->tm_min << ":";
	if (time->tm_sec < 10)
		stream << "0";
	stream << time->tm_sec << "]";
	return (stream.str());

// struct tm {
//   int tm_sec;   // seconds of minutes from 0 to 61
//   int tm_min;   // minutes of hour from 0 to 59
//   int tm_hour;  // hours of day from 0 to 24
//   int tm_mday;  // day of month from 1 to 31
//   int tm_mon;   // month of year from 0 to 11
//   int tm_year;  // year since 1900
//   int tm_wday;  // days since sunday
//   int tm_yday;  // days since January 1st
//   int tm_isdst; // hours of daylight savings time
// }
}
