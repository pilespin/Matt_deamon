# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pilespin <pilespin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/15 18:31:49 by pilespin          #+#    #+#              #
#    Updated: 2016/06/04 20:30:06 by pilespin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all compil clean fclean re run leaks incl
	
NAME	=	Matt_daemon

CC		=	g++
FLAGS	=	-Wall -Wextra -Werror

SDIR	=	src/
HDIR	=	includes/
ODIR	=	obj/
F_EXT	=	cpp
H_EXT	=	hpp
FOLDER	=	-I $(HDIR)

SRCA	=	$(shell cd $(SDIR) && ls -1 *.$(F_EXT))

SRCH	=	$(shell cd $(HDIR) && ls -1 *.$(H_EXT))

SRCO	=	$(SRCA:$(F_EXT)=.o)

SRC 	=	$(patsubst %.$(F_EXT), $(SDIR)%.$(F_EXT), $(SRCA))
HDR		=	$(patsubst %.$(H_EXT), $(HDIR)%.$(H_EXT), $(SRCH))
OBJ		=	$(patsubst %.$(F_EXT), $(ODIR)%.o, $(SRCA))

all: compil

compil:
	@mkdir -p $(ODIR)
	@echo "\033[32m compiling $(NAME) >>> \c \033[0m"
	@make $(NAME)

$(NAME): $(OBJ) $(SRC)
	@$(CC) -o $(NAME) $(OBJ) $(LIB)
	@echo "\033[37m END $(NAME)\033[0m"

$(ODIR)%.o: $(SDIR)%.$(F_EXT) $(HDR)
	@$(CC) -c $< -o $@ $(FLAGS) $(FOLDER)
	@echo "\033[32m ok \033[33m $@\033[0m"

clean:
	@rm -rf $(ODIR)
	@rm -rf the_norm
	@rm -rf the_leaks

fclean: clean
	@rm -f $(NAME)

re: fclean all

run:
	@clear
	@make
	@./$(NAME)

leaks:
	@zsh -c "while ((1)) {sleep 1 ; leaks $(NAME) | grep -A 1 Leak > the_leaks ; clear && cat the_leaks}"

search:
	@zsh -c "grep -n $(s) $(SDIR)**/*.cpp"

coffee:
	@clear
	@echo "                        \`:.      				"
	@echo "                          \`:.      				"
	@echo "                  .:'     ,::      				"
	@echo "                 .:'      ;:'      				"
	@echo "                 ::      ;:'      				"
	@echo "                  :    .:'      					"
	@echo "                   \`.  :.      					"
	@echo "          _________________________      		"
	@echo "         : _ _ _ _ _ _ _ _ _ _ _ _ :      		"
	@echo "     ,---:\".\".\".\".\".\".\".\".\".\".\".\".\":"
	@echo "    : ,'\"\`::.:.:.:.:.:.:.:.:.:.:.::'      		"
	@echo "    \`.\`.  \`:-===-===-===-===-===-:'      		"
	@echo "      \`.\`-._:                   :      		"
	@echo "        \`-.__\`.               ,'      			"
	@echo "    ,--------\`\"\`-------------'--------.      	"
	@echo "     \`\"--.__                   __.--\"'   	   	"
	@echo "            \`\"\"-------------\"\"'    			"
	@echo "    											  	"
	@sleep 0.5
	@clear
	@echo "                       \`:.      				"
	@echo "                          \`:.      				"
	@echo "                   .:'     ,::      				"
	@echo "                .:'        ;:'      				"
	@echo "                ::        ;:'      				"
	@echo "                 :      .:'      				"
	@echo "                  \`.    :.      				"
	@echo "          _________________________      		"
	@echo "         : _ _ _ _ _ _ _ _ _ _ _ _ :      		"
	@echo "     ,---:\".\".\".\".\".\".\".\".\".\".\".\".\":"
	@echo "    : ,'\"\`::.:.:.:.:.:.:.:.:.:.:.::'      		"
	@echo "    \`.\`.  \`:-===-===-===-===-===-:'      		"
	@echo "      \`.\`-._:                   :      		"
	@echo "        \`-.__\`.               ,'      			"
	@echo "    ,--------\`\"\`-------------'--------.      	"
	@echo "     \`\"--.__                   __.--\"'   	   	"
	@echo "            \`\"\"-------------\"\"'    			"
