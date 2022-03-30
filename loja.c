#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>

int old_SLOT;
int SLOT = 10;
struct Client
{   
    char name[30];
    int day;
    int month;
    int year;
    float purchases_month;
    float purchases_tot;
};

void init_list(struct Client *client);  //inicia a struct
int menu_select();  //exibe texto do menu e recebe escolha.
void add_client(struct Client *client);  //adiciona novo cliente
void remove_client(struct Client *client); //remove cliente
void att_purchases(struct Client *client);  //atualiza compras do cliente
void add_price(struct Client *client, int slot); //adiciona o valor de compra do cliente
void check_month(struct Client *client);    // zera os gastos do cliente no mes
int print_client(struct Client *client, int slot);  //printa o cliente
void search_client(struct Client *client);  //procura um cliente pelo nome
void best_client(struct Client *client);    //mostra o melhor cliente
int find_free(struct Client *client);   //encontra um slot vazio na struct
void relocate(struct Client *client);   //realoca a memoria
void back();    // volta para o menu
void clear();   //limpa a tela

int main()
{
    int choice;

    setlocale(LC_ALL, "Portuguese.Brazil");

    struct Client *client = (struct Client *) malloc(SLOT * sizeof(struct Client));
    init_list(client);

    while(1) {
        choice = menu_select();
        switch (choice) {
            case 1:
                add_client(client);
                break;
            case 2:
                att_purchases(client);
                break;
            case 3:
                search_client(client);
                break;
            case 4:
                best_client(client);
                break;
            case 5:
                remove_client(client);
                break;
            case 6:
                check_month(client);
                break;
            case 7:
                exit(1);
                free(client);
                break;
            default:
                exit(1);
        }
    }
    back();
}

//===============================================
//Inicializacao da lista
void init_list(struct Client *client){
    for (int i = 0; i < SLOT; i++) {
        client[i].name[0] = '\0';
        client[i].day = 0;
        client[i].month = 0;
        client[i].year = 0;
        client[i].purchases_month = 0;
        client[i].purchases_tot = 0;
    }
}
//===============================================
//Exibir o menu de selecao e receber opcao
int menu_select(){
    int t;

    clear();
    printf("\n>>>>>>>>> MENU <<<<<<<<<\n");
    printf(" 1 - Cadastrar novo cliente\n");
    printf(" 2 - Atualizar compras\n");
    printf(" 3 - Buscar cliente\n");
    printf(" 4 - Mostrar melhor cliente\n");
    printf(" 5 - Remover um cliente\n");
    printf(" 6 - Zerar montante de compras do mês\n");
    printf(" 7 - Sair\n");

    do {
        printf("\nDigite sua escolha: ");
        scanf("%d%*c", &t);
    } while (t<1 || t>7);

    return t;
}

//===============================================
//Adicionar novo cliente
void add_client(struct Client *client)
{
    int slot, i;
    clear();
    slot = find_free(client);
    if (slot == -1) {
        relocate(client);
        slot = find_free(client);
    }
    printf("Nome do(a) cliente: ");
    fgets(client[slot].name, 30, stdin);
    client[slot].name[strcspn(client[slot].name, "\n")] = '\0';
    printf("\nData de nascimento ( DD/MM/AAAA ): ");
    scanf("%d/%d/%d", &client[slot].day, &client[slot].month, &client[slot].year);
    printf("\nInserir produto?");
    printf("\n 1 - Sim");
    printf("\n 0 - Não\n");
    scanf("%d%*c", &i);
    if (i == 1)
    {
        add_price(client, slot);
    }
    clear();
    printf("\nCliente Cadastrado(a)!\n");
    print_client(client, slot);

    back();
}

