Introdução 

Este trabalho apresenta o desenvolvimento e a avaliação de um sistema de monitoramento e controle de esteiras transportadoras para a indústria alimentícia. O projeto foi desenvolvido em duas plataformas: Windows e Linux, utilizando duas formas de comunicação interprocessos (IPC): pipe nomeado e multithreading. O objetivo é comparar o desempenho das diferentes plataformas e métodos de IPC em termos de tempo de execução. 

Contexto 

Uma Indústria de Alimentos de Santa Catarina chamada FoodSec S.A. possui a tarefa de escanear alimentos por meio de câmeras e verificar se os mesmos estão corretos. Os alimentos podem passar por uma das duas esteiras disponíveis. As duas esteiras são controladas por um por um único computador centralizado. Esse computador recebe dados de um sensor em cada uma das esteiras que captura a contagem de itens que são identificados como seguros. A contagem é exibida em um display perto das esteiras (todos os displays são controlados pela mesma função, é apenas uma replicação). 
