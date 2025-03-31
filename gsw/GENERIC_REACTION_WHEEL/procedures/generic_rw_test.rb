require 'cosmos'
require 'cosmos/script'
require 'mission_lib.rb'

class RW_LPT < Cosmos::Test
  def setup
      enable_TO_and_verify()
  end

  def test_lpt
    start("tests/generic_rw_lpt_test.rb")
  end

  def teardown
      cmd("CFS_RADIO TO_PAUSE_OUTPUT")
  end
end

class RW_CPT < Cosmos::Test
  def setup
      
  end

  def test_cpt
    start("tests/generic_rw_cpt_test.rb")
  end

  def teardown

  end
end

class Generic_rw_Test < Cosmos::TestSuite
  def initialize
      super()
      add_test('RW_CPT')
      add_test('RW_LPT')
  end

  def setup
  end
  
  def teardown
  end
end