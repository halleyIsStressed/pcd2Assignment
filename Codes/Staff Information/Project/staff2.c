#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma warning (disable:4996)

struct StaffInformation {
    char name[30];
    char address[50];
    char phoneNumber[12];
    char gender;
    char position[20];
};

struct Duty {
    char work[7][4][70];  // Assuming you need 4 work slots per day, with up to 70 characters each
};

struct Staff {
    char staffID[10];
    char staffPassword[20];
    struct StaffInformation information;
    struct Duty dutyScadule;
};


void logINpageStaff();
void signUpStaffFunction();
void logInStaffFunction();
void staffPage(struct Staff staffInformation);
void modifyStaff();
void duty(struct Staff dutyShow);
void leaveFunction();
void decoration();
void decorationFlower();
void title();
void line();
void clearInputBuffer();
struct Staff register_staff();

int main() {
    int select;
    char abort;
    do {
        title();
        printf("%37s", "1)Staff\n");
        printf("%37s", "6)Exit\n");
        printf("%45s", "Please select >>");
        scanf("%d", &select);
        clearInputBuffer();
        if (select == 1) {
            system("cls");  // system("clear"); for Linux
            logINpageStaff();
        }
        else if (select = 6) {
            exit(-1);
        }
        system("cls");  // system("clear"); for Linux
        printf("If you want to exit the system, please enter 'X'.\nIf you want back to main page, please enter 'R'.\n");
        printf("%45s", "Please select >>");
        scanf("%c", &abort);
        if (toupper(abort) != 'Y' || toupper(abort) != 'N') {
            printf("Erroe");

        }
        clearInputBuffer();
        system("cls");  // system("clear"); for Linux
    } while (toupper(abort) != 'X');

    return 0;
}

void clearInputBuffer() {
    while (getchar() != '\n');
}

void logINpageStaff() {
    char option;
    do {
        title();
        printf("\n%43s", "Welcome staff to use this system, are you a new employee?\nIf yes please enter 'Y', if No 'N': ");
        scanf(" %c", &option);
        clearInputBuffer();
        if (toupper(option) == 'Y') {
            system("cls");
            signUpStaffFunction();
        }
        else if (toupper(option) == 'N') {
            system("cls");
            logInStaffFunction();
        }
        else {
            printf("Invalid input. Please enter 'Y' for Yes or 'N' for No.\n");
        }
    } while (toupper(option) != 'N' && toupper(option) != 'Y');
}

