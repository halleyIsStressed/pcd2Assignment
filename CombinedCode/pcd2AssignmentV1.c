#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <curl/curl.h>
#include <Windows.h> 
#include <conio.h>

// Defining size for member/staff structure
#define IC_SIZE 13
#define USERNAME_SIZE 30
#define PASSWORD_SIZE 50
#define EMAIL_SIZE 254
#define GENDER_SIZE 2
#define CONTACT_SIZE 12
#define ADDR_SIZE 50
#define POS_SIZE 20
#define ID_SIZE 6

typedef struct {
	char ic[IC_SIZE], username[USERNAME_SIZE], password[PASSWORD_SIZE],
		email[EMAIL_SIZE], gender[GENDER_SIZE], contact_No[CONTACT_SIZE];
}Member;

struct StaffInformation {
	char name[USERNAME_SIZE], address[ADDR_SIZE], phoneNumber[CONTACT_SIZE],
		email[EMAIL_SIZE], position[POS_SIZE];
};

struct Duty {
	char week[7][20];
	char work[7][5][70];  // Assuming you need 4 work slots per day, with up to 70 characters each
};

typedef struct {
	char staffID[ID_SIZE];
	char staffPassword[PASSWORD_SIZE];
	struct StaffInformation information;
	struct Duty dutyScadule;
}Staff;

// Defining size for train structure
#define TRAINS 10
#define STATION 50
#define TIME 10
#define STATUS 20
#define FEEDBACK 100
#define MAINTENANCE 100

#define MAX_STAFF_INFORMATION 10

struct DATE {									
	int day, month, year;						
};

struct Time {									
	int hour, min;								
};

typedef struct {								 // Define a custom data type named TICKET using typedef.
	char ticketID[10], name[30], coach, status[30], departStation[30], arrivStation[30];
	struct DATE bookDate, departDate;			//Holds the booking date, and departure date using the DATE struct
	struct Time time;							// Holds the time using the TIME struct.
	int seatNo;
	double amount;
}TICKET;

typedef struct {
	char fnbName[30];
	double price;
}FNB;

typedef struct {
	int trainID;
	char departureStation[STATION];
	int departurePlatform;
	char departureTime[TIME];
	char arrivalStation[STATION];
	char eta[TIME];
	double ticketPrice;
	int availableSeats;
	char trainStatus[STATUS];
}Train;

typedef struct {
	char maintenance[MAINTENANCE];
	char feedback[FEEDBACK];
}MainFeed;

typedef struct {
	char reporter[USERNAME_SIZE], type[20], colour[10], location[STATION];
}LostItem;

struct upload_status {
	char content[1024];
	size_t bytes_read;
};

// Password recovery function: Defining Format
#define FROM_MAIL "lee.lapyhin0127@gmail.com"	// This is the email we will be using for the password recovery function.
#define APP_PASSWORD ""							// This is the App Password for the email above. Will be empty in github and during code inspection.
#define PAYLOAD_TEXT                                                 \
        "From: <" FROM_MAIL ">\r\n"                                  \
        "To: <%s>\r\n"                                               \
        "Subject: Train Ticketing Password Recovery\r\n"			 \
        "\r\n"                                                       \
        "Your recovery code is %04d.\r\n"                            \
        "\r\n"                                                       \
        "If you haven't made this request, please ignore this message.\r\n"

#define DEBUG 0

// Function Declarations: MEMBER MODULE (Implementation 100%)
void userMain();
bool login(Member* place_to_put_member);
bool memberMenu(Member*);
bool memberModify(Member* current_member, char* newData, int dataSize, int offset);
bool passwordRec(Member* place_to_put_member); 				// Password Recovery Function
void signUp();
int randomNumGen();
int stringInput(const char* prompt, char* buffer, int n); 	// Used as a one-liner for prompting and receiving input for string
char charInput(const char* prompt);							// Used as a one-liner for prompting and receiving input for char
void flush(FILE* stream); 									// Flushes overflowing data
int sendMail(char* to_mail, int code);						// Extra Feature: Sending Email
void memberLostAndFound(Member* current_member);			// Extra Feature: Lost and Found
void lnfReport(Member* current_member);
void lnfSearch();
void sortedDisplay(char*);

// Function Declarations: BOOKING MODULE (Implementation 60%)
int readTicketFile(TICKET ticket[]);
void bookingMain();
void bookingMenu(TICKET ticket[], int* numOfTicket, FNB fnb[], int* numOfItem);
void addBooking(TICKET ticket[], int* numOfTicket);
void searchBooking(TICKET ticket[], int* numOfTicket);
void editBooking(TICKET ticket[], int* numOfTicket);
void displayBooking(TICKET ticket[], int* numOfTicket);
void deleteBooking(TICKET ticket[], int* numOfTicket);
void fnbFunction(FNB fnb[], int* numOfItem);
void paymentFunction();
void exitFunction(TICKET ticket[], int* numOfTicket);


// Function Declarations: STAFF MODULE (Implementation 90%)

void staffMain();
bool staffLogin(Staff* place_to_put_staff);
void staffSignUp();
void staffMenu(Staff* staffInformation);
void clearInputBuffer();
bool staffModify();
void duty(Staff dutyShow);
bool applyResign();
bool leaveFunction(Staff applyLeave);
void searchStaff();
 

// Function Declarations: SCHEDULING MODULE (Implementation 100%)
void scheduleMain();
void addTrain();
void modifyTrain();
void addMainFeed();
void displayMainFeed();
void searchTrain();
void displayTrainList();


// Decorative Functions:
void decorationFlower();
void title();
void decoration();
void line();


// User Interface Functions

// Main Menus
void main() {
	int option;
	char abort;
	do {
		title();
		printf("Are you a Staff or a Member?\n");
		printf("1. Staff\n");
		printf("2. Member\n");
		printf("3. Fuck off\n\n");
		printf(">>>>> ");
		scanf("%d", &option);
		clearInputBuffer();
		switch (option) {
		case 1:
			system("cls");
			staffMain();
			break;
		case 2:
			system("cls");
			userMain();
			break;
		case 3:
			printf("\nSee you!\n");
			getch();
			break;
		default:
			printf("Invalid Input! Try again...");
			getch();
			system("cls");
			break;
		}
		/*do {
			printf("If you want to exit the system, please enter 'X'.\nIf you want to go back to the main page, please enter 'R'.\n");
			printf("Please select >> ");
			scanf("%c", &abort);
			clearInputBuffer();
			system("cls");
		} while (toupper(abort) != 'X' && toupper(abort) != 'R');



		} while (toupper(abort) != 'X');

		return 0;*/
	} while (option!= 3);
}

void staffMain() {
	bool exit = false;
	Staff current_staff;
	while (!exit) {
		int option = 0;

		title();
		printf("Welcome Staff!\n\n");
		printf("Choose your Desired Mode.\n");
		printf("1. Login\n");
		printf("2. Sign Up\n");
		printf("3. Exit\n\n");
		printf(">>>>> ");
		scanf("%d", &option);
		flush(stdin);

		switch (option) {
		case 1:
			if (staffLogin(&current_staff) == true) {		// Login function. If login is a success (receives value 'true'), run everything below.
				printf("Welcome, %s!\n\n", current_staff.information.name);
				staffMenu(&current_staff);
			}
			break;
		case 2:
			system("cls");
			staffSignUp();
			break;
		case 3:
			system("cls");
			return -1;
			break;
		default:
			printf("\nInvalid option! Press any key to Retry...\n");
			getch();
			system("cls");
			title();
		}
	}
}

void userMain() {										// Main Menu. Branches off into Login, Sign Up, Password Recovery, Close Program.
	bool exit = false;
	Member current_member;
	while (!exit) {
		int loginOption = 0;

		title();
		printf("Welcome User!\n\n");
		printf("Choose your Desired Mode.\n");
		printf("1. Login\n");
		printf("2. Sign Up\n");
		printf("3. I forgor my password :skull:\n");
		printf("4. Exit\n\n");
		printf(">>>>> ");
		scanf("%d", &loginOption);

		switch (loginOption) {
		case 1:
			if (login(&current_member) == true) {		// Login function. If login is a success (receives value 'true'), run everything below.
				memberMenu(&current_member);
			}
			break;
		case 2:
			signUp();
			break;
		case 3:
			if (passwordRec(&current_member) == true) {	// Password Recovery function. Runs login() if success (not implemented yet)
				printf("\n\nWelcome, %s!\n", current_member.username);
				memberMenu(&current_member);
			}
			break;
		case 4:
			system("cls");
			return -1;
			break;
		default:
			printf("\nInvalid option! Press any key to Retry...\n");
			getch();
			system("cls");
		}
	}
}


// Pre-Login Functions
bool login(Member* place_to_put_member) {
	char loginName[USERNAME_SIZE], loginPassword[PASSWORD_SIZE];
	FILE* fMem;
	Member memberBuffer;			// Temporary spot to store the structer read from .bin file. Used to compare with user input.

	fMem = fopen("memberlist.bin", "rb");
	if (fMem == NULL) {
		printf("Error at opening File!");
		exit(1);
	}
	flush(stdin);
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
	flush(stdin);
	stringInput("Enter username\t\t> ", new_member.username, USERNAME_SIZE);
	stringInput("Enter password\t\t> ", new_member.password, PASSWORD_SIZE);
	stringInput("Enter ic (without '-')\t> ", new_member.ic, IC_SIZE);
	stringInput("Enter email\t\t> ", new_member.email, EMAIL_SIZE);
	while (!(stringInput("Enter gender (M,F)\t> ", new_member.gender, GENDER_SIZE) == 0 && (new_member.gender[0] == 'M' || new_member.gender[0] == 'F'))) {
		printf("Dumbass do it again.\n\n");
		getch();
		system("cls");
	}
	stringInput("Enter contact number\t> +60", new_member.contact_No, CONTACT_SIZE);

	fwrite(&new_member, sizeof(new_member), 1, fMem);
	printf("\n\Account succesfully registered! Press any button to return to Main Menu...\n");
	fclose(fMem);
	getch();
	system("cls");
}

