# Trabalho da disciplina arquitetura de computadores (CI1212) : Análise de memória cache


## Requisitos para a geração de gráficos
Para gerar os gráficos, é necessário o python instalado no sistema. Recomenda-se utilizar um ambiente virtual (colocamos o nome de "cache-benchmark") para instalar as dependências necessárias.

O ambiente virtual será criado na pasta `~/.venvs/cache-benchmark`:
```bash
python3 -m venv ~/.venvs/cache-benchmark
source ~/.venvs/cache-benchmark/bin/activate
pip install pandas matplotlib
```
_Observação: Apenas a etapa de geração de gráficos foi implementada em Python, para fins de análise. O benchmark principal foi desenvolvido integralmente em linguagem C como descrito no enunciado do trabalho._

## Primeiro Teste: Estimativa do Tamanho da Linha de Cache a partir da Latência de Acesso

### Descrição

<table>
  <tr>
    <th>Arquivo</th>
    <th>Função</th>
  </tr>
  <tr>
    <td>cache_line.c</td>
    <td>Executa o benchmark variando o salto entre acessos ao vetor.Realiza os testes de latência</td>
  </tr>
  <tr>
    <td>graph.py</td>
    <td>Gera o gráfico de latência em função do tamanho do salto.</td>
  </tr>
  <tr>
    <td>dados.csv</td>
    <td>Armazena os resultados coletados pelo benchmark.</td>
  </tr>
</table>

Este teste avalia a latência média de acesso à memória para diferentes valores de stride (salto entre acessos consecutivos ao vetor). A análise dos resultados permite observar o impacto da localidade espacial no desempenho da memória cache.

A partir da variação da latência em função do tamanho do salto, é possível inferir o tamanho aproximado da linha de cache do processador e compreender como o aproveitamento dos dados carregados influencia o tempo de acesso.

## Segundo teste: Estimativa dos Tamanhos das Memórias Cache

### Descrição

<table>
  <tr>
    <th>Arquivo</th>
    <th>Função</th>
  </tr>
  <tr>
    <td>cache_size.c</td>
    <td>Executa o benchmark variando o tamanho do vetor.</td>
  </tr>
  <tr>
    <td>graph.py</td>
    <td>Gera o gráfico de latência em função do tamanho do vetor.</td>
  </tr>
  <tr>
    <td>dados.csv</td>
    <td>Armazena os resultados coletados pelo benchmark.</td>
  </tr>
</table>

Este teste mede a latência média de acesso para vetores de diferentes tamanhos. À medida que o conjunto de dados cresce, os acessos passam a utilizar diferentes níveis da hierarquia de memória, como as caches L1, L2, L3 e a memória principal.

O objetivo é estimar os tamanhos das memórias cache do processador por meio da análise dos pontos em que ocorre aumento da latência de acesso, comparando posteriormente os resultados obtidos com as especificações do hardware utilizado.


## Terceiro teste: Análise de Desempenho de Multiplicação de Matrizes por bloco

### Descrição

<table>
  <tr>
    <th>Arquivo</th>
    <th>Função</th>
  </tr>
  <tr>
    <td>matrix_mult.c</td>
    <td>Implementa as versões convencional, otimizada e em blocos da multiplicação de matrizes.</td>
  </tr>
</table>

Este teste compara o desempenho da multiplicação convencional de matrizes com uma versão otimizada por blocos (blocking). A técnica de blocagem divide as matrizes em submatrizes menores, aumentando o reaproveitamento dos dados já carregados na memória cache.

O objetivo é analisar o impacto da hierarquia de memória no tempo de execução da multiplicação de matrizes, comparando os tempos obtidos pelas diferentes implementações e observando os ganhos de desempenho proporcionados pela melhor utilização da cache.

## Integrantes

| Nome | GRR |
|--------|--------|
| Thalísia Arianna Fernandes Fleck | GRR20256088 |
| Davi Alves Sampaio | GRR20255653 |
| Haico de Toledo Boehs | GRR20253482 |
| Melissa Goulart Kemp | GRR20255413 |
| Analuiza Alves da Cruz | GRR20254471 |
