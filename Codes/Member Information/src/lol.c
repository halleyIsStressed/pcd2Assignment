#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
bool passwordRec(Member* place_to_put_member); 				// Password Recovery Function
const char* domainToIP(const char*);
int connectToServer(const char*);
void signUp(); 												// Register Function
int randomNumGen();
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
				printf("\n%s\n", current_member.gender);
				printf("\n%s\n", current_member.contact_No);
				getch();
				exit = true;
			}

			break;
		case 2:
			signUp();									// Sign Up function. Runs login() if success (not implemented yet)
			break;
		case 3:
			if (passwordRec(&current_member) == true) {	// Password Recovery function. Runs login() if success (not implemented yet)
				printf("\n\nWelcome, %s!\n", current_member.username);
				printf("\n%s\n", current_member.password);
				printf("\n%s\n", current_member.email);
				printf("\n%s\n", current_member.gender);
				printf("\n%s\n", current_member.contact_No);
				getch();
				exit = true;
			};	
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

	stringInput("Enter your username\t\t> ", new_member.username, 30);
	stringInput("Enter your password\t\t> ", new_member.password, 50);
	stringInput("Enter your email\t\t> ", new_member.email, 254);
	while (stringInput("Enter your gender (M,F)\t\t> ", new_member.gender,2) !=0) {
		printf("Dumbass do it again.\n\n");
		getch();
		system("clear");
	}
	stringInput("Enter your contact number\t> +60", new_member.contact_No, 12);

	fwrite(&new_member, sizeof(new_member), 1, fMem);
	printf("\n\nYou have successfully registered an account! Press any button to return to Main Menu...\n");
	fclose(fMem);
	getch();
	system("clear");
}

bool passwordRec(Member* place_to_put_member) {
	int recOption, code = 0, codeAns;
	char loginName[30] = {};
	bool back = false;
	Member memberBuffer;

	FILE* fMem;
	fMem = fopen("memberlist.bin", "rb");

	stringInput("\nEnter your username > ", loginName, 30);
	while (fread(&memberBuffer, sizeof(Member), 1, fMem)) {	
		if (strcmp(memberBuffer.username, loginName) == 0) {
			while (!back) {

				printf("%s", memberBuffer);
				*place_to_put_member = memberBuffer;
				code = randomNumGen();
				printf("%d", code);
				printf("\nChoose Password Recovery Method.\n");
				printf("1. Email Verification\n");
				printf("2. Phone SMS\n");
				printf("3. Back to Main Menu\n\n");
				printf(">>>>> ");
				scanf("%d", &recOption);
				flush(stdin);

				switch (recOption) {
				case 1:
					printf("\n\nA Code is sent to your Email! Please enter it here > ");
					scanf("%d", &codeAns);
					if (codeAns == code) {
						*place_to_put_member = memberBuffer;
						fclose(fMem);
						return true;	
					}
					break;
				case 2:
					break;
				case 3:
					back = true;
					system("clear");
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
		system("clear");
	}
	
}

int randomNumGen() {
	int n, upper = 9999, lower=1000;
	srand(time(NULL));
	n = (rand() % (upper-lower+1)) + lower;
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

    *c = (char) ch;
	flush(stdin);
    return 0;
}

void flush(FILE* stream) {
	int c;
	while ((c = getc(stream)) != '\n' && c != EOF);
}

int connectToServer(const char* server_address) {
	int socket_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(25);
	if (inet_pton(AF_INET,domainToTP(server_address),addr.sin_addr) == 1) {
		connect(socket_fd, &addr, sizeof(addr));
	}
	return socket_fd;
}

const char* domainToIP(const char* target_domain) {
	const char* target_ip;
	struct in_addr *host_address;
	struct hostent *raw_list = gethostbyname(target_domain);
	for(int i = 0 ; raw_list->h_addr_list[i]!=0 ; i++) {
		host_address = raw_list->h_addr_list[i];
		target_ip = inet_ntoa(host_address);
	}
	return target_ip;
} 