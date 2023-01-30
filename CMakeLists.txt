cmake_minimum_required(VERSION 3.19)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Utility.cmake)

option(NXH5SUPPORT_BUILD_TESTS "Enable building NXH5SUPPORT tests" ON)
enable_vcpkg_manifest_feature(TEST_VAR NXH5SUPPORT_BUILD_TESTS FEATURE "tests")

option(NXCOMMON_ENABLE_MULTICORE "Enable multicore support" ON)

project(NXH5Support
    VERSION 0.1.0
    DESCRIPTION "H5Support"
    LANGUAGES CXX
)

set(NXH5SUPPORT_BIN_DIR ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${NXH5SUPPORT_BIN_DIR} CACHE PATH "Single Directory for all Libraries")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${NXH5SUPPORT_BIN_DIR} CACHE PATH "Single Directory for all Executables.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${NXH5SUPPORT_BIN_DIR} CACHE PATH "Single Directory for all static libraries.")

list(APPEND CMAKE_MODULE_PATH ${NXH5Support_SOURCE_DIR}/cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(expected-lite CONFIG REQUIRED)
find_package(span-lite CONFIG REQUIRED)
find_package(HDF5 REQUIRED)
find_package(NXCommon REQUIRED)

if(NXCOMMON_ENABLE_MULTICORE)
    find_package(TBB CONFIG REQUIRED)
endif()

add_library(NXH5Support SHARED)
add_library(NX::H5Support ALIAS NXH5Support)

target_compile_features(NXH5Support
    PUBLIC
    cxx_std_17
)

set_target_properties(NXH5Support
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

H5Support_enable_warnings(TARGET NXH5Support)

if(MSVC)
    target_compile_options(NXH5Support
        PRIVATE
        /MP
    )
endif()

# Force HDF5 1.10 API
target_compile_definitions(NXH5Support PUBLIC "H5_USE_110_API")

target_link_libraries(NXH5Support
    PUBLIC
    fmt::fmt
    HDF5::HDF5
    NX::NXCommon
)

if(UNIX)
    target_link_libraries(NXH5Support
        PRIVATE
        ${CMAKE_DL_LIBS}
    )
endif()

if(NXCOMMON_ENABLE_MULTICORE)
    target_compile_definitions(NXH5Support PUBLIC "NXCOMMON_ENABLE_MULTICORE")
    target_link_libraries(NXH5Support PUBLIC TBB::tbb)
endif()

option(NXH5SUPPORT_ENABLE_LINK_FILESYSTEM "Enables linking to a C++ filesystem library" OFF)

if(NXH5SUPPORT_ENABLE_LINK_FILESYSTEM)
    set(NXH5SUPPORT_FILESYSTEM_LIB "stdc++fs" CACHE STRING "C++ filesystem library to link to")
    set_property(CACHE NXH5SUPPORT_FILESYSTEM_LIB PROPERTY STRINGS "stdc++fs" "c++fs")
    target_link_libraries(NXH5Support
        PUBLIC
        ${NXH5SUPPORT_FILESYSTEM_LIB}
    )
endif()

set(NXH5SUPPORT_GENERATED_DIR ${NXH5Support_BINARY_DIR}/generated)
set(NXH5SUPPORT_GENERATED_HEADER_DIR ${NXH5Support_BINARY_DIR}/generated/NX/H5Support)
set(NXH5SUPPORT_EXPORT_HEADER ${NXH5SUPPORT_GENERATED_HEADER_DIR}/NXH5SUPPORT_EXPORT.hpp)

include(GenerateExportHeader)
generate_export_header(NXH5Support
    EXPORT_FILE_NAME ${NXH5SUPPORT_EXPORT_HEADER}
    CUSTOM_CONTENT_FROM_VARIABLE NXH5SUPPORT_EXPORT_HEADER_EXTRA
)

set(NXH5SUPPORT_SOURCE_DIR ${NXH5Support_SOURCE_DIR}/src/NX/H5Support)

set(NXH5SUPPORT_HDRS
    ${NXH5SUPPORT_SOURCE_DIR}/H5.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/H5Support.hpp

    ${NXH5SUPPORT_SOURCE_DIR}/IO/AttributeIO.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/IO/DatasetIO.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/IO/FileIO.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/IO/GroupIO.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/IO/ObjectIO.hpp

    ${NXH5SUPPORT_SOURCE_DIR}/Readers/AttributeReader.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/Readers/DatasetReader.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/Readers/FileReader.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/Readers/GroupReader.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/Readers/ObjectReader.hpp

    ${NXH5SUPPORT_SOURCE_DIR}/Writers/AttributeWriter.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/Writers/DatasetWriter.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/Writers/FileWriter.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/Writers/GroupWriter.hpp
    ${NXH5SUPPORT_SOURCE_DIR}/Writers/ObjectWriter.hpp
)

set(NXH5SUPPORT_GENERATED_HEADERS
    ${NXH5SUPPORT_EXPORT_HEADER}
)

set(NXH5SUPPORT_SRCS
    ${NXH5SUPPORT_SOURCE_DIR}/H5.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/H5Support.cpp

    ${NXH5SUPPORT_SOURCE_DIR}/IO/AttributeIO.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/IO/DatasetIO.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/IO/FileIO.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/IO/GroupIO.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/IO/ObjectIO.cpp

    ${NXH5SUPPORT_SOURCE_DIR}/Readers/AttributeReader.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/Readers/DatasetReader.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/Readers/FileReader.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/Readers/GroupReader.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/Readers/ObjectReader.cpp

    ${NXH5SUPPORT_SOURCE_DIR}/Writers/AttributeWriter.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/Writers/DatasetWriter.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/Writers/FileWriter.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/Writers/GroupWriter.cpp
    ${NXH5SUPPORT_SOURCE_DIR}/Writers/ObjectWriter.cpp
)

set(NXH5SUPPORT_ALL_HDRS
    ${NXH5SUPPORT_HDRS}
    ${NXH5SUPPORT_GENERATED_HEADERS}
)

target_sources(NXH5Support
    PRIVATE
    ${NXH5SUPPORT_ALL_HDRS}
    ${NXH5SUPPORT_SRCS}
)

source_group(TREE ${NXH5SUPPORT_SOURCE_DIR} PREFIX "NX/H5Support" FILES ${NXH5SUPPORT_HDRS})
source_group(TREE ${NXH5SUPPORT_GENERATED_HEADER_DIR} PREFIX "NX/H5Support/generated" FILES ${NXH5SUPPORT_GENERATED_HEADERS})
source_group(TREE ${NXH5SUPPORT_SOURCE_DIR} PREFIX "NX/H5Support" FILES ${NXH5SUPPORT_SRCS})

set(NXH5SUPPORT_INCLUDE_DIR ${NXH5Support_SOURCE_DIR}/src)

target_include_directories(NXH5Support
    PUBLIC
    $<BUILD_INTERFACE:${NXH5SUPPORT_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${NXH5SUPPORT_GENERATED_DIR}>
    $<INSTALL_INTERFACE:include>
)

set(NXH5SUPPORT_PACKAGE_NAMESPACE "NX::")

option(NXH5SUPPORT_ENABLE_INSTALL "Enables H5Support install rules" ON)

if(NXH5SUPPORT_ENABLE_INSTALL)
    install(TARGETS NXH5Support
        EXPORT NXH5SupportTargets
    )

    export(EXPORT NXH5SupportTargets NAMESPACE ${NXH5SUPPORT_PACKAGE_NAMESPACE})

    include(CMakePackageConfigHelpers)

    set(NXH5SUPPORT_INSTALL_CMAKE_PREFIX "lib/cmake")

    set(NXH5SUPPORT_CONFIG_FILE ${NXH5Support_BINARY_DIR}/NXH5SupportConfig.cmake)
    set(NXH5SUPPORT_VERSION_FILE ${NXH5Support_BINARY_DIR}/NXH5SupportConfigVersion.cmake)

    configure_package_config_file(${NXH5Support_SOURCE_DIR}/cmake/NXH5SupportConfig.cmake.in ${NXH5SUPPORT_CONFIG_FILE}
        INSTALL_DESTINATION ${NXH5SUPPORT_INSTALL_CMAKE_PREFIX}
    )

    write_basic_package_version_file(${NXH5SUPPORT_VERSION_FILE}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES ${NXH5SUPPORT_CONFIG_FILE} ${NXH5SUPPORT_VERSION_FILE}
        DESTINATION ${NXH5SUPPORT_INSTALL_CMAKE_PREFIX}
        COMPONENT develop
    )

    include(GNUInstallDirs)
    install(TARGETS NXH5Support
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT runtime
        LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT runtime
        NAMELINK_COMPONENT develop
        ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT develop
    )

    install(EXPORT NXH5SupportTargets
        NAMESPACE ${NXH5SUPPORT_PACKAGE_NAMESPACE}
        DESTINATION ${NXH5SUPPORT_INSTALL_CMAKE_PREFIX}
    )

    install_with_directory(FILES ${NXH5SUPPORT_HDRS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NX/H5Support
        COMPONENT develop
        BASE_DIR ${NXH5SUPPORT_SOURCE_DIR}
    )

    install_with_directory(FILES ${NXH5SUPPORT_GENERATED_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NX/H5Support
        COMPONENT develop
        BASE_DIR ${NXH5SUPPORT_GENERATED_HEADER_DIR}
    )
endif()

option(NXH5SUPPORT_ENABLE_PACKAGING "Enables NX::H5Support packaging" ON)

if(NXH5SUPPORT_ENABLE_PACKAGING)
    if(WIN32)
        set(CPACK_GENERATOR ZIP)
    else()
        set(CPACK_GENERATOR TGZ)
    endif()

    include(CPack)
endif()

if(NXH5SUPPORT_BUILD_TESTS)
    include(CTest)
    add_subdirectory(test)
endif()
