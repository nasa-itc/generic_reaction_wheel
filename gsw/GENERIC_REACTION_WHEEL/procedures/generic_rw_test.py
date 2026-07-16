import sys
import glob

for p in glob.glob('/gems/gems/openc3-cosmos-nos3-*/targets/GENERIC_REACTION_WHEEL/scripts'):
    if p not in sys.path:
        sys.path.append(p)

from openc3.script import cmd, tlm, check
from openc3.script.suite import Suite, Group

try:
    from nos3.generic_reaction_wheel_lib import *
    from nos3.generic_rw_app_test import run_generic_rw_app_test
    from nos3.generic_rw_device_test import run_generic_rw_device_test
    from nos3.generic_rw_ast_test import run_generic_rw_ast_test
except ImportError:
    pass

class RW_Functional_Test(Group):
    def setup(self):
        safe_GENERIC_REACTION_WHEEL()

    def script_application(self):
        run_generic_rw_app_test()

    def script_device(self):
        run_generic_rw_device_test()

    def teardown(self):
        teardown_RW()

class RW_Automated_Scenario_Test(Group):
    def setup(self):
        safe_GENERIC_REACTION_WHEEL()

    def script_ast(self):
        run_generic_rw_ast_test()

    def teardown(self):
        teardown_RW()

class Generic_rw_Test(Suite):
    def __init__(self):
        super().__init__()
        self.add_group(RW_Functional_Test)
        self.add_group(RW_Automated_Scenario_Test)

    def setup(self):
        safe_GENERIC_REACTION_WHEEL()
  
    def teardown(self):
        teardown_RW()