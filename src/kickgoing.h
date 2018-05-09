#ifndef _KICKGOING_H_
#define _KICKGOING_H_

#include "application.h"


// Type and Struct Defines
// 주변장치 혹은 스케쥴러, 초기화 함수가 리턴하는 데이터타입
typedef enum kick_result_t{
  KICK_OK = 0,
  KICK_ERROR_TIMEOUT = 1,
  KICK_ERROR_RESPONSE = 2,
  KICK_INVALID_PARAMETER = 3,
  KICK_ERROR_INTERNAL = 4
}kick_result_t;
// main loop callback flag: 이 플래그로 인해 여러 periph loop을 실행한다.


typedef enum kick_state_t{
  STATE_NOT_READY = 0,
  STATE_RETURNED,
  STATE_CONNECTED,
  STATE_INSENSIBLE, // 연결이 끊어지고 일정 시간 이내.
  STATE_ERROR
}kick_state_t;

// system 함수들
kick_result_t checksum(uint8_t* data, uint8_t packet_data_length, bool transmit);
String bytes_to_string(uint8_t* data, uint8_t length);
String char_to_string(char* data, uint8_t length);
void critical_error_loop(String ErrorMessage); // 치명적 에러 발생시, 사용종료 후 ble, http, log 메세지 출력

/*
class kick_info{
private:
  const uint8_t offset_kick_code = 0;
  const uint8_t offset_pin_code = 3;
  const uint8_t offset_rent_id = 6; // 우선은 rent_id 6자라고 가정
  const uint8_t offset_start_rent_time = 9;
  const uint8_t offset_end_rent_time = 11;
  const uint8_t offset_access_token_length = 13;
  const uint8_t offset_access_token = 14;

  const uint8_t max_access_token_length = 200;

  String kick_code;
  String pin_code;
  String rent_id;
  int start_rent_time;
  int end_rent_time;
  uint16_t length_token;
  String access_token;

public:
  uint8_t state; // 킥보드의 대여모델
  void write_kick_code(String par_kick_code){
    kick_code = par_kick_code;
    if(!par_kick_code.length()){
      EEPROM.put(offset_kick_code, 0xffff);
      return;
    }

    char kick_code_c[6];
    par_kick_code.toCharArray(kick_code_c, 6);
    EEPROM.put(offset_kick_code, kick_code_c);
  }
  void write_pin_code(String par_pin_code){
    pin_code = par_pin_code;
    if(!par_pin_code.length()){
      EEPROM.put(offset_pin_code, 0xffff);
      return;
    }

    char pin_code_c[6];
    par_pin_code.toCharArray(pin_code_c, 6);
    EEPROM.put(offset_pin_code, pin_code_c);
  }
  void write_rent_id(String par_rent_id){
    rent_id = par_rent_id;
    char rent_id_c[6];
    par_rent_id.toCharArray(rent_id_c, 6);
    EEPROM.put(offset_rent_id, rent_id_c);
  }
  void write_start_rent_time(int par_start_rent_time){
    start_rent_time = par_start_rent_time;
    EEPROM.put(offset_start_rent_time, start_rent_time);
  }
  void write_end_rent_time(int par_end_rent_time){
    end_rent_time = par_end_rent_time;
    EEPROM.put(offset_end_rent_time, end_rent_time);
  }
  void write_access_token(String par_aint lengthccess_token){
    access_token = par_access_token;
    length_token = access_token.length();
    EEPROM.put(offset_access_token_length, length_token);

    char token_c[max_access_token_length];
    access_token.toCharArray(token_c, length_token);
    EEPROM.put(offset_access_token, token_c);
  }
  String read_kick_code(){
    if(kick_code.length())
      return kick_code;

    char kick_code_c[6];
    EEPROM.get(offset_kick_code, kick_code_c);
    if(kick_code_c[0] == 0)
      return String("null");int length

    kick_code = char_to_string(kick_code_c, 6);
    return kick_code;
  }
  String read_pin_code(){
    if(pin_code.length())
      return pin_code;

    char pin_code_c[6];
    EEPROM.get(offset_pin_code, pin_code_c);
    if(pin_code_c[0] == 0)
      return String("null");

    pin_code = char_to_string(pin_code_c, 6);
    return pin_code;
  }
  String read_rent_id(){int length
    if(rent_id.length())
      return rent_id;

    char rent_id_c[6];
    EEPROM.get(offset_rent_id, rent_id_c);
    if(rent_id_c[0] == 0)
      return String("null");

    rent_id = char_to_string(rent_id_c, 6);
    return rent_id;
  }
  String read_access_token(){
    if(access_token.length())
      return access_token;

    EEPROM.get(offset_access_token_length, length_token);
    char* token_c = (char*) malloc(length_token);
    EEPROM.get(offset_access_token, token_c);

    access_token = char_to_string(token_c, length_token);
    free(token_c);

    return access_token;
  }
  void remove_rent_id(){
    write_rent_id(String(""));
  }
  void remove_access_token(){
    write_access_token(String(""));
  }
  void remove_start_rent_time(){
    write_start_rent_time(0);
  }
  void remove_end_rent_time(){
    write_end_rent_time(0);
  }
  String error_message;
};

kick_info my_kick_info;

*/




kick_result_t checksum(uint8_t* data, uint8_t packet_data_length, bool transmit){
  Logger log("app.checksum");

  if(transmit){
    uint16_t buf = 0;
    for(int i=2; i<packet_data_length+4;i++){
      buf += data[i];
    }
    buf = (buf ^ 0xFFFF) & 0xFFFF;
    data[packet_data_length+4] = buf & 0x00FF;
    data[packet_data_length+5] = (buf >> 8);

    return KICK_OK;
  }
   // 관제기가 수신한 데이터가 유효한지 검사한다.
  uint16_t buf = 0;
  for(int i=2; i<packet_data_length+4;i++){
    buf += data[i];
  }
  buf = (buf ^ 0xFFFF) & 0xFFFF;

  if((data[packet_data_length+4] != (buf & 0x00FF)) || (data[packet_data_length+5] != (buf >> 8))){

    String log_s = String("invalid checksum ");
    log_s.concat(bytes_to_string(data, packet_data_length + 6));
    log.warn(log_s);
  }

  return KICK_OK;
}

String bytes_to_string(uint8_t* data, uint8_t length){
  String data_s;
  for(uint8_t i=0; i< length; i++){
    data_s.concat(String(data[i], HEX));
  }
  return data_s;
}
String char_to_string(char* data, uint8_t length){
  String data_s;
  for(uint8_t i=0; i< length; i++){
    data_s.concat(String(data[i]));
  }
  return data_s;
}
void critical_error_loop(String message){
  Logger log("error.critical");
  while(1){
    log.error(message);
    delay(1000);
  }
}
#endif