bool staffLogin(Staff* place_to_put_staff) {
	char loginID[10], loginPassword[PASSWORD_SIZE];
	FILE* fStaff;
	Staff staffBuffer;			// Temporary spot to store the structer read from .bin file. Used to compare with user input.

	fStaff = fopen("Staff.bin", "rb");
	if (fStaff == NULL) {
		printf("Error at opening File!");
		exit(1);
	}

	stringInput("Enter your staff ID > ", loginID, 10);
	stringInput("Enter your password > ", loginPassword, PASSWORD_SIZE);

	while (fread(&staffBuffer, sizeof(Staff), 1, fStaff)) {				// Looping to read each structure variable saved into Staff.bin.

		if (strcmp(staffBuffer.staffID, loginID) == 0) {			// If the program finds a match on the username....

			if (strcmp(staffBuffer.staffPassword, loginPassword) == 0) {	// ...and If it finds that the password also matches...
				system("cls");
				printf("Login Successful!\n");
				*place_to_put_staff = staffBuffer;					// it will pass the values of the temporary storage (user input) to the the struct that stores the member currently logged in.
				fclose(fStaff);
				return true;											// and it will return the boolean value 'true'.
			}
			else {
				printf("Wrong Password!");								// If the password doesn't match, restart.
				printf(" Press any key to try again...\n");
				getch();
				system("cls");
				title();
				break;
			}
		}
	}
	if (strcmp(staffBuffer.staffID, loginID) != 0) {
		printf("Username Not Found! Press any key to return to Main Menu...\n");
		getch();
		system("cls");
	}
	fclose(fStaff);
	return false;
}

void staffSignUp() {
	FILE* signUP = fopen("Staff.bin", "ab");  // Append mode is used to keep adding new records.
	if (signUP == NULL) {
		perror("Error opening file");
		return;
	}
	int position;
	Staff staffInformation;
	title();
	decorationFlower();
	printf("");
	stringInput("New login ID > ", staffInformation.staffID, ID_SIZE);
	stringInput("New password > ", staffInformation.staffPassword, PASSWORD_SIZE);
	printf("\n");
	decorationFlower();
	stringInput("New username > ", staffInformation.information.name, USERNAME_SIZE);
	stringInput("New email    > ", staffInformation.information.email, EMAIL_SIZE);
	stringInput("New contact  > +60", staffInformation.information.phoneNumber, CONTACT_SIZE);
	stringInput("Current Addr > ", staffInformation.information.address, ADDR_SIZE);
	printf("-------------------------------------------------------------\n");
	printf("\n|Position: |1) ENGINEER\t|2) COUNTER ATTENDANT\t| 3)MANAGER|\n");
	printf("-------------------------------------------------------------\n");
	printf("\n%s", "Please select >> ");
	scanf(" %d", &position);
	clearInputBuffer();
	switch (position)
	{
	case 1: {
		strcpy(staffInformation.information.position, "ENGINEER");
		break;
	}
	case 2: {
		strcpy(staffInformation.information.position, "COUNTER ATTENDANT");
		break;
	}
	case 3: {
		strcpy(staffInformation.information.position, "MANAGER");
		break;
	}
	default:
		break;
	}

	// Writes the staff information to the file.
	const char* daysOfWeek[7] = { "SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY" };

	if (strcmp(staffInformation.information.position, "ENGINEER") == 0) {
		// Task descriptions for weekdays
		const char* weekTasks[5][5] = {
	  {"Check Train A", "Safety Inspection", "Equipment Checks", "Paperwork", "Control Systems Check"},
	  {"Check Train B", "Maintenance Engine", "Breaks", "Signal Testing", "Hydraulic Systems Check"},
	  {"Check Train C", "Review Train Schedules", "Breaks", "Business Premises", "Operational Review"},
	  {"Check Train D", "Emergency Drills", "Breaks", "Business Premises", "Safety Protocol Review"},
	  {"Check Train E", "Project Meeting", "Breaks", "Train Cleaning", "Budget Review"}
		};

		const char* restDay = "Rest Day";

		for (int day = 0; day < 7; day++) {
			// Set the day of the week
			strcpy(staffInformation.dutyScadule.week[day], daysOfWeek[day]);

			const char* restDay = "Rest Day";

			// For weekends
			if (day == 0 || day == 6) {
				for (int task = 0; task < 5; task++) {
					strcpy(staffInformation.dutyScadule.work[day][task], restDay);
				}
			}
			else {
				// For weekdays, fetch tasks from the weekTasks array
				int taskIndex = day - 1;  // Adjust for array index starting at 0
				for (int task = 0; task < 5; task++) {
					strcpy(staffInformation.dutyScadule.work[day][task], weekTasks[taskIndex][task]);
				}
			}
		}
	}

	else if (strcmp(staffInformation.information.position, "COUNTER ATTENDANT") == 0) {

		const char* weekTasks[5][5] = {
	 {"Ticket Checking", "Information Desk", "Lunch Break", "Schedule Coordination", "Ticket Validation"},
	 {"Platform Management", "Announcements", "Lunch Break", "Customer Service", "Train Dispatching"},
	 {"Ticket Selling", "Luggage Assistance", "Lunch Break", "Lost and Found", "Baggage Check"},
	 {"Sanitation Check", "Break", "Lunch Break", "Customer Assistance", "Platform Inspection"},
	 {"Equipment Maintenance", "Security Check", "Lunch Break", "Reporting", "Safety Compliance"}
		};

		const char* restDay = "Rest Day";

		for (int day = 0; day < 7; day++) {
			// Set the day of the week
			strcpy(staffInformation.dutyScadule.week[day], daysOfWeek[day]);

			const char* restDay = "Rest Day";

			// For weekends
			if (day == 0 || day == 6) {
				for (int task = 0; task < 5; task++) {
					strcpy(staffInformation.dutyScadule.work[day][task], restDay);
				}
			}
			else {
				// For weekdays, fetch tasks from the weekTasks array
				int taskIndex = day - 1;  // Adjust for array index starting at 0
				for (int task = 0; task < 5; task++) {
					strcpy(staffInformation.dutyScadule.work[day][task], weekTasks[taskIndex][task]);
				}
			}
		}
	}

	else if (strcmp(staffInformation.information.position, "MANAGER") == 0) {
		// Define tasks for a MANAGER for 5 workdays
		const char* weekTasks[5][5] = {
	{"Team Meeting", "Project Review", "Lunch Break", "Client Call", "Documentation"},
	{"Budget Planning", "Team 1-on-1s", "Lunch Break", "Strategy Session", "Review Reports"},
	{"Report Analysis", "Staff Training", "Lunch Break", "Market Research", "Compliance Check"},
	{"Operational Review", "Problem Solving", "Lunch Break", "Networking Event", "Client Feedback"},
	{"Business Development", "Email Correspondence", "Lunch Break", "Plan Next Week", "Market Analysis"}
		};

		const char* restDay = "\t Rest Day \t";
		for (int day = 0; day < 7; day++) {
			// Set the day of the week
			strcpy(staffInformation.dutyScadule.week[day], daysOfWeek[day]);

			const char* restDay = "Rest Day";

			// For weekends
			if (day == 0 || day == 6) {
				for (int task = 0; task < 5; task++) {
					strcpy(staffInformation.dutyScadule.work[day][task], restDay);
				}
			}
			else {
				// For weekdays, fetch tasks from the weekTasks array
				int taskIndex = day - 1;  // Adjust for array index starting at 0
				for (int task = 0; task < 5; task++) {
					strcpy(staffInformation.dutyScadule.work[day][task], weekTasks[taskIndex][task]);
				}
			}
		}
	}
	fwrite(&staffInformation, sizeof(Staff), 1, signUP);
	fclose(signUP);
	printf("Sign Up Complete! Press any key to return to Menu...");
	getch();
	system("cls");
}

