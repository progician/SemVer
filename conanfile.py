from conans import ConanFile, CMake, tools
import os, re


def _get_version():
    try:
        content = tools.load("CMakeLists.txt")
        version = re.search(r"project\s*\(\s*SemVer\s+VERSION\s+(.*)\s*\)", content).group(1)
        return version.strip()
    except Exception:
        return None


class SemVer(ConanFile):
    name = "SemVer"
    description = "C++ library and command line tool to work with Semantic Versioning"
    author = "Gyula Gubacsi <gyula.gubacsi@gmail.com>"
    version = _get_version()
    license = "General Public License 2.0"

    generators = "cmake_find_package"
    settings = "os", "arch", "compiler", "build_type"
    build_requires = "catch2/[^2.6.0]"

    options = {
        "shared": [True, False],
        "debug_info": [True, False]
    }
    default_options = {"shared": True, "debug_info": False}

    exports_sources = "CMakeLists.txt", "include/*", "src/*", "SemVerConfig.cmake"

    def configure(self):
        if self.settings.build_type != "Release":
            self.options.debug_info = False
    
    def _configure_cmake(self):
        modified_build_type = "RelWithDebInfo" \
            if self.options.debug_info else self.settings.build_type
        cmake = CMake(self, build_type=modified_build_type)
        cmake.definitions["CMAKE_PROJECT_INCLUDE"] = \
            os.path.join(self.build_folder, "conan_paths.cmake")
        cmake.configure()
        return cmake
    
    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.cxxflags.append("-std=c++11")