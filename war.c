#include <stdio.h>
#include <string.h>

// Definição da struct Territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    // Declaração do vetor para armazenar 5 territórios
    struct Territorio territorios[5];
    
    printf("=== CADASTRO DE TERRITORIOS ===\n");
    
    // Entrada de dados para os 5 territórios
    for(int i = 0; i < 5; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);
        
        // Leitura do nome
        printf("Digite o nome do territorio: ");
        scanf("%29s", territorios[i].nome);
        
        // Leitura da cor
        printf("Digite a cor do exercito: ");
        scanf("%9s", territorios[i].cor);
        
        // Leitura da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }
    
    // Exibição dos dados cadastrados
    printf("\n=== DADOS CADASTRADOS ===\n");
    for(int i = 0; i < 5; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
    }
    
    return 0;
}