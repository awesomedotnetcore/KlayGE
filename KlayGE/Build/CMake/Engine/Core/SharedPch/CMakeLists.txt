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
elseif(KLAYGE_PLATFORM_DARWIN OR KLAYGE_PLATFORM_IOS)
	set(shared_pch_output "${header_files}" CACHE INTERNAL "" FORCE)
else()
	set(CXX_COMPILE_FLAGS ${CMAKE_CXX_FLAGS})
	if(CMAKE_BUILD_TYPE)
		string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_CMAKE_BUILD_TYPE)
		list(APPEND CXX_COMPILE_FLAGS ${CMAKE_CXX_FLAGS_${UPPER_CMAKE_BUILD_TYPE}})
	endif()

	get_filename_component(pch_header_name ${header_files} NAME)
	get_filename_component(pch_header_pach ${header_files} PATH)

	set(pch_output "${output_dir}/${lib_name}.gch")

	get_directory_property(directory_flags INCLUDE_DIRECTORIES)

	set(current_binary_dir_included_before_path FALSE)
	foreach(item ${directory_flags})
		if(${item} STREQUAL ${pch_header_pach} AND NOT current_binary_dir_included_before_path)
			message(FATAL_ERROR
				"This is the KLAYGE_ADD_GCC_PRECOMPILED_HEADER function. "
				"CMAKE_CURREN_BINARY_DIR has to mentioned at INCLUDE_DIRECTORIES's argument list before ${pch_header_pach}, where ${pch_header_name} is located"
			)
		endif()

		if(${item} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
			set(current_binary_dir_included_before_path TRUE)
		endif()

		list(APPEND CXX_COMPILE_FLAGS "-I\"${item}\"")
	endforeach()

	get_target_property(PIC_OPTION ${lib_name} POSITION_INDEPENDENT_CODE)
	if(PIC_OPTION AND CMAKE_CXX_COMPILE_OPTIONS_PIC)
		list(APPEND CXX_COMPILE_FLAGS "${CMAKE_CXX_COMPILE_OPTIONS_PIC}")
	endif()

	get_directory_property(directory_flags COMPILE_DEFINITIONS)
	foreach(item ${directory_flags})
		list(APPEND CXX_COMPILE_FLAGS "-D${item}")
	endforeach()

	separate_arguments(CXX_COMPILE_FLAGS)
	
	add_custom_command(OUTPUT ${pch_output}
		COMMAND ${CMAKE_CXX_COMPILER} ${CXX_COMPILE_FLAGS} -Wno-error -x c++-header -o ${pch_output} ${header_files}
	)
	add_custom_target(${lib_name}_gch
		DEPENDS ${pch_output}
	)
	add_dependencies(${lib_name} ${lib_name}_gch)

	set(shared_pch_output ${pch_output} CACHE INTERNAL "" FORCE)
endif()