void signUpStaffFunction() {
    FILE* signUP = fopen("Staff.bin", "ab");  // Append mode is used to keep adding new records.
    if (signUP == NULL) {
        perror("Error opening file");
        return;
    }

    struct Staff staff = register_staff();

    // Writes the staff information to the file.
    const char* daysOfWeek[7] = { "SUNDAY\t", "MONDAY\t ", "TUESDAY\t ", "WEDNESDAY\t ", "THURSDAY\t ", "FRIDAY\t ", "SATURDAY\t " };

    if (strcmp(staff.information.position, "ENGINEER") == 0) {
        for (int day = 0; day < 7; day++) {
            // 先将星期名复制到work数组的对应位置
            strcpy(staff.dutyScadule.work[day], daysOfWeek[day]);
            if (day == 0 || day == 6)
            {
                const char* restDay[4] = { "\t Rest Day \t","\t Rest Day \t","\t Rest Day \t","\t Rest Day \t" };
                for (int task = 0; task < 4; task++) {
                    strcat(staff.dutyScadule.work[day], "   | ");
                    strcat(staff.dutyScadule.work[day], restDay[task]);
                }
                strcat(staff.dutyScadule.work[day], "   | ");
            }
            else
            {
                const char* tasks[4] = { "\t Counter A \t", "\t Breaks \t", "\t Business Premises \t", "\t Check Train D \t" };
                // 然后再添加每天的任务
                for (int task = 0; task < 4; task++) {
                    strcat(staff.dutyScadule.work[day], " | ");
                    strcat(staff.dutyScadule.work[day], tasks[task]);
                }
                // 在每天的任务后添加结束符
                strcat(staff.dutyScadule.work[day], "   | ");

            }

        }
    }
    else if (strcmp(staff.information.position, "COUNTER ATTENDANT") == 0) {
        for (int day = 0; day < 7; day++) {
            // 先将星期名复制到work数组的对应位置
            strcpy(staff.dutyScadule.work[day], daysOfWeek[day]);
            if (day == 0 || day == 6)
            {
                const char* restDay[4] = { "\t Rest Day \t","\t Rest Day \t","\t Rest Day \t","\t Rest Day \t" };
                for (int task = 0; task < 4; task++) {
                    strcat(staff.dutyScadule.work[day], " | ");
                    strcat(staff.dutyScadule.work[day], restDay[task]);
                }
                strcat(staff.dutyScadule.work[day], "   | ");
            }
            else
            {
                const char* tasks[4] = { "\t Counter A \t", "\t Breaks \t", "\t Business Premises \t", "\t Check Train D \t" };
                // 然后再添加每天的任务
                for (int task = 0; task < 4; task++) {
                    strcat(staff.dutyScadule.work[day], " | ");
                    strcat(staff.dutyScadule.work[day], tasks[task]);
                }
                // 在每天的任务后添加结束符
                strcat(staff.dutyScadule.work[day], "   | ");

            }

        }
    }
    else if (strcmp(staff.information.position, "TRAIN DRIVER") == 0) {
        for (int day = 0; day < 7; day++) {
            // 先将星期名复制到work数组的对应位置
            strcpy(staff.dutyScadule.work[day], daysOfWeek[day]);
            if (day == 0 || day == 6)
            {
                const char* restDay[4] = { "\t Rest Day \t","\t Rest Day \t","\t   Rest Day   \t","\t Rest Day \t" };
                for (int task = 0; task < 4; task++) {
                    strcat(staff.dutyScadule.work[day], " | ");
                    strcat(staff.dutyScadule.work[day], restDay[task]);
                }
                strcat(staff.dutyScadule.work[day], "   | ");
            }
            else
            {
                const char* tasks[4] = { "\t Counter A \t", "\t Breaks \t", "\t Check Customer \t", "\t Check Train D \t" };
                // 然后再添加每天的任务
                for (int task = 0; task < 4; task++) {
                    strcat(staff.dutyScadule.work[day], " | ");
                    strcat(staff.dutyScadule.work[day], tasks[task]);
                }
                // 在每天的任务后添加结束符
                strcat(staff.dutyScadule.work[day], "   | ");

            }

        }
    }
    if (strcmp(staff.information.position, "ENGINEER") == 0) {
        for (int day = 0; day < 7; day++) {
            // 先将星期名复制到work数组的对应位置
            strcpy(staff.dutyScadule.work[day], daysOfWeek[day]);
            if (day == 0 || day == 6)
            {
                const char* restDay[4] = { "\t Rest Day \t","\t Rest Day \t","\t Rest Day \t","\t Rest Day \t" };
                for (int task = 0; task < 4; task++) {
                    strcat(staff.dutyScadule.work[day], "   | ");
                    strcat(staff.dutyScadule.work[day], restDay[task]);
                }
                strcat(staff.dutyScadule.work[day], "   | ");
            }
            else
            {
                const char* tasks[4] = { "\t Counter A \t", "\t Breaks \t", "\t Business Premises \t", "\t Check Train D \t" };
                // 然后再添加每天的任务
                for (int task = 0; task < 4; task++) {
                    strcat(staff.dutyScadule.work[day], " | ");
                    strcat(staff.dutyScadule.work[day], tasks[task]);
                }
                // 在每天的任务后添加结束符
                strcat(staff.dutyScadule.work[day], "   | ");

            }

        }
    }
    fwrite(&staff, sizeof(struct Staff), 1, signUP);
    fclose(signUP);
}

