#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

// reads from keypress, doesn't echo 
int getch()			//Defining function of getch(), only relevant for linux users
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
#endif
#pragma warning (disable:4996)

typedef struct {
	char memberID[6], username[30], password[50], email[254], gender[2], contact_No[12];
}Member;


bool login(Member* place_to_put_member); 					// Login Function
void signUp(); 												// Register Function
void passwordRec(); 										// Password Recovery Function
int stringInput(const char* prompt, char* buffer, int n); 	// Used as a one-liner for prompting and receiving input for string
int charInput(const char* prompt, char* c);					// Used as a one-liner for prompting and receiving input for char
void flush(FILE* stream); 									// Flushes overflowing data

void main() {												// Main Menu. Branches off into Login, Sign Up, Password Recovery, Close Program.
	bool exit = false;
	int memberOption;
	Member current_member;

	printf("Welcome Member!\n\n");
	while (!exit) {
		printf("Choose your Desired Mode.\n");
		printf("1. Login\n");
		printf("2. Sign Up\n");
		printf("3. I forgor my password :skull:\n");
		printf("4. Exit\n\n");
		printf(">>>>> ");
		scanf("%d", &memberOption);
		flush(stdin);

		switch (memberOption) {
		case 1:
			if (login(&current_member) == true){		// Login function. If login is a success (receives value 'true'), run everything below.
				printf("\n\nWelcome, %s!\n", current_member.username);
				printf("\n%s\n", current_member.password);
				printf("\n%s\n", current_member.email);
				printf("\n%c\n", current_member.gender);
				printf("\n%s\n", current_member.contact_No);
				getch();
				exit = true;
			}

			break;
		case 2:
			signUp();									// Sign Up function. Runs login() if success (not implemented yet)
			break;
		case 3:
			passwordRec();								// Password Recovery function. Runs login() if success (not implemented yet)
			break;
		case 4:
			exit=true;
			break;
		default:
			printf("\nInvalid option! Press any key to Retry...\n");
			getch();
			system("clear");
		}
	}
}

bool login(Member* place_to_put_member) {
	char loginName[30], loginPassword[50];
	FILE* fMem;
	Member memberBuffer;			// Temporary spot to store the structer read from .bin file. Used to compare with user input.

	fMem = fopen("memberlist.bin", "rb");
	if (fMem == NULL) {
		printf("Error at opening File!");
		exit(1);
	}

	stringInput("Enter your username > ", loginName, 30);				// Getting username INPUT
	stringInput("Enter your password > ", loginPassword, 50);			// Getting password INPUT

	while (fread(&memberBuffer, sizeof(Member), 1, fMem)) {				// Looping to read each structure variable saved into memberlist.bin.

		if (strcmp(memberBuffer.username, loginName) == 0) {			// If the program finds a match on the username....

			if (strcmp(memberBuffer.password, loginPassword) == 0) {	// ...and If it finds that the password also matches...
				printf("Login Successful!\n");							
				*place_to_put_member = memberBuffer;					// it will pass the values of the temporary storage (user input) to the the struct that stores the member currently logged in.
				fclose(fMem);
				return true;											// and it will return the boolean value 'true'.
			}
			else {
				printf("Wrong Password!");								// If the password doesn't match, restart.
				printf(" Press any key to try again...\n");
				getch();
				system("clear");
				break;
			}
		}
	}
	if (strcmp(memberBuffer.username,loginName) !=0) {
		printf("Username Not Found! Press any key to return to Main Menu...\n");
		getch();
	}
	fclose(fMem);
	return false;
}


void signUp() {
	Member new_member;

	FILE* fMem;
	fMem = fopen("memberlist.bin", "ab");
	if (fMem == NULL) {
		printf("Error at opening File!");
		exit(1);
	}

	stringInput("Enter your username > ", new_member.username, 30);
	stringInput("Enter your password > ", new_member.password, 50);
	stringInput("Enter your email > ", new_member.email, 254);
	while (charInput("Enter your gender (M,F) > ", new_member.gender) !=0) {
		printf("Dumbass do it again.\n\n");
		getch();
		system("clear");
	}
	stringInput("Enter your contact number > ", new_member.contact_No, 12);

	fwrite(&new_member, sizeof(new_member), 1, fMem);
	printf("\n\nYou have successfully registered an account! Press any button to proceed to Login Page...");
	fclose(fMem);
	getch();
}

void passwordRec() {
	int recOption;
	char loginName[30] = {};
	bool back = false;
	Member memberBuffer;

	FILE* fMem;
	fMem = fopen("memberlist.bin", "rb");

	stringInput("\nEnter your username > ", loginName, 30);
	while (fread(&memberBuffer, sizeof(Member), 1, fMem)) {	
		if (strcmp(memberBuffer.username, loginName) == 0) {
			while (!back) {
				printf("\nChoose Password Recovery Method.\n");
				printf("1. Email Verification\n");
				printf("2. Phone SMS\n");
				printf("3. Back to Main Menu\n\n");
				printf(">>>>> ");
				scanf("%d", &recOption);
				flush(stdin);

				switch (recOption) {
				case 1:

					break;
				case 2:
					break;
				case 3:
					back = true;
					break;
				default:
					printf("???wtf, try the fuck again.");
					getch();
					system("clear");
					break;
				}
			}
			break;
		} // else {Do nothing.}
	}
	if (back == false) {
		printf("Username Not Found! Press any key to return to Main Menu...");
		getch();
	}
	
}


int stringInput(const char* prompt, char* buffer, int n) {
	printf("%s", prompt);

	if (fgets(buffer, n, stdin) == NULL)
		// no input
		return 1;

	if (buffer[strlen(buffer) - 1] != '\n') {
		// input is longer than buffer can hold, \n is still there, need to flush (toilet)
		flush(stdin);
	}

	// remove \n
	buffer[strcspn(buffer, "\n")] = 0;

	if (buffer[0] == 0)
		// Fucking Idiot pressed "ENTER" who tf does that
		return 1;

	// i love you
	return 0;
}

int charInput(const char* prompt, char* c) {
    printf("%s", prompt);
    
    int ch = getc(stdin);

    if (ch == '\n')
        // Fucking Idiot pressed "ENTER" who tf does that
        return 1;

    *c = (char) ch;
	flush(stdin);
    return 0;
}

void flush(FILE* stream) {
	int c;
	while ((c = getc(stream)) != '\n' && c != EOF);
}