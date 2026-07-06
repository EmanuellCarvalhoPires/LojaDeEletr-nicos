#include <stdio.h>
#include <string.h>
#include <time.h>

#define ARQ_PROD "RegistroProdutos.txt"
#define ARQ_VENDA "RegistroVendas.txt"
#define ARQ_CLIENTE "RegistroClientes.txt"
#define ARQ_DEVOLUCAO "RegistroDevolucoes.txt"
#define MAX_VENDAS_DIA 50 // Restrição exigida: 50 vendas por dia

typedef struct Reg_Produto {
    int ID_Produto;
    char Nome_Produto[50];
    float valor_Produto;
} Produto;

typedef struct Reg_Cliente {
    char CPF_Cliente[12];
    char Nome_Cliente[50];
} Cliente;

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct Reg_Venda {
    int ID_venda;
    char Data_Venda[11];
    char CPF_Cliente[12];
    char Nome_Produto[50];
    int quantVendida;
    float valorTotal;
} Reg_Venda;

typedef struct Reg_Devolucao {
    int ID_Devolucao;
    int ID_Venda;
    int vezesDevolvido;
} Devolucao;

// Nova estrutura para agrupar as vendas no Relatório Diário
typedef struct {
    char Nome_Produto[50];
    int quantVendida;
    float valorTotal;
} AgregadoDiario;

// Nova estrutura para associar o mês ao seu faturamento no Relatório Anual
typedef struct {
    int mes;
    float faturamento;
} MesFaturamento;

