#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

typedef enum errors {
    OK,
    INVALID_INPUT,
    MEMORY_ERROR,
    NOT_FOUND,
    FOUND,
} error;

typedef struct user{
    char login[7];
    unsigned int Pin_code;
    int sanctions;
} user;

error check_correct_login(const char *login);

error check_correct_PIN(unsigned int Pin);

void clear_input_buffer();

error registration(user **users, int *count_of_users, int *capacity);

void Time();

void Date();

void Howmuch(time_t past_time, char *flag);

void free_users(user *users);

error login(user **users, const int *count_of_users, int *id);

void main_menu();

void lower_menu();

error Sanctions(user **users, const char *name, unsigned int number, int count_user, int current_id);

int main() {
    char flag[2], user_name[7];
    int current_id = -1, check = 0, login_command, count_command = 0, exit = 1, time, number;
    int reg, capacity = 2, user_count = 0;
    user *users = (user *) malloc (capacity * sizeof(user));
    if (!users) {
        printf("Memory error\n");
        return MEMORY_ERROR;
    }

    lower_menu();
    scanf("%d", &reg);

    while (exit){
        switch (reg) {
            case 1:
                registration(&users, &user_count, &capacity);

                lower_menu();
                scanf("%d", &reg);
                break;
            case 2:
                if (login(&users, &user_count, &current_id) != OK) {
                    lower_menu();
                    scanf("%d", &reg);
                    break;
                }

                while (check == 0 && users[current_id].sanctions != count_command) {
                    main_menu();
                    scanf("%d", &login_command);
                    ++count_command;

                    switch (login_command) {
                        case 1:
                            Time();
                            break;
                        case 2:
                            Date();
                            break;
                        case 3:
                            printf("Enter the time and the flag\n");
                            scanf("%d %s", &time, flag);
                            clear_input_buffer();
                            Howmuch(time, flag);
                            break;
                        case 4:
                            check = 1;
                            break;
                        case 5:
                            printf("Enter a username and a limit on the number of commands\n");
                            scanf("%s %d", user_name, &number);
                            clear_input_buffer();

                            if (Sanctions(&users, user_name, number, user_count, current_id) == FOUND) {
                                printf("Successful\n");
                            } else {
                                printf("Error\n");
                            }

                            break;
                        default:
                            printf("Try again\n");
                            break;
                    }
                }

                check = 0;
                count_command = 0;
                lower_menu();
                scanf("%d", &reg);
                break;
            case 3:
                exit = 0;
                break;
            default:
                printf("Try again\n");

                lower_menu();
                scanf("%d", &reg);
                break;
        }
    }

    free_users(users);
    return 0;
}

error check_correct_login(const char *login) {
    if (!login) {
        return MEMORY_ERROR;
    }

    size_t size = strlen(login), i;

    for (i = 0; i < size; ++i) {
        if (!isalnum(login[i])) {
            return INVALID_INPUT;
        }
    }

    return OK;
}

error check_correct_PIN(unsigned int Pin) {
    if (Pin > 100000) {
        return INVALID_INPUT;
    }

    return OK;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

error registration(user **users, int *count_of_user, int *capacity) {
    if (!users || !count_of_user || !capacity) {
        return MEMORY_ERROR;
    }

    unsigned int pass;
    int i;
    bool check = false;
    char nickname[7];
    user *realloc_user;

    if (*count_of_user == *capacity) {
        *capacity *= 2;
        realloc_user = (user *) realloc(*users, *capacity * sizeof (user));
        if (!realloc_user) {
            return MEMORY_ERROR;
        }
        *users = realloc_user;
    }

    do {
        printf("Write your login for registration: \n");
        scanf("%7s", nickname);
        clear_input_buffer();

        if (nickname[6] != '\0') {
            printf("Incorrect input. \n");
            continue;
        }

        for (i = 0; i < *count_of_user; ++i) {
            if (strcmp((*users)[i].login, nickname) == 0) {
                printf("This login already exists\n");
                return FOUND;
            }
        }

        if (check_correct_login(nickname) != OK) {
            printf("Incorrect input. \n");
        } else {
            check = true;
        }
    } while (check == false);
    check = false;

    do {
        printf("Write your password for registration: \n");
        scanf("%u", &pass);
        clear_input_buffer();
        if (check_correct_PIN(pass) != OK) {
            printf("Incorrect input. \n");
        } else {
            check = true;
        }
    } while (check == false);

    strcpy((*users)[*count_of_user].login, nickname);
    (*users)[*count_of_user].Pin_code = pass;
    (*users)[*count_of_user].sanctions = -3;
    printf("Registration is successful. Your data: \nLogin: %s\nPassword: %u\n", (*users)[*count_of_user].login, (*users)[*count_of_user].Pin_code);
    ++(*count_of_user);

    return OK;
}

void Time() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("Current time: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void Date() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("Current date: %04d-%02d-%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void free_users(user *users) {
    free(users);
}

error login(user **users, const int *count_of_users, int *id) {
    if (!users || !count_of_users) {
        return MEMORY_ERROR;
    }

    int i;
    char login[7];
    unsigned int pass;
    printf("Enter your login for authorization: ");
    scanf("%6s", login);
    clear_input_buffer();
    printf("Enter your Pin-code for authorization: ");
    scanf("%u", &pass);
    clear_input_buffer();

    for (i = 0; i < *count_of_users; ++i) {
        if ((*users)[i].Pin_code == pass && strcmp((*users)[i].login, login) == 0) {
            printf("Login successful\n");
            *id = i;
            return OK;
        }
    }

    printf("The user with this username or password was not found\n");

    return NOT_FOUND;
}

error Sanctions(user **users, const char *name, unsigned int number, int count_user, int current_id) {
    if (!users || !name) {
        return MEMORY_ERROR;
    }

    int i = 0;
    unsigned int answer;

    for (i = 0; i < count_user; ++i) {
        if (strcmp((*users)[i].login, name) == 0 && current_id != i) {
            printf("Write 12345 to confirm the action\n");
            scanf("%u", &answer);

            if (answer != 12345) {
                return INVALID_INPUT;
            }

            (*users)[i].sanctions = number;

            return FOUND;
        }
    }

    return NOT_FOUND;
}

void Howmuch(time_t past_time, char *flag) {
    if (!flag) {
        return;
    }

    if (strlen(flag) != 2 || flag[0] != '-') {
        printf("Invalid flag. Use -s, -m, -h, or -y.\n");
        return;
    }

    time_t now = time(NULL);
    double diff = difftime(now, past_time);
    if (flag[1] == 's')
        printf("%lf seconds\n", diff);
    else if (flag[1] == 'm')
        printf("%lf minutes\n", diff / 60);
    else if (flag[1] == 'h')
        printf("%lf hours\n", diff / 3600);
    else if (flag[1] == 'y')
        printf("%lf years\n", diff / (3600 * 24 * 365.25));
    else
        printf("Invalid flag. Use -s, -m, -h, or -y.\n");
}

void main_menu() {
    printf("------------------------------------------------\n");
    printf("Write the following action in text.\n1 - Output the current time\n2 - Output the current date\n"
           "3 - View the elapsed time\n4 - Logout\n5 - Set a limit on the number of operations\n");
    printf("------------------------------------------------\n");
}

void lower_menu() {
    printf("------------------------------------------------\n");
    printf("1 - sign up, 2 - log in, 3 - exit\n");
    printf("------------------------------------------------\n");
}