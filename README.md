<!-- Local reservado para GIF demonstrativo -->

![Preview](/assets/preview.png)

![C++17](https://img.shields.io/badge/C++-17-blue)
[![SDL2](https://img.shields.io/badge/SDL2-2.32.6-brightgreen)](https://github.com/libsdl-org/SDL/releases/tag/release-2.32.6)
[![SDL2_ttf](https://img.shields.io/badge/SDL2_ttf-2.24.0-brightgreen)](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0)
[![CMAKE](https://img.shields.io/badge/CMake-3.16-brightgreen)](https://cmake.org/download/)
![OS](https://img.shields.io/badge/OS-Linux-blue)

Este projeto é uma recriação do clássico jogo **Pong**, desenvolvida em **C++** utilizando a biblioteca **SDL** para renderização e controle de entrada.
Inclui o modo **Jogador vs IA**, com limite de 24 pontos para definir o vencedor.

O diferencial do projeto está na implementação de uma **rede neural artificial** responsável por controlar um dos jogadores, simulando comportamento inteligente sem intervenção humana.

---

## Requisitos

Este projeto foi desenvolvido e testado em **Linux**.  
Pode ser portado para outros sistemas, mas ajustes na configuração do SDL podem ser necessários.

- **Compilador:** compatível com **C++17** ou superior
- **Ferramentas de build:** [CMake](https://cmake.org/download/) e Make
- **Bibliotecas necessárias:**
  - [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.32.6)
  - [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0)

> As bibliotecas SDL2 e SDL2_ttf precisam estar instaladas e configuradas no sistema antes da compilação.

---

## Execução

```bash
# 1. Clonar o repositório
git clone https://github.com/RaelSouz/Pong-AI.git

# 2. Entrar na pasta do projeto
cd Pong-AI

# 3. Criar diretório para arquivos de compilação
mkdir build && cd build

# 4. Gerar arquivos de build com CMake
cmake ..

# 5. Compilar o projeto usando make
make

# 6. Executar o binário
./Pong-AI
```

## Rede Neural

A inteligência artificial utiliza uma rede neural feedforward composta por:

- **Camada de entrada**: posição da bola *(x, y)* e posição vertical da raquete *(y)*.
- **Camada oculta**: 6 neurônios.
- **Camada de saída**: 1 neurônio, cuja saída define o movimento da raquete (para cima ou para baixo).
- **Treinamento**: backpropagation simples, ajustando pesos sempre que a IA perde um ponto.
- **Função de ativação**: ```tanh```.
