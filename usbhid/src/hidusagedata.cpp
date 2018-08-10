// hidusagedata.cpp

//  MIT License
//  See LICENSE.txt file in root of project
//  Copyright(c) 2018 Simon Parmenter

#include "stdafx.h"

#include "../interface/hidusagedata.hpp"

#include "../interface/common_stl_hdrs.hpp"

namespace USBHID_ns
{
  USBHID_API std::wstring GetString(HID_USAGEPAGE usagePage) noexcept
  {
    using usagePageStringMap_t = std::map<HID_USAGEPAGE, wchar_t const * const>;

    try
    {
      static usagePageStringMap_t const usagePageStringMap
      {
  #define USAGEPAGESTRINGMAP_ELEM(enum, string) {HID_USAGEPAGE::enum, L#string}
        USAGEPAGESTRINGMAP_ELEM(UNDEFINED, UNDEFINED)
        ,USAGEPAGESTRINGMAP_ELEM(GENERIC, GENERIC)
        ,USAGEPAGESTRINGMAP_ELEM(SIMULATION,SIMULATION)
        ,USAGEPAGESTRINGMAP_ELEM(VR,VR)
        ,USAGEPAGESTRINGMAP_ELEM(SPORT,SPORT)
        ,USAGEPAGESTRINGMAP_ELEM(GAME,GAME)
        ,USAGEPAGESTRINGMAP_ELEM(GENERIC_DEVICE,GENERIC DEVICE)
        ,USAGEPAGESTRINGMAP_ELEM(KEYBOARD,KEYBOARD)
        ,USAGEPAGESTRINGMAP_ELEM(LED,LED)
        ,USAGEPAGESTRINGMAP_ELEM(BUTTON,BUTTON)
        ,USAGEPAGESTRINGMAP_ELEM(ORDINAL,ORDINAL)
        ,USAGEPAGESTRINGMAP_ELEM(TELEPHONY,TELEPHONY)
        ,USAGEPAGESTRINGMAP_ELEM(CONSUMER,CONSUMER)
        ,USAGEPAGESTRINGMAP_ELEM(DIGITIZER,DIGITIZER)
        ,USAGEPAGESTRINGMAP_ELEM(HAPTICS,HAPTICS)
        ,USAGEPAGESTRINGMAP_ELEM(PID,PID)
        ,USAGEPAGESTRINGMAP_ELEM(UNI_CODE,UNICODE)
        ,USAGEPAGESTRINGMAP_ELEM(ALPHANUMERIC,ALPHANUMERIC)
        ,USAGEPAGESTRINGMAP_ELEM(SENSOR,SENSOR)
        ,USAGEPAGESTRINGMAP_ELEM(BARCODE_SCANNER,BARCODE SCANNER)
        ,USAGEPAGESTRINGMAP_ELEM(WEIGHING_DEVICE,WEIGHING DEVICE)
        ,USAGEPAGESTRINGMAP_ELEM(MAGNETIC_STRIPE_READER,MAGNETIC STRIPE READER)
        ,USAGEPAGESTRINGMAP_ELEM(CAMERA_CONTROL,CAMERA CONTROL)
        ,USAGEPAGESTRINGMAP_ELEM(ARCADE,ARCADE)
        ,USAGEPAGESTRINGMAP_ELEM(MICROSOFT_BLUETOOTH_HANDSFREE,MICROSOFT BLUETOOTH HANDSFREE)
        ,USAGEPAGESTRINGMAP_ELEM(VENDOR_DEFINED_BEGIN,VENDOR DEFINED BEGIN)
        ,USAGEPAGESTRINGMAP_ELEM(VENDOR_DEFINED_END,VENDOR DEFINED END)
  #undef USAGEPAGESTRINGMAP_ELEM
      };

      return usagePageStringMap.at(usagePage);
    }
    catch(...)
    {
      return{};
    }
  }

