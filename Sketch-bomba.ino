#include <Tone.h> //Importação da biblioteca Tone.h. Essa biblioteca permite gerar tons audíveis utilizando um buzzer. Ela simplifica a geração de tons de diferentes frequências e durações.
#include <Keypad.h> //Importação da biblioteca Keypad.h. Utilizada para interagir com o teclado matricial. Ela fornece métodos para configurar e ler os botões pressionados em um teclado matricial.
#include <Wire.h> //Importação da biblioteca Wire.h. Utilizada para a comunicação I2C (Inter-Integrated Circuit), acoplado na tela LCD para facilitar a comunicação com o Arduino.
#include <LiquidCrystal_I2C.h> //Importação da biblioteca LiquidCrystal_I2C. Esta biblioteca é uma extensão da biblioteca LiquidCrystal que simplifica a utilização de displays LCD com interfaces I2C.

int segContador = 1; //Contagem dos segundos.
int minContador = 10; //Contagem dos minutos, iniciando em 10min. Este valor pode ser alterado.
long miliSegMillis = 0; //Variável para armazenar o tempo decorrido em milissegundos.
long miliSegParaSeg = 1000; //miliSegParaSego de tempo para a contagem dos segundos.
char senha[4]; //Variável para armazenar a senha digitada pelo usuário (4 dígitos).
int qtdCaracteres = 0; //Controla quantos caracteres da senha foram inseridos.
char digitosInformados[4]; //Variável para armazenar os caracteres inseridos pelo usuário.

#define colunas 16 //Define o numero de colunas da tela LCD.
#define linhas 2  //Define o numero de linhas da tela LCD.
#define enderecoI2c 0x27 //Define o endereço padrão I2C da tela LCD.

Tone buzzer; //Objeto do tipo Tone, que será usado para controlar o buzzer.

LiquidCrystal_I2C lcd(enderecoI2c, colunas, linhas); //Objeto do tipo LiquidCrystal_I2C, que será usado para controlar a tela LCD.

//Configurações iniciais do teclado matricial.
const byte linhasTeclado = 4; //Define o número de linhas do teclado matricial.
const byte colunasTeclado = 3; //Define o número de colunas do teclado matricial.

//Definindo as posições das teclas do teclado matricial.
char hexaKeys[linhasTeclado][colunasTeclado] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pinosLinhas[linhasTeclado] = {6, 5, 4, 3}; //Define os pinos das linhas do teclado matricial.
byte pinosColunas[colunasTeclado] = {9, 8, 7}; //Define os pinos das colunas do teclado matricial.

//Criando um objeto do tipo Keypad, que será usado para controlar o teclado matricial.
Keypad tecladoBomba = Keypad(makeKeymap(hexaKeys), pinosLinhas, pinosColunas, linhasTeclado, colunasTeclado);

void setup() {
  buzzer.begin(2); //Inicializa o objeto buzzer.
  lcd.init(); //Inicializa a tela LCD.
  lcd.backlight(); //Liga a luz de fundo da tela LCD.
  Serial.begin(9600); //Inicializa a comunicação serial.
  lcd.clear(); //Limpa a tela LCD.
  lcd.setCursor(0, 0); //Define o cursor na primeira linha e primeira coluna.
  lcd.print("Digite a senha: "); //Imprime na tela a mensagem "Digite a senha: ".

  //Essa estrutura de repetição irá repetir enquanto a quantidade de caracteres inseridos
  //da senha for menor que 4.
  while (qtdCaracteres < 4) {
    lcd.setCursor(qtdCaracteres + 6, 1); //Define o cursor na coluna igual a quantidade
    //de caracteres + 6 e linha 1.
    lcd.cursor(); //Mostra o cursor na tela.
    char key = tecladoBomba.getKey(); //Obtém a tecla pressionada pelo usuário.
    key == NO_KEY;  //Verifica se a tecla pressionada pelo usuário é nula.
    if (key != NO_KEY) { 
      if (key != '*') { //Verifica se a tecla pressionada pelo usuário é diferente de um asterisco (*).
        lcd.print(key); //Mostra na tela a tecla pressionada pelo usuário.
        senha[qtdCaracteres] = key; //Insere na senha a tecla pressionada pelo usuário.
        qtdCaracteres++; //Incrementa a quantidade de caracteres inseridos na senha.
        buzzer.play(NOTE_C6, 200); //Faz o buzzer tocar um som.
      }
    }
  }

  if (qtdCaracteres == 4) { //Verifica se a quantidade de caracteres inseridos na senha for igual a 4.
    delay(500); //Espera 500 milissegundos.
    lcd.noCursor(); //Desabilita o cursor na tela.
    lcd.clear(); //Limpa a tela LCD.
    lcd.home(); //Define o cursor na primeira linha e primeira coluna.
    buzzer.play(NOTE_E6, 200); //Faz o buzzer tocar um som.
    delay(3000); //Espera 3000 milissegundos.
    lcd.clear(); //Limpa a tela LCD.
    qtdCaracteres = 0; //Reinicia a quantidade de caracteres inseridos na senha.
  }
}

