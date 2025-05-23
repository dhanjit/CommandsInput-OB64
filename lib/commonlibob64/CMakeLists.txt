cmake_minimum_required(VERSION 3.21)

include("res/cmake/common.cmake")

option(OBSE_SUPPORT_XBYAK "Enables trampoline support for Xbyak." OFF)
option(REX_OPTION_INI "Enables ini config support for REX." OFF)
option(REX_OPTION_JSON "Enables json config support for REX." OFF)
option(REX_OPTION_TOML "Enables toml config support for REX." OFF)

project(
	CommonLibOB64
	LANGUAGES CXX
)

include(GNUInstallDirs)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "in-source builds are not allowed")
endif()

find_package(mmio REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)

include(res/cmake/sourcelist.cmake)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES ${SOURCES}
)

add_library(
	"${PROJECT_NAME}"
	STATIC
	${SOURCES}
	.clang-format
	res/commonlibob64.natvis
)

add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_compile_definitions(
	"${PROJECT_NAME}"
	PUBLIC
		WINVER=0x0601	# windows 7, minimum supported version by fallout 4
		_WIN32_WINNT=0x0601
		"$<$<BOOL:${OBSE_SUPPORT_XBYAK}>:OBSE_SUPPORT_XBYAK=1>"
		"$<$<BOOL:${REX_OPTION_INI}>:REX_OPTION_INI=1>"
		"$<$<BOOL:${REX_OPTION_JSON}>:REX_OPTION_JSON=1>"
		"$<$<BOOL:${REX_OPTION_TOML}>:REX_OPTION_TOML=1>"
)

target_compile_features(
	"${PROJECT_NAME}"
	PUBLIC
		cxx_std_23
)

if (MSVC)
	target_compile_options(
		"${PROJECT_NAME}"
		PRIVATE
			# warnings -> errors
			/we4715	# 'function' : not all control paths return a value

			# disable warnings
			/wd4061 # enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
			/wd4200 # nonstandard extension used : zero-sized array in struct/union
			/wd4201 # nonstandard extension used : nameless struct/union
			/wd4264 # 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
			/wd4265 # 'type': class has virtual functions, but its non-trivial destructor is not virtual; instances of this class may not be destructed correctly
			/wd4266 # 'function' : no override available for virtual member function from base 'type'; function is hidden
			/wd4324 # 'struct_name' : structure was padded due to __declspec(align())
			/wd4371 # 'classname': layout of class may have changed from a previous version of the compiler due to better packing of member 'member'
			/wd4514 # 'function' : unreferenced inline function has been removed
			/wd4582 # 'type': constructor is not implicitly called
			/wd4583 # 'type': destructor is not implicitly called
			/wd4623 # 'derived class' : default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
			/wd4625 # 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
			/wd4626 # 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
			/wd4686 # 'user-defined type' : possible change in behavior, change in UDT return calling convention
			/wd4702 # unreachable code
			/wd4710 # 'function' : function not inlined
			/wd4711 # function 'function' selected for inline expansion
			/wd4820 # 'bytes' bytes padding added after construct 'member_name'
			/wd5026 # 'type': move constructor was implicitly defined as deleted
			/wd5027 # 'type': move assignment operator was implicitly defined as deleted
			/wd5045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
			/wd5053 # support for 'explicit(<expr>)' in C++17 and earlier is a vendor extension
			/wd5204 # 'type-name': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
			/wd5220 # 'member': a non-static data member with a volatile qualified type no longer implies that compiler generated copy / move constructors and copy / move assignment operators are not trivial
	)
endif()

target_include_directories(
	"${PROJECT_NAME}"
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(
	"${PROJECT_NAME}"
	PUBLIC
 		advapi32.lib
		bcrypt.lib
		d3d11.lib
		d3dcompiler.lib
		dbghelp.lib
		dxgi.lib
		mmio::mmio
		ole32.lib
		spdlog::spdlog
		version.lib
)

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		include/OBSE/Impl/PCH.h
)

install(
	TARGETS "${PROJECT_NAME}"
	EXPORT "${PROJECT_NAME}-targets"
)

install(
	EXPORT "${PROJECT_NAME}-targets"
	NAMESPACE "${PROJECT_NAME}::"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

configure_file(
	res/cmake/config.cmake.in
	"${PROJECT_NAME}Config.cmake"
	@ONLY
)

install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
	DIRECTORY
		"include/OBSE"
		"include/RE"
		"include/REL"
		"include/REX"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
