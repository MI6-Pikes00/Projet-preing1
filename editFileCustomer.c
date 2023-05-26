#include "editFileCustomer.h"
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void loadCustomer(Customer clients[MAX_CLIENTS], int *nbClients)
{
    // Loading clients from files

    DIR *dossier;
    struct dirent *entree;
    dossier = opendir("clientFolder");
    if (dossier == NULL)
    {
        printf("Error when opening the client file.\n");
        return;
    }

    while ((entree = readdir(dossier)) != NULL)
    {
        if (strcmp(entree->d_name, ".") == 0 || strcmp(entree->d_name, "..") == 0)
        {
            continue;
        }

        char fileName[20];
        char *folderPath = "clientFolder/";
        char *filePath = (char *)malloc(strlen(folderPath) + strlen(fileName) + 1);

        // Builds the complete path of the file
        strcpy(fileName, entree->d_name);
        strcpy(filePath, folderPath);
        strcat(filePath, fileName);

        FILE *file = fopen(filePath, "rb");
        if (file == NULL)
        {
            free(filePath);
            printf("Error when opening the file %s.\n", fileName);
            continue;
        }
        fread(&clients[*nbClients], sizeof(Customer), 1, file);
        fclose(file);
        free(filePath);
        *nbClients += 1;
    }
    closedir(dossier);
}

int genereraccountNumber()
{
    // Generation of a random number between 1000 and 9999
    return rand() % (9000) + 1000;
}

int accountNumberAttributed(int accountNumber, Customer clients[], int nbClients)
{
    // Verification of the existence of the account number in the customer files
    for (int i = 0; i < nbClients; i++)
        if (clients[i].reference == accountNumber)
        {
            return 1;
        }
    return 0;
}

void accountRegister(Customer clients[], int *nbClients)
{
    int accountNumber = 0;
    if (*nbClients == MAX_CLIENTS)
    {
        printf("The maximum number of customers is reached.\n");
        return;
    }

    // Entering the new customer's information
    Customer newCustomer;
    printf("Enter your name: ");
    scanf("%s", newCustomer.firstName);
    printf("Enter your first name: ");
    scanf("%s", newCustomer.name);
    printf("Enter your password: ");
    scanf("%s", newCustomer.password);

    // Random generation of the account number
    do
    {
        accountNumber = genereraccountNumber();
    } while (accountNumberAttributed(accountNumber, clients, *nbClients));
    newCustomer.reference = accountNumber;
    newCustomer.nbPurchase = -1;
    // Add the new client to the list of clients    clients[*nbClients] = newCustomer;
    *nbClients += 1;

    // Save the new client in a file
    char fileName[25];
    sprintf(fileName, "clientFolder/%d.dat", accountNumber);

    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
    {
        printf("Error while registering the new customer.\n");
        return;
    }
    fwrite(&newCustomer, sizeof(Customer), 1, file);
    fclose(file);

    printf("Your account has been successfully created.\n");
    printf("This is your account number: %d\n", accountNumber);
    printf("\n");
}

void deleteFile(int accountNumber)
{
    char fileName[25];
    sprintf(fileName, "clientFolder/%d.dat", accountNumber);

    if (remove(fileName) == 0)
    {
        printf("Your %d account has been successfully deleted.\n", accountNumber);
    }
    else
    {
        perror("Error while deleting the file");
    }
}

void deleteAccount(Customer clients[], int nbClients)
{
    int accountNumber = 0;

    printf("Enter your account number: ");
    scanf("%d", &accountNumber);
    printf("\n");

    if (accountNumberAttributed(accountNumber, clients, nbClients))
    {
        int choix = 0;
        do
        {
            printf("Do you want to delete your account\n");
            printf("1. Validate\n");
            printf("2. Cancel\n");

            printf("Enter your choice: ");
            scanf("%d", &choix);
            printf("\n");
            switch (choix)
            {
            case 1: // Deleting file
                deleteFile(accountNumber);
                printf("DONE !\n");
                return;

            case 2: // Cancel
                printf("Annulation ...\n");
                break;
            }
        } while (choix != 2);

        printf("Your account number does not exist.\n");
    }
}

int accountAcces(Customer clients[], int nbClients)
{
    int accountNumber = 0;

    printf("Enter your account number: ");
    scanf("%d", &accountNumber);
    printf("\n");

    char pswd[SIZE];

    printf("Enter your password: ");
    scanf("%s", pswd);
    printf("\n");

    // Search for the client corresponding to the account number
    int i;
    for (i = 0; i < nbClients; i++)
    {
        if (clients[i].reference == accountNumber && strcmp(clients[i].password, pswd) == 0)
        {
            return i;
        }
    }
    printf("Invalid account number.\n");
    return -1;
    printf("\n");
}