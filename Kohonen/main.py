# Obs. Não foi implementado o decaimento da taxa de aprendizado, nem da função para cálculo da vizinhança

import pygame
from random import randint
import numpy as np
from math import pi, exp

# Declaração de Constantes

# Cores para renderização
BRANCO = (255,255,255)
PRETO = (0,0,0)

# Clock Pygame
VELOCIDADE = 30

# Grid
DIMENSAO_X = 120
DIMENSAO_Y = 120
TAMANHO_RECT = 5

# Tamanho tela
LARGURA = DIMENSAO_X*TAMANHO_RECT
ALTURA = DIMENSAO_Y*TAMANHO_RECT

# Taxa de aprendizado
ALPHA = 0.1

# Sigma usado na função gaussiana
SIGMA = DIMENSAO_X/10

# Quantidade de padrões a serem gerados para o aprendizado
n_padroes = 400

def gerar_matriz():
    '''
        Gera uma matriz com as dimensões do grid, onde cada elemento representa uma cor RGB aleatoriamente gerada
    '''
    return [[(randint(0,255), randint(0,255), randint(0,255)) for j in range(DIMENSAO_Y)] for i in range(DIMENSAO_X)]

def gerar_matriz_gauss(x,y):
    va = [[0 for j in range(y)] for i in range(x)]
    va[x // 2][y // 2] = 1
    return va

def dist(a,b):
    '''
        Retorna a distância euclidiana entre dois vetores de n dimensões
    '''
    acumulador = 0
    for i in range(len(a)):
        acumulador += (a[i] - b[i])**2

    return acumulador**(1/2)

def ganhador(matriz, padrao):
    '''
        Dada uma matriz, encontra o item mais próximo do padrão informado (entitulado na função como 'menor')
    '''

    # Inicializando variáveis - considerar menor inicialmente como primeiro item da Matriz
    menor = (0,0)
    d_menor = dist(matriz[menor[0]][menor[1]], padrao)

    for i in range(DIMENSAO_X):
        for j in range(DIMENSAO_Y):
            if dist(matriz[i][j], padrao) <= d_menor:
                menor = (i,j)
                d_menor = dist(matriz[menor[0]][menor[1]], padrao)

    return menor

def desenhar_rect(pos_inicial, cor):
    pygame.draw.rect(tela, cor, pygame.Rect(pos_inicial[0], pos_inicial[1], TAMANHO_RECT, TAMANHO_RECT))

def gerar_padroes(n):
    '''
        Gera n padrões RGB em uma lista
    '''
    return [(randint(0,255), randint(0,255), randint(0,255)) for i in range(n)]

def gauss(wx,wy,sigma,sx,sy):
    '''
        Função para cálculo da vizinhança gaussiana de um ponto localizado em (sx,sy) em relação 
        ao centro (wx,wy) 
    ''' 
    return exp(-((wx - sx)**2 + (wy-sy)**2) / (2*sigma**2))

# Gera os padrões e a matriz representado o grid que aprenderá os padrões
padroes = gerar_padroes(n_padroes)
m = gerar_matriz()

pygame.init()
tela = pygame.display.set_mode((LARGURA,ALTURA), vsync=1)
relogio = pygame.time.Clock()

# Contador para indicar qual padrão deve ser utilizado para aprendizagem
ci = 0

executar = True
while executar:
    
    # Identifica o item da matriz mais próximo do padrão em análise
    w = ganhador(m, padroes[ci])
    
    #print(w)
    #print(ci, m[w[0]][w[1]], end=" - ")
    
    # Aplica o espalhamento utilizando a função gaussiana
    for i in range(len(m)):
        for j in range(len(m[i])):
            r = m[i][j][0]
            g = m[i][j][1]
            b = m[i][j][2]
            gv = gauss(w[0],w[1],SIGMA,i,j)
            m[i][j] = (r+ALPHA*gv*(padroes[ci][0]-r), g+ALPHA*gv*(padroes[ci][1]-g), b+ALPHA*gv*(padroes[ci][2]-b)) 

    # print(ci, m[w[0]][w[1]])

    # Atualiza o índice para considerar o próximo padrão na próxima iteração (e retornar ao ínicio quando todos padrões tiverem sido iterados)
    ci = (ci + 1) % n_padroes

    tela.fill(PRETO)

    # Tratamento de eventos da interface
    for evento in pygame.event.get():
        if evento.type == pygame.QUIT:
            executar = False

    if not executar:
        break

    # Renderiza o grid utilizando os valores RGB armazenados na matriz
    for i in range(0,DIMENSAO_X):
        for j in range(0,DIMENSAO_Y):
            desenhar_rect((i*TAMANHO_RECT, j*TAMANHO_RECT), m[i][j])

    relogio.tick(VELOCIDADE)
    pygame.display.update()

pygame.quit()