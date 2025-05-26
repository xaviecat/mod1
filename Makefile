NAME				:=	mod1

BUILD				:=	build/command-line

CMAKE_BUILD_TYPE	:=	Debug

CACHE				:=	CMakeCache.txt

.PHONY				:	all
all					:	$(BUILD)/$(CACHE)
					cmake --build $(BUILD)

$(BUILD)/$(CACHE)	:
					mkdir -p $(@D)
					cmake -B $(BUILD) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)

run					:	all
					./$(BUILD)/$(NAME)

.PHONY				:	clean
clean				:
					cmake --build $(BUILD) --target $@

.PHONY				:	fclean
fclean				:
					rm -rf $(BUILD)

.PHONY				:	re
re					:	fclean all