void update_file(USERS_LISTE* users_liste){
    FILE* file;
    file = fopen("./user.txt","w");
    if(file){
        fprintf(file,"length : %d\n",users_liste->count);
        for (int i = 0; i < users_liste->count; i++)
        {
            fprintf(file,
                "{\n name : %s ,\n cin : %s ,\n password : %s ,\n id: %d\n}\n",
                users_liste->users[i]->name,
                users_liste->users[i]->cin,
                users_liste->users[i]->mdp,
                users_liste->users[i]->id);        
        }
        fclose(file);
    } else {
        printf("files does not exist!\n");
    }
}
void display_user_info(USER* current_user) {
    printf(" \t\t\t **VOTRE PROFIL SE RESUME DANS CE QUI SUIT **\n\n");
    printf("\t\t\t\t\tName: %s\n", current_user->name);
    printf("\t\t\t\t\tCIN: %s\n", current_user->cin);
    printf("\t\t\t\t\tPassword: %s\n", current_user->mdp);
    printf("\t\t\t\t\tID: %d\n", current_user->id);
}

void append_user(USERS_LISTE* users_liste,USER* current_user){
    users_liste->count++;
    users_liste->users = (users_liste->count == 1) ? 
        calloc(users_liste->count,sizeof(USER)) 
    :
        realloc(users_liste->users,users_liste->count * sizeof(USER));
    users_liste->users[users_liste->count-1] = current_user;
}
void get_users(USERS_LISTE *users_liste){
    FILE* file;
    file = fopen("./user.txt","r");
    users_liste->count = 0;
    users_liste->users = NULL;
    if(file){
        char name[80],cin[20],mdp[80];
        int id,count;
        fscanf(file,"length : %d\n",&count);
        for (int i = 0; i < count; i++)
        {
            fscanf(
                file,
                "{\n name : %s ,\n cin : %s ,\n password : %s ,\n id: %d \n}\n",
                name,
                cin,
                mdp,
                &id);
            USER* new_user = create_new_user(name,mdp,cin,id);
            append_user(users_liste,new_user);
        }
    }
}
bool check_existing_id(USERS_LISTE* users_liste,int id){
    for (int i = 0; i < users_liste->count; i++)
    {
        if(users_liste->users[i]->id == id) {
            return false;
        }
    }
    return true;
}

void sign_up(USERS_LISTE* users_liste,USER** current_user){
    char name[80],cin[20],mdp[80];
    printf("Enter your name : ");
    scanf("%[^\n]%*c",name);
    printf("Enter your cin : ");
    scanf("%[^\n]%*c",cin);
    printf("Enter your password : ");
    scanf("%[^\n]%*c",mdp);
    int id;
    do{
        id = rand() % 10000;
    }while(!check_existing_id(users_liste,id));
    *(current_user) = create_new_user(name,mdp,cin,id);
    printf("Your id is : %d\n",id);
    append_user(users_liste,*(current_user));
    update_file(users_liste);
}

void reset_password(USERS_LISTE* users_liste, USER** current_user) {
  char cin[20];
  
  printf("Enter your CIN: ");
  scanf("%[^\n]%*c", cin);

  int user_index = -1;
  for (int i = 0; i < users_liste->count; i++) {
    if (strcmp(users_liste->users[i]->cin, cin) == 0) {
      user_index = i;
      break;
    }
  }

  if (user_index == -1) {
    printf("Invalid CIN.\n");
    return;
  }

  char* new_password;
  users_liste->users[user_index]->mdp = malloc(strlen(new_password));
  printf("Enter a new password: ");
  scanf("%[^\n]%*c", new_password);

  strcpy(users_liste->users[user_index]->mdp, new_password);
  printf("Password successfully reset.\n");
  update_file(users_liste);
}

void sign_in(USERS_LISTE* users_liste,USER** current_user){
  int id;
  char pass[80];
  printf("Enter your id : ");
  scanf("%d",&id);
  getchar();
  int index = -1;
  for(int i=0;i<users_liste->count;i++){
    if(users_liste->users[i]->id == id){
      index = i;
    }
  }
  if(index != -1){
    char c = 'y';
    do{
      printf("Enter your password: ");
      scanf("%[^\n]%*c",pass);
      if(strcmp(users_liste->users[index]->mdp,pass) == 0){
        printf("User signed in successfully!\n");
        *(current_user) = users_liste->users[index];
      } else {
        printf("Invalid password!\n try again? [y/n]: ");
        scanf("%c",&c);
        getchar();
      }
    } while(strcmp(users_liste->users[index]->mdp,pass)!=0 && c=='y');
  } else {
    char c;
    printf("Invalid id, check your id or maybe sign up if you don't have an account\n");
    printf("Try again or sign up, [y/n] : ");
    c = getchar();
    getchar();
    if(c == 'y'){
      sign_in(users_liste,current_user);
    } else {
      sign_up(users_liste,current_user);
    }
  }
  if (strcmp(users_liste->users[index]->mdp,pass) != 0) {
    char c;
    printf("You have entered the wrong password, Would you like to reset your password? [y/n] : ");
    c = getchar();
    getchar();
    if (c == 'y') {
      reset_password(users_liste, current_user);
    }
  }
}



