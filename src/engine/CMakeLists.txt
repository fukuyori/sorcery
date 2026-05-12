add_library(sorcery_engine)

target_include_directories(sorcery_engine PUBLIC
	${CMAKE_SOURCE_DIR}/inc
	${SORCERY_THIRD_PARTY_INCLUDE_DIRS}
)

target_link_libraries(sorcery_engine PUBLIC
	dear_imgui
	Freetype::Freetype
	${SORCERY_SQLITE_TARGET}
	${SORCERY_UUID_LIBRARIES}
	${SORCERY_SDL2_TARGETS}
)

target_sources(sorcery_engine PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/engine.cpp
)
