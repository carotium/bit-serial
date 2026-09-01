from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = "0.0.1"

eigen_include_dir = "/usr/include/eigen3"

ext_modules = [
    Pybind11Extension(
        "bit_serial",
        ["src/main.cpp"],
        include_dirs=[eigen_include_dir,],
        define_macros=[("VERSION INFO", __version__)],
    ),
]

setup(
    name="bit_serial",
    version=__version__,
    author="carotium",
    description="Bit-serial computation plugin",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.12",
)