void RegistroCliente() {
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
    FILE *file = fopen(ARQ_PROD, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    int ultimoID = 0;
    Produto auxiliar;

    FILE *file2 = fopen(ARQ_PROD, "r");
    if (file2 != NULL) {
        while (fscanf(file2, "%d,%[^,],%f\n", &auxiliar.ID_Produto, auxiliar.Nome_Produto, &auxiliar.valor_Produto) == 3) {
            ultimoID = auxiliar.ID_Produto;
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

void ListarProdutos() {
    FILE *file = fopen(ARQ_PROD, "r");
    if (file == NULL) {
        printf("Nenhum produto cadastrado ou erro ao abrir o arquivo!\n");
        return;
    }

    int id;
    char nome[50];
    float valor;

    printf("\n--- LISTA DE PRODUTOS ---\n");
    printf("ID | NOME\n");
    printf("-------------------------\n");

    while (fscanf(file, "%d,%[^,],%f\n", &id, nome, &valor) == 3) {
        printf("%02d | %s\n", id, nome);
    }

    fclose(file);
    printf("-------------------------\n");
}

int VerificarClienteExiste(char *cpfBuscado) {
    if (strlen(cpfBuscado) < 11) {
        printf("Erro: O CPF digitado e invalido. Ele deve conter exatamente 11 numeros!\n");
        return 0;
    }

    FILE *file = fopen(ARQ_CLIENTE, "r");
    if (file == NULL) return 0;

    char nomeTemp[50];
    char cpfTemp[12];

    while (fscanf(file, "%[^,],%s\n", nomeTemp, cpfTemp) == 2) {
        if (strcmp(cpfTemp, cpfBuscado) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int VerificarProdutoExiste(char *nomeBuscado) {
    FILE *file = fopen(ARQ_PROD, "r");
    if (file == NULL) return 0;

    int idTemp;
    char nomeTemp[50];
    float valorTemp;

    while (fscanf(file, "%d,%[^,],%f\n", &idTemp,nomeTemp,&valorTemp) == 3) {
        if (strcmp(nomeTemp, nomeBuscado) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

float ObterValorProduto(char *nomeBuscado) {
    FILE *file = fopen(ARQ_PROD, "r");
    if (file == NULL) {
        printf("Erro: Arquivo de produtos nao encontrado!\n");
        return -1.0;
    }

    int idTemp;
    char nomeTemp[50];
    float valorTemp;

    while (fscanf(file, "%d,%[^,],%f\n", &idTemp, nomeTemp, &valorTemp) == 3) {
        if (strcmp(nomeTemp, nomeBuscado) == 0) {
            fclose(file);
            return valorTemp;
        }
    }

    fclose(file);
    return -1.0;
}

int VerificarVendaExiste(int idVendaBuscada) {
    FILE *file = fopen(ARQ_VENDA, "r");
    if (file == NULL) return 0;

    Reg_Venda v;
    while (fscanf(file, "%i,%[^,],%[^,],%[^,],%i,%f\n", &v.ID_venda, v.Data_Venda, v.CPF_Cliente, v.Nome_Produto, &v.quantVendida, &v.valorTotal) == 6) {
        if (v.ID_venda == idVendaBuscada) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void RegistroVenda() {
    FILE *file = fopen(ARQ_VENDA, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    Data dataVenda;
    Reg_Venda VendaAux;

    int ultimoID = 0;
    FILE *file2 = fopen(ARQ_VENDA, "r");
    if (file2 != NULL) {
        while (fscanf(file2, "%i,%[^,],%[^,],%[^,],%i,%f\n", &VendaAux.ID_venda, VendaAux.Data_Venda, VendaAux.CPF_Cliente, VendaAux.Nome_Produto, &VendaAux.quantVendida, &VendaAux.valorTotal) == 6) {
            ultimoID = VendaAux.ID_venda;
        }
        fclose(file2);
    }

    printf("Digite o CPF do cliente: ");
    scanf(" %[^\n]", VendaAux.CPF_Cliente);
    int existeSimNao = VerificarClienteExiste(VendaAux.CPF_Cliente);
    if (existeSimNao != 1) {
        printf("Cliente nao cadastrado. Venda cancelada.\n");
        fclose(file);
        return;
    };

    ListarProdutos();
    printf("Digite o nome exato do produto vendido: ");
    scanf(" %[^\n]", VendaAux.Nome_Produto);
    existeSimNao = VerificarProdutoExiste(VendaAux.Nome_Produto);
    if (existeSimNao != 1) {
        printf("Produto nao cadastrado. Venda cancelada.\n");
        fclose(file);
        return;
    };

    float valorProduto = ObterValorProduto(VendaAux.Nome_Produto);

    printf("Quantas unidades desse produto foram vendidas? ");
    scanf("%i", &VendaAux.quantVendida);
    VendaAux.valorTotal = VendaAux.quantVendida * valorProduto;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    dataVenda.dia = tm.tm_mday;
    dataVenda.mes = tm.tm_mon + 1;
    dataVenda.ano = tm.tm_year + 1900;

    sprintf(VendaAux.Data_Venda, "%02d/%02d/%d", dataVenda.dia, dataVenda.mes, dataVenda.ano);
    VendaAux.ID_venda = ultimoID + 1;

    fprintf(file, "%i,%s,%s,%s,%i,%f\n",VendaAux.ID_venda,VendaAux.Data_Venda,VendaAux.CPF_Cliente,
        VendaAux.Nome_Produto, VendaAux.quantVendida, VendaAux.valorTotal);

    fclose(file);
    printf("Registro de venda criado com sucesso! (ID da Venda: %i)\n", VendaAux.ID_venda);
}

void RegistroDevolucao() {
    int idVendaAlvo;
    printf("\n--- REGISTRO DE DEVOLUCAO ---\n");
    printf("Digite o ID da venda que deseja devolver: ");
    scanf("%d", &idVendaAlvo);

    if (!VerificarVendaExiste(idVendaAlvo)) {
        printf("Erro: A venda com ID %d nao foi encontrada no sistema.\n", idVendaAlvo);
        return;
    }

    int maxIdDevolucao = 0;
    int maxVezesDevolvido = 0;
    Devolucao devAux;

    FILE *fileLeitura = fopen(ARQ_DEVOLUCAO, "r");
    if (fileLeitura != NULL) {
        while (fscanf(fileLeitura, "%d,%d,%d\n", &devAux.ID_Devolucao, &devAux.ID_Venda, &devAux.vezesDevolvido) == 3) {
            if (devAux.ID_Devolucao > maxIdDevolucao) {
                maxIdDevolucao = devAux.ID_Devolucao;
            }
            if (devAux.ID_Venda == idVendaAlvo) {
                if (devAux.vezesDevolvido > maxVezesDevolvido) {
                    maxVezesDevolvido = devAux.vezesDevolvido;
                }
            }
        }
        fclose(fileLeitura);
    }

    if (maxVezesDevolvido > 0) {
        int opcao;
        printf("\nATENCAO: Ja existe uma devolucao registrada para a Venda ID %d.\n", idVendaAlvo);
        printf("Para realizar uma nova devolucao do mesmo pedido, havera uma taxa de R$ 20,00.\n");
        printf("Deseja prosseguir e pagar a taxa? (1 - Sim / 0 - Nao): ");
        scanf("%d", &opcao);

        if (opcao == 0) {
            printf("Operacao de devolucao cancelada pelo usuario.\n");
            return;
        } else if (opcao != 1) {
            printf("Opcao invalida. Operacao cancelada.\n");
            return;
        }
    }

    FILE *fileEscrita = fopen(ARQ_DEVOLUCAO, "a");
    if (fileEscrita == NULL) {
        printf("Erro ao abrir o arquivo de devolucoes para escrita!\n");
        return;
    }

    int novoIdDevolucao = maxIdDevolucao + 1;
    int novasVezes = maxVezesDevolvido + 1;

    fprintf(fileEscrita, "%d,%d,%d\n", novoIdDevolucao, idVendaAlvo, novasVezes);
    fclose(fileEscrita);

    printf("\nDevolucao registrada com sucesso!\n");
    printf("ID Devolucao: %d | ID Venda: %d | Total de Devolucoes deste item: %d\n", novoIdDevolucao, idVendaAlvo, novasVezes);
}

// ---------------- FUNÇÕES DE RELATÓRIO ---------------- //

void GerarRelatorioDiario() {
    char dataBuscada[11];
    printf("\n--- RELATORIO DIARIO ---\n");
    printf("Digite a data desejada (DD/MM/AAAA): ");
    scanf("%s", dataBuscada);

    FILE *file = fopen(ARQ_VENDA, "r");
    if (file == NULL) {
        printf("Nenhum registro de venda encontrado.\n");
        return;
    }

    AgregadoDiario produtosVendidos[MAX_VENDAS_DIA];
    int qtdProdutosDiferentes = 0;
    float totalDiario = 0.0;
    int totalPedidos = 0;
    Reg_Venda v;

    // Lendo as vendas e agregando por tipo de produto
    while (fscanf(file, "%i,%[^,],%[^,],%[^,],%i,%f\n", &v.ID_venda, v.Data_Venda, v.CPF_Cliente, v.Nome_Produto, &v.quantVendida, &v.valorTotal) == 6) {
        if (strcmp(v.Data_Venda, dataBuscada) == 0 && totalPedidos < MAX_VENDAS_DIA) {
            int encontrado = 0;
            totalPedidos++;

            // Verifica se o produto já está na lista agregada
            for (int i = 0; i < qtdProdutosDiferentes; i++) {
                if (strcmp(produtosVendidos[i].Nome_Produto, v.Nome_Produto) == 0) {
                    produtosVendidos[i].quantVendida += v.quantVendida;
                    produtosVendidos[i].valorTotal += v.valorTotal;
                    encontrado = 1;
                    break;
                }
            }

            // Se for um produto novo no dia, adiciona à lista
            if (!encontrado) {
                strcpy(produtosVendidos[qtdProdutosDiferentes].Nome_Produto, v.Nome_Produto);
                produtosVendidos[qtdProdutosDiferentes].quantVendida = v.quantVendida;
                produtosVendidos[qtdProdutosDiferentes].valorTotal = v.valorTotal;
                qtdProdutosDiferentes++;
            }
            totalDiario += v.valorTotal;
        }
    }
    fclose(file);

    // Impressão nos moldes exigidos
    printf("\nData da venda | Produto | Qnt. Vendida | Valor vendido daquele produto\n");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < qtdProdutosDiferentes; i++) {
        printf("%s    | %-7s | %-12d | R$ %.2f\n",
            dataBuscada,
            produtosVendidos[i].Nome_Produto,
            produtosVendidos[i].quantVendida,
            produtosVendidos[i].valorTotal);
    }
    printf("------------------------------------------------------------------------\n");
    printf("Valor total no final da lista: R$ %.2f\n", totalDiario);
}

void GerarRelatorioMensal() {
    int anoBuscado;
    printf("\n--- RELATORIO MENSAL ---\n");
    printf("Digite o ano desejado (ex: 2026): ");
    scanf("%d", &anoBuscado);

    FILE *file = fopen(ARQ_VENDA, "r");
    if (file == NULL) {
        printf("Nenhum registro de venda encontrado.\n");
        return;
    }

    float faturamentoMensal[12] = {0}; // Zera faturamento dos 12 meses
    float faturamentoConsolidado = 0.0;
    Reg_Venda v;
    int dia, mes, ano;

    while (fscanf(file, "%i,%[^,],%[^,],%[^,],%i,%f\n", &v.ID_venda, v.Data_Venda, v.CPF_Cliente, v.Nome_Produto, &v.quantVendida, &v.valorTotal) == 6) {
        // Extrai as partes da data
        sscanf(v.Data_Venda, "%d/%d/%d", &dia, &mes, &ano);

        if (ano == anoBuscado && mes >= 1 && mes <= 12) {
            faturamentoMensal[mes - 1] += v.valorTotal;
            faturamentoConsolidado += v.valorTotal;
        }
    }
    fclose(file);

    printf("\nListagem de faturamento consolidado de %d:\n", anoBuscado);
    for (int i = 0; i < 12; i++) {
        printf("Mes %02d: R$ %.2f\n", i + 1, faturamentoMensal[i]);
    }
    printf("-----------------------------\n");
    printf("Valor total consolidado: R$ %.2f\n", faturamentoConsolidado);
}

void GerarRelatorioAnual() {
    int anoBuscado;
    printf("\n--- RELATORIO ANUAL ---\n");
    printf("Digite o ano desejado (ex: 2026): ");
    scanf("%d", &anoBuscado);

    FILE *file = fopen(ARQ_VENDA, "r");
    if (file == NULL) {
        printf("Nenhum registro de venda encontrado.\n");
        return;
    }

    MesFaturamento meses[12];
    for (int i = 0; i < 12; i++) {
        meses[i].mes = i + 1;
        meses[i].faturamento = 0.0;
    }
    float faturamentoAnual = 0.0;
    Reg_Venda v;
    int dia, mes, ano;

    while (fscanf(file, "%i,%[^,],%[^,],%[^,],%i,%f\n", &v.ID_venda, v.Data_Venda, v.CPF_Cliente, v.Nome_Produto, &v.quantVendida, &v.valorTotal) == 6) {
        sscanf(v.Data_Venda, "%d/%d/%d", &dia, &mes, &ano);

        if (ano == anoBuscado && mes >= 1 && mes <= 12) {
            meses[mes - 1].faturamento += v.valorTotal;
            faturamentoAnual += v.valorTotal;
        }
    }
    fclose(file);

    // Ordenação decrescente de faturamento (Bubble Sort Simples)
    for (int i = 0; i < 11; i++) {
        for (int j = i + 1; j < 12; j++) {
            if (meses[j].faturamento > meses[i].faturamento) {
                MesFaturamento temp = meses[i];
                meses[i] = meses[j];
                meses[j] = temp;
            }
        }
    }

    printf("\nTotal das vendas no ano de %d: R$ %.2f\n", anoBuscado, faturamentoAnual);
    printf("\nMeses em ordem decrescente de faturamento:\n");
    printf("------------------------------------------\n");
    for (int i = 0; i < 12; i++) {
        printf("%do lugar -> Mes %02d: R$ %.2f\n", i + 1, meses[i].mes, meses[i].faturamento);
    }
}

// ---------------- MAIN ---------------- //

int main() {
    int aux = -1;
    while (aux != 0) {
        printf("\n========================================\n"
           "        MENU DO SISTEMA DE VENDAS       \n"
           "========================================\n"
           "1. Registrar Produto\n"
           "2. Registrar Cliente\n"
           "3. Registrar Nova Venda (Pedido)\n"
           "4. Registrar Devolucao de Item\n"
           "5. Gerar Relatorio Diario\n"
           "6. Gerar Relatorio Mensal\n"
           "7. Gerar Relatorio Anual\n"
           "8. Listar Produtos Cadastrados\n"
           "0. Sair do Programa\n"
           "========================================\n"
           "Escolha uma opcao:  ");
        scanf("%d", &aux);

        switch (aux) {
            case 1:
                RegistroProduto();
                break;
            case 2:
                RegistroCliente();
                break;
            case 3:
                RegistroVenda();
                break;
            case 4:
                RegistroDevolucao();
                break;
            case 5:
                GerarRelatorioDiario();
                break;
            case 6:
                GerarRelatorioMensal();
                break;
            case 7:
                GerarRelatorioAnual();
                break;
            case 8:
                ListarProdutos();
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                if(aux != 0 && (aux < 1 || aux > 8)) {
                    printf("Opcao invalida. Tente novamente.\n");
                }
                break;
        }
    }

    return 0;
}