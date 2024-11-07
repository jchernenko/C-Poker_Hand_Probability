// Author: Bob Langelaan
// Date: Nov 16th, 2023

// Modified by: Jeff Chernenko - A00745858
// Date: 2024-02-28

// Card shuffling, dealing and probability of hand types

//These websites may be of benefit on this assignment:
// https://en.wikipedia.org/wiki/Poker_probability
// https://en.wikipedia.org/wiki/List_of_poker_hands#:~:text=A%20straight%20is%20a%20hand,K%E2%99%A3%20Q%E2%99%A3%20J

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUITS 4
#define FACES 13
#define CARDS 52
#define HAND_SIZE 5
#define HANDS_IN_DECK 10
#define NUMBER_OF_HANDS_PLAYED 10000000  //Currently set to 10 million
#define NUMB_HANDS_STR "10 million" //Currently set to 10 million
#define TRUE 1
#define FALSE 0

// prototypes of functions supplied
void deal(const unsigned int wDeck[], const char* wFace[], //display all cards in deck
    const char* wSuit[]);
void dealNextHand(unsigned int wDeck[], unsigned int hand[]); //deal out next hand from the deck
int isFourOfAKind(const unsigned int hand[]);  // return true if hand contains four of a kind and false otherwise

// prototypes of functions you must implement
void swap(unsigned int* const, unsigned int* const); //swap the two cards pointed to by the 2 pointers
void shuffle(unsigned int wDeck[]); //shuffle deck
int isPair(const unsigned int hand[]);  // return true if hand contains a pair and false otherwise
int isTwoPair(const unsigned int hand[]);  // return true if hand contains a two pair and false otherwise
int isThreeOfAKind(const unsigned int hand[]);  // return true if hand contains three of a kind and false otherwise
int isStraight(const unsigned int hand[]);  // return true if hand is a straight and false otherwise
int isFlush(const unsigned int hand[]);  // return true if hand is a flush and false otherwise
int isFullHouse(const unsigned int hand[]);  // return true if hand is a full house and false otherwise
int isRoyalFlush(const unsigned int hand[]);  // return true if hand is a royal flush and false otherwise

