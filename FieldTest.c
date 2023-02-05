/* 
 * File:   FieldTest.c
 * Author: Ezra Frary
 */
#include <stdio.h>
#include <stdlib.h>
#include "Field.h"
#include "Uart1.h"
#include "BOARD.h"

static Field testOwnField;
static Field testOppField;
static Field TestingResult;
static GuessData TakeAguess;


int main() {
    
    printf("Testing Field:\n\n");
    int counter = 0;
    // Test for init
    FieldInit(& testOwnField, &testOppField);
    FieldPrint_UART(& testOwnField, &testOppField);
     if (FieldGetSquareStatus(& testOwnField, 1, 1) == FIELD_SQUARE_EMPTY) {
        printf("\nFieldGetSquareStatus(): success\n");
        counter++;
    } else {
        printf("\nFailed!!\n");
    }
    
    // set square test
    FieldSetSquareStatus(& testOwnField, 0, 0, FIELD_SQUARE_SMALL_BOAT);
    if (FieldGetSquareStatus(& testOwnField, 0, 0) == FIELD_SQUARE_SMALL_BOAT) {
        printf("FieldSetSquareStatus(): success\n\n");
        counter++;
    } else {
        printf("Failed!!\n");
    }
    
    FieldSetSquareStatus(&testOwnField, 0, 0, FIELD_SQUARE_EMPTY);
    
    // testing add boat, using field print uart as visual confirmation
    printf("Testing Add boat:\n");
    FieldAddBoat(& testOwnField, 1, 3, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(& testOwnField, 3, 4, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);
    FieldAddBoat(& testOwnField, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_LARGE);
    
    FieldPrint_UART(&testOwnField, &testOppField);
    //  clear the fields
    FieldInit(&testOwnField, &TestingResult);
    FieldInit(&testOppField, &TestingResult);
    
    if (FieldAIPlaceAllBoats(&testOwnField) == SUCCESS) {
        printf("\nFieldAIPlaceAllBoats(): success\n\n");
        counter++;
    } else {
        printf("\nFailed!!\n\n");
    }
    
    FieldAIPlaceAllBoats(&testOppField); 
    TakeAguess = FieldAIDecideGuess(&TestingResult);
    
    
    SquareStatus squareHit = FieldRegisterEnemyAttack(&testOppField, &TakeAguess);
    if (squareHit != FieldGetSquareStatus(&testOppField, TakeAguess.row, TakeAguess.col)) {
        printf("\nFieldAIDecideGuess() and FieldRegisterEnemyAttack(): success\n");
        counter++;
    } else {
        printf("Failed!!\n");
    }
    
    printf("\nFinal result: %d/4 well done!\n", counter);
    while(1);
    
}