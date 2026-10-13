#include<stdio.h>
#include<stdlib.h>
double sub_total;
double discount;
double final_bill;
double standard_price=150;
double premium_price=250;
double recliner_price=400;

void book(int num_tickets,int category){
    if(num_tickets<1 || num_tickets>20){
        printf("Please enter a valid number between 1 to 20\n");
        exit(0);
    }
    switch(category){
        case 1:
            sub_total=num_tickets*standard_price;
            break;
        case 2:
            sub_total=num_tickets*premium_price;
            break;
        case 3:
            sub_total=num_tickets*recliner_price;
            break;
        default:
            printf("Invalid category");
            exit(0);
    }
    if(num_tickets>=5){
        discount=sub_total*0.1;
    }
    else{
        discount=0.00;
    }
    final_bill=sub_total-discount;

}
int main(){
    char name[100];
    int num_tickets;
    int category;
    printf("Enter the Customer Name:");
    scanf("%s", name);
    printf("Enter the number of tickets you want to book:(1-20)");
    scanf("%d", &num_tickets);
    printf("Enter the category of the ticket\n 1.Standard(Rs.150)\n 2.Premium(Rs.250)\n 3.Recliner(Rs.400)\n");
    scanf("%d", &category);

    book(num_tickets,category);

    printf("Customer Name: %s\n", name);
    printf("Ticket category: %d\n", category);
    printf("Number of tickets booked: %d\n", num_tickets);
    printf("Sub Total: Rs %.2lf\n", sub_total);
    printf("Discount: Rs %.2lf\n", discount);
    printf("Final Bill: Rs %.2lf\n", final_bill);
    return 0;
}
