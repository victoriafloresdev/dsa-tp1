import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

plt.switch_backend('Agg')

# Carregar o arquivo CSV
df = pd.read_csv('resultados.csv')

# Número de algoritmos
num_algoritmos = 13

# Número de pontos de dados por algoritmo
num_pontos = 260

# Loop sobre os algoritmos
for i in range(num_algoritmos):
    # Índices do intervalo atual
    inicio = i * num_pontos
    fim = (i + 1) * num_pontos
    
    # Extrair os dados da coluna 9 e converter em numpy array
    coluna_9 = df.iloc[inicio:fim, 8].values.astype(float)

    # Plotar o gráfico
    plt.figure(figsize=(10, 6))
    plt.plot(np.arange(1, num_pontos + 1), coluna_9)
    plt.xlabel('Índice')
    plt.ylabel('Tempo')
    plt.title(f'Algoritmo {i+1}')
    plt.grid(True)
    plt.savefig(f'algoritmo_{i+1}.png')  # Salvar o gráfico como arquivo PNG
    plt.close() 