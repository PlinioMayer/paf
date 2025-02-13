# paf

Plínio
Allocating
(your)
Files

Um gerenciador de arquivos de sistema com o jeitinho do Plínio.
Este projeto simula o gerenciamento de arquivos e pastas dentro de um sistema. A simulação utiliza apenas os metadados, sem realizar alocação de dados de fato.

## Compilação

Este projeto foi desenvolvido completamente em linguagem C, utilizando bibliotecas disponíveis no padrão ANSI. O compilador utilizado foi o GCC em sua versão 99. Em um ambiente Linux, basta instalação do pacote `build-essentials` e execução do comando:

>make all

Em outros ambiente, é necessária a instalação de um compilador GCC e de um utilitário Make.

Em última instância, o passo a passo de compilação pode ser deduzido do arquivo [Makefile](./Makefile).

## Execução

Após a compilação, o executável poderá ser encontrado na pasta [bin](./bin) e, a partir da raíz do projeto, pode ser executado através do comando:

>./bin/paf [arquivo]

[arquivo] é um parâmetro opcional que indica o nome do arquivo a ser criado. O nome padrão é HD.

## Estrutura do projeto

### [bin](./bin)

Contém o binário (paf) resultante da compilação do código fonte.

### [include](./include)

Deve conter todos os arquivos de declaração (arquivos de header, *.h) criados pelo projeto.

### [obj](./obj)

Contém arquivos objeto (*.o), gerados durante a compilação.

### [src](./src)

Deve conter todos os arquivos de código fonte (*.c) criados pelo projeto.

## Camadas

Para facilitar a visualização do projeto, utilizou-se em sua arquitetura uma visão de camadas, onde cada camada possui uma função e uma proximidade com o usuário e com o arquivo binário. A saber:

### CLI

Essa é a camada mais próxima do usuário, responsável por interpretar os seus comandos e exibir os resultados. Compõem essa camada os arquivos [cli.h](./src/cli.h) e [cli.c](./src/cli.c).

### Memória

Essa camada serve de intermediária entre a camada CLI e a camada de IO. Ela é responsável por definir e operar as estruturas de dados representantes dos objetos (arquivos) em memória. Compõem essa camada os arquivos [memoria.h](./src/memoria.h) e [memoria.c](./src/memoria.c).

### IO

Essa camada é a responsável por realizar todas as operações de leitura e escrita em disco. Ela define e gerencia os objetos (arquivos) em seu formato binário. Compõem essa camada os arquivos [io.h](./src/io.h) e [io.c](./src/io.c).

### Utilitários

São arquivos passíveis de utilização em qualquer camada. Abstraem operações comumente utilizadas em uma ou mais etapas de um comando. Compõem essa categoria os arquivos [string.utils.h](./src/string.utils.h), [string.utils.c](./src/string.utils.c) e [boolean.h](./include/boolean.h).

### Entrada

Por fim, o mais famoso arquivo da linguagem C, o [main.c](./src/main.c) serve de entrada para execução do programa.

## Objetos

Para gerenciar os arquivos seguindo este modelo, foram necessárias diversas definições de tipos, explorados com mais detalhes a seguir.

### [boolean](./include/boolean.h)

Um enum criado pelo desconehcimento das particularidades do tipo bool definido na \<stdbool.h\>, para esse projeto era necessário um amplo controle do que viria a ser escrito e lido do disco.

### [tipo_arquivo](./include/io.h)

Um enum utilizado para diferenciar o tipo de arquivo quando da sua leitura do disco. O valor 0 indica se tratar de um documento e o vlaor 1 indica um diretório.

### [flags_t](./include/io.h)

Um byte criado em separado de [arquivo_t](#arquivo_t) para ser utilizado no plano inicial, que incluía alocação de dados. Essas flags permitiriam diferenciar blocos de dados daqueles de metadados, além de indicar se um bloco foi logicamente excluído. Atualmente permite indicar apenas a exclusão.

### [atributos_t](./include/io.h)

Agrupamento de um byte para atributos pertencentes a um [arquivo_t](#arquivo_t). Atualmente indica apenas se trata-se de um arquivo ou diretório, utilizando o enum [tipo_arquivo](#tipo_arquivo).

### [arquivo_t](./include/io.h)

Estrutura utilizada para abstrair o agrupamento de bytes que denota o metadado de um arquivo. Permite obter a posição em bytes de seu diretório "pai", seus [atributos](#atributos_t) e seu nome. O endereço de seu pai é dado por um inteiro sem sinal de 64 bits e seu nome é composto de 256 bytes da tabela ASCII (dos quais apenas 255 são de fato usados, maldito '\0').

### [mem_arquivo_t](./include/memoria.h)

Estrutura utilizada para gerenciar os [arquivos](#arquivo_t) em memória. Possui um ponteiro para um objeto do tipo [arquivo_t](#arquivo_t), contendo os dados salvos em disco, um ponteiro para o objeto mem_arquivo_t "pai", um vetor de objetos do tipo mem_arquivo_t para seus "filhos", um inteiro sem sinal de 64 bits para contagem de "filhos" e um inteiro sem sinal de 64 bits para guardar seu endereço em disco.

### [comando_info_t](./include/memoria.h)

Estrutura utilizada para facilitar a manipulação de comandos.

## HD

HD é o nome padrão do arquivo binário criado durante a execução do programa. Eu vou explicar a referência, caso algum jovem acesse esse repositório no futuro: HD é uma sigla referente a Hard Disk, um tipo de disco magnético muito utilizado pelos antigos para compor a memória secundária. Os "arquivos" gerenciados por esse programa possuem um tamanho total de 265 bytes (266 considerando a flag inicial), portanto, o binário criado por esse programa é organizado da seguinte maneira:

>byte&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0 -> [flags_t](#flags_t)\
>bytes&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1 - 8 -> endereço do pai\
>byte&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;9 -> [atributos_t](#atributos_t)\
>bytes 10 - 265 -> nome
