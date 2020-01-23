//
//  compet.h
//  
//
//  Created by Yves BAZIN on 25/01/18.
//

#ifndef compet_h
#define compet_h



#include <string.h>
#include <stdbool.h>

#include "driver/spi_master.h"
#include "soc/gpio_sig_map.h"
#include "soc/spi_reg.h"
#include "soc/dport_reg.h"
#include "soc/spi_struct.h"
#include "rom/ets_sys.h"
#include "esp_types.h"
#include "esp_attr.h"
#include "esp_intr.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "esp_err.h"
#include "soc/soc.h"
#include "soc/dport_reg.h"
#include "rom/lldesc.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "esp_heap_caps.h"
#include "malloc.h"
#include "driver/spi_common.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define SPI_CHECK(a, str, ret_val) \
if (!(a)) { \
ESP_LOGE(SPI_TAG,"%s(%d): %s", __FUNCTION__, __LINE__, str); \
return (ret_val); \
}

typedef struct spi_device_t spi_device_t;

/*
 Stores a bunch of per-spi-peripheral data.
 */
typedef struct {
    const uint8_t spiclk_out;       //GPIO mux output signals
    const uint8_t spiclk_in;
    const uint8_t spid_out;
    const uint8_t spiq_out;
    const uint8_t spiwp_out;
    const uint8_t spihd_out;
    const uint8_t spid_in;          //GPIO mux input signals
    const uint8_t spiq_in;
    const uint8_t spiwp_in;
    const uint8_t spihd_in;
    const uint8_t spics_out[3];     // /CS GPIO output mux signals
    const uint8_t spics_in;
    const uint8_t spiclk_native;    //IO pins of IO_MUX muxed signals
    const uint8_t spid_native;
    const uint8_t spiq_native;
    const uint8_t spiwp_native;
    const uint8_t spihd_native;
    const uint8_t spics0_native;
    const uint8_t irq;              //irq source for interrupt mux
    const uint8_t irq_dma;          //dma irq source for interrupt mux
    const periph_module_t module;   //peripheral module, for enabling clock etc
    spi_dev_t *hw;              //Pointer to the hardware registers
} spi_signal_conn_t;

static const spi_signal_conn_t io_signal[3] = {
    {
        .spiclk_out = SPICLK_OUT_IDX,
        .spiclk_in = SPICLK_IN_IDX,
        .spid_out = SPID_OUT_IDX,
        .spiq_out = SPIQ_OUT_IDX,
        .spiwp_out = SPIWP_OUT_IDX,
        .spihd_out = SPIHD_OUT_IDX,
        .spid_in = SPID_IN_IDX,
        .spiq_in = SPIQ_IN_IDX,
        .spiwp_in = SPIWP_IN_IDX,
        .spihd_in = SPIHD_IN_IDX,
        .spics_out = {SPICS0_OUT_IDX, SPICS1_OUT_IDX, SPICS2_OUT_IDX},
        .spics_in = SPICS0_IN_IDX,
        .spiclk_native = 6,
        .spid_native = 8,
        .spiq_native = 7,
        .spiwp_native = 10,
        .spihd_native = 9,
        .spics0_native = 11,
        .irq = ETS_SPI1_INTR_SOURCE,
        .irq_dma = ETS_SPI1_DMA_INTR_SOURCE,
        .module = PERIPH_SPI_MODULE,
        .hw = &SPI1
    }, {
        .spiclk_out = HSPICLK_OUT_IDX,
        .spiclk_in = HSPICLK_IN_IDX,
        .spid_out = HSPID_OUT_IDX,
        .spiq_out = HSPIQ_OUT_IDX,
        .spiwp_out = HSPIWP_OUT_IDX,
        .spihd_out = HSPIHD_OUT_IDX,
        .spid_in = HSPID_IN_IDX,
        .spiq_in = HSPIQ_IN_IDX,
        .spiwp_in = HSPIWP_IN_IDX,
        .spihd_in = HSPIHD_IN_IDX,
        .spics_out = {HSPICS0_OUT_IDX, HSPICS1_OUT_IDX, HSPICS2_OUT_IDX},
        .spics_in = HSPICS0_IN_IDX,
        .spiclk_native = 14,
        .spid_native = 13,
        .spiq_native = 12,
        .spiwp_native = 2,
        .spihd_native = 4,
        .spics0_native = 15,
        .irq = ETS_SPI2_INTR_SOURCE,
        .irq_dma = ETS_SPI2_DMA_INTR_SOURCE,
        .module = PERIPH_HSPI_MODULE,
        .hw = &SPI2
    }, {
        .spiclk_out = VSPICLK_OUT_IDX,
        .spiclk_in = VSPICLK_IN_IDX,
        .spid_out = VSPID_OUT_IDX,
        .spiq_out = VSPIQ_OUT_IDX,
        .spiwp_out = VSPIWP_OUT_IDX,
        .spihd_out = VSPIHD_OUT_IDX,
        .spid_in = VSPID_IN_IDX,
        .spiq_in = VSPIQ_IN_IDX,
        .spiwp_in = VSPIWP_IN_IDX,
        .spihd_in = VSPIHD_IN_IDX,
        .spics_out = {VSPICS0_OUT_IDX, VSPICS1_OUT_IDX, VSPICS2_OUT_IDX},
        .spics_in = VSPICS0_IN_IDX,
        .spiclk_native = 18,
        .spid_native = 23,
        .spiq_native = 19,
        .spiwp_native = 22,
        .spihd_native = 21,
        .spics0_native = 5,
        .irq = ETS_SPI3_INTR_SOURCE,
        .irq_dma = ETS_SPI3_DMA_INTR_SOURCE,
        .module = PERIPH_VSPI_MODULE,
        .hw = &SPI3
    }
};

    typedef struct spi_device_t spi_device_t;
    typedef typeof(SPI1.clock) spi_clock_reg_t;
    
