#include <iostream>
#include <vector>
#include "UeiDaq.h"
#include "StaticMethods.h"

using namespace UeiDaq;
using std::vector;
using std::string;
using std::cout;


struct C1
{
public:
    C1(string name): instanceName(name)
    {}
    string instanceName;
};

std::unique_ptr<C1> MakeItem()
{
    std::unique_ptr<C1> item = std::make_unique< C1>("myitem");
    return item;
}


int main1(int argc, char* argv[])
{
    std::unique_ptr<C1> item = MakeItem();
    cout << "item: " << item->instanceName << "\n";

    {
        string url{ "pdna://192.168.100.2/" };
        std::unique_ptr<vector<CUeiDevice*>> _deviceList = StaticMethods::ReadDeviceList2(url);

        int deviceId1 = 0;
        for (CUeiDevice* device : *_deviceList)
        {
            std::string devName = device->GetDeviceName();

            std::cout << "device " << deviceId1++ << " : " << devName << std::endl;

        }
    }



    //{
    //    vector<CUeiDevice*> _deviceList;
    //    string url{ "pdna://192.168.100.2/" };
    //    ReadDeviceList(url, _deviceList);

    //    int deviceId1 = 0;
    //    for (CUeiDevice* device : _deviceList)
    //    {
    //        std::string devName;
    //        //std::string serial;

    //        devName = device->GetDeviceName();
    //        //serial = pDev->GetSerialNumber();

    //        std::cout << "device " << deviceId1++ << " : " << devName << std::endl;

    //    }
    //}

    
   try
   {
      // Create a device enumerator object for the specified family of devices
      CUeiDeviceEnumerator devEnum("pdna://192.168.100.2/");

      CUeiDevice* pDev;
      int deviceId=0;
      // Enumerate through all the devices and list their capabilities
      while((pDev = devEnum.GetDevice(deviceId++)) != NULL)
      {
         std::string devName;
         std::string serial;

         devName = pDev->GetDeviceName();
         serial = pDev->GetSerialNumber();

         std::cout << "device " << deviceId << " : " << devName << " (#" << serial << ")" << std::endl;

         if(pDev->GetNumberOfAIChannels(UeiAIChannelInputModeSingleEnded) > 0)
         {
            std::cout << "Analog Input Subsystem:" << std::endl;
            std::cout << "Number of Analog input channels = " << pDev->GetNumberOfAIChannels(UeiAIChannelInputModeSingleEnded) << std::endl;
            std::cout << "Maximum sampling frequency = " << pDev->GetMaxAIRate() << std::endl;
            std::cout << "Resolution = " << pDev->GetAIResolution() << std::endl;
            std::cout << "Input gains = ";
            tGains gains;
            pDev->GetAIGains(gains);
            for(size_t i = 0; i< gains.size(); i++)
            {
               std::cout << gains[i] << " ";
            }
            std::cout << std::endl; 
            std::cout << std::endl;
         }

         if(pDev->GetNumberOfAOChannels() > 0)
         {
            std::cout << "Analog Output Subsystem:" << std::endl;
            std::cout << "Number of Analog output channels = " << pDev->GetNumberOfAOChannels() << std::endl;
            std::cout << "Maximum genration frequency = " << pDev->GetMaxAORate() << std::endl;
            std::cout << "Resolution = " << pDev->GetAOResolution() << std::endl;
            
            std::cout << std::endl;        
         }

         if(pDev->GetNumberOfDIChannels() > 0)
         {
            std::cout << "Digital Input Subsystem:" << std::endl;
            std::cout << "Number of Digital input ports = " << pDev->GetNumberOfDIChannels() << std::endl;
            std::cout << "Maximum acquisition frequency = " << pDev->GetMaxDIRate() << std::endl;
            std::cout << "Port width = " << pDev->GetDIResolution() << std::endl;
            
            std::cout << std::endl;        
         }

         if(pDev->GetNumberOfDOChannels() > 0)
         {
            std::cout << "Digital Output Subsystem:" << std::endl;
            std::cout << "Number of Digital output ports = " << pDev->GetNumberOfDOChannels() << std::endl;
            std::cout << "Maximum generation frequency = " << pDev->GetMaxDORate() << std::endl;
            std::cout << "Port width = " << pDev->GetDOResolution() << std::endl;
            
            std::cout << std::endl;        
         }

         if(pDev->GetNumberOfCIChannels() > 0)
         {
            std::cout << "Counter Input Subsystem:" << std::endl;
            std::cout << "Number of counter = " << pDev->GetNumberOfCIChannels() << std::endl;
            std::cout << "Counter clock frequency = " << pDev->GetMaxCIRate() << std::endl;
            std::cout << "Resolution = " << pDev->GetCIResolution() << std::endl;
            
            std::cout << std::endl;        
         }

         if(pDev->GetNumberOfCOChannels() > 0)
         {
            std::cout << "Timer Output Subsystem:" << std::endl;
            std::cout << "Number of timers = " << pDev->GetNumberOfCOChannels() << std::endl;
            std::cout << "Timer clock frequency = " << pDev->GetMaxCORate() << std::endl;
            std::cout << "Resolution = " << pDev->GetCOResolution() << std::endl;
            
            std::cout << std::endl;        
         }

         std::cout << std::endl << std::endl;
      };
   }
   catch(CUeiException& e)
   {
      std::cout << "Error " << std::hex << e.GetError() << ": " << e.GetErrorMessage() << std::endl;
   }

   return 0;
}