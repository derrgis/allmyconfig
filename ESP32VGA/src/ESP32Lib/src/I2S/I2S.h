/*
	Author: bitluni 2019
	License: 
	Creative Commons Attribution ShareAlike 2.0
	https://creativecommons.org/licenses/by-sa/2.0/
	
	For further details check out: 
		https://youtube.com/bitlunislab
		https://github.com/bitluni
		http://bitluni.net
*/
#pragma once

#include "esp_heap_caps.h"
#include "soc/soc.h"
#include "soc/gpio_sig_map.h"
#include "soc/i2s_reg.h"
#include "soc/i2s_struct.h"
#include "soc/io_mux_reg.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "rom/lldesc.h"
#include "DMABuffer.h"

class I2S
{
  public:
	int i2sIndex;
	intr_handle_t interruptHandle;
	int dmaBufferCount;
	int dmaBufferActive;
	DMABuffer **dmaBuffers;
	volatile bool stopSignal;

	/// hardware index [0, 1]
	I2S(const int i2sIndex = 0);
	void reset();

	void stop();

	void i2sStop();
	void startTX();
	void startRX();

	void resetDMA();
	void resetFIFO();
	bool initParallelOutputMode(const int *pinMap, long APLLFreq = 100000, int baseClock = -1, int wordSelect = -1);
	bool initParallelInputMode(const int *pinMap, long sampleRate = 100000, int baseClock = -1, int wordSelect = -1);

	void allocateDMABuffers(int count, int bytes);
	void deleteDMABuffers();
    void (*callback)(void);
    
  protected:
	virtual void interrupt();
	
  private:
	static void IRAM_ATTR interrupt(void *arg);
};
