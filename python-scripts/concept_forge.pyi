from collections.abc import Iterable, Iterator
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

class Entity:
    """Base class representing a 3D object with position, rotation, and scale"""

    def __init__(self) -> None:
        """
        Create a new Entity with default position (0,0,0), rotation (0,0,0), and scale (1,1,1).
        """

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

class EntityVector:
    """
    A dynamic array (list) of shared Entity objects.

    This class behaves like a standard Python list and can be used to store or access
    multiple Entity instances, such as those managed by a scene or simulation system.

    Each element is a shared_ptr to an Entity, preserving ownership semantics.
    """

    @overload
    def __init__(self) -> None:
        """Default constructor"""

    @overload
    def __init__(self, arg: EntityVector) -> None:
        """Copy constructor"""

    @overload
    def __init__(self, arg: Iterable[Entity], /) -> None:
        """Construct from an iterable object"""

    def __len__(self) -> int: ...

    def __bool__(self) -> bool:
        """Check whether the vector is nonempty"""

    def __repr__(self) -> str: ...

    def __iter__(self) -> Iterator[Entity]: ...

    @overload
    def __getitem__(self, arg: int, /) -> Entity: ...

    @overload
    def __getitem__(self, arg: slice, /) -> EntityVector: ...

    def clear(self) -> None:
        """Remove all items from list."""

    def append(self, arg: Entity, /) -> None:
        """Append `arg` to the end of the list."""

    def insert(self, arg0: int, arg1: Entity, /) -> None:
        """Insert object `arg1` before index `arg0`."""

    def pop(self, index: int = -1) -> Entity:
        """Remove and return item at `index` (default last)."""

    def extend(self, arg: EntityVector, /) -> None:
        """Extend `self` by appending elements from `arg`."""

    @overload
    def __setitem__(self, arg0: int, arg1: Entity, /) -> None: ...

    @overload
    def __setitem__(self, arg0: slice, arg1: EntityVector, /) -> None: ...

    @overload
    def __delitem__(self, arg: int, /) -> None: ...

    @overload
    def __delitem__(self, arg: slice, /) -> None: ...

    def __eq__(self, arg: object, /) -> bool: ...

    def __ne__(self, arg: object, /) -> bool: ...

    @overload
    def __contains__(self, arg: Entity, /) -> bool: ...

    @overload
    def __contains__(self, arg: object, /) -> bool: ...

    def count(self, arg: Entity, /) -> int:
        """Return number of occurrences of `arg`."""

    def remove(self, arg: Entity, /) -> None:
        """Remove first occurrence of `arg`."""

class Hierarchy:
    def __init__(self) -> None:
        """Create new Hierarchy instance"""

    @property
    def entities(self) -> "std::unordered_map<unsigned int, std::shared_ptr<SimObject::Entity>, std::hash<unsigned int>, std::equal_to<unsigned int>, std::allocator<std::pair<unsigned int const, std::shared_ptr<SimObject::Entity> > > >": ...

    @entities.setter
    def entities(self, arg: "std::unordered_map<unsigned int, std::shared_ptr<SimObject::Entity>, std::hash<unsigned int>, std::equal_to<unsigned int>, std::allocator<std::pair<unsigned int const, std::shared_ptr<SimObject::Entity> > > >", /) -> None: ...

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
