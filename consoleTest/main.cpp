// main.cpp : Defines the entry point for the console application.
//
//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

// This source file is a test harness to test C code on a Teensy 3.2 micro-controller
// https://www.pjrc.com/store/teensy32.html
// Manufacturer name from chip: Teensyduino - sse code below

// Have one, and only one, Teensy 3.2 attached to a USB port on the PC - NOT via a USB hub as the
// Teensy 3.2 could not be written to by the code in this source file.
// If you have 2 Teensy 3.2s then the test code may not read or write to the same Teensy 3.2 unit
// as each unit has 2 device filepaths associated with it.



#include "stdafx.h"

#include "../usbhid/interface/common_stl_hdrs.hpp"
#include "../usbhid/interface/usbhid.hpp"
#include "../usbhid/interface/hidusagedata.hpp"
#include "../usbhid/interface/rawhiddevice.hpp"

namespace
{
  using namespace USBHID_ns;

  void Print(hidDeviceInfo const & devInfo, std::wostream & os);

  // This class can be used by threads to synchronise calls to std::cout
  class PrintToStdCout final
  {
  public:

    template <typename FIRST, typename ... ARGS>
    static void print(FIRST const & first, ARGS const &... args)
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      std::cout << first;
      auto outWithSpace = [](auto const & arg) {
        std::cout << ' ' << arg;
      };
      (..., outWithSpace(args));
      std::cout << std::endl;
    }

  private:
    inline static std::mutex m_mutex;
  };

  // Thread function that blocks until data is available to be read on a RawHidDevice's file
  // and prints the data to std::cout.
  // The function returns if it receives a stop command  - 0xFF -  or there on an error.
  bool ReceiveThread1(RawHidDevice & dev)
  {
    RawHidDevice::usbHidPacket packet;
    while(dev.receive(packet) == RawHidDevice::COMMS_ERROR::SUCCESS)
    {
      if(packet.packet[2] != 0xFF)
      {
        std::uint16_t const & highByte(packet.packet[0] << 8);
        std::uint16_t const & lowByte(packet.packet[1]);
        auto const number{ highByte + lowByte };
        PrintToStdCout::print("Thread1: number received = ", number);
      }
      else
      {
        PrintToStdCout::print("Thread1: finished with true");
        return true;
      }        
    }
    PrintToStdCout::print("Thread1: finished with false");
    return false; 
  }

  // Thread function that blocks until data is available to be read on a RawHidDevice's file
  // and prints the data to std::cout.
  // The function returns if it receives a stop command  - 'STOP' -  or there on an error.
  bool ReceiveThread2(RawHidDevice & dev)
  {

    RawHidDevice::usbHidPacket packet;
    while(dev.receive(packet) == RawHidDevice::COMMS_ERROR::SUCCESS)
    {
      std::string op{ std::cbegin(packet.packet), std::cend(packet.packet) };
      if(op.substr(0, 4) == "STOP")
      {
        PrintToStdCout::print("Thread2: finished with true");
        return true;
      }
      PrintToStdCout::print("Thread2:" ,op);
    }

    PrintToStdCout::print("Thread2: finished with false");
    return false;
  }

}


