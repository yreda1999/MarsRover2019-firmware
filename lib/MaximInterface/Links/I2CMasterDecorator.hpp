/*******************************************************************************
* Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************/

#ifndef MaximInterface_I2CMasterDecorator
#define MaximInterface_I2CMasterDecorator

#include <MaximInterface/Utilities/Export.h>
#include "I2CMaster.hpp"

namespace MaximInterface {

class I2CMasterDecorator : public I2CMaster {
protected:
  explicit I2CMasterDecorator(I2CMaster & master) : master(&master) {}

public:
  void setMaster(I2CMaster & master) { this->master = &master; }

  MaximInterface_EXPORT virtual error_code start(uint_least8_t address);
  
  MaximInterface_EXPORT virtual error_code stop();
  
  MaximInterface_EXPORT virtual error_code writeByte(uint_least8_t data);
  
  MaximInterface_EXPORT virtual error_code
  writeBlock(span<const uint_least8_t> data);
  
  MaximInterface_EXPORT virtual error_code readByte(AckStatus status,
                                                    uint_least8_t & data);
  
  MaximInterface_EXPORT virtual error_code
  readBlock(AckStatus status, span<uint_least8_t> data);

protected:
  MaximInterface_EXPORT virtual error_code
  writePacketImpl(uint_least8_t address, span<const uint_least8_t> data,
                  bool sendStop);
                  
  MaximInterface_EXPORT virtual error_code
  readPacketImpl(uint_least8_t address, span<uint_least8_t> data,
                 bool sendStop);

private:
  I2CMaster * master;
};

} // namespace MaximInterface

#endif