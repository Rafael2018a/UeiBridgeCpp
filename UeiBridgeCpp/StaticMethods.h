#pragma once
#include <string>
#include <vector>
#include "UeiDaq.h"


using namespace std;
using namespace UeiDaq;

struct StaticMethods
{
    static void ReadDeviceList(const std::string& url, std::vector<UeiDaq::CUeiDevice*>& _deviceList)
    {
        CUeiDeviceEnumerator devEnum(url);
        int deviceId = 0;
        CUeiDevice* pDev;
        // Enumerate through all the devices and list their capabilities
        while ((pDev = devEnum.GetDevice(deviceId++)) != NULL)
        {
            _deviceList.push_back(pDev);
        }
    }

    static std::unique_ptr<vector<CUeiDevice*>> ReadDeviceList2(const std::string& url)
    {
        std::unique_ptr<vector<CUeiDevice*>> _deviceList = std::make_unique< vector<CUeiDevice*>>();

        CUeiDeviceEnumerator devEnum(url);
        int deviceId = 0;
        CUeiDevice* pDev;
        // Enumerate through all the devices and list their capabilities
        while ((pDev = devEnum.GetDevice(deviceId++)) != NULL)
        {
            _deviceList->push_back(pDev);
        }
        return _deviceList;
    }

};

