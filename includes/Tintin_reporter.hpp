/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/04 15:17:47 by pilespin          #+#    #+#             */
/*   Updated: 2016/06/04 20:08:47 by pilespin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

class Tintin_reporter {

public:
	Tintin_reporter();
	~Tintin_reporter();
	Tintin_reporter(Tintin_reporter const &src);
	Tintin_reporter &operator=(Tintin_reporter const &rhs);

	int			getValue() const;
	void		newPost(std::string post, std::string cat, bool eol);
	std::string	getTime();

private:
	int _val;
};

std::ostream &operator<<(std::ostream &o, Tintin_reporter &c);
