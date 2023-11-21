import os

from pyqtbuild import PyQtBindings, PyQtProject
from sipbuild import Option


class QGIS(PyQtProject):
    """ The QGIS project. """

    def __init__(self):
        """ Initialize the project. """
        super().__init__()
        self.sip_files_dir = '.'
        self.bindings_factories = [QgisCore]

    def get_options(self):
        """ Return the list of configurable options. """
        options = super().get_options()
        options.append(
                Option('include_dirs', option_type=list,
                        help="additional directory to search for .sip files",
                        metavar="DIR"))
        return options

    def apply_user_defaults(self, tool):
        """ Set default values for user options that haven't been set yet. """
        super().apply_user_defaults(tool)
        if self.include_dirs is not None:
            self.sip_include_dirs += self.include_dirs


class QgisCore(PyQtBindings):
    """ The QGIS Core bindings. """

    def __init__(self, project):
        """ Initialize the bindings. """
        super().__init__(project, 'edlib')
        self.sip_file = 'edlib.sip'
        self.exceptions = True
        self.release_gil = True
        self.disabled_features = []

    def apply_user_defaults(self, tool):
        """ Set default values for user options that haven't been set yet. """
        if self.project.py_platform.startswith('win32'):
            self.tags.append('WS_WIN')
        elif self.project.py_platform == 'darwin':
            self.tags.append('WS_MACX')
        else:
            self.tags.append('WS_X11')
        super().apply_user_defaults(tool)
