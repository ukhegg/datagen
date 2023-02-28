from conan import ConanFile
from conan.tools.scm import Git
from conan.tools.files import copy
import os


class DatagenRecipe(ConanFile):
    name = 'datagen'
    version = '1.0.4'
    user = 'ukhegg'
    channel = 'stable'
    url = 'https://github.com/ukhegg/datagen.git'

    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = ['CMakeToolchain', 'CMakeDeps']

    def requirements(self):
        self.requires('boost/1.81.0')

    def build_requirements(self):
        self.requires('catch2/3.3.1')

    def source(self):
        git = Git(self)
        git.clone(self.url, target='.')
        git.checkout(self.version)

    def package(self):
        copy(self, pattern="*.hpp",
             src=os.path.join(self.source_folder, 'datagen/include/'),
             dst=os.path.join(self.package_folder, 'include'))

    def package_info(self):
        self.cpp_info.include_dirs = 'include'

