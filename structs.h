#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int create_valid_id(int* array,int length){
    int new_id;
    int validId;
    do{
        validId = 1;
        new_id = rand() % 10000;
        for (int i = 0; i < length; i++)
        {
            if(new_id == array[i]){
                validId = 0;
            }
        }
    }while(validId == 0);
    return new_id;
}
typedef struct {
    char* name;
    float price;
    char delay_unity;
    int initial_quantity;
    int current_quantity;
    int* ids;
} PRODUCT;
PRODUCT* fill_product(char* name,int init_qte,int curr_qte,float price,char delay_unt){
    PRODUCT* prod = calloc(1,sizeof(PRODUCT));
    prod->name = calloc(strlen(name)+1,sizeof(char));
    strcpy(prod->name,name);
    prod->initial_quantity = init_qte;
    prod->current_quantity = curr_qte;
    prod->price = price;
    prod->delay_unity = delay_unt;
    prod->ids = calloc(init_qte,sizeof(int));
    return prod;
}
PRODUCT* create_new_product(){
    PRODUCT* prod = calloc(1,sizeof(PRODUCT));
    char name[80],delay_unity;
    printf("Enter the name of the new product :");
    scanf("%[^\n]%*c",name);
    printf("Enter the quantity of the product :");
    scanf("%d",&prod->initial_quantity);
    printf("Enter the price of the product :");
    scanf("%f",&prod->price);
    getchar();
    do{
        printf("per day or hour \n\t d : for day \n\t h : for hour \n Your Choice : ");
        prod->delay_unity = getchar();
    }while(
        prod->delay_unity != 'd' &&
        prod->delay_unity != 'h' && 
        prod->delay_unity != 'D' && 
        prod->delay_unity != 'H');
    prod->name = calloc(strlen(name) + 1,sizeof(char));
    strcpy(prod->name,name);
    prod->current_quantity = prod->initial_quantity;
    prod->ids = calloc(prod->initial_quantity,sizeof(int));
    for (int i = 0; i < prod->initial_quantity; i++)
    {
        prod->ids[i] = create_valid_id(prod->ids,i);
    }
    return prod;
}
typedef struct {
    PRODUCT** prods;
    int type_nb;
} PRODUCTS;
PRODUCTS* create_new_products(){
    PRODUCTS* prods = calloc(1,sizeof(PRODUCT));
    prods->type_nb = 0;
    prods->prods = NULL;
    return prods;
}
typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int min;
} DATE;
DATE new_date(int day,int month,int year,int hour,int min){
    DATE date;
    date.year = year;
    date.month = month;
    date.day = day;
    date.hour = hour;
    date.min = min;
    return date;
}
typedef struct {
    int reservation_id;
    int user_id;
    int product_id;
    char* prod_name;
    DATE reserved_at;
    DATE to_complete_at;
} RESERVED_PRODUCT;
RESERVED_PRODUCT* create_new_res_prod(int user_id,DATE reserved_at,DATE to_complete_at,char* prod_name,int reservation_id,int product_id){
    RESERVED_PRODUCT* res = malloc(sizeof(RESERVED_PRODUCT));
    res->user_id = user_id;
    res->reserved_at = reserved_at;
    res->to_complete_at = to_complete_at;
    res->prod_name = calloc(strlen(prod_name)+1,sizeof(char));
    strcpy(res->prod_name,prod_name);
    res->reservation_id = reservation_id;
    res->product_id = product_id;
    return res;
}
typedef struct {
    RESERVED_PRODUCT** products;
    int count;
} RES_PRODS;
RES_PRODS* create_new_res_products(){
    RES_PRODS* res_prods = calloc(1,sizeof(RES_PRODS));
    res_prods->count = 0;
    res_prods->products = NULL;
    return res_prods;
}
void add_reserved_product(RES_PRODS* res_products,RESERVED_PRODUCT* reserved){
    res_products->count++;
    res_products->products = (res_products->count == 1) ?
            calloc(1,sizeof(RESERVED_PRODUCT)) 
        :
            realloc(res_products->products,res_products->count * sizeof(RESERVED_PRODUCT));
    res_products->products[res_products->count - 1] = reserved; 
}
void update_res_prods_file(RES_PRODS* res_prods){
    FILE *file;
    file = fopen("./reserved.txt","w");
    if(file){
        fprintf(file,"number of reservations : %d\n",res_prods->count);
        fprintf(file,"------------------------------------------------------------------------------------------------\n");
        fprintf(file,"| NO | RES ID | USR ID | PRODUCT NAME         | PRD ID | RESERVATION DATE  | ENDS BEFORE       |\n");
        fprintf(file,"------------------------------------------------------------------------------------------------\n");
        for (int i=0; i < res_prods->count; i++)
        {
            fprintf(
                file,
                "| %02d | %06d | %06d | %-20s | %06d | %02d-%02d-%04d  %02d:%02d | %02d-%02d-%04d  %02d:%02d |\n",
                i+1,
                res_prods->products[i]->reservation_id,
                res_prods->products[i]->user_id,
                res_prods->products[i]->prod_name,
                res_prods->products[i]->product_id,
                res_prods->products[i]->reserved_at.day,
                res_prods->products[i]->reserved_at.month,
                res_prods->products[i]->reserved_at.year,
                res_prods->products[i]->reserved_at.hour,
                res_prods->products[i]->reserved_at.min,
                res_prods->products[i]->to_complete_at.day,
                res_prods->products[i]->to_complete_at.month,
                res_prods->products[i]->to_complete_at.year,
                res_prods->products[i]->to_complete_at.hour,
                res_prods->products[i]->to_complete_at.min);
            fprintf(file,"------------------------------------------------------------------------------------------------\n");
        }
    }
    fclose(file);
}
void show_products(PRODUCTS* products){
    printf("------------------------------------------------------------\n");
    printf("| NO | NAME                 | QUANTITY | PRICE     | DELAY |\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < products->type_nb; i++)
    {
        printf(
            "| %02d | %-20s | %03d/%03d  | %06.2f DH |   %c   |\n",
            i+1,
            products->prods[i]->name,
            products->prods[i]->current_quantity,
            products->prods[i]->initial_quantity,
            products->prods[i]->price,
            products->prods[i]->delay_unity
        );
        printf("------------------------------------------------------------\n");
    }
}
void update_rented_prods_file(RES_PRODS* rented_products){
    FILE *file;
    file = fopen("./rented.txt","w");
    if(file){
        fprintf(file,"number of rented products : %d\n",rented_products->count);
        fprintf(file,"------------------------------------------------------------------------------------------------\n");
        fprintf(file,"| NO | RNT ID | USR ID | PRODUCT NAME         | PRD ID | START DATE        | END DATE          |\n");
        fprintf(file,"------------------------------------------------------------------------------------------------\n");
        for (int i=0; i < rented_products->count; i++)
        {
            fprintf(
                file,
                "| %02d | %06d | %06d | %-20s | %06d | %02d-%02d-%04d  %02d:%02d | %02d-%02d-%04d  %02d:%02d |\n",
                i+1,
                rented_products->products[i]->reservation_id,
                rented_products->products[i]->user_id,
                rented_products->products[i]->prod_name,
                rented_products->products[i]->product_id,
                rented_products->products[i]->reserved_at.day,
                rented_products->products[i]->reserved_at.month,
                rented_products->products[i]->reserved_at.year,
                rented_products->products[i]->reserved_at.hour,
                rented_products->products[i]->reserved_at.min,
                rented_products->products[i]->to_complete_at.day,
                rented_products->products[i]->to_complete_at.month,
                rented_products->products[i]->to_complete_at.year,
                rented_products->products[i]->to_complete_at.hour,
                rented_products->products[i]->to_complete_at.min);
            fprintf(file,"------------------------------------------------------------------------------------------------\n");
        }
    }
    fclose(file);
}
void get_res_prods(RES_PRODS* res_prods){
    FILE* file;
    file = fopen("./reserved.txt","r");
    res_prods->count = 0;
    res_prods->products = NULL;
    if(file){
        int count,reservation_id,user_id,prod_id;
        DATE res_date,end_date;
        char prod_name[20];
        fscanf(file,"number of reservations : %d\n",&count);
        fscanf(file,"%*s\n");
        fscanf(file,"%*[^\n]%*c\n");
        fscanf(file,"%*s\n");
        for (int i = 0; i < count; i++)
        {
            fscanf(
                file,
                "| %*d | %d | %d | %s | %d | %d-%d-%d  %d:%d |  %d-%d-%d  %d:%d |\n",
                &reservation_id,
                &user_id,
                prod_name,
                &prod_id,
                &res_date.day,
                &res_date.month,
                &res_date.year,
                &res_date.hour,
                &res_date.min,
                &end_date.day,
                &end_date.month,
                &end_date.year,
                &end_date.hour,
                &end_date.min
            );
            fscanf(file,"%*s\n");
            add_reserved_product(
                res_prods,
                create_new_res_prod(
                    user_id,
                    res_date,
                    end_date,
                    prod_name,
                    reservation_id,
                    prod_id
                ));
        }
    }
}
void get_rented_prods(RES_PRODS* res_prods){
    FILE* file;
    file = fopen("./rented.txt","r");
    res_prods->count = 0;
    res_prods->products = NULL;
    if(file){
        int count,reservation_id,user_id,prod_id;
        DATE res_date,end_date;
        char prod_name[20];
        fscanf(file,"number of rented products : %d\n",&count);
        fscanf(file,"%*s\n");
        fscanf(file,"%*[^\n]%*c\n");
        fscanf(file,"%*s\n");
        for (int i = 0; i < count; i++)
        {
            fscanf(
                file,
                "| %*d | %d | %d | %s | %d | %d-%d-%d  %d:%d |  %d-%d-%d  %d:%d |\n",
                &reservation_id,
                &user_id,
                prod_name,
                &prod_id,
                &res_date.day,
                &res_date.month,
                &res_date.year,
                &res_date.hour,
                &res_date.min,
                &end_date.day,
                &end_date.month,
                &end_date.year,
                &end_date.hour,
                &end_date.min
            );
            fscanf(file,"%*s\n");
            add_reserved_product(
                res_prods,
                create_new_res_prod(
                    user_id,
                    res_date,
                    end_date,
                    prod_name,
                    reservation_id,
                    prod_id
                ));
        }
    }
}
typedef struct {
    char* name;
    char* cin;
    int id;
    char* mdp;
}USER;
typedef struct{
    USER** users;
    int count;
}USERS_LISTE;
USER* create_new_user(char* name,char*mdp,char* cin,int id){
    USER* user = calloc(1,sizeof(USER));
    user->name = calloc(strlen(name) + 1,sizeof(char));
    user->mdp = calloc(strlen(mdp) + 1,sizeof(char));
    user->cin = calloc(strlen(cin) + 1,sizeof(char));
    strcpy(user->name,name);
    strcpy(user->mdp,mdp);
    strcpy(user->cin,cin);
    user->id = id;
    return user;
}
USERS_LISTE* create_users_liste(){
    USERS_LISTE* liste = calloc(1,sizeof(USERS_LISTE));
    liste->count = 0;
    return liste;
}