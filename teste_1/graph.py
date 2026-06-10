import pandas as pd
import matplotlib.pyplot as plt


def gerar_grafico_cache():
    # Lê o arquivo CSV
    df = pd.read_csv("dados_cache.csv")

    # Agrupa por tamanho e calcula a média das execuções para cada ponto
    df_media = df.groupby("Tamanho_KB")["Latencia_ns"].mean().reset_index()

    # Configuração da área do gráfico
    plt.figure(figsize=(11, 6))
    plt.plot(
        df_media["Tamanho_KB"],
        df_media["Latencia_ns"],
        marker="o",
        linestyle="-",
        linewidth=2,
    )
    # Eixo logarítmico para mostrar melhor a diferença
    plt.yscale("log", base=10)
    # plt.ylim(1.0, 2.5)

    # Aplica escala logarítmica na base 2 para o eixo X (tamanhos de memória)
    plt.xscale("log", base=2)

    # Formata os rótulos do eixo X para exibir KB ou MB de forma legível
    tamanhos = df_media["Tamanho_KB"].unique()
    rotulos = [f"{int(t)} KB" if t < 1024 else f"{int(t / 1024)} MB" for t in tamanhos]
    plt.xticks(tamanhos, rotulos, rotation=45)

    # Visual do gráfico
    plt.title(
        "Latência de Acesso por Tamanho de Bloco",
        fontsize=14,
        fontweight="bold",
    )
    plt.xlabel("Tamanho do Array de Teste", fontsize=12)
    plt.ylabel("Latência Média por Acesso (nanossegundos)", fontsize=12)
    plt.grid(True, which="both", linestyle="--", alpha=0.5)
    plt.tight_layout()

    plt.savefig("grafico_media_cache.png", dpi=300)
    print("Gráfico salvo.")
    plt.show()


if __name__ == "__main__":
    gerar_grafico_cache()
