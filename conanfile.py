from conans import ConanFile, CMake

class Pkg(ConanFile):
    name = "vmpc"
    description = "vMPC2000XL application, an Akai 2000XL emulator"
    version = "0.1"
    settings = "os", "compiler", "arch", "build_type"
    generators = "cmake"
    exports_sources = "src/*"
    requires = ("log4cplus/2.0.0-rc2@bincrafters/stable", "RtAudio/5.1.0@qno/testing", "portaudio/v190600.20161030@bincrafters/stable", "cairo/1.15.14@bincrafters/stable", "sdl2/2.0.9@bincrafters/stable", "sdl2_image/2.0.4@bincrafters/stable", "mpc/0.1@izmar/dev", "ctoot/0.1@izmar/dev", "moduru/0.1@izmar/dev", "rapidjson/1.1.0@bincrafters/stable", "Catch2/2.7.0@catchorg/stable")
    url = "https://github.com/izzyreal/vmpc.git"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

    def package(self):
        self.copy("*.h", src="src/main/cpp", dst="include")
        self.copy("*.hpp", src="src/main/cpp", dst="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
