#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

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
	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
	return ch;
}
#endif
#define IC_SIZE 13
#define USERNAME_SIZE 30
#define PASSWORD_SIZE 50
#define EMAIL_SIZE 254
#define GENDER_SIZE 2
#define CONTACT_SIZE 12
#pragma warning (disable:4996)

typedef struct {
	char ic[IC_SIZE], username[USERNAME_SIZE], password[PASSWORD_SIZE], 
	email[EMAIL_SIZE], gender[GENDER_SIZE], contact_No[CONTACT_SIZE];
}Member;

bool login(Member* place_to_put_member); 					// Login Function
bool memberMenu(Member*);
bool passwordRec(Member* place_to_put_member); 				// Password Recovery Function
void signUp(); 												// Register Function
int randomNumGen();
int stringInput(const char* prompt, char* buffer, int n); 	// Used as a one-liner for prompting and receiving input for string
int charInput(const char* prompt, char* c);					// Used as a one-liner for prompting and receiving input for char
void flush(FILE* stream); 									// Flushes overflowing data


	// User Interface Functions
void main() {												// Main Menu. Branches off into Login, Sign Up, Password Recovery, Close Program.
	bool exit = false;
	Member current_member;

	printf("Welcome User!\n\n");
	while (!exit) {
		int loginOption = 0;
		printf("Choose your Desired Mode.\n");
		printf("1. Login\n");
		printf("2. Sign Up\n");
		printf("3. I forgor my password :skull:\n");
		printf("4. Exit\n\n");
		printf(">>>>> ");
		scanf("%d", &loginOption);
		flush(stdin);

		switch (loginOption) {
		case 1:
			if (login(&current_member) == true) {		// Login function. If login is a success (receives value 'true'), run everything below.
				printf("\n\nWelcome, %s!\n", current_member.username);
				memberMenu(&current_member);
			}
			break;
		case 2:
			signUp();									// Sign Up function. Runs login() if success (not implemented yet)
			break;
		case 3:
			if (passwordRec(&current_member) == true) {	// Password Recovery function. Runs login() if success (not implemented yet)
				printf("\n\nWelcome, %s!\n", current_member.username);
				printf("Your password is : %s\n", current_member.password);
				memberMenu(&current_member);
			}
			break;
		case 4:
			printf("\n\nSee you!");
			getch();
			return -1;
			break;
		default:
			printf("\nInvalid option! Press any key to Retry...\n");
			getch();
			system("cls");
		}
	}
	/* if (warning == 1) {						//Combine: yj part
	*	display thingy;
	* } 
	* else if (warning == 2) {
	*	display thingy but worse;
	* }
	* else if (warning == 3) {
	*	display termination message;
	*	return -1;
	* }
	*/

}

