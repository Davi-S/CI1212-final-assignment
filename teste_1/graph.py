import pandas as pd
import matplotlib.pyplot as plt


def gerar_grafico_linha():
    # Lê o novo arquivo gerado pelo C
    try:
        df = pd.read_csv("dados.csv")
    except FileNotFoundError:
        print("Arquivo não encontrado.")
        return

    plt.figure(figsize=(10, 6))

    # Plota a linha principal
    plt.plot(
        df["Salto_Bytes"],
        df["Latencia_ns"],
        marker="o",
        linestyle="-",
        linewidth=2,
        color="#ff7f0e",
    )

    # Configura o eixo X em escala logarítmica
    plt.xscale("log", base=2)
    saltos = df["Salto_Bytes"].unique()
    rotulos = [f"{int(s)} B" for s in saltos]
    plt.xticks(saltos, rotulos)

    # Visual e textos
    plt.title("Linha de Cache", fontsize=14, fontweight="bold")
    plt.xlabel("Tamanho do Salto (Stride)", fontsize=12)
    plt.ylabel("Latência por Acesso (nanossegundos)", fontsize=12)
    plt.grid(True, which="both", linestyle="--", alpha=0.5)

    plt.tight_layout()
    plt.savefig("grafico_linha_cache.png", dpi=300)
    print("Gráfico do tamanho da linha gerado com sucesso")


if __name__ == "__main__":
    gerar_grafico_linha()
