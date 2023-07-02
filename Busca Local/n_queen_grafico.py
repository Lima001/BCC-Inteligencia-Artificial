import pygame

from random import randint
from copy import deepcopy

N = 8
MAX_ITER = 10

BRANCO = (255,255,255)
PRETO = (0,0,0)
VERMELHO = (255,0,0)

TAMANHO_RECT = 50
LARGURA = N*TAMANHO_RECT
ALTURA = N*TAMANHO_RECT

VELOCIDADE = 30

def desenhar_tabuleiro(estado, tabuleiro):
    for i in range(len(tabuleiro)):
        for j in range(len(tabuleiro)):
            if estado[i] == j:
                texto = fonte.render('Q', False, VERMELHO)
            else:
                texto = fonte.render(str(tabuleiro[i][j]), False, BRANCO)
            
            tela.blit(texto, (i*TAMANHO_RECT,j*TAMANHO_RECT))


def gerar_tabuleiro_vazio(n):
    return [[0 for j in range(n)] for i in range(n)]

def gerar_estado_inicial(n):
    return [randint(0,n-1) for i in range(n)]

def calcular_ataques(estado):
    ataques = 0

    for i in range(len(estado)):
        for j in range(i+1, len(estado)):
            if estado[i] == estado[j] or abs(estado[i]-estado[j]) == j-i:
                ataques += 1

    return ataques

def calcular_estados_vizinhos(estado):
    tabuleiro = gerar_tabuleiro_vazio(len(estado))

    for q in range(len(estado)):
        for i in range(len(estado)):
            estado_auxiliar = deepcopy(estado)
            estado_auxiliar[q] = i
            tabuleiro[i][q] = calcular_ataques(estado_auxiliar)

    return tabuleiro

def aplicar_busca_local(vizinhos):
    pos_menor = (-1,-1)
    menor = float('inf')

    for i in range(len(vizinhos)):
        for j in range(len(vizinhos)):
            if vizinhos[i][j] <= menor:
                menor = vizinhos[i][j]
                pos_menor = (i,j)

    return pos_menor

def aplicar_busca_local_multipla(vizinhos, referencia):
    pos_menor = []
    menor = referencia

    for i in range(len(vizinhos)):
        for j in range(len(vizinhos)):
            
            if vizinhos[i][j] < menor:
                menor = vizinhos[i][j]
                pos_menor = [(i,j)]
            
            elif vizinhos[i][j] == menor:
                pos_menor.append((i,j))

    if len(pos_menor) == 0:
        return []

    return pos_menor[randint(0,len(pos_menor)-1)]

def aplicar_hill_climbing(estado):    
    vizinhos = calcular_estados_vizinhos(estado)
    menor = aplicar_busca_local(vizinhos)
    estado[menor[1]] = menor[0]
    return estado

def aplicar_hill_climbing_side_moving(estado):    
    vizinhos = calcular_estados_vizinhos(estado)
    menor = aplicar_busca_local_multipla(vizinhos,calcular_ataques(estado))
    estado[menor[1]] = menor[0]
    return estado

def aplicar_first_choice_hill_climbing(estado):
    menor = (randint(0,N-1), randint(0,N-1))
    estado_aux = deepcopy(estado)
    estado_aux[menor[1]] = menor[0]

    if calcular_ataques(estado_aux) < calcular_ataques(estado):
        return estado_aux
    
    return estado

def aplicar_random_restart_hill_climbing(estado):
    estado = gerar_estado_inicial(N)
    for j in range(MAX_ITER):
        estado = aplicar_hill_climbing(estado)
    return estado

pygame.init()
pygame.font.init()
fonte = pygame.font.SysFont('Comic Sans MS', 30)
tela = pygame.display.set_mode((LARGURA,ALTURA), vsync=1)
relogio = pygame.time.Clock()

funcao = aplicar_first_choice_hill_climbing
estado = gerar_estado_inicial(N)
vizinhos = calcular_estados_vizinhos(estado)
print(calcular_ataques(estado))

executar = True
while executar:

    tela.fill(PRETO)

    for evento in pygame.event.get():
        if evento.type == pygame.QUIT:
            executar = False

        if evento.type == pygame.KEYDOWN:
            if evento.key == pygame.K_RIGHT:
                estado = funcao(estado)
                vizinhos = calcular_estados_vizinhos(estado)
                print(calcular_ataques(estado))
            
            if evento.key == pygame.K_UP:
                for i in range(MAX_ITER):
                    estado = funcao(estado)
                vizinhos = calcular_estados_vizinhos(estado)
                print(calcular_ataques(estado))

    if not executar:
        break

    desenhar_tabuleiro(estado,vizinhos)

    relogio.tick(VELOCIDADE)
    pygame.display.update()

pygame.quit()