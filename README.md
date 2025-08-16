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

## Instalação e execução

```bash
# Clonar o repositório
git clone https://github.com/RaelSouz/Pong-AI.git

# Entrar na pasta do projeto
cd Pong-AI

# Criar diretório para build
mkdir build && cd build

# Gerar arquivos de build
cmake ..

# Compilar o projeto
make

# Executar o jogo
./Pong-AI
```

## Rede Neural

A AI utiliza uma rede neural feedforward simples:

- **Camada de entrada**: posição da bola *(x, y)* e posição vertical da raquete *(y)*.
- **Camada oculta**: 6 neurônios.
- **Camada de saída**: 1 neurônio que define o movimento da raquete (para cima ou para baixo).
- **Treinamento**: backpropagation simples, ajustando pesos sempre que a IA perde um ponto.
- **Função de ativação**: ```tanh```.

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).