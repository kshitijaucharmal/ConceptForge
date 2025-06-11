namespace MathBindings {
// Wrappers ================================================================
// Vec3 wrapper ------------------------------------------------------------
struct Vec3Wrapper {
  float x, y, z;

  Vec3Wrapper(float x_ = 0, float y_ = 0, float z_ = 0)
  : x(x_), y(y_), z(z_) {}

  // Allow constructing from glm::vec3
  Vec3Wrapper(const glm::vec3& v) : x(v.x), y(v.y), z(v.z) {}

  glm::vec3 to_glm() const { return glm::vec3(x, y, z); }

  Vec3Wrapper operator+(const Vec3Wrapper& other) const {
    return Vec3Wrapper(x + other.x, y + other.y, z + other.z);
  }

  Vec3Wrapper operator-(const Vec3Wrapper& other) const {
    return Vec3Wrapper(x - other.x, y - other.y, z - other.z);
  }

  Vec3Wrapper operator*(float scalar) const {
    return Vec3Wrapper(x * scalar, y * scalar, z * scalar);
  }
};
// -------------------------------------------------------------------

// Functions ================================================================
// --------------------------------------------------------------------------
void vec3_binder(nb::module_ &module) {
  nb::class_<Vec3Wrapper>(module, "Vec3")
  .def(nb::init<float, float, float>(), "x"_a = 0.f, "y"_a = 0.f, "z"_a = 0.f)
  .def_rw("x", &Vec3Wrapper::x)
  .def_rw("y", &Vec3Wrapper::y)
  .def_rw("z", &Vec3Wrapper::z)
  .def("__add__", &Vec3Wrapper::operator+)
  .def("__sub__", &Vec3Wrapper::operator-)
  .def("__mul__", &Vec3Wrapper::operator*)
  .def("__repr__", [](const Vec3Wrapper &v) {
    return "Vec3(" +
    std::to_string(v.x) + ", " +
    std::to_string(v.y) + ", " +
    std::to_string(v.z) + ")";
  });
}

// Final Binder
void math_binder(nb::module_ &module){
    vec3_binder(module);
}

}
