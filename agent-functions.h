bool agent_sign_in()
{
	int a=0,i=0;
    char name[10],c=' ';
    char password[10],code[10];
    char user[10]="admin";
    char pass[10]="admin";

	system("cls");
    printf("    \xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd  Sign in as an agent   \xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd  ");
    printf("\n\n   ENTER USERNAME: ");
	scanf("%s", &name);
	printf(" \n   ENTER PASSWORD: ");
	while(i<10)
	{
	    password[i]=getch();
	    c=password[i];
	    if(c==13) break;
	    else printf("*");
	    i++;
	}
	password[i]='\0';
	bool signedIn = strcmp(name,"admin")==0 && strcmp(password,"admin")==0;
	char choice;
    if(signedIn) return true;
    else {
        printf("\nSomething went wrong , try again [y/n]");
        choice = getchar();
        getchar();
        if(choice == 'y') return agent_sign_in();
        else return false;
    }
}

void update_prods_file(PRODUCTS* prods){
    FILE* file;
    file = fopen("./product.txt","w");
    if(file){
        fprintf(file,"length : %d\n",prods->type_nb);
        for (int i = 0; i < prods->type_nb; i++)
        {
            fprintf(file,
                "{\n name : %s ,\n initial quantity : %d ,\n current quantity :%d ,\n price : %.2f ,\n delay : %c ,\n ids : [ ",
                prods->prods[i]->name,
                prods->prods[i]->initial_quantity,
                prods->prods[i]->current_quantity,
                prods->prods[i]->price,
                prods->prods[i]->delay_unity);
            for (int j = 0; j < prods->prods[i]->initial_quantity-1; j++)
            {
                fprintf(file,"%d , ",prods->prods[i]->ids[j]);
            }
            fprintf(file,"%d ]\n}\n",prods->prods[i]->ids[prods->prods[i]->initial_quantity-1]);
        }
        fclose(file);
    }
}
void add_product(PRODUCTS* prods,PRODUCT* prod){
    prods->type_nb++;
    prods->prods = (prods->type_nb == 1) ? 
        calloc(1,sizeof(PRODUCT))
    :
        realloc(prods->prods,prods->type_nb*sizeof(PRODUCT));
    prods->prods[prods->type_nb - 1] = prod;
    update_prods_file(prods);
}
void get_products(PRODUCTS* prods){
    FILE* file;
    file = fopen("./product.txt","r");
    prods->prods = NULL;
    prods->type_nb = 0;
    if(file){
        int count,init_qte,curr_qte;
        char name[80],delay_unt;
        float price;
        fscanf(file,"length : %d\n",&count);
        for (int i = 0; i < count; i++)
        {fscanf(
                file,
                "{\n name : %s ,\n initial quantity : %d ,\n current quantity : %d ,\n price : %f ,\n delay : %c ,\n ids : [ ",
                name,
                &init_qte,
                &curr_qte,
                &price,
                &delay_unt
            );
            add_product(prods,fill_product(name,init_qte,curr_qte,price,delay_unt));
            for (int j = 0; j < init_qte-1; j++)      
            {
                fscanf(file,"%d , ",&prods->prods[i]->ids[j]);
            }
            fscanf(file,"%d ]\n}\n",&prods->prods[i]->ids[init_qte-1]);
        }
    }
}
void view_reserved_products(RES_PRODS* res_prods){
    printf("------------------------------------------------------------------------------------------------\n");
    printf("| NO | RES ID | USR ID | PRODUCT NAME         | PRD ID | RESERVATION DATE  | ENDS BEFORE       |\n");
    printf("------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < res_prods->count; i++)
    {
        printf("| %02d | %06d | %06d | %-20s | %06d | %02d-%02d-%04d  %02d:%02d | %02d-%02d-%04d  %02d:%02d |\n",
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
        printf("------------------------------------------------------------------------------------------------\n");
    }
}
USER* find_user(USERS_LISTE* users,int id){
    for (int i = 0; i < users->count; i++)
    {
        if(users->users[i]->id == id){
            return users->users[i];
        }
    }
    return NULL;
}
PRODUCT* find_product(PRODUCTS* products,char* prod_name){
    for (int i = 0; i < products->type_nb; i++)
    {
        if(strcasecmp(products->prods[i]->name,prod_name) == 0){
            return products->prods[i];
        }
    }
    return NULL;
}
int find_product_id(PRODUCTS* products,char* prod_name){
    for (int i = 0; i < products->type_nb; i++)
    {
        if(strcasecmp(products->prods[i]->name,prod_name) == 0){
            return i;
        }
    }
    return -1;
}
void delete_res_product(RES_PRODS* res_prods,int index){
    res_prods->count--;
    for (int i = index; i < res_prods->count; i++)
    {
        res_prods->products[i] = res_prods->products[i+1];
    }
}
void printDate(DATE time){
    printf(
        "%02d/%02d/%04d  %02d:%02d \n",
        time.day,
        time.month,
        time.year,
        time.hour,
        time.min
        );
}
long date_to_min(DATE date){
    return date.min + 60*(date.hour + 24*(date.day + 30*(date.month + 12*date.year)));
}
void confirm_reservation(USERS_LISTE* users,RES_PRODS* res_prods,PRODUCTS* products,RES_PRODS* rented_prods){
    if(res_prods->count == 0){
        printf("Nothing reserved!\n");
        return;
    }
    int res_number;
    char choice;
    view_reserved_products(res_prods);
    do{
        choice = 'n';
        printf("Your choice : ");
        scanf("%d",&res_number);
        getchar();
        res_number--;
        if(res_number > res_prods->count || res_number < 0){
            printf("Invalid choice, try again [y/n] : ");
            if(choice == 'n') return;
        } else {
            time_t t = time(NULL);
            struct tm curr = *localtime(&t);
            DATE cur = new_date(curr.tm_mday,curr.tm_mon+1,curr.tm_year+1900,curr.tm_hour,curr.tm_min);
            DATE end = res_prods->products[res_number]->to_complete_at;
            long cur_time = date_to_min(cur);
            long end_time = date_to_min(end);
            if(end_time < cur_time){
                printf("Expired \n");
                printf("Keep going ? [y/n] : ");
                char c;
                scanf("%c",&c);
                if(c == 'n'){
                    int prod_id = find_product_id(products,res_prods->products[res_number]->prod_name);
                    products->prods[prod_id]->current_quantity++;
                    update_prods_file(products);
                    delete_res_product(res_prods,res_number);
                    update_res_prods_file(res_prods);
                    return;
                }
            }
            char cin[20],user_cin[20];
            int delay;
            printf("Enter the cin of the user :");
            scanf("%s",cin);
            strcpy(user_cin,find_user(users,res_prods->products[res_number]->user_id)->cin);
            if(strcasecmp(cin,user_cin) == 0){
                do{
                    printf("Enter the number of ");
                    if(
                        find_product(products,res_prods->products[res_number]->prod_name)->delay_unity == 'D' || 
                        find_product(products,res_prods->products[res_number]->prod_name)->delay_unity == 'd'){
                        printf("days : ");
                    } else {
                        printf("hours : ");
                    }
                    scanf("%d",&delay);
                } while(delay <= 0);
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                DATE start_date = new_date(tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
                if(find_product(products,res_prods->products[res_number]->prod_name)->delay_unity == 'D' || find_product(products,res_prods->products[res_number]->prod_name)->delay_unity == 'd'){
                    tm.tm_mday += delay;
                } else {
                    tm.tm_hour += delay;
                }
                mktime(&tm);
                DATE end_date = new_date(tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
                add_reserved_product(
                    rented_prods,
                    create_new_res_prod(
                        res_prods->products[res_number]->user_id,
                        start_date,
                        end_date,
                        res_prods->products[res_number]->prod_name,
                        res_prods->products[res_number]->reservation_id,
                        res_prods->products[res_number]->product_id));
                printf(
                    "The final price that should be given by the client is : %.2f DH\n",
                    delay*find_product(products,res_prods->products[res_number]->prod_name)->price);
                delete_res_product(res_prods,res_number);
                update_res_prods_file(res_prods);
                update_rented_prods_file(rented_prods);
            } else {
                printf("invalid cin");
            }
        }
    } while(choice == 'y');
}
void show_rented_products(RES_PRODS* rented_products){
    printf("------------------------------------------------------------------------------------------------\n");
    printf("| NO | RNT ID | USR ID | PRODUCT NAME         | PRD ID | START DATE        | END DATE          |\n");
    printf("------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < rented_products->count; i++)
    {
        printf(
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
        printf("------------------------------------------------------------------------------------------------\n");
    }
    
}
void restore_rented_product(RES_PRODS* rented_products,PRODUCTS* products){
    if(rented_products->count == 0){
        printf("---------------------------------------------------------------------------------------------------\n");
        printf("Everything is in stock!\n");
        printf("---------------------------------------------------------------------------------------------------\n");
        return;
    }
    show_rented_products(rented_products);
    int rented_number;
    char choice;
    do{
        choice = 'n';
        printf("Enter the number of the deserved product : ");
        scanf("%d",&rented_number);
        if(rented_number<=0 || rented_number > rented_products->count ){
            printf("Invalid Choice!\n Try again [y/n]? ");
            getchar();
            choice = getchar();
            if(choice == 'n') return;
        }
    }while(choice == 'y');
    time_t t = time(NULL);
    struct tm curr = *localtime(&t);
    DATE curr_date = new_date(curr.tm_mday,curr.tm_mon+1,curr.tm_year+1900,curr.tm_hour,curr.tm_min); 
    DATE end_date = rented_products->products[rented_number]->to_complete_at;
    long curr_time = date_to_min(curr_date);
    long end_time = date_to_min(end_date);
    if(end_time < curr_time){
        printf("Expired!\n");
        long delta = (curr_time - end_time) / 60;
        if(find_product(products,rented_products->products[rented_number]->prod_name)->delay_unity == 'D' || 
            find_product(products,rented_products->products[rented_number]->prod_name)->delay_unity == 'd'){
                delta /= 24;
            }
        float price = find_product(products,rented_products->products[rented_number]->prod_name)->price;
        printf("The client should pay %.2f DH \n",delta*price);
        printf("Finish the rent [y/n] ?  ");
        choice = getchar();
        if(choice == 'n') return;
    }
    delete_res_product(rented_products,rented_number);
    update_rented_prods_file(rented_products);
    int prod_index = find_product_id(products,rented_products->products[rented_number]->prod_name);
    products->prods[prod_index]->current_quantity++;
    update_prods_file(products);
}
void agent_menu(PRODUCTS* products,RES_PRODS* reserved_products,USERS_LISTE* users,RES_PRODS* rented_products){
    if(agent_sign_in()){
        int choice;
        do{
            printf("\n 1 : Add a new product \n 2 : Products state\n 3 : Reserved \n 4 : Rented \n 5 : Quit \nYour choice : ");
            scanf("%d",&choice);
            getchar();
            switch (choice)
            {
            case 1:
                add_product(products,create_new_product());
                break;
            case 2:
                show_products(products);
                break;
            case 3:
                confirm_reservation(users,reserved_products,products,rented_products);
                break;
            case 4:
                restore_rented_product(rented_products,products);
                break;
            default:
                break;
            }
        }while(choice != 5);
    }
}