#define NO_CS 3     //Number of CS pins per SPI host
    
    
    /// struct to hold private transaction data (like tx and rx buffer for DMA).
    typedef struct {
        spi_transaction_t   *trans;
        uint32_t *buffer_to_send;   //equals to tx_data, if SPI_TRANS_USE_RXDATA is applied; otherwise if original buffer wasn't in DMA-capable memory, this gets the address of a temporary buffer that is;
        //otherwise sets to the original buffer or NULL if no buffer is assigned.
        uint32_t *buffer_to_rcv;    // similar to buffer_to_send
    } spi_trans_priv;

    typedef struct {
        spi_device_t *device[NO_CS];
        intr_handle_t intr;
        spi_dev_t *hw;
        spi_trans_priv cur_trans_buf;
        int cur_cs;
        int prev_cs;
        lldesc_t *dmadesc_tx;
        lldesc_t *dmadesc_rx;
        bool no_gpio_matrix;
        int dma_chan;
        int max_transfer_sz;
#ifdef CONFIG_PM_ENABLE
        esp_pm_lock_handle_t pm_lock;
#endif
    } spi_host_t;
 
    typedef struct {
        spi_clock_reg_t reg;
        int eff_clk;
    } clock_config_t;
    
    struct spi_device_t {
        QueueHandle_t trans_queue;
        QueueHandle_t ret_queue;
        spi_device_interface_config_t cfg;
        clock_config_t clk_cfg;
        spi_host_t *host;
    };

static spi_host_t *spihost[3];


    gpio_num_t mappPIN[32]= {
        GPIO_NUM_0,
        GPIO_NUM_1,
        GPIO_NUM_2,
        GPIO_NUM_3,
        GPIO_NUM_4,
        GPIO_NUM_5,
        GPIO_NUM_6,
        GPIO_NUM_7,
        GPIO_NUM_8,
        GPIO_NUM_9,
        GPIO_NUM_10,
        GPIO_NUM_11,
        GPIO_NUM_12,
        GPIO_NUM_13,
        GPIO_NUM_14,
        GPIO_NUM_15,
        GPIO_NUM_16,
        GPIO_NUM_17,
        GPIO_NUM_18,
        GPIO_NUM_19,
        GPIO_NUM_2,
        GPIO_NUM_21,
        GPIO_NUM_22,
        GPIO_NUM_23,
        GPIO_NUM_2,
        GPIO_NUM_25,
        GPIO_NUM_26,
        GPIO_NUM_27,
        GPIO_NUM_2,

    };




