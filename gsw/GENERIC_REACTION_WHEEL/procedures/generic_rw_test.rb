require 'cosmos'
require 'cosmos/script'
require "generic_reaction_wheel_lib.rb"

class RW_Functional_Test < Cosmos::Test
  def setup
      safe_GENERIC_REACTION_WHEEL()
  end

  def test_application
    start("tests/generic_rw_app_test.rb")
  end

  def test_device
    start("tests/generic_rw_device_test.rb")
  end

  def teardown
    teardown_RW()
  end
end

class RW_Automated_Scenario_Test < Cosmos::Test
  def setup
    safe_GENERIC_REACTION_WHEEL()
  end

  def test_ast
    start("tests/generic_rw_ast_test.rb")
  end

  def teardown
    teardown_RW()
  end
end

class Generic_rw_Test < Cosmos::TestSuite
  def initialize
      super()
      add_test('RW_Functional_Test')
      add_test('RW_Automated_Scenario_Test')
  end

  def setup
    safe_GENERIC_REACTION_WHEEL()
  end
  
  def teardown
    teardown_RW()
  end
end