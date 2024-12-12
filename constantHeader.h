#define BUFSIZE 128

const char *welcomeMessage = "Welcone in ENSEA's Shell.\n";
const char *quitMessage = "To quit, type 'exit'.\n";
const char *exitCommand = "exit";
const char *exitMessage = "\nSee you next time!\n";


char buffer[BUFSIZE] = {0};
char statusMessage[BUFSIZE];
char promptMessage[BUFSIZE] = "enseash % ";