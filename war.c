#include <stdio.h>
#include <stdlib.h> // Para calloc e free
#include <string.h> // Para strcmp e strncpy
#include <time.h>   // Para srand

// Define o número fixo de territórios para a simulação (1 a 5)
#define NUM_TERRITORIOS 5

// Definição da Struct (melhor usar typedef para simplificar o uso com ponteiros)
typedef struct {
    char nome[30];
    char cor[10]; 
    int tropas;
} Territorio;

// --- Assinaturas das Funções ---
Territorio* alocarTerritorios();
void cadastrarTerritorios(Territorio* mapa);
void exibirMapa(const Territorio* mapa);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

// --- 1. FUNÇÃO PRINCIPAL (main) ---
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    Territorio* mapa = NULL;
    int escolhaAtacante, escolhaDefensor;

    printf("⚔️ Simulação de Batalha Estratégica - NÍVEL AVENTUREIRO ⚔️\n");
    printf("----------------------------------------------------------\n");

    // Alocação Dinâmica com calloc
    mapa = alocarTerritorios();
    if (mapa == NULL) {
        return 1;
    }

    // Cadastro e Inicialização
    cadastrarTerritorios(mapa);

    printf("\n🗺️ Mapa Inicial dos Territórios:\n");
    exibirMapa(mapa);

    // Laço Interativo para a Fase de Ataques
    printf("\n*** INÍCIO DA FASE DE ATAQUE (Escolha de 1 a %d) ***\n", NUM_TERRITORIOS);
    
    while (1) {
        printf("\n----------------------------------------------------------\n");
        printf("Escolha o índice do Território ATACANTE (1-%d, ou 0 para sair): ", NUM_TERRITORIOS);
        if (scanf("%d", &escolhaAtacante) != 1 || escolhaAtacante < 0 || escolhaAtacante > NUM_TERRITORIOS) {
            printf("❌ Entrada inválida. Tente novamente.\n");
            while (getchar() != '\n'); // Limpa buffer
            continue;
        }

        if (escolhaAtacante == 0) {
            printf("Fase de ataques encerrada.\n");
            break;
        }
        
        printf("Escolha o índice do Território DEFENSOR (1-%d): ", NUM_TERRITORIOS);
        if (scanf("%d", &escolhaDefensor) != 1 || escolhaDefensor < 1 || escolhaDefensor > NUM_TERRITORIOS) {
            printf("❌ Entrada inválida. Tente novamente.\n");
            while (getchar() != '\n'); // Limpa buffer
            continue;
        }

        if (escolhaAtacante == escolhaDefensor) {
            printf("⚠️ Um território não pode atacar a si mesmo! Tente novamente.\n");
            continue;
        }

        // Uso de Ponteiros: Converte índice (1-based para 0-based) para acessar a memória
        Territorio* atacante = mapa + (escolhaAtacante - 1);
        Territorio* defensor = mapa + (escolhaDefensor - 1);

        // Verifica se o atacante possui tropas
        if (atacante->tropas <= 0) {
             printf("🚫 %s não tem tropas para atacar!\n", atacante->nome);
             continue;
        }
        
        printf("\n🔥 %s (Exército %s) ATACA %s (Exército %s)!\n", 
               atacante->nome, atacante->cor, defensor->nome, defensor->cor);

        // Simulação do Ataque
        atacar(atacante, defensor);

        // Exibição Pós-Ataque
        printf("\n✅ Situação ATUALIZADA após o ataque:\n");
        exibirMapa(mapa);
    }
    
    // Liberação de Memória
    liberarMemoria(mapa);

    return 0;
}

// --- 2. IMPLEMENTAÇÃO DAS FUNÇÕES ---

/**
 * @brief Aloca dinamicamente o vetor de Territorio com calloc.
 * @return Ponteiro para o mapa alocado ou NULL em caso de falha.
 */
Territorio* alocarTerritorios() {
    // calloc(quantidade, tamanho_do_tipo)
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    
    if (mapa == NULL) {
        fprintf(stderr, "❌ Erro: Falha ao alocar memória com calloc.\n");
        return NULL;
    }
    printf("✅ Memória alocada dinamicamente para %d territórios (usando calloc).\n", NUM_TERRITORIOS);
    return mapa;
}

