#include <stdio.h>
#include <stdlib.h>
#include <math.h>

                                                     //importante
//os calculos e teoria para resolver esse problema utilizando essa base foram retirados de http://commanber.com/2017/05/21/decision-tree/ 




//struct para armazenar uma estrutura para armazenar os dados das variaveis do calculo da entropia condicional
struct CONDITIONAL_ENTROPY{

 float ma;
 float me;
 float pma1;
 float pma2;
 float pma3;
 float pme1;
 float pme2;
 float pme3;

};
//estrutura que armazena o no da arvore de decisao
struct NO{

  struct NO *pai;
  struct NO *direita;//se menor que a media um valor de uma variavel quando comparado com o pai, então ela vai para direita
  struct NO *esquerda;//se maior que a media um valor de uma variavel quando comparado com o pai, então ela vai para a esquerda
  int variavel;//guarda a qual tipo de variavel a condicao esta atrelada, se 1, então SL, 2 SW, 3 PL e 4 PW
  float condicao;//guarda a media para aquela variavel, onde essa media serve como condicao
  char flor[20];

};
//variaveis da arvore de decisao (a variavel ultimo nem foi utilizada rsrsr)
struct DECISION_TREE{

  struct NO *ultimo;
  struct NO *primeiro; 

};
//deixei a variavel como global para não dar conflito de subprogramas na recursao do algoritmo ID3
struct DECISION_TREE *tree;//arvore que vai guardar a estrutura final

//estrutura que armazena os valores e variaveis de cada tipo de flor, fiz um vetor dessa estrutura
struct DATA{

