NAME				:=	mod1

BUILD				:=	build/command-line

WEB					:=	build/wasm

SHELL				:=	/bin/bash

CMAKE_BUILD_TYPE	:=	Debug

CACHE				:=	CMakeCache.txt

IGN_LEAKS			:=	valgrind_ignore_leaks.txt

VALGRIND			:=	valgrind -s\
						--track-fds=yes --show-mismatched-frees=yes\
						--read-var-info=yes --track-origins=yes\
						--leak-check=full --show-leak-kinds=all\
						--suppressions=$(IGN_LEAKS)\

.PHONY				:	all
all					:	$(BUILD)/$(CACHE)
					cmake --build $(BUILD)

.PHONY				:	web
web					:	$(WEB)/$(CACHE)
					cmake --build $(WEB) # -- VERBOSE=1

$(BUILD)/$(CACHE)	:
					mkdir -p $(@D)
					cmake -B $(BUILD) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)

$(WEB)/$(CACHE)		:
					mkdir -p $(@D)
					source $$HOME/goinfre/emsdk/emsdk_env.sh && \
					$$HOME/goinfre/Qt/6.9.1/wasm_singlethread/bin/qt-cmake \
					-DQT_CHAINLOAD_TOOLCHAIN_FILE=$$HOME/goinfre/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
					-B $(WEB)

.PHONY				:	run
run					:	all
					./$(BUILD)/$(NAME)

.PHONY				:	wrun
wrun					:	web
					$$HOME/goinfre/emsdk/upstream/emscripten/emrun ./$(WEB)/$(NAME).html

.PHONY				:	leaks
leaks				:	all $(IGN_LEAKS)
					$(VALGRIND) ./$(BUILD)/$(NAME)

.PHONY				:	clean
clean				:
					cmake --build $(BUILD) --target $@
					cmake --build $(WEB) --target $@

.PHONY				:	fclean
fclean				:
					rm -rf $(BUILD) $(WEB)

.PHONY				:	re
re					:	fclean all
