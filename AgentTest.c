/* 
 * File:   AgentTest.c
 * Author: Nghia Pham
 */
#include <stdio.h>
#include <stdlib.h>
#include "Agent.h"
#include "BattleBoats.h"
int main() {    
    printf("Testing AgentSetSate and AgentGetState:\n");    
    
    int count = 0;
    
    AgentSetState(AGENT_STATE_START);
    if(AgentGetState() == AGENT_STATE_START){
        printf("Pass!:\n");
        count++;
    }else{
        printf("Something Fail!\n");
    }
    AgentSetState(AGENT_STATE_CHALLENGING);
    if(AgentGetState() == AGENT_STATE_CHALLENGING){
        printf("Pass!:\n");
        count++;
    }else{
        printf("Something Fail!\n");
    }
    AgentSetState(AGENT_STATE_ACCEPTING);
    if(AgentGetState() == AGENT_STATE_ACCEPTING){
        printf("Pass!:\n");
        count++;
    }else{
        printf("Something Fail!\n");
    }
    AgentSetState(AGENT_STATE_ATTACKING);
    if(AgentGetState() == AGENT_STATE_ATTACKING){
        printf("Pass!:\n");
        count++;
    }else{
        printf("Something Fail!\n");
    }
    AgentSetState(AGENT_STATE_DEFENDING);
    if(AgentGetState() == AGENT_STATE_DEFENDING){
        printf("Pass!:\n");
        count++;
    }else{
        printf("Something Fail!\n");
    }
    AgentSetState(AGENT_STATE_WAITING_TO_SEND);
    if(AgentGetState() == AGENT_STATE_WAITING_TO_SEND){
        printf("Pass!:\n");
        count++;
    }else{
        printf("Something Fail!\n");
    }
    AgentSetState(AGENT_STATE_END_SCREEN);
    if(AgentGetState() == AGENT_STATE_END_SCREEN){
        printf("Pass!:\n");
        count++;
    }else{
        printf("Something Fail!\n");
    }
  
    printf("Testing AgentInIt:\n");
    AgentInit();
    if(AgentGetState() == AGENT_STATE_START){
        printf("Pass!:\n");
        count++;
    }else{
        printf("Something Fail!\n");
    }  
    
    
    printf("Testing AgentRun:\n");
    BB_Event event;
    event.type = BB_EVENT_START_BUTTON;
    event.param0 = 0;
    event.param1 = 0;
    event.param2 = 0;
    AgentRun(event);    
    if(AgentGetState() == AGENT_STATE_CHALLENGING){
        printf("Pass!:\n");
        count++;
    }else{
        printf("Something Fail!\n");
    }        
    printf("\nFinal result: %d/9 good job!\n", count);  
    
    while(1);    
}