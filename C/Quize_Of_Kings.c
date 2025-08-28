#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

struct Question {
    char text[1000];
    char difficulty[10];
    char options[4][1000];
    int answer;
    int title;
    int question_id; //question number
} Question;

struct account {
    char username[100];
    int score;
    int answer;
    int correct_answers;
    int incorrect_answers;
    char user_id[100];
} user;

int random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

/*long open_file(char *filename, char *difficulty_mode, long position,bool question_find) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("This file could not open\n");
        exit(0);
    }
    
    // Move file pointer to the correct position
    fseek(fptr, position, SEEK_SET); //SEEK_SET starting of the file 

    // Read question text until '('
    char ch;
    int j = 0;
    while ((ch = getc(fptr)) != EOF) {
        if (ch == '(') {
            break;
        }
        Question.text[j++] = ch;
    }
    Question.text[j] = '\0';
    printf("%s", Question.text);

    // Read question difficulty
    if (fscanf(fptr, "difficulty:%9[^)]", Question.difficulty) != 1) {
        printf("Error reading difficulty from file\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }

    // Skip the remaining characters until a newline
    while (fgetc(fptr) != '\n');
    
    printf("%s\n", Question.difficulty);

    // Read options
    for (int i = 0; i < 4; i++) {
        if (fgets(Question.options[i], 1000, fptr) == NULL) {
            printf("Error reading options from file\n");
            fclose(fptr);
            exit(EXIT_FAILURE);
        }
        Question.options[i][strcspn(Question.options[i], "\n")] = '\0'; // Remove newline
        printf("%s\n", Question.options[i]); // Print the option to verify
    }

    // Read answer as an integer
    char answer_str[20];
    if (fgets(answer_str, 20, fptr) == NULL) {
        printf("Error reading answer from file\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    sscanf(answer_str, "answer:%d", &Question.answer);
    //printf("Answer: %d", Question.answer);

    // Get the current file position
    long new_position = ftell(fptr); //update position of file pointer

    fclose(fptr);

    return new_position;
}*/

char generate_id(char *username,char *user_id,int answers_num){
   //long int random_num=random(1,9999);
    sprintf(user_id,"%s_%d",username,answers_num); //sprintf is to mix and combine a string and an integer
    return *user_id;
}

long open_file(char *filename, char *difficulty_mode, long position,bool question_find) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("This file could not open\n");
        exit(0);
    }

    // Move file pointer to the correct position
    fseek(fptr, position, SEEK_SET);
    long start_position = position; //this part is for that if first high level of difficulty choose then go back to down level do not have problem

    while (!question_find) {
        // Reading the question text
        if (fgets(Question.text, 1000, fptr) == NULL) {
            if (start_position == 0) {
                break;
            }
            
            fseek(fptr, 0, SEEK_SET); //SEEK_SET starting of the file 
            start_position = 0;
            continue;
        }

        // Extracting difficulty
        if (sscanf(Question.text, "%*[^(](difficulty: %9[^)]", Question.difficulty) == 1) {
            if (strcmp(Question.difficulty, difficulty_mode) == 0) {
                question_find = 1;

                printf("Question: %s\n", Question.text);
                printf("Difficulty: %s\n", Question.difficulty);

                for (int i = 0; i < 4; i++) {
                    if (fgets(Question.options[i],1000, fptr) == NULL) {
                        printf("Error reading options from file\n");
                        fclose(fptr);
                        exit(0);
                    }
                    Question.options[i][strcspn(Question.options[i], "\n")] = '\0';
                    printf("%s\n", Question.options[i]);
                }

                if (fscanf(fptr, "answer: %d\n", &Question.answer) != 1) {
                    printf("Error reading answer from file\n");
                    fclose(fptr);
                    exit(0);
                }
            } 
            else {
                // Skip to the next question
                for (int i = 0; i < 5; i++) { // Skip the remaining part of this question
                    while (fgetc(fptr) != '\n' && !feof(fptr));
                }
            }
        }
    }

    if (!question_find) {
        printf("No more questions available with the difficulty level: %s\n", difficulty_mode);
        fclose(fptr);
        exit(0);
    }

    long new_position = ftell(fptr);  //update position of file pointer
    fclose(fptr);
    return new_position;
}


