// UeiBridgeCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "../ExtLib/spdlog/spdlog.h"
#include "UeiDaq.h"
#include "StaticMethods.h"

using namespace UeiDaq;
namespace spd = spdlog;
using std::vector;
using std::string;
using std::cout;

int AnalogOutputDevice();
int AnalogInDevice();

int main()
{

    auto console = spd::stdout_color_mt("main");
    //console->info("Welcome to spdlog!");
    //console->error("Some error message with arg{}..", 1);

	//(_MSC_VER >= 1920) && (_MSC_VER < 1940)
	//auto m = _MSC_VER;

    string url{ "pdna://192.168.100.2/" };
    std::unique_ptr<vector<CUeiDevice*>> _deviceList = StaticMethods::ReadDeviceList2(url);

    int deviceId1 = 0;
	size_t len = _deviceList->size();
	if (len > 0)
	{
		for (CUeiDevice* device : *_deviceList)
		{
			std::string devName = device->GetDeviceName();
			//auto s = device->GetSlot();
			auto i = device->GetIndex();
			console->info("{}, Dev{}", devName, i);
		}
	}
	else
	{
		console->info("No device connected");
	}

    AnalogOutputDevice();
    AnalogInDevice();
}

void GenerateSinWave(double* pBuffer, int nbChannels, int nbSamplePerChannel, int iteration)
{
    int amplitude = (iteration % 10 + 1);

    for (int i = 0; i < nbSamplePerChannel; i++)
    {
        for (int j = 0; j < nbChannels; j++)
        {
            pBuffer[i * nbChannels + j] = amplitude * sin(2 * 3.1415 * (j + 1) * i / nbSamplePerChannel);
        }
    }
}

int AnalogOutputDevice()
{
    CUeiSession mySs;
    double* data;
    auto _logger = spd::stdout_color_mt("AO");

    try
    {
        // Create 2 analog output channels on a powerdaq board
        // From now on the session is AO only
        mySs.CreateAOChannel("pdna://192.168.100.2/Dev0/ao0:1", -10.0, 10.0);

        mySs.ConfigureTimingForSimpleIO();

        // Create a reader object to read data synchronously.
        CUeiAnalogScaledWriter writer(mySs.GetDataStream());

        // Allocate a buffer to hold data to generate
        data = new double[mySs.GetNumberOfChannels() * 100];

        GenerateSinWave(data, mySs.GetNumberOfChannels(), 100, 0);

        _logger->info("Start generating sin wave");
        // Generates all points in the buffer
        for (int i = 0; i < 100; i++)
        {
            std::cout << "Generating scan " << i << std::endl;
            writer.WriteSingleScan(&data[i * 2]);
        }

        delete[] data;

        _logger->info("AO End");
    }
    catch (CUeiException e)
    {
        std::cout << "Error: " << e.GetErrorMessage() << std::endl;
    }

    return 0;
}

int AnalogInDevice()
{
    CUeiSession mySs;
    double data[800];

    try
    {
        // Create 8 analog input channels on a powerdaq board
        // From now on the session is AI only
        mySs.CreateAIChannel("pdna://192.168.100.2/Dev2/ai0:7", -10.0, 10.0, UeiAIChannelInputModeDifferential);

        // By default the timing object is configured for simple I/O so
        // no need to do anything here.
        mySs.ConfigureTimingForSimpleIO();

        // Create a reader object to read data synchronously.
        CUeiAnalogScaledReader reader(mySs.GetDataStream());

        mySs.Start();

        // Read 100 scans
        for (int i = 0; i < 100; i++)
        {
            reader.ReadSingleScan(&data[i * mySs.GetNumberOfChannels()]);
            std::cout << "Ch0 = " << data[i * mySs.GetNumberOfChannels()] << std::endl;
        }

        mySs.Stop();
    }
    catch (CUeiException e)
    {
        std::cout << "Error: " << e.GetErrorMessage() << std::endl;
    }

    return 0;
}