  USBHID_API std::wstring GetString(HID_GENERIC_USAGE usage) noexcept
  {
    using genericUsageStringMap_t = std::map<HID_GENERIC_USAGE, wchar_t const * const>;

    try
    {
      static genericUsageStringMap_t const genericUsageStringMap
      {
#define GENERICPAGESTRINGMAP_ELEM(enum, string) {HID_GENERIC_USAGE::enum, L#string}
        GENERICPAGESTRINGMAP_ELEM(POINTER, POINTER)
        ,GENERICPAGESTRINGMAP_ELEM(MOUSE, MOUSE)
        ,GENERICPAGESTRINGMAP_ELEM(JOYSTICK,JOYSTICK)
        ,GENERICPAGESTRINGMAP_ELEM(GAMEPAD,GAMEPAD)
        ,GENERICPAGESTRINGMAP_ELEM(KEYBOARD,KEYBOARD)
        ,GENERICPAGESTRINGMAP_ELEM(KEYPAD,KEYPAD)
        ,GENERICPAGESTRINGMAP_ELEM(MULTI_AXIS_CONTROLLER,MULTI AXIS CONTROLLER)
        ,GENERICPAGESTRINGMAP_ELEM(TABLET_PC_SYSTEM_CTL,TABLET PC SYSTEM CTL)
        ,GENERICPAGESTRINGMAP_ELEM(PORTABLE_DEVICE_CONTROL,PORTABLE DEVICECONTROL)
        ,GENERICPAGESTRINGMAP_ELEM(INTERACTIVE_CONTROL,INTERACTIVE CONTROL)
        ,GENERICPAGESTRINGMAP_ELEM(X,X)
        ,GENERICPAGESTRINGMAP_ELEM(Y,Y)
        ,GENERICPAGESTRINGMAP_ELEM(Z,Z)
        ,GENERICPAGESTRINGMAP_ELEM(RX,RX)
        ,GENERICPAGESTRINGMAP_ELEM(RY,RY)
        ,GENERICPAGESTRINGMAP_ELEM(RZ,RZ)
        ,GENERICPAGESTRINGMAP_ELEM(SLIDER,SLIDER)
        ,GENERICPAGESTRINGMAP_ELEM(DIAL,DIAL)
        ,GENERICPAGESTRINGMAP_ELEM(WHEEL,WHEEL)
        ,GENERICPAGESTRINGMAP_ELEM(HATSWITCH, HATSWITCH)
        ,GENERICPAGESTRINGMAP_ELEM(BYTE_COUNT, BYTE COUNT)
        ,GENERICPAGESTRINGMAP_ELEM(MOTION_WAKEUP, MOTION WAKEUP)
        ,GENERICPAGESTRINGMAP_ELEM(START, START)
        ,GENERICPAGESTRINGMAP_ELEM(SELECT,SELECT)
        ,GENERICPAGESTRINGMAP_ELEM(VX,VX)
        ,GENERICPAGESTRINGMAP_ELEM(VY,VY)
        ,GENERICPAGESTRINGMAP_ELEM(VZ,VZ)
        ,GENERICPAGESTRINGMAP_ELEM(VBRX,VBRX)
        ,GENERICPAGESTRINGMAP_ELEM(VBRY,VBRY)
        ,GENERICPAGESTRINGMAP_ELEM(VBRZ,VBRZ)
        ,GENERICPAGESTRINGMAP_ELEM(VNO,VNO)
        ,GENERICPAGESTRINGMAP_ELEM(FEATURE_NOTIFICATION,FEATURE NOTIFICATION)
        ,GENERICPAGESTRINGMAP_ELEM(RESOLUTION_MULTIPLIER,RESOLUTION MULTIPLIER)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_POWER,SYSCTL POWER)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_SLEEP,SYSCTL SLEEP)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_WAKE,SYSCTL WAKE)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_CONTEXT_MENU,SYSCTL CONTEXT MENU)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_MAIN_MENU,SYSCTL MAIN MENU)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_APP_MENU,SYSCTL APP MENU)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_HELP_MENU,SYSCTL HELP MENU)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_MENU_EXIT,SYSCTL MENU EXIT)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_MENU_SELECT,SYSCTL MENU SELECT)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_MENU_RIGHT,SYSCTL MENU RIGHT)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_MENU_LEFT,SYSCTL MENU LEFT)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_MENU_UP,SYSCTL MENU UP)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_MENU_DOWN,SYSCTL MENU DOWN)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_COLD_RESTART,SYSCTL COLD RESTART)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_WARM_RESTART,SYSCTL WARM RESTART)
        ,GENERICPAGESTRINGMAP_ELEM(DPAD_UP,DPAD UP)
        ,GENERICPAGESTRINGMAP_ELEM(DPAD_DOWN,DPAD DOWN)
        ,GENERICPAGESTRINGMAP_ELEM(DPAD_RIGHT,DPAD RIGHT)
        ,GENERICPAGESTRINGMAP_ELEM(DPAD_LEFT,DPAD LEFT)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_DOCK,SYSCTL DOCK)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_UNDOCK,SYSCTL UNDOCK)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_SETUP,SYSCTL SETUP)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_SYS_BREAK,SYSCTL SYS BREAK)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_SYS_DBG_BREAK,SYSCTL SYS DBG BREAK)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_APP_BREAK,SYSCTL APP BREAK)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_APP_DBG_BREAK,SYSCTL APP DBG BREAK)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_MUTE,SYSCTL MUTE)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_HIBERNATE,SYSCTL HIBERNATE)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_DISP_INVERT,SYSCTL DISP INVERT)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_DISP_INTERNAL,SYSCTL DISP INTERNAL)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_DISP_EXTERNAL,SYSCTL DISP EXTERNAL)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_DISP_BOTH,SYSCTL DISP BOTH)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_DISP_DUAL,SYSCTL DISP DUAL)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_DISP_TOGGLE,SYSCTL DISP TOGGLE)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_DISP_SWAP,SYSCTL DISP SWAP)
        ,GENERICPAGESTRINGMAP_ELEM(SYSCTL_DISP_AUTOSCALE,SYSCTL DISP AUTOSCALE)
        ,GENERICPAGESTRINGMAP_ELEM(SYSTEM_DISPLAY_ROTATION_LOCK_BUTTON,SYSTEM DISPLAY ROTATION LOCK BUTTON)
        ,GENERICPAGESTRINGMAP_ELEM(SYSTEM_DISPLAY_ROTATION_LOCK_SLIDER_SWITCH,SYSTEM DISPLAY ROTATION LOCK SLIDER SWITCH)
        ,GENERICPAGESTRINGMAP_ELEM(CONTROL_ENABLE,CONTROL ENABLE)
#undef GENERICPAGESTRINGMAP_ELEM

      };

      return genericUsageStringMap.at(usage);

    }
    catch(...)
    {
      return{};
    }

  }

};