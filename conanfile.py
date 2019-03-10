from conans import ConanFile, CMake

class Pkg(ConanFile):
    name = "vmpc"
    description = "vMPC2000XL application, an Akai 2000XL emulator"
    version = "0.1"
    settings = "os", "compiler", "arch", "build_type"
    generators = "cmake"
    exports_sources = "src/*"
    requires = ("wxwidgets/3.1.1@bincrafters/stable", "mpc/0.1@izmar/testing", "ctoot/0.1@izmar/testing", "moduru/0.1@izmar/testing", "rapidjson/1.1.0@bincrafters/stable", "Catch2/2.7.0@catchorg/stable")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

    def package(self):
        self.copy("*.h", src="src/main", dst="include")
        self.copy("*.hpp", src="src/main", dst="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

#    def package_info(self):
#        self.cpp_info.libs=["mpc"]