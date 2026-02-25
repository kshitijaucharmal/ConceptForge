//
// Created by kshitij on 2/25/26.
//

#include "PythonRunner.hpp"

#include "Components/Primitives/Transform.hpp"

namespace PythonRunner {
    static bool python_initialized = false;
    void Init() {
        if (!python_initialized)
        {
            py::initialize_interpreter();
            python_initialized = true;
        }

    }
    void Destroy() {
        py::finalize_interpreter();
        python_initialized = false;
    }

    std::string Execute(const std::string& user_code) {
        const auto globals = py::globals();
        const std::string wrapper = R"(
import sys, io
sys.stdout = io.StringIO()
try:
    exec(user_code)
    captured_output = sys.stdout.getvalue()
finally:
    sys.stdout = sys.__stdout__ # Always reset!
)";

        // Pass the user's code into the Python environment as a variable
        globals["user_code"] = user_code;

        py::exec(wrapper, globals);

        // TODO: This pull the result after execution. Find a way to live update
        // Pull the result back out
        return globals["captured_output"].cast<std::string>();
    }
}
