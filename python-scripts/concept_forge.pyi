from collections.abc import Iterator
import enum
from typing import overload

from . import primitives as primitives


class Vec3:
    """A 3D vector with float components x, y, and z."""

    def __init__(self, arg0: float, arg1: float, arg2: float, /) -> None:
        """
        Constructor with x, y, z

        Args:
            x (float): X-component
            y (float): Y-component
            z (float): Z-component
        """

    @property
    def x(self) -> float:
        """X-component of the vector"""

    @x.setter
    def x(self, arg: float, /) -> None: ...

    @property
    def y(self) -> float:
        """Y-component of the vector"""

    @y.setter
    def y(self, arg: float, /) -> None: ...

    @property
    def z(self) -> float:
        """Z-component of the vector"""

    @z.setter
    def z(self, arg: float, /) -> None: ...

    def __repr__(self) -> str:
        """
        Return the string representation of the vector in the form '<Vec3 x=X, y=Y, z=Z>'
        """

    def __add__(self, arg: Vec3, /) -> Vec3:
        """
        Vector addition. Returns a new Vec3 which is the sum of this and another Vec3.
        """

    def __sub__(self, arg: Vec3, /) -> Vec3:
        """
        Vector subtraction. Returns a new Vec3 which is the difference between this and another Vec3.
        """

    @overload
    def __mul__(self, arg: float, /) -> Vec3:
        """Scalar multiplication. Multiplies the vector by a scalar float."""

    @overload
    def __mul__(self, arg: Vec3, /) -> Vec3:
        """
        Component-wise multiplication. Multiplies each corresponding component of two vectors.
        """

    def __rmul__(self, arg: float, /) -> Vec3:
        """Scalar multiplication. Multiplies the vector by a scalar float."""

    def to_tuple(self) -> "std::tuple<float, float, float>":
        """Convert the vector to a Python tuple of the form (x, y, z)."""

    def to_list(self) -> list[float]:
        """Convert the vector to a Python list [x, y, z]."""

    def to_dict(self) -> dict:
        """Convert the vector to a Python dictionary {'x': x, 'y': y, 'z': z}."""

class DrawMode(enum.Enum):
    """Rendering mode used by shaders"""

    WIREFRAME = 0
    """Render using wireframe mode (edges only)"""

    FILLED = 1
    """Render using filled (solid) polygons"""

WIREFRAME: DrawMode = DrawMode.WIREFRAME

FILLED: DrawMode = DrawMode.FILLED

class ShaderType(enum.Enum):
    """Types of built-in shaders used in the engine"""

    Unlit = 0
    """Unlit shader (no lighting calculations)"""

    Lit = 1
    """Lit shader (basic lighting enabled)"""

    Light = 2
    """Light shader used for rendering light sources"""

Unlit: ShaderType = ShaderType.Unlit

Lit: ShaderType = ShaderType.Lit

Light: ShaderType = ShaderType.Light

class ShaderProgram:
    """Encapsulates a GLSL shader program"""

    def __init__(self) -> None:
        """Create an empty ShaderProgram"""

    def init(self, mode: DrawMode, vertexShaderPath: str, fragmentShaderPath: str) -> None:
        """Compile, link and initialize the shader program"""

    def set_draw_mode(self, mode: DrawMode) -> None:
        """Set the rendering mode (wireframe or filled)"""

    def use(self) -> None:
        """Activate this shader program for subsequent draw calls"""

    def link_shaders(self) -> None:
        """Link the compiled vertex and fragment shaders into a complete program"""

    def send_data_to_shader(self) -> None:
        """Send additional required uniform data to the shader"""

    def set_bool(self, name: str, value: bool) -> None:
        """Set a boolean uniform"""

    def set_int(self, name: str, value: int) -> None:
        """Set an integer uniform"""

    def set_float(self, name: str, value: float) -> None:
        """Set a float uniform"""

    def set_vec2(self, name: str, value: "glm::vec<2, float, (glm::qualifier)0>") -> None:
        """Set a vec2 uniform (2D vector)"""

    def set_vec3(self, name: str, value: Vec3) -> None:
        """Set a vec3 uniform (3D vector)"""

    def set_vec4(self, name: str, value: "glm::vec<4, float, (glm::qualifier)0>") -> None:
        """Set a vec4 uniform (4D vector)"""

    def set_mat4(self, name: str, value: "glm::mat<4, 4, float, (glm::qualifier)0>") -> None:
        """Set a mat4 uniform (4x4 matrix)"""

