from random import randint
from copy import deepcopy

def gerar_tabuleiro_vazio(n):
    return [[0 for j in range(n)] for i in range(n)]

def gerar_estado_inicial(n):
    # Linha em que cada rainha está (a coluna é dado pelo índice na lista)
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
            if vizinhos[i][j] <= menor:
                pos_menor.append((i,j))

    if len(pos_menor) == 0:
        return []

    return pos_menor[randint(0,len(pos_menor)-1)]

def aplicar_hill_climbing(n, maximo_iteracoes=100):    
    estado = gerar_estado_inicial(n)
    
    for i in range(maximo_iteracoes):
        vizinhos = calcular_estados_vizinhos(estado)
        menor = aplicar_busca_local(vizinhos)

        estado[menor[1]] = menor[0]

        if calcular_ataques(estado) == 0:
            break

    return estado

def aplicar_hill_climbing_with_side_moving(n, maximo_iteracoes=100):
    estado = gerar_estado_inicial(n)

    for i in range(maximo_iteracoes):
        vizinhos = calcular_estados_vizinhos(estado)
        menor = aplicar_busca_local_multipla(vizinhos,calcular_ataques(estado))

        if menor == []:
            break
        
        estado[menor[1]] = menor[0]

        if calcular_ataques(estado) == 0:
            break

    return estado

def aplicar_first_choice_hill_climbing(n, maximo_iteracoes=100):
    estado = gerar_estado_inicial(n)

    for i in range(maximo_iteracoes):
        menor = (randint(0,n-1), randint(0,n-1))
        estado_aux = deepcopy(estado)
        estado_aux[menor[1]] = menor[0]

        if calcular_ataques(estado_aux) == 0:
            return estado_aux

        elif calcular_ataques(estado_aux) < calcular_ataques(estado):
            estado = deepcopy(estado_aux)

    return estado

def aplicar_random_restart_hill_climbing(n, maximo_iteracoes=100):
    while True:
        estado = aplicar_hill_climbing(n,maximo_iteracoes)

        if calcular_ataques(estado) == 0:
            break

    return estado

def executar_teste(n,maximo_iteracoes):
    s1 = aplicar_hill_climbing(n,maximo_iteracoes)
    s2 = aplicar_hill_climbing_with_side_moving(n,maximo_iteracoes)
    s3 = aplicar_first_choice_hill_climbing(n,maximo_iteracoes)
    s4 = aplicar_random_restart_hill_climbing(n,maximo_iteracoes)

    print(calcular_ataques(s1))
    print(calcular_ataques(s2))
    print(calcular_ataques(s3))
    print(calcular_ataques(s4))

if __name__ == "__main__":
    executar_teste(8,100)