bool passwordRec(Member* place_to_put_member) {
	int recOption, code = 0, codeAns;
	char loginName[USERNAME_SIZE] = { NULL };
	bool back = false;
	Member memberBuffer;
	FILE* fMem;
	fMem = fopen("memberlist.bin", "rb");
	system("cls");

	title();
	stringInput("\nEnter your username > ", loginName, USERNAME_SIZE);
	while (fread(&memberBuffer, sizeof(Member), 1, fMem)) {
		if (strcmp(memberBuffer.username, loginName) == 0) {
			while (!back) {
				printf("\nSending Email...");
				*place_to_put_member = memberBuffer;
				code = randomNumGen();
				sendMail(memberBuffer.email, code);
				printf("\n\nA Code is sent to your Email! Please enter it here > ");
				scanf("%d", &codeAns);
				flush(stdin);
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
	fclose(fMem);
	if (back == false) {
		printf("Username Not Found! Press any key to return to Main Menu...");
		getch();
		system("cls");
	}
}


// Post-Login Display and Functions: MEMBER MODULE
bool memberMenu(Member* current_member) {
	char confirm[8];
	bool backToMain = false;
	Member memberBuffer;			// Temporary spot to store the structer read from .bin file. Used to compare with user input.


	while (backToMain == false) {
		int memberOption = 0, profileOption = 0, offset;
		printf("Welcome, %s!\n\n", current_member->username);
		printf("Choose Function.\n");
		printf("1. View Profile Details\n");
		printf("2. Display Train Schedule List\n");
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
				printf("Choose Field to Edit.\n");
				printf("1. Username\n");
				printf("2. Password\n");
				printf("3. Email\n");
				printf("4. Gender\n");
				printf("5. Contact Number\n");
				printf("6. Return to Member Interface\n\n");
				printf(">>>>> ");
				int modifyOption;
				char newIc[IC_SIZE], newUser[USERNAME_SIZE], newPassword[PASSWORD_SIZE],
					newEmail[EMAIL_SIZE], newGender[GENDER_SIZE], newContact[CONTACT_SIZE];

				scanf("%d", &modifyOption);
				flush(stdin);
				switch (modifyOption) {
				case 1:
					stringInput("\nEnter new Username > ", newUser, USERNAME_SIZE);
					offset = IC_SIZE - sizeof(Member);
					if (memberModify(current_member, newUser, USERNAME_SIZE, offset) == true);

					backToMain = true;
					break;
				case 2:
					stringInput("\nEnter new Password > ", newPassword, PASSWORD_SIZE);
					offset = (IC_SIZE + USERNAME_SIZE) - sizeof(Member);
					memberModify(current_member, newPassword, PASSWORD_SIZE, offset);
					backToMain = true;
					break;
				case 3:
					stringInput("\nEnter new Email > ", newEmail, EMAIL_SIZE);
					offset = (IC_SIZE + USERNAME_SIZE + PASSWORD_SIZE) - sizeof(Member);
					memberModify(current_member, newEmail, PASSWORD_SIZE, offset);
					backToMain = true;
					break;
				case 4:
					while (!(stringInput("Enter new Gender (M,F)\t\t> ", newGender, GENDER_SIZE) == 0 && (newGender[0] == 'M' || newGender[0] == 'F'))) {
						printf("Invalid Input.\n\n");
						getch();
						system("cls");
					}
					offset = (IC_SIZE + USERNAME_SIZE + PASSWORD_SIZE + EMAIL_SIZE) - sizeof(Member);
					memberModify(current_member, newGender, GENDER_SIZE, offset);
					backToMain = true;
					break;
				case 5:
					stringInput("\nEnter new phone number > +60", newContact, CONTACT_SIZE);
					offset = (IC_SIZE + USERNAME_SIZE + PASSWORD_SIZE + EMAIL_SIZE + GENDER_SIZE) - sizeof(Member);
					memberModify(current_member, newContact, CONTACT_SIZE, offset);
					backToMain = true;
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
				stringInput("Warning: Deleting Account will result in total wipe of your data.\nIf you wish to proceed, enter your password\n>>>>> ", confirm, 9);
				if (strcmp(confirm, current_member->password) == 0) {

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
			displayTrainList();
			break;
		case 3:
			bookingMain();
			break;
		case 4:
			memberLostAndFound(current_member);
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

bool memberModify(Member* current_member, char* newData, int dataSize, int offset) {
	FILE* fMod = fopen("memberlist.bin", "rb+");
	Member memberBuffer;


	if (fMod == NULL) {
		printf("Error at opening File!");
		exit(1);
	}
	while (!feof(fMod)) {
		fread(&memberBuffer, sizeof(Member), 1, fMod);
		if (strcmp(memberBuffer.ic, current_member->ic) == 0) {
			fseek(fMod, offset, SEEK_CUR);
			fwrite(newData, sizeof(char), dataSize, fMod);
			break;

		}
	}
	fclose(fMod);

	printf("Success! Please re-login...");
	getch();
	system("cls");
	return true;
}

void memberLostAndFound(Member* current_member) {
	bool backToMemberMenu = false;
	while (backToMemberMenu == false) {
		int lnfOption = 0;
		printf("\nChoose Mode.\n");
		printf("1. Report Missing Item\n");
		printf("2. Search Missing Item\n");
		printf("3. Return to Member Interface\n");
		printf("\n>>>>> ");
		scanf("%d", &lnfOption);
		flush(stdin);
		switch (lnfOption) {
		case 1:
			lnfReport(current_member);
			break;
		case 2:
			lnfSearch();
			break;
		case 3:
			backToMemberMenu = true;
			break;
		default:
			printf("\nInvalid Input. Press any key to try again.");
			getch();
			system("cls");

		}
	}
	system("cls");
}

void lnfReport(Member* current_member) {
	FILE* fLost;
	fLost = fopen("lostfound.bin", "ab");
	if (fLost == NULL) {
		printf("Error at opening File!");
		exit(1);
	}
	LostItem item;
	strcpy(&item.reporter, current_member->username);
	int loopa = 1;
	stringInput("\nEnter the type of the item (Phone, Bottle, etc) > ", &item.type, 20);
	stringInput("Enter the colour of the item (Blue, White, etc) > ", &item.colour, 10);
	stringInput("Enter the station which this item was lost      > ", &item.location, STATION);

	fwrite(&item, sizeof(item), 1, fLost);
	printf("\nReport Uploaded! Press any key to return...\n");
	getch();
	fclose(fLost);
	system("cls");
}

void lnfSearch() {
	bool backToMemberMenu = false;
	char itemType[20], itemColour[10], itemLocation[STATION];
	

	while (backToMemberMenu == false) {
		int searchOption = 0;
		printf("\nChoose 'search by' method.\n");
		printf("1. Type\n");
		printf("2. Colour\n");
		printf("3. Station\n");
		printf("4. Return to Member Interface\n");
		printf("\n>>>>> ");
		scanf("%d", &searchOption);
		flush(stdin);
		switch (searchOption) {
		case 1:
			stringInput("\nEnter the type of the item (Phone, Bottle, etc) > ", &itemType, 20);
			sortedDisplay(itemType);
			break;
		case 2:
			stringInput("\nEnter the colour of the item (Blue, White, etc) > ", &itemColour, 10);
			sortedDisplay(itemColour);
			break;
		case 3:
			stringInput("\nEnter the station which this item was lost      > ", &itemLocation, STATION);
			sortedDisplay(itemLocation);
			break;
		case 4:
			backToMemberMenu = true;
			system("cls");
			break;
		default:
			printf("\nInvalid Input. Press any key to try again.");
			getch();
			system("cls");
		}
	}

}

void sortedDisplay(char* sorter) {
	FILE* fLost = fopen("lostfound.bin", "rb");
	if (fLost == NULL) {
		printf("Error at opening File!");
		exit(1);
	}
	int count = 0;
	LostItem itemBuffer;
	printf("Lost Item List\n");
	printf("===================\n\n");
	printf("%-20s | %-10s | %-50s | %-30s \n", "Type", "Colour", "Lost on Station", "Reporter");
	printf("-----------------------------------------------------------------------------------------------------------------\n");

	while (fread(&itemBuffer, sizeof(LostItem), 1, fLost)) {				// Looping to read each structure variable saved into lostfound.bin.
		bool printItem = false;

		if (strcmp(itemBuffer.type, sorter) == 0) {
			printItem = true;
		}
		else if (strcmp(itemBuffer.colour, sorter) == 0) {
			printItem = true;
		}
		else if (strcmp(itemBuffer.location, sorter) == 0) {
			printItem = true;
		}

		if (printItem == true){
			count++;
			printf("%-20s | %-10s | %-50s | %-30s \n", itemBuffer.type, itemBuffer.colour, itemBuffer.location, itemBuffer.reporter);
		}
	}

	printf("\n%d item(s) displayed.\nIf any item(s) of interest was found, please report to the Customer Service counter on any Station.\n", count);
	getch();
	system("cls");
	fclose(fLost);
}

void bookingMain() {
	system("cls");											// Clear the console screen.						
	TICKET ticket[100];										// Declare an array of TICKET structs to store ticket information.
	int numOfTicket = readTicketFile(ticket);				// Call the readTicketFile function to read ticket information from a file. Store the number of tickets read into numOfTicket.
	FNB fnb[10];
	int numOfItem = 0;
	bookingMenu(ticket, &numOfTicket, fnb, &numOfItem);  // Call the functionsChoosen function to handle user input and perform actions accordingly.
}

void bookingMenu(TICKET ticket[], int* numOfTicket, FNB fnb[], int* numOfItem) {				// Function to handle various ticket booking operations based on user input. Using parameters to pass value.
	system("cls");
	int choice;

	do {
		printf("============================================\n");
		printf("\t      TICKET BOOKING\n");
		printf("============================================\n\n");
		printf("1. Add Ticket Booking(s)\n");
		printf("2. Search Ticket Booking(s)\n");
		printf("3. Edit Ticket Booking(s)\n");
		printf("4. Cancel Ticket Booking(s)\n");
		printf("5. Display Ticket Booking(s)\n");
		printf("6. F&B\n");
		printf("7. Payment\n");
		printf("8. Exit TICKET BOOKING\n\n");
		printf("Please choose from 1-8: ");
		scanf("%d", &choice);
		rewind(stdin);
		switch (choice) {
		case 1: addBooking(ticket, numOfTicket); break;
		case 2: searchBooking(ticket, numOfTicket); break;
		case 3: editBooking(ticket, numOfTicket); break;
		case 4: deleteBooking(ticket, numOfTicket); break;
		case 5: displayBooking(ticket, numOfTicket); break;
		case 6: fnbFunction(fnb, numOfItem); break;
		case 7: paymentFunction(); break;
		case 8: exitFunction(ticket, numOfTicket); break;
		default: printf("Invalid choice! Please key in again.\n\n");
		};
	} while (choice != 8);					// Continue the loop until the user key in number 8 to exit.
}

void addBooking(TICKET ticket[], int* numOfTicket) {  //void addBooking(FILE *ticketFile)
	system("cls");
	displayTrainList();
	char confirmAdd, cont;
	SYSTEMTIME t; //structure for date, time etc
	printf("\n\n============================================\n");
	printf("\t    Add Ticket Booking\n");
	printf("============================================\n\n");
	do {
		printf("\nEnter Ticket Booking Details: \n");
		printf(" Ticket Holder Name: ");
		scanf("%[^\n]", &ticket[*numOfTicket].name);
		printf(" Deparutre Date: \n");
		printf("\t     dd: ");
		scanf("%d", &ticket[*numOfTicket].departDate.day);
		while (ticket[*numOfTicket].departDate.day <= 0 || ticket[*numOfTicket].departDate.day >= 32) {
			printf("Invalid day! Please enter again.\n");
			printf("\t     dd: ");
			scanf("%d", &ticket[*numOfTicket].departDate.day);
		}
		rewind(stdin);
		printf("\t     mm: ");
		scanf("%d", &ticket[*numOfTicket].departDate.month);
		while (ticket[*numOfTicket].departDate.month <= 0 || ticket[*numOfTicket].departDate.month >= 13) {
			printf("Invalid month! Please enter again.\n");
			printf("\t     mm: ");
			scanf("%d", &ticket[*numOfTicket].departDate.month);
		}
		rewind(stdin);
		printf("\t   yyyy: ");
		scanf("%d", &ticket[*numOfTicket].departDate.year);
		while (ticket[*numOfTicket].departDate.year < 2024 || ticket[*numOfTicket].departDate.year > 2025) {
			printf("Booking only within 2024 - 2025.\n");
			printf("\t     yy: ");
			scanf("%d", &ticket[*numOfTicket].departDate.year);
		}
		rewind(stdin);
		printf(" Departure Station: ");
		scanf("%[^\n]", &ticket[*numOfTicket].departStation);
		rewind(stdin);
		printf(" Arrival Station: ");
		scanf("%[^\n]", &ticket[*numOfTicket].arrivStation);
		rewind(stdin);
		printf(" Coach (A-J): ");
		scanf("%c", &ticket[*numOfTicket].coach);
		ticket[*numOfTicket].coach = toupper(ticket[*numOfTicket].coach);
		while (ticket[*numOfTicket].coach >= 'K') {
			printf("Invalid Coach! Please enter a Coach between A-J.\n");
			printf(" Coach (A-J): ");
			scanf("%c", &ticket[*numOfTicket].coach);
		}
		rewind(stdin);
		printf(" Seat No (1-30): ");
		scanf("%d", &ticket[*numOfTicket].seatNo);
		while (ticket[*numOfTicket].seatNo <= 0 || ticket[*numOfTicket].seatNo > 30) {
			printf("Invalid Seat No! Please enter a Seat No between 1-30.\n");
			printf(" Seat No (1-30): ");
			scanf("%d", &ticket[*numOfTicket].seatNo);
		}
		rewind(stdin);

		printf("Confirm to make the booking? (Y = Yes): ");
		scanf("%c", &confirmAdd);
		rewind(stdin);
		if (toupper(confirmAdd) == 'Y') {
			GetLocalTime(&t); //get real date/time from OS
			ticket[*numOfTicket].time.hour = t.wHour;
			ticket[*numOfTicket].time.min = t.wMinute;
			ticket[*numOfTicket].bookDate.day = t.wDay;
			ticket[*numOfTicket].bookDate.month = t.wMonth;
			ticket[*numOfTicket].bookDate.year = t.wYear;

			sprintf(ticket[*numOfTicket].ticketID, "B%04d", (*numOfTicket) + 1);
			strcpy(ticket[*numOfTicket].status, "Pending payment...");
			printf("\n\n======================================================\n");
			printf("Ticket Summary: \n");
			printf("\n\tBooking ID: \t     %s\n\tTicket Holder Name:  %s\n\tBooking Date: \t     %02d/%02d/%04d\n\tBooking Time: \t     %02d:%02d\n\tDeparture Date:      %02d/%02d/%04d\n\tDeparture Station:   %s\n\tArrival Station:     %s\n\tCoach: \t\t     %c\n\tSeat No: \t     %d\n\tStatus: \t     %s\n",
				ticket[*numOfTicket].ticketID, ticket[*numOfTicket].name, ticket[*numOfTicket].bookDate.day, ticket[*numOfTicket].bookDate.month, ticket[*numOfTicket].bookDate.year, ticket[*numOfTicket].time.hour, ticket[*numOfTicket].time.min,
				ticket[*numOfTicket].departDate.day, ticket[*numOfTicket].departDate.month, ticket[*numOfTicket].departDate.year, ticket[*numOfTicket].departStation, ticket[*numOfTicket].arrivStation,
				ticket[*numOfTicket].coach, ticket[*numOfTicket].seatNo, ticket[*numOfTicket].status);
			(*numOfTicket)++;
			printf("======================================================\n");
		}
		else
			printf("Ticket NOT booked!\n\n");
		printf("\n\nMake another booking? (Y = Yes): ");
		scanf("%c", &cont);
		rewind(stdin);
	} while (toupper(cont) == 'Y');

	system("cls");
}

void searchBooking(TICKET ticket[], int* numOfTicket) {
	system("cls");
	int i, searchMonth, searchYear, count;
	char cont, found;
	printf("============================================\n");
	printf("\t    Search Ticket Booking\n");
	printf("============================================\n\n");
	do {
		count = 0;
		found = 0;
		printf("Search for the Departure Date of ticket(s) in which Month and Year?\n");
		printf("Month: ");
		scanf("%d", &searchMonth);
		while (searchMonth <= 0 || searchMonth >= 13) {
			printf("Invalid month! Please enter again.\n");
			printf("Month: ");
			scanf("%d", &searchMonth);
		}
		rewind(stdin);
		printf("Year:  ");
		scanf("%d", &searchYear);
		while (searchYear < 2024 || searchYear > 2025) {
			printf("Booking only available within 2024 - 2025.\n");
			printf("Year:  ");
			scanf("%d", &searchYear);
		}
		rewind(stdin);
		printf("\n%-10s %-20s %-15s %-15s %-15s %-6s %-8s\n", "Ticket ID", "Ticket Holder Name", "Departure Date", "Departure Station", "Arrival Station", "Coach", "Seat No");
		printf("%-10s %-20s %-15s %-15s %-15s %-6s %-8s\n", "=========", "==================", "==============", "=================", "===============", "=====", "=======");
		for (i = 0; i < *numOfTicket; i++) {
			if (searchMonth == ticket[i].departDate.month && searchYear == ticket[i].departDate.year) {
				found = 1;
				printf("%-10s %-20s %02d/%02d/%04d %18s %12s %10c %7d\n",
					ticket[i].ticketID, ticket[i].name, ticket[i].departDate.day, ticket[i].departDate.month, ticket[i].departDate.year, ticket[i].departStation, ticket[i].arrivStation,
					ticket[i].coach, ticket[i].seatNo);
				count++;
			}
		}
		if (!found) {
			printf("- -Departure Date NOT Found!- -\n\n");
		}
		printf("\n\t%d ticket(s) booked for %02d/%04d\n\n", count, searchMonth, searchYear);
		printf("Search another Departure Date? (Y = Yes): ");
		scanf("%c", &cont);
		rewind(stdin);
	} while (toupper(cont) == 'Y');

	system("cls");
}

void editBooking(TICKET ticket[], int* numOfTicket) {
	system("cls");
	int i, editIndex;
	char editID[10], confirmUpd, cont;
	TICKET temp;
	printf("============================================\n");
	printf("\t    Edit Ticket Booking\n");
	printf("============================================\n\n");
	do {
		editIndex = -1;
		printf("\nEnter Booking ID to edit: ");
		scanf("%s", &editID);
		rewind(stdin);
		i = 0;
		for (int i = 0; i < *numOfTicket; i++) {
			if (strcmp(editID, ticket[i].ticketID) == 0) {
				editIndex = i;
				break;
			}
		}
		if (editIndex == -1) {
			printf("\tError: Ticket Booking NOT Found!\n\n");
		}
		else {
			printf("\nRecord Found: \n");
			printf("\n\tBooking ID: \t     %s\n\tTicket Holder Name:  %s\n\tBooking Date: \t     %02d/%02d/%04d\n\tBooking Time: \t     %02d:%02d\n\tDeparture Date:      %02d/%02d/%04d\n\tDeparture Station:   %s\n\tArrival Station:     %s\n\tCoach: \t\t     %c\n\tSeat No: \t     %d\n\tStatus: \t     %s\n",
				ticket[editIndex].ticketID, ticket[editIndex].name, ticket[editIndex].bookDate.day, ticket[editIndex].bookDate.month, ticket[editIndex].bookDate.year, ticket[editIndex].time.hour, ticket[editIndex].time.min,
				ticket[editIndex].departDate.day, ticket[editIndex].departDate.month, ticket[editIndex].departDate.year, ticket[editIndex].departStation, ticket[editIndex].arrivStation,
				ticket[editIndex].coach, ticket[editIndex].seatNo, ticket[editIndex].status);

			printf("\n\n");
			displayTrainList();
			printf("\nEnter Updated Details: \n");
			printf(" Ticket ID: %s\n", ticket[editIndex].ticketID);
			printf(" Ticket Holder Name: ");
			scanf("%[^\n]", &temp.name);
			rewind(stdin);
			printf(" Deparutre Date: \n");
			printf("\t     dd: ");
			scanf("%d", &temp.departDate.day);
			while (temp.departDate.day <= 0 || temp.departDate.day >= 32) {
				printf("Invalid day! Please enter again.\n");
				printf("\t     dd: ");
				scanf("%d", &temp.departDate.day);
			}
			rewind(stdin);
			printf("\t     mm: ");
			scanf("%d", &temp.departDate.month);
			while (temp.departDate.month <= 0 || temp.departDate.month >= 13) {
				printf("Invalid month! Please enter again.\n");
				printf("\t     mm: ");
				scanf("%d", &temp.departDate.month);
			}
			rewind(stdin);
			printf("\t   yyyy: ");
			scanf("%d", &temp.departDate.year);
			while (temp.departDate.year < 2024 || temp.departDate.year > 2025) {
				printf("Booking only available within 2024 - 2025.\n");
				printf("\t     yy: ");
				scanf("%d", &temp.departDate.year);
			}
			rewind(stdin);
			printf(" Departure Station: ");
			scanf("%[^\n]", &temp.departStation);
			rewind(stdin);
			printf(" Arrival Station: ");
			scanf("%[^\n]", &temp.arrivStation);
			rewind(stdin);
			printf(" Coach (A-J): ");
			scanf("%c", &temp.coach);
			temp.coach = toupper(temp.coach);
			while (temp.coach >= 'K') {
				printf("Invalid Coach! Please enter a Coach between A-J.\n");
				printf(" Coach (A-J): ");
				scanf("%c", &temp.coach);
			}
			rewind(stdin);
			printf(" Seat No (1-30): ");
			scanf("%d", &temp.seatNo);
			if (temp.seatNo <= 0 || temp.seatNo > 31) {
				printf("Invalid Seat No! Please enter between 1-30.\n");
				printf(" Seat No (1-30): ");
				scanf("%d", &temp.seatNo);
			}
			rewind(stdin);

			printf("Confirm to update? (Y = Yes): ");
			scanf("%c", &confirmUpd);
			rewind(stdin);

			if (toupper(confirmUpd) == 'Y') {
				strcpy(temp.ticketID, editID);
				temp.bookDate.day = ticket[editIndex].bookDate.day;
				temp.bookDate.month = ticket[editIndex].bookDate.month;
				temp.bookDate.year = ticket[editIndex].bookDate.year;
				temp.time.hour = ticket[editIndex].time.hour;
				temp.time.min = ticket[editIndex].time.min;
				strcpy(temp.status, ticket[editIndex].status);
				ticket[editIndex] = temp;
				printf("Update Successfully!\n\n");
			}
			else
				printf("\nNo changes made.\n\n");

			printf("\nCurrent Record Details: \n");
			printf("\n\tBooking ID: \t     %s\n\tTicket Holder Name:  %s\n\tBooking Date: \t     %02d/%02d/%04d\n\tBooking Time: \t     %02d:%02d\n\tDeparture Date:      %02d/%02d/%04d\n\tDeparture Station:   %s\n\tArrival Station:     %s\n\tCoach: \t\t     %c\n\tSeat No: \t     %d\n\tStatus: \t     %s\n",
				ticket[editIndex].ticketID, ticket[editIndex].name, ticket[editIndex].bookDate.day, ticket[editIndex].bookDate.month, ticket[editIndex].bookDate.year, ticket[editIndex].time.hour, ticket[editIndex].time.min, ticket[editIndex].departDate.day, ticket[editIndex].departDate.month, ticket[editIndex].departDate.year,
				ticket[editIndex].departStation, ticket[editIndex].arrivStation, ticket[editIndex].coach, ticket[editIndex].seatNo, ticket[editIndex].status);
		}
		printf("\nEdit another ticket booking? (Y = Yes): ");
		scanf("%c", &cont);
		rewind(stdin);
		printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
	} while (toupper(cont) == 'Y');

	system("cls");
}

void displayBooking(TICKET ticket[], int* numOfTicket) {
	system("cls");
	char displayName[30];
	int found;
	char cont;
	printf("============================================\n");
	printf("\t    Display Ticket Booking\n");
	printf("============================================\n\n");
	do {
		found = 0;
		printf("Enter Ticket Holder Name to display: ");
		scanf("%[^\n]", displayName);
		rewind(stdin);
		for (int i = 0; i < *numOfTicket; i++) {
			if (strcmp(displayName, ticket[i].name) == 0) {
				found = 1;
				printf("\n\tBooking ID: \t     %s\n\tTicket Holder Name:  %s\n\tBooking Date: \t     %02d/%02d/%04d\n\tBooking Time: \t     %02d:%02d\n\tDeparture Date:      %02d/%02d/%04d\n\tDeparture Station:   %s\n\tArrival Station:     %s\n\tCoach: \t\t     %c\n\tSeat No: \t     %d\n\tStatus: \t     %s\n",
					ticket[i].ticketID, ticket[i].name, ticket[i].bookDate.day, ticket[i].bookDate.month, ticket[i].bookDate.year, ticket[i].time.hour, ticket[i].time.min, ticket[i].departDate.day, ticket[i].departDate.month, ticket[i].departDate.year,
					ticket[i].departStation, ticket[i].arrivStation, ticket[i].coach, ticket[i].seatNo, ticket[i].status);
			}
		}
		if (!found) {
			printf("- -Booking Name NOT Found!- -\n\n");
		}
		printf("\nDisplay another? (Y = Yes): ");
		scanf("%c", &cont);
		rewind(stdin);
	} while (toupper(cont) == 'Y');

	system("cls");
}

void deleteBooking(TICKET ticket[], int* numOfTicket) {
	system("cls");
	char deleteID[10], cont, confirmDlt;
	int found = 0;
	printf("============================================\n");
	printf("\t    Cancel Ticket Booking\n");
	printf("============================================\n\n");
	do {
		printf("Enter the Booking ID to cancel the booking: ");
		scanf("%s", &deleteID);
		rewind(stdin);
		for (int i = 0; i < *numOfTicket; i++) {
			if (strcmp(deleteID, ticket[i].ticketID) == 0) {
				// Display ticket details
				printf("\nBooking found:\n");
				printf("\n\tBooking ID: \t     %s", ticket[i].ticketID);
				printf("\n\tTicket Holder Name:  %s", ticket[i].name);
				printf("\n\tDeparture Date:      %02d/%02d/%04d", ticket[i].departDate.day, ticket[i].departDate.month, ticket[i].departDate.year);
				printf("\n\tDeparture Station:   %s", ticket[i].departStation);
				printf("\n\tArrival Station:     %s", ticket[i].arrivStation);
				printf("\n\tCoach: \t\t     %c", ticket[i].coach);
				printf("\n\tSeat No: \t     %d", ticket[i].seatNo);
				printf("\n\nConfirm deletion of this ticket? (Y = Yes): ");
				scanf("%c", &confirmDlt);
				rewind(stdin);
				if (toupper(confirmDlt) == 'Y') {
					for (int j = i; j < *numOfTicket - 1; j++) {  // Shift remaining tickets to fill the gap
						ticket[j] = ticket[j + 1];
					}
					(*numOfTicket)--;
					found = 1;
					printf("Ticket with ID %s deleted successfully.\n", deleteID);
				}
				else {
					found = 1;
					printf("Deletion canceled.\n");
				}
			}
		}
		if (!found) {
			printf("\nTicket with ID %s not found.\n", deleteID);
		}

		printf("\nDelete another Ticket Booking? (Y = Yes): ");
		scanf("%c", &cont);
		rewind(stdin);
	} while (toupper(cont) == 'Y');

	system("cls");
}

void fnbFunction(FNB fnb[], int* numOfItem) {
	system("cls");
	int choice, quantity, cont;
	double fnbTotal = 0;
	FILE* fnbFile;
	fnbFile = fopen("F&B.txt", "r");
	if (fnbFile == NULL) {
		printf("Can't open the file F&B.txt!\n\n");
		return;
	}
	else {
		while (fscanf(fnbFile, "%[^|]|%lf\n", &fnb[*numOfItem].fnbName, &fnb[*numOfItem].price) != EOF)
			(*numOfItem)++;
		do {
			printf("============================================\n");
			printf("\t      F&B MENU\n");
			printf("============================================\n\n");
			for (int i = 0; i < *numOfItem; i++) {
				printf("%d. %s - RM%.2lf\n", i + 1, fnb[i].fnbName, fnb[i].price);
			}

			printf("\nPlease enter the item number you'd like to order (1-6): "); //Ask for user's choice
			scanf("%d", &choice);
			if (choice < 1 || choice > *numOfItem) {
				printf("Invalid choice!\n");
				return;
			}

			printf("Enter the quantity: ");  //Ask for the quantity
			scanf("%d", &quantity);
			if (quantity <= 0) {
				printf("Invalid quantity!\n");
				return;
			}
			fnbTotal = fnb[choice - 1].price * quantity;  //Calculate total price

			printf("\nOrder Summary:\n");  //Display order summary
			printf("%d x %s - RM%.2lf\n", quantity, fnb[choice - 1].fnbName, fnbTotal);
			printf("Total amount: RM%.2lf\n\n", fnbTotal);

			printf("Add another order? (Y = Yes): ");
			scanf("%c", &cont);
			rewind(stdin);
		} while (toupper(cont) == 'Y');
	}
	fclose(fnbFile);
	system("cls");
}

void paymentFunction() {
	/*int paid, creditCard, onlineBanking;
	ticket.amount = count * 65.00;
	printf("\nPlace booking:\n");
	printf("Total amount: %.2lf", ticket.amount);
	printf("\nPay by:\n 1. Credit Card\n 2. Online Banking\n >  ");
	scanf("%d", &paid);
	rewind(stdin);
	if (paid = 1) {
		printf("Please enter your Credit Card No: ");
		scanf("%d", &creditCard);
		rewind(stdin);
		printf("\nSuccessfully booked!\n\n\n");
		strcpy(ticket.payment, "Booked");
		fprintf(fptr, "%s|%02d/%02d/%04d|%c|%d|%s\n", ticket.name, ticket.date.day, ticket.date.month, ticket.date.year, ticket.coach, ticket.seatNo, ticket.payment);
	}
	else if (paid = 2) {
		printf("Please enter your Account No: ");
		scanf("%d", &onlineBanking);
		rewind(stdin);
		printf("\nSuccessfully booked!\n\n\n");
		strcpy(ticket.payment, "Booked");
		fprintf(fptr, "%s|%02d/%02d/%04d|%c|%d|%s\n", ticket.name, ticket.date.day, ticket.date.month, ticket.date.year, ticket.coach, ticket.seatNo, ticket.payment);
	}
	else {
		printf("Invalid payment!\nTicket NOT booked!\n\n");
	}*/
}

void exitFunction(TICKET ticket[], int* numOfTicket) {
	system("cls");
	FILE* ticketFile;
	ticketFile = fopen("Ticket.txt", "w");
	if (ticketFile == NULL) {
		printf("Can't open the file Ticket.txt!\n");
		exit(-1);
	}
	for (int i = 0; i < *numOfTicket; i++) {
		fprintf(ticketFile, "%s|%s|%02d/%02d/%04d|%02d:%02d|%02d/%02d/%04d|%s|%s|%c|%d|%s\n",
			ticket[i].ticketID, ticket[i].name, ticket[i].bookDate.day, ticket[i].bookDate.month, ticket[i].bookDate.year, ticket[i].time.hour, ticket[i].time.min, ticket[i].departDate.day, ticket[i].departDate.month, ticket[i].departDate.year,
			ticket[i].departStation, ticket[i].arrivStation, ticket[i].coach, ticket[i].seatNo, ticket[i].status);
	}
	fclose(ticketFile);
	printf("======================================================\n");
	printf("Thank You for using TICKET BOOKING. See you next time!\n");
	printf("======================================================\n");
	exit(-1);
}



// Post-Login Display and Functions: STAFF MODULE
void staffMenu(Staff* staffInformation) {
	int selectF;
	bool returnToMain= false;

	while (returnToMain == false) {
		system("cls");
		title();
		printf("Name : %s\n", staffInformation->information.name);
		printf("Address : %s\n", staffInformation->information.address);
		printf("Phone Number : %s\n", staffInformation->information.phoneNumber);
		printf("Email : %s\n", staffInformation->information.email);
		printf("Position : %s\n", staffInformation->information.position);
		decorationFlower();
		if (strcmp(staffInformation->information.position, "MANAGER") == 0) {
			printf("%25s", "1. Modify\n2. Display Duty Schedule\n3. Add Member Register\n4. Train Scheduling\n5. Ticket Booking\n6. RESIGNATION\n7. Search Staff Information\n8. EXIT\n");
			printf("Select Desired Function >> ");
			rewind(stdin);
			scanf("%d", &selectF);
			switch (selectF) {
			case 1:
				returnToMain = staffModify();
				break;
			case 2: 
				duty(*staffInformation);
				break;
			case 3:		
				signUp();
				break;
			case 4:		// Combine: zw function
				system("cls");
				scheduleMain();
				break;
			case 5:		// Combine: zy function
				break;
			case 6:
				system("cls");
				returnToMain = applyResign();
				break;
			case 7:
				system("cls");
				searchStaff();
				break;
			case 8:
				system("cls");
				returnToMain = true;
				break;
			default:
				break;
			}


		}
		else {
			printf("%25s", "1. Modify\n2. Display Duty Schedule\n3. Add Member Register\n4. Train Scheduling\n5. Ticket Booking\n6. RESIGNATION\n7. EXIT\n");
			printf("Select the function you want >> ");
			rewind(stdin);
			scanf("%d", &selectF);
			switch (selectF)
			{
			case 1:
				returnToMain = staffModify();
				break;
			case 2:
				duty(*staffInformation);
				break;
			case 3:
				signUp();
				break;
			case 4:
				system("cls");
				scheduleMain();
				break;
			case 5:
				break;
			case 6:
				system("cls");
				returnToMain = applyResign();
				break;
			case 7:
				system("cls");
				returnToMain = true;
				break;
			default:
				break;
			}

		}
		decorationFlower();
	}
}

bool staffModify() {
	system("cls");
	Staff modify[10];
	int modifySave = 0, found, selectModify = 0;
	char i = 0, staffPassword[PASSWORD_SIZE], newName[USERNAME_SIZE], addressNew[ADDR_SIZE], phoneNumberNew[CONTACT_SIZE], emailNew[EMAIL_SIZE], position[20], confirm;
	char add;
	FILE* modifyR = fopen("Staff.bin", "rb");
	if (modifyR == NULL) {
		perror("Error opening file");
		return;
	}
	while (fread(&modify[i], sizeof(Staff), 1, modifyR))
	{
		i++;
		modifySave = i;
	}
	fclose(modifyR);
	printf("Enter your Password :");
	rewind(stdin);
	scanf("%s", &staffPassword);

	system("cls");
	do {
		found = 0;
		for (i = 0; i < modifySave; i++) {
			if (strcmp(staffPassword, modify[i].staffPassword) == 0)
			{
				found = 1;
				do
				{

					printf("1)Name : %s\n", modify[i].information.name);
					printf("2)Address: %s\n", modify[i].information.address);
					printf("3)Phone Number : +60%s\n", modify[i].information.phoneNumber);
					printf("4)Email : %s\n", modify[i].information.email);
					printf("Please select the data you want to modify >> ");
					scanf("%d", &selectModify);
					switch (selectModify)
					{
					case 1: {
						printf("Your current Name is %s\n", modify[i].information.name);
						printf("Enter your new data >> ");
						rewind(stdin);
						scanf("%[^\n]", &newName);

						break;
					}
					case 2: { printf("Your current Address is %s\n", modify[i].information.address);
						printf("Enter your new data >> ");
						rewind(stdin);
						scanf("%[^\n]", &addressNew);

						break;
					}
					case 3: {printf("Your current Phone Number  is %s\n", modify[i].information.phoneNumber);
						printf("Enter your new data >> +60");
						rewind(stdin);
						scanf("%[^\n]", &phoneNumberNew);

						break;
					}
					case 4: {
						printf("Your current Email is %s\n", modify[i].information.email);
						printf("Enter your new data >> ");
						rewind(stdin);
						scanf("%[^\n]", &emailNew);

						break;
					}
					default:
						printf("You've entered the wrong information. \n");
						break;
					}

				} while (selectModify < 1 || selectModify > 5);
				printf("Confirm to modify the new data ? (Y=YES)|(N=NO)");
				scanf(" %c", &confirm);
				rewind(stdin);
				if (toupper(confirm) == 'Y') {
					if (selectModify == 1) {
						strcpy(modify[i].information.name, newName);
					}
					else if (selectModify == 2) {
						strcpy(modify[i].information.address, addressNew);
					}
					else if (selectModify == 3) {
						strcpy(modify[i].information.phoneNumber, phoneNumberNew);
					}
					else if (selectModify == 4) {
						strcpy(modify[i].information.email, emailNew);
					}
					else if (selectModify == 5) {
						strcpy(modify[i].information.position, position);
					}
					printf("Data updated successfully.\n");
				}
				else {
					printf("Update canceled.\n");
				}

			}
		}
		if (!found)
		{
			printf("The %s is error password\n", staffPassword);
		}
		system("cls");
		printf("Any more record to modify(Y=yes)");
		rewind(stdin);
		scanf("%c", &add);

	} while (toupper(add) == 'Y');
	FILE* modifyW = fopen("Staff.bin", "wb");
	for (i = 0; i < modifySave; i++)
	{
		fwrite(&modify[i], sizeof(Staff), 1, modifyW);
	}
	fclose(modifyW);


	printf("Changes saved! Please re-login...");
	getch();
	system("cls");
	return true;


}

void duty(Staff dutyShow) {
	bool backToMenu = false;
	while (backToMenu == false) {
		system("cls");
		title();
		decorationFlower();
		int selectApplyLeave = 0;

		printf("Name: %s\n", dutyShow.information.name);
		printf("ID: %s\n", dutyShow.staffID);
		printf("Position: %s\n", dutyShow.information.position);


		if (strcmp(dutyShow.information.position, "ENGINEER") == 0) {
			line();
			printf("|%-10s|%-25s|%-25s|%-25s|%-25s|%-25s\n", " Day", " 8:00a.m.", " 10:00a.m.", " 12:00p.m.", " 2:00p.m.", " 4:00p.m.");
			line();

			for (int day = 0; day < 7; day++) {
				printf("|%-10s", dutyShow.dutyScadule.week[day]);
				if (strcmp(dutyShow.dutyScadule.work[day][0], "Rest Day") == 0 || day == 0 || day == 6) {

					printf("|%-99s\n", "Rest day");
				}
				else {
					for (int task = 0; task < 4; task++) {
						printf("|%-25s", dutyShow.dutyScadule.work[day][task]);
					}
					printf("|%-25s\n", dutyShow.dutyScadule.work[day][4]);
				}
				line();
			}
		}
		else if (strcmp(dutyShow.information.position, "COUNTER ATTENDANT") == 0) {
			line();
			printf("|%-10s|%-25s|%-25s|%-25s|%-25s|%-25s|\n", " Day", " 8:00a.m.", " 10:00a.m.", " 12:00p.m.", " 2:00p.m.", " 4:00p.m.");
			line();

			for (int day = 0; day < 7; day++) {
				printf("|%-10s", dutyShow.dutyScadule.week[day]);
				if (strcmp(dutyShow.dutyScadule.work[day][0], "Rest Day") == 0 || day == 0 || day == 6) {

					printf("| %-99s\n", "Rest day");
				}
				else {

					for (int task = 0; task < 4; task++) {
						printf("|%-25s", dutyShow.dutyScadule.work[day][task]);
					}
					printf("|%-25s\n", dutyShow.dutyScadule.work[day][4]);
				}
				line();
			}
		}
		else if (strcmp(dutyShow.information.position, "MANAGER") == 0) {
			line();
			printf("|%-10s|%-25s|%-25s|%-25s|%-25s|%-25s|\n", " Day", " 8:00a.m.", " 10:00a.m.", " 12:00p.m.", " 2:00p.m.", " 4:00p.m.");
			line();

			for (int day = 0; day < 7; day++) {
				printf("| %-10s", dutyShow.dutyScadule.week[day]);
				if (strcmp(dutyShow.dutyScadule.work[day][0], "Rest Day") == 0 || day == 0 || day == 6) {

					printf("| %-99s\n", "Rest day");
				}
				else {

					for (int task = 0; task < 4; task++) {
						printf("|%-25s", dutyShow.dutyScadule.work[day][task]);
					}
					printf("|%-25s\n", dutyShow.dutyScadule.work[day][4]);
				}
				line();
			}

		}
		else {
			printf("No duty schedule available for this position.\n");
		}

		printf("Apply Leave :1 \t Exit:2\n");
		printf("Select:");
		scanf("%d", &selectApplyLeave);
		clearInputBuffer();
		line();
		switch (selectApplyLeave) {
		case 1:
			system("cls");
			leaveFunction(dutyShow);
			break;
		case 2:
			system("cls");
			backToMenu = true;
			break;
		default:
			printf("Invalid Input! Press Any key to retry..");
			getch();
		}

	}
}

bool applyResign() {
	Staff applyResign[MAX_STAFF_INFORMATION];
	int i, pEntries;
	char selectCom[6], choice, passwordCheck[20], reason[200];
	FILE* sthA = fopen("Staff.bin", "rb");
	if (sthA == NULL) {
		perror("Error opening file");
		return;
	}

	pEntries = 0;
	while (fread(&applyResign[pEntries], sizeof(Staff), 1, sthA) == 1) {
		pEntries++;
	}
	fclose(sthA);

	printf("Are you sure you want to quit for the job?\nY (YES) || N (NO)\n");
	scanf("%s", selectCom);
	clearInputBuffer();

	if (toupper(selectCom[0]) == 'Y') {
		printf("Please enter your Password.\n>>>>> ");
		scanf("%s", passwordCheck);
		clearInputBuffer();

		for (i = 0; i < pEntries; i++) {
			if (strcmp(applyResign[i].staffPassword, passwordCheck) == 0) {
				printf("Your reason for resignation: ");
				scanf("%[^\n]", reason);
				clearInputBuffer();
				printf("Are you sure you want to resign, %s? (Y/N)\n", applyResign[i].information.name);
				scanf("%c", &choice);
				clearInputBuffer();

				if (toupper(choice) == 'Y') {
					printf("Deleting...\n");
					//shift the rest forward
					do {
						applyResign[i] = applyResign[i + 1];
					} while (++i < pEntries - 1);

					//write into file
					FILE* sthW = fopen("Staff.bin", "wb");
					if (sthW == NULL)
					{
						printf("File cannot be opened.\n");
						return;
					}
					fwrite(applyResign, sizeof(Staff), pEntries - 1, sthW);
					printf("Completed.\n");
					getch();
					fclose(sthW);
					return true;
				}
			}
		}
		if (i == pEntries) {
			printf("Password not matched any records or cancellation not confirmed.\n");
		}
	}
	else if (toupper(selectCom[0]) == 'N') {
		printf("Cancellation not confirmed.\n");
	}
	else {
		printf("Invalid command entered.\n");
	}
}

bool leaveFunction(Staff applyLeave) {
	int selectDate = 0;
	Staff applyLeaveSave[MAX_STAFF_INFORMATION];
	int save = 0;
	FILE* applyLifeR = fopen("Staff.bin", "rb");
	if (!applyLifeR) {
		perror("Error opening file for reading");
		return;
	}
	while (fread(&applyLeaveSave[save], sizeof(Staff), 1, applyLifeR))
	{
		save++;
	}
	fclose(applyLifeR);

	char week[15];

	printf("Name: %s\n", applyLeave.information.name);
	printf("ID: %s\n", applyLeave.staffID);
	printf("Position: %s\n", applyLeave.information.position);
	printf("What day you want to apply leave\n");
	printf("---------------------------------------------------------\n");
	printf("|1) MONDAY |2) TUESDAY |3) WEDNESDAY|4) THURSDAY|5) FRIDAY|\n");
	printf("Select>>");
	scanf("%d", &selectDate);
	printf("----------------------------------------------------------\n");
	// Corrected portion of the function
	for (int i = 0; i < save; i++) {
		if (!strcmp(applyLeaveSave[i].staffID, applyLeave.staffID)) {
			const char* restDay = "Rest Day     ";
			if (selectDate >= 1 && selectDate <= 5) {
				for (int task = 0; task <= 4; task++) {
					strcpy(applyLeaveSave[i].dutyScadule.work[selectDate][task], restDay);  // Array is 0-indexed
				}
				switch (selectDate) {
				case 1: strcpy(week, "MONDAY"); break;
				case 2: strcpy(week, "TUESDAY"); break;
				case 3: strcpy(week, "WEDNESDAY"); break;
				case 4: strcpy(week, "THURSDAY"); break;
				case 5: strcpy(week, "FRIDAY"); break;
				}
				printf("Leave applied successfully for %s.\n", week);
			}
			else {
				printf("Invalid day selection.\n");
			}
		}
	}
	getchar();

	FILE* applyLeaveW = fopen("Staff.bin", "wb");
	if (!applyLeaveW) {
		perror("Error opening file for writing");
		return;
	}
	fwrite(applyLeaveSave, sizeof(Staff), save, applyLeaveW);
	fclose(applyLeaveW);
	char select;
	printf("Press any key to return to staff page.");
	getch();
	system("cls");
	return true;
}

void searchStaff() {
	char ID[20];
	int found = 0;
	FILE* searchR = fopen("Staff.bin", "rb");
	if (searchR == NULL) {
		perror("Error opening file");
		return;
	}

	Staff searchInformation[10];
	int p = 0;
	while (fread(&searchInformation[p], sizeof(Staff), 1, searchR) && p < 10) {
		p++;
	}
	fclose(searchR);

	title();
	decoration();
	printf("Please enter the Staff ID you want to find: ");
	scanf("%s", ID);
	clearInputBuffer();

	for (int i = 0; i < p; i++) {
		if (strcmp(searchInformation[i].staffID, ID) == 0) {
			printf("Name: %s\n", searchInformation[i].information.name);
			printf("Address: %s\n", searchInformation[i].information.address);
			printf("Phone Number: %s\n", searchInformation[i].information.phoneNumber);
			printf("Email: %s\n", searchInformation[i].information.email);
			printf("Position: %s\n", searchInformation[i].information.position);
			getchar();
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("Invalid ID entered.\n");
	}
}

void scheduleMain() {
	//prompt user to choose a module
	int choice;
	do {
		printf("============================================\n");
		printf("\t      TRAIN SCHEDULING\n");
		printf("============================================\n\n");
		printf(" 1. Add Train(s)\n");
		printf(" 2. Modify Train(s)\n");
		printf(" 3. Add Feedback & Maintanence(s)\n");
		printf(" 4. Display Feedback & Maintanence\n");
		printf(" 5. Search Train(s)\n");
		printf(" 6. Display All Train(s)\n");
		printf(" 7. Exit \n\n");
		printf("Please select your choice >> ");
		scanf("%d", &choice);
		fflush(stdin);

		switch (choice) {
		case 1: {
			system("cls");
			addTrain();
			break;
		}
		case 2: {
			system("cls");
			modifyTrain();
			break;
		}
		case 3: {
			system("cls");
			addMainFeed();
			break;
		}
		case 4: {
			system("cls");
			displayMainFeed();
			break;
		}
		case 5: {
			system("cls");
			searchTrain();
			break;
		}
		case 6: {
			system("cls");
			displayTrainList();
			break;
		}
		case 7: {
			printf("Exiting menu...\n\n");
			break;
		}
		default:
			printf("Invalid choice, please key in again.\n\n");
		}
	} while (choice != 7);
	printf("END of processing...\n\n");
	system("pause");
}

void addTrain() {
	FILE* add;
	char selection = 'Y';
	Train train;
	int i = 0, count = 0;

	if ((add = fopen("train.txt", "a")) == NULL) {
		printf("Error opening the file...\n\n");
		return;
	}
	//Prompt user to add a new record
	printf("Add A Train Record\n");
	printf("==================\n");
	printf("Add a new train record (Y = Yes)? > ");
	rewind(stdin);
	scanf("%c", &selection);
	printf("\n");
	while (toupper(selection) == 'Y') {
		printf("Enter the Time following the format.\n");
		printf("(Hours:Minutes) (24H Format)\n");
		printf("\n");
		printf("Train ID			: ");
		scanf("%d", &train.trainID);
		printf("Departure Station		: ");
		rewind(stdin);
		scanf("%[^\n]", train.departureStation);
		printf("Departure Platform		: ");
		scanf("%d", &train.departurePlatform);
		printf("Departure Time			: ");
		rewind(stdin);
		scanf("%[^\n]", train.departureTime);
		printf("Arrival Station			: ");
		rewind(stdin);
		scanf("%[^\n]", train.arrivalStation);
		printf("Estimated Arrival Time		: ");
		rewind(stdin);
		scanf("%[^\n]", train.eta);
		printf("Ticket Price			: ");
		scanf("%lf", &train.ticketPrice);
		printf("Available Seats			: ");
		scanf("%d", &train.availableSeats);
		printf("Train Status			: ");
		rewind(stdin);
		scanf("%[^\n]", train.trainStatus);
		printf("\n");
		//write train record to the file
		fprintf(add, "%d#%s#%d#%s#%s#%s#%lf#%d#%s\n",
			train.trainID, train.departureStation, train.departurePlatform, train.departureTime, train.arrivalStation, train.eta,
			train.ticketPrice, train.availableSeats, train.trainStatus);
		printf("Record successfully added...\n\n");
		count++;
		printf("Add another record (Y = Yes) (N = No) ? ");
		rewind(stdin);
		scanf("%c", &selection);
		system("cls");
	}
	printf("\n");
	printf("%d Records have been added\n\n", count);
	fclose(add);
	system("cls");
}

void modifyTrain() {
	FILE* modify;
	Train trains[TRAINS];
	int i = 0, count = 0, modifyId, found = 0, choice, newPlatform, newSeats;
	char confirm, newDptStation[STATION], newTime[TIME], newArrStation[STATION], newEta[TIME], newStatus[STATUS];
	double newPrice;

	if ((modify = fopen("train.txt", "r")) == NULL) {
		printf("Error opening the file...\n\n");
		return;
	}

	//reading data from text file
	while (fscanf(modify, "%d#%[^#]#%d#%[^#]#%[^#]#%[^#]#%lf#%d#%[^\n]\n",
		&trains[i].trainID, trains[i].departureStation, &trains[i].departurePlatform, trains[i].departureTime, trains[i].arrivalStation, trains[i].eta,
		&trains[i].ticketPrice, &trains[i].availableSeats, trains[i].trainStatus) != EOF) {
		i++;
	}
	count = i;
	fclose(modify);
	//prompt user to choose train to modify
	printf("Modify A Train Record\n");
	printf("=====================\n");
	printf("Enter the Train ID of the train you want to modify: ");
	rewind(stdin);
	scanf("%d", &modifyId);

	for (i = 0; i < count; i++) {
		if (trains[i].trainID == modifyId) {
			printf("\nSelect the field you want to modify:\n");
			printf("1. Departure Station\n");
			printf("2. Departure Platform\n");
			printf("3. Departure Time\n");
			printf("4. Arrival Station\n");
			printf("5. Estimated Arrival Time\n");
			printf("6. Ticket Price\n");
			printf("7. Available Seats\n");
			printf("8. Train Status\n");
			printf("\nEnter your choice: ");
			scanf("%d", &choice);

			switch (choice) {
			case 1:
				printf("\nEnter the new departure station: ");
				rewind(stdin);
				scanf("%[^\n]", newDptStation);
				break;
			case 2:
				printf("\nEnter the new departure platform: ");
				scanf("%d", &newPlatform);
				break;
			case 3:
				printf("\nEnter the new departure time: ");
				rewind(stdin);
				scanf("%[^\n]", newTime);
				break;
			case 4:
				printf("\nEnter the new arrival station: ");
				rewind(stdin);
				scanf("%[^\n]", newArrStation);
				break;
			case 5:
				printf("\nEnter the new estimated arrival time: ");
				rewind(stdin);
				scanf("%[^\n]", newEta);
				break;
			case 6:
				printf("\nEnter the new ticket price: ");
				scanf("%lf", &newPrice);
				break;
			case 7:
				printf("\nEnter the new number of available seats: ");
				scanf("%d", &newSeats);
				break;
			case 8:
				printf("\nEnter the new train status: ");
				rewind(stdin);
				scanf("%[^\n]", newStatus);
				break;
			default:
				printf("\nInvalid choice.No changes made.\n");
				return;
			}

			printf("\nConfirm to modify the new data ? (Y=YES)|(N=NO) > ");
			rewind(stdin);
			scanf(" %c", &confirm);
			rewind(stdin);
			//copy new data and replace old data
			if (toupper(confirm) == 'Y') {
				if (choice == 1) {
					strcpy(trains[i].departureStation, newDptStation);
				}
				else if (choice == 2) {
					trains[i].departurePlatform = newPlatform;
				}
				else if (choice == 3) {
					strcpy(trains[i].departureTime, newTime);
				}
				else if (choice == 4) {
					strcpy(trains[i].arrivalStation, newArrStation);
				}
				else if (choice == 5) {
					strcpy(trains[i].eta, newEta);
				}
				else if (choice == 6) {
					trains[i].ticketPrice = newPrice;
				}
				else if (choice == 7) {
					trains[i].availableSeats = newSeats;
				}
				else if (choice == 8) {
					strcpy(trains[i].trainStatus, newStatus);
				}
				printf("Data updated successfully.\n");
			}
			else {
				printf("Update canceled.\n");
			}
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("No train found with the given Train ID.\n");
		return;
	}

	if ((modify = fopen("train.txt", "w")) == NULL) {
		printf("Error opening the file...\n\n");
		return;
	}

	//display updated records
	system("cls");
	printf("\nCurrent Train Record\n\n");
	printf("%-10s | %-20s | %-18s | %-14s | %-16s | %-6s | %-13s | %-16s | %s\n",
		"Train ID",
		"Departure Station",
		"Departure Platform",
		"Departure Time",
		"Arrival Station",
		"ETA",
		"Ticket Price",
		"Available Seats",
		"Train Status");
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");

	for (i = 0; i < count; i++) {
		fprintf(modify, "%d#%s#%d#%s#%s#%s#%lf#%d#%s\n",
			trains[i].trainID, trains[i].departureStation, trains[i].departurePlatform, trains[i].departureTime, trains[i].arrivalStation, trains[i].eta,
			trains[i].ticketPrice, trains[i].availableSeats, trains[i].trainStatus);
		printf("%-10d | %-20s | %-18d | %-14s | %-16s | %-6s | %-13.2lf | %-16d | %s\n",
			trains[i].trainID, trains[i].departureStation, trains[i].departurePlatform, trains[i].departureTime, trains[i].arrivalStation, trains[i].eta,
			trains[i].ticketPrice, trains[i].availableSeats, trains[i].trainStatus);
	}
	printf("\nRecord successfully modified...\n\n");
	printf("Press any key to go back > ");
	getch();
	fclose(modify);
	system("cls");
}

void addMainFeed() {
	FILE* readList;
	FILE* writeList;
	Train trains[TRAINS];
	MainFeed mainfeed[TRAINS];
	int i = 0, count = 0, chosenTrain;

	if ((readList = fopen("train.txt", "r")) == NULL) {
		printf("Error opening the file...\n\n");
		return;
	}

	//reading data from text file
	while (fscanf(readList, "%d#%[^#]#%d#%[^#]#%[^#]#%[^#]#%lf#%d#%[^\n]\n",
		&trains[i].trainID, trains[i].departureStation, &trains[i].departurePlatform,
		trains[i].departureTime, trains[i].arrivalStation, trains[i].eta,
		&trains[i].ticketPrice, &trains[i].availableSeats, trains[i].trainStatus) != EOF) {
		i++;
	}

	//add feedback & maintenance data
	printf("Add Train Feedback & Maintenance\n");
	printf("================================\n");
	printf("Select the train you want to add to: ");
	scanf("%d", &chosenTrain);

	for (int j = 0; j < i; j++) {
		if (chosenTrain == trains[j].trainID) {
			printf("\nAdd details for Train %d\n", trains[j].trainID);
			printf("==============================\n");
			printf("Add Feedback: ");
			rewind(stdin);
			scanf("%[^\n]", mainfeed[j].feedback);
			printf("\nAdd Maintenance: ");
			rewind(stdin);
			scanf("%[^\n]", mainfeed[j].maintenance);
			printf("\n");

			if ((writeList = fopen("staffPurpose.txt", "a")) == NULL) {
				printf("Error opening the file...\n\n");
				return;
			}

			fprintf(writeList, "%d#%s#%s\n",
				trains[j].trainID, mainfeed[j].feedback, mainfeed[j].maintenance);
			printf("Feedback & Maintenance successfully added...\n\n");
			count++;
			fclose(writeList);
			break;
		}
	}

	if (count == 0) {
		printf("Train ID not found.\n");
	}

	printf("%d detail records have been added.\n\n", count);
	printf("Press any key to go back > ");
	getch();
	fclose(readList);
	system("cls");
}

void displayMainFeed() {
	FILE* list;
	Train trains[TRAINS];
	MainFeed mainfeed[TRAINS];
	int i = 0;

	if ((list = fopen("staffPurpose.txt", "r")) == NULL) {
		printf("Error opening the file...\n\n");
		return;
	}

	printf("Train Feedback & Maintenance List\n");
	printf("=================================\n\n");
	printf("%-10s | %-20s | %-18s\n",
		"Train ID",
		"Feedback",
		"Maintenance");

	printf("----------------------------------------------------------------\n");

	//read data from text file and print out
	while (fscanf(list, "%d#%[^#]#%[^\n]",
		&trains[i].trainID, mainfeed[i].feedback, mainfeed[i].maintenance) != EOF) {
		printf("%-10d | %-20s | %-18s \n",
			trains[i].trainID,
			mainfeed[i].feedback,
			mainfeed[i].maintenance);
		i++;
	}

	printf("\nPress any key to go back > ");
	getch();
	system("cls");
	fclose(list);
}

void searchTrain() {
	FILE* search;
	int i = 0, count = 0, searchTrain;
	char cont;
	Train trains[TRAINS];

	if ((search = fopen("train.txt", "r")) == NULL) {
		printf("Error opening the file...\n\n");
		return;
	}

	//reading data from text file
	while (fscanf(search, "%d#%[^#]#%d#%[^#]#%[^#]#%[^#]#%lf#%d#%[^\n]\n",
		&trains[i].trainID, trains[i].departureStation, &trains[i].departurePlatform, trains[i].departureTime, trains[i].arrivalStation, trains[i].eta,
		&trains[i].ticketPrice, &trains[i].availableSeats, trains[i].trainStatus) != EOF) {
		i++;
	}

	//search from text file and display specific train
	do {
		printf("Search A Train Record\n");
		printf("=====================\n");
		printf("Train ID(s) : ");
		scanf("%d", &searchTrain);
		printf("=====================\n\n");
		printf("%-10s | %-20s | %-18s | %-14s | %-16s | %-6s | %-13s | %-16s | %s\n",
			"Train ID", "Departure Station", "Departure Platform", "Departure Time",
			"Arrival Station", "ETA", "Ticket Price", "Available Seats", "Train Status");
		printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");

		for (int i = 0; i < TRAINS; i++) {
			if (searchTrain == trains[i].trainID) {
				printf("%-10d | %-20s | %-18d | %-14s | %-16s | %-6s | %-13.2lf | %-16d | %s\n\n",
					trains[i].trainID, trains[i].departureStation, trains[i].departurePlatform,
					trains[i].departureTime, trains[i].arrivalStation, trains[i].eta,
					trains[i].ticketPrice, trains[i].availableSeats, trains[i].trainStatus);
			}
		}

		printf("\nSearch another (Y = yes)? ");
		rewind(stdin);
		scanf(" %c", &cont);
		system("cls");
	} while (toupper(cont) == 'Y');

	printf("Press any key to go back > ");
	getch();
	system("cls");
}

void displayTrainList() {
	FILE* list;
	Train trains[TRAINS];
	int i = 0;

	if ((list = fopen("train.txt", "r")) == NULL) {
		printf("Error opening the file...\n\n");
		return;
	}

	printf("Train Schedule List\n");
	printf("===================\n\n");
	printf("%-10s | %-20s | %-18s | %-14s | %-16s | %-6s | %-13s | %-16s | %s\n",
		"Train ID",
		"Departure Station",
		"Departure Platform",
		"Departure Time",
		"Arrival Station",
		"ETA",
		"Ticket Price",
		"Available Seats",
		"Train Status");
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");

	//read data from text file and print out
	while (fscanf(list, "%d#%[^#]#%d#%[^#]#%[^#]#%[^#]#%lf#%d#%[^\n]",
		&trains[i].trainID, trains[i].departureStation, &trains[i].departurePlatform, trains[i].departureTime, trains[i].arrivalStation, trains[i].eta,
		&trains[i].ticketPrice, &trains[i].availableSeats, trains[i].trainStatus) != EOF) {
		printf("%-10d | %-20s | %-18d | %-14s | %-16s | %-6s | %-13.2lf | %-16d | %s\n",
			trains[i].trainID,
			trains[i].departureStation,
			trains[i].departurePlatform,
			trains[i].departureTime,
			trains[i].arrivalStation,
			trains[i].eta,
			trains[i].ticketPrice,
			trains[i].availableSeats,
			trains[i].trainStatus);
		i++;
	}
	printf("\nPress any key to go back > ");
	getch();
	system("cls");
	fclose(list);
}



// Non-Display Functions
int randomNumGen() {
	int n, upper = 9999, lower = 1;
	srand(time(NULL));
	n = (rand() % (upper - lower + 1)) + lower;

	return n;
}

static size_t payload_source(char* ptr, size_t size, size_t nmemb, void* userp)
{
	struct upload_status* upload_ctx = (struct upload_status*)userp;
	const char* data;
	size_t room = size * nmemb;

	if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
		return 0;
	}

	data = upload_ctx->content + upload_ctx->bytes_read;

	if (data) {
		size_t len = strlen(data);
		if (room < len)
			len = room;
		memcpy(ptr, data, len);
		upload_ctx->bytes_read += len;

		return len;
	}

	return 0;
}

int sendMail(char* to_mail, int code)
{
	CURL* curl;
	CURLcode res = CURLE_OK;
	struct curl_slist* recipients = NULL;
	struct upload_status upload_ctx = { 0 };

	snprintf(upload_ctx.content, 1024, PAYLOAD_TEXT, to_mail, code);

	curl = curl_easy_init();
	if (curl) {
		/* Set username and password */
		curl_easy_setopt(curl, CURLOPT_USERNAME, FROM_MAIL);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, APP_PASSWORD);

		curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");

		curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_MAIL);
		recipients = curl_slist_append(recipients, to_mail);
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

#ifndef DEBUG
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}

	return (int)res;
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

char charInput(const char* prompt) {
	printf("%s", prompt);

	int ch = getc(stdin);

	if (ch == '\n')
		// Fucking Idiot pressed "ENTER" who tf does that
		return 1;

	flush(stdin);
	return ch;
}

void flush(FILE* stream) {
	int c;
	while ((c = getc(stream)) != '\n' && c != EOF);
}

int readTicketFile(TICKET ticket[]) {						// Function to read ticket information from a file.
	int count = 0;											// Counter for the number of tickets read.
	FILE* readTF;											// File pointer for reading the ticket file.
	TICKET temp;											// Temporary variable to hold ticket data while reading.
	readTF = fopen("Ticket.txt", "r");						// Open the ticket file for reading.
	if (readTF == NULL) {									// Check if the file opened successfully.
		printf("Can't open the file ticket.txt!\n\n");
		exit(-1);
	}
	while (fscanf(readTF, "%[^|]|%[^|]|%02d/%02d/%04d|%02d:%02d|%02d/%02d/%04d|%[^|]|%[^|]|%c|%d|%[^\n]\n",
		temp.ticketID, temp.name, &temp.bookDate.day, &temp.bookDate.month, &temp.bookDate.year, &temp.time.hour, &temp.time.min, &temp.departDate.day, &temp.departDate.month, &temp.departDate.year,
		temp.departStation, temp.arrivStation, &temp.coach, &temp.seatNo, temp.status) != EOF) {		 // Read ticket information from the file until the end of file (EOF) is reached.
		ticket[count] = temp;					// Assign the read ticket data to the corresponding element in the ticket array.
		count++;								// Increment the ticket count.
	}
	fclose(readTF);								// Close the file after reading.

	return count;								 // Return the total number of tickets read from the file.
}

void clearInputBuffer() {
	while (getchar() != '\n');
}

void decorationFlower() {
	printf("%s", "*");
	for (int i = 0; i < 40; i++) {
		printf("*");
	}
	printf("\n");
}

void title() {
	int i;
	printf("\n\n");
	decoration();
	printf("%s", ""); // Centering the title
	for (i = 0; i < 3; i++) {
		printf("#"); // Start border
	}
	printf(" Bla Bla Bla Train Station "); // Title
	for (i = 0; i < 3; i++) {
		printf("#"); // End border
	}
	printf("\n");
	decoration();
	printf("\n");
}

void decoration() {
	int i;
	printf("%s", ""); // Centering the decoration
	for (i = 0; i < 35; i++) {
		printf("*");
	}
	printf("\n");
}

void line() {

	for (int i = 0; i < 150; i++)
	{
		printf("-");
	}
	printf("\n");
}