#include "../include/comunicacion_memoria.h"
//#include "../include/cpu.h"
//#include "../include/funciones_cpu.h"


static void procesar_conexion_memoria_kernel(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_kernel = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

    fd_kernel = cliente_socket;

    send_TAM(cliente_socket,tamanio_paginas);

    send_TAM(cliente_socket,cant_entradas_por_tabla);


    while (cliente_socket != -1) {
    	//log_trace(log_memoria,"Estoy dentro del while cliente socket");

    		uint32_t condicion;
    		//log_trace(log_memoria,"la condicion antes del recv es: %d",condicion);
    	    if (recv(cliente_socket, &condicion, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
    		log_info(log_memoria, "DISCONNECT!");
    		return;
    	    }

    	    if(condicion == 666){
    	    	uint32_t tam_proceso;
				//log_trace(log_memoria,"la condicion antes del recv es: %d",tam_proceso);
				if (recv(cliente_socket, &tam_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "DISCONNECT!");
				return;
				}

				log_debug(log_memoria,"El tam del proceso recbido por kernel es: %d",tam_proceso);

			 //lo vamos a  hacer con tam = 512 para que tenga que ocupar 2 tablas de 2do nivel

			 t_list* lista_paginas_del_proceso; //todo liberar, ESTA LISTA tiene la cantidad de paginas totales del proceso
			 lista_paginas_del_proceso = dividir_proceso_en_paginas(tam_proceso);

			 log_debug(log_memoria,"El size de la lista de paginas del proceso es: %d",list_size(lista_paginas_del_proceso));

			 t_list* tabla_1er_nivel_del_proceso;
			 tabla_1er_nivel_del_proceso = colocar_paginas_en_tabla(lista_paginas_del_proceso);


			 uint32_t indice_tabla = asignar_tabla_1er_nivel_a_proceso(tabla_1er_nivel_del_proceso);

			 log_debug(log_memoria,"El size de la tabla de primer nivel es: %d",list_size(tabla_1er_nivel_del_proceso));


			 if(indice_tabla == -1){
				 log_error(log_memoria,"No hay mas entradas para tablas de primer nivel");
			 }else{
				 send_indice_a_kernel(fd_kernel,indice_tabla);
				 //log_trace(log_memoria,"envie el inice a kernel %d", indice_tabla);
			 }

			char* path_char = pasar_a_char(indice_tabla);

			//log_debug(log_memoria,"El path  char es: %s",path_char);

			crear_archivo(path_char);

			char *ruta_archivo = string_new();
				string_append_with_format(&ruta_archivo,"%s/",path_swap);
				string_append(&ruta_archivo, path_char);

			poner_archivo_con_ceros(ruta_archivo,tam_proceso);

			free(ruta_archivo);

    	    }

    		if(condicion == TLB_RD){
    			//log_warning(log_memoria, "Voy a recibir de TLB READ ");

    			//send_TAM(cliente_socket,TLB_CPY);
    ////
    			uint32_t encontro_en_tabla;
				if (recv(cliente_socket, &encontro_en_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_info(log_memoria, "fallo al recibir si lo encontro o no!");
					return;
				}

				if(encontro_en_tabla == 1234){

					uint32_t marco_aux;
					if (recv(cliente_socket, &marco_aux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El marco es: %d",marco_aux);


					uint32_t desplazamiento;
					if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);


					uint32_t tamanio;
					if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El desplazamienmto es: %d",tamanio);



					uint32_t valor_leido = leer_de_memoria(marco_aux,desplazamiento);
					actualizar_bit_uso_tlb(marco_aux);

					send_TAM(cliente_socket,valor_leido);
					//log_warning(log_memoria,"Le mande un 18 a cpu (vino por TLB)");




				}else{
							uint32_t indice_tabla;
							if (recv(cliente_socket, &indice_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"El indice de la tabla 1er nivel es: %d",indice_tabla);

							uint32_t entrada_1er_nivel;
							if (recv(cliente_socket, &entrada_1er_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"La entrada de 1er nivel es: %d",entrada_1er_nivel);

							uint32_t tamanio;
							if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"El tamanio del proceso: %d",tamanio);


							usleep(retardo_memoria*1000);



							uint32_t indice_tabla_segundo_nivel = obtener_nro_tabla_2do_nivel(indice_tabla,entrada_1er_nivel);
							send_TAM(cliente_socket,indice_tabla_segundo_nivel);
							log_trace(log_memoria,"El indice de la tabla 2do nivel es: %d",indice_tabla_segundo_nivel);

							uint32_t entrada_2do_nivel;
							if (recv(cliente_socket, &entrada_2do_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"La entrada de 2do nivel es: %d",entrada_2do_nivel);

							pagina* pagina_buscada;
							pagina_buscada = buscar_pagina_en_tabla_2do_nivel(indice_tabla_segundo_nivel,entrada_2do_nivel);
//LO COMENTO ACA Y LO AGREGO ABAJO//send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
							//log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
							//free(pagina_buscada);

							uint32_t desplazamiento;
							if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);
//a partir de aca copiar par aweite
							uint32_t valor_leido;
							if(pagina_buscada->bit_presencia == 1){
								log_info(log_memoria,"La pagina buscada  es: %d",pagina_buscada->nro_pagina);

								valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
								pagina_buscada->bit_uso = 1;
								usleep(retardo_memoria*1000);
								send_TAM(cliente_socket,valor_leido);
							//	log_warning(log_memoria,"Le mande un 18 a cpu(La pagina ya estaba en memoria)");
								usleep(retardo_memoria*1000);
								send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
								log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
							}
							else{
								//pthread_mutex_lock(&mutexListaFrame);
								uint32_t frame_a_utilizar = buscar_frame_libre();
								//pthread_mutex_unlock(&mutexListaFrame);

								if(frame_a_utilizar != -1){
									if(al_proceso_le_quedan_frames(indice_tabla)){
									//	log_error(log_memoria,"Entre ya que al proceso le quedan frames");

										//poner_pagina_en_marco(frame_a_utilizar,pagina_buscada,indice_tabla);
										log_info(log_memoria,"La pagina buscada  es: %d",pagina_buscada->nro_pagina);
										traer_proceso_de_swap(indice_tabla,tamanio,pagina_buscada,frame_a_utilizar);
										valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
										pagina_buscada->bit_uso = 1;
										usleep(retardo_memoria*1000);
										send_TAM(cliente_socket,valor_leido);
										//log_warning(log_memoria,"Le mande un 18 a cpu (al proceso le quedaban frames)");
										usleep(retardo_memoria*1000);
										send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
										log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
									}
									else{
										if(el_proceso_tiene_almenos_una_pag_en_mem(indice_tabla)){
										//	log_error(log_memoria,"Entre ya que el proceso tiene al menos un pag en memoria");
											//log_info(log_memoria,"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
										//	log_info(log_memoria,"El indice tabla que voy a pasar por parametro es : %d",indice_tabla);
											//t_list* contenido_de_pagina = list_create();
											log_info(log_memoria,"La pagina buscada  es: %d",pagina_buscada->nro_pagina);
											t_list* contenido_de_pagina = traer_pagina_de_swap(indice_tabla,pagina_buscada->nro_pagina,tamanio);
											//pthread_mutex_lock(&mutexSwap);
											ejecutar_reemplazo(contenido_de_pagina,pagina_buscada,indice_tabla);
											//pthread_mutex_unlock(&mutexSwap);
											list_destroy(contenido_de_pagina);
											valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
											pagina_buscada->bit_uso = 1;
											usleep(retardo_memoria*1000);
											send_TAM(cliente_socket,valor_leido);
										//	log_warning(log_memoria,"Le mande un 18 a cpu (tuvo que reemplazar)");
											usleep(retardo_memoria*1000);
											send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
											log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
									}
							   }
						   }

					}
				}

/////

    		}
    		if(condicion == TLB_CPY){
    			//log_warning(log_memoria, "Voy a recibir de TLB CPY ");

				//send_TAM(cliente_socket,TLB_CPY);
	////
				uint32_t encontro_en_tabla;
				if (recv(cliente_socket, &encontro_en_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_info(log_memoria, "fallo al recibir si lo encontro o no!");
					return;
				}

				if(encontro_en_tabla == 1234){

					uint32_t marco_aux;
					if (recv(cliente_socket, &marco_aux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El marco es: %d",marco_aux);


					uint32_t desplazamiento;
					if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);

					uint32_t tamanio;
					if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El desplazamienmto es: %d",tamanio);


					actualizar_bit_uso_tlb(marco_aux);
					actualizar_bit_modif_tlb(marco_aux);

					send_TAM(cliente_socket,18);
					//log_warning(log_memoria,"Le mande un 18 a cpu (vino por TLB)");





					}else{
					uint32_t indice_tabla;
					if (recv(cliente_socket, &indice_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El indice de la tabla 1er nivel es: %d",indice_tabla);

					uint32_t entrada_1er_nivel;
					if (recv(cliente_socket, &entrada_1er_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"La entrada de 1er nivel es: %d",entrada_1er_nivel);

					uint32_t tamanio;
					if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El tamanio del proceso: %d",tamanio);


					uint32_t indice_tabla_segundo_nivel = obtener_nro_tabla_2do_nivel(indice_tabla,entrada_1er_nivel);
					usleep(retardo_memoria*1000);
					send_TAM(cliente_socket,indice_tabla_segundo_nivel);
					log_trace(log_memoria,"El indice de la tabla 2do nivel es: %d",indice_tabla_segundo_nivel);

					uint32_t entrada_2do_nivel;
					if (recv(cliente_socket, &entrada_2do_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"La entrada de 2do nivel es: %d",entrada_2do_nivel);

					pagina* pagina_buscada;
					pagina_buscada = buscar_pagina_en_tabla_2do_nivel(indice_tabla_segundo_nivel,entrada_2do_nivel);

					uint32_t desplazamiento;
					if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);



					if(pagina_buscada->bit_presencia == 1){

						//valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
						pagina_buscada->bit_uso = 1;
						pagina_buscada->bit_modificado = 1;
						send_TAM(cliente_socket,18);
						//log_warning(log_memoria,"Le mande un 18 a cpu(La pagina ya estaba en memoria)");

						send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
						log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
					}
					else{
							//pthread_mutex_lock(&mutexListaFrame);
						uint32_t frame_a_utilizar = buscar_frame_libre();
							//pthread_mutex_unlock(&mutexListaFrame);

						if(frame_a_utilizar != -1){
							if(al_proceso_le_quedan_frames(indice_tabla)){
								//log_error(log_memoria,"Entre ya que el proceso tiene al menos un pag en memoria");
								traer_proceso_de_swap(indice_tabla,tamanio,pagina_buscada,frame_a_utilizar);
								//poner_pagina_en_marco(frame_a_utilizar,pagina_buscada,indice_tabla);
								//valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
								pagina_buscada->bit_uso = 1;
								pagina_buscada->bit_modificado = 1;
								send_TAM(cliente_socket,18);
								//log_warning(log_memoria,"Le mande un 18 a cpu (al proceso le quedaban frames)");
								usleep(retardo_memoria*1000);
								send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
								log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
							}
							else{
								if(el_proceso_tiene_almenos_una_pag_en_mem(indice_tabla)){
								//	log_error(log_memoria,"Entre ya que el proceso tiene al menos un pag en memoria");
									t_list* contenido_de_pagina = traer_pagina_de_swap(indice_tabla,pagina_buscada->nro_pagina,tamanio);
									//pthread_mutex_lock(&mutexSwap);
									ejecutar_reemplazo(contenido_de_pagina,pagina_buscada,indice_tabla);
									//pthread_mutex_unlock(&mutexSwap);
									list_destroy(contenido_de_pagina);
								//	valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
									pagina_buscada->bit_uso = 1;
									pagina_buscada->bit_modificado = 1;
									send_TAM(cliente_socket,18);
								//	log_warning(log_memoria,"Le mande un 18 a cpu (tuvo que reemplazar)");
									usleep(retardo_memoria*1000);
									send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
									log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
							}
					   }
				   }

			}


	}




    		}
    		if(condicion == TLB_WR){
					//log_warning(log_memoria, "Voy a recibir de TLB WRITE ");

					//send_TAM(cliente_socket,TLB_CPY);
		////
					uint32_t encontro_en_tabla;
					if (recv(cliente_socket, &encontro_en_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir si lo encontro o no!");
						return;
					}

					if(encontro_en_tabla == 1234){

						uint32_t marco_aux;
						if (recv(cliente_socket, &marco_aux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
						}
						log_trace(log_memoria,"El marco es: %d",marco_aux);


						uint32_t desplazamiento;
						if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
						}
						log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);

						uint32_t tamanio;
						if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
						}
						log_trace(log_memoria,"El tamanio: %d",tamanio);

						uint32_t valor;
							if (recv(cliente_socket, &valor, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
						}
						log_trace(log_memoria,"El valor es: %d",valor);

//						escribir_memoria_vino_de_tlb(marco_aux,desplazamiento,valor);
//
//						send_TAM(cliente_socket,valor);
//						//log_warning(log_memoria,"Le mande un 18 a cpu (vino por TLB)");

						//uint32_t valor_leido = leer_de_memoria(marco_aux,desplazamiento);

						escribir_pagina(valor,marco_aux,desplazamiento);

						actualizar_bit_uso_tlb(marco_aux);
						actualizar_bit_modif_tlb(marco_aux);

						send_TAM(cliente_socket,100); // el 100 es por que se escribio correctamente
						//log_warning(log_memoria,"Le mande un 18 a cpu (vino por TLB)");


					}else{
							uint32_t indice_tabla;
							if (recv(cliente_socket, &indice_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"El indice de la tabla 1er nivel es: %d",indice_tabla);

							uint32_t entrada_1er_nivel;
							if (recv(cliente_socket, &entrada_1er_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"La entrada de 1er nivel es: %d",entrada_1er_nivel);


							uint32_t tamanio;
							if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"El tamanio del proceso: %d",tamanio);




							uint32_t indice_tabla_segundo_nivel = obtener_nro_tabla_2do_nivel(indice_tabla,entrada_1er_nivel);
							usleep(retardo_memoria*1000);
							send_TAM(cliente_socket,indice_tabla_segundo_nivel);
							log_trace(log_memoria,"El indice de la tabla 2do nivel es: %d",indice_tabla_segundo_nivel);

							uint32_t entrada_2do_nivel;
							if (recv(cliente_socket, &entrada_2do_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"La entrada de 2do nivel es: %d",entrada_2do_nivel);

							pagina* pagina_buscada;
							pagina_buscada = buscar_pagina_en_tabla_2do_nivel(indice_tabla_segundo_nivel,entrada_2do_nivel);
//							send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
//							log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
							//free(pagina_buscada);

							uint32_t desplazamiento;
							if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);

							uint32_t valor;
							if (recv(cliente_socket, &valor, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
							}
							log_trace(log_memoria,"El valor es: %d",valor);


							if(pagina_buscada->bit_presencia == 1){

								//valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
								//log_info(log_memoria,"La pagina en la que voy a escribir con la instruccion que me dieron es: %d",pagina_buscada->nro_pagina);

								escribir_pagina(valor,pagina_buscada->frame,desplazamiento);
								pagina_buscada->bit_uso = 1;
								pagina_buscada->bit_modificado = 1;
								send_TAM(cliente_socket,100);
								//log_warning(log_memoria,"Le mande un 18 a cpu(La pagina ya estaba en memoria)");
								usleep(retardo_memoria*1000);
								send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
								log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
							}
							else{
									//pthread_mutex_lock(&mutexListaFrame);
								uint32_t frame_a_utilizar = buscar_frame_libre();
								//	pthread_mutex_unlock(&mutexListaFrame);

								if(frame_a_utilizar != -1){
									if(al_proceso_le_quedan_frames(indice_tabla)){
								//		log_error(log_memoria,"Entre ya que al proceso le quedan frames");
									//	log_info(log_memoria,"La pagina en la que voy a escribir con la instruccion que me dieron es: %d",pagina_buscada->nro_pagina);
										traer_proceso_de_swap(indice_tabla,tamanio,pagina_buscada,frame_a_utilizar);
										//poner_pagina_en_marco(frame_a_utilizar,pagina_buscada,indice_tabla);
										//valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
										escribir_pagina(valor,pagina_buscada->frame,desplazamiento);
										pagina_buscada->bit_uso = 1;
										pagina_buscada->bit_modificado = 1;
										send_TAM(cliente_socket,100);
										//log_warning(log_memoria,"Le mande un 18 a cpu (al proceso le quedaban frames)");
										usleep(retardo_memoria*1000);
										send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
										log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
									}
									else{
										if(el_proceso_tiene_almenos_una_pag_en_mem(indice_tabla)){
										//	log_error(log_memoria,"Entre ya que el proceso tiene al menos un pag en memoria");
											t_list* contenido_de_pagina = traer_pagina_de_swap(indice_tabla,pagina_buscada->nro_pagina,tamanio);
											//pthread_mutex_lock(&mutexSwap);
											ejecutar_reemplazo(contenido_de_pagina,pagina_buscada,indice_tabla);
											//pthread_mutex_unlock(&mutexSwap);
											list_destroy(contenido_de_pagina);
											//log_info(log_memoria,"La pagina en la que voy a escribir con la instruccion que me dieron es: %d",pagina_buscada->nro_pagina);

											escribir_pagina(valor,pagina_buscada->frame,desplazamiento);
											pagina_buscada->bit_uso = 1;
											pagina_buscada->bit_modificado = 1;
											send_TAM(cliente_socket,100);
											//log_warning(log_memoria,"Le mande un 18 a cpu (tuvo que reemplazar)");
											usleep(retardo_memoria*1000);
											send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
											log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
									}
							   }
						   }

					}

				}
		}
	/////

    		if(condicion == METER_A_SWAP){
    			uint32_t indice_proceso;
				if (recv(cliente_socket, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir nro de pagina!");
				return;
				}
				log_trace(log_memoria,"Meto a swap al proceso es: %d",indice_proceso);

				pasar_proceso_a_swap(indice_proceso);

				send_TAM(cliente_socket,1773);//esto lo mando a kernel para avisarle que ya lo meti a swap
    		}

    		if(condicion == METER_EN_MEM_PRINCIPAL){
			uint32_t indice_proceso;
			if (recv(cliente_socket, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
			log_info(log_memoria, "fallo al recibir nro de pagina!");
			return;
			}

			log_trace(log_memoria,"Meto a mem principal el proceso: %d",indice_proceso);


			if(!el_proceso_tiene_almenos_una_pag_en_mem(indice_proceso)){
				//log_error(log_memoria,"El proceso es nuevo asi que lo meto de una a mem ppal");
				//poner_proceso_en_mem_ppal(indice_proceso);
			}else{
				//log_error(log_memoria,"El proceso no es nuevo asi que ya lo trajo swap");
			}

			send_TAM(cliente_socket,5555); //le aviso a kernel que ya cargue el proceso

			}

      		if(condicion == SACAR_DE_SWAP){
        			uint32_t indice_proceso;
    				if (recv(cliente_socket, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
    				log_info(log_memoria, "fallo al recibir nro de pagina!");
    				return;
    				}
    				log_trace(log_memoria,"Saco de swap el proceso: %d",indice_proceso);

    				//traer_proceso_de_swap(indice_proceso); // LO COMENTAMOS POR QUE NO HACE FALTA; LAS PAGINAS SE CARGAN A Demanda

    				send_TAM(cliente_socket,1773);//esto lo mando a kernel para avisarle que ya lo meti a swap
        		}
      		if(condicion == SACAR_PROCESO_DE_MEMORIA){
				uint32_t indice_proceso;
				if (recv(cliente_socket, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir nro de pagina!");
				return;
				}
				log_trace(log_memoria,"Saco de memoria el proceso: %d",indice_proceso);


				sacar_proceso_de_memoria(indice_proceso);
				send_TAM(cliente_socket,1773);//esto lo mando a cpu para avisarle que ya lo meti a swap
			}

      		if(condicion == COPIAR_PAGINAS){
				uint32_t marco_origen;
				if (recv(cliente_socket, &marco_origen, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir marco_origen!");
				return;
				}
				log_trace(log_memoria,"El marco origen a cambiar es: %d",marco_origen );


				uint32_t desplazamiento_origen;
				if (recv(cliente_socket, &desplazamiento_origen, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir desplazamiento_origen!");
				return;
				}
				log_trace(log_memoria,"El desplazamiento origen a cambiar es: %d",desplazamiento_origen );

				uint32_t marco_destino;
				if (recv(cliente_socket, &marco_destino, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir marco_destino!");
				return;
				}
				log_trace(log_memoria,"El marco destino a cambiar es: %d",marco_destino );


				uint32_t desplazamiento_destino;
				if (recv(cliente_socket, &desplazamiento_destino, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir desplazamiento_destino!");
				return;
				}
				log_trace(log_memoria,"El desplazamiento destino a cambiar es: %d",desplazamiento_destino );

				uint32_t tamanio;
				if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_info(log_memoria, "fallo al recibir nro de pagina!");
					return;
				}


				copiar_en_memoria(marco_origen,desplazamiento_origen,marco_destino,desplazamiento_destino);

      		}







    }

    log_warning(log_memoria, "El cliente se desconecto de %s server", server_name);


    return;
}



static void procesar_conexion_memoria_cpu(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_kernel = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

   // fd_kernel = 7;
    fd_cpu = 5;
   // send_TAM(fd_cpu,tamanio_paginas);
    bool ya_lo_envie = false;

    log_error(log_memoria,"El cliente socket en procesar cpu es: %d",cliente_socket);

    /*if(!ya_lo_envie){
    	send_TAM(fd_cpu,tamanio_paginas);

    	send_TAM(fd_cpu,cant_entradas_por_tabla);
    }

    ya_lo_envie = true;
*/

   /* while (cliente_socket != -1) {

    	uint32_t condicion;
		if (recv(fd_cpu, &condicion, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
		log_info(log_memoria, "No recibi la condicion!");
		return;
		}

		if(condicion == TLB_CPY){
			log_warning(log_memoria, "Voy a recibir de TLB CPY en condicion cpu");
		}
    }
*/
    log_warning(log_memoria, "El cliente se desconecto de %s server", server_name);


    return;
}



int server_escuchar_memoria(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(logger, server_name, server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        pthread_t hilo2;

        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;


        t_procesar_conexion_args* args2 = malloc(sizeof(t_procesar_conexion_args));
                args2->log = logger;
                args2->fd = cliente_socket;
                args2->server_name = server_name;


       // log_error(log_kernel,"Estoy en sv escuchar antes de procesar conexion");
        /* pthread_create(&hilo2, NULL, (void*) procesar_conexion_memoria_cpu, (void*) args2);
         pthread_detach(hilo2);*/

        pthread_create(&hilo, NULL, (void*) procesar_conexion_memoria_kernel, (void*) args);
        pthread_detach(hilo);


       // log_error(log_kernel,"Estoy en sv escuchar despues de procesar conexion");
        return 1;
    }
    return 0;
}
