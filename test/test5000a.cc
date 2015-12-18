#include "picoscopeInterface.h"
#include "ps5000a.h"
#include "gmock/gmock.h"


using namespace testing;

class ps5000aTest : public Test {

  public:
  picoscope::ps5000a dev; 


};

TEST_F(ps5000aTest, OpenAndClose) {
  ASSERT_THAT(dev.open(), Eq(true));
  dev.close(); 
}  


TEST_F(ps5000aTest, OpenSetupChannelClose) {
  ASSERT_THAT(dev.open(picoscope::PS_8BIT), true);
  dev.setChCoupling(picoscope::A, picoscope::DC);
  dev.setChRange(picoscope::A, picoscope::PS_10MV);
  dev.enableChannel(picoscope::A);
  dev.close();
}


TEST_F(ps5000aTest, TestWaveCapture) {
  ASSERT_THAT(dev.open(picoscope::PS_12BIT), true);
  dev.setChCoupling(picoscope::A, picoscope::DC);
  dev.setChRange(picoscope::A, picoscope::PS_10MV);
  dev.enableChannel(picoscope::A);
  dev.setTimebase(2);
  dev.setSamples(1000); 
  dev.setPreTriggerSamples(500);
  dev.setPostTriggerSamples(500);
  dev.setCaptureCount(1);
  ASSERT_THAT(dev.prepareBuffers(), true);
  dev.captureBlock(); 
 
  dev.close();  
  picoscope::chRange range = picoscope::PS_10MV; 
  vector <vector<short> > data = dev.getWaveforms();
  std::cout << "Waveform size:" << data[0].size() << std::endl; 
  for (auto &waveform : data) {
    std::cout << std::endl;
    std::cout << std::endl; 
    for (int i = 0; i < waveform.size(); i++) { 
      std::cout << dev.adcToMv(waveform[i], range) << ", ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }


}
