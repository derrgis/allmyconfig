/* Copyright (c) 2002,2003,2005,2006,2007 Marek Michalkiewicz, Joerg Wunsch
   Copyright (c) 2007 Eric B. Weddington
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

/* $Id: io.h,v 1.52.2.9 2008/11/03 04:13:14 arcanum Exp $ */

/** \file */
/** \defgroup avr_io <io.h>: AVR device-specific IO definitions
    \code #include <io.h> \endcode

    This header file includes the apropriate IO definitions for the
    device that has been specified by the <tt>-mmcu=</tt> compiler
    command-line switch.  This is done by diverting to the appropriate
    file <tt>&lt;io</tt><em>XXXX</em><tt>.h&gt;</tt> which should
    never be included directly.  Some register names common to all
    AVR devices are defined directly within <tt>&lt;common.h&gt;</tt>,
    which is included in <tt>&lt;io.h&gt;</tt>,
    but most of the details come from the respective include file.

    Note that this file always includes the following files:
    \code 
    #include <sfr_defs.h>
    #include <portpins.h>
    #include <common.h>
    #include <version.h>
    \endcode
    See \ref avr_sfr for more details about that header file.

    Included are definitions of the IO register set and their
    respective bit values as specified in the Atmel documentation.
    Note that inconsistencies in naming conventions,
    so even identical functions sometimes get different names on
    different devices.

    Also included are the specific names useable for interrupt
    function definitions as documented
    \ref avr_signames "here".

    Finally, the following macros are defined:

    - \b RAMEND
    <br>
    The last on-chip RAM address.
    <br>
    - \b XRAMEND
    <br>
    The last possible RAM location that is addressable. This is equal to 
    RAMEND for devices that do not allow for external RAM. For devices 
    that allow external RAM, this will larger than RAMEND.
    <br>
    - \b E2END
    <br>
    The last EEPROM address.
    <br>
    - \b FLASHEND
    <br>
    The last byte address in the Flash program space.
    <br>
    - \b SPM_PAGESIZE
    <br>
    For devices with bootloader support, the flash pagesize
    (in bytes) to be used for the \c SPM instruction. 
    - \b E2PAGESIZE
    <br>
    The size of the EEPROM page.
    
*/

#ifndef _AVR_IO_H_
#define _AVR_IO_H_

#include <sfr_defs.h>

