
#pragma once

namespace MathBindings {
  void binder(nb::module_ &module){
    nb::class_<glm::vec3>(module, "Vec3", "A 3D vector with float components x, y, and z.")
    .def(nb::init<float, float, float>(),
        "Constructor with x, y, z\n\n"
        "Args:\n"
        "    x (float): X-component\n"
        "    y (float): Y-component\n"
        "    z (float): Z-component")
    .def_rw("x", &glm::vec3::x, "X-component of the vector")
    .def_rw("y", &glm::vec3::y, "Y-component of the vector")
    .def_rw("z", &glm::vec3::z, "Z-component of the vector")

    .def("__repr__", [](const glm::vec3 &v) {
      return "<Vec3 x=" + std::to_string(v.x) +
      ", y=" + std::to_string(v.y) +
      ", z=" + std::to_string(v.z) + ">";
    }, "Return the string representation of the vector in the form '<Vec3 x=X, y=Y, z=Z>'")

    // Arithmetic operators
    .def(nb::self + nb::self, "Vector addition. Returns a new Vec3 which is the sum of this and another Vec3.")
    .def(nb::self - nb::self, "Vector subtraction. Returns a new Vec3 which is the difference between this and another Vec3.")
    .def(nb::self * float(), "Scalar multiplication. Multiplies the vector by a scalar float.")
    .def(float() * nb::self, "Scalar multiplication. Multiplies the vector by a scalar float.")
    .def(nb::self * nb::self, "Component-wise multiplication. Multiplies each corresponding component of two vectors.")

    // Conversion methods
    .def("to_tuple", [](const glm::vec3 &v) {
      return std::make_tuple(v.x, v.y, v.z);
    }, "Convert the vector to a Python tuple of the form (x, y, z).")

    .def("to_list", [](const glm::vec3 &v) {
      return std::vector<float>{v.x, v.y, v.z};
    }, "Convert the vector to a Python list [x, y, z].")

    .def("to_dict", [](const glm::vec3 &v) {
      return nb::dict(
        "x"_nb=v.x,
        "y"_nb=v.y,
        "z"_nb=v.z
      );
    }, "Convert the vector to a Python dictionary {'x': x, 'y': y, 'z': z}.");
  }

}