void write_in_file(char *user_id,char *username,int correct_answers,int incorrect_answers,int score){

     FILE *fptr_write=fopen(user_id,"w");
    if (fptr_write== NULL) {
        printf("This file could not open\n");
        exit(0);
    }
    //fprintf(fptr_read, "User_id: %s\n", user_id);
    fprintf(fptr_write, "Username: %s\n", username);
    fprintf(fptr_write, "Score: %d\n", score);
    fprintf(fptr_write, "Correct Answers: %d\n", correct_answers);
    fprintf(fptr_write, "Incorrect Answers: %d\n", incorrect_answers);

    fclose(fptr_write);

}

void read_from_file(char *user_id,char *username,int score,int correct_answers,int incorrect_answers) {
    FILE *fptr_read = fopen(user_id, "r");
    if (fptr_read != NULL) {
        fscanf(fptr_read, "Username: %s\n",user.username);
        fscanf(fptr_read, "Score: %d\n", &user.score);
        fscanf(fptr_read, "Correct Answers: %d\n", &user.correct_answers);
        fscanf(fptr_read, "Incorrect Answers: %d\n", &user.incorrect_answers);
        fclose(fptr_read);
    } 
    else {
        user.score = 0;
        user.correct_answers = 0;
        user.incorrect_answers = 0;
    }
}

int main() {
    srand(time(NULL)); //generate different random numbers

   int play_before=0;
   //char username[100];
    printf("What is your username? ");
    scanf("%s", &user.username);
    printf("How many questions did you answer until now? "); //sum of questions which answered and read file with this name username.total_answers
    scanf("%d",&play_before);
    if(play_before>0){
    *user.user_id=generate_id(user.username,user.user_id,play_before);
    //printf("%s",user.user_id);
    read_from_file(user.user_id,user.username,user.score,user.correct_answers,user.incorrect_answers);
    }
    
    printf("SCORE: %d\n", user.score);
    printf("CORRECT ANSWER: %d\n", user.correct_answers);
    printf("INCORRECT ANSWER: %d\n", user.incorrect_answers);


    int answers_number;
    printf("How many questions do you want to answer? ");
    scanf("%d", &answers_number);
    if (answers_number == 0) {
        int random_answers_number = random(1, 10);
        answers_number = random_answers_number;
        printf("%d\n", answers_number);
    }
    bool question_find=false;
    long position_1 = 0;
    long position_2 = 0;
    long position_3 = 0;
    for (int k = 0; k < answers_number; k++) {
        printf("Choose a title:\n1-HISTORY\n2-GEOGRAPHY\n3-TECHNOLOGY\n");
        scanf("%d", &Question.title);
        if (Question.title == 0) {
            Question.title = random(1, 3);
            printf("Random title: %d\n", Question.title);
        }

        int difficulty; //the number that user choose for difficulty
        printf("The difficulty of question?\n1-Easy\n2-Normal\n3-Hard\n");
        scanf("%d", &difficulty);

        if (difficulty == 0) {
            difficulty = random(1, 3);
            printf("Random difficulty: %d\n", difficulty);
        }
        char difficulty_mode[8]; //switch the user number for difficulty to the mode of that

        switch (difficulty) {
            case 1: strcpy(difficulty_mode, "easy"); break;
            case 2: strcpy(difficulty_mode, "normal"); break;
            case 3: strcpy(difficulty_mode, "hard"); break;
            default: break;
        }
        printf("%s\n", difficulty_mode);

        switch (Question.title) {
            case 1: position_1 = open_file("history.txt", difficulty_mode, position_1,question_find); break;
            case 2: position_2= open_file("geography.txt", difficulty_mode, position_2,question_find); break;
            case 3: position_3= open_file("technology.txt", difficulty_mode, position_3,question_find); break;
            default: printf("Invalid title\n"); break;
        }
        
        printf("Your answer: ");
        scanf("%d", &user.answer);
        if (user.answer == Question.answer) {
            printf("Correct!\n");
            user.correct_answers++;
            if(difficulty==1){
                user.score+=5;
            } //for each easy question score is 5 
            else if(difficulty==2){
                user.score+=10;
            } //for each normal question score is 10
            else if(difficulty==3){
                user.score+=15;
            } //for each hard question score is 15
        } 
        else {
            printf("Incorrect\n");
            printf("Answer: %d\n", Question.answer);
            user.incorrect_answers++;
        }
        printf("CORRECT ANSWERS: %d\n", user.correct_answers);
        printf("INCORRECT ANSWERS: %d\n", user.incorrect_answers);
        printf("SCORE:%d\n",user.score);
    }
    int total_answers=play_before+answers_number;
    *user.user_id=generate_id(user.username,user.user_id,total_answers);
    printf("%s",user.user_id);
   write_in_file(user.user_id,user.username,user.correct_answers,user.incorrect_answers,user.score);

    return 0;
}
