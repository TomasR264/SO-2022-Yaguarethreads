#include "../include/memoria.h"



#include <stdio.h>
#include <stdlib.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<commons/string.h>
#include "utils.h"
#include <math.h>



uint32_t asignar_tabla_1er_nivel_a_proceso(){ // Devuelve el numero de tabla que se le asiga al proceso, o -1 si no hay mas lugar para entradas
	t_list* nueva_entrada = list_create();
	uint32_t a;
	if(entradas_por_tabla >= list_size(lista_tablas_1er_nivel)){
		 a = list_add(lista_tablas_1er_nivel,nueva_entrada);
	}
	else{
		a = -1;
	}
	list_clean_and_destroy_elements(nueva_entrada,free);
	list_destroy(nueva_entrada);
	return a;
}

uint32_t obtener_nro_tabla_2do_nivel(uint32_t numero_tabla_1er_nivel,uint32_t entrada_primer_nivel){
	t_list* tabla_1er_nivel_buscada = list_create();
	tabla_1er_nivel_buscada = list_get(lista_tablas_1er_nivel,numero_tabla_1er_nivel);
	entrada_1er_nivel* entrada_buscada = malloc(sizeof(entrada_primer_nivel));
	entrada_buscada = list_get(tabla_1er_nivel_buscada,entrada_primer_nivel);
	uint32_t resultado = entrada_buscada->nro_tabla_2do_nivel;
	free(entrada_buscada);
	list_clean_and_destroy_elements(tabla_1er_nivel_buscada,free);
	list_destroy(tabla_1er_nivel_buscada);
	return resultado;
}


pagina* buscar_pagina_en_tabla_2do_nivel(uint32_t nro_tabla_2do_nivel,uint32_t nro_entrada){ // Busca la pagina en la tabla de 2do nivel, si no esta, devuelve null
	t_list* tabla_2do_nivel_buscada = list_create();
	tabla_2do_nivel_buscada = list_get(lista_tablas_2do_nivel,nro_tabla_2do_nivel);
	pagina* pagina_buscada = list_get(tabla_2do_nivel_buscada,nro_entrada);
	list_clean_and_destroy_elements(tabla_2do_nivel_buscada,free);
	list_destroy(tabla_2do_nivel_buscada);
	return pagina_buscada;
}



