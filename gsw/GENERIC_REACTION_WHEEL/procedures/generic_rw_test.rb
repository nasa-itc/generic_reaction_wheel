require 'cosmos'
require 'cosmos/script'
require 'mission_lib.rb'

class LPT < Cosmos::Test
  def setup
      enable_TO_and_verify()
  end

  def test_lpt
  end

  def teardown
      cmd("CFS_RADIO TO_PAUSE_OUTPUT")
  end
end

class CPT < Cosmos::Test
  def setup
      
  end

  def test_cpt
  end

  def teardown

  end
end

class Generic_rw_Test < Cosmos::TestSuite
  def initialize
      super()
      add_test('CPT')
      add_test('LPT')
  end

  def setup
  end
  
  def teardown
  end
end