//Aqui o código será repetido em um loop.
void loop() {
  timer(); //Executa a função timer().
  char key2 = tecladoBomba.getKey(); //Obtém a tecla pressionada pelo usuário.

  if (key2 == '*') { //Verifica se a tecla pressionada pelo usuário é igual a um asterisco (*)
    lcd.clear(); //Limpa a tela LCD.
    lcd.setCursor(0, 0); //Define o cursor na primeira linha e primeira coluna.
    lcd.print("Senha: "); //Imprime na tela a mensagem "Senha: ".

    while (qtdCaracteres < 4) { //Essa estrutura de repetição irá repetir enquanto a quantidade de
    //caracteres inseridos na senha for menor que 4. 
      timer(); //Executa a função timer().
      char key2 = tecladoBomba.getKey(); //Obtém a tecla pressionada.
      if (key2 != NO_KEY) { //Verifica se uma tecla foi pressionada.
        lcd.setCursor(qtdCaracteres + 7, 0); //Define o cursor na coluna igual a quantidade de
        //caracteres + 7 e linha 0.
        lcd.cursor(); //Mostra o cursor na tela.
        lcd.print(key2); //Imprime na tela a tecla pressionada pelo usuário.
        digitosInformados[qtdCaracteres] = key2;  //Insere na senha a tecla pressionada pelo usuário.
        qtdCaracteres++; //Incrementa a quantidade de caracteres inseridos na senha.
        buzzer.play(NOTE_C6, 200); //Faz o buzzer tocar um som.
        delay(100); //Espera 100 milissegundos.
        lcd.noCursor(); //Desabilita o cursor na tela.
        lcd.setCursor(qtdCaracteres + 6, 0); //Define o cursor na coluna igual a quantidade de
        //caracteres + 6 e linha 0.
        lcd.print("*"); //Imprime na tela um asterisco (*).
        lcd.setCursor(qtdCaracteres + 7, 0); //Define o cursor na coluna igual a quantidade de
        //caracteres + 67 e linha 0.
        lcd.cursor(); //Mostra o cursor na tela.
      }
    }

    if (qtdCaracteres == 4) { //Verifica se a quantidade de caracteres inseridos na senha é igual a 4.
      //Verifica se a senha digitada pelo usuário é igual a a senha definida.
      if (digitosInformados[0] == senha[0] && digitosInformados[1] == senha[1] && digitosInformados[2] == senha[2] && digitosInformados[3] == senha[3]) {
        lcd.noCursor(); //Desabilita o cursor na tela.
        lcd.home(); //Define o cursor na primeira linha e primeira coluna.
        lcd.print("Bomba desarmada"); //Imprime na tela a mensagem "Bomba desarmada".
        qtdCaracteres = 0; //Reinicia a quantidade de caracteres inseridos na senha.
        delay(25000); //Espera 2500 milissegundos.
      } else { //Se a senha digitada pelo usuário não é igual a a senha definida, então executará esses comandos..
        lcd.noCursor(); //Desabilita o cursor na tela.
        lcd.clear(); //Limpa a tela LCD.
        lcd.home(); //Define o cursor na primeira linha e primeira coluna.
        lcd.print("Senha Errada!"); //Imprime na tela a mensagem "Senha Errada!".
        buzzer.play(NOTE_A2, 90); //Faz o buzzer tocar um som.
        delay(200); //Espera 200 milissegundos.
        buzzer.play(NOTE_A2, 90); //Faz o buzzer tocar um som.
        delay(200); //Espera 200 milissegundos.
        buzzer.play(NOTE_A2, 90); //Faz o buzzer tocar um som.
        delay(200); //Espera 200 milissegundos.
        lcd.noCursor(); //Desabilita o cursor na tela.
        lcd.clear(); //Limpa a tela LCD.
        lcd.home(); //Define o cursor na primeira linha e primeira coluna.
        if (minContador > 0) { //Verifica se a  de minutos é maior que zero.
          minContador = minContador - 1; //Reduz em um minuto o tempo da bomba.
        }
        delay(1500); //Espera 1500 milissegundos.
        qtdCaracteres = 0; //Reinicia a quantidade de caracteres inseridos na senha.
      }
    }
  }
}