bool bisextile(int year){
    return ((year%400==0) || (year%4==0 && year%100!=0));
}
bool check_existing_res_id(RES_PRODS* res_products,int id){
    for (int i = 0; i < res_products->count; i++)
    {
        if(res_products->products[i]->reservation_id == id) {
            return false;
        }
    }
    return true;
}
void ajouterJour(DATE *date) {
    #define NOMBRE_DE_JOURS_PAR_MOIS 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  static const int nombreDeJoursParMois[] = { NOMBRE_DE_JOURS_PAR_MOIS };
  date->day++;
  if (date->day > nombreDeJoursParMois[date->month - 1]) {
    date->day = 1;
    date->month++;
    if (date->month > 12) {
      date->month = 1;
      date->year++;
    }
  }
}
void reserve_product(USER* current_user,PRODUCTS* products,int number,RES_PRODS* res_products){
    if(products->prods[number]->current_quantity == 0){
        printf("Sold out!\n");
    } else {
        products->prods[number]->current_quantity--;
        update_prods_file(products);
        int reservation_identifier;
        do{
            reservation_identifier = rand() % 10000;
        }while(!check_existing_res_id(res_products,reservation_identifier));
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        DATE current_date = new_date(tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
        tm.tm_mday += 1;
        mktime(&tm);
        DATE res_date = new_date(tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
        add_reserved_product(res_products,create_new_res_prod(
            current_user->id,
            current_date,
            res_date,
            products->prods[number]->name,
            reservation_identifier,
            products->prods[number]->ids[products->prods[number]->current_quantity]));
        update_res_prods_file(res_products);
    }
}
void view_reserved_productsUser(RES_PRODS* res_prods,USER* current_user){
    RES_PRODS* res_prodsUser;
    res_prodsUser = create_new_res_products();
    printf("------------------------------------------------------------------------------------------------\n");
    printf("| NO | RES ID | PRODUCT NAME         | PRD ID | RESERVATION DATE  | ENDS BEFORE       |\n");
    printf("------------------------------------------------------------------------------------------------\n");
    int cpt=0;
    for (int i = 0; i < res_prods->count; i++)
    {   if(res_prods->products[i]->user_id==current_user->id){
            cpt+=1;
            add_reserved_product(res_prodsUser,res_prods->products[i]);
            printf("| %02d | %06d | %-20s | %06d | %02d-%02d-%04d  %02d:%02d | %02d-%02d-%04d  %02d:%02d |\n",
            cpt,
            res_prods->products[i]->reservation_id,
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
    int choix,res_del,prod_id;
    printf("1-update reservation\n");
    printf("2-delete reservation\n");
    scanf("%d",&choix);
    fflush(stdin);
    switch (choix)
    {
    case 2:
        printf("The resrvation NO:");
        scanf("%d",&res_del);
        prod_id=res_prodsUser->products[res_del-1]->product_id;
        for (int i = 0; i < res_prods->count; i++){
            if(res_prods->products[i]->product_id==prod_id){
                    delete_res_product(res_prods,i);
                    update_res_prods_file(res_prods);
                    return;
            }
        }
    case 1:
        printf("The resrvation NO:");
        scanf("%d",&res_del);
        prod_id=res_prodsUser->products[res_del-1]->product_id;
        for (int i = 0; i < res_prods->count; i++){
            if(res_prods->products[i]->product_id==prod_id){
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        DATE current_date = new_date(tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
        tm.tm_mday += 1;
        mktime(&tm);
        DATE res_date = new_date(tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900,tm.tm_hour,tm.tm_min);
        res_prods->products[i]->to_complete_at=res_date;
        update_res_prods_file(res_prods);
            }
                
        }  
        break;
    
    case 0:
        return;
        break;
}
}
void user_menu(USERS_LISTE* users_liste,USER** current_user,PRODUCTS* products,RES_PRODS* reserved_products){
    get_users(users_liste);
    int choice;
    do{
        printf("\n1 : Already have an account? Sign in\n2 : Don't have an account? Sign up\n Your choice: ");
        scanf("%d",&choice);
        getchar();
        switch (choice)
        {
        case 1:
            sign_in(users_liste,current_user);
            break;
        case 2:
            sign_up(users_liste,current_user);
            break;
        default:
            printf("Invalid choice!\n");
            break;
        }
    }while(choice != 1 && choice!=2);
    do{
        printf("\n 1 : Pick a product \n 2 : Reserved \n 3 : Profile \n 4 : Log out \n Your Choice : ");
        scanf("%d",&choice);
        getchar();
        switch (choice)
        {
        case 1:
        {
            int prdNb;
            char ready;
            show_products(products);
            do{
                ready = 'n';
                printf("Enter the number associated to the deserved product : ");
                scanf("%d",&prdNb);
                getchar();
                if(prdNb > products->type_nb){
                    printf("Invalid number!\n Would you like to retry again?[y/n] : ");
                    ready = getchar();
                } else {
                    reserve_product(*(current_user),products,prdNb-1,reserved_products);
                }
            }while(ready == 'y');
        }
            break;
        case 2:
            view_reserved_productsUser(reserved_products,*(current_user));      
            break;
        case 3:
        display_user_info(*(current_user));
    
            break;
        case 4:
            break;
        default:
            printf("Invalid choice!\n");
            break;
        }
    }while(choice != 4);
}