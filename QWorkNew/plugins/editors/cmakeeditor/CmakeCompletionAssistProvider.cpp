#include "CmakeCompletionAssistProvider.h"
#include "cmakeeditorconstants.h"

#include <keywordscompletionassist.h>

namespace Cmake {

static const char *const variableKeywords[] = {
    "CMAKE_AR",
    "CMAKE_ARGC",
    "CMAKE_ARGV0",
    "CMAKE_BINARY_DIR",
    "CMAKE_BUILD_TOOL",
    "CMAKE_CACHEFILE_DIR",
    "CMAKE_CACHE_MAJOR_VERSION",
    "CMAKE_CACHE_MINOR_VERSION",
    "CMAKE_CACHE_PATCH_VERSION",
    "CMAKE_CFG_INTDIR",
    "CMAKE_COMMAND",
    "CMAKE_CROSSCOMPILING",
    "CMAKE_CTEST_COMMAND",
    "CMAKE_CURRENT_BINARY_DIR",
    "CMAKE_CURRENT_LIST_DIR",
    "CMAKE_CURRENT_LIST_FILE",
    "CMAKE_CURRENT_LIST_LINE",
    "CMAKE_CURRENT_SOURCE_DIR",
    "CMAKE_DL_LIBS",
    "CMAKE_EDIT_COMMAND",
    "CMAKE_EXECUTABLE_SUFFIX",
    "CMAKE_EXTRA_GENERATOR",
    "CMAKE_EXTRA_SHARED_LIBRARY_SUFFIXES",
    "CMAKE_GENERATOR",
    "CMAKE_GENERATOR_TOOLSET",
    "CMAKE_HOME_DIRECTORY",
    "CMAKE_IMPORT_LIBRARY_PREFIX",
    "CMAKE_IMPORT_LIBRARY_SUFFIX",
    "CMAKE_LINK_LIBRARY_SUFFIX",
    "CMAKE_MAJOR_VERSION",
    "CMAKE_MAKE_PROGRAM",
    "CMAKE_MINIMUM_REQUIRED_VERSION",
    "CMAKE_MINOR_VERSION",
    "CMAKE_PARENT_LIST_FILE",
    "CMAKE_PATCH_VERSION",
    "CMAKE_PROJECT_NAME",
    "CMAKE_RANLIB",
    "CMAKE_ROOT",
    "CMAKE_SCRIPT_MODE_FILE",
    "CMAKE_SHARED_LIBRARY_PREFIX",
    "CMAKE_SHARED_LIBRARY_SUFFIX",
    "CMAKE_SHARED_MODULE_PREFIX",
    "CMAKE_SHARED_MODULE_SUFFIX",
    "CMAKE_SIZEOF_VOID_P",
    "CMAKE_SKIP_RPATH",
    "CMAKE_SOURCE_DIR",
    "CMAKE_STANDARD_LIBRARIES",
    "CMAKE_STATIC_LIBRARY_PREFIX",
    "CMAKE_STATIC_LIBRARY_SUFFIX",
    "CMAKE_TWEAK_VERSION",
    "CMAKE_VERBOSE_MAKEFILE",
    "CMAKE_VERSION",
    "CMAKE_VS_PLATFORM_TOOLSET",
    "CMAKE_XCODE_PLATFORM_TOOLSET",
    "PROJECT_BINARY_DIR",
    "PROJECT_NAME",
    "PROJECT_SOURCE_DIR",
    "[Project name]_BINARY_DIR",
    "[Project name]_SOURCE_DIR",
    "BUILD_SHARED_LIBS",
    "CMAKE_ABSOLUTE_DESTINATION_FILES",
    "CMAKE_AUTOMOC_RELAXED_MODE",
    "CMAKE_BACKWARDS_COMPATIBILITY",
    "CMAKE_BUILD_TYPE",
    "CMAKE_COLOR_MAKEFILE",
    "CMAKE_CONFIGURATION_TYPES",
    "CMAKE_DEBUG_TARGET_PROPERTIES",
    "CMAKE_DISABLE_FIND_PACKAGE_<PackageName>",
    "CMAKE_ERROR_DEPRECATED",
    "CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION",
    "CMAKE_FIND_LIBRARY_PREFIXES",
    "CMAKE_FIND_LIBRARY_SUFFIXES",
    "CMAKE_FIND_PACKAGE_WARN_NO_MODULE",
    "CMAKE_IGNORE_PATH",
    "CMAKE_INCLUDE_PATH",
    "CMAKE_INSTALL_DEFAULT_COMPONENT_NAME",
    "CMAKE_INSTALL_PREFIX",
    "CMAKE_LIBRARY_PATH",
    "CMAKE_MFC_FLAG",
    "CMAKE_MODULE_PATH",
    "CMAKE_NOT_USING_CONFIG_FLAGS",
    "CMAKE_POLICY_DEFAULT_CMP<NNNN>",
    "CMAKE_PREFIX_PATH",
    "CMAKE_PROGRAM_PATH",
    "CMAKE_SKIP_INSTALL_ALL_DEPENDENCY",
    "CMAKE_SYSTEM_IGNORE_PATH",
    "CMAKE_SYSTEM_INCLUDE_PATH",
    "CMAKE_SYSTEM_LIBRARY_PATH",
    "CMAKE_SYSTEM_PREFIX_PATH",
    "CMAKE_SYSTEM_PROGRAM_PATH",
    "CMAKE_USER_MAKE_RULES_OVERRIDE",
    "CMAKE_WARN_DEPRECATED",
    "CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION",
    "APPLE",
    "BORLAND",
    "CMAKE_CL_64",
    "CMAKE_COMPILER_2005",
    "CMAKE_HOST_APPLE",
    "CMAKE_HOST_SYSTEM",
    "CMAKE_HOST_SYSTEM_NAME",
    "CMAKE_HOST_SYSTEM_PROCESSOR",
    "CMAKE_HOST_SYSTEM_VERSION",
    "CMAKE_HOST_UNIX",
    "CMAKE_HOST_WIN32",
    "CMAKE_LIBRARY_ARCHITECTURE",
    "CMAKE_LIBRARY_ARCHITECTURE_REGEX",
    "CMAKE_OBJECT_PATH_MAX",
    "CMAKE_SYSTEM",
    "CMAKE_SYSTEM_NAME",
    "CMAKE_SYSTEM_PROCESSOR",
    "CMAKE_SYSTEM_VERSION",
    "CYGWIN",
    "ENV",
    "MSVC",
    "MSVC10",
    "MSVC11",
    "MSVC12",
    "MSVC60",
    "MSVC70",
    "MSVC71",
    "MSVC80",
    "MSVC90",
    "MSVC_IDE",
    "MSVC_VERSION",
    "UNIX",
    "WIN32",
    "XCODE_VERSION",
    "CMAKE_DEBUG_POSTFIX",
    "CMAKE_RELEASE_POSTFIX",
    "CMAKE_MINSIZEREL_POSTFIX",
    "CMAKE_RELWITHDEBINFO_POSTFIX",
    "CMAKE_C_VISIBILITY_PRESET",
    "CMAKE_CXX_VISIBILITY_PRESET",
    "CMAKE_ARCHIVE_OUTPUT_DIRECTORY",
    "CMAKE_AUTOMOC",
    "CMAKE_AUTOMOC_MOC_OPTIONS",
    "CMAKE_BUILD_WITH_INSTALL_RPATH",
    "CMAKE_DEBUG_POSTFIX",
    "CMAKE_EXE_LINKER_FLAGS",
    "CMAKE_EXE_LINKER_FLAGS_DEBUG",
    "CMAKE_EXE_LINKER_FLAGS_RELEASE",
    "CMAKE_EXE_LINKER_FLAGS_MINSIZEREL",
    "CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO",
    "CMAKE_Fortran_FORMAT",
    "CMAKE_Fortran_MODULE_DIRECTORY",
    "CMAKE_GNUtoMS",
    "CMAKE_INCLUDE_CURRENT_DIR",
    "CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE",
    "CMAKE_INSTALL_NAME_DIR",
    "CMAKE_INSTALL_RPATH",
    "CMAKE_INSTALL_RPATH_USE_LINK_PATH",
    "CMAKE_LIBRARY_OUTPUT_DIRECTORY",
    "CMAKE_LIBRARY_PATH_FLAG",
    "CMAKE_LINK_DEF_FILE_FLAG  ",
    "CMAKE_LINK_DEPENDS_NO_SHARED",
    "CMAKE_LINK_INTERFACE_LIBRARIES",
    "CMAKE_LINK_LIBRARY_FILE_FLAG",
    "CMAKE_LINK_LIBRARY_FLAG",
    "CMAKE_MACOSX_BUNDLE",
    "CMAKE_MODULE_LINKER_FLAGS",
    "CMAKE_MODULE_LINKER_FLAGS_DEBUG",
    "CMAKE_MODULE_LINKER_FLAGS_RELEASE",
    "CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL",
    "CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO",
    "CMAKE_NO_BUILTIN_CHRPATH",
    "CMAKE_PDB_OUTPUT_DIRECTORY",
    "CMAKE_POSITION_INDEPENDENT_CODE",
    "CMAKE_RUNTIME_OUTPUT_DIRECTORY",
    "CMAKE_SHARED_LINKER_FLAGS",
    "CMAKE_SHARED_LINKER_FLAGS_DEBUG",
    "CMAKE_SHARED_LINKER_FLAGS_RELEASE",
    "CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL",
    "CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO",
    "CMAKE_SKIP_BUILD_RPATH",
    "CMAKE_SKIP_INSTALL_RPATH",
    "CMAKE_STATIC_LINKER_FLAGS",
    "CMAKE_STATIC_LINKER_FLAGS_DEBUG",
    "CMAKE_STATIC_LINKER_FLAGS_RELEASE",
    "CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL",
    "CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO",
    "CMAKE_TRY_COMPILE_CONFIGURATION",
    "CMAKE_USE_RELATIVE_PATHS",
    "CMAKE_VISIBILITY_INLINES_HIDDEN",
    "CMAKE_WIN32_EXECUTABLE",
    "EXECUTABLE_OUTPUT_PATH",
    "LIBRARY_OUTPUT_PATH",
    "CMAKE_C_ARCHIVE_APPEND",
    "CMAKE_CXX_ARCHIVE_APPEND",
    "CMAKE_C_ARCHIVE_CREATE",
    "CMAKE_CXX_ARCHIVE_CREATE",
    "CMAKE_C_ARCHIVE_FINISH",
    "CMAKE_CXX_ARCHIVE_FINISH",
    "CMAKE_C_COMPILER",
    "CMAKE_CXX_COMPILER",
    "CMAKE_C_COMPILER_ABI",
    "CMAKE_CXX_COMPILER_ABI",
    "CMAKE_C_COMPILER_ID",
    "CMAKE_CXX_COMPILER_ID",
    "CMAKE_C_COMPILER_LOADED",
    "CMAKE_CXX_COMPILER_LOADED",
    "CMAKE_C_COMPILER_VERSION",
    "CMAKE_CXX_COMPILER_VERSION",
    "CMAKE_C_COMPILE_OBJECT",
    "CMAKE_CXX_COMPILE_OBJECT",
    "CMAKE_C_CREATE_SHARED_LIBRARY",
    "CMAKE_CXX_CREATE_SHARED_LIBRARY",
    "CMAKE_C_CREATE_SHARED_MODULE",
    "CMAKE_CXX_CREATE_SHARED_MODULE",
    "CMAKE_C_CREATE_STATIC_LIBRARY",
    "CMAKE_CXX_CREATE_STATIC_LIBRARY",
    "CMAKE_C_FLAGS",
    "CMAKE_CXX_FLAGS",
    "CMAKE_C_FLAGS_DEBUG",
    "CMAKE_CXX_FLAGS_DEBUG",
    "CMAKE_C_FLAGS_MINSIZEREL",
    "CMAKE_CXX_FLAGS_MINSIZEREL",
    "CMAKE_C_FLAGS_RELEASE",
    "CMAKE_CXX_FLAGS_RELEASE",
    "CMAKE_C_FLAGS_RELWITHDEBINFO",
    "CMAKE_CXX_FLAGS_RELWITHDEBINFO",
    "CMAKE_C_IGNORE_EXTENSIONS",
    "CMAKE_CXX_IGNORE_EXTENSIONS",
    "CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES",
    "CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES",
    "CMAKE_C_IMPLICIT_LINK_DIRECTORIES",
    "CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES",
    "CMAKE_C_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES",
    "CMAKE_CXX_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES",
    "CMAKE_C_IMPLICIT_LINK_LIBRARIES",
    "CMAKE_CXX_IMPLICIT_LINK_LIBRARIES",
    "CMAKE_C_LIBRARY_ARCHITECTURE",
    "CMAKE_CXX_LIBRARY_ARCHITECTURE",
    "CMAKE_C_LINKER_PREFERENCE",
    "CMAKE_CXX_LINKER_PREFERENCE",
    "CMAKE_C_LINKER_PREFERENCE_PROPAGATES",
    "CMAKE_CXX_LINKER_PREFERENCE_PROPAGATES",
    "CMAKE_C_LINK_EXECUTABLE ",
    "CMAKE_CXX_LINK_EXECUTABLE ",
    "CMAKE_C_OUTPUT_EXTENSION",
    "CMAKE_CXX_OUTPUT_EXTENSION",
    "CMAKE_C_PLATFORM_ID",
    "CMAKE_CXX_PLATFORM_ID",
    "CMAKE_C_SIZEOF_DATA_PTR",
    "CMAKE_CXX_SIZEOF_DATA_PTR",
    "CMAKE_C_SOURCE_FILE_EXTENSIONS",
    "CMAKE_CXX_SOURCE_FILE_EXTENSIONS",
    "CMAKE_COMPILER_IS_GNU<LANG>",
    "CMAKE_Fortran_MODDIR_DEFAULT",
    "CMAKE_Fortran_MODDIR_FLAG",
    "CMAKE_Fortran_MODOUT_FLAG",
    "CMAKE_INTERNAL_PLATFORM_ABI",
    "CMAKE_USER_MAKE_RULES_OVERRIDE_C",
    "CMAKE_USER_MAKE_RULES_OVERRIDE_CXX",
    "",
    0
};

static const char *const functionKeywords[] = {
    "add_compile_options",
    "add_custom_command",
    "add_custom_target",
    "add_definitions",
    "add_dependencies",
    "add_executable",
    "add_library",
    "add_subdirectory",
    "add_test",
    "aux_source_directory",
    "break",
    "build_command",
    "cmake_host_system_information",
    "cmake_minimum_required",
    "cmake_policy",
    "configure_file",
    "create_test_sourcelist",
    "define_property",
    "else",
    "elseif",
    "enable_language",
    "enable_testing",
    "endforeach",
    "endfunction",
    "endif",
    "endmacro",
    "endwhile",
    "execute_process",
    "export",
    "file",
    "find_file",
    "find_library",
    "find_package",
    "find_path",
    "find_program",
    "fltk_wrap_ui",
    "foreach",
    "function",
    "get_cmake_property",
    "get_directory_property",
    "get_filename_component",
    "get_property",
    "get_source_file_property",
    "get_target_property",
    "get_test_property",
    "if",
    "include",
    "include_directories",
    "include_external_msproject",
    "include_regular_expression",
    "install",
    "link_directories",
    "list",
    "load_cache",
    "load_command",
    "macro",
    "mark_as_advanced",
    "math",
    "message",
    "option",
    "project",
    "qt_wrap_cpp",
    "qt_wrap_ui",
    "remove_definitions",
    "return",
    "separate_arguments",
    "set",
    "set_directory_properties",
    "set_property",
    "set_source_files_properties",
    "set_target_properties",
    "set_tests_properties",
    "site_name",
    "source_group",
    "string",
    "target_compile_definitions",
    "target_compile_options",
    "target_include_directories",
    "target_link_libraries",
    "try_compile",
    "try_run",
    "unset",
    "variable_watch",
    "while",
    "build_name",
    "exec_program",
    "export_library_dependencies",
    "install_files",
    "install_programs",
    "install_targets",
    "link_libraries",
    "make_directory",
    "output_required_files",
    "remove",
    "subdir_depends",
    "subdirs",
    "use_mangled_mesa",
    "utility_source",
    "variable_requires",
    "write_file",
    "",
    0
};

CmakeCompletionAssistProvider::CmakeCompletionAssistProvider()
{ }

CmakeCompletionAssistProvider::~CmakeCompletionAssistProvider()
{ }

void CmakeCompletionAssistProvider::init()
{
    for (uint i = 0; i < sizeof variableKeywords / sizeof variableKeywords[0] - 1; i++)
        m_variables.append(QLatin1String(variableKeywords[i]));
    for (uint i = 0; i < sizeof functionKeywords / sizeof functionKeywords[0] - 1; i++)
        m_functions.append(QLatin1String(functionKeywords[i]));
}

bool CmakeCompletionAssistProvider::supportsEditor(const Core::Context &context) const
{
    return context.contains(Constants::C_CMAKEEDITOR_ID);
}

TextEditor::IAssistProcessor *CmakeCompletionAssistProvider::createProcessor() const
{
    if (m_variables.isEmpty())
        const_cast<CmakeCompletionAssistProvider *>(this)->init();
    TextEditor::Keywords keywords = TextEditor::Keywords(m_variables, m_functions, QMap<QString, QStringList>());
    return new TextEditor::KeywordsCompletionAssistProcessor(keywords);
}

QStringList CmakeCompletionAssistProvider::variables() const
{
    if (m_variables.isEmpty())
        const_cast<CmakeCompletionAssistProvider *>(this)->init();
    return m_variables;
}

QStringList CmakeCompletionAssistProvider::functions() const
{
    if (m_functions.isEmpty())
        const_cast<CmakeCompletionAssistProvider *>(this)->init();
    return m_functions;
}

}   // namespace Cmake
