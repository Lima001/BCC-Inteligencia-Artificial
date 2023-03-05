import pygame
from math import cos, sin, pi, atan2
from random import randint, random
from time import sleep

# Declaração de Constantes

# Dimensões Tela
LARGURA = 800
ALTURA = 600

# Cores para renderização
BRANCO = (255,255,255)
VERDE = (0,255,0)
VERMELHO = (255,0,0)
AZUL = (0,0,255)
PRETO = (0,0,0)
LARANJA = (255,165,0)
ROXO = (177,156,217)

# Clock
VELOCIDADE = 60

# Define quantos pixels usar para desenhar as componentes da seta de um vetor
TAMANHO_SETA = 10


# Definição de funções auxiliares - relacionadas ao perceptron

# Função de ativiação usada pelo perceptron
def hardlim(x):
    return x>=0

# Gera um conjunto de tamanho n formado por uma tupla do tipo (p,t)
# onde 'p' consiste nos parâmetros (par-ordenado), e t a categoria desse
# par-ordenado.
# 
# Obs. I: O dataset é gerado garantindo que ele pode ser linerarmente separado
# no plano. Isso é feito através de geração induzida de uma reta que divide
# os pontos.
#
# Obs. II: Para esse exemplo, o bias sempre será 0
def gerar_dataset(n):
    # Matriz de pesos aleatória - relaciona-se com uma reta ortogonal que divide o plano
    w = (randint(-50,50), randint(-50,50))
    
    dataset = []
    
    for i in range(n):
        # Parâmetro gerado aleatóriamente
        p = (randint(-50,50), randint(-50,50))
        modulo_p = (p[0]**2 + p[1]**2)**(1/2)
        
        # Escalar aleatório 
        s = randint(50,250)
        
        # Criação de vetor unitário que é escalado por um número.
        # Dessa forma, é possível tornar os pontos mais esparsos
        # pelo plano
        p = (s*p[0]/modulo_p, s*p[1]/modulo_p)
        
        # Categoriza o parãmetro gerado
        if (w[0]*p[0] + w[1]*p[1]) >= 0:
            dataset.append((p,1))
        else:
            dataset.append((p,0))

    return dataset


# Definição de funções auxiliares - utilizadas para manipular vetores

# Retorna o vetor unitário de um vetor v
def get_unitario(v):
    modulo_v = (v[0]**2 + v[1]**2)**(1/2)
    return (v[0]/modulo_v, v[1]/modulo_v)

def somar_vetores(v1, v2):
    return (v1[0]+v2[0], v1[1]+v2[1])


# Definição de funções auxiliares - utilizadas para a computação gráfica

# Converte a coordenada gráfica para cartesiana
def retornar_cartesiano(v):
    return (LARGURA//2+v[0], ALTURA//2-v[1])

# Converte um ângulo em graus para radianos
def rad(angulo):
    return angulo*pi/180

# Desenha um vetor v na tela
def desenhar_vetor(v, cor=(BRANCO)):
    # Desenha a linha do vetor
    pygame.draw.line(tela, cor, retornar_cartesiano((0,0)), retornar_cartesiano(v))
    
    # Desenha a seta do vetor
    teta = atan2(v[1],v[0])

    # Vetor que estende o vetor principal - usado para calcular as setas
    ext = (cos(teta)*TAMANHO_SETA, sin(teta)*TAMANHO_SETA)
    
    # Cálculo das setas do vetor v
    s1 = (cos(rad(135))*ext[0]-sin(rad(135))*ext[1], sin(rad(135))*ext[0]+cos(rad(135))*ext[1])
    s2 = (cos(rad(-135))*ext[0]-sin(rad(-135))*ext[1], sin(rad(-135))*ext[0]+cos(rad(-135))*ext[1])

    # Desenhar as setas
    pygame.draw.line(tela, cor, retornar_cartesiano(v), retornar_cartesiano(somar_vetores(v,s1)))
    pygame.draw.line(tela, cor, retornar_cartesiano(v), retornar_cartesiano(somar_vetores(v,s2)))


# Inicialização de dados que serão usados pelo perceptron
# Matriz de pesos inicial com valores aleatórios
w = get_unitario((randint(-50,50), randint(-50,50)))
# dataset a ser usado para o treino
dataset = gerar_dataset(30)

# Inicialização de objetos utilizados para a interface gráfica
pygame.init()
tela = pygame.display.set_mode((LARGURA,ALTURA), vsync=1)
relogio = pygame.time.Clock()

executar = True
while executar:

    # Percorrer o dataset pegando um parâmetros e sua classificação por vez
    for p,t in dataset:

        tela.fill(PRETO)

        # Tratamento de eventos da interface
        for evento in pygame.event.get():
            if evento.type == pygame.QUIT:
                executar = False

        if not executar:
            break

        # Desenhar eixos do plano cartesiano
        pygame.draw.line(tela, BRANCO, (0,ALTURA//2), (LARGURA,ALTURA//2))
        pygame.draw.line(tela, BRANCO, (LARGURA//2,0), (LARGURA//2,ALTURA))

        # Desenhar todos os pontos do dataset, distinguindo-os por categoria
        # com cores diferentes
        for p_aux,t_aux in dataset:
            if t_aux:
                pygame.draw.circle(tela,VERMELHO,retornar_cartesiano(p_aux),5,3)
            else:
                pygame.draw.circle(tela,VERDE,retornar_cartesiano(p_aux),5,3)

        # Calcula e desenha o vetor (matriz de pesos) e a reta de divisão do plano 
        # (aqui apenas os vetores perpendiculares aos pesos são desenhados)
        wu = get_unitario(w)
        desenhar_vetor((wu[0]*50, wu[1]*50))
        
        w_perp1 = (wu[1]*300,-wu[0]*300)
        desenhar_vetor(w_perp1,LARANJA)
        
        w_perp2 = (-wu[1]*300,wu[0]*300)
        desenhar_vetor(w_perp2,LARANJA)

        pygame.display.update()

        # Fase de treino
        
        # "Seleciona" o ponto que está sendo processado pelo perceptron
        pygame.draw.circle(tela,BRANCO,retornar_cartesiano(p),15,1)

        # Cálculo da predição "a" e do erro da predição "e" em relação a categoria real
        a = hardlim(w[0]*p[0] + w[1]*p[1])
        e = t-a

        # Atualiza a matriz de pesos com os novos valores aprendidos
        w = (w[0]+e*p[0], w[1]+e*p[1])
        # print(w)

        # Calcula e desenha o novo vetor (matriz de pesos) e a reta de divisão do plano 
        # (aqui apenas os vetores perpendiculares aos pesos são desenhados)
        wu = get_unitario(w)
        desenhar_vetor((wu[0]*50, wu[1]*50),ROXO)
        
        w_perp1 = (wu[1]*300,-wu[0]*300)
        desenhar_vetor(w_perp1,ROXO)
        
        w_perp2 = (-wu[1]*300,wu[0]*300)
        desenhar_vetor(w_perp2,ROXO)

        relogio.tick(2)
        pygame.display.update()
        sleep(1)

pygame.quit()