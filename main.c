#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"structs.h"
#include"agent-functions.h"
#include"userfunctions.h"
int main(){
    RES_PRODS* res_products = create_new_res_products();
    RES_PRODS* rented_products = create_new_res_products();
    PRODUCTS* products= create_new_products();
    USERS_LISTE* users = create_users_liste();
    USER* current_user;
    get_users(users);
    get_products(products);
    update_prods_file(products);
    get_res_prods(res_products);
    get_rented_prods(rented_products);
    int choice;
    do{
        printf("\n 1 : Sign up as an agent \n 2 : Sign up as a user \n 3 : Quit \nYour choice: ");
        scanf("%d",&choice);
        getchar();
        switch (choice)
        {
        case 1:
            agent_menu(products,res_products,users,rented_products);
            break;
        case 2:
            user_menu(users,&current_user,products,res_products);
            break;
        case 3:
            break;
        default:
            printf("Invalid choice!");
            break;
        }
    }while(choice != 3);
}