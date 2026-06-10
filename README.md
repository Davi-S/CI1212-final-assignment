# Trabalho da disciplina arquitetura de computadores (CI1212) : Análise de memória cache

## Primeiro Teste: Determinação dos Tamanhos das Memórias Cache

### Descrição

Programa para análise de desempenho da memória cache. 
O teste mede a latência média de acesso a arrays de diferentes tamanhos, 
permitindo identificar os limites das caches da CPU e observar o impacto dos cache misses no tempo de execução.

O objetivo é determinar os tamanhos das memórias cache primárias e secundárias do processador por meio de testes de latência e, 
em seguida, validar os resultados obtidos comparando-os com as especificações físicas reais do
hardware.

<table>
  <tr>
    <th colspan="2">Arquivos
  </tr>
  <tr>
    <td>tam_cache.c</td>
    <td>Realiza os testes de latência</td>
  </tr>
  <tr>
    <td>graph.py</td>
    <td>Gera os gráficos</td>
  </tr>
  <tr>
    <td>dados_cache.csv</td>
    <td>Armazena os resultados</td>
  </tr>
</table>

## Segundo teste: Identificação do Tamanho da Linha de Cache

## Terceiro teste: Análise de Desempenho de Multiplicação de Matrizes por bloco
