/* 
 * File:   NegotiationTest.c
 * Author: Ezra Frary
 */
#include <stdio.h>
#include <stdlib.h>
#include "Negotiation.h"
#include "Uart1.h"
#include "BOARD.h"

int main() {
    
    int count = 0;
    
    printf("\nTesting negotiation:\n");
    
    printf("\nTesting NegotiationHash()\n");
    
    // testing normal sized hashes
    NegotiationData unknownHash = 9;
    NegotiationData rightHash = (unknownHash * unknownHash) % 0xBEEF;
    //printf("%d", correctHash);
    if (NegotiationHash(unknownHash) == rightHash) {
        printf("\tPassed NegotiationHash() \n");
        count++;
    } else {
        printf("\tFailed NegotiationHash()\n");
    }
    
    // testing hashes with overflow
    if (NegotiationHash(3) == 9) {
        printf("\tPassed NegotiationHash() 1\n");
        count++;
    } else {
        printf("\tfailed\n");
    }
    
    if (NegotiationHash(12345) == 43182) {
        printf("\tPassed NegotiationHash() 2\n");
        count++;
    } else {
        printf("\tfailed\n");
    }
    
    if (NegotiationHash(0) == 0) {
        printf("\tPassed NegotiationHash() 3\n");
        count++;
    } else {
        printf("\tfailed\n");
    }
    
    // making sure it can detect cheating and correctness
    printf("\nTesting NegotiationVerify()\n");
    
    NegotiationData cheat = 14;
    if (NegotiationVerify(cheat, rightHash) == 0 &&
            NegotiationVerify(unknownHash, rightHash) == 1) {
        printf("\tPassed NegotiationVerify()\n");
        count++;
    } else {
        printf("\tFailed NegotiationVerify()\n");
    }
    
    if (NegotiationVerify(3, 9) == 1) {
        printf("\tPassed NegotiationVerify() 2\n");
        count++;
    } else {
        printf("\tfailed\n");
    }
    
    if (NegotiationVerify(12345, 43182) == 1) {
        printf("\tPassed NegotiationVerify() 3\n");
        count++;
    } else {
        printf("\tfailed\n");
    }
    
    // making sure both heads and tails work
    printf("\nTesting NegotiationFlipCoins()\n");
    
    if (NegotiateCoinFlip(unknownHash, cheat) == HEADS) {
        printf("\tPassed NegotiateCoinFlip()\n");
        count++;
    } else {
        printf("\tFailed NegotiateCoinFlip\n");
    }
    
    if (NegotiateCoinFlip(10, 10) == TAILS) {
        printf("\tPassed NegotiateCoinFlip()\n");
        count++;
    } else {
        printf("\tFailed NegotiateCoinFlip\n");
    }
    printf("\nFinal result: %d/9 good job!\n", count);
    

    while(1);
}    