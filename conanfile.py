import os

from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, load

required_conan_version = ">=2.4"

class ZapifConan(ConanFile):
    name = "zapif"
    description = (
        "Algebraically simplifies C and C++ preprocessor conditionals and "
        "removes code that the preprocessor would never select."
    )
    license = "Apache-2.0"
    # Repo-local recipe: `url` points at the project itself. When porting to
    # conan-center-index, change `url` to the CCI repo and keep `homepage`.
    url = "https://github.com/ArchRobison/zapif"
    homepage = "https://github.com/ArchRobison/zapif"
    topics = ("preprocessor", "conditional-compilation", "unifdef",
              "code-simplification", "c", "cpp", "cli")
    package_type = "application"
    vendor = True
    settings = "os", "arch", "compiler", "build_type"

    exports_sources = "CMakeLists.txt", "version.txt", "LICENSE", "src/*", "test/*"

    def set_version(self):
        self.version = load(self, os.path.join(self.recipe_folder, "version.txt")).strip()

    def layout(self):
        cmake_layout(self)

    def build_requirements(self):
        if self.settings_build.os == "Windows":
            self.tool_requires("winflexbison/2.5.25")
        else:
            self.tool_requires("flex/2.6.4")
            self.tool_requires("bison/3.8.2")
        self.tool_requires("cmake/[>=4.0 <5]")

    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, 11)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["BUILD_TESTING"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE",
             src=self.source_folder,
             dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        # Application package: nothing to compile or link against. In Conan 2 the
        # package's bindir is added to consumers' PATH automatically.
        self.cpp_info.includedirs = []
        self.cpp_info.libdirs = []

    def package_id(self):
        del self.info.settings.compiler
