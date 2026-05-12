add_library(sorcery_frontend)

target_include_directories(sorcery_frontend PUBLIC
	${CMAKE_SOURCE_DIR}/inc
	${SORCERY_THIRD_PARTY_INCLUDE_DIRS}
)

target_link_libraries(sorcery_frontend PUBLIC
	dear_imgui
	${SORCERY_SDL2_TARGETS}
)

target_sources(sorcery_frontend PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/atlas.cpp
	${CMAKE_CURRENT_LIST_DIR}/bestiary.cpp
	${CMAKE_CURRENT_LIST_DIR}/compendium.cpp
	${CMAKE_CURRENT_LIST_DIR}/license.cpp
	${CMAKE_CURRENT_LIST_DIR}/mainmenu.cpp
	${CMAKE_CURRENT_LIST_DIR}/museum.cpp
	${CMAKE_CURRENT_LIST_DIR}/options.cpp
	${CMAKE_CURRENT_LIST_DIR}/spellbook.cpp
	${CMAKE_CURRENT_LIST_DIR}/splash.cpp
)