void timer() { //Função timer(), responsável por controlar o tempo da bomba.

  //Se o tempo da bomba chegar a zero, será mostrado na tela LCD a mensagem "A bomba Explodiu".
  if (minContador < 0) {
    lcd.noCursor();
    lcd.clear();
    lcd.home();
    lcd.print("A bomba ");
    lcd.setCursor(0, 1);
    lcd.print("Explodiu!");

    //Faz o buzzer tocar um som indicando a explosão da bomba.
    while (minContador < 0) {
      buzzer.play(NOTE_A2, 90);
      delay(100);
      buzzer.play(NOTE_A2, 90);
      delay(100);
      buzzer.play(NOTE_A2, 90);
      delay(100);
      buzzer.play(NOTE_A2, 90);
      delay(100);
      buzzer.play(NOTE_A2, 90);
      delay(100);
      buzzer.play(NOTE_A2, 90);
      delay(100);
    }
  }

  lcd.setCursor(0, 1); //Define o cursor na segunda linha e primeira coluna.
  lcd.print("Tempo:"); //Imprime na tela de LCD a mensagem "Tempo: ".

  if (minContador >= 10) { //Verifica se o tempo da bomba for maior ou igual a 10.
    lcd.setCursor(7, 1); //Define o cursor na coluna 7 e linha 1.
    lcd.print(minContador); //Imprime na tela a quantidade de minutos.
  }

  if (minContador < 10) { //Verifica se o tempo da bomba é menor que 10
    lcd.setCursor(7, 1); //Define o cursor na coluna 7 e linha 1.
    lcd.print(0); //Imprime na tela o valor zero.
    lcd.setCursor(8, 1); //Define o cursor na coluna 8 e linha 1.
    lcd.print(minContador); //Imprime na tela a quantidade de minutos.
  }

  lcd.print(":"); //Imprime na tela ':'.

  if (segContador >= 10) { //Verifica se a contagem de segundos é maior ou igual a 10.
    lcd.setCursor(10, 1); //Define o cursor na coluna 10 e linha 1.
    lcd.print(segContador); //Imprime na tela a quantidade de segundos.
  }

  if (segContador < 10) { //Verifica se a contagem de segundos é menor que 10.
    lcd.setCursor(10, 1); //Define o cursor na coluna 10 e linha 1.
    lcd.print(0); //Imprime na tela o valor zero.
    lcd.setCursor(11, 1); //Define o cursor na coluna 11 e linha 1.
    lcd.print(segContador); //Imprime na tela a quantidade de segundos.
  }

  if (segContador < 1) { //Verifica se a contagem de segundos é menor que 1.
    minContador--; //Reduz a quantidade de minutos.
    segContador = 59; //Reinicia a contagem de segundos.
  }

  //Este bloco é responsável por controlar a contagem regressiva.
  if (segContador > 0) { //Verifica se a contagem de segundos é maior que zero.
    unsigned long tempo = millis(); //Cria uma variável que armazena o tempo decorrido,
    //independente de qualquer pausa (delay).

    //Verifica se o tempo decorrido menos o tempo decorrido anteriormente é maior ou igual
    //a 1000 milissegundos (1 segundo).
    if (tempo - miliSegMillis > miliSegParaSeg) { 
      buzzer.play(NOTE_G5, 200); //Faz o buzzer tocar um som.
      miliSegMillis = tempo; //Atualiza o tempo decorrido.
      segContador--; //Reduz o contador de segundos.
    }
  }
}
