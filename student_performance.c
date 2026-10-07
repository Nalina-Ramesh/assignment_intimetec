#include<stdio.h>

struct student{
    int roll_no;
    char name[50];
    float marks_1;
    float marks_2;
    float marks_3;
    float total;
    float average;
    char grade;
};

void calculate(struct student *s) {
    s->total = s->marks_1 + s->marks_2 + s->marks_3;
    s->average = s->total / 3.0;

    if(s->average >= 85) {
        s->grade = 'A';
    } else if(s->average >= 70) {
        s->grade = 'B';
    } else if(s->average >= 50) {
        s->grade = 'C';
    } else if(s->average >= 35) {
        s->grade = 'D';
    } else {
        s->grade = 'F';
    }
}

void display(struct student s) {
    printf("Roll No: %d\n", s.roll_no);
    printf("Name: %s\n", s.name);
    printf("Marks 1: %.2f\n", s.marks_1);
    printf("Marks 2: %.2f\n", s.marks_2);
    printf("Marks 3: %.2f\n", s.marks_3);
    printf("Total Marks: %.2f\n", s.total);
    printf("Average Marks: %.2f\n", s.average);
    printf("Grade: %c\n", s.grade);
    if(s.grade=='A'){
        printf("Performance:*****\n");
    }
    else if(s.grade=='B'){
        printf("Performance:****\n");
    }
    else if(s.grade=='C'){
        printf("Performance:***\n");
    }
    else if(s.grade=='D'){
        printf("Performance:**\n");
    }
    
}

int main(){
    int n;
    printf("enter the number of students: ");
    scanf("%d", &n);
    struct student s[n];
    for(int i=0; i<n; i++){
        
        printf("Enter roll number: ");
        scanf("%d", &s[i].roll_no);
        printf("Enter name: ");
        scanf("%s", s[i].name);
        printf("Enter marks for subject 1: ");
        scanf("%f", &s[i].marks_1);
        printf("Enter marks for subject 2: ");
        scanf("%f", &s[i].marks_2);
        printf("Enter marks for subject 3: ");
        scanf("%f", &s[i].marks_3);

        calculate(&s[i]);
    }
    for(int i=0; i<n; i++){
        printf("------------------------------\n");
        display(s[i]);
        
    }

}
