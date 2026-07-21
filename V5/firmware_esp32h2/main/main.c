#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "emg_v5";

/* Pins follow the committed DSTK22807 carrier schematic mapping. */
#define PIN_MCP3208_CS    GPIO_NUM_14
#define PIN_MCP3208_CLK   GPIO_NUM_4
#define PIN_MCP3208_MOSI  GPIO_NUM_12
#define PIN_MCP3208_MISO  GPIO_NUM_11

#define SAMPLE_RATE_HZ 1000
#define MCP3208_SPI_HOST SPI2_HOST

typedef enum {
    ADC_CH_EMG1_RAW = 0,
    ADC_CH_EMG1_RECT = 1,
    ADC_CH_EMG1_ENV = 2,
    ADC_CH_EMG2_RAW = 3,
    ADC_CH_EMG2_RECT = 4,
    ADC_CH_EMG2_ENV = 5,
    ADC_CH_VREF_MON = 6,
    ADC_CH_BAT_MON = 7,
} emg_adc_channel_t;

typedef struct __attribute__((packed)) {
    uint32_t sample_index;
    uint16_t ch1_raw;
    uint16_t ch1_rect;
    uint16_t ch1_env;
    uint16_t ch2_raw;
    uint16_t ch2_rect;
    uint16_t ch2_env;
    uint16_t vref;
    uint16_t batt;
} emg_sample_frame_t;

static spi_device_handle_t adc_dev;

static esp_err_t mcp3208_init(void)
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_MCP3208_MOSI,
        .miso_io_num = PIN_MCP3208_MISO,
        .sclk_io_num = PIN_MCP3208_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 3,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000 * 1000,
        .mode = 0,
        .spics_io_num = PIN_MCP3208_CS,
        .queue_size = 1,
    };

    ESP_RETURN_ON_ERROR(spi_bus_initialize(MCP3208_SPI_HOST, &buscfg, SPI_DMA_DISABLED), TAG, "spi bus init failed");
    ESP_RETURN_ON_ERROR(spi_bus_add_device(MCP3208_SPI_HOST, &devcfg, &adc_dev), TAG, "spi add device failed");
    return ESP_OK;
}

static esp_err_t mcp3208_read(uint8_t channel, uint16_t *out_value)
{
    if (channel > 7 || out_value == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t tx[3] = {0};
    uint8_t rx[3] = {0};

    /*
     * MCP3208 command:
     * start bit = 1, single-ended = 1, channel D2:D0.
     */
    tx[0] = 0x06 | ((channel & 0x04) >> 2);
    tx[1] = (uint8_t)((channel & 0x03) << 6);
    tx[2] = 0x00;

    spi_transaction_t t = {
        .length = 24,
        .tx_buffer = tx,
        .rx_buffer = rx,
    };

    ESP_RETURN_ON_ERROR(spi_device_transmit(adc_dev, &t), TAG, "mcp3208 read failed");

    *out_value = (uint16_t)(((rx[1] & 0x0F) << 8) | rx[2]);
    return ESP_OK;
}

static void frame_read(emg_sample_frame_t *frame)
{
    mcp3208_read(ADC_CH_EMG1_RAW, &frame->ch1_raw);
    mcp3208_read(ADC_CH_EMG1_RECT, &frame->ch1_rect);
    mcp3208_read(ADC_CH_EMG1_ENV, &frame->ch1_env);
    mcp3208_read(ADC_CH_EMG2_RAW, &frame->ch2_raw);
    mcp3208_read(ADC_CH_EMG2_RECT, &frame->ch2_rect);
    mcp3208_read(ADC_CH_EMG2_ENV, &frame->ch2_env);
    mcp3208_read(ADC_CH_VREF_MON, &frame->vref);
    mcp3208_read(ADC_CH_BAT_MON, &frame->batt);
}

static void usb_csv_print(const emg_sample_frame_t *f)
{
    printf("%" PRIu32 ",%u,%u,%u,%u,%u,%u,%u,%u\n",
           f->sample_index,
           f->ch1_raw,
           f->ch1_rect,
           f->ch1_env,
           f->ch2_raw,
           f->ch2_rect,
           f->ch2_env,
           f->vref,
           f->batt);
}

static void ble_send_frame_placeholder(const emg_sample_frame_t *frame)
{
    /*
     * TODO: Add Nordic UART-like BLE service once the ESP-IDF BLE profile
     * choice is finalized for ESP32-H2. The binary payload is emg_sample_frame_t.
     */
    (void)frame;
}

void app_main(void)
{
    printf("WARNING: Do not attach electrodes to a human while USB is connected.\n");
    printf("Human testing is battery-only.\n");
    printf("index,ch1_raw,ch1_rect,ch1_env,ch2_raw,ch2_rect,ch2_env,vref,batt\n");

    ESP_ERROR_CHECK(mcp3208_init());

    const int64_t period_us = 1000000 / SAMPLE_RATE_HZ;
    int64_t next_us = esp_timer_get_time();
    uint32_t dropped_timing_slots = 0;
    emg_sample_frame_t frame = {0};

    while (1) {
        const int64_t now_us = esp_timer_get_time();
        if (now_us < next_us) {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }

        if (now_us - next_us > period_us) {
            dropped_timing_slots++;
        }
        next_us += period_us;

        frame.sample_index++;
        frame_read(&frame);
        ble_send_frame_placeholder(&frame);

        if ((frame.sample_index % 100) == 0) {
            usb_csv_print(&frame);
            ESP_LOGI(TAG, "timing_slots_late=%" PRIu32, dropped_timing_slots);
        }
    }
}
