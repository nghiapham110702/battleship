/* 
 * File:   Message.c
 * Author: Ezra Frary
 */
#include <stdint.h>
#include "BattleBoats.h"
#include <string.h>
#include "Message.h"
#include <stdio.h>
#include "BOARD.h"


typedef enum {
    WAIT,
    RECORD,
    RECORDCHECKSUM,
} CurrentProcess;

static char decodedPayload[MESSAGE_MAX_PAYLOAD_LEN];
static char ParseMessageString[3];
static int count = 0;
static CurrentProcess currentProcess = WAIT;

uint8_t Message_CalculateChecksum(const char* payload) {
    uint8_t returnValue = 0;
    int i = 0;
    while (strlen(payload) > i) {
        returnValue ^= payload[i];
        i++;
    }
    return returnValue;
}

int Message_ParseMessage(const char* payload, const char* checksum_string, BB_Event * message_event) {
    //initalize paramaters to defaults
    message_event->param0 = 0;
    message_event->param1 = 0;
    message_event->param2 = 0;
    
    if (strlen(checksum_string) != 2) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    char payload2[MESSAGE_MAX_PAYLOAD_LEN];
    strcpy(payload2, payload);
    
    uint8_t strtoulChecksum = strtoul(checksum_string, NULL, 16);
    
    //making sure there are no errors
    if (Message_CalculateChecksum(payload) != strtoulChecksum) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    char *token = strtok(payload2, ","); //the number of commas (tokens) helps us later
    int numCommas;
    
    if (strcmp(token, "CHA") == 0) {
        numCommas = 1;
        message_event->type = BB_EVENT_CHA_RECEIVED;
    } else if (strcmp(token, "ACC") == 0) {
        numCommas = 1;
        message_event->type = BB_EVENT_ACC_RECEIVED;
    } else if (strcmp(token, "SHO") == 0) {
        numCommas = 2;
        message_event->type = BB_EVENT_SHO_RECEIVED;
    } else if (strcmp(token, "REV") == 0) {
        numCommas = 1;
        message_event->type = BB_EVENT_REV_RECEIVED;
    } else if (strcmp(token, "RES") == 0) {
        numCommas = 3;
        message_event->type = BB_EVENT_RES_RECEIVED;
        
    //error management
    } else {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    for (int i = 0; i < numCommas; i++) {
        token = strtok(NULL, ",");
        
        if (token == NULL) {
            message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }
        
        uint16_t j = atoi(token);
        if (i == 0) {
            message_event->param0 = j;
        } else if (i == 1) {
            message_event->param1 = j;
        } else if (i == 2) {
            message_event->param2 = j;
        }
    }
    
    token = strtok(NULL, ",");
    if (token){
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    return SUCCESS;
    
    
}


int Message_Encode(char *message_string, Message message_to_encode) {
    char message[MESSAGE_MAX_PAYLOAD_LEN];
    char returnMessage[MESSAGE_MAX_LEN];
    uint8_t checksum;
    switch (message_to_encode.type) {
        case MESSAGE_CHA:
            sprintf(message, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            checksum = Message_CalculateChecksum(message);
            sprintf(returnMessage, MESSAGE_TEMPLATE, message, checksum);
            strcpy(message_string, returnMessage);
            break;
        case MESSAGE_ACC:
            sprintf(message, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            checksum = Message_CalculateChecksum(message);
            sprintf(returnMessage, MESSAGE_TEMPLATE, message, checksum);
            strcpy(message_string, returnMessage);
            break;
        case MESSAGE_SHO:
            sprintf(message, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
            checksum = Message_CalculateChecksum(message);
            sprintf(returnMessage, MESSAGE_TEMPLATE, message, checksum);
            strcpy(message_string, returnMessage);
            break;
        case MESSAGE_REV:
            sprintf(message, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            checksum = Message_CalculateChecksum(message);
            sprintf(returnMessage, MESSAGE_TEMPLATE, message, checksum);
            strcpy(message_string, returnMessage);
            break;
        case MESSAGE_RES:
            sprintf(message, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
            checksum = Message_CalculateChecksum(message);
            sprintf(returnMessage, MESSAGE_TEMPLATE, message, checksum);
            strcpy(message_string, returnMessage);
            break;
        case MESSAGE_NONE:
            return 0;
            break;
        case MESSAGE_ERROR:
            break;
    }
    return strlen(message_string);
}

int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event) {
    switch (currentProcess){
        case WAIT:
            if (char_in != '$') { //if we are in wait mode, and char_in is not a $, then there is an error
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                return STANDARD_ERROR;
            } else{
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                currentProcess = RECORD;
            }
            break;
        case RECORD:
            if (count > MESSAGE_MAX_PAYLOAD_LEN) {
                count = 0;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_PAYLOAD_LEN_EXCEEDED;
                currentProcess = WAIT;
                return STANDARD_ERROR;
            } else if (char_in == '\n' || char_in == '$') {
                count = 0;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                decoded_message_event->type = BB_EVENT_ERROR;
                currentProcess = WAIT;
                return STANDARD_ERROR;
            } else if (char_in == '*'){
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                decodedPayload[count] = '\0';
                currentProcess = RECORDCHECKSUM;
                count = 0;
            } else {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                decodedPayload[count] = char_in;
                count++;
            }
            break;
        case RECORDCHECKSUM:
            if (count > 2){
                count = 0;
                decoded_message_event->param0 = BB_ERROR_CHECKSUM_LEN_EXCEEDED;
                currentProcess = WAIT;
                decoded_message_event->type = BB_EVENT_ERROR;
                return STANDARD_ERROR;
            } else if (char_in == '\n'){
                if (count < 2) {
                    count = 0;
                    decoded_message_event->type = BB_EVENT_ERROR;
                    decoded_message_event->param0 = BB_ERROR_CHECKSUM_LEN_INSUFFICIENT;
                    currentProcess = WAIT;
                    return STANDARD_ERROR;
                } else{
                    ParseMessageString[count] = '\0';
                    count = 0;
                    int result = Message_ParseMessage(decodedPayload, ParseMessageString, decoded_message_event);
                    if (result == STANDARD_ERROR) {
                        decoded_message_event->type = BB_EVENT_ERROR;
                        decoded_message_event->param0 = BB_ERROR_MESSAGE_PARSE_FAILURE;
                        currentProcess = WAIT;
                        return STANDARD_ERROR;
                    }
                    currentProcess = WAIT;
                }
            } else if(char_in < 48 || char_in > 70 || (char_in < 65 && char_in > 57)) {
                count = 0;
                currentProcess = WAIT;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_BAD_CHECKSUM;
                return STANDARD_ERROR;
            } else{
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                ParseMessageString[count] = char_in;
                count++;
            }
            break;
    }
    return SUCCESS;
}