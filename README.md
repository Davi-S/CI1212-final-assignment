# Trabalho da disciplina arquitetura de computadores (CI1212) : Análise de memória cache


## Requisitos para a geração de gráficos
Para gerar os gráficos, recomenda-se utilizar um ambiente virtual para instalar as dependências necessárias
O ambiente virtual será criado na pasta `~/.venvs/cache-benchmark`:
```bash
python3 -m venv ~/.venvs/cache-benchmark
source ~/.venvs/cache-benchmark/bin/activate
pip install pandas matplotlib
```

## Primeiro Teste: Inferência do Tamanho da Linha de Cache a partir da Latência de Acesso

## Segundo teste: Determinação dos Tamanhos das Memórias Cache

### Descrição

<table>
  <tr>
    <th colspan="2">Arquivos
  </tr>
  <tr>
    <td>cache.c</td>
    <td>Realiza os testes de latência</td>
  </tr>
  <tr>
    <td>graph.py</td>
    <td>Gera os gráficos</td>
  </tr>
  <tr>
    <td>dados.csv</td>
    <td>Armazena os resultados</td>
  </tr>
</table>

Programa para análise de desempenho da memória cache. O teste mede a latência
média de acesso a arrays de diferentes tamanhos, permitindo identificar os
limites das caches da CPU e observar o impacto dos cache misses no tempo de
execução.

O objetivo é determinar os tamanhos das memórias cache primárias e secundárias
do processador por meio de testes de latência e, em seguida, validar os
resultados obtidos comparando-os com as especificações físicas reais do
hardware.

## Terceiro teste: Análise de Desempenho de Multiplicação de Matrizes por bloco

