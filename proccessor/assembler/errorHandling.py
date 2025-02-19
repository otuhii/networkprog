import functools
import inspect
from typing import Any, Callable, Type
from pprint import pformat

def debug_on_error(func: Callable) -> Callable:
    """
    Decorator that prints all instance attributes when an exception occurs.
    Can be applied to any method in a class.
    """
    @functools.wraps(func)
    def wrapper(self: Any, *args: Any, **kwargs: Any) -> Any:
        try:
            return func(self, *args, **kwargs)
        except Exception as e:
            # Get all instance attributes
            attributes = {
                name: value 
                for name, value in inspect.getmembers(self)
                if not name.startswith('__') and not callable(value)
            }
            
            # Create formatted error message
            error_msg = (
                f"\nError in {self.__class__.__name__}.{func.__name__}:\n"
                f"Exception: {type(e).__name__}: {str(e)}\n"
                f"Instance state:\n{pformat(attributes, indent=2)}"
            )
            
            # Re-raise the exception with the debug information
            raise type(e)(error_msg) from e
            
    return wrapper

def debug_all_methods(cls: Type) -> Type:
    """
    Class decorator that applies debug_on_error to all methods in the class.
    """
    for name, method in inspect.getmembers(cls, predicate=inspect.isfunction):
        if not name.startswith('__'):
            setattr(cls, name, debug_on_error(method))
    return cls