bool login(Member* place_to_put_member) {
	char loginName[USERNAME_SIZE], loginPassword[PASSWORD_SIZE];
	FILE* fMem;
	Member memberBuffer;			// Temporary spot to store the structer read from .bin file. Used to compare with user input.

	fMem = fopen("memberlist.bin", "rb");
	if (fMem == NULL) {
		printf("Error at opening File!");
		exit(1);
	}

	stringInput("Enter your username > ", loginName, USERNAME_SIZE);
	stringInput("Enter your password > ", loginPassword, PASSWORD_SIZE);

	while (fread(&memberBuffer, sizeof(Member), 1, fMem)) {				// Looping to read each structure variable saved into memberlist.bin.

		if (strcmp(memberBuffer.username, loginName) == 0) {			// If the program finds a match on the username....

			if (strcmp(memberBuffer.password, loginPassword) == 0) {	// ...and If it finds that the password also matches...
				system("cls");
				printf("Login Successful!\n");
				*place_to_put_member = memberBuffer;					// it will pass the values of the temporary storage (user input) to the the struct that stores the member currently logged in.
				fclose(fMem);
				return true;											// and it will return the boolean value 'true'.
			}
			else {
				printf("Wrong Password!");								// If the password doesn't match, restart.
				printf(" Press any key to try again...\n");
				getch();
				system("cls");
				break;
			}
		}
	}
	if (strcmp(memberBuffer.username, loginName) != 0) {
		printf("Username Not Found! Press any key to return to Main Menu...\n");
		getch();
		system("cls");
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
	system("cls");
	stringInput("Enter your username\t\t> ", new_member.username, USERNAME_SIZE);
	stringInput("Enter your password\t\t> ", new_member.password, PASSWORD_SIZE);
	stringInput("Enter your ic (without '-')\t> ", new_member.ic, IC_SIZE);
	stringInput("Enter your email\t\t> ", new_member.email, EMAIL_SIZE);
	while (!(stringInput("Enter your gender (M,F)\t\t> ", new_member.gender, GENDER_SIZE) == 0 && (new_member.gender[0] == 'M' || new_member.gender[0] == 'F'))) {
		printf("Dumbass do it again.\n\n");
		getch();
		system("cls");
	}
	stringInput("Enter your contact number\t> +60", new_member.contact_No, CONTACT_SIZE);

	fwrite(&new_member, sizeof(new_member), 1, fMem);
	printf("\n\nYou have successfully registered an account! Press any button to return to Main Menu...\n");
	fclose(fMem);
	getch();
	system("cls");
}

bool passwordRec(Member* place_to_put_member) {
	int recOption, code = 0, codeAns;
	char loginName[USERNAME_SIZE] = {NULL};
	bool back = false;
	Member memberBuffer;

	FILE* fMem;
	fMem = fopen("memberlist.bin", "rb");
	system("cls");
	stringInput("\nEnter your username > ", loginName, USERNAME_SIZE);
	while (fread(&memberBuffer, sizeof(Member), 1, fMem)) {
		if (strcmp(memberBuffer.username, loginName) == 0) {
			while (!back) {
				*place_to_put_member = memberBuffer;
				code = randomNumGen();
				printf("%d", code);
				printf("\n\nA Code is sent to your Email! Please enter it here > ");
				scanf("%d", &codeAns);
				if (codeAns == code) {
					*place_to_put_member = memberBuffer;
					fclose(fMem);
					return true;
				}
				else {
					printf("\n\nCode Missmatch! Returning to Main Menu...");
				}
			}
			break;
		} // else {Do nothing.}
	}
	if (back == false) {
		printf("Username Not Found! Press any key to return to Main Menu...");
		getch();
		system("cls");
	}

}

bool memberMenu(Member* current_member) {
	char confirm[8];
	bool backToMain = false;
	FILE* fMod;
	Member memberBuffer;			// Temporary spot to store the structer read from .bin file. Used to compare with user input.


	while (backToMain == false) {
		int memberOption = 0, profileOption = 0;
		printf("Choose Function.\n");
		printf("1. View Profile Details\n");
		printf("2. Display Train Schedule List\n");			// Combine: zw part
		printf("3. Book a Train Ticket\n");					// Combine: zy part
		printf("4. Lost and Found\n");
		printf("5. Return to Main Menu\n\n");
		printf(">>>>> ");
		scanf("%d", &memberOption);
		flush(stdin);

		switch (memberOption) {
		case 1:
			printf("\nName\t\t> %s\n", current_member->username);
			printf("Gender\t\t> %s\n", current_member->gender);
			printf("Email\t\t> %s\n", current_member->email);
			printf("Phone No.\t> +60%s\n", current_member->contact_No);
			printf("Choose Action.\n");
			printf("1. Edit Details\n");
			printf("2. Delete Account\n");
			printf("3. Return to Member Interface\n\n");
			printf(">>>>> ");
			scanf("%d", &profileOption);
			flush(stdin);
			switch (profileOption) {
			case 1:
				// EDIT START
				fMod = fopen("memberlist.bin", "r+");
				if (fMod == NULL) {
					printf("Error at opening File!");
					exit(1);
				}
				int modifyOption;
				char newIc[IC_SIZE], newUser[USERNAME_SIZE], newPassword[PASSWORD_SIZE],
					newEmail[EMAIL_SIZE], newGender[GENDER_SIZE], newContact[CONTACT_SIZE];
				printf("Choose Field to Edit.\n");
				printf("1. Username\n");
				printf("2. Password\n");
				printf("3. Email\n");
				printf("4. Gender\n");
				printf("5. Contact Number\n");
				printf("6. Return to Member Interface\n\n");
				printf(">>>>> ");
				scanf("%d", &modifyOption);
				flush(stdin);
				switch (modifyOption) {
				case 1:
					stringInput("\nEnter new username > ", newUser, USERNAME_SIZE);
					while (!feof(fMod)) {
						fread(&memberBuffer, sizeof(Member), 1, fMod);
						if (strcmp(memberBuffer.ic, current_member->ic) == 0) {
							fseek(fMod, IC_SIZE - sizeof(Member), SEEK_CUR);
							fwrite(newUser, sizeof(USERNAME_SIZE), 1, fMod);
							break;
						}
					}
					break;
				case 2:
					stringInput("\nEnter new password > ", newPassword, PASSWORD_SIZE);
					while (!feof(fMod)) {
						while (fread(&memberBuffer, sizeof(Member), 1, fMod)) {
							if (strcmp(memberBuffer.ic, current_member->ic) == 0) {
								fseek(fMod, (IC_SIZE + USERNAME_SIZE) - sizeof(Member), SEEK_CUR);
							}
						}
					}
					break;
				case 3:
					stringInput("\nEnter new Email > ", newEmail, EMAIL_SIZE);
					while (!feof(fMod)) {
						while (fread(&memberBuffer, sizeof(Member), 1, fMod)) {
							if (strcmp(memberBuffer.ic, current_member->ic) == 0) {
								fseek(fMod, (IC_SIZE + USERNAME_SIZE + PASSWORD_SIZE) - sizeof(Member), SEEK_CUR);
							}
						}
					}
					break;
				case 4:
					while (!(stringInput("Enter new gender (M,F)\t\t> ", newGender, GENDER_SIZE) == 0 && (newGender[0] == 'M' || newGender[0] == 'F'))) {
						printf("Dumbass do it again.\n\n");
						getch();
						system("cls");
					}
					while (!feof(fMod)) {
						while (fread(&memberBuffer, sizeof(Member), 1, fMod)) {
							if (strcmp(memberBuffer.ic, current_member->ic) == 0) {
								fseek(fMod, (IC_SIZE + USERNAME_SIZE + PASSWORD_SIZE + EMAIL_SIZE) - sizeof(Member), SEEK_CUR);
							}
						}
					}
					break;
				case 5:
					stringInput("\nEnter new phone number > +60", newContact, CONTACT_SIZE);
					while (!feof(fMod)) {
						while (fread(&memberBuffer, sizeof(Member), 1, fMod)) {
							if (strcmp(memberBuffer.ic, current_member->ic) == 0) {
								fseek(fMod, (IC_SIZE + USERNAME_SIZE + PASSWORD_SIZE + EMAIL_SIZE + GENDER_SIZE) - sizeof(Member), SEEK_CUR);
							}
						}
					}
					break;
				case 6:
					system("cls");
					break;
				default:
					printf("\n\Invalid Input! Press any key to try again...\n");
					getch();
					system("cls");
					break;
				}
				break;
			case 2:
				stringInput("Warning: Deleting Account will result in total wipe of your data.\nIf you wish to proceed, type 'confirm'\n>>>>> ", confirm, 8);
				if (strcmp((confirm), "confirm") == 0) {

					FILE* fCopy;
					FILE* fPaste;

					fCopy = fopen("memberlist.bin", "r");
					fPaste = fopen("memberlistbuffer.bin", "w");

					while (!feof(fCopy)) {
						fread(&memberBuffer, sizeof(Member), 1, fCopy);
						if (strcmp(memberBuffer.ic, current_member->ic) != 0) {
							fwrite(&memberBuffer, sizeof(Member), 1, fPaste);
						}
					};
					fclose(fCopy);
					fclose(fPaste);
					remove("memberlist.bin");

					if (rename("memberlistbuffer.bin", "memberlist.bin") == 0) {
						printf("\nAccount Successfully deleted!\nWe are sad to see you go :( ");
						getch();
						system("cls");
						backToMain = true;
					}
				}
				else {
					printf("Phrase Mismatch! Press any key to return to Profile...");
					getch();
				}
				break;
			case 3:
				backToMain = false;
				system("cls");
				break;
			default:
				printf("\n\Invalid Input! Press any key to try again...\n");
				getch();
				system("cls");
				break;
			}
		break;
		case 2:
			printf("\n\Schedule List!\n");					// Combine: zw part
		break;
		case 3:
			printf("\n\Booking Module!\n");					// Combine: zy part
		break;
		case 4:
			printf("\n\Lost and Found!\n");					// Write into: yj part
		break;
		case 5:
			backToMain = true;
			system("cls");
			break;
		default:
			printf("\nInvalid option! Press any key to Retry...\n");
			getch();
			system("cls");
		}
	}

}


	// Non-Display Functions
int randomNumGen() {
	int n, upper = 9999, lower = 1000;
	srand(time(NULL));
	n = (rand() % (upper - lower + 1)) + lower;
	return n;
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

	*c = (char)ch;
	flush(stdin);
	return 0;
}

void flush(FILE* stream) {
	int c;
	while ((c = getc(stream)) != '\n' && c != EOF);
}