  float SL;//sepal length
  float SW;//sepal width
  float PL;//petal length
  float PW;//petal width
  char class[20];// nome das classes :)

};
//função apenas para calcular o tamanho do arquivo :( e retornar esse tamanho para fazer o malloc
int fileSize(FILE *in_file){

   int cont=0;
   struct DATA aux;
   while(fscanf(in_file,"%f %f %f %f %s",&aux.SL,&aux.SW,&aux.PL,&aux.PW,aux.class) != EOF){

     cont=cont+1;

   }

   return cont;

}
//verifica o maior valor da função ganho para cada variavel de acordo com um limiar
int maior(float IG_SL,float IG_SW,float IG_PL,float IG_PW,float limiar){

 if(IG_SL >= IG_SW && IG_SL >= IG_PL && IG_SL >= IG_PW){

   if(IG_SL > limiar){
    return 1;//1 pq denota um valor para a variavel SL, como ela é a primeira coluna, fica como sendo 1
   }else{
 
      return -1;
   } 
 }
 else if(IG_SW >= IG_SL && IG_SW >= IG_PL && IG_SW >= IG_PW){

  if(IG_SW > limiar){
    return 2;//2 pq denota um valor para a variavel SW, como ela é a primeira coluna, fica como sendo 2
   }else{
 
      return -1;
   }
 
 }
 else if(IG_PL >= IG_SL && IG_PL >= IG_SW && IG_PL >= IG_PW){
  
  if(IG_PL > limiar){
    return 3;//3 pq denota um valor para a variavel PL, como ela é a primeira coluna, fica como sendo 3
   }else{
 
      return -1;
   } 

 }
 else if(IG_PW >= IG_SL && IG_PW >= IG_SW && IG_PW >= IG_PL){

  if(IG_PW > limiar){
    return 4;//4 pq denota um valor para a variavel PW, como ela é a primeira coluna, fica como sendo 4
   }else{
 
      return -1;
   }

 }


}
//função que serve para copiar dados de um conjnto em subconjuntos dependendo de o dado foi particionado sendo maior ou menor que a media
void copiarDados(struct DATA *dados, struct DATA *treino,float mean,char c,int indice,int var){

   int cont=0;
  
   for(int i=0;i<indice;i=i+1){

      //se "s" o conjunto eé maior que a media e variavel um, e por ser variavel 1, SL maior que a media porque c==s, então copia os dados referente a isso, senão verifica se a variavel é 1, se c==n portanto SL < media, e copia os dados referente a esse padrão, assim se segue para todo o código
      if((c == 's' && var == 1 && treino[i].SL >= mean) || ((c == 'n' && var == 1 && treino[i].SL < mean))){

             dados[cont] = treino[i];
             cont=cont+1;
      }
      else if((c == 's' && var == 2 && treino[i].SW >= mean) || ((c == 'n' && var == 2 && treino[i].SW < mean))){

             dados[cont] = treino[i];
             cont=cont+1;

      }

      else if((c == 's' && var == 3 && treino[i].PL >= mean) || ((c == 'n' && var == 3 && treino[i].PL < mean))){

            dados[cont] = treino[i];
            cont=cont+1;

      }else if((c == 's' && var == 4 && treino[i].PW >= mean) || ((c == 'n' && var == 4 && treino[i].PW < mean))){

            dados[cont] = treino[i];
            cont=cont+1;

      }

   }

}
//aqui esta o famoso ID3, recursivo, recebendo a base de treino, indice o tamanho da base, entropia global fala por si so e um ponto inicial que é o pai
//cada chamada recursiva leva um no referente a arvore de decisao
void ID3(struct DATA *treino,int indice,float entropia_global, struct NO *pr){

//lembre-se que as equacoes podem ser encontradas no site no informado no inicio desse arquivo

//inicio do calculo da media
 float meanSL=0,meanSW=0,meanPL=0,meanPW=0;

 for(int i=0;i<indice;i=i+1){

    meanSL=meanSL+treino[i].SL;
    meanSW=meanSW+treino[i].SW;
    meanPL=meanPL+treino[i].PL;
    meanPW=meanPW+treino[i].PW;

 }

 meanSL=meanSL/indice;
 meanSW=meanSW/indice;
 meanPL=meanPL/indice;
 meanPW=meanPW/indice;
//fim do calculo da media
 
//calculo da entropia condicional, dado que um evento da ocorreu
//pma1--quantidade de valores maiores que a media reerente a Iris-virginica,pma2 referente a Iris-setosa e pma3 iris-versicolor
//assim se segue para o pme1, porém seria a contagem dos valores menores que a media

//(alguma coluna).me ou (alguma coluna.ma denotam o tamanho de cada coluna maiores ou menores que a media)
 struct CONDITIONAL_ENTROPY SL,SW,PL,PW;
 SL.pma1=0,SL.pma2=0,SL.pma3=0,SW.pma1=0,SW.pma2=0,SW.pma3=0,PL.pma1=0,PL.pma2=0,PL.pma3=0,PW.pma1=0,PW.pma2=0,PW.pma3=0;
 SL.pme1=0,SL.pme2=0,SL.pme3=0,SW.pme1=0,SW.pme2=0,SW.pme3=0,PL.pme1=0,PL.pme2=0,PL.pme3=0,PW.pme1=0,PW.pme2=0,PW.pme3=0;
 SL.ma=0,SW.ma=0,PL.ma=0,PW.ma=0,SL.me=0,SW.me=0,PL.me=0,PW.me=0;
   
 //aqui eu varro minha base de dados com tamanho indice, que seria o ultimo indice na qual a base tem valor. Essa variável só existe por causa do c,
 //pois não tem um jeito de melhor que esse de saber o tamanho de um vetor senão com uma variavel de controle
 for(int i=0;i<indice;i=i+1){

   //printf("%i\n",strcmp(treino[i].class,"Iris-virginica"));

   if(treino[i].SL >= meanSL){

      if(strcmp(treino[i].class,"Iris-virginica")==0){
 
        SL.pma1=SL.pma1+1;

      }
      else if(strcmp(treino[i].class,"Iris-setosa")==0){
 
        SL.pma2=SL.pma2+1;

      }
      else if(strcmp(treino[i].class,"Iris-versicolor")==0){

        SL.pma3=SL.pma3+1;

      }

      SL.ma=SL.ma+1;
   }if(treino[i].SL < meanSL){

      if(strcmp(treino[i].class,"Iris-virginica")==0){
 
        SL.pme1=SL.pme1+1;

      }
      else if(strcmp(treino[i].class,"Iris-setosa")==0){
 
        SL.pme2=SL.pme2+1;

      }
      else if(strcmp(treino[i].class,"Iris-versicolor")==0){

        SL.pme3=SL.pme3+1;

      }

      SL.me=SL.me+1;
   }if(treino[i].SW >= meanSW){

      if(strcmp(treino[i].class,"Iris-virginica")==0){
 
        SW.pma1=SW.pma1+1;

      }
      else if(strcmp(treino[i].class,"Iris-setosa")==0){
 
        SW.pma2=SW.pma2+1;

      }
      else if(strcmp(treino[i].class,"Iris-versicolor")==0){

        SW.pma3=SW.pma3+1;

      }

      SW.ma=SW.ma+1;
   }if(treino[i].SW < meanSW){

      if(strcmp(treino[i].class,"Iris-virginica")==0){
 
        SW.pme1=SW.pme1+1;

      }
      else if(strcmp(treino[i].class,"Iris-setosa")==0){
 
        SW.pme2=SW.pme2+1;

      }
      else if(strcmp(treino[i].class,"Iris-versicolor")==0){

        SW.pme3=SW.pme3+1;

      }

      SW.me=SW.me+1;
   }if(treino[i].PL >= meanPL){

      if(strcmp(treino[i].class,"Iris-virginica")==0){
 
        PL.pma1=PL.pma1+1;

      }
      else if(strcmp(treino[i].class,"Iris-setosa")==0){
 
        PL.pma2=PL.pma2+1;

      }
      else if(strcmp(treino[i].class,"Iris-versicolor")==0){

        PL.pma3=PL.pma3+1;

      }

      PL.ma=PL.ma+1;
   }if(treino[i].PL < meanPL){

      if(strcmp(treino[i].class,"Iris-virginica")==0){
 
        PL.pme1=PL.pma1+1;

      }
      else if(strcmp(treino[i].class,"Iris-setosa")==0){
 
        PL.pme2=PL.pma2+1;

      }
      else if(strcmp(treino[i].class,"Iris-versicolor")==0){

        PL.pme3=PL.pma3+1;

      }

      PL.me=PL.me+1;
   }if(treino[i].PW >= meanPW){

      if(strcmp(treino[i].class,"Iris-virginica")==0){
 
        PW.pma1=PW.pma1+1;

      }
      else if(strcmp(treino[i].class,"Iris-setosa")==0){
 
        PW.pma2=PW.pma2+1;

      }
      else if(strcmp(treino[i].class,"Iris-versicolor")==0){

        PW.pma3=PW.pma3+1;

      }

      PW.ma=PW.ma+1;
 
   }if(treino[i].PW < meanPW){

      if(strcmp(treino[i].class,"Iris-virginica")==0){
 
        PW.pme1=PW.pme1+1;

      }
      else if(strcmp(treino[i].class,"Iris-setosa")==0){
 
        PW.pme2=PW.pme2+1;

      }
      else if(strcmp(treino[i].class,"Iris-versicolor")==0){

        PW.pme3=PW.pme3+1;

      }

      PW.me=PW.me+1;
    
   }

 }
  //fim da contagem de valores maiores que a media e menores

  //aqui acontece uma verificação de um caso base, que seria se o subconjunto dos dados tivesse apenas uma classe, logo essa clase viraria folha
  int totClass1E=SL.pma1+SW.pma1+PL.pma1+PW.pma1;//total dos valores da classe 1 (iris-virginica) maiores que a media
  int totClass2E=SL.pma2+SW.pma2+PL.pma2+PW.pma2;//total dos valores da classe 2 (iris-setosa) maiores que a media
  int totClass3E=SL.pma3+SW.pma3+PL.pma3+PW.pma3;//idem
  int totClass1D=SL.pme1+SW.pme1+PL.pme1+PW.pme1;//total dos valores da classe 1 (iris-virginica) menores que a media
  int totClass2D=SL.pme2+SW.pme2+PL.pme2+PW.pme2;//idem
  int totClass3D=SL.pme3+SW.pme3+PL.pme3+PW.pme3;//idem

  //caso so tenha a classe 1
  if((totClass1E!=0 && totClass2E==0 && totClass3E==0) || (totClass1D!=0 && totClass2D==0 && totClass3D==0)){

   (*pr).condicao=0;//n tem condicao na folha
   (*pr).variavel=-1;//n tem variavel na folha, SL,SW,PL ou PW
   (*pr).direita=NULL;//ela é folha, portanto não tem filhos no lado direito
   (*pr).esquerda=NULL;//ela é folha, portanto não tem filhos no lado esquerdo

   //aqui é um jeito porco e que não tenho orgulho de ter feito para guardar o nome da classe da última variável da folha
   (*pr).flor[0]='I';(*pr).flor[1]='r';(*pr).flor[2]='i';(*pr).flor[3]='s';(*pr).flor[4]='-';
   (*pr).flor[5]='v';(*pr).flor[6]='i';(*pr).flor[7]='r';(*pr).flor[8]='g';(*pr).flor[9]='i';
   (*pr).flor[10]='n';(*pr).flor[11]='i';(*pr).flor[12]='c';(*pr).flor[13]='a';(*pr).flor[14]='\0';

   return;

  }
  //mesma lógica
  else if((totClass1E==0 && totClass2E!=0 && totClass3E==0) || (totClass1D==0 && totClass2D!=0 && totClass3D==0)){

   (*pr).condicao=0;
   (*pr).variavel=-1;
   (*pr).direita=NULL;
   (*pr).esquerda=NULL;

   (*pr).flor[0]='I';(*pr).flor[1]='r';(*pr).flor[2]='i';(*pr).flor[3]='s';(*pr).flor[4]='-';
   (*pr).flor[5]='s';(*pr).flor[6]='e';(*pr).flor[7]='t';(*pr).flor[8]='o';(*pr).flor[9]='s';
   (*pr).flor[10]='a';(*pr).flor[11]='\0';

   return;

  //mesma lógica
  }else if((totClass1E==0 && totClass2E==0 && totClass3E!=0) || (totClass1D==0 && totClass2D==0 && totClass3D!=0)){

   (*pr).condicao=0;
   (*pr).variavel=-1;
   (*pr).direita=NULL;
   (*pr).esquerda=NULL;

   (*pr).flor[0]='I';(*pr).flor[1]='r';(*pr).flor[2]='i';(*pr).flor[3]='s';(*pr).flor[4]='-';
   (*pr).flor[5]='v';(*pr).flor[6]='e';(*pr).flor[7]='r';(*pr).flor[8]='s';(*pr).flor[9]='i';
   (*pr).flor[10]='c';(*pr).flor[11]='o';(*pr).flor[12]='l';(*pr).flor[13]='o';(*pr).flor[14]='r';(*pr).flor[15]='\0';

   return;
  }
  
 
  //aqui termina a verificação

  //aqui tem uma chatice em c para tirar o zero dos log, conforme os calculos, pode ser que tenha uma classe de flor que nao tenha no sub-conjunto
  //se isso ocorrer, log vai ser zero, e esse é mais um jeito porco em c para resolver isso
  //aqui ocorre o calculo da entropia condicional para cada atributo ou variavel do conjunto de flores
 float a,b,c;

 a=log2(SL.pma1/SL.ma);
 b=log2(SL.pma2/SL.ma);
 c=log2(SL.pma3/SL.ma);

 if((SL.pma1/SL.ma) <=0){
  
  a=0; 
 
 }
 if((SL.pma2/SL.ma) <= 0){

  b=0;

 }
 if((SL.pma3/SL.ma) <= 0){

  c=0;

 }
 
 float H_x1_y_SL = -1*((SL.pma1/SL.ma)*a + (SL.pma2/SL.ma)*b + (SL.pma3/SL.ma)*c);
//--------------------------------------------------------------------------------------------------------
 a=log2(SL.pme1/SL.me);
 b=log2(SL.pme2/SL.me);
 c=log2(SL.pme3/SL.me);

 if((SL.pme1/SL.me) <=0){
  
  a=0; 
 
 }
 if((SL.pme2/SL.me) <= 0){

  b=0;

 }
 if((SL.pme3/SL.me) <= 0){

  c=0;

 }

  float H_x2_y_SL = -1*((SL.pme1/SL.me)*a + (SL.pme2/SL.me)*b + (SL.pme3/SL.me)*c);


//-------------------------------------------------------------------------------------------
 a=log2(SW.pma1/SW.ma);
 b=log2(SW.pma2/SW.ma);
 c=log2(SW.pma3/SW.ma);

 if((SW.pma1/SW.ma) <=0){
  
  a=0; 
 
 }
 if((SW.pma2/SW.ma) <= 0){

  b=0;

 }
 if((SW.pma3/SW.ma) <= 0){

  c=0;

 }

  float H_x1_y_SW = -1*((SW.pma1/SW.ma)*a + (SW.pma2/SW.ma)*b + (SW.pma3/SW.ma)*c);

  //------------------------------------------------------------------------------------------


 a=log2(SW.pme1/SW.me);
 b=log2(SW.pme2/SW.me);
 c=log2(SW.pme3/SW.me);

 if((SW.pme1/SW.me) <=0){
  
  a=0; 
 
 }
 if((SW.pme2/SW.me) <= 0){

  b=0;

 }
 if((SW.pme3/SW.me) <= 0){

  c=0;

 }

  float H_x2_y_SW = -1*((SW.pme1/SW.me)*a + (SW.pme2/SW.me)*b + (SW.pme3/SW.me)*c);
  //-----------------------------------------------------------------------------------

 
  a=log2(PL.pma1/PL.ma);
  b=log2(PL.pma2/PL.ma);
  c=log2(PL.pma3/PL.ma);

  if((PL.pma1/PL.ma) <=0){
  
   a=0; 
 
  }
  if((PL.pma2/PL.ma) <= 0){

   b=0;

  }
  if((PL.pma3/PL.ma) <= 0){

   c=0;

  }
 
  float H_x1_y_PL = -1*((PL.pma1/PL.ma)*a + (PL.pma2/PL.ma)*b + (PL.pma3/PL.ma)*c);
  //-----------------------------------------------------------------------------------------------
  

  a=log2(PL.pme1/PL.me);
  b=log2(PL.pme2/PL.me);
  c=log2(PL.pme3/PL.me);

  if((PL.pme1/PL.me) <=0){
  
   a=0; 
 
  }
  if((PL.pme2/PL.me) <= 0){

   b=0;

  }
  if((PL.pme3/PL.me) <= 0){

   c=0;

  }
 
  float H_x2_y_PL = -1*((PL.pme1/PL.me)*a + (PL.pme2/PL.me)*b + (PL.pme3/PL.me)*c);  

  //------------------------------------------------------------------------------------------------------

  a=log2(PW.pma1/PW.ma);
  b=log2(PW.pma2/PW.ma);
  c=log2(PW.pma3/PW.ma);

  if((PW.pma1/PW.ma) <=0){
  
   a=0; 
 
  }
  if((PW.pma2/PW.ma) <= 0){

   b=0;

  }
  if((PW.pma3/PW.ma) <= 0){

   c=0;

  }
  
  float H_x1_y_PW = -1*((PW.pma1/PW.ma)*a + (PW.pma2/PW.ma)*b + (PW.pma3/PW.ma)*c);

  //------------------------------------------------------------------------------------------------------


  a=log2(PW.pme1/PW.me);
  b=log2(PW.pme2/PW.me);
  c=log2(PW.pme3/PW.me);

  if((PW.pme1/PW.me) <=0){
  
   a=0; 
 
  }
  if((PW.pme2/PW.me) <= 0){

   b=0;

  }
  if((PW.pme3/PW.me) <= 0){

   c=0;

  }

  float H_x2_y_PW = -1*((PW.pme1/PW.me)*a + (PW.pme2/PW.me)*b + (PW.pme3/PW.me)*c); 

  //fim do calculo da entropia condicional
  //----------------------------------------------------------------------------------------------------------

  //calculo da entropia geral sobre cada variavel
  float HX_SL = (SL.ma/indice)*H_x1_y_SL + (SL.me/indice)*H_x2_y_SL;
  float HX_SW = (SW.ma/indice)*H_x1_y_SW + (SW.me/indice)*H_x2_y_SW;
  float HX_PL = (PL.ma/indice)*H_x1_y_PL + (PL.me/indice)*H_x2_y_PL;
  float HX_PW = (PW.ma/indice)*H_x1_y_PW + (PW.me/indice)*H_x2_y_PW;

  //calculo do ganho
  float IG_SL = entropia_global - HX_SL;
  float IG_SW = entropia_global - HX_SW;
  float IG_PL = entropia_global - HX_PL;
  float IG_PW = entropia_global - HX_PW;

  //aqui pego a variavel (SL,SW,PL ou PW) que tiveram o maior ganho de acordo com um limiar
  int var = maior(IG_SL,IG_SW,IG_PL,IG_PW,0.01);

  if(var != -1){//se foi maior que limiar a maior funcao ganho, então crio um no da arvore de decisao
        
	  //guardo a variavel da qual pertence ao nó, caso o maior ganho seja do SL, então ele é 1, portanto a condicão do no vai ser da variavel 1
	  ((*pr).variavel)=var;
	 
          //aqui verifico se é o primeiro no que esta na arovre, se for, então ele é a raiz
	  if((*tree).primeiro==NULL){

		 
		 (*pr).pai = NULL;//se é a raiz, não tem pai
		 (*tree).primeiro=pr;//o primeiro recebe o valor do ponteiro do primeiro no
		 (*tree).ultimo=pr;//assim como o ultimo, embora essa variavel nem foi utilizada
		   
	   }

          //se o maior ganho foi da variavel SL, na primeira coluna
	  if(var==1){
             //a condicao ira armazenar o valor da media de SL
	     ((*pr).condicao)=meanSL;
	     //aqui é o código para dividir a base de dados de acordo com a divisao das probabilidades
	     
	     //aqui eu aloco memoria para cada sub-conjunto da base que ira ser particionado
	     struct DATA *baseMaior = malloc(sizeof(struct DATA)*(SL.ma));
	     struct DATA *baseMenor = malloc(sizeof(struct DATA)*(SL.me));
	     
             //copio os dados de acordo com a media da variavel, a base maior significa os valores maiores que a media, e menor o contrário
	     copiarDados(baseMaior,treino,meanSL,'s',indice,var);
	     copiarDados(baseMenor,treino,meanSL,'n',indice,var);

             //aqui eu crio um no para os dois próximos filhos do no atual para cada base
	     struct NO *dir = malloc(sizeof(struct NO));
	     struct NO *esq = malloc(sizeof(struct NO));

             //irei setar como filho do nó atual a direita e esquerda que será o proximo nó da chamada recursiva
	     (*pr).direita = dir;
	     (*pr).esquerda = esq;

             //aqui eu seto o pai dos filhos do vertice atual como sendo ele
             dir->pai=pr;
             esq->pai=pr;
             //faço a chamada recursiva para cada subconjunto maior e menor que a media
	     ID3(baseMaior,SL.ma,entropia_global,esq);
	     ID3(baseMenor,SL.me,entropia_global,dir);


	  }
          //mesma lógica
	  else if(var==2){

	    ((*pr).condicao)=meanSW;

	    struct DATA *baseMaior = malloc(sizeof(struct DATA)*(SW.ma));
	    struct DATA *baseMenor = malloc(sizeof(struct DATA)*(SW.me));

	    copiarDados(baseMaior,treino,meanSW,'s',indice,var);
	    copiarDados(baseMenor,treino,meanSW,'n',indice,var);

	    struct NO *dir = malloc(sizeof(struct NO));
	    struct NO *esq = malloc(sizeof(struct NO));

	    (*pr).direita = dir;
	    (*pr).esquerda = esq;
            dir->pai=pr;
            esq->pai=pr;

	    ID3(baseMaior,SW.ma,entropia_global,esq);
	    ID3(baseMenor,SW.me,entropia_global,dir);

	  }
          //mesma lógica
	  else if(var==3){

	   ((*pr).condicao)=meanPL;

	   struct DATA *baseMaior = malloc(sizeof(struct DATA)*(PL.ma));
	   struct DATA *baseMenor = malloc(sizeof(struct DATA)*(PL.me));

	   copiarDados(baseMaior,treino,meanPL,'s',indice,var);
	   copiarDados(baseMenor,treino,meanPL,'n',indice,var);

	   struct NO *dir = malloc(sizeof(struct NO));
	   struct NO *esq = malloc(sizeof(struct NO));

	   (*pr).direita = dir;
	   (*pr).esquerda = esq;
           dir->pai=pr;
           esq->pai=pr;

	   ID3(baseMaior,PL.ma,entropia_global,esq);
	   ID3(baseMenor,PL.me,entropia_global,dir);

	  }
          //mesma lógica
	  else if(var==4){

	   ((*pr).condicao)=meanPW;

	   struct DATA *baseMaior = malloc(sizeof(struct DATA)*(PW.ma));
	   struct DATA *baseMenor = malloc(sizeof(struct DATA)*(PW.me));

	   copiarDados(baseMaior,treino,meanPW,'s',indice,var);
	   copiarDados(baseMenor,treino,meanPW,'n',indice,var);

	   struct NO *dir = malloc(sizeof(struct NO));
	   struct NO *esq = malloc(sizeof(struct NO));

	   (*pr).direita = dir;
	   (*pr).esquerda = esq;
           dir->pai=pr;
           esq->pai=pr;

	   ID3(baseMaior,PW.ma,entropia_global,esq);
	   ID3(baseMenor,PW.me,entropia_global,dir);

	  }
   //caso a maior função ganho seja menor que o limiar, eu pego a classe que tem o maior numero de atributos, e deixo ela como folha
   }else{
       
       int tot1 = totClass1E + totClass1D;
       int tot2 = totClass2E + totClass2D;    
       int tot3 = totClass3E + totClass3D;

       ((*pr).condicao)=0;
       (*pr).direita = NULL;
       (*pr).esquerda = NULL;   

       if(tot1>=tot2 && tot1 >= tot3){

         (*pr).flor[0]='I';(*pr).flor[1]='r';(*pr).flor[2]='i';(*pr).flor[3]='s';(*pr).flor[4]='-';
         (*pr).flor[5]='v';(*pr).flor[6]='i';(*pr).flor[7]='r';(*pr).flor[8]='g';(*pr).flor[9]='i';
         (*pr).flor[10]='n';(*pr).flor[11]='i';(*pr).flor[12]='c';(*pr).flor[13]='a';(*pr).flor[14]='\0';

        
         
       }
       else if(tot2>=tot1 && tot2>=tot3){

         (*pr).flor[0]='I';(*pr).flor[1]='r';(*pr).flor[2]='i';(*pr).flor[3]='s';(*pr).flor[4]='-';
         (*pr).flor[5]='s';(*pr).flor[6]='e';(*pr).flor[7]='t';(*pr).flor[8]='o';(*pr).flor[9]='s';
         (*pr).flor[10]='a';(*pr).flor[11]='\0';

       }
       else if(tot3>=tot1 && tot3>=tot2){

         (*pr).flor[0]='I';(*pr).flor[1]='r';(*pr).flor[2]='i';(*pr).flor[3]='s';(*pr).flor[4]='-';
         (*pr).flor[5]='v';(*pr).flor[6]='e';(*pr).flor[7]='r';(*pr).flor[8]='s';(*pr).flor[9]='i';
         (*pr).flor[10]='c';(*pr).flor[11]='o';(*pr).flor[12]='l';(*pr).flor[13]='o';(*pr).flor[14]='r';(*pr).flor[15]='\0';

       }

   }
}