esp_err_t spicommon_bus_initialize_io2(spi_host_device_t host, const spi_bus_config_t *bus_config, int dma_chan, int flags, bool *is_native)
{
    bool native = true;
    bool use_quad = (flags & SPICOMMON_BUSFLAG_QUAD) != 0;
    
    SPI_CHECK(bus_config->mosi_io_num < 0 || GPIO_IS_VALID_OUTPUT_GPIO(bus_config->mosi_io_num), "spid pin invalid", ESP_ERR_INVALID_ARG);
    SPI_CHECK(bus_config->sclk_io_num < 0 || GPIO_IS_VALID_OUTPUT_GPIO(bus_config->sclk_io_num), "spiclk pin invalid", ESP_ERR_INVALID_ARG);
    SPI_CHECK(bus_config->miso_io_num < 0 || GPIO_IS_VALID_GPIO(bus_config->miso_io_num), "spiq pin invalid", ESP_ERR_INVALID_ARG);
    if (use_quad) {
        SPI_CHECK(bus_config->quadwp_io_num < 0 || GPIO_IS_VALID_OUTPUT_GPIO(bus_config->quadwp_io_num), "spiwp pin invalid", ESP_ERR_INVALID_ARG);
        SPI_CHECK(bus_config->quadhd_io_num < 0 || GPIO_IS_VALID_OUTPUT_GPIO(bus_config->quadhd_io_num), "spihd pin invalid", ESP_ERR_INVALID_ARG);
    }
    
    //Check if the selected pins correspond to the native pins of the peripheral
    if (bus_config->mosi_io_num >= 0 && bus_config->mosi_io_num != io_signal[host].spid_native) native = false;
    if (bus_config->miso_io_num >= 0 && bus_config->miso_io_num != io_signal[host].spiq_native) native = false;
    if (bus_config->sclk_io_num >= 0 && bus_config->sclk_io_num != io_signal[host].spiclk_native) native = false;
    if (use_quad) {
        if (bus_config->quadwp_io_num >= 0 && bus_config->quadwp_io_num != io_signal[host].spiwp_native) native = false;
        if (bus_config->quadhd_io_num >= 0 && bus_config->quadhd_io_num != io_signal[host].spihd_native) native = false;
    }
    
    *is_native = native;
    if ( native ) {
        ESP_LOGD(SPI_TAG, "SPI%d use native pins.", host );
    } else {
        ESP_LOGD(SPI_TAG, "SPI%d use gpio matrix.", host );
    }
    
    if (native) {
        //All SPI native pin selections resolve to 1, so we put that here instead of trying to figure
        //out which FUNC_GPIOx_xSPIxx to grab; they all are defined to 1 anyway.
        if (bus_config->mosi_io_num > 0) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->mosi_io_num], 1);
        if (bus_config->miso_io_num > 0) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->miso_io_num], 1);
        if (use_quad && bus_config->quadwp_io_num > 0) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->quadwp_io_num], 1);
        if (use_quad && bus_config->quadhd_io_num > 0) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->quadhd_io_num], 1);
        if (bus_config->sclk_io_num > 0) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->sclk_io_num], 1);
    } else {
        //Use GPIO
        if (bus_config->mosi_io_num > 0) {
            PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->mosi_io_num], PIN_FUNC_GPIO);
            gpio_set_direction(mappPIN[bus_config->mosi_io_num], GPIO_MODE_INPUT_OUTPUT);
            gpio_matrix_out(bus_config->mosi_io_num, io_signal[host].spid_out, false, false);
            gpio_matrix_in(bus_config->mosi_io_num, io_signal[host].spid_in, false);
        }
        if (bus_config->miso_io_num > 0) {
            PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->miso_io_num], PIN_FUNC_GPIO);
            gpio_set_direction(mappPIN[bus_config->miso_io_num], GPIO_MODE_INPUT_OUTPUT);
            gpio_matrix_out(bus_config->miso_io_num, io_signal[host].spiq_out, false, false);
            gpio_matrix_in(bus_config->miso_io_num, io_signal[host].spiq_in, false);
        }
        if (use_quad && bus_config->quadwp_io_num > 0) {
            PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->quadwp_io_num], PIN_FUNC_GPIO);
            gpio_set_direction(mappPIN[bus_config->quadwp_io_num], GPIO_MODE_INPUT_OUTPUT);
            gpio_matrix_out(bus_config->quadwp_io_num, io_signal[host].spiwp_out, false, false);
            gpio_matrix_in(bus_config->quadwp_io_num, io_signal[host].spiwp_in, false);
        }
        if (use_quad && bus_config->quadhd_io_num > 0) {
            PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->quadhd_io_num], PIN_FUNC_GPIO);
            gpio_set_direction(mappPIN[bus_config->quadhd_io_num], GPIO_MODE_INPUT_OUTPUT);
            gpio_matrix_out(bus_config->quadhd_io_num, io_signal[host].spihd_out, false, false);
            gpio_matrix_in(bus_config->quadhd_io_num, io_signal[host].spihd_in, false);
        }
        if (bus_config->sclk_io_num > 0) {
            PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[bus_config->sclk_io_num], PIN_FUNC_GPIO);
            gpio_set_direction(mappPIN[bus_config->sclk_io_num], GPIO_MODE_INPUT_OUTPUT);
            gpio_matrix_out(bus_config->sclk_io_num, io_signal[host].spiclk_out, false, false);
            gpio_matrix_in(bus_config->sclk_io_num, io_signal[host].spiclk_in, false);
        }
    }
    
    //Select DMA channel.
    DPORT_SET_PERI_REG_BITS(DPORT_SPI_DMA_CHAN_SEL_REG, 3, dma_chan, (host * 2));
    
    return ESP_OK;
}


