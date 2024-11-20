#define HANDLE_DATA
#ifdef HANDLE_DATA

#include <Arduino.h>

void write_bit(uint8_t bit, int pin) ;
uint8_t read_bit(int pin) ;

void write(uint8_t v, int pin, bool power /* = 0 */)  ;

uint8_t read(int pin);

void split_argument(uint16_t value, uint8_t &first_byte, uint8_t &second_byte);
void split_byte(uint8_t byte_split, uint8_t &first_4b, uint8_t &second_4b);

void sending_mess(uint8_t client_send, uint8_t work_send, uint16_t time_send, byte pin, bool power, bool pin_to_input);
void recive_mess(uint8_t *client_rc, uint8_t *work_rc, uint16_t *time_rc, byte pin);
#endif  