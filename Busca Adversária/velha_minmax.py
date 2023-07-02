from random import shuffle,randint
from copy import deepcopy

def gerar_pontuacao(estado,profundidade):
    if avaliar_vitoria(estado,1):
        return 10-profundidade
    elif avaliar_vitoria(estado,-1):
        return -10+profundidade
    else:
        return 0

def verificar_estado_final(estado):
    for i in range(len(estado)):
        for j in range(len(estado)):
            if estado[i][j] == 0:
                return False
    return True

def avaliar_vitoria(estado,agente):
    
    def verificar_colunas(estado,agente):
        for i in range(3):
            if estado[0][i] + estado[1][i] + estado[2][i] == agente*3:
                return True

        return False

    def verificar_linhas(estado,agente):
        for linha in estado:
            if sum(linha) == agente*3:
                return True

        return False

    def verificar_diagonais(estado,agente):
        diagonal1 = 0
        diagonal2 = 0

        tamanho = len(estado) -1

        for i in range(3):
            diagonal1 += estado[i][i]
            diagonal2 += estado[i][tamanho-i]

        return diagonal1 == agente * 3 or diagonal2 == agente * 3 

    return verificar_colunas(estado,agente) or verificar_linhas(estado,agente) or verificar_diagonais(estado,agente)

def minmax(estado,profundidade,maximizando=True):
    if verificar_estado_final(estado) or avaliar_vitoria(estado,1) or avaliar_vitoria(estado,-1):
        return gerar_pontuacao(estado,profundidade)
    
    if maximizando:
       
        melhor = -float('inf')

        for i in range(len(estado)):
            for j in range(len(estado)):
                
                if estado[i][j] == 0:
                    estado[i][j] = 1
                    pontuacao = minmax(deepcopy(estado),profundidade+1,False)
                    estado[i][j] = 0

                    if pontuacao > melhor:
                        melhor = pontuacao

        return melhor

    else:
        
        melhor = float('inf')

        for i in range(len(estado)):
            for j in range(len(estado)):
                
                if estado[i][j] == 0:
                    estado[i][j] = -1
                    pontuacao = minmax(deepcopy(estado),profundidade+1,True)
                    estado[i][j] = 0

                    if pontuacao < melhor:
                        melhor = pontuacao

        return melhor

def encontrar_melhor_movimento(estado):
    melhor = -float('inf')
    movimentos_possiveis = []

    for i in range(len(estado)):
        for j in range(len(estado)):
            if estado[i][j] == 0:
                estado[i][j] = 1
                pontuacao = minmax(deepcopy(estado),0,False)
                estado[i][j] = 0

                if pontuacao > melhor:
                    melhor = pontuacao
                    movimentos_possiveis = [(i,j)]

                elif pontuacao == melhor:
                    movimentos_possiveis.append((i,j,pontuacao))

    shuffle(movimentos_possiveis)
    return movimentos_possiveis[0]

def print_estado(estado):
    for i in range(len(estado)):
        for j in range(len(estado)):
            print(estado[i][j], end="\t")
        print()

def gerar_exemplo(n):
    estado = [[0 for j in range(3)] for i in  range(3)]
    contador = 0
    
    while contador != n:
        i, j = randint(0,2), randint(0,2)
        
        if estado[i][j] == 0:
            if contador%2 == 0:
                estado[i][j] = 1
            else:
                estado[i][j] = -1
            contador += 1

    print("Estado gerado: ")
    print_estado(estado)

    mover = encontrar_melhor_movimento(estado)
    estado[mover[0]][mover[1]] = 1

    print("Estado com movimento gerado: ")
    print_estado(estado)

def gerar_partida():
    estado = [[0 for j in range(3)] for i in range(3)]

    if randint(0,10)%2 == 0:
        vez = 1
    else:
        vez = -1

    while True:

        print("Tabuleiro Atual")
        print_estado(estado)
        print()
        print(f"Vez de {vez} jogar")

        if vez == -1:
            linha = int(input("Linha: "))
            coluna = int(input("Coluna: "))
            estado[linha][coluna] = -1
        
        else:
            movimento = encontrar_melhor_movimento(estado)
            estado[movimento[0]][movimento[1]] = 1

        print()
        vez*=-1

        if avaliar_vitoria(estado,1):
            vencedor = 1
            break
        elif avaliar_vitoria(estado,-1):
            vencedor = -1
            break
        elif verificar_estado_final(estado):
            vencedor = 0
            break


    print_estado(estado)

    if vencedor != 0:
        print(f"Vencedor foi {vencedor}!")
    else:
        print("Empate!")

if __name__ == "__main__":
    gerar_exemplo(4)
    #gerar_partida()