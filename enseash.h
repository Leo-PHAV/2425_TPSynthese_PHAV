#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


void displayWelcomeMessage();
void displayPrompt(const char *message);
ssize_t readUserCommand(char *buffer, size_t bufferSize);
void executeCommand(const char *command, char *promptMessage);
int calculateElapsedTime(const struct timespec *start, const struct timespec *end);
void complexCommand(const char *buf, char **args, size_t maxArgs);
int handleRedirections(char **args);