//===============================================
//Remover cliente
void remove_client(struct Client *client)
{   
    int x, i, value;
    char nick[30];

    clear();

    printf("\nDigite o nome do(a) cliente a ser excluído(a): ");
    fgets(nick, 30, stdin);
    nick[strcspn(nick, "\n")] = '\0';

    for(i = 0; i < SLOT; i++){
        value = strcmp(nick, client[i].name);
        if (value == 0){
            printf("Cliente removido");
            client[i].name[0] = '\0';
            client[i].purchases_month = 0;
            client[i].purchases_tot = 0;
        }
    }
    if (x == 0){ printf("\nClient não encontrado(a)\n");}
    back();
}

//===============================================
//Atualizar total de compras
void att_purchases(struct Client *client)
{
    int i, value;
    float preco;
    char nick[30];

    clear();
    printf("Insira o nome do(a) cliente: ");
    fgets(nick, 30, stdin);
    nick[strcspn(nick, "\n")] = '\0';

    for (i = 0; i < SLOT; i++)
    {
        value = strcmp(nick, client[i].name);
        if(value == 0){ 
            add_price(client, i);
            print_client(client, i);
            back();
            return;
        }
    }
    if (i == SLOT){ printf("\nClient não encontrado(a)\n");}

    back();
}

//===============================================
//adiciona produtos
void add_price(struct Client *client, int slot)
{
    int i;
    float preco;
    do{
            printf("\nDigite o valor do produto\n");
            printf("(0 para sair): ");
            scanf("%f%*c", &preco);
            client[slot].purchases_month += preco;
            client[slot].purchases_tot += preco;
        }while (preco != 0);
}
//===============================================
//Checar mês para zerar compras
void check_month(struct Client *client)
{   
    clear();
    for (int i = 0; i < SLOT; i++){

        client[i].purchases_month = 0;
        }
    printf("Compras do mês zerado!");
    back();
}

//===============================================
//mostrar cliente
int print_client(struct Client *client, int slot)
{
    if (client[slot].name[0] == '\0'){ 
        return 0; };
    printf("\nNome: %s", client[slot].name);
    printf("\nData de nascimento: %d/%d/%d", client[slot].day, client[slot].month, client[slot].year);
    printf("\ncompras no mês: R$ %.2f", client[slot].purchases_month);
    printf("\nTotal de compras: R$ %.2f\n", client[slot].purchases_tot);
    return 1;
}

//===============================================
//Buscar cliente
void search_client(struct Client *client)
{   
    int i, x, value;
    char nick[30];

    clear();
    printf("Insira o nome do(a) cliente: ");
    fgets(nick, 30, stdin);
    nick[strcspn(nick, "\n")] = '\0';

    for (i = 0; i < SLOT; i++)
    {
        value = strcmp(nick, client[i].name);
        if(value == 0){ x = print_client(client, i); 
        back();
        return;}
    }
    if (i == SLOT){ printf("\nClient não encontrado(a)\n");}
    back();
}
//===============================================
//Mostrar melhor cliente
void best_client(struct Client *client)
{
    clear();
    float bigger= 0;
    int slot;
    for (int i = 0; i < SLOT; i++){
        if (bigger < client[i].purchases_tot){
            bigger = client[i].purchases_tot;
            slot = i;
        }
    }
    print_client(client, slot);
 back();
}

//===============================================
//Encontrar uma posicao livre
int find_free(struct Client *client){
    int i;
    for (i = 0; (client[i].name[0] && i<SLOT) ; i++);
    if (i == SLOT) return -1;
    return i;
}

//===============================================
//Realocar tamanho
void relocate(struct Client *client)
{   
    old_SLOT = SLOT;
    SLOT += 10;
    client = realloc(client, SLOT);
    for (int i = old_SLOT; i < SLOT; i++) {
        client[i].name[0] = '\0';
        client[i].day = 0;
        client[i].month = 0;
        client[i].year = 0;
        client[i].purchases_month = 0;
        client[i].purchases_tot = 0;
    }
}

//===============================================
//Volta para o Menu Principal
void back(){
    printf("\nPressione ENTER para voltar\n");
    getchar();
}

//===============================================
//Limpar terminal
void clear(){
    #if defined(__linux__) || defined(__unix__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}