void logInStaffFunction() {
    FILE* logIn = fopen("Staff.bin", "rb");
    if (logIn == NULL) {
        perror("Error opening file");
        return;
    }
    struct Staff staffCheckingInformation[10];
    char password[20], ID[10];
    int loginSuccessful = 0, i = 0, read = 0;
    while (fread(&staffCheckingInformation[i], sizeof(struct Staff), 1, logIn))
    {
        i++;
        read = i;
    }
    do {
        title();
        decorationFlower();
        printf("\n%43s", "LOG IN (ID): ");
        rewind(stdin);
        scanf("%9s", ID);
        printf("\n%43s", "Password   : ");
        rewind(stdin);
        scanf("%19s", password);
        for (i = 0; i < read; i++)
        {

            if (strcmp(ID, staffCheckingInformation[i].staffID) == 0 && strcmp(password, staffCheckingInformation[i].staffPassword) == 0) {
                loginSuccessful = 1;
                staffPage(staffCheckingInformation[i]);
                getchar();
            }
        }

        if (!loginSuccessful) {
            printf("Invalid ID or password. Please try again.\n");
        }
        rewind(logIn);
    } while (!loginSuccessful);
    fclose(logIn);
}

void staffPage(struct Staff staffInformation) {
    int selectF;
    title();
    printf("Name : %s\n", staffInformation.information.name);
    printf("Address : %s\n", staffInformation.information.address);
    printf("Phone Number : %s\n", staffInformation.information.phoneNumber);
    printf("Gender : %c\n", staffInformation.information.gender);
    printf("Position : %s\n", staffInformation.information.position);
    decorationFlower();
    printf("%25s", "*1)Modify*2)Display Duty Schedule*3)Add Member Register*\n*4)Train Scheduling*5)Ticket Booking*6)EXIT\n");
    printf("Select the function you want >>");
    rewind(stdin);
    scanf("%d", &selectF);
    switch (selectF)
    {
    case 1: {
        modifyStaff();
        break;
    }
    case 2: {
        duty(staffInformation);
        break;
    }
    case 6: {
        system("cls");
        main();
        break;
    }
    default:
        break;
    }
    decorationFlower();

}

