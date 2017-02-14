/*
    SystemTera eib bus access driver
    Copyright (C) 2014 Patrik Pfaffenbauer <patrik.pfaffenbauer@p3.co.at>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "ncn5120.h"

NCN5120SerialDriverDriver::NCN5120SerialDriverDriver (LinkConnectPtr c, IniSection& s)
    : AutoRegister_(c,s) { }

NCN5120SerialDriverDriver::~NCN5120SerialDriverDriver ()
{ }

void
NCN5120SerialDriverDriver::termios_settings(struct termios &t1)
{
    t1.c_cflag = CS8 | CLOCAL | CREAD;
    t1.c_iflag = IGNBRK | INPCK | ISIG;
    t1.c_oflag = 0;
    t1.c_lflag = 0;
    t1.c_cc[VTIME] = 1;
    t1.c_cc[VMIN] = 0;
}

unsigned int NCN5120SerialDriverDriver::default_baudrate()
{
    return 38400;
}

void NCN5120SerialDriverDriver::RecvLPDU (const uchar * data, int len)
{
    skip_char = true;
    TPUARTSerialDriverDriver::RecvLPDU (data, len);
}

void
NCN5120SerialDriverDriver::setstate(enum TSTATE new_state)
{
  if (new_state == T_in_setaddr && my_addr != 0)
    {
      uint8_t addrbuf[4] = { 0xF1, (uint8_t)((my_addr>>8)&0xFF), (uint8_t)(my_addr&0xFF), 0x00 };
      TRACEPRINTF (t, 0, "SendAddr %02X%02X%02X", addrbuf[0],addrbuf[1],addrbuf[2]);
      sendbuf.write(addrbuf, sizeof(addrbuf));
      new_state = T_in_getstate;
    }
  TPUARTSerialDriverDriver::setstate(new_state);
}

