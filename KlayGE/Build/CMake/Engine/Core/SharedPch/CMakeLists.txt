set(lib_name KlayGE_Shared_pch)

set(pch_header "KlayGE/KlayGE.hpp")
set(pch_source "${KLAYGE_PROJECT_DIR}/Core/Src/Base/KlayGE.cpp")

set(source_files ${pch_source})
set(header_files ${KLAYGE_PROJECT_DIR}/Core/Include/${pch_header})

source_group("Source Files" FILES ${source_files})
source_group("Header Files" FILES ${header_files})

include_directories(${Boost_INCLUDE_DIR})
include_directories(${KLAYGE_PROJECT_DIR}/../KFL/include)
include_directories(${KLAYGE_PROJECT_DIR}/Core/Include)
include_directories(${EXTRA_INCLUDE_DIRS})
if(KLAYGE_PLATFORM_ANDROID)
	include_directories(${KLAYGE_PROJECT_DIR}/../External/android_native_app_glue)
endif()

add_library(${lib_name} STATIC
	${source_files} ${header_files}
)

set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})

set_target_properties(${lib_name} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${KLAYGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${KLAYGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${KLAYGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${KLAYGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${KLAYGE_OUTPUT_DIR}
	PROJECT_LABEL ${lib_name}
	DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
	OUTPUT_NAME ${lib_name}${KLAYGE_OUTPUT_SUFFIX}
	FOLDER "KlayGE/Engine/Core"
)

target_link_libraries(${lib_name})

add_dependencies(AllInEngine ${lib_name})

if(KLAYGE_COMPILER_MSVC)
	set(pch_output "${output_dir}/${lib_name}.pch")
	set(pdb_output "${output_dir}/vc${KLAYGE_COMPILER_VERSION}.pdb")

	set_target_properties(${lib_name} PROPERTIES COMPILE_FLAGS "/Fd\"${pdb_output}\"")
	set_source_files_properties(${pch_source} PROPERTIES COMPILE_FLAGS "/Yc\"${pch_header}\" /Fp\"${pch_output}\"")

	set(shared_pch_header ${pch_header} CACHE INTERNAL "" FORCE)
	set(shared_pch_output ${pch_output} CACHE INTERNAL "" FORCE)
	set(shared_pch_pdb ${pdb_output} CACHE INTERNAL "" FORCE)
endif()
