#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct  // For Making User Type Data 
{
    int id;
    char name[50];
    int age;
    char plan[20]; 
    double annual;
    double used;
} 
user;

void display_menu();
void add_subscriber();
void list_subscribers();
void process_claim();
void save_subscriber(user sub);
int get_next_id();
void load_subscribers(user subs[], int *count);
void update_subscriber_file(user[], int count);





int main() {
    int ch;

    do
    {
    
        
       printf("1. Add New Subscriber\n");
       printf("2. List All Subscribers\n");
       printf("3. Process Claim\n");
       printf("4. Exit\n");
       printf("Enter your choice: ");
        
       scanf("%d", &ch);
        

        switch (ch)
        {
            case 1:
                add_subscriber();
                break;
            case 2:
                list_subscribers();
                break;
            case 3:
                process_claim();
                break;
            case 4:
                printf("Exiting the Program!\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    }
    while (ch != 4);
      

    return 0;
}




int get_next_id()  //To set user ID according to inputs
{
    FILE *data = fopen("Data.txt", "r");
    user sub;
    int max_id = 0;

    if (data == NULL)   //Null means there is no data in the file
    {
        return 1; 
    }

    while (fread(&sub, sizeof(user), 1, data)) 
    {
        if (sub.id > max_id)
        {
            max_id = sub.id;
        }
    }

    fclose(data);
    return max_id + 1;
}


void save_subscriber(user sub) //For Saving New User Data
{
    FILE *data = fopen("Data.txt", "a"); 
    if (data == NULL) 
    {
        printf("Error opening file for writing!\n");
        return;
    }
    fwrite(&sub, sizeof(user), 1, data);
    fclose(data);
    printf("Subscriber saved successfully!\n");
}


void load_subscribers(user subs[], int *count) // For Accesing Old User Data
{
    FILE *data = fopen("Data.txt", "r");
    user sub;
    *count = 0;

    if (data == NULL) 
    {
        return; 
    }

    while (fread(&sub, sizeof(user), 1, data)) 
    {
        if (*count < 100) 
        {
            subs[*count] = sub;
            (*count)++;
        }
    }
    fclose(data);
}


void update_subscriber_file(user subs[], int count) //For Updating File With new User Data
{
    FILE *data = fopen("Data.txt", "w");
    if (data == NULL) 
    {
        printf("Error opening file for updating!\n");
        return;
    }
    fwrite(subs, sizeof(user), count, data);
    fclose(data);
}



void add_subscriber() // For Getting New User Data 
{
    user new_sub;

    new_sub.id = get_next_id(); // Will Update according to Data entry number
    
    printf("Subscriber ID: %d\n", new_sub.id);

    printf("Enter Name: ");
    scanf("%s", new_sub.name); 

    printf("Enter Age: ");
    scanf("%d", &new_sub.age);
 
    printf("Enter Plan: ");
    scanf(" %19s", new_sub.plan);

    printf("Enter Annual Claim Limit:");
    scanf("%lf", &new_sub.annual);

    new_sub.used = 0.0;

    save_subscriber(new_sub);
}

void list_subscribers() //For Printing all the users Data
{
    user subs[100];
    int count = 0;

    load_subscribers(subs, &count);

    if (count == 0) 
    {
        printf("No subscribers found\n");
        return;
    }

   
    printf("%-5s %-20s %-5s %-10s %-15s %s\n", 
           "ID", "Name", "Age", "Plan", "Annual Limit", "Used Limit");
    
    for (int i = 0; i < count; i++) 
    {
        printf("%-5d %-20s %-5d %-10s %-15.2lf %.2lf\n",
               subs[i].id, 
               subs[i].name, 
               subs[i].age, 
               subs[i].plan, 
               subs[i].annual, 
               subs[i].used);
    }
}

void process_claim() //To claim the process
{
    int id;
    double claim_amount;
    user subs[100];
    int count = 0;
    int found_index = -1;

    load_subscribers(subs, &count);

    
    printf("Enter Subscriber ID: ");
    scanf("%d", &id);

  
    for (int i = 0; i < count; i++) 
    {
        if (subs[i].id == id) 
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)  // For Wrong ID or non-Existing ID
    {
        printf("Subscriber with ID %d not found.", id);
        return;
    }

    printf("Subscriber: %s (ID: %d)\n", subs[found_index].name, subs[found_index].id);
    printf("Current Used Limit: $%.2lf\n", subs[found_index].used);
    printf("Remaining Limit: $%.2lf\n", subs[found_index].annual - subs[found_index].used);
    
    printf("Enter Claim Amount: $");
    scanf("%lf", &claim_amount);

   
    double remaining_limit = subs[found_index].annual - subs[found_index].used;
    
    if (claim_amount <= 0) 
    {
        printf("Claim amount must be positive.\n");
    } 
    else if (claim_amount <= remaining_limit) 
    {
        
        subs[found_index].used += claim_amount;
        printf("Claim APPROVED for $ %.2lf\n", claim_amount);
        printf("New Used Limit: $%.2lf\n", subs[found_index].used);
    } 
    else 
    {
       
        double covered_amount = remaining_limit;
        double subscriber_payment = claim_amount - remaining_limit;

        if (remaining_limit > 0) 
        {
            
            subs[found_index].used = subs[found_index].annual; 
            printf("Claim Partially Approved\n");
            printf("Covered Amount (Paid by Insurance): $%.2lf\n", covered_amount);
            printf("Subscriber's Responsibility (Must Pay): $%.2lf\n", subscriber_payment);
        } 
        else 
        {
            
            printf("Claim REJECTED: Annual limit of $%.2lf has been exhausted.\n", subs[found_index].annual);
            printf("Subscriber's Responsibility (Must Pay): $%.2lf\n", claim_amount);
        }
    }

    
    update_subscriber_file(subs, count); //To update the procces Amount of users
    
}