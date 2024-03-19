//Importação da biblioteca Keypad.h. Utilizada para interagir com o teclado matricial.
#include <Keypad.h> //Ela fornece métodos para configurar e ler os botões pressionados em um teclado matricial.
#include <Servo.h> //Importação da biblioteca Servo.h. Utilizada para interagir com o servomotor.

//Definindo o número de linhas e colunas do teclado matricial.
const byte linhas = 4; 
const byte colunas = 4;

//Definindo as posições das teclas do teclado matricial.
const char caracteres_teclado[linhas][colunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte conexao_linhas[linhas] = {4, 5, 6, 7}; //Define os pinos das linhas do teclado matricial.
byte conexao_colunas[colunas] = {11, 10, 9, 8}; //Define o número de colunas do teclado matricial.

//Criando um objeto do tipo Keypad, que será usado para controlar o teclado matricial.
Keypad teclado_caixa = Keypad(makeKeymap(caracteres_teclado), conexao_linhas, conexao_colunas, linhas, colunas); //

char senha[] = {'1', '7', '6', '3'}; //Definindo a senha da caixa (4 dígitos). Você pode alterar esta senha.
char digitado[4]; //Variável para armazenar os números digitados.
int caracteres_digitados = 0; //Variável para armazenar a quantidade de caracteres digitados.

Servo servoMotor; //Criando um objeto do tipo Servo, que será usado para interagir com o servo motor.
bool caixaDesbloqueada = false; //Variável para controlar se a caixa está bloqueada ou desbloqueada.

void setup() {
  Serial.begin(9600);  //Inicializa a comunicação serial.
  servoMotor.attach(12); //Inicializa o servo motor no pino 12.
  servoMotor.write(90); //Define a posição inicial do servo.
  Serial.println("\nIniciado"); //Imprime a mensagem de inicialização no monitor serial, útil para testes.
}

//Aqui o código será repetido em um loop.
void loop() {
  char leitor_teclas = teclado_caixa.getKey(); //Lê a tecla pressionada no teclado matricial.

  if (leitor_teclas != NO_KEY) { //Verifica se uma tecla foi pressionada no teclado matricial.
    if (!caixaDesbloqueada) { //Verifica se a caixa está bloqueada.
      digitado[caracteres_digitados] = leitor_teclas; //Armazena a tecla pressionada no teclado matricial na variável digitado.
      caracteres_digitados++; //Incrementa a quantidade de caracteres digitados.
      Serial.print(leitor_teclas); //Imprime a tecla pressionada no teclado matricial no monitor serial.

      if (caracteres_digitados == 4) { //Verifica se a quantidade de caracteres digitados é igual a 4.
        if (checkPassword()) { //Verifica se o retorno da função checkPassword() é verdadeiro.
          Serial.println("\nCORRETO"); //Imprime a mensagem de correto no monitor serial, útil para testes.
          desbloquearCaixa(); //Chama a função desbloquearCaixa().
        } else { //Se retorno da função checkPassword() for falso, executa a linha a seguir.
          Serial.println("\nINCORRETO"); //Imprime a mensagem de erro no monitor serial, útil para testes.
        }

        caracteres_digitados = 0; //Reinicializa a variável caracteres_digitados.
        limparSenhaInserida(); //Chama a função limparSenhaInserida().
      }
    }
  }
}

boolean checkPassword() { //Função que verifica a senha inserida.
  for (int i = 0; i < 4; i++) { //Percorre a quantidade de caracteres digitados.
    if (senha[i] != digitado[i]) { //Verifica se o caractere digitado é diferente da senha.
      return false; //Retorna falso.
    }
  }
  return true; //Se os caracteres digitados forem correspondentes a senha, retorna verdadeiro.
}

void limparSenhaInserida() { //Função que limpa a senha inserida.
  for (int i = 0; i < 4; i++) { //Percorre a quantidade de caracteres digitados.
    digitado[i] = 0; //Atribui zero aos caracteres digitados.
  }
}

void desbloquearCaixa() { //Função que desbloqueia a caixa.
  servoMotor.write(180); //Define a posição do servo.
  Serial.println("Caixa desbloqueada"); //Imprime a mensagem de caixa desbloqueada no monitor serial.
  caixaDesbloqueada = true; //Define a variável caixaDesbloqueada como verdadeiro.
}
