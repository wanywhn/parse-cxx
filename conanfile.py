from conans import ConanFile, CMake, tools


class ParsecxxConan(ConanFile):
    name = "ParseCloud"
    version = "0.0.1"
    license = "GPL"
    author = "Jia Qingtong  wanywhn@qq.com"
    url = "https://github.com/wanywhn/parse-cxx"
    description = "Parse Cloud C++ SDK"
    topics = ("SDK", "PARSE", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    # options = {"shared": [True, False]}
    default_options = {
        # "boost:shared": False,
        # "shared": False,
    }
    generators = {"cmake"}
    requires = \
        'jsonformoderncpp/3.6.1@vthiery/stable', \
        'spdlog/1.3.1@bincrafters/stable', \
        'cpprestsdk/2.10.13@bincrafters/stable', \
        'Catch2/2.9.1@catchorg/stable',
        # 'boost_format/1.69.0@bincrafters/stable', \
        # 'boost_system/1.69.0@bincrafters/stable', \
        # 'boost_regex/1.69.0@bincrafters/stable', \
        # 'boost_program_options/1.69.0@bincrafters/stable', \
        # 'boost_filesystem/1.69.0@bincrafters/stable', \
        # 'boost_thread/1.69.0@bincrafters/stable', \
        # 'boost_date_time/1.69.0@bincrafters/stable',
    # default_options ={"cpprestsdk:exclude_compression":True}

    def source(self):
        self.run("git clone https://github.com/conan-io/hello.git")
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly
        tools.replace_in_file("hello/CMakeLists.txt", "PROJECT(HelloWorld)",
                              '''PROJECT(HelloWorld)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="hello")
        self.copy("*hello.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["parsecloud"]
