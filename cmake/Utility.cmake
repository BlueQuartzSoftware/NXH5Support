function(enable_vcpkg_manifest_feature)
  set(optionsArgs)
  set(oneValueArgs TEST_VAR FEATURE)
  set(multiValueArgs)
  cmake_parse_arguments(ARGS "${optionsArgs}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(${ARGS_TEST_VAR})
    if(NOT ${ARGS_FEATURE} IN_LIST VCPKG_MANIFEST_FEATURES)
      set(VCPKG_MANIFEST_FEATURES ${VCPKG_MANIFEST_FEATURES} ${ARGS_FEATURE} PARENT_SCOPE)
    endif()
  endif()
endfunction()

function(install_with_directory)
  set(optionsArgs)
  set(oneValueArgs DESTINATION COMPONENT BASE_DIR)
  set(multiValueArgs FILES)
  cmake_parse_arguments(ARGS "${optionsArgs}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  foreach(file ${ARGS_FILES})
    file(RELATIVE_PATH relative_file ${ARGS_BASE_DIR} ${file})
    get_filename_component(destination_dir ${relative_file} DIRECTORY)
    install(FILES ${file}
      DESTINATION ${ARGS_DESTINATION}/${destination_dir}
      COMPONENT ${ARGS_COMPONENT}
    )
  endforeach()
endfunction()

function(H5Support_enable_warnings)
  set(optionsArgs)
  set(oneValueArgs TARGET)
  set(multiValueArgs)
  cmake_parse_arguments(ARG "${optionsArgs}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT TARGET ${ARG_TARGET})
    message(FATAL_ERROR "H5Support_enable_warnings must be called with the argument TARGET set to a valid target")
  endif()

  if(MSVC)
    target_compile_options(${ARG_TARGET}
      PRIVATE

      # Suppressed warnings
      /wd4275 # C4275: An exported class was derived from a class that wasn't exported.
      /wd4251 # C4251: 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'

      # Warning to error
      /we4706 # C4706: assignment within conditional expression
      /we4715 # C4715: The specified function can potentially not return a value.
      /we4456 # C4456: declaration of 'identifier' hides previous local declaration
      /we4457 # C4457: declaration of 'identifier' hides function parameter
      /we4458 # C4458: declaration of 'identifier' hides class member
      /we4459 # C4459: declaration of 'identifier' hides global declaration
    )
  else()
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      set(SHADOW_WARNING "shadow")
    else()
      set(SHADOW_WARNING "shadow-all")
    endif()

    target_compile_options(${ARG_TARGET}
      PRIVATE

      # Warning to error
      -Werror=parentheses # Wparentheses: Warn if parentheses are omitted in certain contexts, such as when there is an assignment in a context where a truth value is expected, or when operators are nested whose precedence people often get confused about
      -Werror=return-type # Wreturn-type: Warn about any "return" statement with no return value in a function whose return type is not "void"
      -Werror=${SHADOW_WARNING} # Wshadow: Warn whenever a local variable or type declaration shadows another variable, parameter, type, class member (in C++), or instance variable (in Objective-C) or whenever a built-in function is shadowed.
    )
  endif()
endfunction()
