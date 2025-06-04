NAME				:=	mod1

BUILD				:=	build/command-line

CMAKE_BUILD_TYPE	:=	Debug

CACHE				:=	CMakeCache.txt

IGN_LEAKS			:=	valgrind_ignore_leaks.txt

VALGRIND			:=	valgrind \
						--track-fds=yes --show-mismatched-frees=yes --read-var-info=yes --track-origins=yes -s\
						--leak-check=full --show-leak-kinds=all\
						--suppressions=$(IGN_LEAKS)\

.PHONY				:	all
all					:	$(BUILD)/$(CACHE)
					cmake --build $(BUILD)

$(BUILD)/$(CACHE)	:
					mkdir -p $(@D)
					cmake -B $(BUILD) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)

.PHONY				:	run
run					:	all
					./$(BUILD)/$(NAME)

.PHONY				:	leaks
leaks				:	all $(IGN_LEAKS)
					$(VALGRIND) ./$(BUILD)/$(NAME)

.PHONY				:	clean
clean				:
					cmake --build $(BUILD) --target $@

.PHONY				:	fclean
fclean				:
					rm -rf $(BUILD) $(IGN_LEAKS)

.PHONY				:	re
re					:	fclean all
