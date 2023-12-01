//PWM should be about 25KHz, duty cycle from 0 to 100%
//Using timer 1

//move to util.c
int32_t map(int32_t val_to_map, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
  return (val_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void pwm_init() {
  //Set fast PWM mode with OCR1A as TOP, compare on channel B
  TCCR1A = (1 << WGM11) | (0 << WGM10) | (1 << COM1B1);
  TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10); //No prescaler
  DDRB |= (1 << PB1); //OC1A

  OCR1A = 800;
  OCR1B = 0;
  
}

//Duty: 0-100%
uint8_t pwm_set_duty_cycle(uint8_t duty) {
  OCR1B = map(duty, 0, 100, 0, 800);

  
}