//Find any pin with output muxed to ``func`` and reset it to GPIO
static void reset_func_to_gpio(int func)
{
    for (int x = 0; x < GPIO_PIN_COUNT; x++) {
        if (GPIO_IS_VALID_GPIO(x) && (READ_PERI_REG(GPIO_FUNC0_OUT_SEL_CFG_REG + (x * 4))&GPIO_FUNC0_OUT_SEL_M) == func)  {
            gpio_matrix_out(x, SIG_GPIO_OUT_IDX, false, false);
        }
    }
}


esp_err_t spicommon_bus_free_io2(spi_host_device_t host)
{
    if (REG_GET_FIELD(GPIO_PIN_MUX_REG[io_signal[host].spid_native], MCU_SEL) == 1) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[io_signal[host].spid_native], PIN_FUNC_GPIO);
    if (REG_GET_FIELD(GPIO_PIN_MUX_REG[io_signal[host].spiq_native], MCU_SEL) == 1) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[io_signal[host].spiq_native], PIN_FUNC_GPIO);
    if (REG_GET_FIELD(GPIO_PIN_MUX_REG[io_signal[host].spiclk_native], MCU_SEL) == 1) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[io_signal[host].spiclk_native], PIN_FUNC_GPIO);
    if (REG_GET_FIELD(GPIO_PIN_MUX_REG[io_signal[host].spiwp_native], MCU_SEL) == 1) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[io_signal[host].spiwp_native], PIN_FUNC_GPIO);
    if (REG_GET_FIELD(GPIO_PIN_MUX_REG[io_signal[host].spihd_native], MCU_SEL) == 1) PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[io_signal[host].spihd_native], PIN_FUNC_GPIO);
    reset_func_to_gpio(io_signal[host].spid_out);
    reset_func_to_gpio(io_signal[host].spiq_out);
    reset_func_to_gpio(io_signal[host].spiclk_out);
    reset_func_to_gpio(io_signal[host].spiwp_out);
    reset_func_to_gpio(io_signal[host].spihd_out);
    return ESP_OK;
}

void spicommon_cs_initialize2(spi_host_device_t host, int cs_io_num, int cs_num, int force_gpio_matrix)
{
    if (!force_gpio_matrix && cs_io_num == io_signal[host].spics0_native && cs_num == 0) {
        //The cs0s for all SPI peripherals map to pin mux source 1, so we use that instead of a define.
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[cs_io_num], 1);
    } else {
        //Use GPIO matrix
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[cs_io_num], PIN_FUNC_GPIO);
        gpio_matrix_out(cs_io_num, io_signal[host].spics_out[cs_num], false, false);
        if (cs_num == 0) gpio_matrix_in(cs_io_num, io_signal[host].spics_in, false);
    }
}

void spicommon_cs_free2(spi_host_device_t host, int cs_io_num)
{
    if (cs_io_num == 0 && REG_GET_FIELD(GPIO_PIN_MUX_REG[io_signal[host].spics0_native], MCU_SEL) == 1) {
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[io_signal[host].spics0_native], PIN_FUNC_GPIO);
    }
    reset_func_to_gpio(io_signal[host].spics_out[cs_io_num]);
}




