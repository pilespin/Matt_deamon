
#pragma once

#include <iostream>

class NewName {

public:
	NewName();
	~NewName();
	NewName(NewName const &src);
	NewName &operator=(NewName const &rhs);

	int			getValue() const;
	void		empty();

private:
	int _val;
};

std::ostream &operator<<(std::ostream &o, NewName &c);