// Test for Teensy 3.2
int main()
{
  using namespace USBHID_ns;


  auto const & list{ GetlInstalledDevicesInfo() };

  // Print info on each USB device filepath
  std::cout << "Print out of USB device filepaths and associated basic USB information\n\n";
  std::for_each(std::cbegin(list), std::cend(list), [](const auto & devInfo) { Print(devInfo, std::wcout); std::cout << '\n'; });

  std::cout << "################################################################################\n\n";

  // Check USBHID_ns::GetString()
  std::cout << "Check USBHID_ns::GetString()\n";
  std::wcout << GetString(HID_USAGEPAGE::UNI_CODE) << L'\n' << GetString(HID_GENERIC_USAGE::BYTE_COUNT) << L"\n\n";


  // Get list of installed Teensyduino devices - 
  std::list<RawHidDevice> teensyDevicesList;
  std::for_each(std::cbegin(list), std::cend(list),
    [&teensyDevicesList](auto const & elem)
  {
    if(elem.manufacturerName == L"Teensyduino")
    {
      teensyDevicesList.emplace_back(elem);
    }
  });

  // and sort on usage. As setup there are 2 device filepaths associated with the Teensy 3.2
  // One with the Serial.print in the Teensy 3.2 C code and the other with the RawHID.send and RawHID.receive
  // They have different usages.
  teensyDevicesList.sort([](auto const & elem, auto const & elem1)
  {
    return elem.devInfo().caps.Usage < elem1.devInfo().caps.Usage;
  });

  std::vector<RawHidDevice> teensyDevices
  {
    [&teensyDevicesList]()
    {
      std::vector<RawHidDevice> devs;
      for(auto & elem: teensyDevicesList)
      {
        devs.emplace_back(std::move(elem));
      }
      return devs;
    }()
  };
  std::for_each(std::begin(teensyDevices), std::end(teensyDevices), [](auto & device) { device.open(); });

  std::cout << "################################################################################\n\n";

  // ################ Section to test in a threaded environment ###############
    std::cout << "Thread test\n\n";
  auto recvThrd1 = std::async(std::launch::async, ReceiveThread1, std::ref(teensyDevices[1]));
  auto recvThrd2 = std::async(std::launch::async, ReceiveThread2, std::ref(teensyDevices[0]));

  PrintToStdCout::print("Sending start msg");
  RawHidDevice::usbHidPacket startMsg{ { 1 } };
  teensyDevices[1].send(startMsg, 100);

  ::Sleep(1000);
  PrintToStdCout::print("Sending stop msg");
  RawHidDevice::usbHidPacket stopMsg{ { 0 } };
  teensyDevices[1].send(stopMsg, 100);
  
  auto result1 = recvThrd1.get();
  auto result2 = recvThrd2.get();
  std::cout << "\n\nEnd of Thread test\n\n";
  // ################ End of Section to test in a threaded environment ########

  std::cout << "################################################################################\n\n";

  // ############# Section to test multiple USB packet block reads ############
  std::cout << "Testing block reads\n\n";
  PrintToStdCout::print("Sending start msg");
  teensyDevices[1].send(startMsg, 100);

  ::Sleep(1000);

  auto const & block0{ teensyDevices[0].receive(4, 2000) };
  auto const & block1{ teensyDevices[1].receive(4, 2000) };

  std::cout << "\nPrinting block0 output: number of packets = " << block0.packets.size() << '\n';
  for(auto const & elem : block0.packets)
  {
    std::string op(std::cbegin(elem.packet), std::cend(elem.packet));
    op.erase(op.find('\r'));
    std::cout << op << '\n';
  }

  std::cout << "\nPrinting block1 output: number of packets = " << block1.packets.size() << '\n';
  for(auto const & elem : block1.packets)
  {
    std::uint16_t const & highByte(elem.packet[0] << 8);
    std::uint16_t const & lowByte(elem.packet[1]);
    auto const number{ highByte + lowByte };
    std::cout << number << '\n';
  }

  PrintToStdCout::print("Sending stop msg");
  teensyDevices[1].send(stopMsg, 100);
  std::cout << "\n\nEnd of Testing block reads\n\n";
  // ########## End of Section to test multiple USB packet block reads ########

  std::cout << "################################################################################\n\n";

  // ############# Section to test multiple USB packet contiguous writes and block reads of responses ############
  std::cout << "Testing contiguous USB packets writes and block reads\n\n";
  std::vector<RawHidDevice::usbHidPacket> blockSend{ { 2 },{ 3 } };
  auto result3{ teensyDevices[1].send(blockSend[0], 100) };
  auto result4{ teensyDevices[1].send(blockSend[1], 100) };

  ::Sleep(100);

  RawHidDevice::usbHidPacket pkt;
  //auto const & recv = teensyDevices[0].receive(pkt, 200);

  auto const & block2 = teensyDevices[0].receive(4, 2000);
  std::cout << "\nPrinting block2 output: number of packets read = " << block2.packets.size() << '\n';
  for(auto const & elem : block2.packets)
  {
    std::string op(std::cbegin(elem.packet), std::cend(elem.packet));
    op.erase(op.find('\r'));
    std::cout << op << '\n';
  }
  std::cout << "\n\nEnd of Testing contiguous USB packets writes and block reads\n";
  // ############# End of Section to test multiple USB packet contiguous writes and block reads of responses ############

  std::cout << "\n\n################################################################################\n\n";
  std::cout << "END OF TEST HARNESS\n";
  return 0;
}

namespace
{
  void Print(hidDeviceInfo const & devInfo, std::wostream & os)
  {
    os <<
      L"Device path: " << devInfo.path <<
      L"\nManfacturer's name: " << devInfo.manufacturerName <<

      L"\nVendorID: " << std::hex << std::showbase << devInfo.attributes.VendorID <<
      L"\nProductID: " << devInfo.attributes.ProductID <<
      L"\nVersion Number: " << std::dec << std::noshowbase << devInfo.attributes.VersionNumber <<
      L"\nUsage Page: " << std::hex << std::showbase << devInfo.caps.UsagePage <<
      L"\nUsage: " << std::hex << std::showbase << devInfo.caps.Usage <<
      L"\nInput Report Byte Length: " << std::dec << std::noshowbase << devInfo.caps.InputReportByteLength <<

      L"\nOutput Report Byte Length: " << devInfo.caps.OutputReportByteLength <<
      L"\nFeature Report Byte Length: " << devInfo.caps.FeatureReportByteLength <<
      L"\nNumber Link Collection Nodes: " << devInfo.caps.NumberLinkCollectionNodes <<
      L"\nNumber Input Value Caps: " << devInfo.caps.NumberInputValueCaps <<
      L"\nNumber Input Button Caps: " << devInfo.caps.NumberInputButtonCaps <<
      L"\nNumber Input Data Indices: " << devInfo.caps.NumberInputDataIndices <<
      L"\nNumber Output Button Caps: " << devInfo.caps.NumberOutputButtonCaps <<
      L"\nNumber Output Value Caps: " << devInfo.caps.NumberOutputValueCaps <<
      L"\nNumber Output Data Indices: " << devInfo.caps.NumberOutputDataIndices <<
      L"\nNumber Feature Button Caps: " << devInfo.caps.NumberFeatureButtonCaps <<
      L"\nNumber Output Value Caps: " << devInfo.caps.NumberOutputValueCaps <<
      L"\nNumber Feature Data Indices: " << devInfo.caps.NumberFeatureDataIndices << '\n';
  }
}