esp_err_t spi_bus_initialize2(spi_host_device_t host, const spi_bus_config_t *bus_config, int dma_chan)
{
    bool native, spi_chan_claimed, dma_chan_claimed;
    /* ToDo: remove this when we have flash operations cooperating with this */
    SPI_CHECK(host!=SPI_HOST, "SPI1 is not supported", ESP_ERR_NOT_SUPPORTED);
    
    SPI_CHECK(host>=SPI_HOST && host<=VSPI_HOST, "invalid host", ESP_ERR_INVALID_ARG);
    SPI_CHECK( dma_chan >= 0 && dma_chan <= 2, "invalid dma channel", ESP_ERR_INVALID_ARG );
    
    spi_chan_claimed=spicommon_periph_claim(host);
    SPI_CHECK(spi_chan_claimed, "host already in use", ESP_ERR_INVALID_STATE);
    
    if ( dma_chan != 0 ) {
        dma_chan_claimed=spicommon_dma_chan_claim(dma_chan);
        if ( !dma_chan_claimed ) {
            spicommon_periph_free( host );
            SPI_CHECK(dma_chan_claimed, "dma channel already in use", ESP_ERR_INVALID_STATE);
        }
    }
    
    spihost[host]=(spi_host_t*)malloc(sizeof(spi_host_t));
    if (spihost[host]==NULL) goto nomem;
    memset(spihost[host], 0, sizeof(spi_host_t));
#ifdef CONFIG_PM_ENABLE
    esp_err_t err = esp_pm_lock_create(ESP_PM_APB_FREQ_MAX, 0, "spi_master",
                                       &spihost[host]->pm_lock);
    if (err != ESP_OK) {
        goto nomem;
    }
#endif //CONFIG_PM_ENABLE
   
    spicommon_bus_initialize_io(host, bus_config, dma_chan, SPICOMMON_BUSFLAG_MASTER|SPICOMMON_BUSFLAG_QUAD, &native);
    spihost[host]->no_gpio_matrix=native;
    
    spihost[host]->dma_chan=dma_chan;
    if (dma_chan == 0) {
        spihost[host]->max_transfer_sz = 32;
    } else {
        //See how many dma descriptors we need and allocate them
        int dma_desc_ct=(bus_config->max_transfer_sz+SPI_MAX_DMA_LEN-1)/SPI_MAX_DMA_LEN;
        if (dma_desc_ct==0) dma_desc_ct=1; //default to 4k when max is not given
        spihost[host]->max_transfer_sz = dma_desc_ct*SPI_MAX_DMA_LEN;
        spihost[host]->dmadesc_tx=(lldesc_t*)heap_caps_malloc(sizeof(lldesc_t)*dma_desc_ct, MALLOC_CAP_DMA);
        spihost[host]->dmadesc_rx=(lldesc_t*)heap_caps_malloc(sizeof(lldesc_t)*dma_desc_ct, MALLOC_CAP_DMA);
        if (!spihost[host]->dmadesc_tx || !spihost[host]->dmadesc_rx) goto nomem;
    }
    esp_intr_alloc(spicommon_irqsource_for_host(host), ESP_INTR_FLAG_INTRDISABLED, spi_intr, (void*)spihost[host], &spihost[host]->intr);
    spihost[host]->hw=spicommon_hw_for_host(host);
    
    spihost[host]->cur_cs = NO_CS;
    spihost[host]->prev_cs = NO_CS;
    
    //Reset DMA
    spihost[host]->hw->dma_conf.val|=SPI_OUT_RST|SPI_IN_RST|SPI_AHBM_RST|SPI_AHBM_FIFO_RST;
    spihost[host]->hw->dma_out_link.start=0;
    spihost[host]->hw->dma_in_link.start=0;
    spihost[host]->hw->dma_conf.val&=~(SPI_OUT_RST|SPI_IN_RST|SPI_AHBM_RST|SPI_AHBM_FIFO_RST);
    //Reset timing
    spihost[host]->hw->ctrl2.val=0;
    
    //Disable unneeded ints
    spihost[host]->hw->slave.rd_buf_done=0;
    spihost[host]->hw->slave.wr_buf_done=0;
    spihost[host]->hw->slave.rd_sta_done=0;
    spihost[host]->hw->slave.wr_sta_done=0;
    spihost[host]->hw->slave.rd_buf_inten=0;
    spihost[host]->hw->slave.wr_buf_inten=0;
    spihost[host]->hw->slave.rd_sta_inten=0;
    spihost[host]->hw->slave.wr_sta_inten=0;
    
    //Force a transaction done interrupt. This interrupt won't fire yet because we initialized the SPI interrupt as
    //disabled.  This way, we can just enable the SPI interrupt and the interrupt handler will kick in, handling
    //any transactions that are queued.
    spihost[host]->hw->slave.trans_inten=1;
    spihost[host]->hw->slave.trans_done=1;
    
    return ESP_OK;
    
nomem:
    if (spihost[host]) {
        free(spihost[host]->dmadesc_tx);
        free(spihost[host]->dmadesc_rx);
#ifdef CONFIG_PM_ENABLE
        if (spihost[host]->pm_lock) {
            esp_pm_lock_delete(spihost[host]->pm_lock);
        }
#endif
    }
    free(spihost[host]);
    spicommon_periph_free(host);
    spicommon_dma_chan_free(dma_chan);
    return ESP_ERR_NO_MEM;
}
/*
esp_err_t spi_bus_free2(spi_host_device_t host)
{
    int x;
    SPI_CHECK(host>=SPI_HOST && host<=VSPI_HOST, "invalid host", ESP_ERR_INVALID_ARG);
    SPI_CHECK(spihost[host]!=NULL, "host not in use", ESP_ERR_INVALID_STATE);
    for (x=0; x<NO_CS; x++) {
        SPI_CHECK(spihost[host]->device[x]==NULL, "not all CSses freed", ESP_ERR_INVALID_STATE);
    }
    
    if ( spihost[host]->dma_chan > 0 ) {
        spicommon_dma_chan_free ( spihost[host]->dma_chan );
    }
#ifdef CONFIG_PM_ENABLE
    esp_pm_lock_delete(spihost[host]->pm_lock);
#endif
    spihost[host]->hw->slave.trans_inten=0;
    spihost[host]->hw->slave.trans_done=0;
    esp_intr_free(spihost[host]->intr);
    spicommon_periph_free(host);
    free(spihost[host]->dmadesc_tx);
    free(spihost[host]->dmadesc_rx);
    free(spihost[host]);
    spihost[host]=NULL;
    return ESP_OK;
}

*/
/*
static inline void spi_set_clock(spi_dev_t *hw, spi_clock_reg_t reg) {
    hw->clock.val = reg.val;
}

static void IRAM_ATTR spi_intr(void *arg)
{
    int i;
    BaseType_t r;
    BaseType_t do_yield=pdFALSE;
    spi_trans_priv *trans_buf=NULL;
    spi_transaction_t *trans=NULL;
    spi_host_t *host=(spi_host_t*)arg;
    
    //Ignore all but the trans_done int.
    if (!host->hw->slave.trans_done) return;
    
   
    if (host->cur_cs != NO_CS) {
        spi_transaction_t *cur_trans = host->cur_trans_buf.trans;
        //Okay, transaction is done.
        if (host->cur_trans_buf.buffer_to_rcv && host->dma_chan == 0 ) {
            //Need to copy from SPI regs to result buffer.
            for (int x=0; x < cur_trans->rxlength; x+=32) {
                //Do a memcpy to get around possible alignment issues in rx_buffer
                uint32_t word=host->hw->data_buf[x/32];
                int len=cur_trans->rxlength-x;
                if (len>32) len=32;
                memcpy(&host->cur_trans_buf.buffer_to_rcv[x/32], &word, (len+7)/8);
            }
        }
        //Call post-transaction callback, if any
        if (host->device[host->cur_cs]->cfg.post_cb) host->device[host->cur_cs]->cfg.post_cb(cur_trans);
        //Return transaction descriptor.
        xQueueSendFromISR(host->device[host->cur_cs]->ret_queue, &host->cur_trans_buf, &do_yield);
        host->cur_cs = NO_CS;
    }
    //Tell common code DMA workaround that our DMA channel is idle. If needed, the code will do a DMA reset.
    if (host->dma_chan) spicommon_dmaworkaround_idle(host->dma_chan);
    
 
    //ToDo: This is a stupidly simple low-cs-first priority scheme. Make this configurable somehow. - JD
    for (i=0; i<NO_CS; i++) {
        if (host->device[i]) {
            r=xQueueReceiveFromISR(host->device[i]->trans_queue, &host->cur_trans_buf, &do_yield);
            trans_buf = &host->cur_trans_buf;
            //Stop looking if we have a transaction to send.
            if (r) break;
        }
    }
    if (i==NO_CS) {
        //No packet waiting. Disable interrupt.
        esp_intr_disable(host->intr);
#ifdef CONFIG_PM_ENABLE
        //Release APB frequency lock
        esp_pm_lock_release(host->pm_lock);
#endif
    } else {
        host->hw->slave.trans_done=0; //clear int bit
        //We have a transaction. Send it.
        spi_device_t *dev=host->device[i];
        trans = trans_buf->trans;
        host->cur_cs=i;
        //We should be done with the transmission.
        assert(host->hw->cmd.usr == 0);
        
        //Reconfigure according to device settings, but only if we change CSses.
        if (i!=host->prev_cs) {
            int apbclk=APB_CLK_FREQ;
            int effclk=dev->clk_cfg.eff_clk;
            spi_set_clock(host->hw, dev->clk_cfg.reg);
            //Configure bit order
            host->hw->ctrl.rd_bit_order=(dev->cfg.flags & SPI_DEVICE_RXBIT_LSBFIRST)?1:0;
            host->hw->ctrl.wr_bit_order=(dev->cfg.flags & SPI_DEVICE_TXBIT_LSBFIRST)?1:0;
            
            //Configure polarity
            //SPI iface needs to be configured for a delay in some cases.
            int nodelay=0;
            int extra_dummy=0;
            if (host->no_gpio_matrix) {
                if (effclk >= apbclk/2) {
                    nodelay=1;
                }
            } else {
                if (effclk >= apbclk/2) {
                    nodelay=1;
                    extra_dummy=1;          //Note: This only works on half-duplex connections. spi_bus_add_device checks for this.
                } else if (effclk >= apbclk/4) {
                    nodelay=1;
                }
            }
            
            if (dev->cfg.mode==0) {
                host->hw->pin.ck_idle_edge=0;
                host->hw->user.ck_out_edge=0;
                host->hw->ctrl2.miso_delay_mode=nodelay?0:2;
            } else if (dev->cfg.mode==1) {
                host->hw->pin.ck_idle_edge=0;
                host->hw->user.ck_out_edge=1;
                host->hw->ctrl2.miso_delay_mode=nodelay?0:1;
            } else if (dev->cfg.mode==2) {
                host->hw->pin.ck_idle_edge=1;
                host->hw->user.ck_out_edge=1;
                host->hw->ctrl2.miso_delay_mode=nodelay?0:1;
            } else if (dev->cfg.mode==3) {
                host->hw->pin.ck_idle_edge=1;
                host->hw->user.ck_out_edge=0;
                host->hw->ctrl2.miso_delay_mode=nodelay?0:2;
            }
            
            //configure dummy bits
            host->hw->user.usr_dummy=(dev->cfg.dummy_bits+extra_dummy)?1:0;
            host->hw->user1.usr_dummy_cyclelen=dev->cfg.dummy_bits+extra_dummy-1;
            //Configure misc stuff
            host->hw->user.doutdin=(dev->cfg.flags & SPI_DEVICE_HALFDUPLEX)?0:1;
            host->hw->user.sio=(dev->cfg.flags & SPI_DEVICE_3WIRE)?1:0;
            
            host->hw->ctrl2.setup_time=dev->cfg.cs_ena_pretrans-1;
            host->hw->user.cs_setup=dev->cfg.cs_ena_pretrans?1:0;
            host->hw->ctrl2.hold_time=dev->cfg.cs_ena_posttrans-1;
            host->hw->user.cs_hold=(dev->cfg.cs_ena_posttrans)?1:0;
            
            //Configure CS pin
            host->hw->pin.cs0_dis=(i==0)?0:1;
            host->hw->pin.cs1_dis=(i==1)?0:1;
            host->hw->pin.cs2_dis=(i==2)?0:1;
        }
        host->prev_cs = i;
        //Reset SPI peripheral
        host->hw->dma_conf.val |= SPI_OUT_RST|SPI_IN_RST|SPI_AHBM_RST|SPI_AHBM_FIFO_RST;
        host->hw->dma_out_link.start=0;
        host->hw->dma_in_link.start=0;
        host->hw->dma_conf.val &= ~(SPI_OUT_RST|SPI_IN_RST|SPI_AHBM_RST|SPI_AHBM_FIFO_RST);
        host->hw->dma_conf.out_data_burst_en=1;
        //Set up QIO/DIO if needed
        host->hw->ctrl.val &= ~(SPI_FREAD_DUAL|SPI_FREAD_QUAD|SPI_FREAD_DIO|SPI_FREAD_QIO);
        host->hw->user.val &= ~(SPI_FWRITE_DUAL|SPI_FWRITE_QUAD|SPI_FWRITE_DIO|SPI_FWRITE_QIO);
        if (trans->flags & SPI_TRANS_MODE_DIO) {
            if (trans->flags & SPI_TRANS_MODE_DIOQIO_ADDR) {
                host->hw->ctrl.fread_dio=1;
                host->hw->user.fwrite_dio=1;
            } else {
                host->hw->ctrl.fread_dual=1;
                host->hw->user.fwrite_dual=1;
            }
            host->hw->ctrl.fastrd_mode=1;
        } else if (trans->flags & SPI_TRANS_MODE_QIO) {
            if (trans->flags & SPI_TRANS_MODE_DIOQIO_ADDR) {
                host->hw->ctrl.fread_qio=1;
                host->hw->user.fwrite_qio=1;
            } else {
                host->hw->ctrl.fread_quad=1;
                host->hw->user.fwrite_quad=1;
            }
            host->hw->ctrl.fastrd_mode=1;
        }
        
        
        //Fill DMA descriptors
        if (trans_buf->buffer_to_rcv) {
            host->hw->user.usr_miso_highpart=0;
            if (host->dma_chan == 0) {
                //No need to setup anything; we'll copy the result out of the work registers directly later.
            } else {
                spicommon_dmaworkaround_transfer_active(host->dma_chan); //mark channel as active
                spicommon_setup_dma_desc_links(host->dmadesc_rx, ((trans->rxlength+7)/8), (uint8_t*)trans_buf->buffer_to_rcv, true);
                host->hw->dma_in_link.addr=(int)(&host->dmadesc_rx[0]) & 0xFFFFF;
                host->hw->dma_in_link.start=1;
            }
        } else {
            //DMA temporary workaround: let RX DMA work somehow to avoid the issue in ESP32 v0/v1 silicon
            if (host->dma_chan != 0 ) {
                host->hw->dma_in_link.addr=0;
                host->hw->dma_in_link.start=1;
            }
        }
        
        if (trans_buf->buffer_to_send) {
            if (host->dma_chan == 0) {
                //Need to copy data to registers manually
                for (int x=0; x < trans->length; x+=32) {
                    //Use memcpy to get around alignment issues for txdata
                    uint32_t word;
                    memcpy(&word, &trans_buf->buffer_to_send[x/32], 4);
                    host->hw->data_buf[(x/32)+8]=word;
                }
                host->hw->user.usr_mosi_highpart=1;
            } else {
                spicommon_dmaworkaround_transfer_active(host->dma_chan); //mark channel as active
                spicommon_setup_dma_desc_links(host->dmadesc_tx, (trans->length+7)/8, (uint8_t*)trans_buf->buffer_to_send, false);
                host->hw->user.usr_mosi_highpart=0;
                host->hw->dma_out_link.addr=(int)(&host->dmadesc_tx[0]) & 0xFFFFF;
                host->hw->dma_out_link.start=1;
                host->hw->user.usr_mosi_highpart=0;
            }
        }
        
        host->hw->mosi_dlen.usr_mosi_dbitlen=trans->length-1;
        if ( dev->cfg.flags & SPI_DEVICE_HALFDUPLEX ) {
            host->hw->miso_dlen.usr_miso_dbitlen=trans->rxlength-1;
        } else {
            //rxlength is not used in full-duplex mode
            host->hw->miso_dlen.usr_miso_dbitlen=trans->length-1;
        }
        
        //Configure bit sizes, load addr and command
        int cmdlen;
        if ( trans->flags & SPI_TRANS_VARIABLE_CMD ) {
            cmdlen = ((spi_transaction_ext_t*)trans)->command_bits;
        } else {
            cmdlen = dev->cfg.command_bits;
        }
        int addrlen;
        if ( trans->flags & SPI_TRANS_VARIABLE_ADDR ) {
            addrlen = ((spi_transaction_ext_t*)trans)->address_bits;
        } else {
            addrlen = dev->cfg.address_bits;
        }
        host->hw->user1.usr_addr_bitlen=addrlen-1;
        host->hw->user2.usr_command_bitlen=cmdlen-1;
        host->hw->user.usr_addr=addrlen?1:0;
        host->hw->user.usr_command=cmdlen?1:0;
        
        // output command will be sent from bit 7 to 0 of command_value, and then bit 15 to 8 of the same register field.
        uint16_t command = trans->cmd << (16-cmdlen);    //shift to MSB
        host->hw->user2.usr_command_value = (command>>8)|(command<<8);  //swap the first and second byte
        // shift the address to MSB of addr (and maybe slv_wr_status) register.
        // output address will be sent from MSB to LSB of addr register, then comes the MSB to LSB of slv_wr_status register.
        if (addrlen>32) {
            host->hw->addr = trans->addr >> (addrlen- 32);
            host->hw->slv_wr_status = trans->addr << (64 - addrlen);
        } else {
            host->hw->addr = trans->addr << (32 - addrlen);
        }
        
        host->hw->user.usr_mosi=( (!(dev->cfg.flags & SPI_DEVICE_HALFDUPLEX) && trans_buf->buffer_to_rcv) || trans_buf->buffer_to_send)?1:0;
        host->hw->user.usr_miso=(trans_buf->buffer_to_rcv)?1:0;
        
        //Call pre-transmission callback, if any
        if (dev->cfg.pre_cb) dev->cfg.pre_cb(trans);
        //Kick off transfer
        host->hw->cmd.usr=1;
    }
    if (do_yield) portYIELD_FROM_ISR();
}

*/
    
#ifdef __cplusplus
}
#endif

#endif /* compet_h */
