from conans import CMake, ConanFile
import os

class TestPackage(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        bin_path = os.path.join(self.build_folder, "test_package")
        self.run("%s" % bin_path)