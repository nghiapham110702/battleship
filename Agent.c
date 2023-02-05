/* 
 * File:   Agent.c
 * Author: Ezra Frary
 */
#include "Agent.h"
#include "Ascii.h"
#include "BattleBoats.h"
#include "BOARD.h"
#include "Buttons.h"
#include "CircularBuffer.h"
#include "Message.h"
#include "Negotiation.h"
#include "Field.h"
#include "FieldOled.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Uart1.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


struct Agent {
    AgentState state;
    NegotiationData negotationdata;
    NegotiationData negotationdata2;
    Field player1;
    Field player2;
    Message message;
};

static char *printerror;//used when ther is an error
static FieldOledTurn turn;
static struct Agent agent;
static int turns = 0;


//initalize the things we need to
void AgentInit(void) {
    turns = 0;
    turn = FIELD_OLED_TURN_NONE;
    agent.state = AGENT_STATE_START;
}

//I am aware that a case statement would have made this function a little prettier and easier to read
Message AgentRun(BB_Event event) {
    if(event.type == BB_EVENT_START_BUTTON){
        if (agent.state == AGENT_STATE_START) {
            agent.negotationdata = rand() & 0xFFFF;
            agent.message.type = MESSAGE_CHA;
            agent.message.param0 = NegotiationHash(agent.negotationdata);
            FieldInit(&agent.player1, &agent.player2);
            FieldAIPlaceAllBoats(&agent.player1);
            agent.state = AGENT_STATE_CHALLENGING;
        }else {
            agent.message.type = MESSAGE_NONE;
        }
    }else if (event.type == BB_EVENT_CHA_RECEIVED){
        if (agent.state == AGENT_STATE_START) {
            agent.negotationdata = rand() & 0xFFFF; 
            agent.negotationdata2 = event.param0;
            agent.message.type = MESSAGE_ACC;
            agent.message.param0 = agent.negotationdata;
                
            FieldInit(&agent.player1, &agent.player2);
            FieldAIPlaceAllBoats(&agent.player1);
            agent.state = AGENT_STATE_ACCEPTING;
                
        }else {
            agent.message.type = MESSAGE_NONE;
        }
    }else if (event.type == BB_EVENT_ACC_RECEIVED){
        if (agent.state == AGENT_STATE_CHALLENGING) {
            agent.message.type = MESSAGE_REV;
            agent.message.param0 = agent.negotationdata;
            NegotiationOutcome outcome = NegotiateCoinFlip(agent.negotationdata, event.param0);
            if (outcome == HEADS) {
                turn = FIELD_OLED_TURN_MINE;
                agent.state = AGENT_STATE_WAITING_TO_SEND;
            }else {
                turn = FIELD_OLED_TURN_THEIRS;
                agent.state = AGENT_STATE_DEFENDING;
            }
                
        }else {
            agent.message.type = MESSAGE_NONE;
        }
    }else if (event.type == BB_EVENT_REV_RECEIVED){
         if (agent.state == AGENT_STATE_ACCEPTING) {
            NegotiationOutcome outcome = NegotiateCoinFlip(agent.negotationdata, event.param0);
            if (NegotiationVerify(event.param0, agent.negotationdata2) == FALSE) {
                char *cheat = "cheating message here, press reset button to start again\n";
                OledDrawString(cheat);
                OledUpdate();
                agent.state = AGENT_STATE_END_SCREEN;
                agent.message.type = MESSAGE_NONE;
                return agent.message;
            }
            if (outcome == TAILS) {
                // determine and send shot here
                turn = FIELD_OLED_TURN_MINE;
                GuessData guess = FieldAIDecideGuess(&agent.player2);
                agent.message.type = MESSAGE_SHO;
                agent.message.param0 = guess.row;
                agent.message.param1 = guess.col;
                agent.state = AGENT_STATE_ATTACKING;
            } else {
                // else if tails
                agent.message.type = MESSAGE_NONE;
                turn = FIELD_OLED_TURN_THEIRS;
                agent.state = AGENT_STATE_DEFENDING;
            }
        } else {
            agent.message.type = MESSAGE_NONE;
        }
    }else if(event.type == BB_EVENT_SHO_RECEIVED){
        if (agent.state == AGENT_STATE_DEFENDING) {
            GuessData opGuess;
            opGuess.row = event.param0;
            opGuess.col = event.param1;
            FieldRegisterEnemyAttack(&agent.player1, &opGuess);
                
            agent.message.type = MESSAGE_RES;
            agent.message.param0 = event.param0;
            agent.message.param1 = event.param1;
            agent.message.param2 = opGuess.result;
                
            if (FieldGetBoatStates(&agent.player1) == 0b00000000) {
                agent.message.type = MESSAGE_NONE;
                char *gameOver = "defeat\n";
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(gameOver);
                OledUpdate();
                agent.state = AGENT_STATE_END_SCREEN;
                return agent.message;
            } else {
                turn = FIELD_OLED_TURN_MINE;
                agent.state = AGENT_STATE_WAITING_TO_SEND;
            }
        } else {
            agent.message.type = MESSAGE_NONE;
        }
    }else if(event.type == BB_EVENT_RES_RECEIVED){
        if (agent.state == AGENT_STATE_ATTACKING) {
            GuessData player1Guess;
            player1Guess.row = event.param0;
            player1Guess.col = event.param1;
            player1Guess.result = event.param2;
            FieldUpdateKnowledge(&agent.player2, &player1Guess);
            if (FieldGetBoatStates(&agent.player2) == 0b00000000) {
                agent.message.type = MESSAGE_NONE;
                char *gameOver = "victory :)\n";
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(gameOver);
                OledUpdate();
                agent.state = AGENT_STATE_END_SCREEN;
                return agent.message;
            } else {
                agent.message.type = MESSAGE_NONE;
                turn = FIELD_OLED_TURN_THEIRS;
                agent.state = AGENT_STATE_DEFENDING;
            }
        } else {
            agent.message.type = MESSAGE_NONE;
        }
    }else if(event.type == BB_EVENT_MESSAGE_SENT){
        if (agent.state == AGENT_STATE_WAITING_TO_SEND) {
            turns++;
            GuessData guess = FieldAIDecideGuess(&agent.player2);
            
            agent.message.type = MESSAGE_SHO;
            agent.message.param0 = guess.row;
            agent.message.param1 = guess.col;            
            agent.state = AGENT_STATE_ATTACKING;
                
        } else {
            agent.message.type = MESSAGE_NONE;
        }
    }else if(event.type == BB_EVENT_RESET_BUTTON){
        agent.message.type = MESSAGE_NONE;
        char *tempWelcome = "Press BTN4 to start \nor wait for challenge\n";
        OledClear(OLED_COLOR_BLACK);
        OledDrawString(tempWelcome);
        OledUpdate();
        AgentInit();
        return agent.message;
        
    }else if(event.type == BB_EVENT_ERROR){
        switch (event.param0) {
            case BB_ERROR_BAD_CHECKSUM:
                printerror = "BB_ERROR_BAD_CHECKSUM";
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(printerror);
                OledUpdate();
                break;
            case BB_ERROR_PAYLOAD_LEN_EXCEEDED:
                printerror = "BB_ERROR_PAYLOAD_LEN_EXCEEDED";
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(printerror);
                OledUpdate();
                break;
            case BB_ERROR_INVALID_MESSAGE_TYPE:
                printerror = "BB_ERROR_INVALID_MESSAGE_TYPE";
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(printerror);
                OledUpdate();
                break;
            case BB_ERROR_MESSAGE_PARSE_FAILURE:
                printerror = "BB_ERROR_MESSAGE_PARSE_FAILURE";
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(printerror);
                OledUpdate();
                break;
            case BB_ERROR_CHECKSUM_LEN_EXCEEDED:
                printerror = "BB_ERROR_CHECKSUM_LEN_EXCEEDED";
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(printerror);
                OledUpdate();
                break;
            case BB_ERROR_CHECKSUM_LEN_INSUFFICIENT:
                printerror = "BB_ERROR_CHECKSUM_LEN_INSUFFICIENT";
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(printerror);
                OledUpdate();
                break;
            default:
                printerror = "default error";
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(printerror);
                OledUpdate();
                break;
            }
            
        agent.state = AGENT_STATE_END_SCREEN;
        agent.message.type = MESSAGE_ERROR;
        return agent.message;    
    }else{
        agent.message.type = MESSAGE_NONE;
    }
    
    OledClear(OLED_COLOR_BLACK);
    FieldOledDrawScreen(&agent.player1, &agent.player2, turn, turns);
    OledUpdate();
    return agent.message;
}

AgentState AgentGetState(void) {
    return agent.state;
}

void AgentSetState(AgentState newState) {
    agent.state = newState;
}