//função que mostra a arvore criada, modo de busca foi infixa
void mostrarArvoreDecisao(struct NO *n){
 
 
 if(n->pai == NULL){
 
    printf("Raiz: media -> %f valor da coluna ->(1=SL,2=SW,3=PL,4=PW) %i\n",n->condicao,n->variavel);

 }

 if(n->esquerda != NULL){

    if(n->pai != NULL)
      printf("esquerda: media -> %f valor da coluna ->(1=SL,2=SW,3=PL,4=PW) %i\n",n->condicao,n->variavel);
    
    mostrarArvoreDecisao(n->esquerda);    

 }
 if(n->direita != NULL){
  
    if(n->pai != NULL)
      printf("direita: media -> %f valor da coluna ->(1=SL,2=SW,3=PL,4=PW) %i\n",n->condicao,n->variavel);
    
    mostrarArvoreDecisao(n->direita); 

 }

 if(n->esquerda == NULL && n->direita == NULL){

    printf("folha: %s\n",n->flor);

  }

}
//função que valida a base de teste dado que a arvore de decisão já foi criada
void validarTeste(struct DATA *teste, struct NO *mo, int tam){

 int cont=0;

 printf("Dados correspodentes a uma porcentagem da base de dados para serem testados\n");

 //aqui é o tamanho da base de teste, eu intero sobre ela
 for(int i=0;i<tam;i=i+1){

   struct NO *no=mo;//variavel auxiliar para eu não perder o endereço
   
   //enquanto eu não chegar em um nó folha, eu continuo varrendo
   while(no->esquerda != NULL || no->direita != NULL){

       if(no->variavel == 1){//se o primeiro no da minha arvore de decisao é 1

          if(teste[i].SL >= no->condicao){//verifico se a variavel 1 da minha base de teste é maior ou igual a condicao nesse no
 
             no=no->esquerda;//se for maior então segue para esquerda
             
          }
          else{

            no=no->direita;//caso contrário para direita

          }

       }else if(no->variavel == 2){//mesma lógica

          if(teste[i].SW >= no->condicao){
 
             no=no->esquerda;
             
          }
          else{

            no=no->direita;

          }

       }else if(no->variavel == 3){//mesma lógica


         if(teste[i].PL >= no->condicao){
 
             no=no->esquerda;
             
          }
          else{

            no=no->direita;

          }

       }else if(no->variavel == 4){//mesma lógica

         if(teste[i].PW >= no->condicao){
 
             no=no->esquerda;
             
          }
          else{

            no=no->direita;

          }

       }
   }

   if(strcmp(teste[i].class,no->flor)==0)//comparo para ver se a flor encontrada foi a esperada
    cont=cont+1;
   
   printf("dados de teste[%i]: %s resposta da arvore: %s\n",i,teste[i].class,no->flor);//mostro o que foi encontrado

 }

 //calculo da porcentagem de acerto
 float totalTest=tam;
 float numAcertos=cont;

 printf("\nPorcentagem de acertos: %.2f\n",(numAcertos*100)/totalTest);//mostro o resultado do calculo da porcentagem

}