#if defined (__AVR_AT94K__)
#  include <ioat94k.h>
#elif defined (__AVR_AT43USB320__)
#  include <io43u32x.h>
#elif defined (__AVR_AT43USB355__)
#  include <io43u35x.h>
#elif defined (__AVR_AT76C711__)
#  include <io76c711.h>
#elif defined (__AVR_AT86RF401__)
#  include <io86r401.h>
#elif defined (__AVR_AT90PWM1__)
#  include <io90pwm1.h>
#elif defined (__AVR_AT90PWM2__)
#  include <io90pwmx.h>
#elif defined (__AVR_AT90PWM2B__)
#  include <io90pwm2b.h>
#elif defined (__AVR_AT90PWM3__)
#  include <io90pwmx.h>
#elif defined (__AVR_AT90PWM3B__)
#  include <io90pwm3b.h>
#elif defined (__AVR_AT90PWM216__)
#  include <io90pwm216.h>
#elif defined (__AVR_AT90PWM316__)
#  include <io90pwm316.h>
#elif defined (__AVR_ATmega32C1__)
#  include <iom32c1.h>
#elif defined (__AVR_ATmega32M1__)
#  include <iom32m1.h>
#elif defined (__AVR_ATmega32U4__)
#  include <iom32u4.h>
#elif defined (__AVR_ATmega32U6__)
#  include <iom32u6.h>
#elif defined (__AVR_ATmega128__)
#  include <iom128.h>
#elif defined (__AVR_ATmega1280__)
#  include <iom1280.h>
#elif defined (__AVR_ATmega1281__)
#  include <iom1281.h>
#elif defined (__AVR_ATmega1284P__)
#  include <iom1284p.h>
#elif defined (__AVR_ATmega2560__)
#  include <iom2560.h>
#elif defined (__AVR_ATmega2561__)
#  include <iom2561.h>
#elif defined (__AVR_AT90CAN32__)
#  include <iocan32.h>
#elif defined (__AVR_AT90CAN64__)
#  include <iocan64.h>
#elif defined (__AVR_AT90CAN128__)
#  include <iocan128.h>
#elif defined (__AVR_AT90USB82__)
#  include <iousb82.h>
#elif defined (__AVR_AT90USB162__)
#  include <iousb162.h>
#elif defined (__AVR_AT90USB646__)
#  include <iousb646.h>
#elif defined (__AVR_AT90USB647__)
#  include <iousb647.h>
#elif defined (__AVR_AT90USB1286__)
#  include <iousb1286.h>
#elif defined (__AVR_AT90USB1287__)
#  include <iousb1287.h>
#elif defined (__AVR_ATmega64__)
#  include <iom64.h>
#elif defined (__AVR_ATmega640__)
#  include <iom640.h>
#elif defined (__AVR_ATmega644__)
#  include <iom644.h>
#elif defined (__AVR_ATmega644P__)
#  include <iom644.h>
#elif defined (__AVR_ATmega645__)
#  include <iom645.h>
#elif defined (__AVR_ATmega6450__)
#  include <iom6450.h>
#elif defined (__AVR_ATmega649__)
#  include <iom649.h>
#elif defined (__AVR_ATmega6490__)
#  include <iom6490.h>
#elif defined (__AVR_ATmega103__)
#  include <iom103.h>
#elif defined (__AVR_ATmega32__)
#  include <iom32.h>
#elif defined (__AVR_ATmega323__)
#  include <iom323.h>
#elif defined (__AVR_ATmega324P__)
#  include <iom324.h>
#elif defined (__AVR_ATmega325__)
#  include <iom325.h>
#elif defined (__AVR_ATmega325P__)
#  include <iom325.h>
#elif defined (__AVR_ATmega3250__)
#  include <iom3250.h>
#elif defined (__AVR_ATmega3250P__)
#  include <iom3250.h>
#elif defined (__AVR_ATmega328P__)
#  include <iom328p.h>
#elif defined (__AVR_ATmega329__)
#  include <iom329.h>
#elif defined (__AVR_ATmega329P__)
#  include <iom329.h>
#elif defined (__AVR_ATmega3290__)
#  include <iom3290.h>
#elif defined (__AVR_ATmega3290P__)
#  include <iom3290.h>
#elif defined (__AVR_ATmega32HVB__)
#  include <iom32hvb.h>
#elif defined (__AVR_ATmega406__)
#  include <iom406.h>
#elif defined (__AVR_ATmega16__)
#  include <iom16.h>
#elif defined (__AVR_ATmega161__)
#  include <iom161.h>
#elif defined (__AVR_ATmega162__)
#  include <iom162.h>
#elif defined (__AVR_ATmega163__)
#  include <iom163.h>
#elif defined (__AVR_ATmega164P__)
#  include <iom164.h>
#elif defined (__AVR_ATmega165__)
#  include <iom165.h>
#elif defined (__AVR_ATmega165P__)
#  include <iom165p.h>
#elif defined (__AVR_ATmega168__)
#  include <iom168.h>
#elif defined (__AVR_ATmega168P__)
#  include <iom168p.h>
#elif defined (__AVR_ATmega169__)
#  include <iom169.h>
#elif defined (__AVR_ATmega169P__)
#  include <iom169p.h>
#elif defined (__AVR_ATmega8HVA__)
#  include <iom8hva.h>
#elif defined (__AVR_ATmega16HVA__)
#  include <iom16hva.h>
#elif defined (__AVR_ATmega8__)
#  include <iom8.h>
#elif defined (__AVR_ATmega48__)
#  include <iom48.h>
#elif defined (__AVR_ATmega48P__)
#  include <iom48p.h>
#elif defined (__AVR_ATmega88__)
#  include <iom88.h>
#elif defined (__AVR_ATmega88P__)
#  include <iom88p.h>
#elif defined (__AVR_ATmega8515__)
#  include <iom8515.h>
#elif defined (__AVR_ATmega8535__)
#  include <iom8535.h>
#elif defined (__AVR_AT90S8535__)
#  include <io8535.h>
#elif defined (__AVR_AT90C8534__)
#  include <io8534.h>
#elif defined (__AVR_AT90S8515__)
#  include <io8515.h>
#elif defined (__AVR_AT90S4434__)
#  include <io4434.h>
#elif defined (__AVR_AT90S4433__)
#  include <io4433.h>
#elif defined (__AVR_AT90S4414__)
#  include <io4414.h>
#elif defined (__AVR_ATtiny22__)
#  include <iotn22.h>
#elif defined (__AVR_ATtiny26__)
#  include <iotn26.h>
#elif defined (__AVR_AT90S2343__)
#  include <io2343.h>
#elif defined (__AVR_AT90S2333__)
#  include <io2333.h>
#elif defined (__AVR_AT90S2323__)
#  include <io2323.h>
#elif defined (__AVR_AT90S2313__)
#  include <io2313.h>
#elif defined (__AVR_ATtiny2313__)
#  include <iotn2313.h>
#elif defined (__AVR_ATtiny13__)
#  include <iotn13.h>
#elif defined (__AVR_ATtiny13A__)
#  include <iotn13a.h>
#elif defined (__AVR_ATtiny25__)
#  include <iotn25.h>
#elif defined (__AVR_ATtiny45__)
#  include <iotn45.h>
#elif defined (__AVR_ATtiny85__)
#  include <iotn85.h>
#elif defined (__AVR_ATtiny24__)
#  include <iotn24.h>
#elif defined (__AVR_ATtiny44__)
#  include <iotn44.h>
#elif defined (__AVR_ATtiny84__)
#  include <iotn84.h>
#elif defined (__AVR_ATtiny261__)
#  include <iotn261.h>
#elif defined (__AVR_ATtiny461__)
#  include <iotn461.h>
#elif defined (__AVR_ATtiny861__)
#  include <iotn861.h>
#elif defined (__AVR_ATtiny43U__)
#  include <iotn43u.h>
#elif defined (__AVR_ATtiny48__)
#  include <iotn48.h>
#elif defined (__AVR_ATtiny88__)
#  include <iotn88.h>
#elif defined (__AVR_ATtiny167__)
#  include <iotn167.h>
/* avr1: the following only supported for assembler programs */
#elif defined (__AVR_ATtiny28__)
#  include <iotn28.h>
#elif defined (__AVR_AT90S1200__)
#  include <io1200.h>
#elif defined (__AVR_ATtiny15__)
#  include <iotn15.h>
#elif defined (__AVR_ATtiny12__)
#  include <iotn12.h>
#elif defined (__AVR_ATtiny11__)
#  include <iotn11.h>
#elif defined (__AVR_ATxmega64A1__)
#  include <iox64a1.h>
#elif defined (__AVR_ATxmega64A3__)
#  include <iox64a3.h>
#elif defined (__AVR_ATxmega128A1__)
#  include <iox128a1.h>
#elif defined (__AVR_ATxmega128A3__)
#  include <iox128a3.h>
#elif defined (__AVR_ATxmega256A3__)
#  include <iox256a3.h>
#elif defined (__AVR_ATxmega256A3B__)
#  include <iox256a3b.h>
#else
#  if !defined(__COMPILING_AVR_LIBC__)
#    warning "device type not defined"
#  endif
#endif

#include <portpins.h>

#include <common.h>

#include <version.h>

/* Include fuse.h after individual IO header files. */
#include <fuse.h>

/* Include lock.h after individual IO header files. */
#include <lock.h>

#endif /* _AVR_IO_H_ */
