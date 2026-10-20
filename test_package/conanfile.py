from conan import ConanFile
from conan.tools.build import can_run


class TestPackageConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    test_type = "explicit"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def test(self):
        # Cross-built binaries (e.g. armv8 built on an x86_64 machine) can't run here.
        if can_run(self):
            # zapif accepts -v (not --version); prints "zapif <version>" and exits 0.
            self.run("zapif -v", env="conanrun")
