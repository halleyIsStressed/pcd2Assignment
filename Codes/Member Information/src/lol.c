#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

/* reads from keypress, doesn't echo */
int getch()
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

void main() {
	bool exit = false;
	int memberOption;
	Member current_member;

	printf("Welcome Member!");
	while (!exit) {
		printf("\nChoose your Desired Mode.\n");
		printf("1. Login\n");
		printf("2. Sign Up\n");
		printf("3. I forgor my password :skull:\n");
		printf("4. Exit\n\n");
		printf(">>>>> ");
		scanf("%d", &memberOption);
		flush(stdin);

		switch (memberOption) {
		case 1:
			if (login(&current_member) == true){
				printf("\n\nWelcome, %s!\n", current_member.username);
				printf("\n\n%s\n", current_member.password);
				printf("\n\n%s\n", current_member.email);
				printf("\n\n%c\n", current_member.gender);
				printf("\n\n%s\n", current_member.contact_No);
			}
			break;
		case 2:
			signUp();
			break;
		case 3:
			passwordRec();
			break;
		case 4:
			exit=true;
			break;
		default:
			printf("\nInvalid option! Fuck off lmao\n");
			getch();
		}
	}
}

bool login(Member* place_to_put_member) {
	char loginName[30], loginPassword[50];
	FILE* fMem;
	Member memberBuffer;

	fMem = fopen("memberlist.bin", "rb");
	if (fMem == NULL) {
		printf("Error at opening File!");
		exit(1);
	}

	stringInput("Enter your username > ", loginName, 30);
	stringInput("Enter your password > ", loginPassword, 50);

	while (fread(&memberBuffer, sizeof(Member), 1, fMem)) {

		if (strcmp(memberBuffer.username, loginName) == 0) {

			if (strcmp(memberBuffer.password, loginPassword) == 0) {
				printf("Login Successful!\n");
				*place_to_put_member = memberBuffer;
				fclose(fMem);
				return true;
			}
			else {
				printf("Wrong Password!\n");
			}
		}
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
	}
	stringInput("Enter your contact number > ", new_member.contact_No, 12);

	fwrite(&new_member, sizeof(new_member), 1, fMem);
	printf("\n\nYou have successfully registered an account! Press any button to proceed to Login Page...");
	fclose(fMem);
	getch();

	
}


void passwordRec() {
	printf("Password Rec");
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