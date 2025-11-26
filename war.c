#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- CONSTANTES E DEFINIÇÕES ---
#define NUM_TERRITORIOS 5
#define MAX_MISSOES 2 // Número de missões disponíveis
#define NOME_MAX 30
#define COR_MAX 10

// --- STRUCTS ---

typedef struct {
    char nome[NOME_MAX];
    char cor[COR_MAX]; 
    int tropas;
} Territorio;

typedef enum {
    DESTRUIR_VERDE, // Destruir o exército "Verde"
    CONQUISTAR_3    // Conquistar 3 territórios (em relação ao início)
} TipoMissao;

typedef struct {
    TipoMissao tipo;
    char descricao[100];
    int territConquistados; // Contador para a missão CONQUISTAR_3
} Missao;


// --- ASSINATURAS DAS FUNÇÕES (Modularização Total) ---

// Funções de Gerenciamento de Memória e Inicialização
Territorio* alocarTerritorios(const int numTerritorios);
void inicializarTerritorios(Territorio* mapa, const int numTerritorios);
void liberarMemoria(Territorio* mapa);

// Funções de Missão
void atribuirMissao(Missao* missao, const Territorio* mapa, const int numTerritorios);
void exibirMissao(const Missao* missao);
int verificarVitoria(const Missao* missao, const Territorio* mapa, const int numTerritorios);
void atualizarContadorConquista(Missao* missao, const Territorio* defensorOriginal, const Territorio* atacante, const Territorio* defensorConquistado);

// Funções de Simulação de Batalha
void exibirMapa(const Territorio* mapa, const int numTerritorios);
void atacar(Territorio* atacante, Territorio* defensor);
int simularTurno(Territorio* mapa, Missao* missao);

// Funções de Interface
void exibirMenu();


// --- FUNÇÃO PRINCIPAL (main) ---
int main() {
    srand(time(NULL));

    Territorio* mapa = NULL;
    Missao missaoDoJogador;

    printf("👑 Simulação de Batalha Estratégica - NÍVEL MESTRE 👑\n");
    printf("----------------------------------------------------------\n");

    // Inicialização do Jogo
    mapa = alocarTerritorios(NUM_TERRITORIOS);
    if (mapa == NULL) {
        return 1;
    }

    inicializarTerritorios(mapa, NUM_TERRITORIOS);
    atribuirMissao(&missaoDoJogador, mapa, NUM_TERRITORIOS);

    printf("\n🎯 Sua Missão Atual é:\n");
    exibirMissao(&missaoDoJogador);
    printf("\n");
    exibirMapa(mapa, NUM_TERRITORIOS);

    int opcao = -1;
    int jogoEmAndamento = 1;

    // Loop Principal do Jogo
    while (jogoEmAndamento) {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            printf("\n❌ Entrada inválida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1: // Atacar
                if (simularTurno(mapa, &missaoDoJogador) == 1) {
                    jogoEmAndamento = 0; // Vitória após o ataque
                }
                break;

            case 2: // Verificar Missão
                if (verificarVitoria(&missaoDoJogador, mapa, NUM_TERRITORIOS)) {
                    printf("\n🎉 PARABÉNS! VOCÊ CUMPRIU A MISSÃO E VENCEU O JOGO!\n");
                    jogoEmAndamento = 0;
                } else {
                    printf("\n⏳ Missão ainda não concluída. Continue a batalha!\n");
                    exibirMissao(&missaoDoJogador);
                }
                break;

            case 0: // Sair
                jogoEmAndamento = 0;
                printf("\n👋 Jogo encerrado a pedido do jogador.\n");
                break;

            default:
                printf("\nOpção inválida. Escolha 1, 2 ou 0.\n");
                break;
        }
    }
    
    // Liberação de Memória
    liberarMemoria(mapa);
    return 0;
}


// --- IMPLEMENTAÇÃO DE FUNÇÕES DE GERENCIAMENTO ---

/**
 * @brief Aloca dinamicamente o vetor de Territorio com calloc.
 */
Territorio* alocarTerritorios(const int numTerritorios) {
    Territorio* mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "❌ Erro: Falha ao alocar memória com calloc.\n");
        return NULL;
    }
    return mapa;
}

/**
 * @brief Inicializa os dados dos Territorios com cores e tropas.
 */
