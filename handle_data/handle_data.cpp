#include <Arduino.h>
#include "handle_data.h"

void write_bit(uint8_t bit, int pin)
{
	
	if (bit & 1) {
		noInterrupts();
		digitalWrite(pin, LOW);
		pinMode(pin, OUTPUT);	// drive output low
		delayMicroseconds(10);
    
		digitalWrite(pin, HIGH);	// drive output high
		interrupts();
		delayMicroseconds(65);
	} else {
		noInterrupts();
		digitalWrite(pin, LOW);
		pinMode(pin, OUTPUT);	// drive output low
		delayMicroseconds(75);
		digitalWrite(pin, HIGH);	// drive output high
		interrupts();
		//delayMicroseconds(5);
	}
}

//
// Read a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
uint8_t read_bit(int pin)
{
	uint8_t r;

	noInterrupts();
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delayMicroseconds(3);
	pinMode(pin, INPUT);	// let pin float, pull up will raise
	delayMicroseconds(15);
	r = digitalRead(pin);
	interrupts();
	delayMicroseconds(58);
	return r;
}

//
// Write a byte. The writing code uses the active drivers to raise the
// pin high, if you need power after the write (e.g. DS18S20 in
// parasite power mode) then set 'power' to 1, otherwise the pin will
// go tri-state at the end of the write to avoid heating in a short or
// other mishap.
//
void write(uint8_t v, int pin, bool power /* = 0 */) {
    uint8_t bitMask;

	digitalWrite(pin, HIGH);

    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
	write_bit( (bitMask & v)?1:0, pin);
    }
    if ( !power) {
	noInterrupts();
	pinMode(pin, INPUT);
	digitalWrite(pin, LOW);
	interrupts();
    }
}
//
// Read a byte
//
uint8_t read(int pin) {
    uint8_t bitMask;
    uint8_t r = 0;

    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
	if ( read_bit(pin)) {r |= bitMask;}
    }
    return r;
}

void sending_mess(uint8_t client_send, uint8_t work_send, uint16_t time_send, byte pin, bool power, bool pin_to_input){
	byte info_send = work_send << 4;
	info_send |= client_send;
	// Serial.println(client_send);
	
	uint8_t list_byte[3] = {info_send, 0, 0};
	split_argument(time_send, list_byte[1], list_byte[2]);
	// Serial.println(list_byte[1]);
	// Serial.println(list_byte[2]);

	for (int i = 0; i < 3; i++){
		write(list_byte[i], pin, power);
		delayMicroseconds(15);
	}
	if (pin_to_input){
		pinMode(pin, INPUT);
		digitalWrite(pin, LOW);
	}
}

void recive_mess(uint8_t *client_rc, uint8_t *work_rc, uint16_t *time_rc, byte pin){
	uint8_t recive_byte[3];
	
	if (!digitalRead(pin)){
		for (int i = 0; i < 3; i++){
			recive_byte[i] = read(pin);
		}
		// if (recive_byte[0] != 0){Serial.println(recive_byte[0]);}
		// Serial.println(recive_byte[1]);
		// Serial.println(recive_byte[2]);
		split_byte(recive_byte[0], *client_rc, *work_rc);
		*time_rc = recive_byte[2] << 8;
		*time_rc |= recive_byte[1];
	}
}

void split_argument(uint16_t value, uint8_t &first_byte, uint8_t &second_byte){
  uint16_t bitMask;
  first_byte = 0;
  second_byte = 0;
  for (bitMask = 0x01; bitMask < 0b1<<8; bitMask <<= 1){
    if (bitMask & value){first_byte |= bitMask;}
  }
  for (bitMask = 0x01; bitMask < 0b1<<8; bitMask <<= 1){
    if (bitMask<<8 & value){second_byte |= bitMask;}
  }
}

void split_byte(uint8_t byte_split, uint8_t &first_4b, uint8_t &second_4b){
  uint8_t bitMask;
  first_4b = 0;
  second_4b = 0;
  for (bitMask = 0x01; bitMask < 0x01<<4; bitMask <<= 1){
    if (bitMask & byte_split){first_4b |= bitMask;}
  }
  for (bitMask = 0x01; bitMask < 0x01<<4; bitMask <<= 1){
    if (bitMask<<4 & byte_split){second_4b |= bitMask;}
  }
}