void modifyStaff() {
    system("cls");
    struct Staff modify[10];
    int modifySave = 0, found, selectModify = 0;
    char i = 0, staffPassword[20], newName[50], adressNew[50], phoneNumberNew[13], genderNew, position[20], confirm;
    char add;
    FILE* modifyR = fopen("Staff.bin", "rb");
    if (modifyR == NULL) {
        perror("Error opening file");
        return;
    }
    while (fread(&modify[i], sizeof(struct Staff), 1, modifyR))
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

                    printf("1)Name :%s\n", modify[i].information.name);
                    printf("2)Address:%s\n", modify[i].information.address);
                    printf("3)Phone Number :%s\n", modify[i].information.phoneNumber);
                    printf("4)Gender :%c\n", modify[i].information.gender);
                    printf("5)Position :%s\n", modify[i].information.position);
                    printf("Please select the data you want to modify >> ");
                    scanf("%d", &selectModify);
                    switch (selectModify)
                    {
                    case 1: {
                        printf("Your current Name is %s\n", modify[i].information.name);
                        printf("Enter your new data >>");
                        rewind(stdin);
                        scanf("%[^\n]", &newName);

                        break;
                    }
                    case 2: { printf("Your current Address is %s\n", modify[i].information.address);
                        printf("Enter your new data >>");
                        rewind(stdin);
                        scanf("%[^\n]", &adressNew);

                        break;
                    }
                    case 3: {printf("Your current Phone Number  is %s\n", modify[i].information.phoneNumber);
                        printf("Enter your new data >>");
                        rewind(stdin);
                        scanf("%[^\n]", &phoneNumberNew);

                        break;
                    }
                    case 4: { printf("Your current Gender is %c\n", modify[i].information.gender);
                        printf("Enter your new data >>");
                        rewind(stdin);
                        scanf("%c", &genderNew);

                        break;
                    }
                    case 5: { printf("Your current Position is %s\n", modify[i].information.position);
                        printf("Enter your new data >>");
                        rewind(stdin);
                        scanf("%[^\n]", &position);

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
                        strcpy(modify[i].information.address, adressNew);
                    }
                    else if (selectModify == 3) {
                        strcpy(modify[i].information.phoneNumber, phoneNumberNew);
                    }
                    else if (selectModify == 4) {
                        strcpy(modify[i].information.gender, genderNew);
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
        fwrite(&modify[i], sizeof(struct Staff), 1, modifyW);
    }
    fclose(modifyW);
}

void duty(struct Staff dutyShow) {
    system("cls");  // 清屏开始新的显示
    title();        // 显示标题
    decorationFlower();  // 显示装饰花纹

    // 打印员工基本信息
    printf("Name: %s\n", dutyShow.information.name);
    printf("ID: %s\n", dutyShow.staffID);
    printf("Position: %s\n", dutyShow.information.position);

    // 只有职位为 ENGINEER 时才显示详细的值班表
    if (strcmp(dutyShow.information.position, "ENGINEER") == 0) {
        line();
        printf("|\tDay\t| 8.00a.m. | 9.00a.m. | 10.00 a.m. | 11.00 a.m. | 12.00 p.m. | 1.00 p.m. | 2.00 p.m. | 3.00 p.m. | 4.00 p.m. | 5.00 p.m. |\n");
        line();

        for (int k = 0; k < 7; k++) {
            printf("| %s", dutyShow.dutyScadule.work[k]);
            printf("\n");
        }

        line();
    }
    else {
        printf("No duty schedule available for this position.\n");
    }

    // 在退出前等待用户输入，避免控制台太快被清除
    printf("Press any key to continue...\n");
    getchar();  // 捕获之前的残留输入
    getchar();  // 等待新的输入
}

void leaveFunction() {

}



void decorationFlower() {
    printf("%27s", "*");
    for (int i = 0; i < 40; i++) {
        printf("*");
    }
    printf("\n");
}

void title() {
    int i;
    printf("\n\n");
    decoration();
    printf("%28s", ""); // Centering the title
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
    printf("%28s", ""); // Centering the decoration
    for (i = 0; i < 35; i++) {
        printf("*");
    }
    printf("\n");
}

void line() {

    for (int i = 0; i < 140; i++)
    {
        printf("-");
    }
    printf("\n");
}

struct Staff register_staff() {
    int position;
    struct Staff staff;
    title();
    decorationFlower();
    printf("\n%43s", "New login ID: ");
    scanf("%9s", staff.staffID);  // Ensures buffer does not overflow, leaves space for null terminator
    clearInputBuffer();
    printf("\n%43s", "New Password: ");
    scanf("%19s", staff.staffPassword);
    clearInputBuffer();
    decorationFlower();
    printf("\n%43s", "Name: ");
    scanf(" %[^\n]", staff.information.name);
    clearInputBuffer();
    printf("\n%43s", "Gender: ");
    scanf(" %c", &staff.information.gender);
    clearInputBuffer();
    printf("\n%43s", "New Phone Number: ");
    scanf("%11s", staff.information.phoneNumber);
    clearInputBuffer();
    printf("\n%43s", "Current Home Address: ");
    scanf(" %[^\n]", staff.information.address);
    clearInputBuffer();
    printf("\n%43s", "Position: 1) ENGINEER\n \t 2) COUNTER ATTENDANT\n \t 3) TRAIN DRIVER 4)MANAGER");
    printf("\n%43s", "Please select >>");
    scanf(" %d", &position);
    switch (position)
    {
    case 1: {
        strcpy(staff.information.position, "ENGINEER");
        break;
    }
    case 2: {
        strcpy(staff.information.position, "COUNTER ATTENDANT");
        break;
    }
    case 3: {
        strcpy(staff.information.position, "TRAIN DRIVER");
        break;
    }
    case 4: {
        strcpy(staff.information.position, "MANAGER");
        break;
    }

    default:
        printf("error");
        break;
    }
    clearInputBuffer();

    return staff;
}

