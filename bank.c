#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD "admin123"

struct Account {
    int acc_no;
    char name[50];
    float balance;
};

void createAccount() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "ab");

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.acc_no);
    printf("Enter Name: ");
    scanf(" %[^\n]", acc.name);
    printf("Enter Initial Deposit: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);
    printf("Account created successfully!\n");
}

void displayAccount() {
    int acc_no, found = 0;
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "rb");

    printf("\nEnter Account Number to Search: ");
    scanf("%d", &acc_no);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("\nAccount Number: %d\nName: %s\nBalance: ₹%.2f\n",
                   acc.acc_no, acc.name, acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

void depositWithdraw(int isDeposit) {
    int acc_no, found = 0;
    float amount;
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "rb+");

    printf("\nEnter Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("Current Balance: ₹%.2f\n", acc.balance);
            printf("Enter Amount to %s: ", isDeposit ? "Deposit" : "Withdraw");
            scanf("%f", &amount);

            if (!isDeposit && acc.balance < amount) {
                printf("Insufficient balance.\n");
                fclose(fp);
                return;
            }

            acc.balance += isDeposit ? amount : -amount;

            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);

            printf("%s Successful. New Balance: ₹%.2f\n",
                   isDeposit ? "Deposit" : "Withdrawal", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

int login() {
    char pass[20];
    printf("Enter Admin Password: ");
    scanf("%s", pass);
    if (strcmp(pass, PASSWORD) == 0) {
        return 1;
    } else {
        printf("Incorrect password.\n");
        return 0;
    }
}

int main() {
    int choice;

    if (!login()) return 0;

    do {
        printf("\n--- Bank Management System ---\n");
        printf("1. Create Account\n");
        printf("2. View Account\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: displayAccount(); break;
            case 3: depositWithdraw(1); break;
            case 4: depositWithdraw(0); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid option. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