class Entity:
    """Base class representing a 3D object with position, rotation, and scale"""

    def __init__(self) -> None:
        """
        Create a new Entity with default position (0,0,0), rotation (0,0,0), and scale (1,1,1).
        """

    @property
    def shader(self) -> ShaderProgram: ...

    @shader.setter
    def shader(self, arg: ShaderProgram, /) -> None: ...

    def get_position(self) -> Vec3:
        """Return the current position of the Entity as a Vec3 (x, y, z)."""

    def get_rotation(self) -> Vec3:
        """
        Return the current rotation of the Entity as a Vec3 in Euler angles (in degrees).
        """

    def get_scale(self) -> Vec3:
        """Return the current scale of the Entity as a Vec3 (x, y, z)."""

    def set_position(self, position: Vec3) -> None:
        """
        Set the position of the Entity.

        Args:
            position (Vec3): The new position vector.
        """

    def set_rotation(self, rotation: Vec3) -> None:
        """
        Set the rotation of the Entity using Euler angles in degrees.

        Args:
            rotation (Vec3): The new rotation vector.
        """

    def set_scale(self, scale: Vec3) -> None:
        """
        Set the scale of the Entity.

        Args:
            scale (Vec3): The new scale vector.
        """

    def translate(self, delta: Vec3) -> None:
        """
        Translate (move) the Entity by a given delta vector.

        Args:
            delta (Vec3): The amount to move in each axis.
        """

    @overload
    def rotate(self, delta: Vec3) -> None:
        """
        Rotate the Entity by a delta rotation in Euler angles (degrees).

        Args:
            delta (Vec3): Rotation to apply in each axis.
        """

    @overload
    def rotate(self, angle: float, axis: Vec3) -> None:
        """
        Rotate the Entity around a given axis by a specified angle (degrees).

        Args:
            angle (float): Angle in degrees.
            axis (Vec3): Axis to rotate around.
        """

    def scale(self, deltaFactor: Vec3) -> None:
        """
        Scale the Entity by multiplying its current scale by the given factor.

        Args:
            deltaFactor (Vec3): Scale multiplier for each axis.
        """

class EntityMap:
    @overload
    def __init__(self) -> None:
        """Default constructor"""

    @overload
    def __init__(self, arg: EntityMap) -> None:
        """Copy constructor"""

    @overload
    def __init__(self, arg: dict[int, Entity], /) -> None:
        """Construct from a dictionary"""

    def __len__(self) -> int: ...

    def __bool__(self) -> bool:
        """Check whether the map is nonempty"""

    def __repr__(self) -> str: ...

    @overload
    def __contains__(self, arg: int, /) -> bool: ...

    @overload
    def __contains__(self, arg: object, /) -> bool: ...

    def __iter__(self) -> Iterator[int]: ...

    def __getitem__(self, arg: int, /) -> Entity: ...

    def __delitem__(self, arg: int, /) -> None: ...

    def clear(self) -> None:
        """Remove all items"""

    def __setitem__(self, arg0: int, arg1: Entity, /) -> None: ...

    def update(self, arg: EntityMap, /) -> None:
        """Update the map with element from `arg`"""

    def __eq__(self, arg: object, /) -> bool: ...

    def __ne__(self, arg: object, /) -> bool: ...

    class ItemView:
        def __len__(self) -> int: ...

        def __iter__(self) -> Iterator[tuple[int, Entity]]: ...

    class KeyView:
        @overload
        def __contains__(self, arg: int, /) -> bool: ...

        @overload
        def __contains__(self, arg: object, /) -> bool: ...

        def __len__(self) -> int: ...

        def __iter__(self) -> Iterator[int]: ...

    class ValueView:
        def __len__(self) -> int: ...

        def __iter__(self) -> Iterator[Entity]: ...

    def keys(self) -> EntityMap.KeyView:
        """Returns an iterable view of the map's keys."""

    def values(self) -> EntityMap.ValueView:
        """Returns an iterable view of the map's values."""

    def items(self) -> EntityMap.ItemView:
        """Returns an iterable view of the map's items."""

class Hierarchy:
    def __init__(self) -> None:
        """Create new Hierarchy instance"""

    @property
    def entities(self) -> EntityMap: ...

    @entities.setter
    def entities(self, arg: EntityMap, /) -> None: ...

class ConceptForge:
    """
    The main application context that manages the rendering window, input, GUI, and scene entities.
    This class acts as the core engine loop handler and entity manager.
    """

    def __init__(self) -> None:
        """Create a new ConceptForge instance with default settings."""

    def window_should_close(self) -> bool:
        """
        Check if the rendering window should close (e.g. user clicked the close button).

        Returns:
            bool: True if the window should close, False otherwise.
        """

    def dt(self) -> float:
        """
        Return the time elapsed between the current and previous frame.

        Useful for time-based animation and physics.

        Returns:
            float: Delta time in seconds.
        """

    def process_input(self) -> None:
        """Poll and process input events from the window (keyboard, mouse, etc)."""

    def render(self) -> None:
        """Clear the screen and render all registered entities."""

    def calc_projection(self) -> None:
        """
        Recalculate the camera projection matrix based on current parameters like field of view, aspect ratio, etc.
        """

    def gui_management(self) -> None:
        """Render the GUI (e.g. ImGui windows for editor features)."""

    @property
    def window(self) -> "WindowManagement::Window":
        """The rendering window object associated with the application."""

    @window.setter
    def window(self, arg: "WindowManagement::Window", /) -> None: ...

    @property
    def input_man(self) -> "InputManagement::Input":
        """The input manager handling keyboard and mouse states."""

    @input_man.setter
    def input_man(self, arg: "InputManagement::Input", /) -> None: ...

    @property
    def hierarchy(self) -> Hierarchy:
        """Hierarchy object that holds references to all entities in the scene"""

    @hierarchy.setter
    def hierarchy(self, arg: Hierarchy, /) -> None: ...
