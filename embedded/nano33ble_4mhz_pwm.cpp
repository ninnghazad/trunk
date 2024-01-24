// Generate 4Mhz PWM on Arduino NANO 33 BLE using nordic api inside mbed

#define SEQ_POLARITY_BIT (0x8000)
#define NDEBUG 1
#include "mbed.h"
#include "nrfx_pwm.h"

const int8_t pwm_pin{digitalPinToPinName(12)};
static nrfx_pwm_t pwm_instance(NRFX_PWM_INSTANCE(0));
static nrf_pwm_sequence_t sequence;

void setup() {
#ifndef NDEBUG
	Serial.begin(9600);
	while (!Serial) {
		delay(1);
	}
#endif

	ret_code_t result{NRFX_SUCCESS};

	const uint16_t period{2};
	static uint16_t pulse{1};

	nrfx_pwm_config_t config = NRFX_PWM_DEFAULT_CONFIG;
	config.output_pins[0] = pwm_pin;  // | NRFX_PWM_PIN_INVERTED;
	config.output_pins[1] = 0xFF;
	config.output_pins[2] = 0xFF;
	config.output_pins[3] = 0xFF;
	config.base_clock = NRF_PWM_CLK_8MHz;
	config.top_value = period;
	config.count_mode = NRF_PWM_MODE_UP;
	config.load_mode = NRF_PWM_LOAD_COMMON;
	config.step_mode = NRF_PWM_STEP_AUTO;

	result = nrfx_pwm_init(&pwm_instance, &config, NULL);
	assert(result == NRFX_SUCCESS);

	sequence.values.p_common = &pulse;
	sequence.length = NRF_PWM_VALUES_LENGTH(pulse);	 // how many 16bit values p_* has
	assert(NRF_PWM_VALUES_LENGTH(pulse) == sizeof(pulse) / sizeof(uint16_t));
	sequence.repeats = 0;
	sequence.end_delay = 0;
	result = nrfx_pwm_simple_playback(&pwm_instance, &sequence, 1, NRFX_PWM_FLAG_LOOP);
	assert(result == NRFX_SUCCESS);

#ifndef NDEBUG
	Serial.println("OK");
	digitalWrite(24u, LOW);
#endif
}

void loop() {
	delay(1);
}