int main(void)
{
    // define and initialize deck array
    unsigned int deck[CARDS];

    // initialize deck with values 0 to CARDS-1
    // value / 13 caluclates suit #     { "Hearts", "Diamonds", "Clubs", "Spades" };
    // value % 13 calculates face card  {Ace, 2, 3, ... 10, Jack, Queen, King}
    for (size_t card = 0; card < CARDS; ++card)
    {
        deck[card] = card;
    }

    srand((unsigned int)time(NULL)); // seed random-number generator

    // initialize suit array
    const char* suit[SUITS] =
    { "Hearts", "Diamonds", "Clubs", "Spades" };

    // initialize face array
    const char* face[FACES] =
    { "Ace", "Deuce", "Three", "Four",
     "Five", "Six", "Seven", "Eight",
     "Nine", "Ten", "Jack", "Queen", "King" };

    shuffle(deck);  // uncomment after completing the implementation
    // of the swap() and shuffle() functions

    deal(deck, face, suit); // display the deck unshuffled

    unsigned int hand[HAND_SIZE]; // will contain the cards in the hand.

    //Define and initialize variables used to count each type of hand
    unsigned int pairCount = 0;
    unsigned int twoPairCount = 0;
    unsigned int threeOfAKindCount = 0;
    unsigned int straightCount = 0;
    unsigned int flushCount = 0;
    unsigned int fullHouseCount = 0;
    unsigned int fourOfAKindCount = 0;
    unsigned int straightFlushCount = 0;
    unsigned int royalFlushCount = 0;

    // Shuffle the deck for the first time
    // After this, we shuffle deck every time we do not have enough undealth cards
    // for a complete hand which will be every 10 deals assuming five card hands
    shuffle(deck);

    // Deal out NUMBER_OF_HANDS_PLAYED hands
    for (size_t hands = 1; hands < NUMBER_OF_HANDS_PLAYED; ++hands)
    {
        dealNextHand(deck, hand); // Deal out next 5 cards from the deck into the array hand

        // Does hand have a pair?
        if (isPair(hand))
        {
            ++pairCount;  // Yes, increment pair count
        }

        // Does hand have two pair?
        if (isTwoPair(hand))
        {
            ++twoPairCount;
        }

        // Does hand have three of a kind?
        if (isThreeOfAKind(hand))
        {
            ++threeOfAKindCount;
        }

        // check hand for a straight or flush
        if (isStraight(hand) || isFlush(hand)) {
            if (isFlush(hand)) {                    // if it's a flush or a straight, check for straight flush or royal flush
                if (isRoyalFlush(hand)) {           // check if it's a royal flush
                    ++royalFlushCount;              // if yes, increment royal flush count
                }
                else if (isStraight(hand)) {        // if not a royal flush, check for a straight flush
                    ++straightFlushCount;           // if yes, increment straight flush
                }
                else {
                    ++flushCount;                   // increment flush if not a straight
                }
            }
            else {
                ++straightCount;                    // increment straight if not a flush
            }
        }

        // Does hand have a flush?
        else if (isFlush(hand)) 
        {
            ++flushCount;
        }

        // Does hand have a full house?
        if (isFullHouse(hand))
        {
            ++fullHouseCount;
        }

        // Does hand have four of a kind?
        if (isFourOfAKind(hand))
        {
            ++fourOfAKindCount;
        }
    }

    printf("\nA pair occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a pair is %.2f %c", pairCount, pairCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nTwo pair occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of two pair is %.2f %c", twoPairCount, twoPairCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nThree of a kind occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of three of a kind is %.2f %c", threeOfAKindCount, threeOfAKindCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nA straight occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a straight is %.3f %c", straightCount, straightCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nA flush occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a flush is %.3f %c", flushCount, flushCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nA full house occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a full house is %.4f %c", fullHouseCount, fullHouseCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nFour of a kind occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of four of a kind is %.4f %c", fourOfAKindCount, fourOfAKindCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nStraight flush occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a straight flush is %.5f %c", straightFlushCount, straightFlushCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    printf("\nRoyal flush occurred %d times in "NUMB_HANDS_STR" hands, which means the probability of a royal flush is %.5f %c", royalFlushCount, royalFlushCount * 100.0 / NUMBER_OF_HANDS_PLAYED, '%');
    puts("");

}

// Display all of the cards in the deck
void deal(const unsigned int wDeck[], const char* wFace[],
    const char* wSuit[])
{
    // deal each of the cards
    for (size_t card = 0; card < CARDS; ++card) {
        size_t suit = wDeck[card] / FACES;
        size_t face = wDeck[card] % FACES;
        printf("%5s of %-8s%c", wFace[face], wSuit[suit],
            card % 4 == 3 ? '\n' : '\t'); // 4-column format
    }
}

// Deal out the next HAND_SIZE cards into the array hand[]
// If not enough undealt cards left in wDeck, shuffle wDeck and
//    start dealing from beginning of the wDeck again.
void dealNextHand(unsigned int wDeck[], unsigned int hand[])
{
    static unsigned int currentCard = 0; //remembers which is the next card to be dealt.

    // Shuffle deck and start from beginning of the deck if not enough cards left for a whole hand
    if ((currentCard + HAND_SIZE) >= CARDS)
    {
        shuffle(wDeck);
        currentCard = 0;
    }

    // Deal out HAND_SIZE cards from wDeck into the hand array
    for (size_t card = 0; card < HAND_SIZE; ++card)
    {
        hand[card] = wDeck[currentCard++];
    }
}

/*

Below is the complete implementation of the isFourOfAKind() function.

You should be able to use a variation of this function to compute two of a kind, three of a kind and full house.

The straight, flush and royal flush(optional) calculations will require different logic.

After you have completed the swap() and shuffle() functions and uncommented the call to shuffle(),
this start code should run without errors and the your result for 4 of a kind should be very close to
my results in my sample output.  Then you will be ready to tackle completing the other function stubs.

*/

// Returns true if hand contains four of kind and false otherwise
int isFourOfAKind(const unsigned int hand[])
{
    unsigned int faceCnt[FACES] = { 0 };

    // Calculate the frequency of each face card in the hand
    // In other words, it calculates how many Aces, deuces, 3's, etc. in the hand of 5 cards
    for (size_t card = 0; card < HAND_SIZE; ++card)
    {
        ++faceCnt[hand[card] % FACES];
    }

    // Iterate through all of the frequency counts to find out if
    //     any face card occurred 4 times in the hand
    for (size_t face = 0; face < FACES; ++face)
    {
        if (faceCnt[face] == 4)
        {
            return TRUE; // Yes!!! Return true.
        }
    }

    return FALSE; //Nope ;-(
}

// Student implements the 8 functions below

// Swap the two unsigned ints pointed to by the pointers card1 and card2
void swap(unsigned int* const card1, unsigned int* const card2) {

    unsigned int temp = *card1;
    *card1 = *card2;
    *card2 = temp;
}

// Shuffle cards in deck
// Must invoke swap() function above
void shuffle(unsigned int wDeck[]) {

    for (size_t i = 0; i < CARDS; ++i) {
        size_t j = rand() % CARDS; // shuffle
        swap(&wDeck[i], &wDeck[j]); // swap the current card with the randomly selected card
    }
}

// Returns true if hand contains only a pair and false other wise
// NOTE: Will return FALSE if the hand contains any of these hand types as well:
//  1) Hand has 2 pairs
//  2) Hand has 3 of a kind
//  2) Hand is a full house
//  3) Hand has 4 of a kind
int isPair(const unsigned int hand[]) {

    unsigned int faceCount[FACES] = { 0 };

    // count the number of each face card in the hand
    for (size_t card = 0; card < HAND_SIZE; ++card) {
        ++faceCount[hand[card] % FACES];
    }

    int pairCount = 0;

    for (size_t face = 0; face < FACES; ++face) {
        if (faceCount[face] == 2) {
            ++pairCount; // if pair located, increment pairCount
        }
    }

    // check for exactly one pair and no other combinations
    if (pairCount == 1) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// Returns true if hand contains two pairs and false otherwise
// NOTE: Will return FALSE if the hand contains any of these hand types as well:
//  1) Hand has 3 of a kind
//  2) Hand is a full house
//  3) Hand has 4 of a kind
int isTwoPair(const unsigned int hand[]) {

    unsigned int faceCount[FACES] = { 0 };

    // count the number of each face card in the hand
    for (size_t card = 0; card < HAND_SIZE; ++card) {
        ++faceCount[hand[card] % FACES];
    }

    int pairCount = 0;

    for (size_t face = 0; face < FACES; ++face) {
        if (faceCount[face] == 2) {
            ++pairCount; // if pair located, increment pairCount
        }
    }

    // check for two pairs and no other combinations
    if (pairCount == 2) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// Returns true if hand contains 3 of a kind and false otherwise
// NOTE: Will return FALSE if the hand contains any of these hand types as well:
//  1) Hand has a full house
//  2) Hand has 4 of a kind
int isThreeOfAKind(const unsigned int hand[]) {

    unsigned int faceCount[FACES] = { 0 };

    // count the number of each face card in the hand
    for (size_t card = 0; card < HAND_SIZE; ++card) {
        ++faceCount[hand[card] % FACES];
    }

    for (size_t face = 0; face < FACES; ++face) {
        if (faceCount[face] == 3) {
            return TRUE; // if 3 of a kind located, return true
        }
    }
    return FALSE;
}

// Returns true if hand is a straight and false otherwise
// This function does NOT check if also a flush.

// VERY IMPORTANT: The Ace can be used to create a straight in 2 different ways:
//  Ace, 2, 3, 4, 5 and
//  10, Jack, Queen, King, Ace
//  You need to check for both uses of the Ace to create a straight.
//
//  Note there are other ways to get a straight that do not require an Ace
//       For example, 8, 9 , 10, J , Q is also a straight.
int isStraight(const unsigned int hand[]) {

    unsigned int faceCount[FACES] = { 0 };
    unsigned int minFace = 13;
    unsigned int maxFace = 0;

    for (size_t card = 0; card < HAND_SIZE; ++card) {

        unsigned int face = hand[card] % FACES;

        ++faceCount[face];

        if (face < minFace) {
            minFace = face;
        }
        if (face > maxFace) {
            maxFace = face;
        }
    }

    // check for ace to five straight
    if (minFace == 0 && maxFace == 12 && faceCount[1] && faceCount[2] && faceCount[3] && faceCount[4])
        return TRUE;

    int straightCount = 0;

    // check for other straights
    for (unsigned int face = minFace; face <= maxFace && faceCount[face]; ++face) {
        ++straightCount;
    }

    if (straightCount == HAND_SIZE) { // if straight sequence is found, return true
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// Returns true if hand is a flush and false otherwise
int isFlush(const unsigned int hand[]) {

    int suit = hand[0] / FACES; // start with finding suit of the first card

    for (size_t card = 1; card < HAND_SIZE; ++card) {
        if (hand[card] / FACES != suit) {
            return FALSE; // return false if different suit located
        }
    }

    return TRUE; // else return true
}

// return true if hand is a full house and false otherwise
int isFullHouse(const unsigned int hand[]) {

    unsigned int faceCount[FACES] = { 0 };

    for (size_t card = 0; card < HAND_SIZE; ++card) {
        ++faceCount[hand[card] % FACES];
    }

    int hasThreeOfAKind = FALSE;
    int hasPair = FALSE;

    for (size_t face = 0; face < FACES; ++face) {
        if (faceCount[face] == 3) // check for three of a kind
            hasThreeOfAKind = TRUE;
        else if (faceCount[face] == 2) // check for pair
            hasPair = TRUE;
    }

    if (hasThreeOfAKind == TRUE && hasPair == TRUE) { // if hand has a three of a kind and a pair, return true
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// return true if hand is a royal flush and false otherwise
int isRoyalFlush(const unsigned int hand[]) {

    // start with checking if hand is a flush
    int suit = hand[0] / FACES;
    for (size_t card = 1; card < HAND_SIZE; ++card) {
        if (hand[card] / FACES != suit) {
            return FALSE; // not a flush
        }
    }

    // if flush is true, check for ace-high straight
    unsigned int faceCount[FACES] = { 0 };
    for (size_t card = 0; card < HAND_SIZE; ++card) {
        ++faceCount[hand[card] % FACES];
    }

    // if flush and ace-high straight are true, check for straight that is exactly 10, J, Q, K, Ace
    if (faceCount[0] == 1 && // Ace
        faceCount[9] == 1 && // 10
        faceCount[10] == 1 && // J
        faceCount[11] == 1 && // Q
        faceCount[12] == 1)   // K
    {
        return TRUE; // it's a royal flush
    }

    return FALSE; // it's not a royal flush
}
