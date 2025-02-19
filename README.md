🛠️ Projeto: Controle de LEDs e Display com Joystick e ADC no RP2040

🐖 Sumário

🎯 Objetivos

👌 Descrição do Projeto

⚙️ Funcionalidades Implementadas

🛠️ Requisitos do Projeto

💂️ Estrutura do Repositório

🖥️ Como Compilar

🤝 Contribuições

🎦 Demonstração em Vídeo

💡 Considerações Finais

---

🎯 Objetivos

- Compreender o funcionamento do conversor analógico-digital (**ADC**) no **RP2040**.
- Utilizar **PWM** para controlar a intensidade de **LEDs RGB** com base nos valores do joystick.
- Representar a posição do joystick no **display SSD1306** por meio de um quadrado móvel.
- Aplicar o protocolo de comunicação **I2C** para integração com o display.

---

👌 Descrição do Projeto

Este projeto utiliza a placa **BitDogLab** para capturar valores do **joystick** e controlar **LEDs RGB** e um **display SSD1306** com base nas entradas analógicas. Os componentes utilizados incluem:

- **LED RGB** (GPIOs **11, 12 e 13**).
- **Botão do Joystick** (GPIO **22**).
- **Joystick** (GPIOs **26 e 27** para os eixos X e Y).
- **Botão A** (GPIO **5**).
- **Display SSD1306** via **I2C** (GPIOs **14 e 15**).

O joystick fornece valores analógicos que controlam a **intensidade luminosa dos LEDs** e movimentam um **quadrado no display**. O botão do joystick **alterna o estado do LED Verde e modifica a borda do display**, enquanto o botão A **ativa ou desativa os LEDs PWM**.

---

⚙️ Funcionalidades Implementadas

1. **Controle de LEDs RGB por Joystick:**
   - O **LED Azul** ajusta sua intensidade conforme o **eixo Y**.
   - O **LED Vermelho** ajusta sua intensidade conforme o **eixo X**.
   - Ambos variam entre **0 e 4095**, sendo apagados no centro e atingindo brilho máximo nos extremos.
   - Controle de intensidade via **PWM** para transições suaves.

2. **Exibição Gráfica no Display SSD1306:**
   - Um **quadrado de 8x8 pixels** representa a posição do joystick.
   - O movimento do quadrado é proporcional aos valores analógicos capturados.
   - Comunicação com o display via **I2C**.

3. **Interação com Botões:**
   - O **botão do joystick**:
     - Alterna o estado do **LED Verde**.
     - Modifica a **borda do display** a cada acionamento.
   - O **botão A**:
     - Liga/desliga os **LEDs PWM**.

---

🛠️ Requisitos do Projeto

- **Uso de Interrupções:** Controle eficiente dos botões utilizando **rotinas IRQ**.
- **Debouncing:** Implementação de tratamento de bouncing via software.
- **Utilização do Display SSD1306:** Aplicação de gráficos simples com **I2C**.
- **Código bem estruturado e comentado:** Facilita o entendimento e manutenção.

---

💂️ Estrutura do Repositório

```
├── controle_adc.c        # Código principal do projeto
├── README.md             # Este arquivo
├── joystick_adc.h        # Configuração do joystick e ADC
├── joystick_adc.c        # Implementação do controle do joystick
├── display_control.h     # Configuração do display SSD1306
├── display_control.c     # Implementação da exibição no display
└── ...                   # Demais arquivos necessários
```

---

🖥️ Como Compilar

1. Clone o repositório:
   ```sh
   git clone https://github.com/zrcrd/tarefaADC.git
   ```
2. Acesse a pasta do projeto:
   ```sh
   cd https://github.com/zrcrd/tarefaADC.git
   ```
3. Compile o projeto no **VS Code** configurado para **RP2040**.
4. Carregue o código na **placa BitDogLab**.

### 🖥️ Método Alternativo:

1. Baixe o repositório como **arquivo ZIP**.
2. Extraia os arquivos para uma pasta.
3. Importe o projeto no **VS Code** com a extensão **Raspberry Pi Pico**.
4. Compile e envie o código para o **RP2040** em **modo boot select**.
5. Teste a interação do joystick com os LEDs e o display.

---

🧑‍💻 Autor

**josé Ricardo de Souza Santos**

---

📝 Descrição

Este projeto faz parte da **Unidade 4 | Capítulo 8** do curso, consolidando o conhecimento sobre **ADC**, **PWM**, **I2C** e **manipulação de hardware** com o **RP2040**.

---

🤝 Contribuições

Contribuições são bem-vindas! Para contribuir, siga os passos abaixo:

1. Faça um **fork** do repositório.
2. Crie uma nova branch:
   ```sh
   git checkout -b minha-feature
   ```
3. Realize suas alterações e faça um commit:
   ```sh
   git commit -m 'Adicionando nova funcionalidade'
   ```
4. Envie suas alterações:
   ```sh
   git push origin minha-feature
   ```
5. Abra um **Pull Request**.

---

🎦 Demonstração em Vídeo

O vídeo de demonstração do projeto pode ser acessado pelo link:


---

💡 Considerações Finais

Este projeto é uma excelente oportunidade para consolidar conhecimentos sobre **ADC no RP2040**, controle de **LEDs RGB**, utilização de **PWM**, integração com **I2C** e manipulação de botões com **interrupções**.

Se tiver dúvidas ou sugestões, sinta-se à vontade para contribuir! 

---
