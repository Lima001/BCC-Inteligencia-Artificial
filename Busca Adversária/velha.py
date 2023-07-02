from velha_minmax import *
import pygame

BRANCO = (255,255,255)
PRETO = (0,0,0)
VERMELHO = (255,0,0)
AZUL = (0,0,255)

TAMANHO_RECT = 150
LARGURA = 3*TAMANHO_RECT
ALTURA = 4*TAMANHO_RECT

VELOCIDADE = 30

def desenhar_tabuleiro(estado,vez,vencedor,mapa_simbolos=["X","O"]):
    pygame.draw.line(tela, PRETO, (TAMANHO_RECT,0), (TAMANHO_RECT,3*TAMANHO_RECT))
    pygame.draw.line(tela, PRETO, (TAMANHO_RECT*2,0), (TAMANHO_RECT*2,3*TAMANHO_RECT))
    pygame.draw.line(tela, PRETO, (0,TAMANHO_RECT), (LARGURA,TAMANHO_RECT))
    pygame.draw.line(tela, PRETO, (0,TAMANHO_RECT*2), (LARGURA,TAMANHO_RECT*2))
    pygame.draw.line(tela, PRETO, (0,TAMANHO_RECT*3), (LARGURA,TAMANHO_RECT*3))

    if vez == 1:
        texto = fonte.render(f"Vez de {mapa_simbolos[0]}", False, PRETO)
    elif vez == -1:
        texto = fonte.render(f"Vez de {mapa_simbolos[1]}", False, PRETO)
    else:
        if vencedor == 1:
            texto = fonte.render(f"Vencedor: {mapa_simbolos[0]}!", False, PRETO)
        elif vencedor == -1:
            texto = fonte.render(f"Vencedor {mapa_simbolos[1]}!", False, PRETO)
        else:
            texto = fonte.render(f"Empate!", False, PRETO)


    tela.blit(texto, (0,3*TAMANHO_RECT))

    for i in range(len(estado)):
        for j in range(len(estado)):
            
            if estado[i][j] == 1:
                texto = fonte.render(mapa_simbolos[0], False, VERMELHO)
                tela.blit(texto, (i*TAMANHO_RECT,j*TAMANHO_RECT))
            
            elif estado[i][j] == -1:
                texto = fonte.render(mapa_simbolos[1], False, AZUL)
                tela.blit(texto, (i*TAMANHO_RECT,j*TAMANHO_RECT))


pygame.init()
pygame.font.init()
fonte = pygame.font.SysFont('Comic Sans MS', 100)
tela = pygame.display.set_mode((LARGURA,ALTURA), vsync=1)
relogio = pygame.time.Clock()

estado = [[0 for j in range(3)] for i in range(3)]
mapa_simbolos = ["X","O"]

if randint(0,10)%2 == 0:
    vez = 1
else:
    vez = -1

vencedor = None
executar = True
while executar:

    tela.fill(BRANCO)

    for evento in pygame.event.get():
        if evento.type == pygame.QUIT:
            executar = False
        
        if evento.type == pygame.MOUSEBUTTONUP and not vencedor and vez == -1:
            pos = pygame.mouse.get_pos()
            estado[pos[0]//TAMANHO_RECT][pos[1]//TAMANHO_RECT] = -1
            vez *= -1

    if not executar:
        break

    desenhar_tabuleiro(estado,vez,vencedor,mapa_simbolos)

    relogio.tick(VELOCIDADE)
    pygame.display.update()

    if vez != 0:

        if avaliar_vitoria(estado,1):
            vencedor = 1
            vez = 0
        
        elif avaliar_vitoria(estado,-1):
            vencedor = -1
            vez = 0
            
        elif verificar_estado_final(estado):
            vencedor = 0
            vez = 0             
        
        if vez == 1:
            movimento = encontrar_melhor_movimento(estado)
            estado[movimento[0]][movimento[1]] = 1
            vez*=-1

pygame.quit()