/* 
 * File:   MessageTest.c
 * Author: Nghia Pham
 */
#include "xc.h"
#include <stdio.h>
#include "Message.h"
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include "OledDriver.h"
#include "Oled.h"
#include "Ascii.h"

static BB_Event EventType;
static Message testMessage;
static char *payload;
static char *StringResult;
static char message[MESSAGE_MAX_LEN];
static char *TestMessage2;

int main() {
    uint8_t checksum;
    int Pass = 0;
    
    printf("Testing Message.c\n\n");
    
    // tests for calculate checksum
    printf("Testing Message_CalculateChecksum():\n\n");
    payload = "CHA,1";
    checksum = Message_CalculateChecksum(payload);
    if (checksum == 0x57) {
        printf("\tTest 1: passed!\n");
        Pass++;
    } else {
        printf("\tTest 1: failed!\n");
        
    }
    
    payload = "SHO,1,2";
    checksum = Message_CalculateChecksum(payload);
    if (checksum == 0x57) {
        printf("\tTest 2: passed!\n");
        Pass++;
    } else {
        printf("\tTest 2: failed!\n");
        
    }
    
    payload = "RES,1,2,3";
    checksum = Message_CalculateChecksum(payload);
    if (checksum == 0x58) {
        printf("\tTest 3: passed!\n\n");
        Pass++;
    } else {
        printf("\tTest 3: failed! \n\n");
    }
    
    // tests for Message_ParseMessage()
    printf("Testing Message_ParseMessage():\n\n");
    int result;
    payload = "CHA,1";
    StringResult = "57";
    result = Message_ParseMessage(payload, StringResult, &EventType);
    if (result && EventType.type == BB_EVENT_CHA_RECEIVED && EventType.param0 == 1 ) {
        printf("\tTest 1: passed!\n");
        Pass++;
    } else {
        printf("\tTest 1: failed!\n");
    }
    
    payload = "SHO,1,2";
    StringResult = "57";
    result = Message_ParseMessage(payload, StringResult, &EventType);
    if (result && EventType.type == BB_EVENT_SHO_RECEIVED && EventType.param0 == 1 && EventType.param1 == 2) {
        printf("\tTest 2: passed!\n");
        Pass++;
    } else {
        printf("\tTest 2: failed!\n");
    }
    
    payload = "RES,1,2,3";
    StringResult = "58";
    result = Message_ParseMessage(payload, StringResult, &EventType);
    if (result && EventType.type == BB_EVENT_RES_RECEIVED && EventType.param0 == 1 && EventType.param1 == 2
            && EventType.param2 == 3) {
        printf("\tTest 3: passed!\n");
        Pass++;
    } else {
        printf("\tTest 3: failed!\n");
       
    }
    
    
    // testing message encode
    printf("Testing Message_Encode():\n\n");
    
    testMessage.type = MESSAGE_CHA;
    testMessage.param0 = 1;
    Message_Encode(message, testMessage);
    if (strcmp(message, "$CHA,1*57\n") == 0) {
        printf("\tTest 1: passed!\n");
        Pass++;
    } else {
        printf("\tTest 1: failed!\n");
    }
    
    testMessage.type = MESSAGE_SHO;
    testMessage.param0 = 1;
    testMessage.param1 = 2;
    Message_Encode(message, testMessage);
    if (strcmp(message, "$SHO,1,2*57\n") == 0) {
        printf("\tTest 2: passed!\n");
        Pass++;
    } else {
        printf("\tTest 2: failed!\n");
        
    }
    
    testMessage.type = MESSAGE_RES;
    testMessage.param0 = 1;
    testMessage.param1 = 2;
    testMessage.param2 = 3;
    Message_Encode(message, testMessage);
    if (strcmp(message, "$RES,1,2,3*58\n") == 0) {
        printf("\tTest 3: passed!\n\n");
        Pass++;
    } else {
        printf("\tTest 3: failed!\n\n");
    }
    
    // testing message decode
    printf("Testing Message_Decode():\n\n");
    
    // each test will send the char one by one until its done
    int iter = 0;
    
    TestMessage2 = "$CHA,1*57\n";
    for (iter = 0; iter < strlen(TestMessage2); iter++) {
        Message_Decode(TestMessage2[iter], &EventType);
    } 
    if (EventType.type == BB_EVENT_CHA_RECEIVED && EventType.param0 == 1) {
        printf("\tTest 1: passed!\n");
        Pass++;
    } else {
        printf("\tTest 1: failed! \n");
    }
    
    TestMessage2 = "$SHO,1,2*57\n";
    for (iter = 0; iter < strlen(TestMessage2); iter++) {
        Message_Decode(TestMessage2[iter], &EventType);
    } 
    if (EventType.type == BB_EVENT_SHO_RECEIVED && EventType.param0 == 1 && EventType.param1 == 2) {
        printf("\tTest 2: passed!\n");
        Pass++;
    } else {
        printf("\tTest 2: failed! \n");
    }
    
    TestMessage2 = "$RES,1,2,3*58\n";
    for (iter = 0; iter < strlen(TestMessage2); iter++) {
        Message_Decode(TestMessage2[iter], &EventType);
    } 
    if (EventType.type == BB_EVENT_RES_RECEIVED && EventType.param0 == 1 && EventType.param1 == 2 && EventType.param2 == 3) {
        printf("\tTest 3: passed!\n");
        Pass++;
    } else {
        printf("\tTest 3: failed!\n");
       
    }
    
    printf("\nFinal result: %d/12 nice job!\n", Pass);
    
    while (1);
    
}