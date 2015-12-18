#include "picoscopeInterface.h"
#include "ps6000.h"
#include "gmock/gmock.h"


using namespace testing;

class ps6000Test : public Test {

  public:
  picoscope::ps6000 dev; 


};


TEST_F(ps6000Test, OpenAndClose) {
  ASSERT_THAT(dev.open(), Eq(false));
  dev.close(); 
}  










