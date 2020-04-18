# Algoritmo-ID3-em-C
Arvore de decisão construída com o algoritmo ID3 para a base de dados Iris

O algoritmo construído aqui segui as explicações no site http://commanber.com/2017/05/21/decision-tree/. O código foi feito inteiramente em C puro, então precisará de um compilador gcc para rodar. Há ponteiros sem desalocação, deixei por conta do sistema, então se ocorrer algum problema com windows, por favor entre em contato comigo. 
A entrada para esse código é uma base de dados em txt, que foi organizada e embaralhada em python, depois foi lida com o C
Não fiz em python para deixa o código com um bom desempenho, apenas organizei os dados por ele. Então para testar com uma base maior ou menor se quiser, use o código em python para organizar os dados, ou deixe no formato que estara aqui para teste.
O algoritmo usa 80% da base Iris e 20% para teste, sendo a base composta por 150 amostras. O algoritmo printa a arvore de decisão, parece meio confusa, mas segue o modo infixo de navegação.
Logo depois, mostra os resultados comparando-se a base de teste ao que foi encontrado na arvore. 