int main(void){

      
   //abro o arquivo em txt. Note que o arquivo estava em formato .data, eu fiz um script em python para embaralhar os dados e salvar como txt para eu
   //abrir em c, eu sei que é coisa de idiota, mas quis fazer em C pelo maior desafio.
   FILE *in_file  = fopen("iris.txt", "r"); 
   //apenas para alocar o tamanho do arquivo
   int tamanho=fileSize(in_file);//calculo o tamanho do arquivo
   
   FILE *file  = fopen("iris.txt", "r");//leio de novo para não perde a referencia, ponteiro é chato de mexer
   struct DATA *data = malloc(sizeof(struct DATA)*tamanho);//aloco

   int cont=0;
   while(fscanf(file,"%f %f %f %f %s",&data[cont].SL,&data[cont].SW,&data[cont].PL,&data[cont].PW,data[cont].class) != EOF){//contabilizo o tamanho

     cont=cont+1;
   }
   
   
   //aqui é criado um dataset de treino e teste

   int indice = tamanho*0.8;//aqui pegaremos 80% dos dados para treino e 20% para treino;  

   struct DATA *treino = malloc(sizeof(struct DATA)*indice);//80 % da base total
   struct DATA *teste = malloc(sizeof(struct DATA)*(tamanho-indice));//20% da base total

   for(int i=0;i<indice;i=i+1){//0 até indice
     treino[i] = data[i];
   }
   
   for(int i=indice;i<tamanho;i=i+1){//indice até o tamanho total da base
     teste[i-indice] = data[i];
   }

   double p = 1.0/3.0;
   float h_x = -1*(p*log2(p) + p*log2(p) + p*log2(p));//entropia dos dados, pegando as três classes possíveis 1/3
   //aqui acontecera a recursão do algoritmo
   tree=malloc(sizeof(struct DECISION_TREE));
   (*tree).primeiro=NULL;
   (*tree).ultimo=NULL;
  
   struct NO *pr = malloc(sizeof(struct NO));//aloco o tamanho do nó inicial   

   ID3(treino,indice,h_x,pr);//faço a magica
   
   printf("Metodo de print infixa\n");
   printf("------------------------\n\n");
   mostrarArvoreDecisao((*tree).primeiro);//printa tamanho da arvore
   printf("-------------------------------------------------------\n\n");

   validarTeste(teste,(*tree).primeiro,(tamanho-indice));//valida os teste

   //programa foi inteiramente testado e feito em linux, portanto se houver problemas com ponteiros no windows, favor entrar en contato comigo: joao_junior174@outlook.com

  /*
  descomenta se não estiver no linux
  system("pause");
  return 0; 
  */

}
