#include <stdio.h>
#include <stdlib.h>

char name[100];
int num_tickets;
int category;

double sub_total;
double discount;

double standard_price = 150;
double premium_price = 250;
double recliner_price = 400;

void calculate_subTotal()
{
    if (num_tickets < 1 || num_tickets > 20)
    {
        printf("Please enter a valid number between 1 to 20\n");
        exit(0);
    }
    switch (category)
    {
    case 1:
        sub_total = num_tickets * standard_price;
        break;
    case 2:
        sub_total = num_tickets * premium_price;
        break;
    case 3:
        sub_total = num_tickets * recliner_price;
        break;
    default:
        printf("Invalid category");
    }
    
}
void calculate_discount()
{
    if (num_tickets >= 10)
    {
        discount = sub_total * 0.1;
    }
    else
    {
        discount = 0;
    }
}

void get_info()
{
    printf("Enter the Customer Name:");
    scanf("%s", name);
    printf("Enter the number of tickets you want to book(1-20):");
    scanf("%d", &num_tickets);
    while (num_tickets < 1 || num_tickets > 20)
    {
        printf("Please enter a valid number between 1 to 20\n");
        scanf("%d", &num_tickets);
    }
    printf("1.Standard(Rs.150/-)\n 2.Premium(Rs.250/-)\n 3.Recliner(Rs.400/-)\n Enter the Ticket Category:");
    scanf("%d", &category);
    while (category < 1 || category > 3)
    {
        printf("Please enter a valid category (1-3)\n");
        scanf("%d", &category);
    }
}

void display_bill()
{
    printf("--------------------------------------------\n");
    printf("Customer Name: %s\n", name);
    printf("Ticket category: %d\n", category);
    printf("Number of tickets booked: %d\n", num_tickets);
    printf("Sub Total: Rs %.2lf\n", sub_total);
    printf("Discount: Rs %.2lf\n", discount);
    printf("---------------------------------------------\n");
    printf("Final Bill: Rs %.2lf\n", sub_total - discount);
    printf("----------------------------------------------\n");
}
int main()
{
    get_info();
    calculate_subTotal();
    calculate_discount();
    display_bill();
    return 0;
}
