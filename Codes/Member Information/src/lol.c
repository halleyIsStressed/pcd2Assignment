#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#pragma warning (disable:4996)

typedef struct {
	char username[50], password[50], email[254];
}Member;


bool login(Member* place_to_put_member);
void registr();
void passwordRec();
int input(const char* prompt, char* buffer, int n);
void flush(FILE* stream);

void main() {
	int memberOption;
	Member current_member;

	printf("Welcome Member!\nTo Proceed to Login Page, Enter 1\nTo Register an account, Enter 2.\nIf you forgot your password, Enter 3.\n\n>>>");
	scanf("%d", &memberOption);
	flush(stdin);
	switch (memberOption) {
	case 1:
		if (login(&current_member) == true){
			printf("\n\nWelcome, %s!\n", current_member.username);
		}
		break;
	case 2:
		registr();
		break;
	case 3:
		passwordRec();
		break;
	default:
		printf("Invalid option!");
	}
}

bool login(Member* place_to_put_member) {
	char loginName[50], loginPassword[50];
	char username_buffer[50], password_buffer[50], email_buffer[254];

	FILE* fMem;

	fMem = fopen("memberlist.txt", "r");
	if (fMem == NULL) {
		printf("Error at opening File!");
		exit(1);
	}

	input("Enter your username > ", loginName, 50);
	input("Enter your password > ", loginPassword, 50);
	
	while (fscanf(fMem, "%[^:]:%[^:]:%s\n", username_buffer, password_buffer, email_buffer) == 3) {

		if (strcmp(username_buffer, loginName) == 0) {

			if (strcmp(password_buffer, loginPassword) == 0) {
				printf("Login Successful!\n");
				strcpy(place_to_put_member->username, username_buffer);
				strcpy(place_to_put_member->password, password_buffer);
				strcpy(place_to_put_member->email, email_buffer);
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


void registr() {
	char username[50], password[50], email[50];
	FILE* fMem;
	fMem = fopen("memberlist.txt", "a");
	if (fMem == NULL) {
		printf("Error at opening File!");
		exit(1);
	}

	printf("Enter your username > ");
	rewind(stdin);
	scanf("%s", &username);

	printf("Enter your password > ");
	rewind(stdin);
	scanf("%s", &password);

	printf("Enter your email > ");
	rewind(stdin);
	scanf("%s", &email);

	fprintf(fMem, "%s\,%s\,%s\n", username, password, email);

}
void passwordRec() {
	printf("Password Rec");
}


int input(const char* prompt, char* buffer, int n) {
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
		// man just pressed enter
		return 1;

	// all OK!
	return 0;
}

void flush(FILE* stream) {
	int c;
	while ((c = getc(stream)) != '\n' && c != EOF);
}