/**
 * @brief Inicializa os dados (nome, cor, tropas) dos Territorios.
 * @param mapa Ponteiro para o vetor de Territorio.
 */
void cadastrarTerritorios(Territorio* mapa) {
    printf("\n-- Cadastro/Inicialização Automática dos Territórios --\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        // Uso de Ponteiro: Acessando o elemento i com mapa + i
        Territorio* t = mapa + i; 

        // Atribuição de valores iniciais
        snprintf(t->nome, sizeof(t->nome), "Territorio_%d", i + 1);
        
        if (i < 3) {
            strncpy(t->cor, "Azul", sizeof(t->cor) - 1);
        } else {
            strncpy(t->cor, "Vermelho", sizeof(t->cor) - 1);
        }
        t->cor[sizeof(t->cor) - 1] = '\0';
        t->tropas = (i % 3) + 3; // Tropas iniciais entre 3 e 5
        
        printf("  [%d] %s (Cor: %s, Tropas: %d) cadastrado.\n", 
               i + 1, t->nome, t->cor, t->tropas);
    }
}

/**
 * @brief Exibe o mapa completo.
 * @param mapa Ponteiro constante para o vetor de Territorio.
 */
void exibirMapa(const Territorio* mapa) {
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        const Territorio* t = mapa + i; // Uso de Ponteiro
        // Exibe o índice 1-based para o usuário
        printf("  [%d] %s | Dono: %s | Tropas: %d\n", i + 1, t->nome, t->cor, t->tropas);
    }
}

/**
 * @brief Simula um ataque com dados aleatórios e aplica a lógica de conquista.
 * @param atacante Ponteiro para o Territorio atacante.
 * @param defensor Ponteiro para o Territorio defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Simulação de Dados de Ataque e Defesa (1 a 6)
    int dadoAtacante = (rand() % 6) + 1; 
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("   [Simulação] Dado Atacante (%s): %d | Dado Defensor (%s): %d\n", 
           atacante->cor, dadoAtacante, defensor->cor, dadoDefensor);

    // Lógica: Empates favorecem o atacante (dadoAtacante >= dadoDefensor)
    if (dadoAtacante >= dadoDefensor) {
        printf("   🏆 O %s VENCEU o ataque!\n", atacante->nome);
        
        // Se atacante vence → defensor perde 1 tropa
        if (defensor->tropas > 0) {
            defensor->tropas--;
            printf("   ➖ %s perdeu 1 tropa. Tropas restantes: %d.\n", defensor->nome, defensor->tropas);
        }

        // Se defensor perde todas → território é conquistado
        if (defensor->tropas == 0) {
            
            // Verifica se é uma conquista (cores diferentes)
            if (strcmp(atacante->cor, defensor->cor) != 0) {
                
                // Conquista: Mudar a cor do exército/dono
                strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
                defensor->cor[sizeof(defensor->cor) - 1] = '\0';
                
                // O território conquistado recebe 1 tropa para defesa mínima
                defensor->tropas = 1; 
                
                printf("   🚩 TERRITÓRIO CONQUISTADO! %s agora pertence ao exército %s.\n", 
                       defensor->nome, defensor->cor);
            } else {
                // Caso raro: se os territórios já eram do mesmo dono, apenas garante 1 tropa mínima.
                defensor->tropas = 1;
                printf("   ⚠️ %s já era do exército %s. Mantém o controle com 1 tropa.\n", defensor->nome, defensor->cor);
            }

        }

    } else {
        printf("   😔 O %s (Defensor) VENCEU a defesa! O ataque falhou.\n", defensor->nome);
        // Não há perda de tropas para nenhum lado quando o defensor vence estritamente.
    }
}


/**
 * @brief Libera a memória alocada dinamicamente.
 * @param mapa Ponteiro para o vetor alocado.
 */
void liberarMemoria(Territorio* mapa) {
    // Uso de free para liberar a memória alocada por calloc
    if (mapa != NULL) {
        free(mapa);
        printf("\n✅ Memória alocada liberada com sucesso (usando free).\n");
    }
}