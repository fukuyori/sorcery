# Paths returned by GET_RUNTIME_DEPENDENCIES keep the casing reported by the
# OS (for example "C:/WINDOWS/system32"), so the exclude patterns must not
# depend on a particular casing. CMake regular expressions have no
# case-insensitive flag, hence the explicit character classes below.
if(POLICY CMP0207)
	cmake_policy(SET CMP0207 NEW)
endif()

set(SORCERY_WINDOWS_DIR_REGEX "[Ww][Ii][Nn][Dd][Oo][Ww][Ss]")
set(SORCERY_SYSTEM32_DIR_REGEX "[Ss][Yy][Ss][Tt][Ee][Mm]32")
set(SORCERY_SYSWOW64_DIR_REGEX "[Ss][Yy][Ss][Ww][Oo][Ww]64")

file(GET_RUNTIME_DEPENDENCIES
	EXECUTABLES "${SORCERY_EXE}"

	DIRECTORIES
		"${SORCERY_RUNTIME_DIR}"

	RESOLVED_DEPENDENCIES_VAR resolved_dependencies
	UNRESOLVED_DEPENDENCIES_VAR unresolved_dependencies

	PRE_EXCLUDE_REGEXES
		"api-ms-win-.*"
		"ext-ms-.*"

	POST_EXCLUDE_REGEXES
		".*${SORCERY_WINDOWS_DIR_REGEX}[/\\\\]${SORCERY_SYSTEM32_DIR_REGEX}[/\\\\].*"
		".*${SORCERY_WINDOWS_DIR_REGEX}[/\\\\]${SORCERY_SYSWOW64_DIR_REGEX}[/\\\\].*"
)

foreach(dll IN LISTS resolved_dependencies)
	file(COPY "${dll}" DESTINATION "${SORCERY_DEST_DIR}")
endforeach()

if(unresolved_dependencies)
	message(WARNING
		"Unresolved runtime dependencies: ${unresolved_dependencies}")
endif()