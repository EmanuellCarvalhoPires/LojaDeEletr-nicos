#include <stdio.h>

#define ARQ_PROD "RegistroProdutos.txt"
#define ARQ_VENDA "RegistroVendas.txt"
#define ARQ_CLIENTE "RegistroClientes.txt"

typedef struct Reg_Produto {
    int ID_Produto;
    char Nome_Produto[50];
    float valor_Produto;
} Produto;

typedef struct Reg_Cliente {
    char CPF_Cliente[12]; // Coloquei 12 pois o CPF tem 11 números + 1 caractere invisível de fim de texto ('\0')
    char Nome_Cliente[50];
} Cliente;

void RegistroCliente() {
    //FUNÇÃO PARA CRIAR REGISTRO DO CLIENTE

    FILE *file = fopen(ARQ_CLIENTE, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    Cliente auxiliar;

    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]", auxiliar.Nome_Cliente);

    printf("Digite o CPF do cliente: ");
    scanf("%s", auxiliar.CPF_Cliente);


    fprintf(file, "%s,%s\n", auxiliar.Nome_Cliente, auxiliar.CPF_Cliente);

    fclose(file);
    printf("Registro criado com sucesso!\n");
}

void RegistroProduto() {
    //FUNÇÃO PARA CRIAR REGISTRO DO PRODUTO

    FILE *file = fopen(ARQ_PROD, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    int ultimoID = 0;
    Produto auxiliar;

    //DEFINIR ID DO PRODUTO BASEADO NO ID DO ÚLTIMO PRODUTO ADICIONADO NO ARQUIVO
    FILE *file2 = fopen(ARQ_PROD, "r");
    if (file2 != NULL) {
        while (fscanf(file2, "%d,%[^,],%f\n", &auxiliar.ID_Produto, auxiliar.Nome_Produto, &auxiliar.valor_Produto) == 3) {
            ultimoID = auxiliar.ID_Produto; // Vai substituindo pelo ID da linha atual
        }
        fclose(file2);
    }

    printf("Digite o nome do produto: ");
    scanf(" %[^\n]", auxiliar.Nome_Produto);

    printf("Digite o valor do produto: ");
    scanf("%f", &auxiliar.valor_Produto);
    auxiliar.ID_Produto = ultimoID + 1;

    fprintf(file, "%d,%s,%.2f\n", auxiliar.ID_Produto, auxiliar.Nome_Produto, auxiliar.valor_Produto);

    fclose(file);
    printf("Registro criado com sucesso!\n");
}


int main() {
    RegistroCliente();

    return 0;aa
}
