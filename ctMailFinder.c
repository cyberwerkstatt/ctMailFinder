#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMAIL_LENGTH 256
#define MAX_COMMAND_LENGTH 512
#define MAX_LINE_LENGTH 1024

int execute_command_and_write_results(const char *email, FILE *output_file)
{
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "holehe --only-used %s", email);

    FILE *pipe = popen(command, "r");
    if (pipe == NULL)
    {
        fprintf(output_file, "Fehler beim Ausfuehren des Befehls für %s\n", email);
        return 0; // Misserfolg
    }

    char line[MAX_LINE_LENGTH];
    int success_found = 0;
    while (fgets(line, sizeof(line), pipe) != NULL)
    {
        if (strncmp(line, "[+]", 3) == 0)
        {
            if (!success_found)
            {
                fprintf(output_file, "Erfolg fuer %s:\n", email);
                success_found = 1; // Erfolg gefunden
            }
            char *start = strstr(line, " ") + 1;
            if (start != NULL)
            {
                fprintf(output_file, "Verwendet auf: %s", start);
            }
        }
    }

    if (!success_found)
    {
        fprintf(output_file, "Keine Details gefunden für %s.\n", email);
    }
    fprintf(output_file, "\n----------------------------------------\n");
    pclose(pipe);
    return success_found; // Gebe den Erfolgstatus zurück
}

int main()
{
    char filename[20];
    printf("********* ctMailFinder *********\n");
    printf("\n");
    printf("Wie heisst deine Emailliste 'z.B: mails.txt' ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    FILE *source_file = fopen(filename, "r");
    if (source_file == NULL)
    {
        perror("Fehler beim Oeffnen der Quelldatei");
        system("error.bat");
        return 1;
    }

    FILE *output_file = fopen("result.txt", "w");
    if (output_file == NULL)
    {
        perror("Fehler beim Oeffnen der Zieldatei");
        system("target_error.bat");
        fclose(source_file);
        return 1;
    }

    int counter = 1; // Zähler für erfolgreiche Verarbeitungen
    char email[MAX_EMAIL_LENGTH];
    while (fgets(email, sizeof(email), source_file) != NULL)
    {
        email[strcspn(email, "\n")] = 0;
        if (strlen(email) > 0)
        {
            printf("Ueberpruefe Email: %s | Nummer: %i \n", email, counter); // Zeige die aktuell überprüfte E-Mail-Adresse an
            if (execute_command_and_write_results(email, output_file))
            {
                counter++; // Erhöhe den Zähler bei Erfolg
            }
        }
    }

    fclose(source_file);
    fclose(output_file);
    printf("%d E-Mails wurden erfolgreich verarbeitet.\n", counter - 1);
    system("success.bat");
    return 0;
}
