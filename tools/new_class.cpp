
#include "NewName.hpp"

NewName::NewName() 						{	this->_val = 0;	}

NewName::~NewName()						{}

NewName::NewName(NewName const &src)	{	*this = src;	}

NewName	&NewName::operator=(NewName const &rhs) {

	if (this != &rhs)
	{
		this->_val = rhs._val;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &o, NewName &c) {
	o << "NewName: " << c.getValue() << " ";
	return (o);
}
///////////////////////////////////////////////////////////////////////////////
int		NewName::getValue() const	{	return (this->_val);	}
///////////////////////////////////////////////////////////////////////////////
void	NewName::empty() {

}