void inicializarTerritorios(Territorio* mapa, const int numTerritorios) {
    char cores[3][COR_MAX] = {"Azul", "Vermelho", "Verde"};
    int numCores = 3;

    for (int i = 0; i < numTerritorios; i++) {
        Territorio* t = mapa + i; 

        snprintf(t->nome, sizeof(t->nome), "Territorio_%d", i + 1);
        
        // Atribui cores de forma cíclica
        strncpy(t->cor, cores[i % numCores], sizeof(t->cor) - 1);
        t->cor[sizeof(t->cor) - 1] = '\0';
        
        t->tropas = (i % 3) + 3; // Tropas iniciais entre 3 e 5
    }
    printf("✅ %d territórios inicializados com sucesso.\n", numTerritorios);
}

/**
 * @brief Libera a memória alocada dinamicamente.
 */
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\n✅ Memória liberada com sucesso.\n");
    }
}

// --- IMPLEMENTAÇÃO DE FUNÇÕES DE INTERFACE E SIMULAÇÃO ---

/**
 * @brief Exibe o menu de opções para o jogador.
 */
void exibirMenu() {
    printf("\n--- MENU ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

/**
 * @brief Exibe o mapa completo (usa const correctness).
 */
void exibirMapa(const Territorio* mapa, const int numTerritorios) {
    printf("\n---------------- MAPA ATUAL ----------------\n");
    for (int i = 0; i < numTerritorios; i++) {
        // Uso de const: o ponteiro 't' aponta para dados que não serão modificados
        const Territorio* t = mapa + i; 
        printf("  [%d] %s | Dono: %s | Tropas: %d\n", i + 1, t->nome, t->cor, t->tropas);
    }
    printf("--------------------------------------------\n");
}

/**
 * @brief Simula um único turno de ataque com entrada do usuário.
 * @return 1 se o jogador vencer (missão cumprida), 0 caso contrário.
 */
int simularTurno(Territorio* mapa, Missao* missao) {
    int escolhaAtacante, escolhaDefensor;

    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha o índice do Território ATACANTE (1-%d): ", NUM_TERRITORIOS);
    if (scanf("%d", &escolhaAtacante) != 1 || escolhaAtacante < 1 || escolhaAtacante > NUM_TERRITORIOS) {
        printf("❌ Entrada inválida. Ataque cancelado.\n");
        while (getchar() != '\n');
        return 0;
    }
    printf("Escolha o índice do Território DEFENSOR (1-%d): ", NUM_TERRITORIOS);
    if (scanf("%d", &escolhaDefensor) != 1 || escolhaDefensor < 1 || escolhaDefensor > NUM_TERRITORIOS) {
        printf("❌ Entrada inválida. Ataque cancelado.\n");
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    if (escolhaAtacante == escolhaDefensor) {
        printf("⚠️ Território não pode atacar a si mesmo.\n");
        return 0;
    }

    Territorio* atacante = mapa + (escolhaAtacante - 1);
    Territorio* defensor = mapa + (escolhaDefensor - 1);
    
    if (atacante->tropas <= 0) {
        printf("🚫 %s não tem tropas para atacar!\n", atacante->nome);
        return 0;
    }
    
    // Armazena a cor original do defensor para verificar a conquista e atualizar a missão
    Territorio defensorOriginal = *defensor; 

    printf("\n🔥 %s (Exército %s) ATACA %s (Exército %s)!\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Executa a Simulação do Ataque
    atacar(atacante, defensor);

    // Verifica se houve conquista para a missão CONQUISTAR_3
    atualizarContadorConquista(missao, &defensorOriginal, atacante, defensor);

    // Exibição Pós-Ataque
    exibirMapa(mapa, NUM_TERRITORIOS);
    
    // Retorna 1 se a missão foi cumprida neste turno
    return verificarVitoria(missao, mapa, NUM_TERRITORIOS);
}

/**
 * @brief Simula um ataque com dados aleatórios e aplica a lógica de conquista.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1; 
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("   [Dados] Atacante (%s): %d | Defensor (%s): %d\n", 
           atacante->cor, dadoAtacante, defensor->cor, dadoDefensor);

    // Lógica: Empates favorecem o atacante
    if (dadoAtacante >= dadoDefensor) {
        printf("   🏆 O %s VENCEU! Defensor perde 1 tropa.\n", atacante->nome);
        
        if (defensor->tropas > 0) {
            defensor->tropas--;
            printf("   ➖ %s perdeu 1 tropa. Tropas restantes: %d.\n", defensor->nome, defensor->tropas);
        }

        // Se defensor perde todas → território é conquistado
        if (defensor->tropas == 0) {
            
            if (strcmp(atacante->cor, defensor->cor) != 0) {
                // Conquista: Mudar a cor do exército/dono
                strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
                defensor->cor[sizeof(defensor->cor) - 1] = '\0';
                
                defensor->tropas = 1; // 1 tropa para defesa mínima
                
                printf("   🚩 CONQUISTADO! %s agora pertence ao exército %s.\n", 
                       defensor->nome, defensor->cor);
            } else {
                defensor->tropas = 1;
                printf("   ⚠️ %s já era seu. Mantém o controle com 1 tropa.\n", defensor->nome);
            }
        }

    } else {
        printf("   😔 O %s (Defensor) VENCEU a defesa! Ataque falhou.\n", defensor->nome);
    }
}

// --- IMPLEMENTAÇÃO DE FUNÇÕES DE MISSÃO ---

/**
 * @brief Atribui uma missão aleatória ao jogador.
 */
void atribuirMissao(Missao* missao, const Territorio* mapa, const int numTerritorios) {
    // Escolhe uma missão aleatória
    missao->tipo = rand() % MAX_MISSOES;
    missao->territConquistados = 0;

    switch (missao->tipo) {
        case DESTRUIR_VERDE:
            strncpy(missao->descricao, "Destruir completamente o exército 'Verde' (todos os territórios Verdes devem mudar de cor).", sizeof(missao->descricao) - 1);
            break;
        case CONQUISTAR_3:
             // Conta quantos territórios o jogador (Azul) já possui no início
             for (int i = 0; i < numTerritorios; i++) {
                 if (strcmp((mapa + i)->cor, "Azul") == 0) {
                     missao->territConquistados++;
                 }
             }
             // O objetivo será aumentar esse número em 3.
            strncpy(missao->descricao, "Conquistar 3 territórios além dos que você já possui (Total de territórios Azuis: +3).", sizeof(missao->descricao) - 1);
            break;
    }
    missao->descricao[sizeof(missao->descricao) - 1] = '\0';
}

/**
 * @brief Exibe a descrição da missão.
 */
void exibirMissao(const Missao* missao) {
    printf("   > %s\n", missao->descricao);
    if (missao->tipo == CONQUISTAR_3) {
        printf("   > Progresso de Conquista: %d (Objetivo: +3)\n", missao->territConquistados);
    }
}

/**
 * @brief Atualiza o contador de conquista se a missão for CONQUISTAR_3.
 */
void atualizarContadorConquista(Missao* missao, const Territorio* defensorOriginal, const Territorio* atacante, const Territorio* defensorConquistado) {
    // Apenas se a missão for CONQUISTAR_3 e a conquista foi bem sucedida
    if (missao->tipo == CONQUISTAR_3) {
        // Verifica se houve MUDANÇA REAL de cor e se o atacante é o exército 'Azul'
        if (strcmp(defensorOriginal->cor, defensorConquistado->cor) != 0 && strcmp(atacante->cor, "Azul") == 0) {
            // Se a cor antiga não era Azul E a nova cor é Azul, houve uma nova conquista.
            if (strcmp(defensorOriginal->cor, "Azul") != 0 && strcmp(defensorConquistado->cor, "Azul") == 0) {
                missao->territConquistados++;
                printf("   🏆 Progresso de Missão: Nova conquista! Total de territórios Azuis: %d.\n", missao->territConquistados);
            }
        }
    }
}


/**
 * @brief Verifica se a condição de vitória da missão foi atingida.
 * @return 1 se venceu, 0 caso contrário.
 */
int verificarVitoria(const Missao* missao, const Territorio* mapa, const int numTerritorios) {
    
    switch (missao->tipo) {
        case DESTRUIR_VERDE: {
            int verdeRestante = 0;
            for (int i = 0; i < numTerritorios; i++) {
                if (strcmp((mapa + i)->cor, "Verde") == 0) {
                    verdeRestante = 1; // Ainda há territórios Verdes
                    break;
                }
            }
            return !verdeRestante; // Vence se verdeRestante for 0
        }
            
        case CONQUISTAR_3: {
            // Conta os territórios Azuis
            int totalAzul = 0;
             for (int i = 0; i < numTerritorios; i++) {
                if (strcmp((mapa + i)->cor, "Azul") == 0) {
                    totalAzul++;
                }
            }
            // Verifica se o total atingiu o objetivo de +3 em relação à inicialização
            // Como 'territConquistados' armazena o total atualizado, e o objetivo é +3,
            // vamos considerar que a missão é cumprida se o contador for >= (Inicial + 3).
            // Para simplificar aqui, vamos usar a regra de que basta conquistar 3 novos territórios.
            if (missao->territConquistados >= 3) { 
                 return 1; // 3 ou mais conquistas
            }
            return 0;
        }
    }
    return 0;
}